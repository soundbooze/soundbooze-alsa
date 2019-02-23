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

#ifndef __TUBEWARMTH_H
#define __TUBEWARMTH_H

#include <stdlib.h>
#include <math.h>

#define EPS 0.000000001f
#define LIMIT(v,l,u) ((v)<(l)?(l):((v)>(u)?(u):(v)))

typedef struct {
	float *drive;
	float *blend;

	float prev_med;
	float prev_out;

	float rdrive;
	float rbdr;
	float kpa;
	float kpb;
	float kna;
	float knb;
	float ap;
	float an;
	float imr;
	float kc;
	float srct;
	float sq;
	float pwrq;

	float prev_drive;
	float prev_blend;

	unsigned long sample_rate;
	float run_adding_gain;
} TubeWarmth;

__BEGIN_DECLS
void * instantiate_TubeWarmth(int);
void run_TubeWarmth(void *, float *, float *, unsigned int);
void cleanup_TubeWarmth(void *);
__END_DECLS

#endif
