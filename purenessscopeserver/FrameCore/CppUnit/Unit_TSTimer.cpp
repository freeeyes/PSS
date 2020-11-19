#include "Unit_TSTimer.h"

#ifdef _CPPUNIT_TEST

void test_timer_run_execute(shared_ptr<ITimerInfo> arg)
{
    ACE_UNUSED_ARG(arg);
    OUR_DEBUG((LM_INFO, "[CUnit_TimerThread]timer is active.\n"));
}

CUnit_TimerThread::CUnit_TimerThread()
{
}

void CUnit_TimerThread::setUp(void)
{
    m_pTimerThread = new CTimerManager();
}

void CUnit_TimerThread::tearDown(void)
{
    m_pTimerThread->Close();
    SAFE_DELETE(m_pTimerThread);
}

void CUnit_TimerThread::Test_TimerThread(void)
{
    bool blRet = false;

    milliseconds millsleep(1000);
    int nRet = m_pTimerThread->Add_Timer(1, millsleep, test_timer_run_execute, nullptr);

    if (0 != nRet)
    {
        blRet = true;
        CPPUNIT_ASSERT_MESSAGE("[Test_TimerThread]Add_Timer is fail.", true == blRet);
    }

    ACE_Time_Value tvSleep(2, 0);
    ACE_OS::sleep(tvSleep);

    m_pTimerThread->Close();

    OUR_DEBUG((LM_INFO, "[CUnit_TimerThread]objActiveTimer is close.\n"));
}

#endif
