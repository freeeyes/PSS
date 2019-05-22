#include "TimerThread.h"


//定时器线程
#if PSS_PLATFORM == PLATFORM_WIN
DWORD WINAPI thr_fn(void* arg)
#else
void* thr_fn(void* arg)
#endif
{
    //得到定时器列表参数
    ts_timer::CTimerInfoList* pTimerInfoList = reinterpret_cast<ts_timer::CTimerInfoList*>(arg);

    //最后运行的TimerID
    int nLastRunTimerID = 0;
    int nTimeCost       = 0;
    int nInterval       = 0;

    ts_timer::CTime_Value obj_Now;

    while (pTimerInfoList->Get_Run())
    {
        if (pTimerInfoList->Get_Event_Type() != ts_timer::TIMER_PAUSE)
        {
            bool blExpire = true;

            while (blExpire)
            {
                //得到定时器下一个执行对象列表
                obj_Now = ts_timer::GetTimeofDay();
                std::vector<_Lcm_Info>* pList = pTimerInfoList->Get_Next_Assemble();

                if (NULL == pList)
                {
                    break;
                }

                for (int i = 0; i < (int)pList->size(); i++)
                {
                    ts_timer::ITimerInfo* pTimerInfo = pTimerInfoList->GetTimerInfo((*pList)[i].m_nIndex);

                    //得到定时器下一个执行对象的差距时间
                    ts_timer::CTime_Value tvInterval = pTimerInfo->Get_Next_Time() - obj_Now;
                    nInterval = (int)tvInterval.Get_milliseconds();

                    if (nInterval < 0)
                    {
                        //计算错误周期
                        std::vector<ts_timer::CTime_Value> vecTimoutList;
                        ts_timer::CTime_Value obj_Time_Begin = pTimerInfo->Get_Next_Time();
                        Get_Timout_TimeInfo(obj_Time_Begin,
                                            pTimerInfo->Get_Timer_Frequency(),
                                            obj_Now,
                                            vecTimoutList);

                        pTimerInfo->Do_Error_Events(nLastRunTimerID, nTimeCost, vecTimoutList);

                        //重新计算下一次到期时间
                        pTimerInfo->Get_Next_Timer(obj_Now, true);
                    }
                    else
                    {
                        blExpire = false;
                    }
                }
            }

        }

        pTimerInfoList->Lock();

        if (nInterval >= 0 && pTimerInfoList->Get_Event_Type() != ts_timer::TIMER_PAUSE && pTimerInfoList->GetCurrTimerCount() > 0)
        {
#if PSS_PLATFORM == PLATFORM_WIN
            SleepConditionVariableCS(reinterpret_cast<PCONDITION_VARIABLE>(pTimerInfoList->Get_cond()),
                                     reinterpret_cast<PCRITICAL_SECTION>(pTimerInfoList->Get_mutex()),
                                     nInterval);
#else
            struct timespec outtime;

            struct timeval now;
            struct timeval Interval;
            struct timeval abstime;

            Interval.tv_sec = (int)(nInterval / 1000);
            Interval.tv_usec = (nInterval % 1000) * 1000;

            gettimeofday(&now, NULL);
            timeradd(&now, &Interval, &abstime);

            outtime.tv_sec = abstime.tv_sec;
            outtime.tv_nsec = abstime.tv_usec * 1000;  //单位是纳秒

            pthread_cond_timedwait(pTimerInfoList->Get_cond(),
                                   pTimerInfoList->Get_mutex(),
                                   &outtime);
#endif
        }
        else
        {
            //定时器暂停或者列表无数据
            if (pTimerInfoList->Get_Event_Type() == ts_timer::TIMER_PAUSE || pTimerInfoList->GetCurrTimerCount() == 0)
            {
#if PSS_PLATFORM == PLATFORM_WIN
                SleepConditionVariableCS(reinterpret_cast<PCONDITION_VARIABLE>(pTimerInfoList->Get_cond()),
                                         reinterpret_cast<PCRITICAL_SECTION>(pTimerInfoList->Get_mutex()),
                                         INFINITE);
#else
                pthread_cond_wait(pTimerInfoList->Get_cond(),
                                  pTimerInfoList->Get_mutex());
#endif
            }

        }

        //得到当前时间
        ts_timer::CTime_Value obj_Now = ts_timer::GetTimeofDay();

        if (pTimerInfoList->Get_Event_Type() == ts_timer::TIMER_STOP)
        {
            //关闭当前线程
            PIRNTF("[thr_fn]sig Close.\n");
            pTimerInfoList->Set_Run(false);
        }
        else if (pTimerInfoList->Get_Event_Type() == ts_timer::TIMER_MODIFY
                 || pTimerInfoList->Get_Event_Type() == ts_timer::TIMER_RESTORE)
        {
            //重新计算下循环列表
            pTimerInfoList->Calculation_Run_Assemble(obj_Now);
            pTimerInfoList->Set_Event_Type(ts_timer::TIMER_DO_EVENT);
            pTimerInfoList->UnLock();
            continue;
        }
        else if (pTimerInfoList->Get_Event_Type() == ts_timer::TIMER_PAUSE)
        {
            //暂停
            pTimerInfoList->UnLock();
            continue;
        }
        else
        {
            std::vector<_Lcm_Info>* pList = pTimerInfoList->Get_Curr_Assemble();

            for (int i = 0; i < (int)pList->size(); i++)
            {
                ts_timer::ITimerInfo* pTimerInfo = pTimerInfoList->GetTimerInfo((*pList)[i].m_nIndex);

                if (ts_timer::TIMER_STATE_DEL == pTimerInfo->Do_Timer_Event(obj_Now))
                {
                    pTimerInfoList->Del_Timer(pTimerInfoList->GetTimerInfo((*pList)[i].m_nIndex)->Get_Timer_ID());
                    //重新计算列表
                    pTimerInfoList->Calculation_Run_Assemble(obj_Now);
                }
            }

            ts_timer::CTime_Value obj_End = ts_timer::GetTimeofDay();
            ts_timer::CTime_Value obj_Interval = obj_End - obj_Now;

            //计算定时任务执行时间
            nTimeCost = obj_Interval.Get_milliseconds();
        }

        pTimerInfoList->UnLock();

    }

    return 0;
}

ts_timer::CTimerThread::CTimerThread()
{

}

ts_timer::CTimerThread::~CTimerThread()
{
    //判断工作线程是否正在运行
    if (m_TimerInfoList.Get_Run() == true)
    {
        Modify(TIMER_STOP);
    }

    Get_Sleep(10);

    m_TimerInfoList.Close();
}

void ts_timer::CTimerThread::Init(int nMaxCount)
{
    Close();

    m_TimerInfoList.Init(nMaxCount);
}

void ts_timer::CTimerThread::Close()
{
    //发起唤醒线程操作
    if (NULL != m_TimerInfoList.Get_mutex() && NULL != m_TimerInfoList.Get_cond())
    {
        Modify(TIMER_STOP);
    }
}

void ts_timer::CTimerThread::Run()
{
    m_TimerInfoList.Set_Run(true);

#if PSS_PLATFORM == PLATFORM_WIN
    DWORD nThreadID = 0;
    CreateThread(NULL, 0, thr_fn, (PVOID)&m_TimerInfoList, 0, &nThreadID);
    m_TimerInfoList.Set_Thread_ID(nThreadID);
#else
    pthread_t nThreadID;
    pthread_create(&nThreadID, NULL, thr_fn, (void*)&m_TimerInfoList);
    m_TimerInfoList.Set_Thread_ID(nThreadID);
#endif
    CTime_Value ttNow = GetTimeofDay();
    printf("[CTimerThread::Run]<%s> is Run.\n", ttNow.Get_string().c_str());
}

/*
* Add_Timer函数，成功返回true
* nTimerID 定时任务ID
* nFrequency 时间间隔，单位是毫秒。
* pttBegin 定时器开始时间（第一次不执行）
* fn_Timeout_Callback 任务回调事件，不可为空
* pArgContext 任务到期执行的参数
* fn_Timeout_Error_Callback 定时器没按时执行的报错事件，可以为空
*/
bool ts_timer::CTimerThread::Add_Timer(ITimeNode* pTimeNode, void* pArgContext)
{
    if (NULL == pTimeNode)
    {
        return false;
    }

    ITimerInfo* pTimerInfo = new ITimerInfo();

    pTimerInfo->Set_Timer_Param(pTimeNode, pArgContext);

    bool blRet = m_TimerInfoList.Add_Timer(pTimerInfo);

    if (false == blRet)
    {
        delete pTimerInfo;
        pTimerInfo = NULL;
    }
    else
    {
        Modify(TIMER_MODIFY);
    }

    return blRet;
}

bool ts_timer::CTimerThread::Del_Timer(int nTimerID)
{
    bool blRet = m_TimerInfoList.Del_Timer(nTimerID);

    if (true == blRet)
    {
        Modify(TIMER_MODIFY);
    }

    return blRet;
}

bool ts_timer::CTimerThread::Pause()
{
    Modify(TIMER_PAUSE);
    return true;
}

bool ts_timer::CTimerThread::Restore()
{
    Modify(TIMER_RESTORE);
    return true;
}

void ts_timer::CTimerThread::Modify(EM_Event_Type emType)
{
    m_TimerInfoList.Lock();

    m_TimerInfoList.Set_Event_Type(emType);

    m_TimerInfoList.UnLock();

    if (NULL != m_TimerInfoList.Get_cond())
    {
#if PSS_PLATFORM == PLATFORM_WIN
        WakeAllConditionVariable(m_TimerInfoList.Get_cond());
#else
        pthread_cond_signal(m_TimerInfoList.Get_cond());
#endif
    }
}
