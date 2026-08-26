#pragma once

enum AWCCPowerState_t {
    AWCCPowerStateAC = 0,  // NOTE: these values are important to keep as is
    AWCCPowerStateBAT = 1,
};

class Helper {
   public:
    static AWCCPowerState_t PowerState();
};
