
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void checkRoot() {
    if (geteuid() != 0) {
        printf("This script must be run as root.\n");

        char *pkexec_path = "/usr/bin/pkexec";
        char **new_argv =
            (char **)malloc(sizeof(char *) * (2 + 1));

        if (!new_argv) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        new_argv[0] = pkexec_path;
        new_argv[1] = strdup(getenv("_"));
        new_argv[2] = NULL;

        if (execvp(pkexec_path, new_argv) == -1) {
            perror("Failed to elevate privileges with pkexec");
            free(new_argv[1]);
            free(new_argv);
            exit(EXIT_FAILURE);
        }
    }
}

void executeAcpiCall(const char *command) {
	checkRoot();
	FILE *acpi_file = fopen("/proc/acpi/call", "w");
	if (!acpi_file) {
		perror("Unable to open /proc/acpi/call");
		exit(1);
	}
	fprintf(acpi_file, "%s\n", command);
	fclose(acpi_file);
}

void quietMode() {
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x15 {1, 0xa3, 0x00, 0x00}"); // Quiet mode
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x25 {1, 0x01, 0x00, 0x00}");
	printf("Quiet mode activated.\n");
}

void performanceMode() {
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x15 {1, 0xa1, 0x00, 0x00}"); // Performance mode
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x25 {1, 0x01, 0x00, 0x00}");
	printf("Performance mode activated.\n");
}

void gamingMode() {
	executeAcpiCall("\\_SB.AMWW.WMAX 0 0x15 {1, 0xab, 0x00, 0x00}"); // GMode
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x25 {1, 0x01, 0x00, 0x00}");
	printf("Gaming mode activated.\n");
}

int getFanSpeed() {
	FILE *fan_speed_file = fopen("/sys/class/hwmon/hwmon4/fan1_input", "r");
	if (!fan_speed_file) {
		perror("Unable to open fan speed file");
		exit(1);
	}

	int fan_speed;
	fscanf(fan_speed_file, "%d", &fan_speed);
	fclose(fan_speed_file);

	return fan_speed;
}

void toggleGMode() {
	int fan_speed = getFanSpeed();

	printf("Current fan speed: %d RPM\n", fan_speed);
	printf("Fan speed threshold: %d RPM\n", 3000);

	if (fan_speed > 3000) {
		performanceMode();
	} else {
		gamingMode();
	}
}
