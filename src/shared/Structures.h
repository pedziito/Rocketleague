#pragma once

#include <string>
#include <vector>

namespace shared {

struct AnalogStickData {
    float x;
    float y;
    float magnitude;
    float angle;
};

struct DeadzoneConfig {
    enum class DeadzoneType { LINEAR, QUADRATIC, CUBIC, SMOOTH };
    DeadzoneType type;
    float innerDeadzone;
    float outerDeadzone;
    float sensitivity;
    bool enableSmoothing;
    float smoothingFactor;
};

struct DriftProfile {
    float driftThreshold;
    float calibrationTime;
    bool enableAdaptive;
    std::vector<float> offsetX;
    std::vector<float> offsetY;
};

struct PollingConfig {
    int targetPollingRate;
    bool enableForcePolling;
    int cpuCore;
    bool prioritizeThread;
    int threadPriority;
};

struct PerformanceMetrics {
    float averageLatency;
    float peakLatency;
    float averagePollingRate;
    float inputJitter;
    int droppedFrames;
    float cpuUsage;
    long long uptimeMs;
};

} // namespace shared
