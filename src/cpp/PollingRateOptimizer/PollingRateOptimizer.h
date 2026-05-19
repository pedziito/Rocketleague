#pragma once

#include <atomic>
#include <thread>
#include <mutex>

namespace rocket {

class PollingRateOptimizer {
public:
    struct PollingConfig {
        int targetPollingRate = 1000;
        bool enableForcePolling = true;
        int cpuCore = 0;
        bool prioritizeThread = true;
        int threadPriority = THREAD_PRIORITY_HIGHEST;
    };

    explicit PollingRateOptimizer(const PollingConfig& config);
    ~PollingRateOptimizer();

    int GetCurrentPollingRate() const;
    bool SetPollingRate(int hz);
    bool StartHighPriorityReadThread();
    void StopReadThread();
    int MeasureActualPollingRate();

private:
    PollingConfig _config;
    std::atomic<bool> _running;
    std::thread _readThread;
    std::mutex _lock;
    int _currentRate;
    void ReadLoop();
};

} // namespace rocket
