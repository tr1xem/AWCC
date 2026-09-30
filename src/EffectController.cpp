#include "EffectController.h"
#include <algorithm> // for std::min
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <sys/stat.h>
#include <vector>
using std::mt19937;

EffectController::~EffectController() {
    m_lightfx.deviceClose();
    LOG_S(INFO) << "Effect Controller deinitialized";
}

void EffectController::BrightnessZones(uint8_t value,
                                       const std::vector<uint8_t> &zones,
                                       bool persist) {
    if (zones.empty())
        return;
    value = std::min<int>(value, 100);
    m_lightfx.deviceAcquire();
    m_lightfx.SendSetDim(100 - value, zones);
    m_lightfx.deviceRelease();
    if (!persist)
        return;
    std::ofstream ofs(m_brightnessFile, std::ios::trunc);
    if (ofs.is_open()) {
        ofs << static_cast<int>(value);
        chmod(m_brightnessFile.c_str(), 0666); // set permission
    }
    LOG_S(INFO) << "Device Brightness set to: " << static_cast<int>(value)
                << "%";
}

void EffectController::Brightness(uint8_t value) {
    BrightnessZones(value, m_zoneAll, true);
}

int EffectController::getBrightness() {
    std::ifstream ifs(m_brightnessFile);
    int value = 0;
    if (ifs.is_open()) {
        ifs >> value;
        value = std::min(value, 100);
    } else {
        // File does not exist, create it with a default value of 50
        value = 50;
        std::ofstream ofs(m_brightnessFile, std::ios::trunc);
        if (ofs.is_open()) {
            ofs << value;
            ofs.close();
            // Set permissions to 0660 (rw-rw----)
            chmod(m_brightnessFile.c_str(), 0666);
        }
    }
    return value;
}

void EffectController::EmitStatic(const std::vector<uint8_t> &zones,
                                  uint32_t color) {
    for (uint8_t zoneId : zones) {
        std::vector<uint8_t> zone = {zoneId};
        m_lightfx.SendZoneSelect(1, std::span<const uint8_t>(zone));
        m_lightfx.SendAddAction(m_actionColor, 1, 2, color);
    }
}

void EffectController::EmitBreathe(const std::vector<uint8_t> &zones,
                                   uint32_t color) {
    for (uint8_t zoneId : zones) {
        std::vector<uint8_t> zone = {zoneId};
        m_lightfx.SendZoneSelect(1, std::span<const uint8_t>(zone));
        m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);
        m_lightfx.SendAddAction(m_actionMorph, 2000, 64, color);
        m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
        m_lightfx.SendAddAction(m_actionMorph, 2000, 64, 0);
    }
}

void EffectController::EmitSpectrum(const std::vector<uint8_t> &zones,
                                    uint16_t duration) {
    for (uint8_t zoneId : zones) {
        std::vector<uint8_t> zone = {zoneId};
        m_lightfx.SendZoneSelect(1, std::span<const uint8_t>(zone));
        m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFF0000);
        m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFA500);
        m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0xFFFF00);
        m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x008000);
        m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x00BFFF);
        m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x0000FF);
        m_lightfx.SendAddAction(m_actionMorph, duration, 64, 0x800080);
    }
}

void EffectController::EmitWave(const std::vector<uint8_t> &zones,
                                uint32_t color) {
    const size_t n = zones.size();
    for (size_t i = 0; i < n; ++i) {
        std::vector<uint8_t> zone = {zones[i]};
        m_lightfx.SendZoneSelect(1, zone);
        for (size_t j = 0; j < n; ++j) {
            m_lightfx.SendAddAction(m_actionMorph, 500, 64,
                                    (i == j) ? color : 0);
        }
    }
}

void EffectController::EmitRainbow(const std::vector<uint8_t> &zones,
                                   uint16_t duration) {
    const std::array<uint32_t, 7> colors = {
        0xFF0000, // Red
        0xFFA500, // Orange
        0xFFFF00, // Yellow
        0x008000, // Green
        0x00BFFF, // Sky Blue
        0x0000FF, // Blue
        0x800080  // Purple
    };

    const size_t n = zones.size();
    for (size_t i = 0; i < n; ++i) {
        std::vector<uint8_t> zone = {zones[i]};
        m_lightfx.SendZoneSelect(1, zone);
        for (size_t j = 0; j < colors.size(); ++j) {
            size_t colorIndex = (i + j) % colors.size();
            m_lightfx.SendAddAction(m_actionMorph, duration, 64,
                                    colors[colorIndex]);
        }
    }
}

void EffectController::EmitBackAndForth(const std::vector<uint8_t> &zones,
                                        uint32_t color) {
    const size_t n = zones.size();
    if (n <= 1) {
        EmitStatic(zones, color);
        return;
    }
    std::vector<size_t> sequence;
    for (size_t i = 0; i < n; ++i)
        sequence.push_back(i);
    for (size_t i = n - 2; i > 0; --i)
        sequence.push_back(i);

    for (size_t i = 0; i < n; ++i) {
        std::vector<uint8_t> zone = {zones[i]};
        m_lightfx.SendZoneSelect(1, zone);
        for (size_t step : sequence) {
            m_lightfx.SendAddAction(m_actionMorph, 500, 64,
                                    (i == step) ? color : 0);
        }
    }
}

void EffectController::EmitDefaultBlue(const std::vector<uint8_t> &zones) {
    for (uint8_t zoneId : zones) {
        std::vector<uint8_t> zone = {zoneId};
        m_lightfx.SendZoneSelect(1, std::span<const uint8_t>(zone));
        m_lightfx.SendAddAction(m_actionColor, 2000, 250, 0x00FFFF);
    }
}

void EffectController::ApplyPrograms(
    const std::vector<ChassisProgram> &programs) {
    bool any = false;
    for (const ChassisProgram &program : programs) {
        if (!program.zones.empty())
            any = true;
    }
    if (!any)
        return;

    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(0x0061);
    m_lightfx.SendAnimationConfigStart(0x0061);
    for (const ChassisProgram &program : programs) {
        if (program.zones.empty())
            continue;
        switch (program.mode) {
        case 1:
            EmitBreathe(program.zones, program.color);
            break;
        case 2:
            EmitSpectrum(program.zones, 1000);
            break;
        case 3:
            EmitWave(program.zones, program.color);
            break;
        case 4:
            EmitRainbow(program.zones, 500);
            break;
        case 5:
            EmitBackAndForth(program.zones, program.color);
            break;
        case 6:
            EmitDefaultBlue(program.zones);
            break;
        default:
            EmitStatic(program.zones, program.color);
            break;
        }
    }
    m_lightfx.SendAnimationConfigSave(0x0061);
    m_lightfx.SendAnimationSetDefault(0x0061);
    m_lightfx.deviceRelease();
}

void EffectController::ClearPrograms() {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(0x0061);
    m_lightfx.deviceRelease();
}

void EffectController::StaticColor(uint32_t color) {
    ApplyPrograms({ChassisProgram{m_zoneAll, 0, color}});
}

void EffectController::Breathe(uint32_t color) {
    ApplyPrograms({ChassisProgram{m_zoneAll, 1, color}});
}

void EffectController::Spectrum(uint16_t duration) {
    (void)duration;
    ApplyPrograms({ChassisProgram{m_zoneAll, 2, 0}});
}

void EffectController::Wave(uint32_t color) {
    ApplyPrograms({ChassisProgram{m_zoneAll, 3, color}});
}

void EffectController::Rainbow(uint16_t duration) {
    (void)duration;
    ApplyPrograms({ChassisProgram{m_zoneAll, 4, 0}});
}

void EffectController::BackAndForth(uint32_t color) {
    ApplyPrograms({ChassisProgram{m_zoneAll, 5, color}});
}

void EffectController::DefaultBlue() {
    ApplyPrograms({ChassisProgram{m_zoneAll, 6, 0x00FFFF}});
}

static uint32_t m_randomColor() {
    static const uint32_t colors[] = {
        0xFF0000, // Red
        0x00FF00, // Green
        0x0000FF, // Blue
        0xFFFF00, // Yellow
        0xFF00FF, // Magenta
        0x00FFFF, // Cyan
        0xFF8000, // Orange
        0x8000FF, // Purple
    };

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, std::size(colors) - 1);
    return colors[dist(gen)];
}

void EffectController::ScanZones() {
    std::map<std::string, int> zonesFound;
    int maxZone = 0x8000;

    int zone = 1;
    for (; zone <= 0x20; ++zone) {
        uint32_t color = m_randomColor();
        std::vector<uint8_t> zoneVec = {static_cast<uint8_t>(zone)};

        m_lightfx.deviceAcquire();
        m_lightfx.SendSetDim(0, std::span<const uint8_t>(zoneVec));
        m_lightfx.SendAnimationConfigStart(0);
        m_lightfx.SendZoneSelect(1, std::span<const uint8_t>(zoneVec));
        m_lightfx.SendAddAction(m_actionColor, 1, 2, color);
        m_lightfx.SendAnimationConfigPlay(0);
        m_lightfx.deviceRelease();

        while (true) {
            std::cout << "Is zone 0x" << std::hex << zone
                      << " colored? (y/n): ";
            std::string reply;
            std::getline(std::cin >> std::ws, reply);

            if (!reply.empty() && (reply[0] == 'y' || reply[0] == 'Y')) {
                std::cout << "Enter a name for this zone: ";
                std::string zonename;
                std::getline(std::cin >> std::ws, zonename);
                zonesFound[zonename] = zone;
                break;
            } else if (!reply.empty() && (reply[0] == 'n' || reply[0] == 'N')) {
                break;
            } else {
                std::cout << "Please enter only 'y' or 'n'." << '\n';
            }
        }
    }
    for (zone = 0x40; zone <= maxZone; zone *= 2) {
        std::vector<uint8_t> zoneVec = {static_cast<uint8_t>(zone)};
        uint32_t color = m_randomColor();

        m_lightfx.deviceAcquire();
        m_lightfx.SendSetDim(0, std::span<const uint8_t>(zoneVec));
        m_lightfx.SendAnimationConfigStart(0);
        m_lightfx.SendZoneSelect(1, std::span<const uint8_t>(zoneVec));
        m_lightfx.SendAddAction(m_actionColor, 1, 2, color);
        m_lightfx.SendAnimationConfigPlay(0);
        m_lightfx.deviceRelease();

        while (true) {
            std::cout << "Is zone 0x" << std::hex << zone
                      << " colored? (y/n): ";
            std::string reply;
            std::getline(std::cin >> std::ws, reply);

            if (!reply.empty() && (reply[0] == 'y' || reply[0] == 'Y')) {
                std::cout << "Enter a name for this zone: ";
                std::string zonename;
                std::getline(std::cin >> std::ws, zonename);
                zonesFound[zonename] = zone;
                break;
            } else if (!reply.empty() && (reply[0] == 'n' || reply[0] == 'N')) {
                break;
            } else {
                std::cout << "Please enter only 'y' or 'n'." << '\n';
            }
        }
    }

    std::cout << "Zones found:\n";
    for (const auto &z : zonesFound) {
        std::cout << z.first << ": 0x" << std::hex << z.second << "\n";
    }
}
