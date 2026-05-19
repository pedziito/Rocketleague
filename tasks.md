# Rocket League Controller Optimization System - Tasks

## Project Overview
This is a comprehensive Windows-based controller optimization and overclocking system for Rocket League. The system will include controller input optimization, polling rate enhancement, deadzone curve customization, anti-drift algorithms, and lag reduction.

**Tech Stack:**
- **Backend:** C++ (Core performance optimization, USB HID communication)
- **Frontend:** C# (.NET WinForms/WPF for GUI)
- **Target:** Windows 10/11

---

## Phase 1: Project Structure & Setup

### Task 1.1: Create Directory Structure

```
Rocketleague/
├── src/
│   ├── cpp/
│   │   ├── HIDController/
│   │   │   ├── HIDController.h
│   │   │   ├── HIDController.cpp
│   │   │   ├── InputProcessor.h
│   │   │   ├── InputProcessor.cpp
│   │   │   ├── AntiDrift.h
│   │   │   └── AntiDrift.cpp
│   │   ├── PollingRateOptimizer/
│   │   │   ├── PollingRateOptimizer.h
│   │   │   └── PollingRateOptimizer.cpp
│   │   └── PerformanceMonitor/
│   │       ├── PerformanceMonitor.h
│   │       └── PerformanceMonitor.cpp
│   ├── csharp/
│   │   ├── ControllerUI/
│   │   │   ├── MainWindow.xaml
│   │   │   ├── MainWindow.xaml.cs
│   │   │   ├── ProfileManager.cs
│   │   │   └── SettingsPanel.xaml
│   │   ├── Interop/
│   │   │   ├── CppInterop.cs
│   │   │   └── ControllerWrapper.cs
│   │   └── ControllerOptimizer.csproj
│   └── shared/
│       ├── Structures.h
│       └── Constants.h
├── config/
│   ├── default-profiles.json
│   └── settings.json
├── build/
│   ├── CMakeLists.txt
│   └── build-debug.bat
├── docs/
│   ├── ARCHITECTURE.md
│   └── API_REFERENCE.md
├── tests/
│   ├── cpp-tests/
│   └── csharp-tests/
├── .gitignore
├── README.md
└── tasks.md
```

### Task 1.2: Create CMakeLists.txt
Create `build/CMakeLists.txt` with:
- C++ project configuration
- Windows-specific settings (WinAPI, USB libraries)
- Output DLL generation for C# interop
- Include paths for HID API (setupapi.lib, winusb.lib)

### Task 1.3: Create Visual Studio Solution (.NET)
Create C# project structure with:
- WPF application targeting .NET 6.0+
- Project references for C++ DLL interop
- NuGet packages: Newtonsoft.Json, MaterialDesignInXaml

---

## Phase 2: Core C++ Engine

### Task 2.1: HID Controller Detection & Enumeration
**File:** `src/cpp/HIDController/HIDController.h` & `.cpp`

Implement:
```cpp
class HIDController {
  public:
    struct ControllerInfo {
        HANDLE deviceHandle;
        unsigned int vendorID;
        unsigned int productID;
        std::string devicePath;
        std::string deviceName;
        int inputReportSize;
        int outputReportSize;
    };
    
    std::vector<ControllerInfo> EnumerateControllers();
    bool OpenController(const ControllerInfo& info);
    bool CloseController();
    bool ReadInput(std::vector<unsigned char>& inputData);
    bool WriteOutput(const std::vector<unsigned char>& outputData);
    ControllerInfo GetCurrentControllerInfo();
};
```

Features:
- Use Windows Setup API (setupapi.h) to enumerate HID devices
- Support Xbox 360, Xbox One, PS4, PS5, generic HID controllers
- Filter by vendor/product IDs
- Store device paths and handles
- Error handling for device disconnection

### Task 2.2: Input Processor & Deadzone Optimization
**File:** `src/cpp/HIDController/InputProcessor.h` & `.cpp`

Implement:
```cpp
class InputProcessor {
  public:
    enum class DeadzoneType { LINEAR, QUADRATIC, CUBIC, SMOOTH };
    
    struct DeadzoneConfig {
        DeadzoneType type;
        float innerDeadzone;    // 0.0-1.0
        float outerDeadzone;    // 0.0-1.0
        float sensitivity;      // 0.5-2.0
        bool enableSmoothing;
        float smoothingFactor;
    };
    
    struct AnalogStickData {
        float x, y;             // -1.0 to 1.0
        float magnitude;
        float angle;
    };
    
    InputProcessor(const DeadzoneConfig& config);
    AnalogStickData ProcessAnalogStick(short rawX, short rawY);
    void UpdateConfig(const DeadzoneConfig& newConfig);
    void ApplyCustomCurve(const std::vector<float>& curvePoints);
};
```

Features:
- Parse raw HID input to extract analog stick values
- Implement multiple deadzone curve algorithms (linear, quadratic, cubic, smooth step)
- Apply sensitivity multipliers
- Implement exponential scaling for precision
- Real-time curve adjustment
- Trigger optimization (convert 0-255 to -1.0 to 1.0 range)

### Task 2.3: Anti-Drift Algorithm
**File:** `src/cpp/HIDController/AntiDrift.h` & `.cpp`

Implement:
```cpp
class AntiDrift {
  public:
    struct DriftProfile {
        float driftThreshold;       // Acceptable drift magnitude
        float calibrationTime;      // Time to calibrate in seconds
        bool enableAdaptive;        // Learn drift patterns
        std::vector<float> offsetX, offsetY;
    };
    
    AntiDrift(const DriftProfile& profile);
    void Calibrate(const std::vector<AnalogStickData>& samples);
    AnalogStickData CorrectDrift(const AnalogStickData& input);
    void UpdateProfile(const DriftProfile& newProfile);
};
```

Features:
- Calibration routine (collect 100+ stationary samples)
- Store baseline offset values
- Real-time drift correction
- Adaptive learning (update offsets based on usage)
- Per-axis compensation
- Threshold-based filtering

### Task 2.4: Polling Rate Optimizer
**File:** `src/cpp/PollingRateOptimizer/PollingRateOptimizer.h` & `.cpp`

Implement:
```cpp
class PollingRateOptimizer {
  public:
    struct PollingConfig {
        int targetPollingRate;      // Hz (125, 250, 500, 1000, 4000, 8000)
        bool enableForcePolling;
        int cpuCore;                // Affinity
        bool prioritizeThread;
        int threadPriority;         // THREAD_PRIORITY_*
    };
    
    PollingRateOptimizer(const PollingConfig& config);
    int GetCurrentPollingRate();
    bool SetPollingRate(int hz);
    bool StartHighPriorityReadThread();
    void StopReadThread();
    int MeasureActualPollingRate();
};
```

Features:
- Measure and report current polling rate
- Attempt to increase USB polling interval via HID_SET_FEATURE_REPORT
- Set thread affinity to specific CPU core
- Elevate thread priority (HIGH_PRIORITY or REALTIME)
- Dedicated input reading thread with minimal latency
- Performance metrics (actual vs requested)

### Task 2.5: Performance Monitor
**File:** `src/cpp/PerformanceMonitor/PerformanceMonitor.h` & `.cpp`

Implement:
```cpp
class PerformanceMonitor {
  public:
    struct PerformanceMetrics {
        float averageLatency;       // ms
        float peakLatency;
        float averagePollingRate;   // Hz
        float inputJitter;
        int droppedFrames;
        float cpuUsage;
        long long uptimeMs;
    };
    
    void StartMonitoring();
    void StopMonitoring();
    PerformanceMetrics GetMetrics();
    void ResetMetrics();
    void LogMetricsToFile(const std::string& filename);
};
```

Features:
- Measure input latency (timestamp comparison)
- Track jitter and dropped inputs
- CPU usage monitoring (using Windows Performance API)
- Uptime tracking
- Real-time metrics export to JSON
- Performance logging/history

### Task 2.6: Create DLL Export Wrapper
**File:** `src/cpp/HIDController/ControllerEngine.h` & `.cpp`

Create C-style interface for C# interop.

---

## Phase 3: C# GUI & Management

### Task 3.1: Create WPF Main Window
### Task 3.2: Implement C# Interop Layer
### Task 3.3: Create Controller Wrapper Class
### Task 3.4: Profile Manager
### Task 3.5: Settings Panel UI
### Task 3.6: Input Visualization Component
### Task 3.7: Settings Persistence

---

## Phase 4: Build & Compilation

### Task 4.1: CMake Build Configuration
### Task 4.2: Create Build Batch Scripts
### Task 4.3: Visual Studio Solution Setup

---

## Phase 5: Testing & Validation

### Task 5.1: Unit Tests (C++)
### Task 5.2: Integration Tests (C#)
### Task 5.3: Real Device Testing

---

## Phase 6: Documentation

### Task 6.1: Architecture Document
### Task 6.2: API Reference
