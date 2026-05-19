#include "HIDController.h"
#include <initguid.h>
#include <setupapi.h>
#include <hidsdi.h>
#include <stdexcept>

#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "Hid.lib")

namespace rocket {

HIDController::HIDController() : _isOpen(false) {}

HIDController::~HIDController() {
    CloseController();
}

std::vector<HIDController::ControllerInfo> HIDController::EnumerateControllers() {
    std::vector<ControllerInfo> controllers;
    // Placeholder enumeration. Real implementation should use SetupDiGetClassDevs / HidD_GetAttributes.
    return controllers;
}

bool HIDController::OpenController(const ControllerInfo& info) {
    if (_isOpen) {
        CloseController();
    }

    _currentController = info;
    _currentController.deviceHandle = CreateFileA(
        info.devicePath.c_str(), GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED, nullptr);

    _isOpen = (_currentController.deviceHandle != INVALID_HANDLE_VALUE);
    return _isOpen;
}

bool HIDController::CloseController() {
    if (_isOpen && _currentController.deviceHandle) {
        CloseHandle(_currentController.deviceHandle);
        _currentController.deviceHandle = nullptr;
        _isOpen = false;
    }
    return true;
}

bool HIDController::ReadInput(std::vector<unsigned char>& inputData) {
    if (!_isOpen || !_currentController.deviceHandle) {
        return false;
    }

    inputData.resize(_currentController.inputReportSize);
    DWORD bytesRead = 0;
    BOOL result = ReadFile(_currentController.deviceHandle, inputData.data(), static_cast<DWORD>(inputData.size()), &bytesRead, nullptr);
    return result == TRUE && bytesRead > 0;
}

bool HIDController::WriteOutput(const std::vector<unsigned char>& outputData) {
    if (!_isOpen || !_currentController.deviceHandle) {
        return false;
    }

    DWORD bytesWritten = 0;
    BOOL result = WriteFile(_currentController.deviceHandle, outputData.data(), static_cast<DWORD>(outputData.size()), &bytesWritten, nullptr);
    return result == TRUE && bytesWritten == outputData.size();
}

HIDController::ControllerInfo HIDController::GetCurrentControllerInfo() const {
    return _currentController;
}

} // namespace rocket
