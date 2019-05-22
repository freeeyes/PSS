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

        //添加定时器
        bool Add_Timer(ITimeNode* pTimeNode, void* pArgContext);

        //删除定时器
        bool Del_Timer(int nTimerID);

        //暂停定时器
        bool Pause();

        //恢复暂停定时器
        bool Restore();

    private:
        void Modify(EM_Event_Type emType);

        CTimerInfoList   m_TimerInfoList;
    };
}

#endif
