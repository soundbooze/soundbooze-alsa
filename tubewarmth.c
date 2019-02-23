/*                                                     -*- linux-c -*-
    Copyright (C) 2004 Tom Szilagyi
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "tubewarmth.h"

void * 
instantiate_TubeWarmth(int sr) {
  
  void *ptr;
	
	if ((ptr = (TubeWarmth *) malloc(sizeof(TubeWarmth))) != NULL) {
		((TubeWarmth *)ptr)->sample_rate = sr;
		((TubeWarmth *)ptr)->run_adding_gain = 1.0f;

		((TubeWarmth *)ptr)->prev_med = 0.0f;
		((TubeWarmth *)ptr)->prev_out = 0.0f;

		((TubeWarmth *)ptr)->rdrive = 0.0f;
		((TubeWarmth *)ptr)->rbdr = 0.0f;
		((TubeWarmth *)ptr)->kpa = 0.0f;
		((TubeWarmth *)ptr)->kpb = 0.0f;
		((TubeWarmth *)ptr)->kna = 0.0f;
		((TubeWarmth *)ptr)->knb = 0.0f;
		((TubeWarmth *)ptr)->ap = 0.0f;
		((TubeWarmth *)ptr)->an = 0.0f;
		((TubeWarmth *)ptr)->imr = 0.0f;
		((TubeWarmth *)ptr)->kc = 0.0f;
		((TubeWarmth *)ptr)->srct = 0.0f;
		((TubeWarmth *)ptr)->sq = 0.0f;
		((TubeWarmth *)ptr)->pwrq = 0.0f;

		((TubeWarmth *)ptr)->prev_drive = -1.0f;
		((TubeWarmth *)ptr)->prev_blend = -11.0f;

    /*
     * manual
     
    ((TubeWarmth *)ptr)->drive = 0.6f;
		((TubeWarmth *)ptr)->blend = 0.2f;

    port_range_hints[DRIVE].LowerBound = 0.1f;
    port_range_hints[DRIVE].UpperBound = 10.0f;
    port_range_hints[BLEND].LowerBound = -10.0f;
    port_range_hints[BLEND].UpperBound = 10.0f;
    */

		return ptr;
	}
  return NULL;
}

static inline float
M(float x) {

	if ((x > EPS) || (x < -EPS))
		return x;
	else
		return 0.0f;
}

static inline float
D(float x) {

	if (x > EPS)
		return sqrt(x);
	else if (x < -EPS)
		return sqrt(-x);
	else
		return 0.0f;
}

void 
run_TubeWarmth(void *Instance, float *input, float *output, unsigned int SampleCount) {
  
	TubeWarmth *ptr = (TubeWarmth *)Instance;

	float drive = 5.0f; //LIMIT(*(ptr->drive),0.1f,10.0f);
	float blend = 4.0f; //LIMIT(*(ptr->blend),-10.0f,10.0f);

	unsigned long sample_index;
	unsigned long sample_count = SampleCount;
	unsigned long sample_rate = ptr->sample_rate;

	float rdrive = ptr->rdrive;
	float rbdr = ptr->rbdr;
	float kpa = ptr->kpa;
	float kpb = ptr->kpb;
	float kna = ptr->kna;
	float knb = ptr->knb;
	float ap = ptr->ap;
	float an = ptr->an;
	float imr = ptr->imr;
	float kc = ptr->kc;
	float srct = ptr->srct;
	float sq = ptr->sq;
	float pwrq = ptr->pwrq;

	float prev_med;
	float prev_out;
	float in;
	float med;
	float out;

	if ((ptr->prev_drive != drive) || (ptr->prev_blend != blend)) {

		rdrive = 12.0f / drive;
		rbdr = rdrive / (10.5f - blend) * 780.0f / 33.0f;
		kpa = D(2.0f * (rdrive*rdrive) - 1.0f) + 1.0f;
		kpb = (2.0f - kpa) / 2.0f;
		ap = ((rdrive*rdrive) - kpa + 1.0f) / 2.0f;
		kc = kpa / D(2.0f * D(2.0f * (rdrive*rdrive) - 1.0f) - 2.0f * rdrive*rdrive);

		srct = (0.1f * sample_rate) / (0.1f * sample_rate + 1.0f);
		sq = kc*kc + 1.0f;
		knb = -1.0f * rbdr / D(sq);
		kna = 2.0f * kc * rbdr / D(sq);
		an = rbdr*rbdr / sq;
		imr = 2.0f * knb + D(2.0f * kna + 4.0f * an - 1.0f);
		pwrq = 2.0f / (imr + 1.0f);

		ptr->prev_drive = drive;
		ptr->prev_blend = blend;
	}

	for (sample_index = 0; sample_index < sample_count; sample_index++) {

		in = *(input++);
		prev_med = ptr->prev_med;
		prev_out = ptr->prev_out;

		if (in >= 0.0f) {
			med = (D(ap + in * (kpa - in)) + kpb) * pwrq;
		} else {
			med = (D(an - in * (kna + in)) + knb) * pwrq * -1.0f;
		}

		out = srct * (med - prev_med + prev_out);

		if (out < -1.0f)
			out = -1.0f;
		
		*(output++) = out;

		ptr->prev_med = M(med);
		ptr->prev_out = M(out);
	}

	ptr->rdrive = rdrive;
	ptr->rbdr = rbdr;
	ptr->kpa = kpa;
	ptr->kpb = kpb;
	ptr->kna = kna;
	ptr->knb = knb;
	ptr->ap = ap;
	ptr->an = an;
	ptr->imr = imr;
	ptr->kc = kc;
	ptr->srct = srct;
	ptr->sq = sq;
	ptr->pwrq = pwrq;
}

void
set_run_adding_gain_TubeWarmth(void *Instance, float gain) {
	TubeWarmth *ptr = (TubeWarmth *)Instance;
	ptr->run_adding_gain = gain;
}

void 
run_adding_TubeWarmth(void *Instance, float *input, float *output,
		      unsigned int SampleCount) {
  
	TubeWarmth *ptr = (TubeWarmth *)Instance;

	float drive = 5.0f; // LIMIT(*(ptr->drive),0.1f,10.0f);
	float blend = 4.0f; // LIMIT(*(ptr->blend),-10.0f,10.0f);

	unsigned long sample_index;
	unsigned long sample_count = SampleCount;
	unsigned long sample_rate = ptr->sample_rate;

	float rdrive = ptr->rdrive;
	float rbdr = ptr->rbdr;
	float kpa = ptr->kpa;
	float kpb = ptr->kpb;
	float kna = ptr->kna;
	float knb = ptr->knb;
	float ap = ptr->ap;
	float an = ptr->an;
	float imr = ptr->imr;
	float kc = ptr->kc;
	float srct = ptr->srct;
	float sq = ptr->sq;
	float pwrq = ptr->pwrq;

	float prev_med;
	float prev_out;
	float in;
	float med;
	float out;

	if ((ptr->prev_drive != drive) || (ptr->prev_blend != blend)) {

		rdrive = 12.0f / drive;
		rbdr = rdrive / (10.5f - blend) * 780.0f / 33.0f;
		kpa = D(2.0f * (rdrive*rdrive) - 1.0f) + 1.0f;
		kpb = (2.0f - kpa) / 2.0f;
		ap = ((rdrive*rdrive) - kpa + 1.0f) / 2.0f;
		kc = kpa / D(2.0f * D(2.0f * (rdrive*rdrive) - 1.0f) - 2.0f * rdrive*rdrive);

		srct = (0.1f * sample_rate) / (0.1f * sample_rate + 1.0f);
		sq = kc*kc + 1.0f;
		knb = -1.0f * rbdr / D(sq);
		kna = 2.0f * kc * rbdr / D(sq);
		an = rbdr*rbdr / sq;
		imr = 2.0f * knb + D(2.0f * kna + 4.0f * an - 1.0f);
		pwrq = 2.0f / (imr + 1.0f);

		ptr->prev_drive = drive;
		ptr->prev_blend = blend;
	}

	for (sample_index = 0; sample_index < sample_count; sample_index++) {

		in = *(input++);
		prev_med = ptr->prev_med;
		prev_out = ptr->prev_out;

		if (in >= 0.0f) {
			med = (D(ap + in * (kpa - in)) + kpb) * pwrq;
		} else {
			med = (D(an - in * (kna + in)) + knb) * pwrq * -1.0f;
		}

		out = srct * (med - prev_med + prev_out);

		if (out < -1.0f)
			out = -1.0f;
		
		*(output++) += out * ptr->run_adding_gain;

		ptr->prev_med = M(med);
		ptr->prev_out = M(out);
	}

	ptr->rdrive = rdrive;
	ptr->rbdr = rbdr;
	ptr->kpa = kpa;
	ptr->kpb = kpb;
	ptr->kna = kna;
	ptr->knb = knb;
	ptr->ap = ap;
	ptr->an = an;
	ptr->imr = imr;
	ptr->kc = kc;
	ptr->srct = srct;
	ptr->sq = sq;
	ptr->pwrq = pwrq;
}

void 
cleanup_TubeWarmth(void *Instance) {
	free(Instance);
}
