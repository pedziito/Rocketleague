using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using Newtonsoft.Json;

namespace Rocketleague.ControllerOptimizer.Interop
{
    public class ControllerInfo
    {
        [JsonProperty("deviceName")]
        public string Name { get; set; }

        [JsonProperty("devicePath")]
        public string Path { get; set; }
    }

    public class DeadzoneProfile
    {
        public float Inner { get; set; }
        public float Outer { get; set; }
        public int Type { get; set; }
    }

    public class PerformanceMetrics
    {
        public float AverageLatency { get; set; }
        public float PeakLatency { get; set; }
        public float AveragePollingRate { get; set; }
        public float InputJitter { get; set; }
        public int DroppedFrames { get; set; }
        public float CpuUsage { get; set; }
        public long UptimeMs { get; set; }
    }

    public class ControllerManager : IDisposable
    {
        private IntPtr _controllerHandle;
        private bool _disposed;

        public event Action<byte[]> OnInputReceived;
        public event Action<PerformanceMetrics> OnMetricsUpdated;

        public void Initialize()
        {
            _controllerHandle = CppInterop.CreateController();
        }

        public (List<ControllerInfo> Controllers, string RawJson) GetAvailableControllers()
        {
            var output = new StringBuilder(131072);
            int length = CppInterop.EnumerateControllers(output, output.Capacity);
            string rawJson = length > 0 ? output.ToString(0, Math.Min(length, output.Capacity - 1)) : string.Empty;
            if (length == output.Capacity - 1)
            {
                rawJson += "\n[Warning] EnumerateControllers result may be truncated. Increase buffer size if necessary.";
            }

            var controllers = new List<ControllerInfo>();

            try
            {
                controllers = JsonConvert.DeserializeObject<List<ControllerInfo>>(rawJson) ?? new List<ControllerInfo>();
            }
            catch (Exception ex)
            {
                rawJson += $"\nJSON parse error: {ex.Message}";
            }

            return (controllers, rawJson);
        }

        public bool Connect(int controllerIndex)
        {
            return CppInterop.OpenController(_controllerHandle, controllerIndex);
        }

        public void Disconnect()
        {
            if (_controllerHandle != IntPtr.Zero)
            {
                CppInterop.DestroyController(_controllerHandle);
                _controllerHandle = IntPtr.Zero;
            }
        }

        public void SetDeadzoneProfile(DeadzoneProfile profile)
        {
            if (_controllerHandle == IntPtr.Zero) return;
            CppInterop.SetDeadzoneConfig(_controllerHandle, profile.Inner, profile.Outer, profile.Type);
        }

        public void SetPollingRate(int hz)
        {
            if (_controllerHandle == IntPtr.Zero) return;
            // no managed binding yet for SetPollingRate; extensibility placeholder
        }

        public PerformanceMetrics GetMetrics()
        {
            if (_controllerHandle == IntPtr.Zero) return new PerformanceMetrics();
            IntPtr resultPtr = CppInterop.GetPerformanceMetrics(_controllerHandle);
            var values = new float[8];
            Marshal.Copy(resultPtr, values, 0, values.Length);
            return new PerformanceMetrics
            {
                AverageLatency = values[0],
                PeakLatency = values[1],
                AveragePollingRate = values[2],
                InputJitter = values[3],
                DroppedFrames = (int)values[4],
                CpuUsage = values[5],
                UptimeMs = (long)values[6]
            };
        }

        public void Dispose()
        {
            if (_disposed) return;
            Disconnect();
            _disposed = true;
        }
    }
}
