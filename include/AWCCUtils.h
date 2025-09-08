# ifndef __AWCC_UTILS_H
# define __AWCC_UTILS_H

static inline int AWCCUtils_MinInt (int x, int y) { return x < y ? x : y; }
static inline int AWCCUtils_MaxInt (int x, int y) { return x > y ? x : y; }

// ACPI prefix for commands
extern const char *acpi_prefix;

// ACPI Communication Functions
void executeAcpiCall(const char *command);
char *readAcpiResponse(void);
int check_current_mode(const char *desired_hex);

// System Utilities
void checkRoot(const char *command, char **argv);

// struct AWCCUtils_t {
// } extern const AWCCUtils;

# endif // __AWCC_UTILS_H
