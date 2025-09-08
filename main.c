#include "include/fans.h"
#include "include/lighting_controls.h"
#include "include/lights.h"
#include "include/thermal_modes.h"
#include "include/AWCC.h"
#include "include/AWCCAutoBoost.h"
#include "include/AWCCConfig.h"
#include "include/AWCCControl.h"
#include "include/AWCCSystemLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(void) { generate_full_help_menu(); }

int main(int argc, char **argv) {
  detectCpuVendor();
  device_open();

  if (argc >= 2) {
    // Try lighting commands first
    if (execute_lighting_command(argc - 1, argv + 1) == 0) {
      // Successfully handled by modular lighting system
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

      // Fan boost controls
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
    } else if (strcmp(argv[1], "autoboost") == 0) {
      checkRoot(argv[1], argv);
      AWCC.Initialize();
      struct AWCCConfig_t conf_ac = AWCCDefaultConfigAC();
      struct AWCCConfig_t conf_bat = AWCCDefaultConfigBAT();
      AWCCAutoBoost.Start(&conf_ac, &conf_bat, &AWCCSystemLoggerDefault, &AWCCControlDefault);
      AWCC.Deinitialize();
    } else {
      print_usage();
    }
  } else {
    print_usage();
  }

  device_close();
  return 0;
}
