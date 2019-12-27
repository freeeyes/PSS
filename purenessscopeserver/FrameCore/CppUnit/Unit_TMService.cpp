#include "Unit_TMService.h"

#ifdef _CPPUNIT_TEST

CUnit_TMService::CUnit_TMService()
{
}

void CUnit_TMService::setUp(void)
{
    m_pTMService = App_ServerObject::instance()->GetTMService();
}

void CUnit_TMService::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_TMService::tearDown]is CLose."));
}

void CUnit_TMService::Test_TMService(void)
{
    bool blRet = false;
    int nData = 0;
    CTestUser _test_user;
    Enum_Timer_Mode emTimerMode = Timer_Mode_Run_Once;

    int nRet = m_pTMService->AddMessage("test1", 1234, 1, 0, 1000, (void*)&nData, emTimerMode, (IMessagePrecess*)&_test_user);

    if (0 != nRet)
    {
        blRet = true;
        CPPUNIT_ASSERT_MESSAGE("[CUnit_TMService]m_pTMService->AddMessage is fail.", true == blRet);
        return;
    }

    ACE_Time_Value tvSleep(2, 0);
    ACE_OS::sleep(tvSleep);

    if (1 != nData)
    {
        blRet = true;
        CPPUNIT_ASSERT_MESSAGE("[CUnit_TMService]nData is not 1.", true == blRet);
        return;
    }

    OUR_DEBUG((LM_INFO, "[CUnit_TMService]objActiveTimer is finish.\n"));
}

#endif
