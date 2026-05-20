#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <string>
#include <vector>
#include <windows.h>

namespace rocket {

class HIDController {
public:
    struct ControllerInfo {
        HANDLE deviceHandle = nullptr;
        unsigned int vendorID = 0;
        unsigned int productID = 0;
        std::string devicePath;
        std::string deviceName;
        int inputReportSize = 0;
        int outputReportSize = 0;
    };

    HIDController();
    ~HIDController();

    std::vector<ControllerInfo> EnumerateControllers();
    bool OpenController(const ControllerInfo& info);
    bool CloseController();
    bool ReadInput(std::vector<unsigned char>& inputData);
    bool WriteOutput(const std::vector<unsigned char>& outputData);
    ControllerInfo GetCurrentControllerInfo() const;

private:
    ControllerInfo _currentController;
    bool _isOpen;
};

} // namespace rocket
