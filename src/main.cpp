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
    awccthermals.setThermalMode(ThermalModes::Cool);
    awccthermals.setThermalMode(ThermalModes::Performance);
    return 0;
}
