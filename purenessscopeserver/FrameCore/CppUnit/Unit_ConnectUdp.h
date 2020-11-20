#ifndef _UNIT_CONNECTUDP_H
#define _UNIT_CONNECTUDP_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "PostUdpServerMessage.h"

const short COMMAND_AUTOTEST_UDP_HEAD = 0x1001;

class CUnit_ConnectUdp : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ConnectUdp);
    CPPUNIT_TEST(Test_Connect_Udp_Server);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ConnectUdp() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_Connect_Udp_Server(void);
private:
    int m_nServerID = 0;
    int m_nTestCount = 0;
};

#endif

#endif
