#pragma once

#include "Thermals.h"

struct GLFWwindow;

struct FanSensors {
    int cpuC = -1;
    int gpuC = -1;
    int cpuRpm = -1;
    int gpuRpm = -1;
};

#include "FanCurveMath.h"

struct FanCurve {
    bool enabled = false;
    FanCurvePoint cpu[4]{{45, 0}, {65, 25}, {80, 60}, {95, 100}};
    FanCurvePoint gpu[4]{{50, 0}, {70, 30}, {82, 70}, {95, 100}};
    int seenCpuC = -1;
    int seenGpuC = -1;

    void load();
    void save() const;
    void tick(Thermals &thermals, int &cpuBoost, int &gpuBoost);
};

FanSensors ReadFanSensors();

bool NotifyRunningInstance();
void StartInstanceServer();
void PollInstanceServer(GLFWwindow *window);
void StopInstanceServer();

bool AutostartEnabled();
void SetAutostartEnabled(bool enabled);
