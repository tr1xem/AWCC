#include "include/fans.h"
#include "include/lighting_controls.h"
#include "include/lights.h"
#include "include/thermal_modes.h"
#include "include/supported_devices.h"
#include "include/AWCC.h"
#include "include/AWCCAutoBoost.h"
#include "include/AWCCConfig.h"
#include "include/AWCCControl.h"
#include "include/AWCCSystemLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_mode = 0;

void print_usage(void) { generate_full_help_menu(); }

int main(int argc, char **argv) {
  // Check for --test-mode flag first
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--test-mode") == 0) {
      test_mode = 1;
      printf("Test mode enabled - bypassing device detection and feature validation\n");
      // Remove --test-mode from argv to avoid issues with other command parsing
      for (int j = i; j < argc - 1; j++) {
        argv[j] = argv[j + 1];
      }
      argc--;
      break;
    }
  }

  // Skip device detection in test mode
  if (!test_mode) {
    device_detection_result_t detection_result = detect_device_model();
    
    switch (detection_result) {
      case DEVICE_DETECTION_SUCCESS:
        printf("Device detected: %s\n", get_device_name());
        break;
      case DEVICE_DETECTION_UNSUPPORTED:
        fprintf(stderr, "Error: Unsupported device detected\n");
        fprintf(stderr, "Run 'awcc device-info' for more details\n");
        return 1;
      case DEVICE_DETECTION_ACPI_FAILED:
      case DEVICE_DETECTION_DMI_FAILED:
        fprintf(stderr, "Warning: Device detection failed, using basic functionality\n");
        // Continue with limited functionality
        break;
    }
  } else {
    printf("Skipping device detection in test mode\n");
  }
  
  detectCpuVendor();
  device_open();

  if (argc >= 2) {
    // Device info command
    if (strcmp(argv[1], "device-info") == 0) {
      print_device_info();
    // Try lighting commands first
    } else if (execute_lighting_command(argc - 1, argv + 1) == 0) {
      // Check if lighting is supported (skip in test mode)
      if (!test_mode && !is_feature_supported("lighting") && g_current_device) {
        fprintf(stderr, "Warning: RGB lighting not supported on %s\n", get_device_name());
      }
      // Successfully handled by modular lighting system
    } else if (strcmp(argv[1], "qm") == 0 || strcmp(argv[1], "query") == 0) {
      if (!test_mode && g_current_device && !is_feature_supported("thermal_modes")) {
        fprintf(stderr, "Error: Thermal modes not supported on %s\n", get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      printf("Current mode: %s\n", get_current_mode_name());
    } else if (strcmp(argv[1], "modes") == 0) {
      if (!test_mode && g_current_device && !is_feature_supported("thermal_modes")) {
        fprintf(stderr, "Error: Thermal modes not supported on %s\n", get_device_name());
        device_close();
        return 1;
      }
      list_available_modes();
    } else if (execute_thermal_mode_by_command(argv[1]) == 0) {
      if (!test_mode && g_current_device && !is_feature_supported("thermal_modes")) {
        fprintf(stderr, "Error: Thermal modes not supported on %s\n", get_device_name());
        device_close();
        return 1;
      }
      // Successfully handled by modular thermal mode system
    } else if (strcmp(argv[1], "gt") == 0) {
      if (!test_mode && g_current_device && !is_feature_supported("gmode_toggle")) {
        fprintf(stderr, "Error: G-Mode toggle not supported on %s\n", get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      toggleGMode();

      // Fan boost controls
    } else if (strcmp(argv[1], "cb") == 0 ||
               strcmp(argv[1], "getcpufanboost") == 0) {
      if (!test_mode && g_current_device && !is_feature_supported("fan_boost")) {
        fprintf(stderr, "Error: Fan boost control not supported on %s\n", get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      getFanBoost("cpu");
    } else if (strcmp(argv[1], "gb") == 0 ||
               strcmp(argv[1], "getgpufanboost") == 0) {
      if (!test_mode && g_current_device && !is_feature_supported("fan_boost")) {
        fprintf(stderr, "Error: Fan boost control not supported on %s\n", get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      getFanBoost("gpu");
    } else if (strcmp(argv[1], "scb") == 0 ||
               strcmp(argv[1], "setcpufanboost") == 0) {
      if (!test_mode && g_current_device && !is_feature_supported("fan_boost")) {
        fprintf(stderr, "Error: Fan boost control not supported on %s\n", get_device_name());
        device_close();
        return 1;
      }
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
      if (!test_mode && g_current_device && !is_feature_supported("fan_boost")) {
        fprintf(stderr, "Error: Fan boost control not supported on %s\n", get_device_name());
        device_close();
        return 1;
      }
      if (argc < 3) {
        fprintf(stderr, "error: missing value for GPU fan boost\n");
        device_close();
        return 1;
      }
      int value = atoi(argv[2]);
      checkRoot(argv[1], argv);
      setFanBoost("gpu", value);
    } else if (strcmp(argv[1], "autoboost") == 0) {
      if (!test_mode && g_current_device && !is_feature_supported("autoboost")) {
        fprintf(stderr, "Error: AutoBoost not supported on %s\n", get_device_name());
        device_close();
        return 1;
      }
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
  cleanup_device_detection();
  return 0;
}
