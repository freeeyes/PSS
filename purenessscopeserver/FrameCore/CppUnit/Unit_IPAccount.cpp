#include "Unit_IPAccount.h"

#ifdef _CPPUNIT_TEST

void CUnit_IPAccount::setUp(void)
{
    m_pIPAccount = std::make_shared<CIPAccount>();

    m_pIPAccount->Init(10);
}

void CUnit_IPAccount::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_IPAccount::tearDown]Finish.");
}

void CUnit_IPAccount::Test_IPAccount_Add(void)
{
    bool blRet = false;
    blRet = m_pIPAccount->AddIP("127.0.0.1");

    CPPUNIT_ASSERT_MESSAGE("[CUnit_IPAccount]Test_IPAccount_Add is fail.", true == blRet);

    _IPAccount obj_IPAccount;
    ACE_Date_Time dtNowTime;
    blRet = obj_IPAccount.Check(dtNowTime);

    CPPUNIT_ASSERT_MESSAGE("[CUnit_IPAccount]obj_IPAccount->Check is fail.", true == blRet);
}

void CUnit_IPAccount::Test_IPAccount_Count(void)
{
    bool blRet = false;
    m_pIPAccount->AddIP("127.0.0.1");

    if (1 == m_pIPAccount->GetCount())
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_IPAccount_Count]GetCount is fail.", true == blRet);
}

void CUnit_IPAccount::Test_IPAccount_LastConnectCount(void)
{
    bool blRet = false;
    m_pIPAccount->AddIP("127.0.0.1");

    if (0 == m_pIPAccount->GetLastConnectCount())
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_IPAccount_LastConnectCount]GetLastConnectCount is fail.", true == blRet);
}

void CUnit_IPAccount::Test_IPAccount_GetInfo(void)
{
    bool blRet = false;
    m_pIPAccount->AddIP("127.0.0.1");

    vecIPAccount VecIPAccount;
    m_pIPAccount->GetInfo(VecIPAccount);

    if (1 == VecIPAccount.size() &&
        ACE_OS::strcmp(VecIPAccount[0].m_strIP.c_str(), "127.0.0.1") == 0)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_IPAccount_GetInfo]GetInfo is fail.", true == blRet);
}

void CUnit_IPAccount::Test_CConnectAccount(void)
{
    bool blRet = false;
    CConnectAccount objConnectAccount;

    objConnectAccount.Init(0, 1000, 0, 1000);

    if (0 != objConnectAccount.Get4ConnectMin() || 1000 != objConnectAccount.GetConnectMax())
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_CConnectAccount]Get4ConnectMin or GetConnectMax is error.", true == blRet);
        return;
    }

    if (0 != objConnectAccount.GetDisConnectMin()
        || 1000 != objConnectAccount.GetDisConnectMax()
        || 0 != objConnectAccount.GetCurrDisConnect())
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_CConnectAccount]GetDisConnectMin or GetDisConnectMin is error.", true == blRet);
        return;
    }

    if (false == objConnectAccount.AddConnect() || false == objConnectAccount.AddDisConnect())
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_CConnectAccount]GetDisConnectMin or GetDisConnectMin is error.", true == blRet);
    }
    m_nTestCount++;
}

#endif


