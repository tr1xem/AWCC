#define _GNU_SOURCE
#include "thermal_modes.h"
#include "AWCCUtils.h"
#include "supported_devices.h"
#include "lighting_controls.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//==============================================================================
// Global State
//==============================================================================

static device_capabilities_t g_device_caps;
static thermal_mode_t g_thermal_modes[THERMAL_MODE_COUNT];
static bool g_initialized = false;

//==============================================================================
// Mode Definitions
//==============================================================================

static void initialize_mode_definitions(void) {
  g_thermal_modes[THERMAL_MODE_QUIET] =
      (thermal_mode_t){.id = THERMAL_MODE_QUIET,
                       .name = "Quiet",
                       .hex_code = "0xa3",
                       .description = "Quiet mode for minimal noise",
                       .cli_command = "quiet",
                       .cli_alias = "q",
                       .enabled = true,
                       .requires_root = true,
                       .pre_check = NULL,
                       .post_action = NULL};

  g_thermal_modes[THERMAL_MODE_BATTERY_SAVER] =
      (thermal_mode_t){.id = THERMAL_MODE_BATTERY_SAVER,
                       .name = "Battery Saver",
                       .hex_code = "0xa5",
                       .description = "Battery saving mode",
                       .cli_command = "battery",
                       .cli_alias = "bs",
                       .enabled = true,
                       .requires_root = true,
                       .pre_check = NULL,
                       .post_action = NULL};

  g_thermal_modes[THERMAL_MODE_BALANCED] =
      (thermal_mode_t){.id = THERMAL_MODE_BALANCED,
                       .name = "Balanced",
                       .hex_code = "0xa0",
                       .description = "Balanced performance and power",
                       .cli_command = "balance",
                       .cli_alias = "b",
                       .enabled = true,
                       .requires_root = true,
                       .pre_check = NULL,
                       .post_action = NULL};

  g_thermal_modes[THERMAL_MODE_PERFORMANCE] =
      (thermal_mode_t){.id = THERMAL_MODE_PERFORMANCE,
                       .name = "Performance",
                       .hex_code = "0xa1",
                       .description = "High performance mode",
                       .cli_command = "performance",
                       .cli_alias = "p",
                       .enabled = true,
                       .requires_root = true,
                       .pre_check = NULL,
                       .post_action = NULL};

  g_thermal_modes[THERMAL_MODE_GAMING] =
      (thermal_mode_t){.id = THERMAL_MODE_GAMING,
                       .name = "Gaming",
                       .hex_code = "0xab",
                       .description = "Gaming mode (G-Mode)",
                       .cli_command = "gmode",
                       .cli_alias = "g",
                       .enabled = true,
                       .requires_root = true,
                       .pre_check = NULL,
                       .post_action = NULL};
}

//==============================================================================
// Device Capability Detection
//==============================================================================

device_capabilities_t detect_device_capabilities(void) {
  device_capabilities_t caps = {.supports_quiet = true,
                                .supports_gaming = true,
                                .supports_battery_saver = true,
                                .supports_performance = true,
                                .supports_balanced = true,
                                .has_gpu_fan_control = true,
                                .has_cpu_fan_control = true,
                                .acpi_prefix = "AMWW"};


  extern const char *acpi_prefix;
  caps.acpi_prefix = acpi_prefix;

  return caps;
}

static void apply_device_capabilities(void) {
  g_thermal_modes[THERMAL_MODE_QUIET].enabled = g_device_caps.supports_quiet;
  g_thermal_modes[THERMAL_MODE_BATTERY_SAVER].enabled =
      g_device_caps.supports_battery_saver;
  g_thermal_modes[THERMAL_MODE_BALANCED].enabled =
      g_device_caps.supports_balanced;
  g_thermal_modes[THERMAL_MODE_PERFORMANCE].enabled =
      g_device_caps.supports_performance;
  g_thermal_modes[THERMAL_MODE_GAMING].enabled = g_device_caps.supports_gaming;
}

//==============================================================================
// Initialization
//==============================================================================

void thermal_modes_init(void) {
  if (g_initialized) {
    return;
  }

  g_device_caps = detect_device_capabilities();
  initialize_mode_definitions();
  apply_device_capabilities();
  g_initialized = true;
}

//==============================================================================
// Mode Lookup Functions
//==============================================================================

thermal_mode_t *get_available_modes(size_t *count) {
  thermal_modes_init();
  *count = THERMAL_MODE_COUNT;
  return g_thermal_modes;
}

thermal_mode_t *find_mode_by_command(const char *command) {
  thermal_modes_init();

  for (size_t i = 0; i < THERMAL_MODE_COUNT; i++) {
    if (!g_thermal_modes[i].enabled) {
      continue;
    }

    if (strcmp(command, g_thermal_modes[i].cli_command) == 0 ||
        strcmp(command, g_thermal_modes[i].cli_alias) == 0) {
      return &g_thermal_modes[i];
    }
  }
  return NULL;
}

thermal_mode_t *find_mode_by_id(thermal_mode_id_t id) {
  thermal_modes_init();

  if (id >= THERMAL_MODE_COUNT) {
    return NULL;
  }

  return g_thermal_modes[id].enabled ? &g_thermal_modes[id] : NULL;
}

bool is_mode_supported(thermal_mode_id_t mode_id) {
  thermal_modes_init();

  if (mode_id >= THERMAL_MODE_COUNT) {
    return false;
  }

  return g_thermal_modes[mode_id].enabled;
}

//==============================================================================
// Mode Execution
//==============================================================================

static void execute_acpi_mode_change(const char *hex_code) {
  extern const char *acpi_prefix;

  char cmd[256];
  snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x15 {0x01, %s, 0x00, 0x00}",
           acpi_prefix, hex_code);
  executeAcpiCall(cmd);
}

int execute_thermal_mode(thermal_mode_id_t mode_id) {
  thermal_mode_t *mode = find_mode_by_id(mode_id);
  if (!mode) {
    fprintf(stderr, "error: thermal mode not supported or disabled\n");
    return 1;
  }

  if (mode->requires_root && geteuid() != 0) {
    // Use checkRoot to handle sudo elevation properly
    char *fake_argv[] = {"awcc", (char*)mode->cli_command, NULL};
    checkRoot(mode->cli_command, fake_argv);
    // checkRoot will exec with elevated privileges or exit, so we shouldn't reach here
    return 1;
  }

  if (mode->pre_check && mode->pre_check() != 0) {
    return 1;
  }

  if (check_current_mode(mode->hex_code)) {
    return 0;
  }

  execute_acpi_mode_change(mode->hex_code);
  printf("%s mode activated.\n", mode->name);

  if (mode->post_action) {
    mode->post_action();
  }

  return 0;
}

int execute_thermal_mode_by_command(const char *command) {
  thermal_mode_t *mode = find_mode_by_command(command);
  if (!mode) {
    return 1;
  }

  // Check if this specific thermal mode is supported on current device
  if (g_current_device && !is_thermal_mode_supported(command)) {
    fprintf(stderr, "Error: Thermal mode '%s' not supported on %s\n", mode->name, get_device_name());
    return 1;
  }

  return execute_thermal_mode(mode->id);
}

//==============================================================================
// Status and Information Functions
//==============================================================================

const char *get_current_mode_name(void) {
  extern const char *acpi_prefix;

  char cmd[256];
  snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x14 {0x0b, 0x00, 0x00, 0x00}",
           acpi_prefix);
  executeAcpiCall(cmd);

  usleep(100000);
  char *response = readAcpiResponse();

  thermal_modes_init();
  for (size_t i = 0; i < THERMAL_MODE_COUNT; i++) {
    if (strstr(response, g_thermal_modes[i].hex_code)) {
      return g_thermal_modes[i].name;
    }
  }

  return "Unknown";
}

void list_available_modes(void) {
  thermal_modes_init();

  printf("Available thermal modes:\n");
  for (size_t i = 0; i < THERMAL_MODE_COUNT; i++) {
    if (g_thermal_modes[i].enabled) {
      printf("  %-12s (%s) - %s\n", g_thermal_modes[i].cli_command,
             g_thermal_modes[i].cli_alias, g_thermal_modes[i].description);
    }
  }
}

//==============================================================================
// Help Generation Functions
//==============================================================================

void generate_thermal_modes_help(void) {
  thermal_modes_init();

  printf("Fan Controls (Run as root):\n");
  printf("  qm                     Query current fan mode\n");
  printf("  modes                  List available thermal modes\n");

  for (size_t i = 0; i < THERMAL_MODE_COUNT; i++) {
    if (g_thermal_modes[i].enabled) {
      printf("  %-12s (%-2s)       %s\n", g_thermal_modes[i].cli_command,
             g_thermal_modes[i].cli_alias, g_thermal_modes[i].description);
    }
  }

  printf("  gt                     Toggle G-Mode (useful for keybinds)\n\n");
}

void generate_full_help_menu(void) {
  printf("\nAlienware Command Center for Dell G Series\n");
  printf("==========================================\n\n");
  printf("Usage:\n");
  printf("  awcc [command] [arguments]...\n");
  printf("  awcc --test-mode [command] [arguments]...\n\n");

  generate_lighting_help();

  generate_thermal_modes_help();

  device_capabilities_t caps = detect_device_capabilities();
  if (caps.has_cpu_fan_control || caps.has_gpu_fan_control) {
    printf("Fan Boost Controls (Run as root):\n");
    if (caps.has_cpu_fan_control) {
      printf("  cb                      Get CPU fan boost\n");
      printf("  scb <value>             Set CPU fan boost (1-100)\n");
    }
    if (caps.has_gpu_fan_control) {
      printf("  gb                      Get GPU fan boost\n");
      printf("  sgb <value>             Set GPU fan boost (1-100)\n");
    }
    printf("\n");
    
    printf("Fan RPM/Info Controls (Run as root):\n");
    if (caps.has_cpu_fan_control) {
      printf("  cr                      Get CPU fan RPM\n");
    }
    if (caps.has_gpu_fan_control) {
      printf("  gr                      Get GPU fan RPM\n");
    }
    printf("  cfn                     Get CPU fan name\n");
    printf("  gfn                     Get GPU fan name\n");
    printf("  fans                    Show all fans status (RPM, boost, names)\n");
    printf("\n");
  }

  printf("Advanced Controls (Run as root):\n");
  printf("  autoboost               Start automatic thermal management daemon\n");
  printf("\n");
  
  printf("System Information:\n");
  printf("  device-info             Show detected device model and supported features\n");
  printf("\n");
  
  printf("Options:\n");
  printf("  --test-mode             Skip device detection and feature validation\n");
  printf("                          (for testing on unsupported devices)\n");
  printf("\n");
}
