#ifdef WIN32

#include "WindowsCPU.h"

typedef  LONG  (WINAPI*  PROCNTQSI)(UINT,PVOID,ULONG,PULONG);
PROCNTQSI  NtQuerySystemInformation;

int GetProcessCPU_Idel()
{
    CpuUsage objCpu;

    short u2CpuRote = objCpu.GetUsage();
    Sleep(1000);
    u2CpuRote = objCpu.GetUsage();

    return (int)u2CpuRote;
}

int GetProcessMemorySize()
{
    PROCESS_MEMORY_COUNTERS pmc;

    if(::GetProcessMemoryInfo(::GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        int nWorkSize = (int)pmc.WorkingSetSize;
        int nPageSize = (int)pmc.PagefileUsage;
        return nWorkSize + nPageSize;
    }

    return -1;
}

#endif
