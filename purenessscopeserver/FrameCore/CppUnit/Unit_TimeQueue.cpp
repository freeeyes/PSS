#include "Unit_TimeQueue.h"

#ifdef _CPPUNIT_TEST

void CUnit_TimerManager::setUp(void)
{
    m_pTimeTask = new CTimeTask();
}

void CUnit_TimerManager::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_TimerManager::tearDown]Finish.\n"));
}

void CUnit_TimerManager::Test_TimerManager(void)
{
    auto timerMgr = App_TimerManager::instance()->GetTimerPtr();

    timerMgr->addTimer(std::chrono::microseconds(500), []() {
        CTimeTask Time_Task;
        Time_Task.Test_Timer_Task();
        });

    std::this_thread::sleep_for(std::chrono::microseconds(600));

    cout << "[Test_Timer]End" << endl;

    OUR_DEBUG((LM_INFO, "[CUnit_TimerManager]objActiveTimer is close.\n"));
}

#endif
