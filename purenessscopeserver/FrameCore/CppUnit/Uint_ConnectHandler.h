#ifndef UNIT_CONNECTHANDLER_H
#define UNIT_CONNECTHANDLER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ConnectHandler.h"
#include "HandlerManager.h"

class CUnit_ConnectHandler : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ConnectHandler);
    CPPUNIT_TEST(Test_ConnectHandler_Stream);
    CPPUNIT_TEST(Test_ConnectHandler_Debug);
    CPPUNIT_TEST(Test_ConnectHandler_Close_Queue);
    CPPUNIT_TEST(Test_ConnectHandler_PostMessage);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ConnectHandler() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_ConnectHandler_Stream(void);

    void Test_ConnectHandler_Debug(void);

    void Test_ConnectHandler_Close_Queue(void);

    void Test_ConnectHandler_PostMessage(void);

private:
    shared_ptr<CConnectHandler> m_pConnectHandler = nullptr;
    int m_nTestCount = 0;
};

#endif

#endif
