#ifndef UNIT_COMMANDACCOUNT_H
#define UNIT_COMMANDACCOUNT_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "CommandAccount.h"

class CUnit_CommandAccount : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_CommandAccount);
    CPPUNIT_TEST(Test_CommandAccount_Init);
    CPPUNIT_TEST(Test_CommandAccount_Alert);
    CPPUNIT_TEST(Test_CommandAccount_Flow);
    CPPUNIT_TEST(Test_CommandAccount_PortList);
    CPPUNIT_TEST(Test_Error);
    CPPUNIT_TEST_SUITE_END();

public:
    virtual ~CUnit_CommandAccount();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_CommandAccount_Init(void);          //测试正常输出
    void Test_CommandAccount_Alert(void);         //测试告警输出
    void Test_CommandAccount_Flow(void);          //测试输入输出数据
    void Test_CommandAccount_PortList(void);      //测试端口数据
    void Test_Error(void);                        //测试错误

protected:
    CCommandAccount* m_pCommandAccount;
    uint8            m_u1CommandAccount;
    uint8            m_u1Flow;
    uint16           m_u2RecvTimeout;
};

#endif

#endif
