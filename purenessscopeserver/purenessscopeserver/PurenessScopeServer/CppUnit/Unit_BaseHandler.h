#ifndef UNIT_BASEHANDLER_H
#define UNIT_BASEHANDLER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "BaseHander.h"

class CUnit_Basehandler : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_Basehandler);
    CPPUNIT_TEST(Test_Udp_Common_Recv_Stream);
    CPPUNIT_TEST(Test_Common_Recv_Stream);
    CPPUNIT_TEST(Test_Tcp_Common_File_Message);
    CPPUNIT_TEST(Test_Tcp_Common_ClientInfo);
    CPPUNIT_TEST(Test_Tcp_Common_Send_Input_To_Cache);
    CPPUNIT_TEST(Test_Tcp_Common_Manager_Timeout_CheckInfo);
    CPPUNIT_TEST_SUITE_END();
public:

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_Udp_Common_Recv_Stream(void);
    void Test_Common_Recv_Stream(void);
    void Test_Tcp_Common_File_Message(void);
    void Test_Tcp_Common_ClientInfo(void);
    void Test_Tcp_Common_Send_Input_To_Cache(void);
    void Test_Tcp_Common_Manager_Timeout_CheckInfo(void);
private:
    int m_nBaseHandlerID;

};

#endif

#endif
