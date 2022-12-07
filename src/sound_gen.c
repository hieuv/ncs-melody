#include <sound_gen.h>
#include <math.h>

static sg_osc_state_t m_osc_list[NUM_OSCILLATORS];

float sin_table[256];

float note_table[NUM_OCTAVES*NOTES_PR_OCTAVE];

static sg_osc_state_t *alloc_oscillator(void)
{
	for(int i = 0; i < NUM_OSCILLATORS; i++) {
		if(!m_osc_list[i].used) {
			m_osc_list[i].used = true;
			return &m_osc_list[i];
		}
	}
	return 0;
} 

static float gen_func_sinus(sg_osc_state_t *state)
{
	return sin_table[state->phase_i >> 8];
}

static float gen_func_square(sg_osc_state_t *state)
{
	if(state->phase_i > 0x8000) return 1.0f;
	else return -1.0f;
}

static float gen_func_sawtooth(sg_osc_state_t *state)
{
	return ((float)state->phase_i / 32768.0f) - 1.0f;
}

static bool run_adsr_comp(sg_adsr_t *adsr, uint32_t t)
{
	static float tmp;
	float tf = (float)t;
	if(tf < adsr->a) {
		adsr->result = tf / adsr->a;
	} else if(tf < adsr->s) {
		adsr->result = 1.0f;
	} else if(tf < (adsr->s + adsr->r)){
		tmp = (tf - adsr->s);
		adsr->result = (adsr->r - (tf - adsr->s)) / adsr->r;
	} else {
		adsr->result = 0.0f;
		return true;
	}
	return false;
}

void sg_fill_buffer(nrf_pwm_values_common_t *dst_ptr, int num)
{
	sg_osc_state_t *osc;
	static float tmp_res_buf[1000];
	int active_oscillators = 0;
	for(int i = 0; i < num; i++) tmp_res_buf[i] = 0.0f;
	float result;
	for(int osc_index = 0; osc_index < NUM_OSCILLATORS; osc_index++) {
		if(m_osc_list[osc_index].used) {
			active_oscillators++;
			osc = &m_osc_list[osc_index];
			for(int i = 0; i < num; i++) {
				tmp_res_buf[i] += osc->func(osc)*osc->adsr.result;
				osc->t_i++;
				osc->phase_i = (osc->phase_i + osc->phase_diff_i) & 0xFFFF;
				if((i%SG_ADSR_SUBDIV) == 0) {
					if(run_adsr_comp(&osc->adsr, osc->t_i / SG_SAMPLES_PR_MS)) {
						osc->used = false;
						//printk("Freeing osc\n");
						break;
					}
					// For simplicity we bake amplitude calculations into the ADSR result
					osc->adsr.result = osc->adsr.result * osc->amplitude * osc->global_amplitude;
				}
			}
		}
	}
	if(active_oscillators == 0) {
		for(int i = 0; i < num; i++) dst_ptr[i] = 127;
	} else {
		for(int i = 0; i < num; i++) dst_ptr[i] = (uint16_t)((tmp_res_buf[i] + 1.0f) * 127.0f);
	}
}

void sg_play_freq(float frequency, float amp, int instrument_index)
{
	sg_osc_state_t *new_osc = alloc_oscillator();
	if(new_osc) {
		new_osc->t = 0.0f;
		new_osc->frequency = frequency;
		new_osc->amplitude = amp;
		new_osc->t_i = 0;
		new_osc->phase_i = 0;
		new_osc->phase_diff_i = (uint32_t)(frequency * 256.0f * 256.0f / (float)SG_SAMPLE_FREQ);
		switch(instrument_index) {
			case 0:
				new_osc->adsr.a = 50;
				new_osc->adsr.d = 100;
				new_osc->adsr.s = 100;
				new_osc->adsr.r = 500;
				new_osc->global_amplitude = 0.5f;
				new_osc->func = gen_func_sinus;
				break;
			case 1:
				new_osc->adsr.a = 50;
				new_osc->adsr.d = 100;
				new_osc->adsr.s = 300;
				new_osc->adsr.r = 3000;
				new_osc->global_amplitude = 0.8f;
				new_osc->func = gen_func_sinus;
				break;
		}
	}
}

int sg_play_note(int note_index, float amp, int instrument_index)
{
	if(note_index >= (NUM_OCTAVES*NOTES_PR_OCTAVE)) return -1;
	sg_play_freq(note_table[note_index], amp, instrument_index);
}

#define PI 3.14159265358979f
void sg_init(void)
{
	for(int i = 0; i < NUM_OSCILLATORS; i++) {
		m_osc_list[i].used = false;
	}
	for(int i = 0; i < 256; i++) {
		sin_table[i] = sinf(PI * 2.0f * (float)i / 256.0f);
	}
	for(int i = 0; i < NUM_OCTAVES; i++) {
		note_table[i*NOTES_PR_OCTAVE] = OCTAVE_0_START_FREQ * (float)(1 << i);
		for(int note = 1; note < NOTES_PR_OCTAVE; note++) {
			note_table[i*NOTES_PR_OCTAVE + note] = note_table[i*NOTES_PR_OCTAVE + note - 1] * HALF_NOTE_FACTOR;
		}
	}
}