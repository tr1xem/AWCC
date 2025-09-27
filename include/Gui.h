#pragma once
#include "AcpiUtils.h"
#include "Thermals.h"

namespace Gui {
// Call this once at startup
void App(int h, int w, Thermals &thermals, AcpiUtils &acpiUtils, int &selected,
         int &gpuBoost, int &cpuBoost);

void SetupImGuiStyle();

} // namespace Gui
