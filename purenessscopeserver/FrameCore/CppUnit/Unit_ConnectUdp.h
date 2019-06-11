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
    CUnit_ConnectUdp();

    virtual ~CUnit_ConnectUdp();

    CUnit_ConnectUdp(const CUnit_ConnectUdp& ar);

    CUnit_ConnectUdp& operator = (const CUnit_ConnectUdp& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_Connect_Udp_Server(void);
private:
    int m_nServerID;
};

#endif

#endif
