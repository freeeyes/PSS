#ifndef UNIT_CONSOLEMESSAGE_H
#define UNIT_CONSOLEMESSAGE_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ConsoleMessage.h"

class CUnit_ConsoleMessage : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ConsoleMessage);
    CPPUNIT_TEST(Test_DoMessage_ShowModule);
    CPPUNIT_TEST(Test_DoMessage_ClientMessageCount);
    CPPUNIT_TEST(Test_DoMessage_CommandInfo);
    CPPUNIT_TEST(Test_DoMessage_WorkThreadState);
    CPPUNIT_TEST(Test_DoMessage_ClientInfo);
    CPPUNIT_TEST(Test_DoMessage_ForbiddenIP);
    CPPUNIT_TEST(Test_DoMessage_ShowForbiddenList);
    CPPUNIT_TEST(Test_DoMessage_LifedIP);
    CPPUNIT_TEST(Test_DoMessage_ServerConnectTCP);
    CPPUNIT_TEST(Test_DoMessage_ServerConnectUDP);
    CPPUNIT_TEST(Test_DoMessage_ShowProcessInfo);
    CPPUNIT_TEST(Test_DoMessage_ShowClientHisTory);
    CPPUNIT_TEST(Test_DoMessage_ShowAllCommandInfo);
    CPPUNIT_TEST(Test_DoMessage_ShowServerInfo);
    CPPUNIT_TEST(Test_DoMessage_ReConnectServer);
    CPPUNIT_TEST(Test_DoMessage_CommandTimeout);
    CPPUNIT_TEST_SUITE_END();

public:
    virtual ~CUnit_ConsoleMessage();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_DoMessage_ShowModule(void);
    void Test_DoMessage_ClientMessageCount(void);
    void Test_DoMessage_CommandInfo(void);
    void Test_DoMessage_WorkThreadState(void);
    void Test_DoMessage_ClientInfo(void);
    void Test_DoMessage_ForbiddenIP(void);
    void Test_DoMessage_ShowForbiddenList(void);
    void Test_DoMessage_LifedIP(void);
    void Test_DoMessage_ServerConnectTCP(void);
    void Test_DoMessage_ServerConnectUDP(void);
    void Test_DoMessage_ShowProcessInfo(void);
    void Test_DoMessage_ShowClientHisTory(void);
    void Test_DoMessage_ShowAllCommandInfo(void);
    void Test_DoMessage_ShowServerInfo(void);
    void Test_DoMessage_ReConnectServer(void);
    void Test_DoMessage_CommandTimeout(void);

private:
    bool Create_Command(const char* pCommand, uint16 u2ReturnCommandID);

    CConsoleMessage* m_pConsoleMessage;
};

#endif

#endif
