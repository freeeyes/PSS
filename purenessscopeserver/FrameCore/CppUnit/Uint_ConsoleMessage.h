#ifndef UNIT_CONSOLEMESSAGE_H
#define UNIT_CONSOLEMESSAGE_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ConsoleMessage.h"
#include "BaseConsoleHandle.h"
#include <string>
#include <sstream>

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
    CPPUNIT_TEST(Test_Do_Message_ReLoadModule);
    CPPUNIT_TEST(Test_Do_Error_Command);
    CPPUNIT_TEST(Test_Check_Console_Ip);
    CPPUNIT_TEST(Test_Do_Message_CloseClient);
    CPPUNIT_TEST(Test_Do_Message_UDPClientInfo);
    CPPUNIT_TEST(Test_Do_Message_SetTrackIP);
    CPPUNIT_TEST(Test_Do_Message_SetLogLevelInfo);
    CPPUNIT_TEST(Test_Do_Message_SetWorkThreadAI);
    CPPUNIT_TEST(Test_Do_Message_SetMaxConnectCount);
    CPPUNIT_TEST(Test_Do_Message_SetConnectLog);
    CPPUNIT_TEST(Test_Do_Message_AddListen);
    CPPUNIT_TEST(Test_Do_Message_DelListen);
    CPPUNIT_TEST(Test_Do_Message_TestFileStart);
    CPPUNIT_TEST(Test_Do_Message_TestFileStop);
    CPPUNIT_TEST(Test_Do_Message_PoolSet);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ConsoleMessage() = default;

    void setUp(void) final;

    void tearDown(void) final;

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
    void Test_Do_Error_Command(void);
    void Test_Check_Console_Ip(void);
    void Test_Do_Message_CloseClient(void);
    void Test_Do_Message_UDPClientInfo(void);
    void Test_Do_Message_SetTrackIP(void);
    void Test_Do_Message_SetLogLevelInfo(void);
    void Test_Do_Message_SetWorkThreadAI(void);
    void Test_Do_Message_SetMaxConnectCount(void);
    void Test_Do_Message_SetConnectLog(void);
    void Test_Do_Message_AddListen(void);
    void Test_Do_Message_DelListen(void);
    void Test_Do_Message_TestFileStart(void);
    void Test_Do_Message_TestFileStop(void);
    void Test_Do_Message_PoolSet(void);
    void Test_Do_Message_ReLoadModule(void);

private:
    bool Create_Command(const char* pCommand, uint16 u2ReturnCommandID);
    bool Create_Command_Error(const char* pCommand);

    shared_ptr<CConsoleMessage> m_pConsoleMessage = nullptr;
    int m_nTestCount = 0;
};

#endif

#endif
