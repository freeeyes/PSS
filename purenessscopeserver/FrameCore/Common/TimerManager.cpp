#include "TimerManager.h"

void PSS_Timer_Manager::Start()
{
    m_timerMgr = std::make_shared<brynet::TimerMgr>();

    m_ttTimerThread = std::thread([this]()
        {
            m_timerMgr->schedule();
            OUR_DEBUG((LM_INFO, "[PSS_Timer_Manager::start]End.\n"));
        });
}

void PSS_Timer_Manager::Close()
{
    m_timerMgr->Close();

    m_ttTimerThread.join();
}

brynet::TimerMgr::Ptr PSS_Timer_Manager::GetTimerPtr() const
{
    return m_timerMgr;
}
