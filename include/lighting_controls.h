#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
  LIGHTING_BRIGHTNESS,
  LIGHTING_STATIC,
  LIGHTING_SPECTRUM,
  LIGHTING_BREATHE,
  LIGHTING_RAINBOW,
  LIGHTING_WAVE,
  LIGHTING_BKF,
  LIGHTING_DEFAULTBLUE,
  LIGHTING_COMMAND_COUNT
} lighting_command_id_t;

typedef struct {
  lighting_command_id_t id;
  const char *command;
  const char *description;
  const char *usage;
  int min_args;
  int max_args;
} lighting_command_t;

int execute_lighting_command(int argc, char **argv);
bool is_lighting_command(const char *command);
void list_lighting_commands(void);
void generate_lighting_help(void);
