#include "TimerManager.h"

CTimerManager::CTimerManager(void)
{
    m_pTimerQueue = NULL;
}

bool CTimerManager::Init()
{
    if(NULL == m_pTimerQueue)
    {
        m_pTimerQueue = new ACE_Timer_Heap();
    }

    if(NULL == m_pTimerQueue)
    {
        OUR_DEBUG((LM_INFO,"[CTimerManager::Init] m_pTimerQueue = NULL.\n"));
        return false;
    }

    return true;
}

void CTimerManager::Close()
{
    if(NULL != m_pTimerQueue)
    {
        delete m_pTimerQueue;
        m_pTimerQueue = NULL;
    }
}

CTimerManager::~CTimerManager(void)
{
    Close();
}

void CTimerManager::wait_for_event(void)
{
    if(NULL == m_pTimerQueue)
    {
        OUR_DEBUG((LM_INFO,"[CTimerManager::wait_for_event] m_pTimerQueue = NULL.\n"));
        return;
    }

    while(true)
    {
        //ACE_Time_Value tvNow(ACE_OS::gettimeofday());
        //ACE_Time_Value* tvTimeout = m_pTimerQueue->calculate_timeout(&tvNow);

        //if(*tvTimeout == ACE_Time_Value::zero)
        //{
        //  m_pTimerQueue->expire();
        //}
        //else
        //{
        //  ACE_Time_Value tvNext(ACE_OS::gettimeofday());

        //  tvNext += *tvTimeout;

        //  if(m_EvTimer.wait(&tvNext) == -1)
        //  {
        //      m_pTimerQueue->expire();
        //  }
        //}

        //int time = m_pTimerQueue->expire();
        if(m_pTimerQueue->is_empty())
        {
            ACE_OS::sleep(1);
        }
        else
        {
            ACE_Time_Value time;
            time.msec(10);
            ACE_OS::sleep(time);
        }
    }
}

long CTimerManager::schedule(ACE_Event_Handler* cb, void* arg, const ACE_Time_Value& abs_time, const ACE_Time_Value& interval)
{
    if(NULL == m_pTimerQueue)
    {
        OUR_DEBUG((LM_INFO,"[CTimerManager::schedule] m_pTimerQueue = NULL.\n"));
        return 0;
    }

    return m_pTimerQueue->schedule(cb, arg, m_pTimerQueue->gettimeofday()+abs_time, interval);
}

int CTimerManager::cancel(int nTimerID)
{
    if(NULL == m_pTimerQueue)
    {
        OUR_DEBUG((LM_INFO,"[CTimerManager::cancel] m_pTimerQueue = NULL.\n"));
        return 0;
    }

    return m_pTimerQueue->cancel(nTimerID);
}

int CTimerManager::reset_interval(long timer_id, const ACE_Time_Value& interval)
{
    if(NULL == m_pTimerQueue)
    {
        OUR_DEBUG((LM_INFO,"[CTimerManager::reset_interval] m_pTimerQueue = NULL.\n"));
        return 0;
    }

    return m_pTimerQueue->reset_interval(timer_id, interval);
}

CTimerManagerTask::CTimerManagerTask()
{
}

CTimerManagerTask::~CTimerManagerTask()
{

}

int CTimerManagerTask::open(void* args)
{
    if(args != NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CTimerManagerTask::open] args not NULL.\n"));
    }

    OUR_DEBUG((LM_ERROR, "[CTimerManagerTask::open] Begin.\n"));

    if(activate(THREAD_PARAM, MAX_TIMERMANAGER_THREADCOUNT)  == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CTimerManagerTask::open] CTimerManagerTask Start error!\n"));
        return -1;
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CTimerManagerTask::open] OK\n"));
        return 0;
    }
}

int CTimerManagerTask::svc(void)
{
    m_TimerManager.wait_for_event();
    return 0;
}

bool CTimerManagerTask::Start()
{
    if(false == m_TimerManager.Init())
    {
        return false;
    }

    if(0 == open())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CTimerManagerTask::Stop()
{
    m_TimerManager.Close();
    return true;
}

long CTimerManagerTask::schedule(ACE_Event_Handler* cb, void* arg, const ACE_Time_Value& abs_time, const ACE_Time_Value& interval)
{
    return m_TimerManager.schedule(cb, arg, abs_time, interval);
}

int CTimerManagerTask::cancel(int nTimerID)
{
    return m_TimerManager.cancel(nTimerID);
}

