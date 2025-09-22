#include "AcpiUtils.h"
#include "Thermals.h"
#include "database.h"
#include <loguru.hpp>
int main(int argc, char *argv[]) {
    // Initialize loguru <- should always be the first thing to do
    loguru::g_stderr_verbosity = -1;
    loguru::init(argc, argv);

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

    return 0;
}
