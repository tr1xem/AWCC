#include "Thermals.h"
#include <loguru.hpp>
int main(int argc, char *argv[]) {
    // Initialize loguru <- should always be the first thing to do
    loguru::init(argc, argv);

    Thermals awccthermals;
    // awccthermals.setThermalMode(Cool);
    awccthermals.setThermalMode(Performance);
}
