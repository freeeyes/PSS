#ifndef UNIT_CONNECTCLIENT_H
#define UNIT_CONNECTCLIENT_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ConnectClient.h"

class CUnit_ConnectClient : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ConnectClient);
    CPPUNIT_TEST(Test_handle_input);
    CPPUNIT_TEST(Test_GetTimeout);
    CPPUNIT_TEST(Test_Output_Debug_Data);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ConnectClient() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_handle_input(void);

    void Test_GetTimeout(void);

    void Test_Output_Debug_Data(void);

private:
    shared_ptr<CConnectClient> m_pConnectClient = nullptr;
    int m_nTestCount = 0;
};

#endif

#endif
