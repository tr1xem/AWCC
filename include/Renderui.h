#pragma once
#include "AcpiUtils.h"
#include "EffectController.h"
#include "Thermals.h"
#include <GLFW/glfw3.h>

namespace RenderUi {
bool Init(Thermals &thermals, AcpiUtils &acpiUtils, EffectController &effects);

} // namespace RenderUi
