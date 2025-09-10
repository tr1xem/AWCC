#define _GNU_SOURCE
#include "supported_devices.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Global device configuration pointer
device_config_t *g_current_device = NULL;

// Device database - add new devices here
static device_config_t supported_devices[] = {
    {.identifier = "Dell G15 5530",
     .acpi_prefix = "AMWW",
     .acpi_model_id = 0x0000,
     .is_dmi_fallback = true,
     .features = {.has_fan_boost = true,
                  .has_thermal_modes = true,
                  .has_lighting = true,
                  .has_autoboost = true,
                  .has_cpu_temp = true,
                  .has_gpu_temp = true,
                  .has_gmode_toggle = true,
                  .thermal_modes = {.quiet = true,
                                    .balanced = true,
                                    .performance = true,
                                    .battery_saver = true,
                                    .gmode = true},
                  .lighting = {.brightness_control = false,
                               .static_color = true,
                               .spectrum_effect = true,
                               .breathing_effect = true,
                               .rainbow_effect = true,
                               .wave_effect = true,
                               .back_forth_effect = true}}},

    {.identifier = "Dell G15 5511",
     .acpi_prefix = "AMWW",
     .acpi_model_id = 0x0b75,
     .is_dmi_fallback = false,
     .features = {.has_fan_boost = true,
                  .has_thermal_modes = true,
                  .has_lighting = true,
                  .has_autoboost = true,
                  .has_cpu_temp = true,
                  .has_gpu_temp = true,
                  .has_gmode_toggle = true,
                  .thermal_modes = {.quiet = true,
                                    .balanced = true,
                                    .performance = true,
                                    .battery_saver = true,
                                    .gmode = true},
                  .lighting = {.brightness_control = true,
                               .static_color = true,
                               .spectrum_effect = true,
                               .breathing_effect = true,
                               .rainbow_effect = true,
                               .wave_effect = true,
                               .back_forth_effect = true}}},
};

// Forward declarations
static uint32_t try_acpi_detection(const char *acpi_prefix, bool debug);
static char *read_dmi_product_name(void);
static device_config_t *find_device_by_acpi_id(uint32_t acpi_id);
static device_config_t *find_device_by_dmi_name(const char *dmi_name);
static void set_current_device(device_config_t *device);

device_detection_result_t detect_device_model(void) {
  // First determine CPU vendor to know which ACPI prefix to use
  const char *acpi_prefix = "AMWW"; // Default to Intel

  FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
  if (cpuinfo) {
    char line[256];
    while (fgets(line, sizeof(line), cpuinfo)) {
      if (strstr(line, "vendor_id")) {
        if (strstr(line, "AuthenticAMD")) {
          acpi_prefix = "AMW3";
        }
        break;
      }
    }
    fclose(cpuinfo);
  }

  // Try ACPI detection first
  uint32_t acpi_model_id = try_acpi_detection(acpi_prefix, false);

  if (acpi_model_id != 2) {
    // Found device via ACPI
    device_config_t *device = find_device_by_acpi_id(acpi_model_id);
    if (device) {
      set_current_device(device);
      return DEVICE_DETECTION_SUCCESS;
    }
    // ACPI returned valid ID but device not in our database
    printf("Warning: Unknown ACPI device ID 0x%04x, falling back to DMI "
           "detection\n",
           acpi_model_id);
  }

  // Fallback to DMI detection
  char *dmi_name = read_dmi_product_name();
  if (!dmi_name) {
    return DEVICE_DETECTION_DMI_FAILED;
  }

  device_config_t *device = find_device_by_dmi_name(dmi_name);
  free(dmi_name);

  if (!device) {
    return DEVICE_DETECTION_UNSUPPORTED;
  }

  // Update the device's ACPI prefix based on CPU detection
  strcpy(device->acpi_prefix, acpi_prefix);
  set_current_device(device);

  return DEVICE_DETECTION_SUCCESS;
}

static uint32_t try_acpi_detection(const char *acpi_prefix, bool debug) {
  if (debug)
    printf("Trying /proc/acpi/call method...\n");

  char acpi_command[256];
  snprintf(acpi_command, sizeof(acpi_command),
           "\\_SB.%s.WMAX 0 0x1a {0x02, 0xa02, 0x00, 0x00}", acpi_prefix);

  // Try to write to /proc/acpi/call
  FILE *call_fp = fopen("/proc/acpi/call", "w");
  if (call_fp) {
    fputs(acpi_command, call_fp);
    fclose(call_fp);

    // Read the result
    call_fp = fopen("/proc/acpi/call", "r");
    if (call_fp) {
      char result[128];
      if (fgets(result, sizeof(result), call_fp)) {
        fclose(call_fp);

        // Remove trailing whitespace and % character
        char *end = result + strlen(result) - 1;
        while (end >= result && (*end == '\n' || *end == '\r' || *end == ' ' ||
                                 *end == '\t' || *end == '%')) {
          *end = '\0';
          end--;
        }

        if (debug)
          printf("ACPI response via /proc/acpi/call: '%s'\n", result);

        // Parse hex output
        char *hex_start = strstr(result, "0x");
        if (hex_start) {
          char *endptr;
          uint32_t parsed_result = (uint32_t)strtoul(hex_start, &endptr, 16);
          if (debug)
            printf("Parsed ACPI model ID: 0x%04x\n", parsed_result);
          return parsed_result;
        } else {
          if (debug)
            printf("No hex value found in /proc/acpi/call response\n");
        }
      } else {
        fclose(call_fp);
        if (debug)
          printf("No output from /proc/acpi/call\n");
      }
    } else {
      if (debug)
        printf("Failed to read /proc/acpi/call\n");
    }
  } else {
    if (debug)
      printf(
          "Failed to write to /proc/acpi/call (may need root permissions)\n");
  }

  if (debug)
    printf("ACPI detection failed - no valid response received\n");
  return 0;
}

static char *read_dmi_product_name(void) {
  FILE *fp = fopen("/sys/class/dmi/id/product_name", "r");
  if (!fp) {
    return NULL;
  }

  char *product_name = malloc(64);
  if (!product_name) {
    fclose(fp);
    return NULL;
  }

  if (fgets(product_name, 64, fp)) {
    // Remove newline
    char *newline = strchr(product_name, '\n');
    if (newline) {
      *newline = '\0';
    }
    fclose(fp);
    return product_name;
  }

  fclose(fp);
  free(product_name);
  return NULL;
}

static device_config_t *find_device_by_acpi_id(uint32_t acpi_id) {
  size_t num_devices = sizeof(supported_devices) / sizeof(supported_devices[0]);

  for (size_t i = 0; i < num_devices; i++) {
    if (!supported_devices[i].is_dmi_fallback &&
        supported_devices[i].acpi_model_id == acpi_id) {
      return &supported_devices[i];
    }
  }
  return NULL;
}

static device_config_t *find_device_by_dmi_name(const char *dmi_name) {
  size_t num_devices = sizeof(supported_devices) / sizeof(supported_devices[0]);

  for (size_t i = 0; i < num_devices; i++) {
    if (supported_devices[i].is_dmi_fallback &&
        strcmp(supported_devices[i].identifier, dmi_name) == 0) {
      return &supported_devices[i];
    }
  }
  return NULL;
}

static void set_current_device(device_config_t *device) {
  g_current_device = device;
}

bool is_feature_supported(const char *feature_name) {
  if (!g_current_device) {
    return false;
  }

  if (strcmp(feature_name, "fan_boost") == 0 ||
      strcmp(feature_name, "has_fan_boost") == 0) {
    return g_current_device->features.has_fan_boost;
  }
  if (strcmp(feature_name, "thermal_modes") == 0 ||
      strcmp(feature_name, "has_thermal_modes") == 0) {
    return g_current_device->features.has_thermal_modes;
  }
  if (strcmp(feature_name, "lighting") == 0 ||
      strcmp(feature_name, "has_lighting") == 0) {
    return g_current_device->features.has_lighting;
  }
  if (strcmp(feature_name, "autoboost") == 0 ||
      strcmp(feature_name, "has_autoboost") == 0) {
    return g_current_device->features.has_autoboost;
  }
  if (strcmp(feature_name, "cpu_temp") == 0 ||
      strcmp(feature_name, "has_cpu_temp") == 0) {
    return g_current_device->features.has_cpu_temp;
  }
  if (strcmp(feature_name, "gpu_temp") == 0 ||
      strcmp(feature_name, "has_gpu_temp") == 0) {
    return g_current_device->features.has_gpu_temp;
  }
  if (strcmp(feature_name, "gmode_toggle") == 0 ||
      strcmp(feature_name, "has_gmode_toggle") == 0) {
    return g_current_device->features.has_gmode_toggle;
  }

  return false;
}

bool is_thermal_mode_supported(const char *mode_name) {
  if (!g_current_device) {
    return false;
  }

  // First check if thermal modes are supported at all
  if (!g_current_device->features.has_thermal_modes) {
    return false;
  }

  // Check specific thermal modes
  if (strcmp(mode_name, "quiet") == 0 || strcmp(mode_name, "q") == 0) {
    return g_current_device->features.thermal_modes.quiet;
  }
  if (strcmp(mode_name, "balanced") == 0 || strcmp(mode_name, "balance") == 0 ||
      strcmp(mode_name, "b") == 0) {
    return g_current_device->features.thermal_modes.balanced;
  }
  if (strcmp(mode_name, "performance") == 0 || strcmp(mode_name, "p") == 0) {
    return g_current_device->features.thermal_modes.performance;
  }
  if (strcmp(mode_name, "battery_saver") == 0 ||
      strcmp(mode_name, "battery") == 0 || strcmp(mode_name, "bs") == 0) {
    return g_current_device->features.thermal_modes.battery_saver;
  }
  if (strcmp(mode_name, "gmode") == 0 || strcmp(mode_name, "g") == 0) {
    return g_current_device->features.thermal_modes.gmode;
  }

  return false;
}

bool is_lighting_effect_supported(const char *effect_name) {
  if (!g_current_device) {
    return false;
  }

  // First check if lighting is supported at all
  if (!g_current_device->features.has_lighting) {
    return false;
  }

  // Check specific lighting effects
  if (strcmp(effect_name, "brightness") == 0) {
    return g_current_device->features.lighting.brightness_control;
  }
  if (strcmp(effect_name, "static") == 0) {
    return g_current_device->features.lighting.static_color;
  }
  if (strcmp(effect_name, "spectrum") == 0) {
    return g_current_device->features.lighting.spectrum_effect;
  }
  if (strcmp(effect_name, "breathe") == 0) {
    return g_current_device->features.lighting.breathing_effect;
  }
  if (strcmp(effect_name, "rainbow") == 0) {
    return g_current_device->features.lighting.rainbow_effect;
  }
  if (strcmp(effect_name, "wave") == 0) {
    return g_current_device->features.lighting.wave_effect;
  }
  if (strcmp(effect_name, "bkf") == 0) {
    return g_current_device->features.lighting.back_forth_effect;
  }

  return false;
}

void build_device_info_string(char *buffer, size_t buffer_size) {
  int offset = 0;

  if (!g_current_device) {
    offset += snprintf(buffer + offset, buffer_size - offset,
                       "Device Detection Report:\n======================\n");

    // CPU Information
    offset += snprintf(buffer + offset, buffer_size - offset, "CPU Vendor: ");
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo) {
      char line[256];
      bool found_vendor = false;
      while (fgets(line, sizeof(line), cpuinfo)) {
        if (strstr(line, "vendor_id")) {
          if (strstr(line, "AuthenticAMD")) {
            offset += snprintf(buffer + offset, buffer_size - offset, "AMD\n");
          } else if (strstr(line, "GenuineIntel")) {
            offset +=
                snprintf(buffer + offset, buffer_size - offset, "Intel\n");
          } else {
            offset +=
                snprintf(buffer + offset, buffer_size - offset, "Unknown\n");
          }
          found_vendor = true;
          break;
        }
      }
      if (!found_vendor)
        offset += snprintf(buffer + offset, buffer_size - offset, "Unknown\n");
      fclose(cpuinfo);
    } else {
      offset +=
          snprintf(buffer + offset, buffer_size - offset, "Unable to detect\n");
    }

    // DMI Product Name
    offset +=
        snprintf(buffer + offset, buffer_size - offset, "DMI Product Name: ");
    FILE *dmi = fopen("/sys/class/dmi/id/product_name", "r");
    if (dmi) {
      char product_name[64];
      if (fgets(product_name, sizeof(product_name), dmi)) {
        // Remove newline
        char *newline = strchr(product_name, '\n');
        if (newline)
          *newline = '\0';
        offset += snprintf(buffer + offset, buffer_size - offset, "%s\n",
                           product_name);
      } else {
        offset +=
            snprintf(buffer + offset, buffer_size - offset, "Unable to read\n");
      }
      fclose(dmi);
    } else {
      offset +=
          snprintf(buffer + offset, buffer_size - offset, "Not available\n");
    }

    // Determine ACPI prefix based on CPU
    const char *acpi_prefix = "AMWW"; // Default Intel
    cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo) {
      char line[256];
      while (fgets(line, sizeof(line), cpuinfo)) {
        if (strstr(line, "vendor_id") && strstr(line, "AuthenticAMD")) {
          acpi_prefix = "AMW3";
          break;
        }
      }
      fclose(cpuinfo);
    }

    offset += snprintf(buffer + offset, buffer_size - offset,
                       "ACPI Prefix: %s\n", acpi_prefix);

    // Try ACPI detection without debug output and show result
    uint32_t detected_id = try_acpi_detection(acpi_prefix, false);
    offset += snprintf(buffer + offset, buffer_size - offset,
                       "Parsed ACPI model ID: 0x%04x\n", detected_id);

    offset += snprintf(buffer + offset, buffer_size - offset,
                       "\n\nTo add support for this device:\n"
                       "1. Run the ACPI command above to get the device ID\n"
                       "2. Report the device info at the project repository\n"
                       "3. Include your DMI product name and ACPI response\n");
    return;
  }

  offset += snprintf(buffer + offset, buffer_size - offset,
                     "Device Information:\n==================\n");
  offset += snprintf(buffer + offset, buffer_size - offset, "Model: %s\n",
                     g_current_device->identifier);
  offset += snprintf(buffer + offset, buffer_size - offset, "ACPI Prefix: %s\n",
                     g_current_device->acpi_prefix);
  offset +=
      snprintf(buffer + offset, buffer_size - offset, "Detection Method: %s\n",
               g_current_device->is_dmi_fallback ? "DMI" : "ACPI");
  if (!g_current_device->is_dmi_fallback) {
    offset +=
        snprintf(buffer + offset, buffer_size - offset,
                 "ACPI Model ID: 0x%04x\n", g_current_device->acpi_model_id);
  }

  offset += snprintf(buffer + offset, buffer_size - offset,
                     "\nSupported Features:\n");
  offset += snprintf(buffer + offset, buffer_size - offset,
                     "  Fan Boost Control: %s\n",
                     g_current_device->features.has_fan_boost ? "Yes" : "No");
  offset +=
      snprintf(buffer + offset, buffer_size - offset, "  Thermal Modes: %s\n",
               g_current_device->features.has_thermal_modes ? "Yes" : "No");

  if (g_current_device->features.has_thermal_modes) {
    offset += snprintf(
        buffer + offset, buffer_size - offset, "    - Quiet Mode: %s\n",
        g_current_device->features.thermal_modes.quiet ? "Yes" : "No");
    offset += snprintf(
        buffer + offset, buffer_size - offset, "    - Balanced Mode: %s\n",
        g_current_device->features.thermal_modes.balanced ? "Yes" : "No");
    offset += snprintf(
        buffer + offset, buffer_size - offset, "    - Performance Mode: %s\n",
        g_current_device->features.thermal_modes.performance ? "Yes" : "No");
    offset += snprintf(
        buffer + offset, buffer_size - offset, "    - Battery Saver Mode: %s\n",
        g_current_device->features.thermal_modes.battery_saver ? "Yes" : "No");
    offset +=
        snprintf(buffer + offset, buffer_size - offset, "    - G-Mode: %s\n",
                 g_current_device->features.thermal_modes.gmode ? "Yes" : "No");
  }

  offset +=
      snprintf(buffer + offset, buffer_size - offset, "  RGB Lighting: %s\n",
               g_current_device->features.has_lighting ? "Yes" : "No");

  if (g_current_device->features.has_lighting) {
    offset += snprintf(
        buffer + offset, buffer_size - offset, "    - Brightness Control: %s\n",
        g_current_device->features.lighting.brightness_control ? "Yes" : "No");
    offset += snprintf(
        buffer + offset, buffer_size - offset, "    - Static Colors: %s\n",
        g_current_device->features.lighting.static_color ? "Yes" : "No");
    offset += snprintf(
        buffer + offset, buffer_size - offset, "    - Spectrum Effect: %s\n",
        g_current_device->features.lighting.spectrum_effect ? "Yes" : "No");
    offset += snprintf(
        buffer + offset, buffer_size - offset, "    - Breathing Effect: %s\n",
        g_current_device->features.lighting.breathing_effect ? "Yes" : "No");
    offset += snprintf(
        buffer + offset, buffer_size - offset, "    - Rainbow Effect: %s\n",
        g_current_device->features.lighting.rainbow_effect ? "Yes" : "No");
    offset += snprintf(
        buffer + offset, buffer_size - offset, "    - Wave Effect: %s\n",
        g_current_device->features.lighting.wave_effect ? "Yes" : "No");
    offset += snprintf(
        buffer + offset, buffer_size - offset,
        "    - Back & Forth Effect: %s\n",
        g_current_device->features.lighting.back_forth_effect ? "Yes" : "No");
  }

  offset +=
      snprintf(buffer + offset, buffer_size - offset, "  Auto Boost: %s\n",
               g_current_device->features.has_autoboost ? "Yes" : "No");
  offset +=
      snprintf(buffer + offset, buffer_size - offset, "  CPU Temperature: %s\n",
               g_current_device->features.has_cpu_temp ? "Yes" : "No");
  offset +=
      snprintf(buffer + offset, buffer_size - offset, "  GPU Temperature: %s\n",
               g_current_device->features.has_gpu_temp ? "Yes" : "No");
  offset +=
      snprintf(buffer + offset, buffer_size - offset, "  G-Mode Toggle: %s\n",
               g_current_device->features.has_gmode_toggle ? "Yes" : "No");
}

void print_device_info(void) {
  char buffer[4096]; // Large buffer for the device info
  build_device_info_string(buffer, sizeof(buffer));
  printf("%s", buffer);
}

const char *get_device_name(void) {
  if (!g_current_device) {
    return "Unknown Device";
  }
  return g_current_device->identifier;
}

const char *get_acpi_prefix(void) {
  if (!g_current_device) {
    return "AMWW"; // Default fallback
  }
  return g_current_device->acpi_prefix;
}

bool validate_lighting_via_dmi(const char *effect_name) {
  // Read DMI product name
  char *dmi_name = read_dmi_product_name();
  if (!dmi_name) {
    return false; // Can't read DMI, assume not supported
  }

  // Find device in supported_devices array by DMI name
  size_t num_devices = sizeof(supported_devices) / sizeof(supported_devices[0]);
  device_config_t *device = NULL;

  for (size_t i = 0; i < num_devices; i++) {
    if (strcmp(supported_devices[i].identifier, dmi_name) == 0) {
      device = &supported_devices[i];
      break;
    }
  }

  free(dmi_name);

  if (!device) {
    return false; // Device not in our database
  }

  // Check if lighting is supported at all
  if (!device->features.has_lighting) {
    return false;
  }

  // Check specific lighting effects
  if (strcmp(effect_name, "brightness") == 0) {
    return device->features.lighting.brightness_control;
  }
  if (strcmp(effect_name, "static") == 0) {
    return device->features.lighting.static_color;
  }
  if (strcmp(effect_name, "spectrum") == 0) {
    return device->features.lighting.spectrum_effect;
  }
  if (strcmp(effect_name, "breathe") == 0) {
    return device->features.lighting.breathing_effect;
  }
  if (strcmp(effect_name, "rainbow") == 0) {
    return device->features.lighting.rainbow_effect;
  }
  if (strcmp(effect_name, "wave") == 0) {
    return device->features.lighting.wave_effect;
  }
  if (strcmp(effect_name, "bkf") == 0) {
    return device->features.lighting.back_forth_effect;
  }
  if (strcmp(effect_name, "defaultblue") == 0) {
    return device->features.lighting
        .static_color; // Uses static color capability
  }

  return false; // Unknown effect
}

void cleanup_device_detection(void) { g_current_device = NULL; }
