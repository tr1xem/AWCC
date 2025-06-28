#include "lights.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>	// For time()
#include <unistd.h> // for sleep()
void brightness(uint8_t value) {
	device_acquire();
	send_set_dim(100 - value, 4, ZONE_ALL);
	device_release();
}

// Refactor for static color
void static_color(uint32_t color) {
	device_acquire();
	send_animation_remove(1);
	send_animation_config_start(1);
	send_zone_select(1, 4, ZONE_ALL);
	send_add_action(ACTION_COLOR, 1, 2, color);
	send_animation_config_save(1);
	send_animation_set_default(1);
	device_release();
}

// Refactor for breathe effect
void breathe(uint32_t color) {
	device_acquire();
	send_animation_remove(1);
	send_animation_config_start(1);
	send_zone_select(1, 4, ZONE_ALL);
	send_add_action(ACTION_MORPH, 500, 64, color);
	send_add_action(ACTION_MORPH, 2000, 64, color);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 2000, 64, 0);
	send_animation_config_play(0);
	send_animation_config_save(1);
	send_animation_set_default(1);
	device_release();
}

// Refactor for spectrum effect
void spectrum(uint16_t duration) {
	device_acquire();
	send_animation_remove(1);
	send_animation_config_start(1);
	send_zone_select(1, 4, ZONE_ALL);
	send_add_action(ACTION_MORPH, duration, 64, 0xFF0000);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFa500);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFFF00);
	send_add_action(ACTION_MORPH, duration, 64, 0x008000);
	send_add_action(ACTION_MORPH, duration, 64, 0x00BFFF);
	send_add_action(ACTION_MORPH, duration, 64, 0x0000FF);
	send_add_action(ACTION_MORPH, duration, 64, 0x800080);
	send_animation_config_save(1);
	send_animation_set_default(1);
	device_release();
}

void wave(uint32_t color) {
	device_acquire();

	send_animation_remove(1);
	send_animation_config_start(1);

	send_zone_select(1, 1, ZONE_LEFT);
	send_add_action(ACTION_MORPH, 500, 64, color);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);

	send_zone_select(1, 1, ZONE_MIDDLE_LEFT);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, color);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);

	send_zone_select(1, 1, ZONE_MIDDLE_RIGHT);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, color);
	send_add_action(ACTION_MORPH, 500, 64, 0);

	send_zone_select(1, 1, ZONE_RIGHT);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, color);
	/*send_animation_config_play(1);*/
	send_animation_config_save(1);
	send_animation_set_default(1);
	device_release();
}

void rainbow(uint16_t duration) {
	device_acquire();

	send_animation_remove(1);
	send_animation_config_start(1);

	send_zone_select(1, 1, ZONE_LEFT);
	send_add_action(ACTION_MORPH, duration, 64, 0xFF0000);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFa500);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFFF00);
	send_add_action(ACTION_MORPH, duration, 64, 0x008000);
	send_add_action(ACTION_MORPH, duration, 64, 0x00BFFF);
	send_add_action(ACTION_MORPH, duration, 64, 0x0000FF);
	send_add_action(ACTION_MORPH, duration, 64, 0x800080);

	send_zone_select(1, 1, ZONE_MIDDLE_LEFT);
	send_add_action(ACTION_MORPH, duration, 64, 0x800080);
	send_add_action(ACTION_MORPH, duration, 64, 0xFF0000);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFa500);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFFF00);
	send_add_action(ACTION_MORPH, duration, 64, 0x008000);
	send_add_action(ACTION_MORPH, duration, 64, 0x00BFFF);
	send_add_action(ACTION_MORPH, duration, 64, 0x0000FF);

	send_zone_select(1, 1, ZONE_MIDDLE_RIGHT);
	send_add_action(ACTION_MORPH, duration, 64, 0x0000FF);
	send_add_action(ACTION_MORPH, duration, 64, 0x800080);
	send_add_action(ACTION_MORPH, duration, 64, 0xFF0000);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFa500);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFFF00);
	send_add_action(ACTION_MORPH, duration, 64, 0x008000);
	send_add_action(ACTION_MORPH, duration, 64, 0x00BFFF);

	send_zone_select(1, 1, ZONE_RIGHT);
	send_add_action(ACTION_MORPH, duration, 64, 0x00BFFF);
	send_add_action(ACTION_MORPH, duration, 64, 0x0000FF);
	send_add_action(ACTION_MORPH, duration, 64, 0x800080);
	send_add_action(ACTION_MORPH, duration, 64, 0xFF0000);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFa500);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFFF00);
	send_add_action(ACTION_MORPH, duration, 64, 0x008000);

	send_animation_config_save(1);
	send_animation_set_default(1);
	/*send_animation_config_play(1);*/

	device_release();
}

void back_and_forth(uint32_t color) {
	device_acquire();

	send_animation_remove(1);
	send_animation_config_start(1);

	send_zone_select(1, 1, ZONE_LEFT);
	send_add_action(ACTION_MORPH, 500, 64, color);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);

	send_zone_select(1, 1, ZONE_MIDDLE_LEFT);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, color);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, color);

	send_zone_select(1, 1, ZONE_MIDDLE_RIGHT);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, color);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, color);
	send_add_action(ACTION_MORPH, 500, 64, 0);

	send_zone_select(1, 1, ZONE_RIGHT);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, color);
	send_add_action(ACTION_MORPH, 500, 64, 0);
	send_add_action(ACTION_MORPH, 500, 64, 0);

	/*send_animation_config_play(1);*/
	send_animation_config_save(1);
	send_animation_set_default(1);
	device_release();
}

void defaultblue(uint32_t color) {
	device_acquire();
	send_animation_remove(1);
	send_animation_config_start(1);
	send_zone_select(1, 4, ZONE_ALL);
	send_add_action(ACTION_COLOR, 1, 2, color);
	send_animation_config_save(1);
	send_animation_set_default(1);
	device_release();
}

void testzones() {
	srand(time(NULL)); // Seed random generator once

	for (uint8_t zone = 0x00; zone <= 0x09; zone++) {
		uint32_t color =
			(rand() % 256 << 16) | (rand() % 256 << 8) | (rand() % 256);

		device_acquire();
		send_animation_remove(1);
		send_animation_config_start(1);

		printf("Setting zone 0x%02X with color #%06X\n", zone, color);
		send_zone_select(1, 4, zone);
		send_add_action(ACTION_COLOR, 1, 2, color);
		send_animation_config_save(1);
		send_animation_set_default(1);

		sleep(5); // Wait 5 seconds
		device_release();
	}
}

void example_spectrum(uint16_t duration) {
	device_acquire();
	send_animation_config_start(0);
	send_zone_select(1, 4, ZONE_ALL);
	send_add_action(ACTION_MORPH, duration, 64, 0xFF0000);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFa500);
	send_add_action(ACTION_MORPH, duration, 64, 0xFFFF00);
	send_add_action(ACTION_MORPH, duration, 64, 0x008000);
	send_add_action(ACTION_MORPH, duration, 64, 0x00BFFF);
	send_add_action(ACTION_MORPH, duration, 64, 0x0000FF);
	send_add_action(ACTION_MORPH, duration, 64, 0x800080);
	send_animation_config_play(0);
	device_release();
}

void example_breathe(uint16_t duration, uint32_t color) {
	device_acquire();
	send_animation_config_start(0);
	send_zone_select(1, 4, ZONE_ALL);
	send_add_action(ACTION_MORPH, duration, 64, color);
	send_add_action(ACTION_MORPH, duration, 64, 0);
	send_animation_config_play(0);
	device_release();
}
