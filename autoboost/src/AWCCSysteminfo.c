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

	char *model = strtok(product_name, " ");
	char *version = strtok(NULL, " ");

	switch (field) {
	case AWCCSystemInfoModel:
		return model;
	case AWCCSystemInfoVersion:
		return version;
	default:
		return NULL;
	}
}
