#ifndef _TSTIMER_H
#define _TSTIMER_H

#include "TimerNodeList.hpp"

class ITSTimerManager
{
public:
    virtual ~ITSTimerManager() {};

    //��Ӷ�ʱ��
    virtual bool Add_Timer(int timer_id, milliseconds timer_interval, TimerFunctor&& f, shared_ptr<ITimerInfo> arg) = 0;

    //ɾ����ʱ��
    virtual bool Del_Timer(int nTimerID) = 0;

    //��ͣ��ʱ��
    virtual bool Pause() = 0;

    //�ָ���ͣ��ʱ��
    virtual bool Restore() = 0;
};

#endif
