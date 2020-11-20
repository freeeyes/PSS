#include "Unit_ProfileTime.h"

#ifdef _CPPUNIT_TEST

void CUnit_ProfileTime::setUp(void)
{
    m_pProfileTime = std::make_shared<CProfileTime>();
}

void CUnit_ProfileTime::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_ProfileTime::tearDown]Finish.\n"));
}

void CUnit_ProfileTime::Test_ProfileTime(void)
{
    bool blRet = false;

    if (false == m_pProfileTime->Start())
    {
        OUR_DEBUG((LM_INFO, "[Test_ProfileTime]m_pProfileTime->Start() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ProfileTime]m_pProfileTime->Start() Error.", true == blRet);
        return;
    }

    ACE_Time_Value tvSleep(0, 1000);
    ACE_OS::sleep(tvSleep);

    uint64 u8TimeCost = m_pProfileTime->Stop();

    if (0 == u8TimeCost)
    {
        OUR_DEBUG((LM_INFO, "[Test_ProfileTime]m_pProfileTime->Stop() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ProfileTime]m_pProfileTime->Stop() Error.", true == blRet);
    }
}

#endif
