#pragma once
#include "database.h"
#include <bitset>

class AcpiUtils {
  private:
    int acpiModelId{};
    std::bitset<7> m_featureSetBits{"1111111"};
    std::bitset<8> m_thermalModeBits{"1111111"};
    std::bitset<6> m_lightingModesBits{"111111"};
    static auto getPrefix() -> const char *;

  public:
    AcpiUtils();
    void executeAcpiCommand(int arg1, int arg2 = 0x00, int arg3 = 0x00,
                            int arg4 = 0x00);
    [[nodiscard]] auto hasFeature(FeatureSet f) const -> bool;

    [[nodiscard]] auto hasThermalMode(ThermalModeSet m) const -> bool;

    [[nodiscard]] auto hasLightingMode(LightingSet l) const -> bool;
    void deviceInfo() const;
};
