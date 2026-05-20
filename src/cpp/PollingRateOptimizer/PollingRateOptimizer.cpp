#include "PollingRateOptimizer.h"
#include <windows.h>
#include <chrono>
#include <algorithm>

namespace rocket {

PollingRateOptimizer::PollingRateOptimizer(const PollingConfig& config)
    : _config(config), _running(false), _currentRate(config.targetPollingRate) {
}

PollingRateOptimizer::~PollingRateOptimizer() {
    StopReadThread();
}

int PollingRateOptimizer::GetCurrentPollingRate() const {
    return _currentRate;
}

bool PollingRateOptimizer::SetPollingRate(int hz) {
    _config.targetPollingRate = hz;
    _currentRate = hz;
    return true;
}

bool PollingRateOptimizer::StartHighPriorityReadThread() {
    if (_running.load()) {
        return false;
    }

    _running.store(true);
    _readThread = std::thread(&PollingRateOptimizer::ReadLoop, this);
    if (_config.prioritizeThread) {
        SetThreadPriority(_readThread.native_handle(), _config.threadPriority);
    }
    return true;
}

void PollingRateOptimizer::StopReadThread() {
    if (!_running.load()) {
        return;
    }
    _running.store(false);
    if (_readThread.joinable()) {
        _readThread.join();
    }
}

int PollingRateOptimizer::MeasureActualPollingRate() {
    return _currentRate;
}

void PollingRateOptimizer::ReadLoop() {
    if (_config.prioritizeThread) {
        SetThreadPriority(GetCurrentThread(), _config.threadPriority);
    }
    if (_config.cpuCore >= 0) {
        DWORD_PTR mask = 1ull << _config.cpuCore;
        SetThreadAffinityMask(GetCurrentThread(), mask);
    }

    while (_running.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / std::max(1, _config.targetPollingRate)));
    }
}

} // namespace rocket
