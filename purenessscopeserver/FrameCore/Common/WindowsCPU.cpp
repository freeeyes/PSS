#include "WindowsCPU.h"

#if PSS_PLATFORM == PLATFORM_WIN
typedef  LONG  (WINAPI*  PROCNTQSI)(UINT,PVOID,ULONG,PULONG);
PROCNTQSI  NtQuerySystemInformation;

int GetProcessCPU_Idel()
{
    CpuUsage objCpu;

    int nCpuRoteBegin = objCpu.GetUsage();
    Sleep(1000);
    int nCpuRoteEnd = objCpu.GetUsage();

    return (int)((nCpuRoteEnd - nCpuRoteBegin) / 2);
}

uint32 GetProcessMemorySize()
{
    PROCESS_MEMORY_COUNTERS pmc;

    if (::GetProcessMemoryInfo(::GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        uint32 u4WorkSize = (uint32)pmc.WorkingSetSize;
        uint32 u4PageSize = (uint32)pmc.PagefileUsage;
        return (u4WorkSize + u4PageSize) / (1024 * 1024);
    }

    return 0;
};

#endif
