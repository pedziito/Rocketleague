# API Reference

## C++ Engine

### `HIDController`
- `std::vector<ControllerInfo> EnumerateControllers()` - Enumerate available HID game controllers.
- `bool OpenController(const ControllerInfo& info)` - Open selected controller by device path.
- `bool CloseController()` - Close the currently opened controller.
- `bool ReadInput(std::vector<unsigned char>& inputData)` - Read the next input report.
- `bool WriteOutput(const std::vector<unsigned char>& outputData)` - Send output report.
- `ControllerInfo GetCurrentControllerInfo()` - Return the active controller details.

### `InputProcessor`
- `AnalogStickData ProcessAnalogStick(short rawX, short rawY)` - Normalize analog stick values.
- `void UpdateConfig(const DeadzoneConfig& newConfig)` - Update deadzone and sensitivity settings.
- `void ApplyCustomCurve(const std::vector<float>& curvePoints)` - Apply a custom response curve.

### `AntiDrift`
- `void Calibrate(const std::vector<AnalogStickData>& samples)` - Calibrate stationary stick samples.
- `AnalogStickData CorrectDrift(const AnalogStickData& input)` - Correct drift from current stick input.
- `void UpdateProfile(const DriftProfile& newProfile)` - Replace the active drift profile.

### `PollingRateOptimizer`
- `int GetCurrentPollingRate()` - Query the current USB polling rate.
- `bool SetPollingRate(int hz)` - Request a new target polling rate.
- `bool StartHighPriorityReadThread()` - Start a high-priority reading thread.
- `void StopReadThread()` - Stop the polling thread.
- `int MeasureActualPollingRate()` - Measure real polling frequency.

### `PerformanceMonitor`
- `void StartMonitoring()` - Begin collecting latency and jitter metrics.
- `void StopMonitoring()` - Stop monitoring.
- `PerformanceMetrics GetMetrics()` - Return current metrics.
- `void ResetMetrics()` - Reset collected metrics.
- `void LogMetricsToFile(const std::string& filename)` - Export metrics to JSON.

## C# Interop

### `CppInterop`
- `CreateController()` - Create a native controller engine instance.
- `DestroyController(IntPtr handle)` - Release the native controller engine.
- `EnumerateControllers(StringBuilder output, int outSize)` - Request enumeration results.
- `OpenController(IntPtr handle, int index)` - Open controller at the selected index.
- `ReadInput(IntPtr handle, byte[] buffer, ref int size)` - Read input report bytes.
- `SetDeadzoneConfig(IntPtr handle, float inner, float outer, int type)` - Send settings to the native layer.
- `GetPollingRate(IntPtr handle)` - Request the current polling rate.
- `GetPerformanceMetrics(IntPtr handle)` - Request metric values as a float array.

## Profiles

Profiles are stored in JSON and include:
- `Name`
- `DeadzoneConfig`
- `PollingRate`
- `DriftProfile`
- `AntiDriftEnabled`
- `CreatedAt`
- `LastModified`
