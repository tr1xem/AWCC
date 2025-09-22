#include "AcpiUtils.h"
#include "Thermals.h"
#include "database.h"
#include <loguru.hpp>
int main(int argc, char *argv[]) {
    // Initialize loguru <- should always be the first thing to do
    loguru::init(argc, argv);

    // Initialize AcpiUtils
    AcpiUtils acpiUtils;

    Thermals awccthermals(acpiUtils);
    awccthermals.setThermalMode(ThermalModes::Cool);
    awccthermals.setThermalMode(ThermalModes::Performance);
    return 0;
}
