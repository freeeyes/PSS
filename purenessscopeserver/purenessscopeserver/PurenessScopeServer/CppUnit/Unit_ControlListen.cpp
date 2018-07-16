#include "Unit_ControlListen.h"

#ifdef _CPPUNIT_TEST

CUnit_ControlListen::~CUnit_ControlListen()
{
    m_pControlListen = NULL;
}

void CUnit_ControlListen::setUp(void)
{
    m_pControlListen = new CControlListen();
}

void CUnit_ControlListen::tearDown(void)
{
    delete m_pControlListen;
    m_pControlListen = NULL;
}

void CUnit_ControlListen::Test_ControlListen(void)
{
    bool blRet = false;

    if (false == m_pControlListen->AddListen("127.0.0.1", 10005, TYPE_IPV4, 1))
    {
        OUR_DEBUG((LM_INFO, "[Test_ControlListen]m_pControlListen->AddListen() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ControlListen]m_pControlListen->AddListen() Error.", true == blRet);
        return;
    }

    ACE_Time_Value tvSleep(0, 1000);
    ACE_OS::sleep(tvSleep);

    if (0 >= m_pControlListen->GetListenCount())
    {
        OUR_DEBUG((LM_INFO, "[Test_ControlListen]m_pControlListen->GetListenCount() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ControlListen]m_pControlListen->GetListenCount() Error.", true == blRet);
        return;
    }

    if (1 != m_pControlListen->GetServerID())
    {
        OUR_DEBUG((LM_INFO, "[Test_ControlListen]m_pControlListen->GetServerID() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ControlListen]m_pControlListen->GetServerID() Error.", true == blRet);
        return;
    }

    if (false == m_pControlListen->DelListen("127.0.0.1", 10005))
    {
        OUR_DEBUG((LM_INFO, "[Test_ControlListen]m_pControlListen->AddListen() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ControlListen]m_pControlListen->AddListen() Error.", true == blRet);
    }
}

#endif
