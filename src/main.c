/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
//#include <zephyr/drivers/gpio.h>
#include <nrfx_pwm.h>
#include <sound_gen.h>
#include <math.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

#define PWM_COUNTERTOP  255

#define PWM_BUF_LENGTH 1000

K_SEM_DEFINE(sem_update_pwm_buf, 0, 1);

static nrfx_pwm_t my_pwm = NRFX_PWM_INSTANCE(1);
static int buf_to_update_index;

void nrfx_pwm_handler(nrfx_pwm_evt_type_t event_type, void * p_context)
{
	switch(event_type) {
		case NRFX_PWM_EVT_END_SEQ0:
			buf_to_update_index = 0;
			k_sem_give(&sem_update_pwm_buf);
			//printk("0");
			break;
		case NRFX_PWM_EVT_END_SEQ1:
			buf_to_update_index = 1;
			k_sem_give(&sem_update_pwm_buf);
			//printk("1");
			break;
		default:
			break;
	}
}

static int pwm_init(void)
{
	static nrfx_pwm_config_t const config0 =
    {
        .output_pins =
        {
            0,						// channel 0
            NRFX_PWM_PIN_NOT_USED,	// channel 1
            NRFX_PWM_PIN_NOT_USED,	// channel 2
            NRFX_PWM_PIN_NOT_USED	// channel 3
        },
        .irq_priority = 5,
        .base_clock   = NRF_PWM_CLK_16MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = PWM_COUNTERTOP,
        .load_mode    = NRF_PWM_LOAD_COMMON,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    return (nrfx_pwm_init(&my_pwm, &config0, nrfx_pwm_handler, NULL) == NRFX_SUCCESS) ? 0 : -1;

	// If PWM callbacks are to be used, remember to configure the interrupts correctly
	IRQ_CONNECT(PWM1_IRQn, 0, nrfx_pwm_1_irq_handler, 0, 0);
	irq_enable(PWM1_IRQn);
}

//static nrf_pwm_values_common_t *next_buf;
// This array cannot be allocated on stack (hence "static") and it must be in RAM 
static nrf_pwm_values_common_t seq_values[PWM_BUF_LENGTH];
static nrf_pwm_values_common_t seq_values_b[PWM_BUF_LENGTH];

static void pwm_set_duty_cycle()
{
	sg_fill_buffer(seq_values, PWM_BUF_LENGTH);
	sg_fill_buffer(seq_values_b, PWM_BUF_LENGTH);

    static nrf_pwm_sequence_t const seq =
    {
        .values.p_common = &seq_values,
        .length          = PWM_BUF_LENGTH,
        .repeats         = 1,
        .end_delay       = 0
    };

	static nrf_pwm_sequence_t const seq2 =
    {
        .values.p_common = &seq_values_b,
        .length          = PWM_BUF_LENGTH,
        .repeats         = 1,
        .end_delay       = 0
    };
	nrfx_pwm_complex_playback(&my_pwm, &seq, &seq2, 1, NRFX_PWM_FLAG_SIGNAL_END_SEQ0 | NRFX_PWM_FLAG_SIGNAL_END_SEQ1 | NRFX_PWM_FLAG_LOOP);
}

void main(void)
{
	//int ret;

	pwm_init();
	
	pwm_set_duty_cycle();

	printk("Hello\n");

	sg_init();
	
	while (1) {
		k_sem_take(&sem_update_pwm_buf, K_FOREVER);
		switch(buf_to_update_index) {
			case 0:
				sg_fill_buffer(seq_values, PWM_BUF_LENGTH);
				break;
			case 1:
				sg_fill_buffer(seq_values_b, PWM_BUF_LENGTH);
				break;
			default:
				break;
		}
		//sg_fill_buffer(buf_ptrs[buf_to_update_index], PWM_BUF_LENGTH);
	}
}

void thread_play_notes_func(void)
{
	float freq = 50.0f;
	while(1) {
		sg_play_note(freq, 1.0f, 0);
		//sg_play_note(freq*2.0f, 1.0f, 0);
		freq *= 2.0f;
		if(freq > 10000.0f) freq = 50.0f;
		k_msleep(3000);
	}
}

K_THREAD_DEFINE(thread_play_notes, 1024, thread_play_notes_func, 0, 0, 0, K_LOWEST_APPLICATION_THREAD_PRIO, 0, 100); 
