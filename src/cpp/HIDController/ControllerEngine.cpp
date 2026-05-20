#include "ControllerEngine.h"
#include "HIDController.h"
#include "InputProcessor.h"
#include "AntiDrift.h"
#include "../PerformanceMonitor/PerformanceMonitor.h"
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstring>

struct NativeControllerContext {
    std::unique_ptr<rocket::HIDController> hid;
    std::unique_ptr<rocket::InputProcessor> processor;
    std::unique_ptr<rocket::AntiDrift> drift;
    std::unique_ptr<rocket::PerformanceMonitor> monitor;
    std::vector<unsigned char> lastBuffer;
};

static std::string EscapeJsonString(const std::string& input) {
    std::string output;
    output.reserve(input.size() * 2);
    for (char c : input) {
        switch (c) {
            case '\\': output += "\\\\"; break;
            case '"': output += "\\\""; break;
            case '\b': output += "\\b"; break;
            case '\f': output += "\\f"; break;
            case '\n': output += "\\n"; break;
            case '\r': output += "\\r"; break;
            case '\t': output += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    char buf[7];
                    sprintf(buf, "\\u%04x", static_cast<unsigned char>(c));
                    output += buf;
                } else {
                    output += c;
                }
                break;
        }
    }
    return output;
}

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

    rocket::HIDController hid;
    auto controllers = hid.EnumerateControllers();
    std::string json = "[";

    for (size_t i = 0; i < controllers.size(); ++i) {
        const auto& controller = controllers[i];
        json += "{";
        json += "\"deviceName\":\"" + EscapeJsonString(controller.deviceName) + "\",";
        json += "\"devicePath\":\"" + EscapeJsonString(controller.devicePath) + "\",";
        json += "\"vendorID\":" + std::to_string(controller.vendorID) + ",";
        json += "\"productID\":" + std::to_string(controller.productID);
        json += "}";
        if (i + 1 < controllers.size()) {
            json += ",";
        }
    }

    json += "]";

    int count = std::min(static_cast<int>(json.size()), std::max(0, outSize - 1));
    memcpy(out, json.data(), count);
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
