#include "ConsoleMessage.h"

CConsoleMessage::CConsoleMessage()
{
}

int CConsoleMessage::Init()
{
    m_objConsolePromissions.Init(CONSOLECONFIG);

    SetConsoleKey(GetXmlConfigAttribute(xmlConsoleKeys)->vec);

    //初始化支持命令数组
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_LOADMODULE, DoMessage_LoadModule);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_UNLOADMODULE, DoMessage_UnLoadModule);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_RELOADMODULE, DoMessage_ReLoadModule);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_SHOWMOUDLE, DoMessage_ShowModule);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_CLIENTCOUNT, DoMessage_ClientMessageCount);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_COMMANDINFO, DoMessage_CommandInfo);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_THREADINFO, DoMessage_WorkThreadState);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_CLIENTINFO, DoMessage_ClientInfo);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_FORBIDDENIP, DoMessage_ForbiddenIP);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_FORBIDDENIPSHOW, DoMessage_ShowForbiddenList);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_LIFTED, DoMessage_LifedIP);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_COLSECLIENT, DoMessage_CloseClient);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_UDPCONNECTINFO, DoMessage_UDPClientInfo);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_SERVERCONNECT_TCP, DoMessage_ServerConnectTCP);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_SERVERCONNECT_UDP, DoMessage_ServerConnectUDP);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_PROCESSINFO, DoMessage_ShowProcessInfo);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_CLIENTHISTORY, DoMessage_ShowClientHisTory);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_ALLCOMMANDINFO, DoMessage_ShowAllCommandInfo);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_SERVERINFO, DoMessage_ShowServerInfo);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_SERVERRECONNECT, DoMessage_ReConnectServer);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_COMMANDTIMEOUT, DoMessage_CommandTimeout);
    m_objHashMessageLogicList.bind(CONSOLEMESSAHE_COMMANDTIMEOUTCLR, DoMessage_CommandTimeoutclr);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_COMMANDDATALOG, DoMessage_CommandDataLog);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_SETDEBUG, DoMessage_SetDebug);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_SHOWDEBUG, DoMessage_ShowDebug);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_SETTRACKIP, DoMessage_SetTrackIP);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_SETTRACECOMMAND, DoMessage_SetTraceCommand);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_GETTRACKIPINFO, DoMessage_GetTrackCommand);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_GETCONNECTIPINFO, DoMessage_GetConnectIPInfo);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_GETLOGINFO, DoMessage_GetLogLevelInfo);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_SETLOGLEVEL, DoMessage_SetLogLevelInfo);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_GETWTAI, DoMessage_GetThreadAI);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_GETWTTIMEOUT, DoMessage_GetWorkThreadTO);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_SETWTAI, DoMessage_SetWorkThreadAI);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_GETNICKNAMEINFO, DoMessage_GetNickNameInfo);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_SETCONNECTLOG, DoMessage_SetConnectLog);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_SETMAXCONNECTCOUNT, DoMessage_SetMaxConnectCount);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_ADD_LISTEN, DoMessage_AddListen);
    m_objHashMessageLogicList.bind(CONSOLEMESSAGE_DEL_LISTEN, DoMessage_DelListen);
    m_objHashMessageLogicList.bind(CONSOLEMESSATE_SHOW_LISTEN, DoMessage_ShowListen);
    m_objHashMessageLogicList.bind(CONSOLEMESSATE_MONITOR_INFO, DoMessage_MonitorInfo);
    m_objHashMessageLogicList.bind(CONSOLEMESSATE_FILE_TEST_START, DoMessage_TestFileStart);
    m_objHashMessageLogicList.bind(CONSOLEMESSATE_FILE_TEST_STOP, DoMessage_TestFileStop);
    m_objHashMessageLogicList.bind(CONSOLEMESSATE_PORT_FLOW, DoMessage_PortList);
    m_objHashMessageLogicList.bind(CONSOLEMESSATE_PACKET_STATE, Do_Message_BuffPacket);
    m_objHashMessageLogicList.bind(CONSOLEMESSATE_POOL_SET, Do_Message_PoolSet);

    return 0;
}

int CConsoleMessage::Dispose(ACE_Message_Block* pmb, IBuffPacket* pBuffPacket, uint8& u1OutputType)
{
    //处理命令
    if(NULL == pmb)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::Dispose]pmb is NULL.\n"));
        return CONSOLE_MESSAGE_FAIL;
    }

    char* pCommand = new char[(uint32)pmb->length() + 1];

    pCommand[(uint32)pmb->length() - 1] = '\0';

    memcpy_safe((char* )pmb->rd_ptr(), (uint32)pmb->length(), pCommand, (uint32)pmb->length());

    //去除数据尾部的终止符
    if (pCommand[pmb->length() - 1] == '&')
    {
        pCommand[pmb->length() - 1] = '\0';
    }
    else
    {
        pCommand[pmb->length() - 3] = '\0';
    }

    //解析命令，把数据切割出来
    if(CONSOLE_MESSAGE_SUCCESS != ParseCommand(pCommand, pBuffPacket, u1OutputType))
    {
        SAFE_DELETE_ARRAY(pCommand);
        return CONSOLE_MESSAGE_FAIL;
    }
    else
    {
        SAFE_DELETE_ARRAY(pCommand);
        return CONSOLE_MESSAGE_SUCCESS;
    }
}

int CConsoleMessage::ParsePlugInCommand(const char* pCommand, IBuffPacket* pBuffPacket)
{
    uint8 u1OutputType = 0;

    //拼接插件调用完整指令
    char szPluginCommand[MAX_BUFF_200] = { '\0' };
    sprintf_safe(szPluginCommand, MAX_BUFF_200, "b plugin %s", pCommand);
    return ParseCommand_Plugin(szPluginCommand, pBuffPacket, u1OutputType);
}

bool CConsoleMessage::GetCommandInfo(const char* pCommand, _CommandInfo& CommandInfo, bool blCheck)
{
    int nLen = (int)ACE_OS::strlen(pCommand);
    char szKey[MAX_BUFF_100] = {'\0'};

    AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONSOLEDATA, "<Command>%s.", pCommand);

    if(nLen > MAX_BUFF_100*2 + 1)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetCommandInfo]pCommand is too long.\n"));
        return false;
    }

    //获得输出模式
    char szOutputType[MAX_BUFF_100] = { '\0' };
    char* pKeyBegin = ACE_OS::strstr((char*)pCommand, COMMAND_SPLIT_STRING);

    if (NULL == pKeyBegin)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetCommandInfo]OutputType is no find.\n"));
        return false;
    }

    memcpy_safe((char*)pCommand, (int)(pKeyBegin - pCommand), szOutputType, (int)(pKeyBegin - pCommand), true);

    if (ACE_OS::strcmp(szOutputType, "b") == 0)
    {
        CommandInfo.m_u1OutputType = 0;
    }
    else
    {
        CommandInfo.m_u1OutputType = 1;
    }

    //获得key值
    char* pCommandBegin = ACE_OS::strstr((char*)pKeyBegin + ACE_OS::strlen(COMMAND_SPLIT_STRING), COMMAND_SPLIT_STRING);

    if (NULL == pCommandBegin)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetCommandInfo]CommandBegin is no find.\n"));
        return false;
    }

    memcpy_safe((char* )pKeyBegin + ACE_OS::strlen(COMMAND_SPLIT_STRING),
                (int)(pCommandBegin - pKeyBegin - ACE_OS::strlen(COMMAND_SPLIT_STRING)),
                szKey,
                MAX_BUFF_100,
                true);

    memcpy_safe(szKey, (uint32)ACE_OS::strlen(szKey), CommandInfo.m_szUser, MAX_BUFF_50, true);

    if (true == blCheck && false == CheckConsoleKey(szKey))
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetCommandInfo]szKey is invalid.\n"));
        return false;
    }

    //获得命令头
    char* pParamBegin = ACE_OS::strstr((char*)pCommandBegin + ACE_OS::strlen(COMMAND_SPLIT_STRING), COMMAND_SPLIT_STRING);

    if (NULL == pParamBegin)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetCommandInfo]ParamBegin is no find.\n"));
        return false;
    }

    uint32 u4Data4Len = 0;
    u4Data4Len = (uint32)(pParamBegin - pCommandBegin - ACE_OS::strlen(COMMAND_SPLIT_STRING));
    memcpy_safe(pCommandBegin + ACE_OS::strlen(COMMAND_SPLIT_STRING), u4Data4Len, (char*)CommandInfo.m_szCommandTitle, (uint32)(pParamBegin - pCommandBegin - (int)ACE_OS::strlen(COMMAND_SPLIT_STRING)));
    CommandInfo.m_szCommandTitle[u4Data4Len] = '\0';

    //获得扩展参数
    u4Data4Len = (uint32)(nLen - (pParamBegin - pCommand - ACE_OS::strlen(COMMAND_SPLIT_STRING)) + 1);
    memcpy_safe(pParamBegin + ACE_OS::strlen(COMMAND_SPLIT_STRING), u4Data4Len, (char*)CommandInfo.m_szCommandExp, u4Data4Len);
    CommandInfo.m_szCommandExp[u4Data4Len] = '\0';

    return true;
}

int CConsoleMessage::ParseCommand_Plugin(const char* pCommand, IBuffPacket* pBuffPacket, uint8& u1OutputType)
{
    _CommandInfo CommandInfo;

    IBuffPacket* pCurrBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    if (NULL == pCurrBuffPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::ParseCommand]pCurrBuffPacket is NULL.\n"));
        return CONSOLE_MESSAGE_FAIL;
    }

    if (false == GetCommandInfo(pCommand, CommandInfo, false))
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::ParseCommand]pCommand format is error.\n"));
        return CONSOLE_MESSAGE_FAIL;
    }

    u1OutputType = CommandInfo.m_u1OutputType;

    //执行命令
    return DoCommand(CommandInfo, pCurrBuffPacket, pBuffPacket);
}

int CConsoleMessage::ParseCommand(const char* pCommand, IBuffPacket* pBuffPacket, uint8& u1OutputType)
{
    _CommandInfo CommandInfo;

    IBuffPacket* pCurrBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    if(NULL == pCurrBuffPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::ParseCommand]pCurrBuffPacket is NULL.\n"));
        return CONSOLE_MESSAGE_FAIL;
    }

    if(false == GetCommandInfo(pCommand, CommandInfo))
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::ParseCommand]pCommand format is error.\n"));
        App_BuffPacketManager::instance()->Delete(pCurrBuffPacket);
        return CONSOLE_MESSAGE_FAIL;
    }

    //判断当前命令是否可以执行
    int nPromission = m_objConsolePromissions.Check_Promission(CommandInfo.m_szCommandTitle, CommandInfo.m_szUser);

    if (0 != nPromission)
    {
        App_BuffPacketManager::instance()->Delete(pCurrBuffPacket);
        return CONSOLE_MESSAGE_FAIL;
    }

    u1OutputType = CommandInfo.m_u1OutputType;

    //执行命令
    return DoCommand(CommandInfo, pCurrBuffPacket, pBuffPacket);
}

int CConsoleMessage::DoCommand(_CommandInfo& CommandInfo, IBuffPacket* pCurrBuffPacket, IBuffPacket* pReturnBuffPacket)
{
    uint16 u2ReturnCommandID = CONSOLE_COMMAND_UNKNOW;

    if (NULL == pCurrBuffPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::ParseCommand]pCurrBuffPacket is NULL.\n"));
        return CONSOLE_MESSAGE_FAIL;
    }

    //查找并处理指令信息
    DoMessage_Logic fn_DoMessage_Logic = NULL;
    m_objHashMessageLogicList.find((string)CommandInfo.m_szCommandTitle, fn_DoMessage_Logic);

    if (NULL != fn_DoMessage_Logic)
    {
        //找到了，执行指令
        fn_DoMessage_Logic(CommandInfo, pCurrBuffPacket, u2ReturnCommandID);
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleMessage::ParseCommand]Command is no Find.\n"));
    }

    //拼接返回数据包内容
    uint32 u4PacketSize = pCurrBuffPacket->GetPacketLen();

    if (u4PacketSize == 0 || CONSOLE_COMMAND_UNKNOW == u2ReturnCommandID)
    {
        App_BuffPacketManager::instance()->Delete(pCurrBuffPacket);
        return CONSOLE_MESSAGE_FAIL;
    }

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pReturnBuffPacket) << u2ReturnCommandID;
        pReturnBuffPacket->WriteStream(pCurrBuffPacket->GetData(), pCurrBuffPacket->GetPacketLen());
        App_BuffPacketManager::instance()->Delete(pCurrBuffPacket);
    }
    else
    {
        pReturnBuffPacket->WriteStream(pCurrBuffPacket->GetData(), pCurrBuffPacket->GetPacketLen());
        App_BuffPacketManager::instance()->Delete(pCurrBuffPacket);
    }

    return CONSOLE_MESSAGE_SUCCESS;
}

bool CConsoleMessage::SetConsoleKey(vector<xmlConsoleKeys::_ConsoleKey> vecConsoleKeyList)
{
    m_vecConsolekeyList.swap(vecConsoleKeyList);
    return true;
}

bool CConsoleMessage::CheckConsoleKey(const char* pKey)
{
    for (int i = 0; i < (int)m_vecConsolekeyList.size(); i++)
    {
        if (ACE_OS::strcmp(m_vecConsolekeyList[i].Key.c_str(), pKey) == 0)
        {
            //key值对上了
            return true;
        }
    }

    return false;
}
