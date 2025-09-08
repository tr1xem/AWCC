#include "include/effects.h"
#include "include/fans.h"
#include "include/lights.h"
#include "include/thermal_modes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void print_usage(void) { generate_full_help_menu(); }

int main(int argc, char **argv) {
  detectCpuVendor();
  device_open();

  if (argc >= 2) {

    if (!strcmp(argv[1], "brightness") && argc >= 3) {
      uint8_t value = 100 - atoi(argv[2]);
      if (value > 100) {
        fprintf(stderr, "error: brightness value must be between 0 and 100\n");
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
    } else if (strcmp(argv[1], "qm") == 0 || strcmp(argv[1], "query") == 0) {

      checkRoot(argv[1], argv);
      printf("Current mode: %s\n", get_current_mode_name());
    } else if (strcmp(argv[1], "modes") == 0) {
      list_available_modes();
    } else if (execute_thermal_mode_by_command(argv[1]) == 0) {
      // Successfully handled by modular thermal mode system
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
