#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

void executeAcpiCall(const char *command);

void checkRoot(const char *command, char **argv);
void send_notification(const char *app_name, const char *message);
void quietMode();
void performanceMode();
void gamingMode();
void balanceMode();
void batteryMode();
void detectCpuVendor();
void getFanBoost(const char *device);
void setFanBoost(const char *device, int value);
void printCurrentMode();
void toggleGMode();

#endif
