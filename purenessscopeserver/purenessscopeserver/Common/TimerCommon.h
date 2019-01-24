#ifndef _TIMER_COMMON_H
#define _TIMER_COMMON_H

#include <vector>
#include "Time_Value.h"
#if PSS_PLATFORM == PLATFORM_WIN
#include "windows.h"
#else
#include<sys/time.h>
#endif

//timer的一些公用API函数
//add by freeyes

namespace ts_timer
{
#define MAX_TIMOUTLIST_COUNT 100

#if PSS_PLATFORM == PLATFORM_WIN
#define PIRNTF printf_s
#else
#define PIRNTF printf
#endif

    inline CTime_Value GetTimeofDay()
    {
        CTime_Value obj_Time_Value;

#if PSS_PLATFORM == PLATFORM_WIN

        FILETIME   tfile;
        ::GetSystemTimeAsFileTime(&tfile);

        ULARGE_INTEGER _100ns;
        _100ns.LowPart = tfile.dwLowDateTime;
        _100ns.HighPart = tfile.dwHighDateTime;

        ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
        epoch.LowPart  = 0xD53E8000;
        epoch.HighPart = 0x019DB1DE;

        _100ns.QuadPart -= epoch.QuadPart;

        // Convert 100ns units to seconds;
        time_t tv_sec = (time_t)(_100ns.QuadPart / (10000 * 1000));
        // Convert remainder to microseconds;
        suseconds_t tv_usec = (suseconds_t)((_100ns.QuadPart % (10000 * 1000)) / 10);

        obj_Time_Value.Set_time(tv_sec, tv_usec);
#else
        struct  timeval    tvNow;
        struct  timezone   tz;
        gettimeofday(&tvNow, &tz);
        time_t tv_sec = (time_t)tvNow.tv_sec;
        suseconds_t tv_usec = (suseconds_t)tvNow.tv_usec;

        obj_Time_Value.Set_time(tv_sec, tv_usec);
#endif
        return obj_Time_Value;
    }

    //获得超时时间列表
    inline void Get_Timout_TimeInfo(CTime_Value& ttBegin, int nFrequency, CTime_Value& ttNow, std::vector<CTime_Value>& vecTimoutList)
    {
        int nSeconds  = nFrequency / 1000;
        int nUseconds = (nFrequency % 1000) * 1000;

        //超过最大次数就退出循环，保持效率。
        int nCurrCount = 0;

        while (true)
        {
            if (nCurrCount >= MAX_TIMOUTLIST_COUNT)
            {
                //如果超过最大循环次数，退出
                return;
            }

            if (ttBegin.Get_milliseconds() > ttNow.Get_milliseconds())
            {
                return;
            }
            else
            {
                //计算下次到达时间
                vecTimoutList.push_back(ttBegin);
                ttBegin = ttBegin + CTime_Value(nSeconds, nUseconds);
            }

            nCurrCount++;
        }
    }

    //获得sleep时间数
    inline void Get_Sleep(int nTime)
    {
#if PSS_PLATFORM == PLATFORM_WIN
        Sleep(nTime);
#else
        struct timespec rqt;
        rqt.tv_sec = 0;
        rqt.tv_nsec = (long)nTime * 1000 * 1000; //纳秒

        nanosleep(&rqt, NULL);
#endif
    }
};

#endif