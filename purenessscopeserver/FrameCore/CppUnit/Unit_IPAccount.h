#ifndef _UNIT_IPACCOUNT_H
#define _UNIT_IPACCOUNT_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "IPAccount.h"

class CUnit_IPAccount : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_IPAccount);
    CPPUNIT_TEST(Test_IPAccount_Add);
    CPPUNIT_TEST(Test_IPAccount_Count);
    CPPUNIT_TEST(Test_IPAccount_LastConnectCount);
    CPPUNIT_TEST(Test_IPAccount_GetInfo);
    CPPUNIT_TEST(Test_CConnectAccount);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_IPAccount() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_IPAccount_Add(void);

    void Test_IPAccount_Count(void);

    void Test_IPAccount_LastConnectCount(void);

    void Test_IPAccount_GetInfo(void);

    void Test_CConnectAccount(void);

private:
    shared_ptr<CIPAccount> m_pIPAccount = nullptr;
    int m_nTestCount         = 0;
};

#endif

#endif
