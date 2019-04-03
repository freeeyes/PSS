#ifndef _UNIT_CONNECTTCP_H
#define _UNIT_CONNECTTCP_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "PostServerMessage.h"

#define COMMAND_AUTOTEST_HEAD     0x1000

class CUnit_ConnectTcp : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ConnectTcp);
    CPPUNIT_TEST(Test_Connect_Tcp_Server);
    CPPUNIT_TEST(Test_Connect_Tcp_Server_With_Local);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ConnectTcp();

    virtual ~CUnit_ConnectTcp();

    CUnit_ConnectTcp(const CUnit_ConnectTcp& ar);

    CUnit_ConnectTcp& operator = (const CUnit_ConnectTcp& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_Connect_Tcp_Server(void);

    void Test_Connect_Tcp_Server_With_Local(void);

private:
    int m_nServerID;
};

#endif

#endif
