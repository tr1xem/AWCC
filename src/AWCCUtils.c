# include "AWCCUtils.h"
# include "thermal_modes.h"

# define _GNU_SOURCE
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>

const char *acpi_prefix = "AMWW"; // Default to Intel

void executeAcpiCall(const char *command) {
  FILE *acpi_file = fopen("/proc/acpi/call", "w");
  if (!acpi_file) {
    perror("Unable to open /proc/acpi/call");
    exit(1);
  }
  fprintf(acpi_file, "%s\n", command);
  fclose(acpi_file);
}

char *readAcpiResponse() {
  FILE *fp = fopen("/proc/acpi/call", "r");
  if (!fp) {
    perror("Unable to read /proc/acpi/call");
    exit(EXIT_FAILURE);
  }
  static char buffer[128];
  fgets(buffer, sizeof(buffer), fp);
  fclose(fp);

  for (char *p = buffer; *p; ++p)
    *p = tolower(*p);
  return buffer;
}

int check_current_mode(const char *desired_hex) {
  char cmd[256];
  snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x14 {0x0b, 0x00, 0x00, 0x00}",
           acpi_prefix);
  executeAcpiCall(cmd);

  usleep(100000);
  char *response = readAcpiResponse();

  if (strstr(response, desired_hex)) {
    return 1;
  }
  return 0;
}

void checkRoot(const char *command, char **argv) {
  if (geteuid() != 0) {
    const char *rootCommands[] = {
        "q",
        "quiet",
        "p",
        "performance",
        "g",
        "gmode",
        "gt",
        "b",
        "balance",
        "bs",
        "battery",
        "qm",
        "query",
        "gb",
        "cb",
        "sgb",
        "scb",
        "getcpufanboost",
        "getgpufanboost",
        "setcpufanboost",
        "setgpufanboost",
        "cr",
        "gr",
        "getcpufanrpm",
        "getgpufanrpm",
        "fans",
        "autoboost",
    };
    size_t numCommands = sizeof(rootCommands) / sizeof(rootCommands[0]);

    int requiresRoot = 0;
    for (size_t i = 0; i < numCommands; i++) {
      if (strcmp(command, rootCommands[i]) == 0) {
        requiresRoot = 1;
        break;
      }
    }

    if (requiresRoot) {
      printf("This command requires root privileges, elevating...\n");

      char *new_argv[] = {"/usr/bin/pkexec", argv[0], argv[1],
                          argv[2],           argv[3], NULL};
      execvp("/usr/bin/pkexec", new_argv);

      perror("Failed to elevate privileges");
      exit(EXIT_FAILURE);
    }
  }
}

// const struct AWCCUtils_t AWCCUtils = {
// };
