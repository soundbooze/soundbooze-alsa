/*
 * Copyright (c) 2019 SoundBooze <soundbooze@gmail.com>
 *                            _ _                        
 *  ___  ___  _   _ _ __   __| | |__   ___   ___ _______ 
 * / __|/ _ \| | | | '_ \ / _` | '_ \ / _ \ / _ \_  / _ \
 * \__ \ (_) | |_| | | | | (_| | |_) | (_) | (_) / /  __/
 * |___/\___/ \__,_|_| |_|\__,_|_.__/ \___/ \___/___\___|
 *                                                     
 * UDUK™ Free as an AIR License
 * 
 */

#include <signal.h>
#include <pthread.h>

#include "alsa.h"
#include "pcm.h"

pthread_t live_thread;

struct AlsaCapture c;
struct AlsaPlay p;

void 
exit_sighandler (int signum) {
  pthread_cancel(live_thread);

  snd_pcm_drain(c.handle);
  snd_pcm_close(c.handle);
  free(c.buffer);

  snd_pcm_drain(p.handle);
  snd_pcm_close(p.handle);
  free(p.buffer);

  /*
  snd_pcm_hw_params_free(c.params);
  snd_pcm_hw_params_free(p.params);
  */

  exit(0);
}

void *
process (void *data)
{
  while (1) {

    c.rc = snd_pcm_mmap_readi(c.handle, c.buffer, c.frames);
    if (c.rc == -EPIPE) {
      fprintf(stderr, "overrun occurred\n");
      snd_pcm_prepare(c.handle);
    } else if (c.rc < 0) {
      fprintf(stderr, "error from read: %s\n", snd_strerror(c.rc));
    } else if (c.rc != c.frames) {
      fprintf(stderr, "short read: read %d frames\n", c.rc);
    }

    register_t i = 0, j = 0;
    for(i = 0; i < c.size; i++, j+=2) {
      p.buffer[j] = c.buffer[i]; 
      p.buffer[j+1] = c.buffer[i];
    }

    p.rc = snd_pcm_mmap_writei(p.handle, p.buffer, p.frames);
    if (p.rc == -EPIPE) {
      fprintf(stderr, "underrun occurred\n");
      snd_pcm_prepare(p.handle);
    } else if (p.rc < 0) {
      fprintf(stderr, "error from writei: %s\n", snd_strerror(p.rc));
    } else if (p.rc != p.frames) {
      fprintf(stderr, "short write: wrote %d frames\n", p.rc);
    }

  }

  return NULL;
}

int
main (int argc, char *argv[])
{
  signal(SIGINT, exit_sighandler);

  alsa_capture("hw:1");
  alsa_play("hw:0");

  // priority - attributes  ...
  if (pthread_create(&live_thread, NULL, process, NULL)) {
    fprintf(stderr, "Error creating thread\n");
    return -1;
  }

  if (pthread_join(live_thread, NULL)) {
    fprintf(stderr, "Error joining thread\n");
    return -1;
  }

  return 0;
}
