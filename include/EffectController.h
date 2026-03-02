#pragma once
#include "LightFX.h"
#include <cstdint>
#include <vector>
#include <array>
#include <loguru.hpp>

class EffectController {
  public:
    EffectController(LightFX &LightFX,
                     const std::vector<uint8_t> &kbdZones = {
                         0x00, 0x01, 0x02, 0x03})
        : m_lightfx(LightFX) {
        if (kbdZones.size() >= 4) {
            m_zoneLeft = {kbdZones[0]};
            m_zoneMiddleLeft = {kbdZones[1]};
            m_zoneMiddleRight = {kbdZones[2]};
            m_zoneRight = {kbdZones[3]};
            m_zoneAll = kbdZones;
        } else if (!kbdZones.empty()) {
            m_zoneLeft = {kbdZones[0]};
            m_zoneMiddleLeft = {kbdZones[0]};
            m_zoneMiddleRight = {kbdZones[0]};
            m_zoneRight = {kbdZones[0]};
            m_zoneAll = kbdZones;
        }

        m_lightfx.deviceOpen();
        LOG_S(INFO) << "Effect Controller initialized";
    };
    ~EffectController();

    void Brightness(uint8_t value);
    void LightBarBrightness(uint8_t value);
    void StaticColor(uint32_t color);
    void Breathe(uint32_t color);
    void LightBarBreathe(uint32_t color);
    void Spectrum(uint16_t duration);
    void LightBarSpectrum(uint16_t duration);
    void Wave(uint32_t color);
    void LightBarDefaultBlue();
    void Rainbow(uint16_t duration);
    void LightbarRainbow(uint16_t duration);
    void BackAndForth(uint32_t color);
    void DefaultBlue();
    void ScanZones();
    int getBrightness();

  private:
    LightFX &m_lightfx;
    std::string m_brightnessFile = "/tmp/brightness"; // path of your choice
    static constexpr uint8_t m_actionColor = 0x00;
    static constexpr uint8_t m_actionMorph = 0x02;
    // TODO: Add way to individually set color for each zone
    static constexpr std::array<uint8_t, 13> m_lightbar = {
        0x1, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9};

    std::vector<uint8_t> m_zoneLeft = {0x00};
    std::vector<uint8_t> m_zoneMiddleLeft = {0x01};
    std::vector<uint8_t> m_zoneMiddleRight = {0x02};
    std::vector<uint8_t> m_zoneRight = {0x03};
    std::vector<uint8_t> m_zoneAll = {0x00, 0x01, 0x02, 0x03};
};
