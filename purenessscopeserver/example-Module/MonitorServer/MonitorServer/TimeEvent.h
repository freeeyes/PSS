#ifndef _TIMER_EVENT_H
#define _TIMER_EVENT_H

#include "define.h"
#include "DataManager.h"

//�����߳�ʱ��ģʽ
class CTimeEventTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CTimeEventTask();
    ~CTimeEventTask();

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);
};

#endif
