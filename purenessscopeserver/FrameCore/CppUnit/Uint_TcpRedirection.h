#ifndef UNIT_TCPREDIRECTION_H
#define UNIT_TCPREDIRECTION_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "TcpRedirection.h"
#include "ConnectHandler.h"
#include "ClientReConnectManager.h"
#include "XmlConfig.h"

class CUnit_Redirection : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_Redirection);
    CPPUNIT_TEST(Test_Redirection);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_Redirection() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_Redirection(void);

private:
    int Create_client(int nClientPort, const char* pIP, int nServerPort);

    CForwardManager* m_pForwardManager = nullptr;
    int m_nTestCount = 0;
};

#endif

#endif
