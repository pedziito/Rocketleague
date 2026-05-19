#include "AntiDrift.h"
#include <numeric>
#include <cmath>

namespace rocket {

AntiDrift::AntiDrift(const DriftProfile& profile)
    : _profile(profile), _offsetX(0.0f), _offsetY(0.0f) {
}

void AntiDrift::Calibrate(const std::vector<InputProcessor::AnalogStickData>& samples) {
    if (samples.empty()) {
        return;
    }

    float avgX = 0.0f;
    float avgY = 0.0f;

    for (const auto& sample : samples) {
        avgX += sample.x;
        avgY += sample.y;
    }

    avgX /= static_cast<float>(samples.size());
    avgY /= static_cast<float>(samples.size());

    _offsetX = std::fabs(avgX) < _profile.driftThreshold ? 0.0f : avgX;
    _offsetY = std::fabs(avgY) < _profile.driftThreshold ? 0.0f : avgY;
}

InputProcessor::AnalogStickData AntiDrift::CorrectDrift(const InputProcessor::AnalogStickData& input) {
    InputProcessor::AnalogStickData output = input;
    output.x -= _offsetX;
    output.y -= _offsetY;

    if (std::fabs(output.x) < _profile.driftThreshold) {
        output.x = 0.0f;
    }
    if (std::fabs(output.y) < _profile.driftThreshold) {
        output.y = 0.0f;
    }

    if (_profile.enableAdaptive) {
        _offsetX = _offsetX * 0.95f + input.x * 0.05f;
        _offsetY = _offsetY * 0.95f + input.y * 0.05f;
    }

    output.magnitude = std::sqrt(output.x * output.x + output.y * output.y);
    output.angle = std::atan2(output.y, output.x);
    return output;
}

void AntiDrift::UpdateProfile(const DriftProfile& newProfile) {
    _profile = newProfile;
}

} // namespace rocket
