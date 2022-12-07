#ifndef __SOUND_GEN_H
#define __SOUND_GEN_H

#include <zephyr.h>
#include <nrfx_pwm.h>

#define NUM_OSCILLATORS 4
#define SG_ADSR_SUBDIV 100
#define SG_SAMPLE_FREQ 31250
#define SG_SAMPLES_PR_MS 31

typedef struct {
	float a;
	float d;
	float s;
	float r;
	float result;
} sg_adsr_t;

typedef float (*osc_gen_func_t)(struct sg_osc_state_t *state);

typedef struct {
	bool used;
	float t;
	uint32_t t_i;
	uint32_t phase_i;
	uint32_t phase_diff_i;
	uint32_t amp_i;
	float amplitude;
	float frequency;
	float val;

	// Instrument variables
	float global_amplitude;
	sg_adsr_t adsr;
	osc_gen_func_t func;
} sg_osc_state_t;

typedef struct {
	osc_gen_func_t gen_func;
} sg_oscillator_t;

void sg_init();

void sg_fill_buffer(nrf_pwm_values_common_t *dst_ptr, int num);

void sg_play_note(float frequency, float amp, int instrument_index);


#endif