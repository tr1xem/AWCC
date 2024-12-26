#include "include/lights.h"
#include "include/fans.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void wave(uint32_t color) {
	device_acquire();
	send_animation_config_start(0);

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

	send_animation_config_play(0);
	device_release();
}

void rainbow(uint16_t duration) {
	device_acquire();
	send_animation_config_start(0);

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

	send_animation_config_play(0);
	device_release();
}

void back_and_forth(uint32_t color) {
	device_acquire();
	send_animation_config_start(0);

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

	send_animation_config_play(0);
	device_release();
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

void example_static(uint32_t color) {
	device_acquire();
	send_animation_config_start(0);
	send_zone_select(1, 4, ZONE_ALL);
	send_add_action(ACTION_COLOR, 1, 2, color);
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

void print_usage(void) {
	printf("Usage: humanfx [command] [arguments]...\n");
	printf("Alienware 187c:0550/187c:0551 controller\n");
	printf("\n");
	printf("  brightness <value>\tSet brightness\n");
	printf("  rainbow <duration>\tSet Rainbows Spectrum\n");
	printf("  wave <color>\tSet Wave Effect\n");
	printf("  bkf <color>\tSet Back and fourth effect\n");
	printf("  static <color>\tStatic color\n");
	printf("  breathe <color>\tIt lives and breathes!\n");
	printf("  spectrum <duration>\tCycles through all colors\n");
	printf("\n");
}

int main(int argc, char **argv) {
	device_open();

	if (argc > 2) {
		if (!strcmp(argv[1], "brightness")) {
			uint8_t value = 100 - atoi(argv[2]);
			if (value > 100) {
				fprintf(stderr,
						"error: brightness value must be between 0 and 100\n");
				device_close();
				return 1;
			}
			device_acquire();
			send_set_dim(value, 4, ZONE_ALL);
			device_release();
		} else if (!strcmp(argv[1], "static")) {
			uint32_t color = strtol(argv[2], NULL, 16);
			if (color == 0) {
				fprintf(stderr, "error: invalid color %s\n", argv[2]);
				device_close();
				return 1;
			}
			device_acquire();
			send_animation_remove(1);
			send_animation_config_start(1);
			send_zone_select(1, 4, ZONE_ALL);
			send_add_action(ACTION_COLOR, 1, 2, color);
			send_animation_config_save(1);
			send_animation_set_default(1);
			device_release();
		} else if (!strcmp(argv[1], "spectrum")) {
			uint16_t duration = strtol(argv[2], NULL, 10);
			if (duration == 0) {
				fprintf(stderr, "error: invalid duration %s\n", argv[2]);
				device_close();
				return 1;
			}
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
		} else if (!strcmp(argv[1], "breathe")) {
			uint32_t color = strtol(argv[2], NULL, 16);
			if (color == 0) {
				fprintf(stderr, "error: invalid color %s\n", argv[2]);
				device_close();
				return 1;
			}
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
		} else if (!strcmp(argv[1], "rainbow")) {
			uint16_t duration = strtol(argv[2], NULL, 10);
			if (duration == 0) {
				fprintf(stderr, "error: invalid duration %s\n", argv[2]);
				device_close();
				return 1;
			}
			rainbow(duration);
		} else if (!strcmp(argv[1], "wave")) {

			uint32_t color = strtol(argv[2], NULL, 16);
			if (color == 0) {
				fprintf(stderr, "error: invalid color %s\n", argv[2]);
				device_close();
				return 1;
			}
			wave(color);
		} else if (!strcmp(argv[1], "bkf")) {

			uint32_t color = strtol(argv[2], NULL, 16);
			if (color == 0) {
				fprintf(stderr, "error: invalid color %s\n", argv[2]);
				device_close();
				return 1;
			}
			back_and_forth(color);
		}

		else if (strcmp(argv[1], "q") == 0 || strcmp(argv[1], "quiet") == 0) {
			quietMode();
		} else if (strcmp(argv[1], "p") == 0 ||
				   strcmp(argv[1], "performance") == 0) {
			performanceMode();
		} else if (strcmp(argv[1], "g") == 0 || strcmp(argv[1], "gmode") == 0) {
			gamingMode();
		} else if (strcmp(argv[1], "gt") == 0 && argc >= 3) {
			int threshold = (argc >= 4)
								? atoi(argv[3])
								: 3000; // Default to 3000 RPM if not provided
			toggleGMode(argv[2], threshold);
			// Reset lighting after mode switch if required
		} else {
			print_usage();
		}
	} else {
		print_usage();
	}

	// Example temporary effects
	// example_wave(0xFFFFFF);
	// example_rainbow(500);
	// example_back_and_forth(0xFFFFFF);
	// example_spectrum(1000);
	// example_static(0xFF00FF);
	// example_breathe(2500, 0xFFFFFF);

	// Example setting as default
	// device_acquire();
	// send_animation_remove(1);
	// send_animation_config_start(1);
	// send_zone_select(1, 4, ZONE_ALL);
	// send_add_action(ACTION_COLOR, 1, 2, 0xFFFFFF);
	// send_animation_config_save(1);
	// send_animation_set_default(1);
	// device_release();

	// Example of reading data from device
	// device_acquire();
	// send_request_animation_count();
	// uint8_t ret[33] = {0};
	// device_receive(ret, sizeof(ret));
	// printf("[ ");
	// for (uint16_t i = 0; i < sizeof(ret); i++)
	// 	printf("%02x ", ret[i]);
	// printf("]\n");
	// device_release();

	// Set brightness
	// device_acquire();
	// send_set_dim(BRIGHTNESS_FULL, 4, ZONE_ALL);
	// device_release();

	device_close();
	return 0;
}
