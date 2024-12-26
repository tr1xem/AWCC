#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

// Function to execute ACPI calls
void executeAcpiCall(const char *command);

// Functions to activate various modes
void quietMode();
void performanceMode();
void gamingMode();

// Function to get the current fan speed
int getFanSpeed();

// Function to toggle gaming mode based on fan speed
void toggleGMode();

#endif // FAN_CONTROL_H

