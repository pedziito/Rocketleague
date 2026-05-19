#pragma once

#include <string>
#include "../shared/Structures.h"

namespace rocket {

class PerformanceMonitor {
public:
    void StartMonitoring();
    void StopMonitoring();
    shared::PerformanceMetrics GetMetrics() const;
    void ResetMetrics();
    void LogMetricsToFile(const std::string& filename) const;

private:
    shared::PerformanceMetrics _metrics;
    bool _monitoring = false;
    long long _startTimeMs = 0;
    void UpdateCpuUsage();
};

} // namespace rocket
