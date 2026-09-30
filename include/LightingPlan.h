#pragma once

#include <cstdint>
#include <vector>

// Chassis pages are split with the Alienware 16 Area-51 AA16250 zone numbers.
// Another model's zones still go through All lighting as one list. A zone
// only shows on a named page when its id falls in that range.

inline std::vector<uint8_t> ZonesInRange(const std::vector<uint8_t> &all,
                                         uint8_t lo, uint8_t hi) {
    std::vector<uint8_t> out;
    for (uint8_t zone : all) {
        if (zone >= lo && zone <= hi)
            out.push_back(zone);
    }
    return out;
}

// Zone 0x1b is the Area-51 power button. Rainbow for that LED is powerrainbow.
// A chassis rainbow on it strobes.
inline std::vector<uint8_t> ZonesForEffect(const std::vector<uint8_t> &zones,
                                           int mode) {
    if (mode != 4)
        return zones;
    std::vector<uint8_t> out;
    out.reserve(zones.size());
    for (uint8_t zone : zones) {
        if (zone != 0x1b)
            out.push_back(zone);
    }
    return out;
}

struct ChassisSplit {
    std::vector<uint8_t> lightbar;
    std::vector<uint8_t> logo;
    std::vector<uint8_t> speakers;
    std::vector<uint8_t> trackpad;
    std::vector<uint8_t> other;
};

inline ChassisSplit SplitChassisZones(const std::vector<uint8_t> &all) {
    ChassisSplit split;
    split.lightbar = ZonesInRange(all, 0x02, 0x1A);
    split.logo = ZonesInRange(all, 0x1C, 0x1C);
    split.speakers = ZonesInRange(all, 0x1D, 0x1E);
    split.trackpad = ZonesInRange(all, 0x1F, 0x28);
    for (uint8_t zone : all) {
        const bool named = (zone >= 0x02 && zone <= 0x1A) || zone == 0x1C ||
                           (zone >= 0x1D && zone <= 0x1E) ||
                           (zone >= 0x1F && zone <= 0x28);
        if (!named)
            split.other.push_back(zone);
    }
    return split;
}

// Menu index to alienfx_cli keyboardeffect name. Static, default blue, and
// an unknown index are set with setkeys, not a named effect. Back and Forth
// is wave because the keyboard firmware has no back-and-forth command.
inline const char *KeyboardCliEffect(int effect) {
    switch (effect) {
    case 1:
        return "breathe";
    case 2:
        return "spectrum";
    case 3:
        return "wave";
    case 4:
        return "rainbow";
    case 5:
        return "wave";
    case 7:
        return "pulse";
    default:
        return nullptr;
    }
}
