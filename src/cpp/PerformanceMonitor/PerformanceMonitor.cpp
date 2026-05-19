#include "PerformanceMonitor.h"
#include <fstream>
#include <chrono>
#include <Windows.h>

namespace rocket {

void PerformanceMonitor::StartMonitoring() {
    _monitoring = true;
    _startTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
    ResetMetrics();
}

void PerformanceMonitor::StopMonitoring() {
    _monitoring = false;
}

shared::PerformanceMetrics PerformanceMonitor::GetMetrics() const {
    return _metrics;
}

void PerformanceMonitor::ResetMetrics() {
    _metrics = {};
    _metrics.averageLatency = 0.0f;
    _metrics.peakLatency = 0.0f;
    _metrics.averagePollingRate = 0.0f;
    _metrics.inputJitter = 0.0f;
    _metrics.droppedFrames = 0;
    _metrics.cpuUsage = 0.0f;
    _metrics.uptimeMs = 0;
}

void PerformanceMonitor::LogMetricsToFile(const std::string& filename) const {
    std::ofstream stream(filename);
    if (!stream.is_open()) {
        return;
    }

    stream << "{\n";
    stream << "  \"averageLatency\": " << _metrics.averageLatency << ",\n";
    stream << "  \"peakLatency\": " << _metrics.peakLatency << ",\n";
    stream << "  \"averagePollingRate\": " << _metrics.averagePollingRate << ",\n";
    stream << "  \"inputJitter\": " << _metrics.inputJitter << ",\n";
    stream << "  \"droppedFrames\": " << _metrics.droppedFrames << ",\n";
    stream << "  \"cpuUsage\": " << _metrics.cpuUsage << ",\n";
    stream << "  \"uptimeMs\": " << _metrics.uptimeMs << "\n";
    stream << "}\n";
}

void PerformanceMonitor::UpdateCpuUsage() {
    // placeholder for Windows performance API measurements
}

} // namespace rocket
