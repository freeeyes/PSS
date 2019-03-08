#ifndef _TSTIMER_H
#define _TSTIMER_H

#include "TimerInfo.h"

class ITSTimerManager
{
public:
    virtual ~ITSTimerManager() {};

    //��Ӷ�ʱ��
    virtual bool Add_Timer(int nTimerID,
                           int nFrequency,
                           ts_timer::CTime_Value* pttBegin,
                           ts_timer::Timeout_Callback fn_Timeout_Callback,
                           void* pArgContext,
                           ts_timer::Timeout_Error_Callback fn_Timeout_Error_Callback) = 0;

    //ɾ����ʱ��
    virtual bool Del_Timer(int nTimerID) = 0;

    //��ͣ��ʱ��
    virtual bool Pause() = 0;

    //�ָ���ͣ��ʱ��
    virtual bool Restore() = 0;
};

#endif
