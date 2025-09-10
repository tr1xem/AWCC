#include "include/AWCC.h"
#include "include/AWCCAutoBoost.h"
#include "include/AWCCConfig.h"
#include "include/AWCCControl.h"
#include "include/AWCCDaemon.h"
#include "include/AWCCSystemLogger.h"
#include "include/lighting_controls.h"
#include "include/lights.h"
#include "include/supported_devices.h"
#include "include/thermal_modes.h"
#include "src/AWCCUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for getuid, access, execvp

int test_mode = 0;

// Helper function to map command strings to AWCC modes
enum AWCCMode_t command_to_awcc_mode(const char *command) {
  if (strcmp(command, "q") == 0 || strcmp(command, "quiet") == 0) {
    return AWCCModeQuiet;
  } else if (strcmp(command, "p") == 0 || strcmp(command, "performance") == 0) {
    return AWCCModePerformance;
  } else if (strcmp(command, "b") == 0 || strcmp(command, "balance") == 0) {
    return AWCCModeBalanced;
  } else if (strcmp(command, "g") == 0 || strcmp(command, "gmode") == 0) {
    return AWCCModeG;
  } else if (strcmp(command, "bs") == 0 || strcmp(command, "battery") == 0) {
    return AWCCModeBatterySaver;
  }
  return -1; // Invalid mode
}

// Helper function to check if command is a thermal mode command
int is_thermal_mode_command(const char *command) {
  enum AWCCMode_t mode = command_to_awcc_mode(command);
  return mode != (enum AWCCMode_t)(-1);
}

// Helper function to validate command feature support
int validate_command_features(const char *command) {
  if (test_mode || !g_current_device) {
    return 0; // Skip validation in test mode or if no device detected
  }

  // Thermal mode commands
  if (is_thermal_mode_command(command)) {
    if (!is_feature_supported("thermal_modes")) {
      fprintf(stderr, "Error: Thermal modes not supported on %s\n", get_device_name());
      return 1;
    }
    if (!is_thermal_mode_supported(command)) {
      fprintf(stderr, "Error: Thermal mode '%s' not supported on %s\n", command, get_device_name());
      return 1;
    }
  }
  // Query and modes commands
  else if (strcmp(command, "qm") == 0 || strcmp(command, "query") == 0 || strcmp(command, "modes") == 0) {
    if (!is_feature_supported("thermal_modes")) {
      fprintf(stderr, "Error: Thermal modes not supported on %s\n", get_device_name());
      return 1;
    }
  }
  // G-mode toggle
  else if (strcmp(command, "gt") == 0) {
    if (!is_feature_supported("gmode_toggle")) {
      fprintf(stderr, "Error: G-Mode toggle not supported on %s\n", get_device_name());
      return 1;
    }
    if (!is_thermal_mode_supported("gmode")) {
      fprintf(stderr, "Error: G-Mode thermal mode not supported on %s\n", get_device_name());
      return 1;
    }
  }
  // Fan boost commands
  else if (strcmp(command, "cb") == 0 || strcmp(command, "getcpufanboost") == 0 ||
           strcmp(command, "gb") == 0 || strcmp(command, "getgpufanboost") == 0 ||
           strcmp(command, "sb") == 0 || strcmp(command, "setcpufanboost") == 0 ||
           strcmp(command, "sgb") == 0 || strcmp(command, "setgpufanboost") == 0 ||
           strcmp(command, "fan") == 0 || strcmp(command, "fanspeed") == 0 ||
           strcmp(command, "fanname") == 0) {
    if (!is_feature_supported("fan_boost")) {
      fprintf(stderr, "Error: Fan boost control not supported on %s\n", get_device_name());
      return 1;
    }
  }
  // AutoBoost commands
  else if (strncmp(command, "autoboost", 9) == 0) {
    if (!is_feature_supported("autoboost")) {
      fprintf(stderr, "Error: AutoBoost not supported on %s\n", get_device_name());
      return 1;
    }
  }
  // Lighting commands are handled by execute_lighting_command with their own validation

  return 0; // Command is supported
}

void print_usage(void) { generate_full_help_menu(); }

int execute_via_daemon(int argc, char **argv) {
  struct AWCCCommand_t cmd = {0};
  struct AWCCResponse_t response = {0};

  if (argc < 2) {
    print_usage();
    return 0;
  }

  // Map command line arguments to daemon commands
  if (strcmp(argv[1], "qm") == 0 || strcmp(argv[1], "query") == 0) {
    cmd.command_type = AWCC_CMD_GET_MODE;
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      int mode = atoi(response.data);
      const char *mode_names[] = {"Quiet", "Battery Saver", "Balanced",
                                  "Performance", "G-Mode"};
      printf("Current mode: %s\n", mode_names[mode]);
    }
  } else if (strcmp(argv[1], "modes") == 0) {
    cmd.command_type = AWCC_CMD_LIST_MODES;
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0) {
      printf("%s\n", response.data);
    }
  } else if (is_thermal_mode_command(argv[1])) {
    cmd.command_type = AWCC_CMD_SET_MODE;
    enum AWCCMode_t mode = command_to_awcc_mode(argv[1]);
    snprintf(cmd.args, sizeof(cmd.args), "%d", mode);
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("%s\n", response.data);
    }
  } else if (strcmp(argv[1], "gt") == 0) {
    cmd.command_type = AWCC_CMD_TOGGLE_GMODE;
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0) {
      printf("%s\n", response.data);
    }
  } else if (strcmp(argv[1], "cb") == 0 ||
             strcmp(argv[1], "getcpufanboost") == 0) {
    cmd.command_type = AWCC_CMD_GET_FAN_BOOST;
    strcpy(cmd.args, "cpu");
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("CPU fan boost: %s%%\n", response.data);
    }
  } else if (strcmp(argv[1], "gb") == 0 ||
             strcmp(argv[1], "getgpufanboost") == 0) {
    cmd.command_type = AWCC_CMD_GET_FAN_BOOST;
    strcpy(cmd.args, "gpu");
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("GPU fan boost: %s%%\n", response.data);
    }
  } else if (strcmp(argv[1], "scb") == 0 ||
             strcmp(argv[1], "setcpufanboost") == 0) {
    if (argc < 3) {
      fprintf(stderr, "error: missing value for CPU fan boost\n");
      return 1;
    }
    cmd.command_type = AWCC_CMD_SET_FAN_BOOST;
    snprintf(cmd.args, sizeof(cmd.args), "cpu %s", argv[2]);
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("%s\n", response.data);
    }
  } else if (strcmp(argv[1], "sgb") == 0 ||
             strcmp(argv[1], "setgpufanboost") == 0) {
    if (argc < 3) {
      fprintf(stderr, "error: missing value for GPU fan boost\n");
      return 1;
    }
    cmd.command_type = AWCC_CMD_SET_FAN_BOOST;
    snprintf(cmd.args, sizeof(cmd.args), "gpu %s", argv[2]);
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("%s\n", response.data);
    }
  } else if (strcmp(argv[1], "cr") == 0 ||
             strcmp(argv[1], "getcpufanrpm") == 0) {
    cmd.command_type = AWCC_CMD_GET_FAN_RPM;
    strcpy(cmd.args, "cpu");
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("CPU fan RPM: %s\n", response.data);
    }
  } else if (strcmp(argv[1], "gr") == 0 ||
             strcmp(argv[1], "getgpufanrpm") == 0) {
    cmd.command_type = AWCC_CMD_GET_FAN_RPM;
    strcpy(cmd.args, "gpu");
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("GPU fan RPM: %s\n", response.data);
    }
  } else if (strcmp(argv[1], "cfn") == 0 ||
             strcmp(argv[1], "getcpufanname") == 0) {
    cmd.command_type = AWCC_CMD_GET_FAN_NAME;
    strcpy(cmd.args, "cpu");
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("CPU fan name: %s\n", response.data);
    }
  } else if (strcmp(argv[1], "gfn") == 0 ||
             strcmp(argv[1], "getgpufanname") == 0) {
    cmd.command_type = AWCC_CMD_GET_FAN_NAME;
    strcpy(cmd.args, "gpu");
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("GPU fan name: %s\n", response.data);
    }
  } else if (strcmp(argv[1], "fans") == 0) {
    cmd.command_type = AWCC_CMD_FAN_STATUS;
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("Fan Status:\n%s\n", response.data);
    }
  } else if (strcmp(argv[1], "device-info") == 0) {
    // Device-info always uses direct execution to ensure proper device detection
    return -1; // Fallback to direct execution
  } else if (strcmp(argv[1], "autoboost") == 0) {
    cmd.command_type = AWCC_CMD_AUTOBOOST;
    // Include any additional arguments for autoboost
    if (argc > 2) {
      snprintf(cmd.args, sizeof(cmd.args), "%s", argv[2]);
    } else {
      strcpy(cmd.args, "status"); // default to status if no args
    }
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("%s\n", response.data);
    }
  } else if (strncmp(argv[1], "lights", 6) == 0 ||
             strcmp(argv[1], "lighting") == 0) {
    cmd.command_type = AWCC_CMD_LIGHTING;
    // Include the full command and arguments for lighting
    size_t pos = 0;
    for (int i = 1; i < argc && pos < sizeof(cmd.args) - 1; i++) {
      if (i > 1)
        cmd.args[pos++] = ' ';
      size_t len = strlen(argv[i]);
      if (pos + len < sizeof(cmd.args) - 1) {
        strcpy(cmd.args + pos, argv[i]);
        pos += len;
      }
    }
    cmd.response_needed = 1;

    if (awcc_daemon_send_command(&cmd, &response) == 0 &&
        response.status == 0) {
      printf("%s\n", response.data);
    }
  } else {
    printf("Command '%s' not supported via daemon, falling back to direct "
           "execution\n",
           argv[1]);
    return -1; // Indicate fallback needed
  }

  if (response.status != 0) {
    fprintf(stderr, "Error: %s\n", response.data);
    return 1;
  }

  return 0;
}

int main(int argc, char **argv) {
  int use_daemon = 0;

  // Check for --test-mode flag first
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--test-mode") == 0) {
      test_mode = 1;
      printf("Test mode enabled - bypassing device detection and feature "
             "validation\n");
      // Remove --test-mode from argv to avoid issues with other command parsing
      for (int j = i; j < argc - 1; j++) {
        argv[j] = argv[j + 1];
      }
      argc--;
      break;
    }
  }

  // EARLY VALIDATION PHASE - Run device detection before any command execution
  // Skip only for test mode and device-info command
  if (!test_mode && argc >= 2 && strcmp(argv[1], "device-info") != 0) {
    device_detection_result_t detection_result = detect_device_model();
    
    switch (detection_result) {
    case DEVICE_DETECTION_SUCCESS:
      printf("Device detected: %s\n", get_device_name());
      
      // Level 2: Feature validation for supported devices
      if (validate_command_features(argv[1]) != 0) {
        return 1; // Feature not supported
      }
      break;
    case DEVICE_DETECTION_UNSUPPORTED:
      fprintf(stderr, "Error: Unsupported device detected\n");
      fprintf(stderr, "Run 'awcc device-info' for more details\n");
      return 1;
    case DEVICE_DETECTION_ACPI_FAILED:
    case DEVICE_DETECTION_DMI_FAILED:
      fprintf(stderr, "Warning: Device detection failed, using basic functionality\n");
      // Continue with limited functionality - skip feature validation
      break;
    }
  }

  // Device detection for device-info command (always runs regardless of support status)
  if (!test_mode && argc >= 2 && strcmp(argv[1], "device-info") == 0) {
    // Check if we need sudo for ACPI detection
    if (getuid() != 0 && access("/proc/acpi/call", W_OK) != 0) {
      printf("ACPI detection requires root privileges. Re-running with sudo...\n");
      
      // Rebuild command with sudo
      char **sudo_args = malloc((argc + 2) * sizeof(char*));
      if (!sudo_args) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
      }
      
      sudo_args[0] = "sudo";
      sudo_args[1] = argv[0];  // Original program path
      for (int i = 1; i < argc; i++) {
        sudo_args[i + 1] = argv[i];
      }
      sudo_args[argc + 1] = NULL;
      
      execvp("sudo", sudo_args);
      perror("Failed to execute with sudo");
      free(sudo_args);
      exit(1);
    }
    
    detect_device_model(); // Run detection but don't exit on unsupported - let device-info handle the display
  }

  // Check if daemon is running and we should use it
  if (!test_mode && awcc_daemon_is_running()) {
    use_daemon = 1;
  }

  // If using daemon, try daemon execution first
  if (use_daemon && argc >= 2) {
    int daemon_result = execute_via_daemon(argc, argv);
    if (daemon_result != -1) {
      // Command was handled by daemon (successfully or with error)
      return daemon_result;
    }
    // If daemon_result == -1, fallback to direct execution
    printf("Falling back to direct execution\n");
  }

  // Skip device detection in test mode
  if (!test_mode) {
    // Device-info no longer needs privilege elevation for diagnostic purposes
  } else {
    printf("Skipping device detection in test mode\n");
  }

  device_open();

  // Initialize AWCC interface
  AWCC.Initialize();

  if (argc >= 2) {
    // Device info command
    if (strcmp(argv[1], "device-info") == 0) {
      print_device_info();
      // Try lighting commands first
    } else if (execute_lighting_command(argc - 1, argv + 1) == 0) {
      // Check if lighting is supported (skip in test mode)
      if (!test_mode && !is_feature_supported("lighting") && g_current_device) {
        fprintf(stderr, "Warning: RGB lighting not supported on %s\n",
                get_device_name());
      }
      // Successfully handled by modular lighting system
    } else if (strcmp(argv[1], "qm") == 0 || strcmp(argv[1], "query") == 0) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("thermal_modes")) {
        fprintf(stderr, "Error: Thermal modes not supported on %s\n",
                get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      printf("Current mode: %s\n", AWCC.GetModeName(AWCC.GetMode()));
    } else if (strcmp(argv[1], "modes") == 0) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("thermal_modes")) {
        fprintf(stderr, "Error: Thermal modes not supported on %s\n",
                get_device_name());
        device_close();
        return 1;
      }
      list_available_modes();
    } else if (is_thermal_mode_command(argv[1])) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("thermal_modes")) {
        fprintf(stderr, "Error: Thermal modes not supported on %s\n",
                get_device_name());
        device_close();
        return 1;
      }
      // Check if this specific thermal mode is supported
      if (!test_mode && g_current_device &&
          !is_thermal_mode_supported(argv[1])) {
        fprintf(stderr, "Error: Thermal mode '%s' not supported on %s\n",
                argv[1], get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      enum AWCCMode_t mode = command_to_awcc_mode(argv[1]);
      AWCC.SetMode(mode);
      printf("%s mode activated.\n", AWCC.GetModeName(mode));
    } else if (strcmp(argv[1], "gt") == 0) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("gmode_toggle")) {
        fprintf(stderr, "Error: G-Mode toggle not supported on %s\n",
                get_device_name());
        device_close();
        return 1;
      }
      // Also check if gmode thermal mode is supported
      if (!test_mode && g_current_device &&
          !is_thermal_mode_supported("gmode")) {
        fprintf(stderr, "Error: G-Mode thermal mode not supported on %s\n",
                get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      AWCC.ToggleGMode();

      // Fan boost controls
    } else if (strcmp(argv[1], "cb") == 0 ||
               strcmp(argv[1], "getcpufanboost") == 0) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("fan_boost")) {
        fprintf(stderr, "Error: Fan boost control not supported on %s\n",
                get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      printf("CPU fan boost: %d%%\n", AWCC.GetFanBoost(AWCCFanCPU));
    } else if (strcmp(argv[1], "gb") == 0 ||
               strcmp(argv[1], "getgpufanboost") == 0) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("fan_boost")) {
        fprintf(stderr, "Error: Fan boost control not supported on %s\n",
                get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      printf("GPU fan boost: %d%%\n", AWCC.GetFanBoost(AWCCFanGPU));
    } else if (strcmp(argv[1], "scb") == 0 ||
               strcmp(argv[1], "setcpufanboost") == 0) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("fan_boost")) {
        fprintf(stderr, "Error: Fan boost control not supported on %s\n",
                get_device_name());
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
      AWCC.SetFanBoost(AWCCFanCPU, value);
      printf("CPU fan boost set to %d%%\n", value);
    } else if (strcmp(argv[1], "sgb") == 0 ||
               strcmp(argv[1], "setgpufanboost") == 0) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("fan_boost")) {
        fprintf(stderr, "Error: Fan boost control not supported on %s\n",
                get_device_name());
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
      AWCC.SetFanBoost(AWCCFanGPU, value);
      printf("GPU fan boost set to %d%%\n", value);

      // Fan RPM and Name Query Commands
    } else if (strcmp(argv[1], "cr") == 0 ||
               strcmp(argv[1], "getcpufanrpm") == 0) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("fan_boost")) {
        fprintf(stderr, "Error: Fan RPM query not supported on %s\n",
                get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      printf("CPU fan RPM: %d\n", AWCC.GetFanRpm(AWCCFanCPU));
    } else if (strcmp(argv[1], "gr") == 0 ||
               strcmp(argv[1], "getgpufanrpm") == 0) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("fan_boost")) {
        fprintf(stderr, "Error: Fan RPM query not supported on %s\n",
                get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      printf("GPU fan RPM: %d\n", AWCC.GetFanRpm(AWCCFanGPU));
    } else if (strcmp(argv[1], "cfn") == 0 ||
               strcmp(argv[1], "getcpufanname") == 0) {
      printf("CPU fan name: %s\n", AWCC.GetFanName(AWCCFanCPU));
    } else if (strcmp(argv[1], "gfn") == 0 ||
               strcmp(argv[1], "getgpufanname") == 0) {
      printf("GPU fan name: %s\n", AWCC.GetFanName(AWCCFanGPU));
    } else if (strcmp(argv[1], "fans") == 0) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("fan_boost")) {
        fprintf(stderr, "Error: Fan information not supported on %s\n",
                get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      printf("Fan Status:\n");
      printf("  %s: %d RPM, %d%% boost\n", AWCC.GetFanName(AWCCFanCPU),
             AWCC.GetFanRpm(AWCCFanCPU), AWCC.GetFanBoost(AWCCFanCPU));
      printf("  %s: %d RPM, %d%% boost\n", AWCC.GetFanName(AWCCFanGPU),
             AWCC.GetFanRpm(AWCCFanGPU), AWCC.GetFanBoost(AWCCFanGPU));
    } else if (strcmp(argv[1], "autoboost") == 0) {
      if (!test_mode && g_current_device &&
          !is_feature_supported("autoboost")) {
        fprintf(stderr, "Error: AutoBoost not supported on %s\n",
                get_device_name());
        device_close();
        return 1;
      }
      checkRoot(argv[1], argv);
      AWCC.Initialize();
      struct AWCCConfig_t conf_ac = AWCCDefaultConfigAC();
      struct AWCCConfig_t conf_bat = AWCCDefaultConfigBAT();
      AWCCAutoBoost.Start(&conf_ac, &conf_bat, &AWCCSystemLoggerDefault,
                          &AWCCControlDefault);
      AWCC.Deinitialize();
    } else {
      print_usage();
    }
  } else {
    print_usage();
  }

  AWCC.Deinitialize();
  device_close();
  cleanup_device_detection();
  return 0;
}
