#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef enum {
  THERMAL_MODE_QUIET,
  THERMAL_MODE_BATTERY_SAVER,
  THERMAL_MODE_BALANCED,
  THERMAL_MODE_PERFORMANCE,
  THERMAL_MODE_GAMING,
  THERMAL_MODE_COUNT
} thermal_mode_id_t;

typedef struct {
  thermal_mode_id_t id;
  const char *name;
  const char *hex_code;
  const char *description;
  const char *cli_command;
  const char *cli_alias;
  bool enabled;
  bool requires_root;
  int (*pre_check)(void);
  void (*post_action)(void);
} thermal_mode_t;

typedef struct {
  bool supports_quiet;
  bool supports_gaming;
  bool supports_battery_saver;
  bool supports_performance;
  bool supports_balanced;
  bool has_gpu_fan_control;
  bool has_cpu_fan_control;
  const char *acpi_prefix;
} device_capabilities_t;

typedef struct {
  const char *device_id;
  thermal_mode_t *available_modes;
  size_t mode_count;
  thermal_mode_id_t default_mode;
  device_capabilities_t capabilities;
} device_mode_map_t;

void thermal_modes_init(void);
device_capabilities_t detect_device_capabilities(void);
thermal_mode_t *get_available_modes(size_t *count);
thermal_mode_t *find_mode_by_command(const char *command);
thermal_mode_t *find_mode_by_id(thermal_mode_id_t id);
int execute_thermal_mode(thermal_mode_id_t mode_id);
int execute_thermal_mode_by_command(const char *command);
const char *get_current_mode_name(void);
void list_available_modes(void);
bool is_mode_supported(thermal_mode_id_t mode_id);

// Help generation functions
void generate_thermal_modes_help(void);
void generate_full_help_menu(void);
