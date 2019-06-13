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
    CUnit_ConnectClient();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_handle_input(void);

    void Test_GetTimeout(void);

    void Test_Output_Debug_Data(void);

private:
    CConnectClient* m_pConnectClient;
};

#endif

#endif
