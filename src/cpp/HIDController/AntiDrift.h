#pragma once

#include <vector>
#include "InputProcessor.h"

namespace rocket {

class AntiDrift {
public:
    struct DriftProfile {
        float driftThreshold = 0.02f;
        float calibrationTime = 3.0f;
        bool enableAdaptive = false;
        std::vector<float> offsetX;
        std::vector<float> offsetY;
    };

    explicit AntiDrift(const DriftProfile& profile);
    void Calibrate(const std::vector<InputProcessor::AnalogStickData>& samples);
    InputProcessor::AnalogStickData CorrectDrift(const InputProcessor::AnalogStickData& input);
    void UpdateProfile(const DriftProfile& newProfile);

private:
    DriftProfile _profile;
    float _offsetX = 0.0f;
    float _offsetY = 0.0f;
};

} // namespace rocket
