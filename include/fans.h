#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

#include "thermal_modes.h"

void executeAcpiCall(const char *command);
char *readAcpiResponse(void);
int check_current_mode(const char *desired_hex);

void checkRoot(const char *command, char **argv);
void detectCpuVendor(void);
void getFanBoost(const char *device);
void setFanBoost(const char *device, int value);
void printCurrentMode(void);
void toggleGMode(void);

#endif
