#ifndef SUPPORTED_DEVICES_H
#define SUPPORTED_DEVICES_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char identifier[64];        // ACPI hex ID or DMI model name
    char acpi_prefix[8];        // "AMWW" or "AMW3"
    uint32_t acpi_model_id;     // 0x0000 for DMI-detected devices
    bool is_dmi_fallback;       // true if detected via DMI
    struct {
        bool has_fan_boost;
        bool has_thermal_modes;
        bool has_lighting;
        bool has_autoboost;
        bool has_cpu_temp;
        bool has_gpu_temp;
        bool has_gmode_toggle;
    } features;
} device_config_t;

typedef enum {
    DEVICE_DETECTION_SUCCESS,
    DEVICE_DETECTION_ACPI_FAILED,
    DEVICE_DETECTION_DMI_FAILED,
    DEVICE_DETECTION_UNSUPPORTED
} device_detection_result_t;

// Global device configuration
extern device_config_t *g_current_device;

// Function declarations
device_detection_result_t detect_device_model(void);
bool is_feature_supported(const char* feature_name);
void print_device_info(void);
const char* get_device_name(void);
const char* get_acpi_prefix(void);
void cleanup_device_detection(void);

// Feature check macros for convenience
#define CHECK_FEATURE(feature) do { \
    if (!is_feature_supported(#feature)) { \
        fprintf(stderr, "Error: %s not supported on %s\n", #feature, get_device_name()); \
        return 1; \
    } \
} while(0)

#define CHECK_FEATURE_VOID(feature) do { \
    if (!is_feature_supported(#feature)) { \
        fprintf(stderr, "Error: %s not supported on %s\n", #feature, get_device_name()); \
        return; \
    } \
} while(0)

#endif // SUPPORTED_DEVICES_H