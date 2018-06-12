#include "Unit_IPAccount.h"

#ifdef _CPPUNIT_TEST

CUnit_IPAccount::~CUnit_IPAccount()
{

}

void CUnit_IPAccount::setUp(void)
{
    m_pIPAccount = new CIPAccount();

    m_pIPAccount->Init(10);
}

void CUnit_IPAccount::tearDown(void)
{
    SAFE_DELETE(m_pIPAccount);
}

void CUnit_IPAccount::Test_IPAccount_Add(void)
{
    bool blRet = false;
    blRet = m_pIPAccount->AddIP("127.0.0.1");

    CPPUNIT_ASSERT_MESSAGE("[CUnit_IPAccount]Test_IPAccount_Add is fail.", true == blRet);
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

#endif


