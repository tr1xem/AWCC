#include "AcpiUtils.h"
#include "Thermals.h"
#include "database.h"
#include <cstring>
#include <loguru.hpp>
int main(int argc, char *argv[]) {
    // Initialize loguru <- should always be the first thing to do
    loguru::g_stderr_verbosity = -1;
    // Look for -v in arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            loguru::g_stderr_verbosity = 0; // INFO level
        }
    }

    // Initialize AcpiUtil -> then use it to pass by reference
    AcpiUtils acpiUtils;

    Thermals awccthermals(acpiUtils);
    LOG_S(INFO) << "Current Thermal Mode: "
                << awccthermals.getCurrentModeName();
    awccthermals.setThermalMode(ThermalModes::Cool);
    LOG_S(INFO) << "Current Thermal Mode: "
                << awccthermals.getCurrentModeName();
    awccthermals.setThermalMode(ThermalModes::Quiet);
    LOG_S(INFO) << "Current Thermal Mode: "
                << awccthermals.getCurrentModeName();
    awccthermals.setThermalMode(ThermalModes::Performance);
    // awccthermals.setThermalMode(ThermalModes::Gmode);

    acpiUtils.deviceInfo();
    return 0;
}
