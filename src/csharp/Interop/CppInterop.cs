using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Rocketleague.ControllerOptimizer.Interop
{
    public static class CppInterop
    {
        private const string DllName = "ControllerEngine.dll";

        [DllImport(DllName, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateController();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyController(IntPtr handle);

        [DllImport(DllName, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int EnumerateControllers(StringBuilder output, int outSize);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool OpenController(IntPtr handle, int index);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool ReadInput(IntPtr handle, byte[] buffer, ref int size);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetDeadzoneConfig(IntPtr handle, float inner, float outer, int type);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetPollingRate(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetPerformanceMetrics(IntPtr handle);
    }
}
