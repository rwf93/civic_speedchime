#include <pico/stdlib.h>

#include <hardware/pwm.h>
#include <hardware/clocks.h>
#include <hardware/gpio.h>

#include <hardware/pio.h>

#include <stdio.h>

#include "pio_reader.pio.h"

typedef struct pwm_reader {
	uint gpio_pin, state;
	uint32_t pulse_width, period;
	PIO pio_interface;
} pwm_reader_t;

void setup_pwm_reader(pwm_reader_t *reader, uint gpio_pin) {
	reader->gpio_pin = gpio_pin;
	reader->pio_interface = pio0;
	reader->state = 0;

	pio_gpio_init(reader->pio_interface, reader->gpio_pin);
	uint pio_offset = pio_add_program(reader->pio_interface, &pio_reader_program);
	pio_sm_config cfg = pio_reader_program_get_default_config(pio_offset);

	sm_config_set_jmp_pin(&cfg, reader->gpio_pin);
	sm_config_set_in_shift(&cfg, false, false, 0);
	pio_sm_init(reader->pio_interface, reader->state, pio_offset, &cfg);
	pio_sm_set_enabled(reader->pio_interface, reader->state, true);
} 

int pwm_read(pwm_reader_t *reader) {
	pio_sm_clear_fifos(reader->pio_interface, reader->state);

	while(pio_sm_get_rx_fifo_level(reader->pio_interface, reader->state) < 2);

	uint32_t t1 = (0xFFFFFFFF - pio_sm_get(reader->pio_interface, reader->state));
	uint32_t t2 = (0xFFFFFFFF - pio_sm_get(reader->pio_interface, reader->state));
	
	if(t1 > t2) {
		reader->period = t1;
		reader->pulse_width = t2;
	} else {
		reader->period = t2;
		reader->pulse_width = t1;
	}
	
	reader->pulse_width = 2 * reader->pulse_width;
	reader->period = 2 * reader->period;
	
	return 0;
}

float read_duty_cycle(pwm_reader_t *reader) {
	if(pwm_read(reader) == -1)
		return -1;
	
	return ((float)reader->pulse_width / (float)reader->period);
}

float read_period(pwm_reader_t *reader) {
	if(pwm_read(reader) == -1)
		return -1;
	
	return (reader->period * 0.000000008);
}

float read_pulse_width(pwm_reader_t *reader) {
	if(pwm_read(reader)== -1)
		return -1;

	return (reader->pulse_width * 0.000000008);
}

int main() {
	stdio_init_all();

	pwm_reader_t reader;
	setup_pwm_reader(&reader, 16);
	
	while(true) {
		printf("%.8f %.8f %.8f\n", read_duty_cycle(&reader), read_period(&reader), read_pulse_width(&reader));
	}

	return 0;
}
