#include "helper.h"

#include <fstream>
#include <string>

#include "loguru.hpp"

AWCCPowerState_t PowerState() {
    std::ifstream file("/sys/class/power_supply/BAT0/status");

    if (!file) {
        throw std::runtime_error(
            "Unable to read /sys/class/power_supply/BAT0/status");
    }

    std::string status;
    std::getline(file, status);

    if (status == "Discharging") {
        return AWCCPowerStateBAT;
    }

    return AWCCPowerStateAC;
}
