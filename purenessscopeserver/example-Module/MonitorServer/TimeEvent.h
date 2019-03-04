#ifndef _TIMER_EVENT_H
#define _TIMER_EVENT_H

#include "define.h"
#include "DataManager.h"

//工作线程时序模式
class CTimeEventTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CTimeEventTask();
    ~CTimeEventTask();

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);
};

#endif
