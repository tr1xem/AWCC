#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

void executeAcpiCall(const char *command);

void checkRoot(const char *command, char **argv);

void quietMode();
void performanceMode();
void gamingMode();

int getFanSpeed();

void toggleGMode();

#endif
