#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define default_mode performanceMode()

void checkRoot(const char *command, char **argv) {
	if (geteuid() != 0) {
		const char *rootCommands[] = {"q",		 "quiet", "p",	"performance",
									  "g",		 "gmode", "gt", "b",
									  "balance", "bs"};
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
								argv[2],		   argv[3], NULL};
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

void quietMode() {
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x15 {1, 0xa3, 0x00, 0x00}"); // Quiet mode
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x25 {1, 0x01, 0x00, 0x00}"); // Shared call
	printf("Quiet mode activated.\n");
}

void performanceMode() {
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x15 {1, 0xa1, 0x00, 0x00}"); // Performance mode
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x25 {1, 0x01, 0x00, 0x00}"); // Shared call
	printf("Performance mode activated.\n");
}

void batteryMode() {
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x15 {1, 0xa5, 0x00, 0x00}"); // Battery mode
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x25 {1, 0x01, 0x00, 0x00}"); // Shared call
	printf("Battery Saver mode activated.\n");
}
void balanceMode() {
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x15 {1, 0xa0, 0x00, 0x00}"); // Balanced mode
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x25 {1, 0x01, 0x00, 0x00}"); // Shared call
	printf("Balance mode activated.\n");
}
void gamingMode() {
	executeAcpiCall("\\_SB.AMWW.WMAX 0 0x15 {1, 0xab, 0x00, 0x00}"); // GMode
	executeAcpiCall(
		"\\_SB.AMWW.WMAX 0 0x25 {1, 0x01, 0x00, 0x00}"); // Shared call
	printf("Gaming mode activated.\n");
}

int getFanSpeed() {
	char buffer[256];
	int fan_speed = -1;

	FILE *fp = popen("sensors", "r");
	if (!fp) {
		perror("Failed to run sensors command");
		exit(EXIT_FAILURE);
	}

	while (fgets(buffer, sizeof(buffer), fp)) {
		if (strstr(buffer, "CPU Fan")) {
			sscanf(buffer, "%*[^:]: %d", &fan_speed);
			break;
		}
	}

	pclose(fp);

	if (fan_speed == -1) {
		fprintf(stderr, "Unable to find CPU fan speed in sensors output\n");
		exit(EXIT_FAILURE);
	}

	return fan_speed;
}

void toggleGMode() {
	int fan_speed = getFanSpeed();

	printf("Current fan speed: %d RPM\n", fan_speed);
	printf("Fan speed threshold: %d RPM\n", 3000);

	if (fan_speed > 3000) {
		default_mode;
	} else {
		gamingMode();
	}
}
