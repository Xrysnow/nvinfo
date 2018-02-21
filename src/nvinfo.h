#ifdef NVINFO_EXPORTS
#define NVINFO_API __declspec(dllexport)
#else
#define NVINFO_API __declspec(dllimport)
#endif
#include <cstdint>
#include <nvapi.h>

struct GPUUsage
{
	uint32_t gpu;// graphic engine
	uint32_t fb;// frame buffer
	uint32_t vid;// video engine
	uint32_t bus;// bus interface
};
struct GPUClocks
{
	uint32_t graphics;
	uint32_t memory;
	uint32_t processor;
	uint32_t video;
};

extern "C"{
	NVINFO_API bool Init();
	NVINFO_API bool UnLoad();
	// common info
	NVINFO_API bool IsInited();
	NVINFO_API const char* GetErrStr();
	NVINFO_API uint32_t GetGPUNum();
	NVINFO_API uint32_t GetDriverVersion();
	NVINFO_API const char* GetBuildBranchString();
	NVINFO_API const char* GetGPUName(uint32_t i);
	//
	//
	// Performance State
	NVINFO_API bool GetPstates20(uint32_t i, NV_GPU_PERF_PSTATES20_INFO* info);
	NVINFO_API bool GetCurrentPstate(uint32_t i, NV_GPU_PERF_PSTATE_ID* info);
	NVINFO_API bool GetDynamicPstatesInfoEx(uint32_t i, NV_GPU_DYNAMIC_PSTATES_INFO_EX* info);
	// Memory
	NVINFO_API bool GetMemoryInfo(uint32_t i, NV_DISPLAY_DRIVER_MEMORY_INFO* info);
	// Thermal
	NVINFO_API bool GetThermalSettings(uint32_t i, uint32_t sensorIndex, NV_GPU_THERMAL_SETTINGS* info);
	// Clock
	NVINFO_API bool GetAllClockFrequencies(uint32_t i, NV_GPU_CLOCK_FREQUENCIES* info);
	//
	//
	// usage
	NVINFO_API bool GetUsages(uint32_t i, GPUUsage* info);
	NVINFO_API bool GetCoreUsage(uint32_t i, uint32_t* info);
	// temp
	NVINFO_API bool GetTemperature(uint32_t i, uint32_t* info);
	// freq
	NVINFO_API bool GetCurrentClocks(uint32_t i, GPUClocks* info);
	NVINFO_API bool GetMaxClocks(uint32_t i, GPUClocks* info);
	NVINFO_API bool GetCurrentCoreClock(uint32_t i, uint32_t* info);
	NVINFO_API bool GetMaxCoreClock(uint32_t i, uint32_t* info);
	// mem
	NVINFO_API bool GetFreeMemory(uint32_t i, uint32_t* info);
	NVINFO_API bool GetTotalMemory(uint32_t i, uint32_t* info);
}

