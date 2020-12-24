#include "Unit_ControlListen.h"

#ifdef _CPPUNIT_TEST

void CUnit_ControlListen::setUp(void)
{
    m_pControlListen = std::make_shared<CControlListen>();
}

void CUnit_ControlListen::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_ControlListen::tearDown]Finish.");
}

void CUnit_ControlListen::Test_ControlListen(void)
{
    bool blRet = false;

    if (false == m_pControlListen->AddListen("127.0.0.1", 10007, TYPE_IPV4, 1))
    {
        PSS_LOGGER_DEBUG("[Test_ControlListen]m_pControlListen->AddListen() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_ControlListen]m_pControlListen->AddListen() Error.", true == blRet);
        return;
    }

    ACE_Time_Value tvSleep(0, 1000);
    ACE_OS::sleep(tvSleep);

    if (0 >= m_pControlListen->GetListenCount())
    {
        PSS_LOGGER_DEBUG("[Test_ControlListen]m_pControlListen->GetListenCount() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_ControlListen]m_pControlListen->GetListenCount() Error.", true == blRet);
        return;
    }

    if (1 != m_pControlListen->GetServerID())
    {
        PSS_LOGGER_DEBUG("[Test_ControlListen]m_pControlListen->GetServerID() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_ControlListen]m_pControlListen->GetServerID() Error.", true == blRet);
        return;
    }

    if (false == m_pControlListen->DelListen("127.0.0.1", 10007))
    {
        PSS_LOGGER_DEBUG("[Test_ControlListen]m_pControlListen->AddListen() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_ControlListen]m_pControlListen->AddListen() Error.", true == blRet);
    }
}

#endif
