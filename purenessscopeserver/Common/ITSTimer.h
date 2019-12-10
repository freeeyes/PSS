#ifndef _TSTIMER_H
#define _TSTIMER_H

#include "TimerNodeList.hpp"

class ITSTimerManager
{
public:
    virtual ~ITSTimerManager() {};

    //添加定时器
    virtual bool Add_Timer(int timer_id, milliseconds timer_interval, TimerFunctor&& f, ITimerInfo* arg) = 0;

    //删除定时器
    virtual bool Del_Timer(int nTimerID) = 0;

    //暂停定时器
    virtual bool Pause() = 0;

    //恢复暂停定时器
    virtual bool Restore() = 0;
};

#endif
