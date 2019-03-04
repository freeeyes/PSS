#include "ProfileTime.h"

CProfileTime::CProfileTime(void)
{
    //在windows下，获得滴答的频率
#if PSS_PLATFORM == PLATFORM_WIN
    ::QueryPerformanceFrequency(&m_liPerfFreq);
    ::QueryPerformanceFrequency(&m_liPerfStart);
    ::QueryPerformanceFrequency(&m_liPerfStop);
#endif
}

CProfileTime::~CProfileTime(void)
{
}

bool CProfileTime::Start()
{
#if PSS_PLATFORM == PLATFORM_WIN
    QueryPerformanceCounter(&m_liPerfStart);
#else
    m_HighResTimer.start();
#endif

    return true;
}

uint64 CProfileTime::Stop()
{
#if PSS_PLATFORM == PLATFORM_WIN
    QueryPerformanceCounter(&m_liPerfStop);

    return (((m_liPerfStop.QuadPart - m_liPerfStart.QuadPart) * 1000 * 1000 * 1000)/m_liPerfFreq.QuadPart);
#else
    m_HighResTimer.stop();

    uint64 u8TimeCost = 0;
    m_HighResTimer.elapsed_time(u8TimeCost);
    return u8TimeCost;
#endif
}


