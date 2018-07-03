#include "Unit_TimeQueue.h"

#ifdef _CPPUNIT_TEST

CTimeTask::CTimeTask(void)
{

}

CTimeTask::~CTimeTask(void)
{

}

int CTimeTask::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    ACE_UNUSED_ARG(tv);
    ACE_UNUSED_ARG(arg);

    OUR_DEBUG((LM_INFO, "[CTimeTask::handle_timeout]!!!!!!!!!!!!!!!!!!!!!!!!.\n"));

    return 0;
}

CUnit_TimerManager::~CUnit_TimerManager()
{

}

void CUnit_TimerManager::setUp(void)
{
    m_pTimeTask = new CTimeTask();
}

void CUnit_TimerManager::tearDown(void)
{
    SAFE_DELETE(m_pTimeTask);
}

void CUnit_TimerManager::Test_TimerManager(void)
{
    bool blRet = false;
    ActiveTimer objActiveTimer;
    objActiveTimer.activate();

    ACE_Time_Value tvNow = ACE_OS::gettimeofday();
    long lTimerID = objActiveTimer.schedule(m_pTimeTask, NULL, tvNow + ACE_Time_Value(0, 1000));

    if (-1 == lTimerID)
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_TimerManager]schedule is fail.", true == blRet);
    }

    ACE_Time_Value tvSleep(0, 2000);
    ACE_OS::sleep(tvSleep);

    objActiveTimer.deactivate();
    objActiveTimer.close();

    ACE_Time_Value tvSleep(0, 1000);
    ACE_OS::sleep(tvSleep);
    OUR_DEBUG((LM_INFO, "[CUnit_TimerManager]objActiveTimer is close.\n"));
}

#endif
