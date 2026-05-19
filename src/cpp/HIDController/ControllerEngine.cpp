#include "ControllerEngine.h"
#include "HIDController.h"
#include "InputProcessor.h"
#include "AntiDrift.h"
#include "../PerformanceMonitor/PerformanceMonitor.h"
#include <vector>
#include <memory>

struct NativeControllerContext {
    std::unique_ptr<rocket::HIDController> hid;
    std::unique_ptr<rocket::InputProcessor> processor;
    std::unique_ptr<rocket::AntiDrift> drift;
    std::unique_ptr<rocket::PerformanceMonitor> monitor;
    std::vector<unsigned char> lastBuffer;
};

extern "C" {

void* CreateController() {
    auto context = new NativeControllerContext();
    context->hid = std::make_unique<rocket::HIDController>();
    rocket::InputProcessor::DeadzoneConfig config;
    context->processor = std::make_unique<rocket::InputProcessor>(config);
    rocket::AntiDrift::DriftProfile profile;
    context->drift = std::make_unique<rocket::AntiDrift>(profile);
    context->monitor = std::make_unique<rocket::PerformanceMonitor>();
    return context;
}

void DestroyController(void* handle) {
    delete static_cast<NativeControllerContext*>(handle);
}

int EnumerateControllers(char* out, int outSize) {
    if (!out) {
        return 0;
    }
    std::string placeholder = "[]";
    int count = static_cast<int>(placeholder.copy(out, std::max(0, outSize - 1)));
    out[count] = '\0';
    return count;
}

bool OpenController(void* handle, int index) {
    if (!handle) {
        return false;
    }
    auto context = static_cast<NativeControllerContext*>(handle);
    auto controllers = context->hid->EnumerateControllers();
    if (index < 0 || index >= static_cast<int>(controllers.size())) {
        return false;
    }
    return context->hid->OpenController(controllers[index]);
}

bool ReadInput(void* handle, unsigned char* buffer, int* size) {
    if (!handle || !buffer || !size) {
        return false;
    }
    auto context = static_cast<NativeControllerContext*>(handle);
    std::vector<unsigned char> input;
    if (!context->hid->ReadInput(input)) {
        return false;
    }
    int bytesToCopy = std::min(*size, static_cast<int>(input.size()));
    memcpy(buffer, input.data(), bytesToCopy);
    *size = bytesToCopy;
    return true;
}

void SetDeadzoneConfig(void* handle, float inner, float outer, int type) {
    if (!handle) {
        return;
    }

    auto context = static_cast<NativeControllerContext*>(handle);
    rocket::InputProcessor::DeadzoneConfig config;
    config.innerDeadzone = inner;
    config.outerDeadzone = outer;
    config.type = static_cast<rocket::InputProcessor::DeadzoneType>(type);
    context->processor->UpdateConfig(config);
}

int GetPollingRate(void* handle) {
    if (!handle) {
        return 0;
    }
    return 1000;
}

float* GetPerformanceMetrics(void* handle) {
    (void)handle;
    static float metrics[8] = {0};
    return metrics;
}

}
