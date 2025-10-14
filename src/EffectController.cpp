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

void EffectController::Brightness(uint8_t value) {
    value = std::min<int>(value, 100);
    m_lightfx.deviceAcquire();
    m_lightfx.SendSetDim(100 - value, m_zoneAll);
    m_lightfx.deviceRelease();
    std::ofstream ofs(m_brightnessFile, std::ios::trunc);
    if (ofs.is_open()) {
        ofs << static_cast<int>(value);
        chmod(m_brightnessFile.c_str(), 0666); // set permission
    }
    LOG_S(INFO) << "Device Brightness set to: " << static_cast<int>(value)
                << "%";
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

// TODO: DOCUMENT LIGHTBAR
void EffectController::LightBarBrightness(uint8_t value) {
    value = std::min<int>(value, 100);
    m_lightfx.deviceAcquire();
    m_lightfx.SendSetDim(100 - value, m_lightbar);
    m_lightfx.deviceRelease();
    // std::ofstream ofs(m_brightnessFile, std::ios::trunc);
    // if (ofs.is_open()) {
    //     ofs << static_cast<int>(value);
    //     chmod(m_brightnessFile.c_str(), 0666); // set permission
    // }
}

void EffectController::LightBarSpectrum(uint16_t duration) {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(1);
    m_lightfx.SendAnimationConfigStart(1);
    m_lightfx.SendZoneSelect(1, m_lightbar);
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

void EffectController::LightBarBreathe(uint32_t color) {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(1);
    m_lightfx.SendAnimationConfigStart(1);
    m_lightfx.SendZoneSelect(1, m_lightbar);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 2000, 64, color);
    m_lightfx.SendAddAction(m_actionMorph, 500, 64, 0);
    m_lightfx.SendAddAction(m_actionMorph, 2000, 64, 0);
    m_lightfx.SendAnimationConfigSave(1);
    m_lightfx.SendAnimationSetDefault(1);
    m_lightfx.deviceRelease();
}

void EffectController::LightbarRainbow(uint16_t duration) {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(1);
    m_lightfx.SendAnimationConfigStart(1);

    const std::array<uint32_t, 7> colors = {
        0xFF0000, // Red
        0xFFA500, // Orange
        0xFFFF00, // Yellow
        0x008000, // Green
        0x00BFFF, // Sky Blue
        0x0000FF, // Blue
        0x800080  // Purple
    };

    for (size_t i = 0; i < m_lightbar.size(); ++i) {
        std::vector<uint8_t> zone = {m_lightbar[i]};
        m_lightfx.SendZoneSelect(1, std::span<const uint8_t>(zone));

        for (size_t j = 0; j < colors.size(); ++j) {
            size_t colorIndex = (i + j) % colors.size();
            m_lightfx.SendAddAction(m_actionMorph, duration, 64,
                                    colors[colorIndex]);
        }
    }

    m_lightfx.SendAnimationConfigSave(1);
    m_lightfx.SendAnimationSetDefault(1);
    m_lightfx.deviceRelease();
}

void EffectController::LightBarDefaultBlue() {
    m_lightfx.deviceAcquire();
    m_lightfx.SendAnimationRemove(1);
    m_lightfx.SendAnimationConfigStart(1);
    m_lightfx.SendZoneSelect(1, m_lightbar);
    m_lightfx.SendAddAction(m_actionColor, 1, 2, 0x00FFFF);
    m_lightfx.SendAnimationConfigSave(1);
    m_lightfx.SendAnimationSetDefault(1);
    m_lightfx.deviceRelease();
}
