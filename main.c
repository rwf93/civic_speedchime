#include <pico/stdlib.h>

#include <hardware/pwm.h>
#include <hardware/clocks.h>
#include <hardware/gpio.h>

#include <hardware/pio.h>

#include <hardware/adc.h>

#include <stdint.h>
#include <stdio.h>

#include <pico/multicore.h>

#include <hardware/sync.h>

#include "hardware/timer.h"
#include "pio_reader.pio.h"

#define SPEED_CHIME_PIN 15
#define SPEED_SENSOR_PIN 16
#define WHEEL_CIRCUMFERENCE 1806

static bool speed_sensor_enable = true;

static uint32_t now = 0;
static uint32_t gap = 0;
static uint32_t last = 0;

float pulse_to_kmh(uint32_t pulse_gap_us) {
    if (pulse_gap_us == 0) return 0.0f;
    return (WHEEL_CIRCUMFERENCE * 3.6f) / (float)pulse_gap_us;
}

// 100: 7000-8000 range
// 30-40: last read was 23000 - 23400
void gpio_callback(uint gpio, uint32_t events) {
	now = time_us_32();
	gap = now - last;
	last = now;
/*
	if(gap > 6000) {
		printf("%lu\n", gap);
		if(gap <= 7000) {
			uint32_t disabled_interrupts = save_and_disable_interrupts();	
		
			gpio_put(SPEED_CHIME_PIN, true);
			busy_wait_us_32(250000);
			gpio_put(SPEED_CHIME_PIN, false);
			busy_wait_us_32(1e6);

			gpio_put(SPEED_CHIME_PIN, true);
			busy_wait_us_32(250000);
			gpio_put(SPEED_CHIME_PIN, false);
			busy_wait_us_32(1e6);
			
			restore_interrupts(disabled_interrupts);
		}
	}
*/
	/*
	if(gap > 4097) {
		printf("%lu\n", gap);

		if(gap <= 23500) {
			sleep_ms(250);
			gpio_put(SPEED_CHIME_PIN, true);
			sleep_ms(1000);
			gpio_put(SPEED_CHIME_PIN, false);
			sleep_ms(250);
			gpio_put(SPEED_CHIME_PIN, true);
			sleep_ms(1000);
			gpio_put(SPEED_CHIME_PIN, false);
		}

		if(gap >= 7000 && gap <= 8000)
			printf("hitting 90-100?\n");
		if(gap >= 12000 && gap <= 13200)
			printf("hitting 60\n");
		if(gap >= 15200 && gap <= 15500)
			printf("hitting 50\n");
		if(gap >= 18500 && gap <= 19000)
			printf("hitting 40\n");
		if(gap >= 23100 && gap <= 23500)
			printf("hitting 30\n");

	}
	*/
}

int main() {
	stdio_init_all();

	gpio_init(SPEED_CHIME_PIN);
	gpio_set_dir(SPEED_CHIME_PIN, GPIO_OUT);

	now = time_us_32();
	gpio_set_irq_enabled_with_callback(SPEED_SENSOR_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

	while(1) {
		if(gap > 6000) {
			printf("%lu\n", gap);
			if(gap <= 7000) {
			}
		}
	}

	return 0;
}
