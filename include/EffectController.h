#pragma once
#include "LightFX.h"
#include <cstdint>
#include <loguru.hpp>

class EffectController {
  public:
    EffectController(LightFX &LightFX) : m_lightfx(LightFX) {
        m_lightfx.deviceOpen();
        LOG_S(INFO) << "Effect Controller intialized";
    };
    ~EffectController() {
        m_lightfx.deviceClose();
        LOG_S(INFO) << "Effect Controller deintialized";
    };

    void Brightness(uint8_t value);
    void StaticColor(uint32_t color);
    void Breathe(uint32_t color);
    void Spectrum(uint16_t duration);
    void Wave(uint32_t color);
    void Rainbow(uint16_t duration);
    void BackAndForth(uint32_t color);
    void DefaultBlue();
    int getBrightness();

  private:
    LightFX &m_lightfx;
    std::string m_brightnessFile =
        "/tmp/device_brightness.txt"; // path of your choice
    static constexpr uint8_t m_actionColor = 0x00;
    static constexpr uint8_t m_actionMorph = 0x02;
    static constexpr std::array<uint8_t, 1> m_zoneLeft = {0x00};
    static constexpr std::array<uint8_t, 1> m_zoneMiddleLeft = {0x01};
    static constexpr std::array<uint8_t, 1> m_zoneMiddleRight = {0x02};
    static constexpr std::array<uint8_t, 1> m_zoneRight = {0x03};
    static constexpr std::array<uint8_t, 4> m_zoneAll = {
        *m_zoneLeft.data(), *m_zoneMiddleLeft.data(), *m_zoneMiddleRight.data(),
        *m_zoneRight.data()};
};
