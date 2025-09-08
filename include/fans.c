#include "thermal_modes.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define default_mode performanceMode()

const char *acpi_prefix = "AMWW"; // Default to Intel

void detectCpuVendor() {
  FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
  if (!cpuinfo) {
    perror("Failed to open /proc/cpuinfo");
    return;
  }

  char line[256];
  while (fgets(line, sizeof(line), cpuinfo)) {
    if (strstr(line, "vendor_id")) {
      if (strstr(line, "AuthenticAMD")) {
        acpi_prefix = "AMW3"; // AMD detected
      }
      break;
    }
  }
  // printf("Detected CPU vendor: %s\n", acpi_prefix);
  fclose(cpuinfo);
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

void executeAcpiCall(const char *command) {
  FILE *acpi_file = fopen("/proc/acpi/call", "w");
  if (!acpi_file) {
    perror("Unable to open /proc/acpi/call");
    exit(1);
  }
  fprintf(acpi_file, "%s\n", command);
  fclose(acpi_file);
}

const char *getModeNameFromHex(const char *hex) {
  if (strcmp(hex, "0xa0") == 0)
    return "Balanced";
  if (strcmp(hex, "0xa1") == 0)
    return "Performance";
  if (strcmp(hex, "0xa3") == 0)
    return "Quiet";
  if (strcmp(hex, "0xa5") == 0)
    return "Battery Saver";
  if (strcmp(hex, "0xab") == 0)
    return "Gaming";
  if (strcmp(hex, "0x0") == 0)
    return "Manual";
  return "Unknown";
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
    printf("You're already in %s mode (%s).\n", getModeNameFromHex(desired_hex),
           desired_hex);
    return 1;
  } else {
    printf("Switching to %s mode...\n", getModeNameFromHex(desired_hex));
    return 0;
  }
}

void printCurrentMode() {
  char cmd[256];
  snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x14 {0x0b, 0x00, 0x00, 0x00}",
           acpi_prefix);
  executeAcpiCall(cmd);

  usleep(100000);
  char *response = readAcpiResponse();

  if (strstr(response, "0xa0")) {
    printf("Current mode: Balanced\n");
  } else if (strstr(response, "0xa1")) {
    printf("Current mode: Performance\n");
  } else if (strstr(response, "0xa3")) {
    printf("Current mode: Quiet\n");
  } else if (strstr(response, "0xa5")) {
    printf("Current mode: Battery Saver\n");
  } else if (strstr(response, "0xab")) {
    printf("Current mode: Gaming (G-Mode)\n");
  } else if (strstr(response, "0x0")) {
    printf("Current mode: Manual\n");
  } else {
    printf("Current mode: Unknown (%s)\n", response);
  }
}

int extractHexValue(const char *response) {
  char *start = strstr(response, "0x");
  if (!start) {
    fprintf(stderr, "No hex value found in response.\n");
    return -1;
  }
  unsigned int value;
  if (sscanf(start, "%x", &value) == 1)
    return value;
  else {
    fprintf(stderr, "Failed to parse hex value.\n");
    return -1;
  }
}

void getFanBoost(const char *device) {
  char cmd[256];
  if (strcmp(device, "cpu") == 0) {
    snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x14 {0x0c, 0x32, 0x00, 0x00}",
             acpi_prefix);
  } else if (strcmp(device, "gpu") == 0) {
    snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x14 {0x0c, 0x33, 0x00, 0x00}",
             acpi_prefix);
  } else {
    fprintf(stderr, "Unknown device type: %s\n", device);
    return;
  }

  executeAcpiCall(cmd);

  usleep(100000);
  char *response = readAcpiResponse();
  int value = extractHexValue(response);

  if (value >= 0) {
    printf("Current %s Fan Boost: %d%%\n",
           strcmp(device, "cpu") == 0 ? "CPU" : "GPU", value);
  }
}

void setFanBoost(const char *device, int value) {
  if (value < 0 || value > 100) {
    fprintf(stderr, "Fan boost value must be between 1 and 100.\n");
    return;
  }

  char cmd[256];
  if (strcmp(device, "cpu") == 0) {
    snprintf(cmd, sizeof(cmd),
             "\\_SB.%s.WMAX 0 0x15 {0x02, 0x32, 0x%02x, 0x00}", acpi_prefix,
             value);
  } else if (strcmp(device, "gpu") == 0) {
    snprintf(cmd, sizeof(cmd),
             "\\_SB.%s.WMAX 0 0x15 {0x02, 0x33, 0x%02x, 0x00}", acpi_prefix,
             value);
  } else {
    fprintf(stderr, "Unknown device type: %s\n", device);
    return;
  }

  executeAcpiCall(cmd);
  printf("%s Fan Boost set to %d%%.\n",
         strcmp(device, "cpu") == 0 ? "CPU" : "GPU", value);
}

void quietMode() {
  if (check_current_mode("0xa3"))
    return;
  char cmd[256];
  snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x15 {0x01, 0xa3, 0x00, 0x00}",
           acpi_prefix);
  executeAcpiCall(cmd);
  printf("Quiet mode activated.\n");
}

void performanceMode() {
  if (check_current_mode("0xa1"))
    return;
  char cmd[256];
  snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x15 {0x01, 0xa1, 0x00, 0x00}",
           acpi_prefix);
  executeAcpiCall(cmd);
  printf("Performance mode activated.\n");
}

void batteryMode() {
  if (check_current_mode("0xa5"))
    return;
  char cmd[256];
  snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x15 {0x01, 0xa5, 0x00, 0x00}",
           acpi_prefix);
  executeAcpiCall(cmd);
  printf("Battery Saver mode activated.\n");
}

void balanceMode() {
  if (check_current_mode("0xa0"))
    return;
  char cmd[256];
  snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x15 {0x01, 0xa0, 0x00, 0x00}",
           acpi_prefix);
  executeAcpiCall(cmd);
  printf("Balance mode activated.\n");
}

void gamingMode() {
  if (check_current_mode("0xab"))
    return;
  char cmd[256];
  snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x15 {0x01, 0xab, 0x00, 0x00}",
           acpi_prefix);
  executeAcpiCall(cmd);
  printf("Gaming mode activated.\n");
}

void toggleGMode() {
  char cmd[256];
  snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x25 {0x02, 0x00, 0x00, 0x00}",
           acpi_prefix);
  executeAcpiCall(cmd);

  usleep(100000);
  char *response = readAcpiResponse();

  if (strstr(response, "0x0")) {
    printf("G-Mode is currently OFF. Enabling Gaming Mode...\n");
    execute_thermal_mode(THERMAL_MODE_GAMING);
    snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x25 {0x01, 0x01, 0x00, 0x00}",
             acpi_prefix);
    executeAcpiCall(cmd);
  } else if (strstr(response, "0x1")) {
    printf("G-Mode is currently ON. Reverting to Default Mode...\n");
    execute_thermal_mode(THERMAL_MODE_PERFORMANCE);
    snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x25 {0x01, 0x00, 0x00, 0x00}",
             acpi_prefix);
    executeAcpiCall(cmd);
  } else {
    fprintf(stderr, "Unable to determine G-Mode status. Response: %s\n",
            response);
  }
}
