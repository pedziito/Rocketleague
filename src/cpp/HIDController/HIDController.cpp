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

    GUID hidGuid;
    HidD_GetHidGuid(&hidGuid);
    HDEVINFO deviceInfo = SetupDiGetClassDevsA(&hidGuid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (deviceInfo == INVALID_HANDLE_VALUE) {
        return controllers;
    }

    SP_DEVICE_INTERFACE_DATA interfaceData;
    interfaceData.cbSize = sizeof(interfaceData);

    for (DWORD index = 0; SetupDiEnumDeviceInterfaces(deviceInfo, nullptr, &hidGuid, index, &interfaceData); ++index) {
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetailA(deviceInfo, &interfaceData, nullptr, 0, &requiredSize, nullptr);
        if (requiredSize == 0) {
            continue;
        }

        std::vector<char> detailData(requiredSize);
        auto deviceInterfaceDetail = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA_A>(detailData.data());
        deviceInterfaceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);

        SP_DEVINFO_DATA devInfoData;
        devInfoData.cbSize = sizeof(devInfoData);

        if (!SetupDiGetDeviceInterfaceDetailA(deviceInfo, &interfaceData, deviceInterfaceDetail, requiredSize, nullptr, &devInfoData)) {
            continue;
        }

        std::string devicePath = deviceInterfaceDetail->DevicePath;
        std::string friendlyName = devicePath;

        CHAR propertyBuffer[256] = {};
        if (SetupDiGetDeviceRegistryPropertyA(deviceInfo, &devInfoData, SPDRP_FRIENDLYNAME, nullptr, reinterpret_cast<PBYTE>(propertyBuffer), sizeof(propertyBuffer), nullptr)) {
            friendlyName = propertyBuffer;
        } else if (SetupDiGetDeviceRegistryPropertyA(deviceInfo, &devInfoData, SPDRP_DEVICEDESC, nullptr, reinterpret_cast<PBYTE>(propertyBuffer), sizeof(propertyBuffer), nullptr)) {
            friendlyName = propertyBuffer;
        }

        ControllerInfo controller;
        controller.devicePath = devicePath;
        controller.deviceName = friendlyName;

        HANDLE hDevice = CreateFileA(devicePath.c_str(), GENERIC_READ | GENERIC_WRITE,
                                     FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                                     OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
        if (hDevice != INVALID_HANDLE_VALUE) {
            HIDD_ATTRIBUTES attributes;
            attributes.Size = sizeof(attributes);
            if (HidD_GetAttributes(hDevice, &attributes)) {
                controller.vendorID = attributes.VendorID;
                controller.productID = attributes.ProductID;
                controller.inputReportSize = 64;
                controller.outputReportSize = 64;
            }
            CloseHandle(hDevice);
        }

        controllers.push_back(controller);
    }

    SetupDiDestroyDeviceInfoList(deviceInfo);
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
