#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum AWCCSystemInfoField_t {
	AWCCSystemInfoModel,
	AWCCSystemInfoVersion,
};

static const char *GetSystemInfo(enum AWCCSystemInfoField_t field) {
	static char product_name[256] = {0};
	static _Bool initialized = 0;

	if (!initialized) {
		FILE *file = fopen("/sys/class/dmi/id/product_name", "r");
		if (!file) {
			perror("Unable to open /sys/class/dmi/id/product_name");
			exit(EXIT_FAILURE);
		}

		if (!fgets(product_name, sizeof(product_name), file)) {
			perror("Failed to read product name");
			fclose(file);
			exit(EXIT_FAILURE);
		}
		fclose(file);

		// Remove trailing newline
		product_name[strcspn(product_name, "\n")] = '\0';
		initialized = 1;
	}

	// Make a copy of the product_name to avoid modifying the original string
	char product_name_copy[256];
	strncpy(product_name_copy, product_name, sizeof(product_name_copy));
	product_name_copy[sizeof(product_name_copy) - 1] = '\0';

	// Skip the first token (e.g., "Dell")
	char *token = strtok(product_name_copy, " ");
	token = strtok(NULL, " "); // Model
	char *model = token;
	token = strtok(NULL, " "); // Version
	int version = token ? atoi(token) : 0;

	switch (field) {
	case AWCCSystemInfoModel:
		return model;
	case AWCCSystemInfoVersion: {
		static char version_str[16];
		snprintf(version_str, sizeof(version_str), "%d", version);
		return version_str;
	}
	default:
		return NULL;
	}
}
