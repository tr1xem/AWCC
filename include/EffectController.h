#pragma once
#include "LightFX.h"
#include <array>
#include <cstdint>
#include <loguru.hpp>
#include <vector>

struct ChassisProgram {
    std::vector<uint8_t> zones;
    int mode = 0;
    uint32_t color = 0;
};

class EffectController {
  public:
    EffectController(LightFX &LightFX) : m_lightfx(LightFX) {
        m_lightfx.deviceOpen();
        LOG_S(INFO) << "Effect Controller initialized";
    };

    void setZones(const std::vector<uint8_t> &kbdZones) {
        m_zoneAll = kbdZones;
        LOG_S(INFO) << "Keyboard zones updated to " << m_zoneAll.size()
                    << " zones";
    }

    // One chassis animation. Each program is a zone group with its own effect,
    // so changing the logo can keep a lightbar color already set this session.
    void ApplyPrograms(const std::vector<ChassisProgram> &programs);
    void ClearPrograms();
    void BrightnessZones(uint8_t value, const std::vector<uint8_t> &zones,
                         bool persist);
    ~EffectController();

    void Brightness(uint8_t value);
    void StaticColor(uint32_t color);
    void Breathe(uint32_t color);
    void Spectrum(uint16_t duration);
    void Wave(uint32_t color);
    void Rainbow(uint16_t duration);
    void BackAndForth(uint32_t color);
    void DefaultBlue();
    void ScanZones();
    int getBrightness();

  private:
    void EmitStatic(const std::vector<uint8_t> &zones, uint32_t color);
    void EmitBreathe(const std::vector<uint8_t> &zones, uint32_t color);
    void EmitSpectrum(const std::vector<uint8_t> &zones, uint16_t duration);
    void EmitWave(const std::vector<uint8_t> &zones, uint32_t color);
    void EmitRainbow(const std::vector<uint8_t> &zones, uint16_t duration);
    void EmitBackAndForth(const std::vector<uint8_t> &zones, uint32_t color);
    void EmitDefaultBlue(const std::vector<uint8_t> &zones);

    LightFX &m_lightfx;
    std::string m_brightnessFile = "/etc/awcc/brightness";
    static constexpr uint8_t m_actionColor = 0x00;
    static constexpr uint8_t m_actionMorph = 0x02;
    // Keyboard zones fetched from database
    std::vector<uint8_t> m_zoneAll = {0x00, 0x01, 0x02, 0x03};
};
