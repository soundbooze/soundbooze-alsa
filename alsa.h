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

#ifndef __ALSA_H
#define __ALSA_H

#include <alsa/asoundlib.h>

#define ALSA_PCM_NEW_HW_PARAMS_API

#define FRAMES        128
#define SAMPLERATE  44100

struct AlsaCapture {
  int channel;
  int rc;
  unsigned int val;
  int dir;
  int size;
  int16_t *buffer;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  snd_pcm_uframes_t frames;
};

extern struct AlsaCapture c;

struct AlsaPlay {
  int channel;
  int rc;
  unsigned int val;
  int dir;
  int size;
  int16_t *buffer;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  snd_pcm_uframes_t frames;
};

extern struct AlsaPlay p;

__BEGIN_DECLS
int alsa_capture (const char *name);
int alsa_play (const char *name);
__END_DECLS

#endif
