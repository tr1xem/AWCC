#include "EffectController.h"
#include <algorithm> // for std::min
#include <fstream>

void EffectController::Brightness(uint8_t value) {
    value = std::min<int>(value, 100);
    m_lightfx.deviceAcquire();
    m_lightfx.SendSetDim(100 - value, m_zoneAll);
    m_lightfx.deviceRelease();
    std::ofstream ofs(m_brightnessFile, std::ios::trunc);
    if (ofs.is_open()) {
        ofs << static_cast<int>(value);
    }
    LOG_S(INFO) << "Device Brightness set to: " << static_cast<int>(value)
                << "%";
}
int EffectController::getBrightness() {
    std::ifstream ifs(m_brightnessFile);
    int value = 0;
    if (ifs.is_open()) {
        ifs >> value;
    }
    value = std::min(value, 100);
    return value;
}

void EffectController::StaticColor(uint32_t color) {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(1);
    m_lightfx.SendAnimationConfigStart(1);
    m_lightfx.SendZoneSelect(1, m_zoneAll);
    m_lightfx.SendAddAction(m_actionColor, 1, 2, color);
    m_lightfx.SendAnimationConfigSave(1);
    m_lightfx.SendAnimationSetDefault(1);
    m_lightfx.deviceRelease();
}

void EffectController::Breathe(uint32_t color) {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(1);
    m_lightfx.SendAnimationConfigStart(1);
    m_lightfx.SendZoneSelect(1, m_zoneAll);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 2000, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 2000, 64, 0);
    m_lightfx.SendAnimationConfigPlay(0);
    m_lightfx.SendAnimationConfigSave(1);
    m_lightfx.SendAnimationSetDefault(1);
    m_lightfx.deviceRelease();
}

void EffectController::Spectrum(uint16_t duration) {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(1);
    m_lightfx.SendAnimationConfigStart(1);
    m_lightfx.SendZoneSelect(1, m_zoneAll);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFF0000);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFA500);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFFF00);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x008000);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x00BFFF);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x0000FF);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x800080);
    m_lightfx.SendAnimationConfigSave(1);
    m_lightfx.SendAnimationSetDefault(1);
    m_lightfx.deviceRelease();
}

void EffectController::Wave(uint32_t color) {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(1);
    m_lightfx.SendAnimationConfigStart(1);

    m_lightfx.SendZoneSelect(1, m_zoneLeft);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);

    m_lightfx.SendZoneSelect(1, m_zoneMiddleLeft);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);

    m_lightfx.SendZoneSelect(1, m_zoneMiddleRight);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);

    m_lightfx.SendZoneSelect(1, m_zoneRight);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);

    // m_lightfx.SendAnimationConfigPlay(1);
    m_lightfx.SendAnimationConfigSave(1);
    m_lightfx.SendAnimationSetDefault(1);
    m_lightfx.deviceRelease();
}

void EffectController::Rainbow(uint16_t duration) {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(1);
    m_lightfx.SendAnimationConfigStart(1);

    m_lightfx.SendZoneSelect(1, m_zoneLeft);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFF0000);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFA500);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFFF00);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x008000);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x00BFFF);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x0000FF);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x800080);

    m_lightfx.SendZoneSelect(1, m_zoneMiddleLeft);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x800080);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFF0000);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFA500);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFFF00);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x008000);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x00BFFF);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x0000FF);

    m_lightfx.SendZoneSelect(1, m_zoneMiddleRight);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x0000FF);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x800080);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFF0000);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFA500);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFFF00);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x008000);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x00BFFF);

    m_lightfx.SendZoneSelect(1, m_zoneRight);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x00BFFF);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x0000FF);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x800080);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFF0000);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFA500);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFFF00);
    m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x008000);

    m_lightfx.SendAnimationConfigSave(1);
    m_lightfx.SendAnimationSetDefault(1);
    // m_lightfx.SendAnimationConfigPlay(1);
    m_lightfx.deviceRelease();
}

void EffectController::BackAndForth(uint32_t color) {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(1);
    m_lightfx.SendAnimationConfigStart(1);

    m_lightfx.SendZoneSelect(1, m_zoneLeft);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);

    m_lightfx.SendZoneSelect(1, m_zoneMiddleLeft);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);

    m_lightfx.SendZoneSelect(1, m_zoneMiddleRight);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);

    m_lightfx.SendZoneSelect(1, m_zoneRight);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);

    // m_lightfx.SendAnimationConfigPlay(1);
    m_lightfx.SendAnimationConfigSave(1);
    m_lightfx.SendAnimationSetDefault(1);
    m_lightfx.deviceRelease();
}

void EffectController::DefaultBlue() {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(1);
    m_lightfx.SendAnimationConfigStart(1);
    m_lightfx.SendZoneSelect(1, m_zoneAll);
    m_lightfx.SendAddAction(m_actionColor, 1, 2, 0x00FFFF);
    m_lightfx.SendAnimationConfigSave(1);
    m_lightfx.SendAnimationSetDefault(1);
    m_lightfx.deviceRelease();
}
