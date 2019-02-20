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

#include "alsa.h"

int 
alsa_capture (const char *name) 
{
  c.rc = snd_pcm_open(&c.handle, name, SND_PCM_STREAM_CAPTURE, 0);
  if (c.rc < 0) {
    fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(c.rc));
    return -1;
  }

  snd_pcm_hw_params_alloca(&c.params);
  snd_pcm_hw_params_any(c.handle, c.params);
  snd_pcm_hw_params_set_access(c.handle, c.params, SND_PCM_ACCESS_MMAP_INTERLEAVED);
  snd_pcm_hw_params_set_format(c.handle, c.params, SND_PCM_FORMAT_S16_LE);

  c.channel = 1;
  snd_pcm_hw_params_set_channels(c.handle, c.params, c.channel);

  c.val = SAMPLERATE;
  snd_pcm_hw_params_set_rate_near(c.handle, c.params, &c.val, &c.dir);

  c.frames = FRAMES;
  snd_pcm_hw_params_set_period_size_near(c.handle, c.params, &c.frames, &c.dir);

  c.rc = snd_pcm_hw_params(c.handle, c.params);
  if (c.rc < 0) {
    fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(c.rc));
    return -1;
  }

  snd_pcm_hw_params_get_period_size(c.params, &c.frames, &c.dir);

  c.size = c.frames * (2 * c.channel); /* 2 bytes/sample, 1 channels */
  c.buffer = (int16_t *) malloc(c.size * sizeof(int16_t));

  snd_pcm_hw_params_get_period_time(c.params, &c.val, &c.dir);

  return 0;
}

int 
alsa_play (const char *name) 
{
  p.rc = snd_pcm_open(&p.handle, name, SND_PCM_STREAM_PLAYBACK, 0);
  if (p.rc < 0) {
    fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(p.rc));
    return -1;
  }

  snd_pcm_hw_params_alloca(&p.params);
  snd_pcm_hw_params_any(p.handle, p.params);
  snd_pcm_hw_params_set_access(p.handle, p.params, SND_PCM_ACCESS_MMAP_INTERLEAVED);
  snd_pcm_hw_params_set_format(p.handle, p.params, SND_PCM_FORMAT_S16_LE);

  p.channel = 2;
  snd_pcm_hw_params_set_channels(p.handle, p.params, p.channel);

  p.val = SAMPLERATE;
  snd_pcm_hw_params_set_rate_near(p.handle, p.params, &p.val, &p.dir);

  p.frames = FRAMES;
  snd_pcm_hw_params_set_period_size_near(p.handle, p.params, &p.frames, &p.dir);

  p.rc = snd_pcm_hw_params(p.handle, p.params);
  if (p.rc < 0) {
    fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(p.rc));
    return -1;
  }

  snd_pcm_hw_params_get_period_size(p.params, &p.frames, &p.dir);

  p.size = p.frames * (2 * p.channel); /* 2 bytes/sample, 2 channels */
  p.buffer = (int16_t *) malloc(p.size * sizeof(int16_t));

  snd_pcm_hw_params_get_period_time(p.params, &p.val, &p.dir);

  return 0;
}
