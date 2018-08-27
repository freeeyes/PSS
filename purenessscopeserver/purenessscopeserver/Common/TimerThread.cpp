#include "TimerThread.h"


//定时器线程
#ifdef WIN32
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

    ts_timer::CTime_Value obj_Now = ts_timer::GetTimeofDay();

    while (pTimerInfoList->Get_Run())
    {
        if (pTimerInfoList->Get_Event_Type() != ts_timer::TIMER_PAUSE)
        {
            //得到定时器下一个执行对象的差距时间
            nInterval = pTimerInfoList->Get_Next_Timer(obj_Now);

            if (nInterval < 0)
            {
                //计算错误周期
                std::vector<ts_timer::CTime_Value> vecTimoutList;
                ts_timer::CTime_Value obj_Time_Begin = pTimerInfoList->Get_Curr_Timer()->Get_Next_Time();
                Get_Timout_TimeInfo(obj_Time_Begin,
                                    pTimerInfoList->Get_Curr_Timer()->Get_Timer_Frequency(),
                                    obj_Now,
                                    vecTimoutList);

                pTimerInfoList->Get_Curr_Timer()->Do_Error_Events(nLastRunTimerID, nTimeCost, obj_Time_Begin, vecTimoutList);

                //重新计算下一次到期时间
                nInterval = pTimerInfoList->Get_Next_Timer(obj_Now);
            }
        }

        pTimerInfoList->Lock();

        if (nInterval >= 0 && pTimerInfoList->Get_Event_Type() != ts_timer::TIMER_PAUSE && pTimerInfoList->GetCurrTimerCount() > 0)
        {
#ifdef WIN32
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
#ifdef WIN32
                SleepConditionVariableCS(reinterpret_cast<PCONDITION_VARIABLE>(pTimerInfoList->Get_cond()),
                                         reinterpret_cast<PCRITICAL_SECTION>(pTimerInfoList->Get_mutex()),
                                         INFINITE);
#else
                pthread_cond_wait(pTimerInfoList->Get_cond(),
                                  pTimerInfoList->Get_mutex());
#endif
            }

        }

        if (pTimerInfoList->Get_Event_Type() == ts_timer::TIMER_STOP)
        {
            //关闭当前线程
            PIRNTF("[thr_fn]sig Close.\n");
            pTimerInfoList->Set_Run(false);
        }
        else if (pTimerInfoList->Get_Event_Type() == ts_timer::TIMER_MODIFY
                 || pTimerInfoList->Get_Event_Type() == ts_timer::TIMER_PAUSE
                 || pTimerInfoList->Get_Event_Type() == ts_timer::TIMER_RESTORE)
        {
            //重新计算下一次唤醒时间
            obj_Now = ts_timer::GetTimeofDay();
            pTimerInfoList->UnLock();
            continue;
        }
        else
        {
            pTimerInfoList->Get_Event_Type();

            //执行定时器
            ts_timer::CTime_Value obj_Begin = ts_timer::GetTimeofDay();

            nLastRunTimerID = pTimerInfoList->Get_Curr_Timer()->Get_Timer_ID();

            if (NULL != pTimerInfoList->Get_Curr_Timer())
            {
                ts_timer::EM_Timer_State emstate = pTimerInfoList->Get_Curr_Timer()->Do_Timer_Event(obj_Begin);

                if (ts_timer::TIMER_STATE_DEL == emstate)
                {
                    //如果执行完需要清除定时器，在这里回收定时器
                    pTimerInfoList->Del_Timer(pTimerInfoList->Get_Curr_Timer()->Get_Timer_ID());
                }
            }

            obj_Now = ts_timer::GetTimeofDay();
            ts_timer::CTime_Value obj_Interval = obj_Now - obj_Begin;

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

#ifdef WIN32
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
bool ts_timer::CTimerThread::Add_Timer(int nTimerID, int nFrequency, CTime_Value* pttBegin, Timeout_Callback fn_Timeout_Callback, void* pArgContext, Timeout_Error_Callback fn_Timeout_Error_Callback)
{
    if (NULL == fn_Timeout_Callback)
    {
        return false;
    }

    ITimerInfo* pTimerInfo = new ITimerInfo();

    if (NULL != pttBegin)
    {
        //有开始时间
        pTimerInfo->Set_Timer_Param(nTimerID, nFrequency, *pttBegin, fn_Timeout_Callback, pArgContext, fn_Timeout_Error_Callback);
    }
    else
    {
        //从现在开始
        CTime_Value ttBegin = GetTimeofDay();
        pTimerInfo->Set_Timer_Param(nTimerID, nFrequency, ttBegin, fn_Timeout_Callback, pArgContext, fn_Timeout_Error_Callback);
    }

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
    m_TimerInfoList.Set_Event_Type(emType);

    m_TimerInfoList.Lock();

    if (NULL != m_TimerInfoList.Get_cond())
    {
#ifdef WIN32
        WakeAllConditionVariable(m_TimerInfoList.Get_cond());
#else
        pthread_cond_signal(m_TimerInfoList.Get_cond());
#endif
    }

    m_TimerInfoList.UnLock();
}
