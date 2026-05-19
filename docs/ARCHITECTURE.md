# Architecture

## Overview
The system is split into two main layers:
- `src/cpp/`: Native engine providing HID enumeration, input processing, polling optimization, anti-drift correction, and performance monitoring.
- `src/csharp/`: WPF frontend for controller management, profile handling, and live visualization.

## Component Relationships
- `ControllerEngine.dll` exposes a C API to the C# frontend.
- `CppInterop` maps native DLL functions into managed methods.
- `ControllerManager` orchestrates controller connection, input polling, and profile application.

## Data Flow
1. Controller enumeration via USB/HID APIs in C++.
2. Raw input is read and normalized in `InputProcessor`.
3. Drift corrections are applied through `AntiDrift`.
4. `PollingRateOptimizer` manages thread priority and USB polling rate.
5. `PerformanceMonitor` tracks latency, jitter, and frame drops.
6. C# UI reads metrics and displays live feedback.

## Thread Model
- Dedicated read thread for controller input.
- Optional high-priority thread affinity for low latency.
- UI thread handles rendering, profile changes, and user interaction.

## Performance Considerations
- Use of `HID_SET_FEATURE_REPORT` to adjust polling.
- Deadzone curves and sensitivity scaling near the stick center.
- Drift calibration buffers stationary input samples before applying offsets.
