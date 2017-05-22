#ifdef WIN32

#include "WindowsCPU.h"

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
