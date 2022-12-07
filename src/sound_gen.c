#include <sound_gen.h>
#include <math.h>

static sg_osc_state_t m_osc_list[NUM_OSCILLATORS];

int16_t sin_table[256];

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

static int32_t gen_func_sinus(sg_osc_state_t *state)
{
	return (((int32_t)sin_table[state->phase_i >> 8]/2)*state->adsr.result);
}

static int32_t gen_func_square(sg_osc_state_t *state)
{
	if(state->phase_i > 0x8000) return state->adsr.result * 0x0FFF;
	else return -(state->adsr.result * 0x0FFF);
}

static int32_t gen_func_sawtooth(sg_osc_state_t *state)
{
	return (((int32_t)state->phase_i - 0x8000)/4) * state->adsr.result;
}

static bool run_adsr_comp(sg_adsr_t *adsr, uint32_t t)
{
	static uint32_t tmp;
	if(t < adsr->a) {
		adsr->result = t * 65536 / adsr->a;
	} else if(t < adsr->s) {
		adsr->result = 65535;
	} else if(t < (adsr->s + adsr->r)){
		tmp = (t - adsr->s);
		adsr->result = (adsr->r - (t - adsr->s)) * 65535 / adsr->r;
	} else {
		adsr->result = 0;
		return true;
	}
	return false;
}

void sg_fill_buffer(nrf_pwm_values_common_t *dst_ptr, int num)
{
	sg_osc_state_t *osc;
	int active_oscillators = 0;
	for(int i = 0; i < num; i++) dst_ptr[i] = 0x8000;
	for(int osc_index = 0; osc_index < NUM_OSCILLATORS; osc_index++) {
		if(m_osc_list[osc_index].used) {
			active_oscillators++;
			osc = &m_osc_list[osc_index];
			for(int i = 0; i < num; i++) {
				//dst_ptr[i] = (uint16_t)((cosf(osc->t*osc->frequency*6.28f)+1.0f)*127.0f*osc->amplitude);
				//osc->t += 1.0f/31250.0f;
				//dst_ptr[i] = ((int32_t)sin_table[osc->phase_i >> 8]*osc->adsr.result) / 0x1000000 + 0x8000;
				dst_ptr[i] += osc->func(osc) / 0x10000 + 0x8000;
				osc->t_i++;
				osc->phase_i += osc->phase_diff_i;
				osc->phase_i &= 0xFFFF;
				if((i%SG_ADSR_SUBDIV) == 0) {
					if(run_adsr_comp(&osc->adsr, osc->t_i / SG_SAMPLES_PR_MS)) {
						osc->used = false;
						//printk("Freeing osc\n");
						break;
					}
				}
			}
		}
	}
	if(active_oscillators == 0) {
		
	}
}

void sg_play_note(float frequency, float amp, int instrument_index)
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
				new_osc->adsr.s = 300;
				new_osc->adsr.r = 1000;
				new_osc->func = gen_func_sinus;
				break;
			case 1:
				new_osc->adsr.a = 50;
				new_osc->adsr.d = 100;
				new_osc->adsr.s = 300;
				new_osc->adsr.r = 2000;
				new_osc->func = gen_func_sinus;
				break;
		}
		
		//printk("Allocating osc\n");
	}
}

#define PI 3.14159265358979f
void sg_init(void)
{
	for(int i = 0; i < NUM_OSCILLATORS; i++) {
		m_osc_list[i].used = false;
	}
	for(int i = 0; i < 256; i++) {
		sin_table[i] = (int16_t)((sinf(PI * 2.0f * (float)i / 256.0f)) * 32767.0f);
	}
}