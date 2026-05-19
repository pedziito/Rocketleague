#pragma once

#include <vector>
#include "../shared/Structures.h"

namespace rocket {

class InputProcessor {
public:
    enum class DeadzoneType { LINEAR, QUADRATIC, CUBIC, SMOOTH };

    struct DeadzoneConfig {
        DeadzoneType type = DeadzoneType::LINEAR;
        float innerDeadzone = 0.05f;
        float outerDeadzone = 0.95f;
        float sensitivity = 1.0f;
        bool enableSmoothing = false;
        float smoothingFactor = 0.5f;
    };

    struct AnalogStickData {
        float x = 0.0f;
        float y = 0.0f;
        float magnitude = 0.0f;
        float angle = 0.0f;
    };

    InputProcessor(const DeadzoneConfig& config);
    AnalogStickData ProcessAnalogStick(short rawX, short rawY);
    void UpdateConfig(const DeadzoneConfig& newConfig);
    void ApplyCustomCurve(const std::vector<float>& curvePoints);

private:
    DeadzoneConfig _config;
    std::vector<float> _curvePoints;
    float NormalizeAxis(short value) const;
    float ApplyDeadzone(float value) const;
};

} // namespace rocket
