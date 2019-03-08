#ifndef _TIMER_THREAD_H
#define _TIMER_THREAD_H

#include "TimerInfo.h"
#include "ITSTimer.h"

namespace ts_timer
{
    class CTimerThread : public ITSTimerManager
    {
    public:
        CTimerThread();
        ~CTimerThread();

        void Init(int nMaxCount = MAX_TIMER_LIST_COUNT);

        void Close();

        void Run();

        //��Ӷ�ʱ��
        bool Add_Timer(int nTimerID,
                       int nFrequency,
                       CTime_Value* pttBegin,
                       Timeout_Callback fn_Timeout_Callback,
                       void* pArgContext,
                       Timeout_Error_Callback fn_Timeout_Error_Callback);

        //ɾ����ʱ��
        bool Del_Timer(int nTimerID);

        //��ͣ��ʱ��
        bool Pause();

        //�ָ���ͣ��ʱ��
        bool Restore();

    private:
        void Modify(EM_Event_Type emType);

        CTimerInfoList   m_TimerInfoList;
    };
}

#endif
