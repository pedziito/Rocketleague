#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) void* CreateController();
__declspec(dllexport) void DestroyController(void* handle);
__declspec(dllexport) int EnumerateControllers(char* out, int outSize);
__declspec(dllexport) bool OpenController(void* handle, int index);
__declspec(dllexport) bool ReadInput(void* handle, unsigned char* buffer, int* size);
__declspec(dllexport) void SetDeadzoneConfig(void* handle, float inner, float outer, int type);
__declspec(dllexport) int GetPollingRate(void* handle);
__declspec(dllexport) float* GetPerformanceMetrics(void* handle);

#ifdef __cplusplus
}
#endif
