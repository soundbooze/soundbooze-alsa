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

#include "pcm.h"

void
pcm_to_float (const int16_t *buf_p, float *buf_d, size_t N)
{
  register_t i = 0;
  for (; i < N; i++) {
    float d = ((float) *(buf_p + i)) / (float) PCM_S16LE;
    if (d > 1) d = 1.0;
    if (d < -1) d = -1.0;
    *(buf_d + i) = d;
  }
}

void
float_to_pcm (const float *buf_d, int16_t *buf_p, size_t N)
{
  register_t i = 0;
  for (; i < N; i++) {
    float d = *(buf_d + i) * (float) PCM_S16LE;
    if (d > PCM_S16LE) d = PCM_S16LE;
    if (d < -PCM_S16LE) d = -PCM_S16LE;
    *(buf_p + i) = (int16_t) d;
  }
}
