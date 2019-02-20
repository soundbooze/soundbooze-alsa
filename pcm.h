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

#ifndef __PCM_H
#define __PCM_H

#include <sys/types.h>

#define PCM_S16LE 32768

__BEGIN_DECLS
void pcm_to_float (const int16_t *, float *, size_t);
void float_to_pcm (const float *, int16_t *, size_t);
__END_DECLS

#endif
