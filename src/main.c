
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <errno.h>

#include <hal/nrf_gpiote.h>
#include <hal/nrf_gpio.h>
#include <nrfx_pwm.h>

#include <sound_gen.h>
#include <songs.h>

#include <math.h>

#ifndef CONFIG_BOARD_THINGY53_NRF5340_CPUAPP
#warning "This project was made for the thingy53_nrf5340_cpuapp target. Please perform necessary updates for other targets"
#endif

#define PWM_COUNTERTOP  255

#define PWM_BUF_LENGTH 1000

// Thingy53 Buzzer pin
#define PWM_PIN NRF_GPIO_PIN_MAP(1, 15)

K_SEM_DEFINE(sem_update_pwm_buf, 0, 1);

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL,
		      0x84, 0xaa, 0x60, 0x74, 0x52, 0x8a, 0x8b, 0x86,
		      0xd3, 0x4c, 0xb7, 0x1d, 0x1d, 0xdc, 0x53, 0x8d),
};

static nrfx_pwm_t my_pwm = NRFX_PWM_INSTANCE(1);
static int buf_to_update_index;

void nrfx_pwm_handler(nrfx_pwm_evt_type_t event_type, void * p_context)
{
	switch(event_type) {
		case NRFX_PWM_EVT_END_SEQ0:
			buf_to_update_index = 0;
			k_sem_give(&sem_update_pwm_buf);
			break;
		case NRFX_PWM_EVT_END_SEQ1:
			buf_to_update_index = 1;
			k_sem_give(&sem_update_pwm_buf);
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
            PWM_PIN,				// channel 0
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
        .values.p_common = (const nrf_pwm_values_common_t*)&seq_values,
        .length          = PWM_BUF_LENGTH,
        .repeats         = 1,
        .end_delay       = 0
    };

	static nrf_pwm_sequence_t const seq2 =
    {
        .values.p_common = (const nrf_pwm_values_common_t*)&seq_values_b,
        .length          = PWM_BUF_LENGTH,
        .repeats         = 1,
        .end_delay       = 0
    };
	nrfx_pwm_complex_playback(&my_pwm, &seq, &seq2, 1, NRFX_PWM_FLAG_SIGNAL_END_SEQ0 | NRFX_PWM_FLAG_SIGNAL_END_SEQ1 | NRFX_PWM_FLAG_LOOP);
}

extern volatile bool stop_playing_flag;

static int cmd_stop(
	const struct shell *shell, size_t argc, char *argv[])
{
    shell_fprintf(shell, SHELL_NORMAL, "ACK music_box stop\n");
	stop_playing_flag = true;
    return 0;
}
 
static int cmd_play(
	const struct shell *shell, size_t argc, char *argv[])
{
    shell_fprintf(shell, SHELL_NORMAL, "TODO: Play command\n");
    return 0;
}
 
SHELL_STATIC_SUBCMD_SET_CREATE(
	music_box_cmds,
	SHELL_CMD(play, NULL,
		"usage example:\n$ music_box play\n",
		cmd_play),
	SHELL_CMD(stop, NULL,
		"usage example:\n$ music_box stop\n",
		cmd_stop),
	SHELL_SUBCMD_SET_END
	);

SHELL_CMD_REGISTER(music_box, &music_box_cmds, "Control Music Box", NULL);


int main(void)
{
	int err;
	
	pwm_init();
	pwm_set_duty_cycle();

	err = bt_enable(NULL);
	if (err) {
		printk("ERROR bt_enable %d", err);
		while (1) { /* spin */ };
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("ERROR bt_le_adv_start %d", err);
		while (1) { /* spin */ };
	}

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
	}

	return 0;
}

void thread_play_notes_func(void)
{
	music_play_song(&song_holy_night);
}

K_THREAD_DEFINE(thread_play_notes, 1024, thread_play_notes_func, 0, 0, 0, K_LOWEST_APPLICATION_THREAD_PRIO, 0, 100); 
