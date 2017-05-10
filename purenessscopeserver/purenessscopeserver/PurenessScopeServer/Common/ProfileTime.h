#ifndef _PROFILETIME_H
#define _PROFILETIME_H

//统计过程执行时间的性能指标参数
//以前的性能指标统计太过分散，在这里做一个包装
//在windows下，ACE的性能指标有偏差，所以在这里做一个宏，分开对待。
//add by freeeyes

#include "define.h"
#include "ace/Profile_Timer.h"

class CProfileTime
{
public:
    CProfileTime(void);
    ~CProfileTime(void);

    bool       Start();               //开始计时
    uint64     Stop();                //结束计时，并返回处理时间，单位是纳秒

private:
#ifndef __LINUX__
    LARGE_INTEGER m_liPerfFreq;    //CPU的滴答时间，仅仅是windows使用
    LARGE_INTEGER m_liPerfStart;   //开始时间
    LARGE_INTEGER m_liPerfStop;    //结束时间
#else
    ACE_High_Res_Timer m_HighResTimer;
#endif

};
#endif

