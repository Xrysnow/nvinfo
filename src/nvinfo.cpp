#include "stdafx.h"
#include "nvinfo.h"
#include <nvapi_lite_common.h>
#include <nvapi.h>

#define CHECKRET(call) auto ret = call;\
	if (ret != NVAPI_OK){\
		NvAPI_GetErrorMessage(ret, ERRSTR);\
		return false;\
	}\
	return true;
#define CHECK_I i = min(i, GPUNum - 1);
#define CLAMP(v,_min,_max) v=max(v,_min);v=min(v,_max);

NvAPI_ShortString ERRSTR;
bool INITED = false;
NvPhysicalGpuHandle GPUHandles[NVAPI_MAX_PHYSICAL_GPUS];
NvU32 GPUNum = -1;
NvU32 DriverVersion;
NvAPI_ShortString BuildBranchString;
NvAPI_ShortString GPUName;

bool Init()
{
	auto ret = NvAPI_Initialize();
	if (ret != NVAPI_OK) {
		NvAPI_GetErrorMessage(ret, ERRSTR);
		return false;
	}
	ret = NvAPI_EnumPhysicalGPUs(GPUHandles, &GPUNum);
	if (ret != NVAPI_OK) {
		NvAPI_GetErrorMessage(ret, ERRSTR);
		return false;
	}
	ret = NvAPI_SYS_GetDriverAndBranchVersion(&DriverVersion, BuildBranchString);
	if (ret != NVAPI_OK) {
		NvAPI_GetErrorMessage(ret, ERRSTR);
		return false;
	}
	INITED = true;
	return true;
}

bool UnLoad(){
	if (!INITED)
		return true;
	CHECKRET(NvAPI_Unload());
}

bool IsInited(){
	return INITED;
}

const char* GetErrStr(){
	return ERRSTR;
}

uint32_t GetGPUNum(){
	return GPUNum;
}

uint32_t GetDriverVersion(){
	return DriverVersion;
}

const char* GetBuildBranchString(){
	return BuildBranchString;
}

const char* GetGPUName(uint32_t i){
	CHECK_I;
	NvAPI_GPU_GetFullName(GPUHandles[i], GPUName);
	return GPUName;
}

bool GetCurrentPstate(uint32_t i, NV_GPU_PERF_PSTATE_ID* info)
{
	CHECK_I;
	CHECKRET(NvAPI_GPU_GetCurrentPstate(GPUHandles[i], info));
}

bool GetDynamicPstatesInfoEx(uint32_t i, NV_GPU_DYNAMIC_PSTATES_INFO_EX* info)
{
	CHECK_I;
	info->version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;
	CHECKRET(NvAPI_GPU_GetDynamicPstatesInfoEx(GPUHandles[i], info));
}

bool GetPstates20(uint32_t i, NV_GPU_PERF_PSTATES20_INFO* info)
{
	CHECK_I;
	info->version = NV_GPU_PERF_PSTATES20_INFO_VER;
	CHECKRET(NvAPI_GPU_GetPstates20(GPUHandles[i], info));
}

bool GetMemoryInfo(uint32_t i, NV_DISPLAY_DRIVER_MEMORY_INFO* info)
{
	CHECK_I;
	info->version = NV_DISPLAY_DRIVER_MEMORY_INFO_VER;
	CHECKRET(NvAPI_GPU_GetMemoryInfo(GPUHandles[i], info));
}

bool GetThermalSettings(uint32_t i,uint32_t sensorIndex, NV_GPU_THERMAL_SETTINGS* info)
{
	CHECK_I;
	info->version = NV_GPU_THERMAL_SETTINGS_VER;
	CHECKRET(NvAPI_GPU_GetThermalSettings(GPUHandles[i], sensorIndex, info));
}

bool GetAllClockFrequencies(uint32_t i, NV_GPU_CLOCK_FREQUENCIES* info)
{
	CHECK_I;
	info->version = NV_GPU_CLOCK_FREQUENCIES_VER;
	CHECKRET(NvAPI_GPU_GetAllClockFrequencies(GPUHandles[i], info));
}

bool GetUsages(uint32_t i, GPUUsage* info)
{
	NV_GPU_DYNAMIC_PSTATES_INFO_EX inf;
	if (!GetDynamicPstatesInfoEx(i, &inf))
		return false;
	info->gpu= inf.utilization[0].percentage;
	info->fb= inf.utilization[1].percentage;
	info->vid= inf.utilization[2].percentage;
	info->bus= inf.utilization[3].percentage;
	return true;
}

bool GetCoreUsage(uint32_t i, uint32_t* info){
	GPUUsage u;
	if (!GetUsages(i,&u))
		return false;
	*info = u.gpu;
	return true;
}

bool GetTemperature(uint32_t i, uint32_t* info)
{
	NV_GPU_THERMAL_SETTINGS inf;
	if (!GetThermalSettings(i,0, &inf))
		return false;
	*info = inf.sensor[0].currentTemp;
	return true;
}

bool GetCurrentClocks(uint32_t i, GPUClocks* info)
{
	NV_GPU_CLOCK_FREQUENCIES inf;
	inf.ClockType = NV_GPU_CLOCK_FREQUENCIES_CURRENT_FREQ;
	if (!GetAllClockFrequencies(i, &inf))
		return false;
	info->graphics = inf.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency;
	info->memory = inf.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency;
	info->processor = inf.domain[NVAPI_GPU_PUBLIC_CLOCK_PROCESSOR].frequency;
	info->video = inf.domain[NVAPI_GPU_PUBLIC_CLOCK_VIDEO].frequency;
	return true;
}

bool GetMaxClocks(uint32_t i, GPUClocks* info)
{
	NV_GPU_CLOCK_FREQUENCIES inf;
	inf.ClockType = NV_GPU_CLOCK_FREQUENCIES_BOOST_CLOCK;
	if (!GetAllClockFrequencies(i, &inf))
		return false;
	info->graphics = inf.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency;
	info->memory = inf.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency;
	info->processor = inf.domain[NVAPI_GPU_PUBLIC_CLOCK_PROCESSOR].frequency;
	info->video = inf.domain[NVAPI_GPU_PUBLIC_CLOCK_VIDEO].frequency;
	return true;
}

bool GetCurrentCoreClock(uint32_t i, uint32_t* info)
{
	GPUClocks inf;
	if (!GetCurrentClocks(i, &inf))
		return false;
	*info = inf.graphics;
	return true;
}

bool GetMaxCoreClock(uint32_t i, uint32_t* info)
{
	GPUClocks inf;
	if (!GetMaxClocks(i, &inf))
		return false;
	*info = inf.graphics;
	return true;
}



