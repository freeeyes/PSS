#include "Uint_ConsoleMessage.h"

#ifdef _CPPUNIT_TEST

void CUnit_ConsoleMessage::setUp(void)
{
    m_pConsoleMessage = std::make_shared<CConsoleMessage>();
    m_pConsoleMessage->Init();
}

void CUnit_ConsoleMessage::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_ConsoleMessage::tearDown]Finish.");
}

bool CUnit_ConsoleMessage::Create_Command(const char* pCommand, uint16 u2ReturnCommandID)
{
    bool blRet             = false;
    uint8 u1Output         = 0;
    ACE_Message_Block* pmb = nullptr;

    auto pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    //拼接命令字
    auto nCommandLen = (int)ACE_OS::strlen(pCommand);

    pmb = new ACE_Message_Block(ACE_OS::strlen(pCommand));
    memcpy_safe(pCommand, nCommandLen, pmb->wr_ptr(), nCommandLen);
    pmb->wr_ptr(ACE_OS::strlen(pCommand));

    //执行命令
    if (EM_CONSOLE_MESSAGE::CONSOLE_MESSAGE_SUCCESS != m_pConsoleMessage->Dispose(pmb, pBuffPacket, u1Output))
    {
        std::stringstream ss_format;
        ss_format << "[Create_Command](" << pCommand << ")m_pConsoleMessage->Dispose.";
        string strError = ss_format.str();

        CPPUNIT_ASSERT_MESSAGE(strError.c_str(), true == blRet);
        return false;
    }

    if (pCommand[0] == 'b')
    {
        //比较返回关键字是否一致
        uint16 u2CommandID = 0;

        (*pBuffPacket) >> u2CommandID;

        if (u2ReturnCommandID != u2CommandID)
        {
            char szError[MAX_BUFF_200] = { '\0' };
            sprintf_safe(szError, MAX_BUFF_200, "[Create_Command](%s) u2ReturnCommandID error(%d).", pCommand, u2CommandID);
            CPPUNIT_ASSERT_MESSAGE(szError, true == blRet);
            return false;
        }
    }

    //回收命令行内存
    pmb->release();

    return blRet;
}

bool CUnit_ConsoleMessage::Create_Command_Error(const char* pCommand)
{
    bool blRet = false;
    uint8 u1Output = 0;
    ACE_Message_Block* pmb = nullptr;

    auto pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    //拼接命令字
    int nCommandLen = (int)ACE_OS::strlen(pCommand);

    pmb = new ACE_Message_Block(ACE_OS::strlen(pCommand));
    memcpy_safe(pCommand, nCommandLen, pmb->wr_ptr(), nCommandLen);
    pmb->wr_ptr(ACE_OS::strlen(pCommand));

    //执行命令
    if (EM_CONSOLE_MESSAGE::CONSOLE_MESSAGE_FAIL != m_pConsoleMessage->Dispose(pmb, pBuffPacket, u1Output))
    {
        char szError[MAX_BUFF_200] = { '\0' };
        sprintf_safe(szError, MAX_BUFF_200, "[Create_Command_Error](%s)m_pConsoleMessage->Dispose.", pCommand);
        CPPUNIT_ASSERT_MESSAGE(szError, true == blRet);
        return false;
    }
    else
    {
        blRet = true;
    }

    //回收命令行内存
    pmb->release();

    return blRet;
}

void CUnit_ConsoleMessage::Test_DoMessage_ShowModule(void)
{
    //测试二进制输出
    Create_Command("b freeeyes ShowModule -a&", CONSOLE_COMMAND_SHOWMOUDLE);

    //测试文本输出
    Create_Command("t freeeyes ShowModule -a&", CONSOLE_COMMAND_SHOWMOUDLE);
}

void CUnit_ConsoleMessage::Test_DoMessage_ClientMessageCount(void)
{
    Create_Command("b freeeyes ClientCount -cp&", CONSOLE_COMMAND_CLIENTCOUNT);
    Create_Command("t freeeyes ClientCount -cp&", CONSOLE_COMMAND_CLIENTCOUNT);
}

void CUnit_ConsoleMessage::Test_DoMessage_CommandInfo(void)
{
    Create_Command("b freeeyes CommandInfo 0x1000&", CONSOLE_COMMAND_COMMANDINFO);
    Create_Command("t freeeyes CommandInfo 0x1000&", CONSOLE_COMMAND_COMMANDINFO);
}

void CUnit_ConsoleMessage::Test_DoMessage_WorkThreadState(void)
{
    Create_Command("b freeeyes WorkThreadState -s&", CONSOLE_COMMAND_THREADINFO);
    Create_Command("t freeeyes WorkThreadState -s&", CONSOLE_COMMAND_THREADINFO);
}

void CUnit_ConsoleMessage::Test_DoMessage_ClientInfo(void)
{
    Create_Command("b freeeyes ConnectClient -a&", CONSOLE_COMMAND_CLIENTINFO);
    Create_Command("t freeeyes ConnectClient -a&", CONSOLE_COMMAND_CLIENTINFO);
}

void CUnit_ConsoleMessage::Test_DoMessage_ForbiddenIP(void)
{
    Create_Command("b freeeyes ForbiddenIP -c 127.0.1.0 -t 0 -s 30 &", CONSOLE_COMMAND_FORBIDDENIP);
    Create_Command("t freeeyes ForbiddenIP -c 127.0.1.0 -t 0 -s 30 &", CONSOLE_COMMAND_FORBIDDENIP);
}

void CUnit_ConsoleMessage::Test_DoMessage_ShowForbiddenList(void)
{
    Create_Command("b freeeyes ShowForbiddenIP -a&", CONSOLE_COMMAND_FORBIDDENIPSHOW);
    Create_Command("t freeeyes ShowForbiddenIP -a&", CONSOLE_COMMAND_FORBIDDENIPSHOW);
}

void CUnit_ConsoleMessage::Test_DoMessage_LifedIP(void)
{
    Create_Command("b freeeyes LiftedIP 127.0.1.0&", CONSOLE_COMMAND_LIFTED);
    Create_Command("t freeeyes LiftedIP 127.0.1.0&", CONSOLE_COMMAND_LIFTED);
}

void CUnit_ConsoleMessage::Test_DoMessage_ServerConnectTCP(void)
{
    Create_Command("b freeeyes ServerConnectTCP -a&", CONSOLE_COMMAND_SERVERCONNECT_TCP);
    Create_Command("t freeeyes ServerConnectTCP -a&", CONSOLE_COMMAND_SERVERCONNECT_TCP);
}

void CUnit_ConsoleMessage::Test_DoMessage_ServerConnectUDP(void)
{
    Create_Command("b freeeyes ServerConnectUDP -a&", CONSOLE_COMMAND_SERVERCONNECT_UDP);
    Create_Command("t freeeyes ServerConnectUDP -a&", CONSOLE_COMMAND_SERVERCONNECT_UDP);
}

void CUnit_ConsoleMessage::Test_DoMessage_ShowProcessInfo(void)
{
    Create_Command("b freeeyes ShowCurrProcessInfo -a&", CONSOLE_COMMAND_PROCESSINFO);
    Create_Command("t freeeyes ShowCurrProcessInfo -a&", CONSOLE_COMMAND_PROCESSINFO);
}

void CUnit_ConsoleMessage::Test_DoMessage_ShowClientHisTory(void)
{
    Create_Command("b freeeyes ShowConnectHistory -a&", CONSOLE_COMMAND_CLIENTHISTORY);
    Create_Command("t freeeyes ShowConnectHistory -a&", CONSOLE_COMMAND_CLIENTHISTORY);
}

void CUnit_ConsoleMessage::Test_DoMessage_ShowAllCommandInfo(void)
{
    Create_Command("b freeeyes ShowAllCommand -a&", CONSOLE_COMMAND_ALLCOMMANDINFO);
    Create_Command("t freeeyes ShowAllCommand -a&", CONSOLE_COMMAND_ALLCOMMANDINFO);
}

void CUnit_ConsoleMessage::Test_DoMessage_ShowServerInfo(void)
{
    Create_Command("b freeeyes ShowServerInfo -a&", CONSOLE_COMMAND_SERVERINFO);
    Create_Command("t freeeyes ShowServerInfo -a&", CONSOLE_COMMAND_SERVERINFO);
}

void CUnit_ConsoleMessage::Test_DoMessage_ReConnectServer(void)
{
    Create_Command("b freeeyes ReConnectServer -s 127.0.0.1&", CONSOLE_COMMAND_SERVERRECONNECT);
    Create_Command("t freeeyes ReConnectServer -s 127.0.0.1&", CONSOLE_COMMAND_SERVERRECONNECT);
}

void CUnit_ConsoleMessage::Test_DoMessage_CommandTimeout(void)
{
    Create_Command("b freeeyes CommandTimeout -a&", CONSOLE_COMMAND_COMMANDTIMEOUT);
    Create_Command("t freeeyes CommandTimeout -a&", CONSOLE_COMMAND_COMMANDTIMEOUT);
}

void CUnit_ConsoleMessage::Test_DoMessage_CommandTimeoutclr(void)
{
    Create_Command("b freeeyes CommandTimeoutclr -a&", CONSOLE_COMMAND_COMMANDTIMEOUTCLR);
    Create_Command("t freeeyes CommandTimeoutclr -a&", CONSOLE_COMMAND_COMMANDTIMEOUTCLR);
}

void CUnit_ConsoleMessage::Test_DoMessage_CommandDataLog(void)
{
    Create_Command("b freeeyes CommandDataLog -a&", CONSOLE_COMMAND_COMMANDDATALOG);
    Create_Command("t freeeyes CommandDataLog -a&", CONSOLE_COMMAND_COMMANDDATALOG);
}

void CUnit_ConsoleMessage::Test_DoMessage_SetDebug(void)
{
    char szCommand[MAX_BUFF_200] = { '\0' };
    sprintf_safe(szCommand, MAX_BUFF_200, "b freeeyes SetDebug -s %d &", GetXmlConfigAttribute(xmlServerType)->Debug);
    Create_Command(szCommand, CONSOLE_COMMAND_SETDEBUG);
    sprintf_safe(szCommand, MAX_BUFF_200, "t freeeyes SetDebug -s %d &", GetXmlConfigAttribute(xmlServerType)->Debug);
    Create_Command(szCommand, CONSOLE_COMMAND_SETDEBUG);
}

void CUnit_ConsoleMessage::Test_DoMessage_ShowDebug(void)
{
    Create_Command("b freeeyes ShowDebug -a&", CONSOLE_COMMAND_SHOWDEBUG);
    Create_Command("t freeeyes ShowDebug -a&", CONSOLE_COMMAND_SHOWDEBUG);
}

void CUnit_ConsoleMessage::Test_DoMessage_GetTrackCommand(void)
{
    Create_Command("b freeeyes GetTrackCommandInfo -a&", CONSOLE_COMMAND_GETTRACKCOMMAND);
    Create_Command("t freeeyes GetTrackCommandInfo -a&", CONSOLE_COMMAND_GETTRACKCOMMAND);
}

void CUnit_ConsoleMessage::Test_DoMessage_GetConnectIPInfo(void)
{
    Create_Command("b freeeyes GetConnectIPInfo -s 1&", CONSOLE_COMMAND_GETCONNECTIPINFO);
    Create_Command("t freeeyes GetConnectIPInfo -s 1&", CONSOLE_COMMAND_GETCONNECTIPINFO);
}

void CUnit_ConsoleMessage::Test_DoMessage_GetLogLevelInfo(void)
{
    Create_Command("b freeeyes GetLogInfo -a&", CONSOLE_COMMAND_GETLOGINFO);
    Create_Command("t freeeyes GetLogInfo -a&", CONSOLE_COMMAND_GETLOGINFO);
}

void CUnit_ConsoleMessage::Test_DoMessage_GetThreadAI(void)
{
    Create_Command("b freeeyes GetWorkThreadAI -a&", CONSOLE_COMMAND_GETWTAI);
    Create_Command("t freeeyes GetWorkThreadAI -a&", CONSOLE_COMMAND_GETWTAI);
}

void CUnit_ConsoleMessage::Test_DoMessage_GetWorkThreadTO(void)
{
    Create_Command("b freeeyes GetWorkThreadTO -a&", CONSOLE_COMMAND_GETWTTIMEOUT);
    Create_Command("t freeeyes GetWorkThreadTO -a&", CONSOLE_COMMAND_GETWTTIMEOUT);
}

void CUnit_ConsoleMessage::Test_DoMessage_GetNickNameInfo(void)
{
    Create_Command("b freeeyes GetNickNameInfo -n 127.0.0.1&", CONSOLE_COMMAND_GETNICKNAMEINFO);
    Create_Command("t freeeyes GetNickNameInfo -n 127.0.0.1&", CONSOLE_COMMAND_GETNICKNAMEINFO);
}

void CUnit_ConsoleMessage::Test_DoMessage_ShowListen(void)
{
    Create_Command("b freeeyes ShowListen -a&", CONSOLE_COMMAND_SHOW_LISTEN);
    Create_Command("t freeeyes ShowListen -a&", CONSOLE_COMMAND_SHOW_LISTEN);
}

void CUnit_ConsoleMessage::Test_DoMessage_MonitorInfo(void)
{
    Create_Command("b freeeyes Monitor -a&", CONSOLE_COMMAND_MONITOR_INFO);
    Create_Command("t freeeyes Monitor -a&", CONSOLE_COMMAND_MONITOR_INFO);
}

void CUnit_ConsoleMessage::Test_DoMessage_PortList(void)
{
    Create_Command("b freeeyes PortFlow -a&", CONSOLE_COMMAND_PORT_FLOW);
    Create_Command("t freeeyes PortFlow -a&", CONSOLE_COMMAND_PORT_FLOW);
}

void CUnit_ConsoleMessage::Test_Do_Message_BuffPacket(void)
{
    Create_Command("b freeeyes BuffPacketState -a&", CONSOLE_COMMAND_PACKET_STATE);
    Create_Command("t freeeyes BuffPacketState -a&", CONSOLE_COMMAND_PACKET_STATE);
}

void CUnit_ConsoleMessage::Test_Do_Message_LoadModule(void)
{
    //测试插件卸载和添加
    Create_Command("b freeeyes UnLoadModule TCPTest&", CONSOLE_COMMAND_UNLOADMOUDLE);

    ACE_Time_Value tvSleep(0, 10000);
    ACE_OS::sleep(tvSleep);

    Create_Command("b freeeyes LoadModule ./,libTcpTest.so,&", CONSOLE_COMMAND_LOADMOUDLE);

    ACE_OS::sleep(tvSleep);

    //测试文本输出
    Create_Command("t freeeyes UnLoadModule TCPTest&", CONSOLE_COMMAND_UNLOADMOUDLE);

    ACE_OS::sleep(tvSleep);

    Create_Command("t freeeyes LoadModule ./,libTcpTest.so,&", CONSOLE_COMMAND_LOADMOUDLE);

    ACE_OS::sleep(tvSleep);
}

void CUnit_ConsoleMessage::Test_Do_Error_Command(void)
{
    Create_Command_Error("b xxxxx test");
}

void CUnit_ConsoleMessage::Test_Check_Console_Ip(void)
{
    bool blRet = false;
    blRet = check_console_ip("127.0.0.1");

    if (false == blRet)
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_Check_Console_Ip]check_console_ip() false.", true == blRet);
    }
    m_nTestCount++;
}

void CUnit_ConsoleMessage::Test_Do_Message_CloseClient(void)
{
    Create_Command("b freeeyes CloseClient 1&", CONSOLE_COMMAND_COLSECLIENT);
    Create_Command("t freeeyes CloseClient 1&", CONSOLE_COMMAND_COLSECLIENT);
}

void CUnit_ConsoleMessage::Test_Do_Message_UDPClientInfo(void)
{
    Create_Command("b freeeyes UDPConnectClient -a&", CONSOLE_COMMAND_UDPCONNECTINFO);
    Create_Command("t freeeyes UDPConnectClient -a&", CONSOLE_COMMAND_UDPCONNECTINFO);
}

void CUnit_ConsoleMessage::Test_Do_Message_SetTrackIP(void)
{
    Create_Command("b freeeyes SetTrackIP -i 127.0.0.1 -c 5 &", CONSOLE_COMMAND_SETTRACKIP);
    Create_Command("t freeeyes SetTrackIP -i 127.0.0.1 -c 5 &", CONSOLE_COMMAND_SETTRACKIP);
}

void CUnit_ConsoleMessage::Test_Do_Message_SetLogLevelInfo(void)
{
    Create_Command("b freeeyes SetLogLevel -l 1&", CONSOLE_COMMAND_SETLOGLEVEL);
    Create_Command("t freeeyes SetLogLevel -l 1&", CONSOLE_COMMAND_SETLOGLEVEL);
}

void CUnit_ConsoleMessage::Test_Do_Message_SetWorkThreadAI(void)
{
    Create_Command("b freeeyes SetWorkThreadAI -i 1,30,30,30&", CONSOLE_COMMAND_SETWTAI);
    Create_Command("t freeeyes SetWorkThreadAI -i 1,30,30,30&", CONSOLE_COMMAND_SETWTAI);
}

void CUnit_ConsoleMessage::Test_Do_Message_SetMaxConnectCount(void)
{
    Create_Command("b freeeyes SetMaxConnectCount -n 10000 &", CONSOLE_COMMAND_SETMAXCONNECTCOUNT);
    Create_Command("t freeeyes SetMaxConnectCount -n 10000 &", CONSOLE_COMMAND_SETMAXCONNECTCOUNT);
}

void CUnit_ConsoleMessage::Test_Do_Message_SetConnectLog(void)
{
    Create_Command("b freeeyes SetConnectLog -n 1 -f 0 &", CONSOLE_COMMAND_SETCONNECTLOG);
    Create_Command("t freeeyes SetConnectLog -n 1 -f 0 &", CONSOLE_COMMAND_SETCONNECTLOG);
}

void CUnit_ConsoleMessage::Test_Do_Message_AddListen(void)
{
    Create_Command("b freeeyes AddListen -i 127.0.0.1 -p 10071 -t 1 -n 1&", CONSOLE_COMMAND_ADD_LISTEN);
    Create_Command("t freeeyes AddListen -i 127.0.0.1 -p 10071 -t 1 -n 1&", CONSOLE_COMMAND_ADD_LISTEN);
}

void CUnit_ConsoleMessage::Test_Do_Message_DelListen(void)
{
    Create_Command("b freeeyes DelListen -i 127.0.0.1 -p 10071 -t 1 &", CONSOLE_COMMAND_DEL_LISTEN);
    Create_Command("t freeeyes DelListen -i 127.0.0.1 -p 10071 -t 1 &", CONSOLE_COMMAND_DEL_LISTEN);
}

void CUnit_ConsoleMessage::Test_Do_Message_TestFileStart(void)
{
    Create_Command("b freeeyes TestFileStart -f FileTestCfg.xml&", CONSOLE_COMMAND_FILE_TEST_START);
    Create_Command("t freeeyes TestFileStart -f FileTestCfg.xml&", CONSOLE_COMMAND_FILE_TEST_START);
}

void CUnit_ConsoleMessage::Test_Do_Message_TestFileStop(void)
{
    Create_Command("b freeeyes TestFileStop -a&", CONSOLE_COMMAND_FILE_TEST_STOP);
    Create_Command("t freeeyes TestFileStop -a&", CONSOLE_COMMAND_FILE_TEST_STOP);
}

void CUnit_ConsoleMessage::Test_Do_Message_PoolSet(void)
{
    Create_Command("b freeeyes PoolSet -n BuffPacket -b false&", CONSOLE_COMMAND_POOL_SET);
    Create_Command("b freeeyes PoolSet -n PacketParse -b false&", CONSOLE_COMMAND_POOL_SET);
}

void CUnit_ConsoleMessage::Test_Do_Message_ReLoadModule(void)
{
    //测试插件卸载和添加
    Create_Command("b freeeyes ReLoadModule TCPTest&", CONSOLE_COMMAND_RELOADMOUDLE);

    ACE_Time_Value tvSleep(0, 10000);
    ACE_OS::sleep(tvSleep);

    //测试文本输出
    Create_Command("t freeeyes ReLoadModule TCPTest&", CONSOLE_COMMAND_RELOADMOUDLE);

    ACE_OS::sleep(tvSleep);
}

#endif

