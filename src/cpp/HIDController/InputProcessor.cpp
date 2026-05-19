#include "InputProcessor.h"
#include <cmath>

namespace rocket {

InputProcessor::InputProcessor(const DeadzoneConfig& config)
    : _config(config) {
}

float InputProcessor::NormalizeAxis(short value) const {
    const float normalized = static_cast<float>(value) / 32767.0f;
    return std::max(-1.0f, std::min(1.0f, normalized));
}

float InputProcessor::ApplyDeadzone(float value) const {
    float absValue = std::fabs(value);
    if (absValue < _config.innerDeadzone) {
        return 0.0f;
    }

    float range = 1.0f - _config.outerDeadzone;
    float scaled = (absValue - _config.innerDeadzone) / std::max(0.0001f, 1.0f - _config.innerDeadzone);
    switch (_config.type) {
        case DeadzoneType::QUADRATIC:
            scaled = scaled * scaled;
            break;
        case DeadzoneType::CUBIC:
            scaled = scaled * scaled * scaled;
            break;
        case DeadzoneType::SMOOTH:
            scaled = scaled * scaled * (3.0f - 2.0f * scaled);
            break;
        default:
            break;
    }

    return (value < 0.0f ? -scaled : scaled);
}

InputProcessor::AnalogStickData InputProcessor::ProcessAnalogStick(short rawX, short rawY) {
    float x = NormalizeAxis(rawX);
    float y = NormalizeAxis(rawY);

    x = ApplyDeadzone(x);
    y = ApplyDeadzone(y);

    if (_config.enableSmoothing) {
        x = std::copysign(std::pow(std::fabs(x), 1.0f + _config.smoothingFactor), x);
        y = std::copysign(std::pow(std::fabs(y), 1.0f + _config.smoothingFactor), y);
    }

    x *= _config.sensitivity;
    y *= _config.sensitivity;

    float magnitude = std::sqrt(x * x + y * y);
    float angle = std::atan2(y, x);

    return {x, y, magnitude, angle};
}

void InputProcessor::UpdateConfig(const DeadzoneConfig& newConfig) {
    _config = newConfig;
}

void InputProcessor::ApplyCustomCurve(const std::vector<float>& curvePoints) {
    _curvePoints = curvePoints;
}

} // namespace rocket
