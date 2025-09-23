#include "LightFX.h"
extern "C" {
#include <libusb.h>
}
#include <loguru.hpp>

LightFX::LightFX() { LOG_S(INFO) << "LightFX Module initialized"; }
