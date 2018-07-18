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
    CPPUNIT_TEST(Test_DoMessage_CommandTimeoutclr);
    CPPUNIT_TEST(Test_DoMessage_CommandDataLog);
    CPPUNIT_TEST(Test_DoMessage_SetDebug);
    CPPUNIT_TEST(Test_DoMessage_ShowDebug);
    CPPUNIT_TEST(Test_DoMessage_GetTrackCommand);
    CPPUNIT_TEST(Test_DoMessage_GetConnectIPInfo);
    CPPUNIT_TEST(Test_DoMessage_GetLogLevelInfo);
    CPPUNIT_TEST(Test_DoMessage_GetThreadAI);
    CPPUNIT_TEST(Test_DoMessage_GetWorkThreadTO);
    CPPUNIT_TEST(Test_DoMessage_GetNickNameInfo);
    CPPUNIT_TEST(Test_DoMessage_ShowListen);
    CPPUNIT_TEST(Test_DoMessage_MonitorInfo);
    CPPUNIT_TEST(Test_DoMessage_PortList);
    CPPUNIT_TEST(Test_Do_Message_BuffPacket);
    CPPUNIT_TEST(Test_Do_Message_LoadModule);
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
    void Test_DoMessage_CommandTimeoutclr(void);
    void Test_DoMessage_CommandDataLog(void);
    void Test_DoMessage_SetDebug(void);
    void Test_DoMessage_ShowDebug(void);
    void Test_DoMessage_GetTrackCommand(void);
    void Test_DoMessage_GetConnectIPInfo(void);
    void Test_DoMessage_GetLogLevelInfo(void);
    void Test_DoMessage_GetThreadAI(void);
    void Test_DoMessage_GetWorkThreadTO(void);
    void Test_DoMessage_GetNickNameInfo(void);
    void Test_DoMessage_ShowListen(void);
    void Test_DoMessage_MonitorInfo(void);
    void Test_DoMessage_PortList(void);
    void Test_Do_Message_BuffPacket(void);
    void Test_Do_Message_LoadModule(void);

private:
    bool Create_Command(const char* pCommand, uint16 u2ReturnCommandID);

    CConsoleMessage* m_pConsoleMessage;
};

#endif

#endif
