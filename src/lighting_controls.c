#include "lighting_controls.h"
#include "effects.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const lighting_command_t lighting_commands[] = {
    {LIGHTING_BRIGHTNESS, "brightness", "Set keyboard brightness",
     "brightness <0-100>", 2, 2},
    {LIGHTING_STATIC, "static", "Set static color", "static <color_hex>", 2, 2},
    {LIGHTING_SPECTRUM, "spectrum", "Color spectrum effect",
     "spectrum <duration_ms>", 2, 2},
    {LIGHTING_BREATHE, "breathe", "Breathing effect", "breathe <color_hex>", 2,
     2},
    {LIGHTING_RAINBOW, "rainbow", "Rainbow wave effect",
     "rainbow <duration_ms>", 2, 2},
    {LIGHTING_WAVE, "wave", "Wave effect", "wave <color_hex>", 2, 2},
    {LIGHTING_BKF, "bkf", "Back and forth effect", "bkf <color_hex>", 2, 2},
    {LIGHTING_DEFAULTBLUE, "defaultblue", "Default blue color", "defaultblue",
     1, 1},
};

static const lighting_command_t *find_lighting_command(const char *command) {
  for (int i = 0; i < LIGHTING_COMMAND_COUNT; i++) {
    if (strcmp(lighting_commands[i].command, command) == 0) {
      return &lighting_commands[i];
    }
  }
  return NULL;
}

static int execute_brightness_command(__attribute__((unused)) int argc,
                                      char **argv) {
  int input_value = atoi(argv[1]);
  if (input_value < 0 || input_value > 100) {
    fprintf(stderr, "error: brightness value must be between 0 and 100\n");
    return 1;
  }
  brightness((uint8_t)input_value);
  return 0;
}

static int execute_static_command(__attribute__((unused)) int argc,
                                  char **argv) {
  uint32_t color = strtol(argv[1], NULL, 16);
  if (color == 0 && strcmp(argv[1], "0") != 0) {
    fprintf(stderr, "error: invalid color %s\n", argv[1]);
    return 1;
  }
  static_color(color);
  return 0;
}

static int execute_spectrum_command(__attribute__((unused)) int argc,
                                    char **argv) {
  uint16_t duration = strtol(argv[1], NULL, 10);
  if (duration == 0 && strcmp(argv[1], "0") != 0) {
    fprintf(stderr, "error: invalid duration %s\n", argv[1]);
    return 1;
  }
  spectrum(duration);
  return 0;
}

static int execute_breathe_command(__attribute__((unused)) int argc,
                                   char **argv) {
  uint32_t color = strtol(argv[1], NULL, 16);
  if (color == 0 && strcmp(argv[1], "0") != 0) {
    fprintf(stderr, "error: invalid color %s\n", argv[1]);
    return 1;
  }
  breathe(color);
  return 0;
}

static int execute_rainbow_command(__attribute__((unused)) int argc,
                                   char **argv) {
  uint16_t duration = strtol(argv[1], NULL, 10);
  if (duration == 0 && strcmp(argv[1], "0") != 0) {
    fprintf(stderr, "error: invalid duration %s\n", argv[1]);
    return 1;
  }
  rainbow(duration);
  return 0;
}

static int execute_wave_command(__attribute__((unused)) int argc, char **argv) {
  uint32_t color = strtol(argv[1], NULL, 16);
  if (color == 0 && strcmp(argv[1], "0") != 0) {
    fprintf(stderr, "error: invalid color %s\n", argv[1]);
    return 1;
  }
  wave(color);
  return 0;
}

static int execute_bkf_command(__attribute__((unused)) int argc, char **argv) {
  uint32_t color = strtol(argv[1], NULL, 16);
  if (color == 0 && strcmp(argv[1], "0") != 0) {
    fprintf(stderr, "error: invalid color %s\n", argv[1]);
    return 1;
  }
  back_and_forth(color);
  return 0;
}

static int execute_defaultblue_command(__attribute__((unused)) int argc,
                                       __attribute__((unused)) char **argv) {
  defaultblue(0x00FFFF);
  return 0;
}

int execute_lighting_command(int argc, char **argv) {
  if (argc < 1) {
    return 1;
  }

  const lighting_command_t *cmd = find_lighting_command(argv[0]);
  if (!cmd) {
    return 1;
  }

  if (argc < cmd->min_args || argc > cmd->max_args) {
    fprintf(stderr, "error: %s requires %d argument(s). Usage: %s\n",
            cmd->command, cmd->max_args - 1, cmd->usage);
    return 1;
  }

  switch (cmd->id) {
  case LIGHTING_BRIGHTNESS:
    return execute_brightness_command(argc, argv);
  case LIGHTING_STATIC:
    return execute_static_command(argc, argv);
  case LIGHTING_SPECTRUM:
    return execute_spectrum_command(argc, argv);
  case LIGHTING_BREATHE:
    return execute_breathe_command(argc, argv);
  case LIGHTING_RAINBOW:
    return execute_rainbow_command(argc, argv);
  case LIGHTING_WAVE:
    return execute_wave_command(argc, argv);
  case LIGHTING_BKF:
    return execute_bkf_command(argc, argv);
  case LIGHTING_DEFAULTBLUE:
    return execute_defaultblue_command(argc, argv);
  default:
    return 1;
  }
}

void list_lighting_commands(void) {
  printf("Available lighting commands:\n");
  for (int i = 0; i < LIGHTING_COMMAND_COUNT; i++) {
    printf("  %-12s - %s\n", lighting_commands[i].command,
           lighting_commands[i].description);
  }
}

void generate_lighting_help(void) {
  printf("Lighting Controls:\n");
  for (int i = 0; i < LIGHTING_COMMAND_COUNT; i++) {
    printf("  %-20s %s\n", lighting_commands[i].usage,
           lighting_commands[i].description);
  }
  printf("\n");
}
