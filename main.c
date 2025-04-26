#include "include/effects.h"
#include "include/fans.h"
#include "include/lights.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void print_usage(void) {
	printf("\nAlienware Command Center for Dell G Series\n");
	printf("==========================================\n\n");
	printf("Usage:\n");
	printf("  awcc [command] [arguments]...\n\n");

	printf("Lighting Controls:\n");
	printf("  brightness <value>     Set keyboard brightness (0-100)\n");
	printf("  static <color>         Set static color (hex RGB)\n");
	printf("  breathe <color>        Breathing color effect\n");
	printf("  wave <color>           Wave color effect\n");
	printf("  bkf <color>            Back-and-forth color effect\n");
	printf("  rainbow <duration>     Rainbow spectrum cycle (ms)\n");
	printf("  spectrum <duration>    Full color cycle (ms)\n");
	printf("  defaultblue            Set default static blue color\n\n");

	printf("Fan Controls (Run as root):\n");
	printf("  qm                     Query current fan mode\n");
	printf("  g                      Set G-Mode\n");
	printf("  q                      Set Quiet Mode\n");
	printf("  p                      Set Performance Mode\n");
	printf("  b                      Set Balanced Mode\n");
	printf("  bs                     Set Battery Saver Mode\n");
	printf("  gt                     Toggle G-Mode (useful for keybinds)\n\n");

	printf("Fan Boost Controls (Run as root):\n");
	printf("  cb                      Get CPU fan boost\n");
	printf("  gb                      Get GPU fan boost\n");
	printf("  scb <value>             Set CPU fan boost (1-100)\n");
	printf("  sgb <value>             Set GPU fan boost (1-100)\n\n");
}

int main(int argc, char **argv) {
	detectCpuVendor();
	device_open();

	if (argc >= 2) {

		if (!strcmp(argv[1], "brightness") && argc >= 3) {
			uint8_t value = 100 - atoi(argv[2]);
			if (value > 100) {
				fprintf(stderr,
						"error: brightness value must be between 0 and 100\n");
				device_close();
				return 1;
			}
			brightness(value);
			char message[256]; // Make sure the buffer is large enough
			sprintf(message, "Set Brightness to %d%%", atoi(argv[2]));
		} else if (!strcmp(argv[1], "static")) {
			uint32_t color = strtol(argv[2], NULL, 16);
			if (color == 0) {
				fprintf(stderr, "error: invalid color %s\n", argv[2]);
				device_close();
				return 1;
			}
			static_color(color);
		} else if (!strcmp(argv[1], "spectrum") && argc >= 3) {
			uint16_t duration = strtol(argv[2], NULL, 10);
			if (duration == 0) {
				fprintf(stderr, "error: invalid duration %s\n", argv[2]);
				device_close();
				return 1;
			}
			spectrum(duration);
		} else if (!strcmp(argv[1], "breathe") && argc >= 3) {
			uint32_t color = strtol(argv[2], NULL, 16);
			if (color == 0) {
				fprintf(stderr, "error: invalid color %s\n", argv[2]);
				device_close();
				return 1;
			}
			breathe(color);
		} else if (!strcmp(argv[1], "rainbow") && argc >= 3) {
			uint16_t duration = strtol(argv[2], NULL, 10);
			if (duration == 0) {
				fprintf(stderr, "error: invalid duration %s\n", argv[2]);
				device_close();
				return 1;
			}
			rainbow(duration);
		} else if (!strcmp(argv[1], "wave") && argc >= 3) {

			uint32_t color = strtol(argv[2], NULL, 16);
			if (color == 0) {
				fprintf(stderr, "error: invalid color %s\n", argv[2]);
				device_close();
				return 1;
			}
			wave(color);
		} else if (!strcmp(argv[1], "bkf") && argc >= 3) {

			uint32_t color = strtol(argv[2], NULL, 16);
			if (color == 0) {
				fprintf(stderr, "error: invalid color %s\n", argv[2]);
				device_close();
				return 1;
			}
			back_and_forth(color);
		} else if (!strcmp(argv[1], "defaultblue")) {
			defaultblue(0x00FFFF);

			// Fan control
		} else if (strcmp(argv[1], "qm") == 0 ||
				   strcmp(argv[1], "query") == 0) {

			checkRoot(argv[1], argv);
			printCurrentMode();
		} else if (strcmp(argv[1], "q") == 0 || strcmp(argv[1], "quiet") == 0) {

			checkRoot(argv[1], argv);
			quietMode();
		} else if (strcmp(argv[1], "bs") == 0 ||
				   strcmp(argv[1], "battery") == 0) {

			checkRoot(argv[1], argv);
			batteryMode();
		} else if (strcmp(argv[1], "b") == 0 ||
				   strcmp(argv[1], "balance") == 0) {

			checkRoot(argv[1], argv);
			balanceMode();
		} else if (strcmp(argv[1], "p") == 0 ||
				   strcmp(argv[1], "performance") == 0) {

			checkRoot(argv[1], argv);
			performanceMode();

		} else if (strcmp(argv[1], "g") == 0 || strcmp(argv[1], "gmode") == 0) {

			checkRoot(argv[1], argv);
			gamingMode();
		} else if (strcmp(argv[1], "gt") == 0) {
			checkRoot(argv[1], argv);
			toggleGMode();
		} else if (strcmp(argv[1], "cb") == 0 ||
				   strcmp(argv[1], "getcpufanboost") == 0) {
			checkRoot(argv[1], argv);
			getFanBoost("cpu");

		} else if (strcmp(argv[1], "gb") == 0 ||
				   strcmp(argv[1], "getgpufanboost") == 0) {
			checkRoot(argv[1], argv);
			getFanBoost("gpu");

		} else if (strcmp(argv[1], "scb") == 0 ||
				   strcmp(argv[1], "setcpufanboost") == 0) {
			if (argc < 3) {
				fprintf(stderr, "error: missing value for CPU fan boost\n");
				device_close();
				return 1;
			}
			int value = atoi(argv[2]);
			checkRoot(argv[1], argv);
			setFanBoost("cpu", value);
		} else if (strcmp(argv[1], "sgb") == 0 ||
				   strcmp(argv[1], "setgpufanboost") == 0) {
			if (argc < 3) {
				fprintf(stderr, "error: missing value for GPU fan boost\n");
				device_close();
				return 1;
			}
			int value = atoi(argv[2]);
			checkRoot(argv[1], argv);
			setFanBoost("gpu", value);
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
