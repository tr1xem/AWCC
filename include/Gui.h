#pragma once
#include "AcpiUtils.h"
#include "EffectController.h"
#include "Thermals.h"
#include <imgui.h>

namespace Gui {
// Call this once at startup
void App(int h, int w, Thermals &thermals, AcpiUtils &acpiUtils,
         int &selectedMode, int &gpuBoost, int &cpuBoost, ImFont &smallFont,
         ImFont &fontbold, int &brightness, EffectController &effects,
         bool &turbo);

void SetupImGuiStyle();

} // namespace Gui
