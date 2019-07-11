#include "Unit_TSTimer.h"

#ifdef _CPPUNIT_TEST

CUnit_TimerThread::CUnit_TimerThread() : m_pTimerThread(NULL), m_pThreadNode(NULL)
{
}

void CUnit_TimerThread::setUp(void)
{
    m_pTimerThread = new ts_timer::CTimerThread();
    m_pThreadNode = new CTestTimeNode();
    m_pTimerThread->Init(10);
}

void CUnit_TimerThread::tearDown(void)
{
    m_pTimerThread->Close();

    ACE_Time_Value tvSleep(0, 1000);
    ACE_OS::sleep(tvSleep);

    SAFE_DELETE(m_pTimerThread);
    SAFE_DELETE(m_pThreadNode);
}

void CUnit_TimerThread::Test_TimerThread(void)
{
    bool blRet = false;

    //启动定时器
    m_pTimerThread->Run();

    //设置时间节点参数
    m_pThreadNode->SetTimerID(1);
    m_pThreadNode->SetFrequency(1000);

    int nRet = m_pTimerThread->Add_Timer((ts_timer::ITimeNode* )m_pThreadNode, NULL);

    if (0 != nRet)
    {
        blRet = true;
        CPPUNIT_ASSERT_MESSAGE("[Test_TimerThread]Add_Timer is fail.", true == blRet);
    }

    ACE_Time_Value tvSleep(5, 0);
    ACE_OS::sleep(tvSleep);

    OUR_DEBUG((LM_INFO, "[CUnit_TimerThread]objActiveTimer is close.\n"));
}

#endif
