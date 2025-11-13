#include "helper.h"
#include <fstream>
#include <loguru.hpp>
#include <sstream>
#include <string>

const char *Helper::getDeviceName() {
    static std::string deviceName;
    std::ifstream dmiFile("/sys/class/dmi/id/product_name");
    if (!dmiFile.is_open()) {
        LOG_S(ERROR) << "Cannot read /sys/class/dmi/id/product_name";
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << dmiFile.rdbuf();
    deviceName = buffer.str();

    if (!deviceName.empty() && deviceName.back() == '\n') {
        deviceName.pop_back();
    }

    return deviceName.c_str();
};
