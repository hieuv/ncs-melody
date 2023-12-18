#ifndef __SOUND_GEN_H
#define __SOUND_GEN_H

#include <zephyr/kernel.h>
#include <nrfx_pwm.h>

#define NUM_OSCILLATORS 16
#define SG_ADSR_SUBDIV 100
#define SG_SAMPLE_FREQ 31250
#define SG_SAMPLES_PR_MS 31

#define NUM_OCTAVES 7
#define NOTES_PR_OCTAVE 12
#define OCTAVE_0_START_FREQ 55.0f
#define HALF_NOTE_FACTOR 1.05946309f

struct sg_osc_state_s;
typedef struct og_osc_state_s sg_osc_state_t;

typedef struct {
	float a;
	float d;
	float s;
	float r;
	float result;
} sg_adsr_t;

typedef float (*osc_gen_func_t)(sg_osc_state_t *state);

struct og_osc_state_s {
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
};

typedef struct {
	osc_gen_func_t gen_func;
} sg_oscillator_t;

typedef struct {
	float a, d, s, r;
	float amp_base;
	osc_gen_func_t osc_func;
} sg_instrument_t;

float gen_func_sinus(sg_osc_state_t *state);

float gen_func_square(sg_osc_state_t *state);

float gen_func_sawtooth(sg_osc_state_t *state);

float gen_func_noise(sg_osc_state_t *state);

void sg_init();

void sg_fill_buffer(nrf_pwm_values_common_t *dst_ptr, int num);

int sg_play_freq(float frequency, float amp, sg_instrument_t *instr);

int sg_play_note(int note_index, float amp, sg_instrument_t *instr);

#endif