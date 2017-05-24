#ifndef _WINDOWSCPU_H
#define _WINDOWSCPU_H

#ifdef WIN32

#include  <windows.h>
#include  <Psapi.h>
#include  <conio.h>
#include  <stdio.h>

//在windows下获得CPU使用率的方法
class CpuUsage
{
public:
    CpuUsage():m_nCpuUsage(-1),IsFirstRun(true),m_lRunCount(0)
    {
        ZeroMemory(&m_ftPrevSysKernel, sizeof(FILETIME));
        ZeroMemory(&m_ftPrevSysUser, sizeof(FILETIME));

        ZeroMemory(&m_ftPrevProcKernel, sizeof(FILETIME));
        ZeroMemory(&m_ftPrevProcUser, sizeof(FILETIME));
    }

    short  GetUsage()
    {
        short nCpuCopy = m_nCpuUsage;

        if (::InterlockedIncrement(&m_lRunCount) == 1)
        {
            FILETIME ftSysIdle, ftSysKernel, ftSysUser;
            FILETIME ftProcCreation, ftProcExit, ftProcKernel, ftProcUser;

            if (!GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser) ||
                !GetProcessTimes(GetCurrentProcess(), &ftProcCreation, &ftProcExit, &ftProcKernel, &ftProcUser))
            {
                ::InterlockedDecrement(&m_lRunCount);
                return nCpuCopy;
            }

            if (!IsFirstRun)
            {
                /*
                CPU usage is calculated by getting the total amount of time the system has operated
                since the last measurement (made up of kernel + user) and the total
                amount of time the process has run (kernel + user).
                */
                ULONGLONG ftSysKernelDiff = SubtractTimes(ftSysKernel, m_ftPrevSysKernel);
                ULONGLONG ftSysUserDiff = SubtractTimes(ftSysUser, m_ftPrevSysUser);

                ULONGLONG ftProcKernelDiff = SubtractTimes(ftProcKernel, m_ftPrevProcKernel);
                ULONGLONG ftProcUserDiff = SubtractTimes(ftProcUser, m_ftPrevProcUser);

                ULONGLONG nTotalSys =  ftSysKernelDiff + ftSysUserDiff;
                ULONGLONG nTotalProc = ftProcKernelDiff + ftProcUserDiff;

                if (nTotalSys > 0)
                {
                    m_nCpuUsage = (short)((100.0 * nTotalProc) / nTotalSys);
                }
            }

            m_ftPrevSysKernel = ftSysKernel;
            m_ftPrevSysUser = ftSysUser;
            m_ftPrevProcKernel = ftProcKernel;
            m_ftPrevProcUser = ftProcUser;

            nCpuCopy = m_nCpuUsage;
        }

        IsFirstRun=false;
        ::InterlockedDecrement(&m_lRunCount);

        return nCpuCopy;
    }
private:
    ULONGLONG SubtractTimes(const FILETIME& ftA, const FILETIME& ftB)
    {
        LARGE_INTEGER a, b;
        a.LowPart = ftA.dwLowDateTime;
        a.HighPart = ftA.dwHighDateTime;

        b.LowPart = ftB.dwLowDateTime;
        b.HighPart = ftB.dwHighDateTime;

        return a.QuadPart - b.QuadPart;
    }

    bool IsFirstRun;

    //system total times
    FILETIME m_ftPrevSysKernel;
    FILETIME m_ftPrevSysUser;

    //process times
    FILETIME m_ftPrevProcKernel;
    FILETIME m_ftPrevProcUser;

    short m_nCpuUsage;

    volatile LONG m_lRunCount;
};

int GetProcessCPU_Idel();
int GetProcessMemorySize();

#endif

#endif
