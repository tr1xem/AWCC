#pragma once
#include "database.h"
#include <bitset>

class AcpiUtils {
  private:
    int acpiModelId{};
    std::bitset<7> m_featureSetBits{"1111111"};
    std::bitset<8> m_thermalModeBits{"1111111"};
    std::bitset<6> m_lightingModesBits{"111111"};

  public:
    AcpiUtils();
    void executeAcpiCommand(const char *prefix, int arg1, int arg2 = 0x00,
                            int arg3 = 0x00, int arg4 = 0x00);
    bool hasFeature(FeatureSet f) const {
        return (m_featureSetBits.to_ulong() & static_cast<unsigned long>(f)) !=
               0;
    }

    bool hasThermalMode(ThermalModeSet m) const {
        return (m_thermalModeBits.to_ulong() & static_cast<unsigned long>(m)) !=
               0;
    }

    bool hasLightingMode(LightingSet l) const {
        return (m_lightingModesBits.to_ulong() &
                static_cast<unsigned long>(l)) != 0;
    }
    void deviceInfo() const;
};
