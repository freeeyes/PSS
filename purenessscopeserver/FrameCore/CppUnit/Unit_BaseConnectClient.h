#ifndef _UNIT_BASECONNECTCLIENT_H
#define _UNIT_BASECONNECTCLIENT_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "PostServerMessage.h"
#include "BaseConnectClient.h"
#include "BaseClientConnectManager.h"

class CUnit_BaseConnectClient : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_BaseConnectClient);
    CPPUNIT_TEST(Test_Common_Send_ConnectError);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_BaseConnectClient() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_Common_Send_ConnectError(void);

private:
    uint16 m_u2CommandID = 0;
    int m_nTestCount = 0;
};

#endif

#endif
