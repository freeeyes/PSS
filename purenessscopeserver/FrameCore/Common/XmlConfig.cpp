#include "XmlConfig.h"
#include "define.h"
#include "IpCheck.h"

/*����Ԫ�ض�Ӧ��Ԫ��λ���Ѿ��ڹ���ʱ��ö��ȷ��*/
IConfigOpeation* IConfigOpeation::_array[XML_Config_MAX];


/*���徲̬����󲢰󶨶�Ӧö�٣�ʵ�ַ���ģ�庯��*/

#define CLASSNAME(ClassName) (this##ClassName)

#define DefineClassAndFunc(ClassName, XMLConfig)                            \
    static ClassName    CLASSNAME(ClassName)(XMLConfig, #ClassName);      \
    template<>                                                                  \
    ClassName* XMainConfig::GetXmlConfig<ClassName>()                           \
    {                                                                           \
        return dynamic_cast<ClassName*>(IConfigOpeation::_array[static_cast<int>(XMLConfig)]);    \
    }


/*xml�����ļ���Ӧ�������õľ�̬�࣬��Ҫ���������ļ���ǩ����Ҫ�ڴ���Ӿ�̬��������ж���ֱ��ʹ��,Ҳ�������ⲿʹ��*/
/*****************�����ͷ��غ���һһ��Ӧ*********************/
DefineClassAndFunc(xmlRecvInfo, XmlConfig::XML_Config_RecvInfo)
DefineClassAndFunc(xmlSendInfo, XmlConfig::XML_Config_SendInfo)
DefineClassAndFunc(xmlNetWorkMode, XmlConfig::XML_Config_NetWorkMode)
DefineClassAndFunc(xmlTCPServerIPs, XmlConfig::XML_Config_TCPServerIPs)
DefineClassAndFunc(xmlUDPServerIPs, XmlConfig::XML_Config_UDPServerIPs)
DefineClassAndFunc(xmlTTyDrives, XmlConfig::XML_Config_TTyDrives)
DefineClassAndFunc(xmlServer2Server, XmlConfig::XML_Config_Server2Servers)
DefineClassAndFunc(xmlConnectServer, XmlConfig::XML_Config_ConnectServer)
DefineClassAndFunc(xmlClientInfo, XmlConfig::XML_Config_ClientInfo)
DefineClassAndFunc(xmlTTyClientManagerInfo, XmlConfig::XML_Config_TTyClientManager)
DefineClassAndFunc(xmlModuleInfos, XmlConfig::XML_Config_ModuleInfos)
DefineClassAndFunc(xmlModuleMangager, XmlConfig::XML_Config_ModuleMangager)
DefineClassAndFunc(xmlMonitor, XmlConfig::XML_Config_Monitor)
DefineClassAndFunc(xmlThreadInfoAI, XmlConfig::XML_Config_ThreadInfoAI)
DefineClassAndFunc(xmlThreadInfo, XmlConfig::XML_Config_ThreadInfo)
DefineClassAndFunc(xmlConsole, XmlConfig::XML_Config_Console)
DefineClassAndFunc(xmlConsoleKeys, XmlConfig::XML_Config_ConsoleKeys)
DefineClassAndFunc(xmlConsoleClients, XmlConfig::XML_Config_ConsoleClients)
DefineClassAndFunc(xmlAceDebug, XmlConfig::XML_Config_AceDebug)
DefineClassAndFunc(xmlCommandAccount, XmlConfig::XML_Config_CommandAccount)
DefineClassAndFunc(xmlCoreSetting, XmlConfig::XML_Config_CoreSetting)
DefineClassAndFunc(xmlServerType, XmlConfig::XML_Config_ServerType)
DefineClassAndFunc(xmlServerID, XmlConfig::XML_Config_ServerID)
DefineClassAndFunc(xmlServerName, XmlConfig::XML_Config_ServerName)
DefineClassAndFunc(xmlServerVersion, XmlConfig::XML_Config_ServerVersion)
DefineClassAndFunc(xmlPacketParses, XmlConfig::XML_Config_PacketParses)
DefineClassAndFunc(xmlBuffPacket, XmlConfig::XML_Config_BuffPacket)
DefineClassAndFunc(xmlMessage, XmlConfig::XML_Config_Message)
DefineClassAndFunc(xmlTcpRedirection, XmlConfig::XML_Config_Redirection)
DefineClassAndFunc(xmlAlertConnect, XmlConfig::XML_Config_AlertConnect)
DefineClassAndFunc(xmlIP, XmlConfig::XML_Config_IP)
DefineClassAndFunc(xmlClientData, XmlConfig::XML_Config_ClientData)
DefineClassAndFunc(xmlCommandInfos, XmlConfig::XML_Config_CommandInfos)
DefineClassAndFunc(xmlMails, XmlConfig::XML_Config_Mails)
DefineClassAndFunc(xmlCommandsTimeout, XmlConfig::XML_Config_Commands_Timeout)

bool XMainConfig::Init()
{
    //��ʼ��xml�ļ�
    return InitFile(MAINCONFIG, XmlConfig::XML_Config_RecvInfo, XmlConfig::XML_Config_Redirection)
           && InitFile(ALERTCONFIG, XmlConfig::XML_Config_AlertConnect, XmlConfig::XML_Config_Commands_Timeout);
}

bool XMainConfig::InitFile(const char* pFileName, XmlConfig start, XmlConfig end)
{
    bool bKet = true;

    if (m_XmlOperation.Init(pFileName))
    {
        /*
        ע��forѭ��Ϊ2���ж�����
        1.��Χ
        2.init�������ؽ��
        */
        int i = static_cast<int>(start);

        for (; i <= static_cast<int>(end) && bKet; ++i)
        {
            bKet = IConfigOpeation::_array[i]->Init(&m_XmlOperation);
        }

        if (false == bKet)
        {
            OUR_DEBUG((LM_INFO, "[XMainConfig::InitFile](%s) Init is false.\n",
                       IConfigOpeation::_array[i]->ClassName().c_str()));
        }
    }
    else
    {
        bKet = false;
        OUR_DEBUG((LM_INFO, "[XMainConfig::Init]File Read Error = %s.\n", pFileName));
    }

    return bKet;
}

/**********************************��ӦXmlConfig����Եĳ�ʼ����Ϊ***************************************/

bool xmlRecvInfo::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint16("RecvInfo", "RecvQueueTimeout", RecvQueueTimeout)
           && pXmlOperation->Read_XML_Data_Single_Uint32("RecvInfo", "RecvBuffSize", RecvBuffSize);
}

bool xmlSendInfo::Init(CXmlOpeation* pXmlOperation)
{
    bool bKet = false;

    if (pXmlOperation->Read_XML_Data_Single_Uint16("SendInfo", "SendTimeout", SendTimeout)
        && pXmlOperation->Read_XML_Data_Single_Uint16("SendInfo", "SendQueueMax", SendQueueMax)
        && pXmlOperation->Read_XML_Data_Single_Uint16("SendInfo", "PutQueueTimeout", PutQueueTimeout)
        && pXmlOperation->Read_XML_Data_Single_Uint32("SendInfo", "BlockCount", BlockCount)
        && pXmlOperation->Read_XML_Data_Single_Uint32("SendInfo", "MaxBlockSize", MaxBlockSize))
    {
        SendDatamark = MaxBlockSize;
        bKet = pXmlOperation->Read_XML_Data_Single_Uint16("SendInfo", "TcpNodelay", TcpNodelay);
    }

    return bKet;
}

bool xmlNetWorkMode::Init(CXmlOpeation* pXmlOperation)
{
    bool bKet = false;
    std::string strMode;
    std::string strNetByteOrder;

    if (pXmlOperation->Read_XML_Data_Single_String("NetWorkMode", "Mode", strMode)
        && pXmlOperation->Read_XML_Data_Single_Uint16("NetWorkMode", "ThreadCount", ThreadCount)
        && pXmlOperation->Read_XML_Data_Single_Uint16("NetWorkMode", "BackLog", BackLog)
        && pXmlOperation->Read_XML_Data_Single_String("NetWorkMode", "ByteOrder", strNetByteOrder))
    {
        SetNetByteOrder(strNetByteOrder);
        SetLocalByteOrder();
        bKet = SetIOMode(strMode);
    }

    return bKet;
}

bool xmlNetWorkMode::SetIOMode(const std::string& pData)
{
    bool bKet = true;

    if (pData.compare("Iocp") == 0)
    {
        Mode = NETWORKMODE::NETWORKMODE_PRO_IOCP;
    }
    else if (pData.compare("Select") == 0)
    {
        Mode = NETWORKMODE::NETWORKMODE_RE_SELECT;
    }
    else if (pData.compare("Poll") == 0)
    {
        Mode = NETWORKMODE::NETWORKMODE_RE_TPSELECT;
    }
    else if (pData.compare("Epoll") == 0)
    {
        Mode = NETWORKMODE::NETWORKMODE_RE_EPOLL;
    }
    else if (pData.compare("Epoll_et") == 0)
    {
        Mode = NETWORKMODE::NETWORKMODE_RE_EPOLL_ET;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[XMainConfig::SetIOMode]NetworkMode is Invalid!!, please read main.xml desc.\n"));
        bKet = false;
    }

    return bKet;
}

void xmlNetWorkMode::SetLocalByteOrder()
{
    //�ж��ֽ���
    if (O32_HOST_ORDER == O32_LITTLE_ENDIAN)
    {
        LocalByteOrder = ENUM_CHAR_ORDER::SYSTEM_LITTLE_ORDER;
        OUR_DEBUG((LM_INFO, "[XMainConfig::SetLocalByteOrder]SYSYTEM SYSTEM_LITTLE_ORDER.\n"));
    }
    else
    {
        LocalByteOrder = ENUM_CHAR_ORDER::SYSTEM_BIG_ORDER;
        OUR_DEBUG((LM_INFO, "[XMainConfig::SetLocalByteOrder]SYSYTEM SYSTEM_BIG_ORDER.\n"));
    }
}

void xmlNetWorkMode::SetNetByteOrder(const std::string& pData)
{
    if (pData.compare("NET_ORDER") == 0)
    {
        NetByteOrder = true;
    }
    else
    {
        NetByteOrder = false;
    }
}


bool xmlTCPServerIPs::Init(CXmlOpeation* pXmlOperation)
{
    bool bKet = true;
    TiXmlElement* pIP = NULL;
    TiXmlElement* pPort = NULL;
    TiXmlElement* pPacketParseID = NULL;

    _TCPServerIP tcpServerIP;

    while (bKet
           && pXmlOperation->Read_XML_Data_Multiple_String("TCPServerIP", "ip", tcpServerIP.ip, pIP)
           && pXmlOperation->Read_XML_Data_Multiple_Uint16("TCPServerIP", "port", tcpServerIP.port, pPort)
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("TCPServerIP", "ParseID", tcpServerIP.packetparseid, pPacketParseID))
    {
        bKet = Check_IPType(tcpServerIP.ip, tcpServerIP.ipType);
        vec.push_back(tcpServerIP);
    }

    return bKet;
}

bool xmlUDPServerIPs::Init(CXmlOpeation* pXmlOperation)
{
    bool bKet = true;
    TiXmlElement* pUIP = NULL;
    TiXmlElement* pUPort = NULL;
    TiXmlElement* pUMaxRecvSize = NULL;
    TiXmlElement* pUPacketParseID = NULL;

    _UDPServerIP udpServerIP;

    while (bKet
           && pXmlOperation->Read_XML_Data_Multiple_String("UDPServerIP", "uip", udpServerIP.uip, pUIP)
           && pXmlOperation->Read_XML_Data_Multiple_Uint16("UDPServerIP", "uport", udpServerIP.uport, pUPort)
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("UDPServerIP", "uMaxRecvSize", udpServerIP.uMaxRecvSize, pUMaxRecvSize)
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("UDPServerIP", "uParseID", udpServerIP.uPacketParseID, pUPacketParseID))
    {
        bKet = Check_IPType(udpServerIP.uip, udpServerIP.uipType);
        vec.push_back(udpServerIP);
    }

    return bKet;
}

bool xmlTTyDrives::Init(CXmlOpeation* pXmlOperation)
{
    bool bKet = true;
    TiXmlElement* pTTyID          = NULL;
    TiXmlElement* pParseID        = NULL;
    TiXmlElement* pBaud           = NULL;
    TiXmlElement* pDataBits       = NULL;
    TiXmlElement* pStopBits       = NULL;
    TiXmlElement* pParity         = NULL;
    TiXmlElement* pPortName       = NULL;
    TiXmlElement* pPortDesc       = NULL;

    _TTyDrives TTyDrives;

    while (bKet
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("TTy", "ComID", TTyDrives.u4TTyID, pTTyID)
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("TTy", "TTyParseID", TTyDrives.u4PacketParseID, pParseID)
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("TTy", "Baud", TTyDrives.u4Baud, pBaud)
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("TTy", "DataBits", TTyDrives.u4DataBits, pDataBits)
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("TTy", "StopBits", TTyDrives.u4StopBits, pStopBits)
           && pXmlOperation->Read_XML_Data_Multiple_String("TTy", "Parity", TTyDrives.strParity, pParity)
           && pXmlOperation->Read_XML_Data_Multiple_String("TTy", "PortName", TTyDrives.strPortName, pPortName)
           && pXmlOperation->Read_XML_Data_Multiple_String("TTy", "PortDes", TTyDrives.strDesc, pPortDesc))
    {
        vec.push_back(TTyDrives);
    }

    return bKet;
}

bool xmlServer2Server::Init(CXmlOpeation* pXmlOperation)
{
    bool bKet = true;
    TiXmlElement* pServerID      = NULL;
    TiXmlElement* pServerIP      = NULL;
    TiXmlElement* pServerPort    = NULL;
    TiXmlElement* pParketParseID = NULL;

    _Server2Server Server2Server;

    while (bKet
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("ServerToServer", "S2SID", Server2Server.u4ServerID, pServerID)
           && pXmlOperation->Read_XML_Data_Multiple_String("ServerToServer", "S2SIp", Server2Server.strServerIP, pServerIP)
           && pXmlOperation->Read_XML_Data_Multiple_Uint16("ServerToServer", "S2SPort", Server2Server.u2ServerPort, pServerPort)
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("ServerToServer", "S2SPacketParseID", Server2Server.u4PacketParseID, pParketParseID))
    {
        vec.push_back(Server2Server);
    }

    return bKet;
}

bool xmlConnectServer::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint32("ConnectServer", "TimeInterval", TimeInterval)
           && pXmlOperation->Read_XML_Data_Single_Uint32("ConnectServer", "Recvbuff", Recvbuff)
           && pXmlOperation->Read_XML_Data_Single_Uint8("ConnectServer", "RunType", RunType)
           && pXmlOperation->Read_XML_Data_Single_Uint16("ConnectServer", "TimeCheck", TimeCheck)
           && pXmlOperation->Read_XML_Data_Single_Uint16("ConnectServer", "Count", Count);
}

bool xmlClientInfo::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint32("ClientInfo", "HandlerCount", HandlerCount)
           && pXmlOperation->Read_XML_Data_Single_Uint32("ClientInfo", "MaxHandlerCount", MaxHandlerCount)
           && pXmlOperation->Read_XML_Data_Single_Uint16("ClientInfo", "MaxConnectTime", MaxConnectTime)
           && pXmlOperation->Read_XML_Data_Single_Uint16("ClientInfo", "CheckAliveTime", CheckAliveTime)
           && pXmlOperation->Read_XML_Data_Single_Uint32("ClientInfo", "MaxBuffRecv", MaxBuffRecv);
}

bool xmlModuleInfos::Init(CXmlOpeation* pXmlOperation)
{
    TiXmlElement* pName = NULL;
    TiXmlElement* pPath = NULL;
    TiXmlElement* pParam = NULL;

    _ModuleInfo moduleInfo;

    while (pXmlOperation->Read_XML_Data_Multiple_String("ModuleInfo", "ModuleSName", moduleInfo.szModuleName, pName)
           && pXmlOperation->Read_XML_Data_Multiple_String("ModuleInfo", "ModuleSPath", moduleInfo.szModulePath, pPath)
           && pXmlOperation->Read_XML_Data_Multiple_String("ModuleInfo", "ModuleSParam", moduleInfo.szModuleParam, pParam))
    {
        vec.push_back(moduleInfo);
    }

    return true;
}

bool xmlModuleMangager::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint16("ModuleManager", "MaxCount", MaxCount);
}

bool xmlMonitor::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint8("Monitor", "CpuAndMemory", CpuAndMemory)
           && pXmlOperation->Read_XML_Data_Single_Uint16("Monitor", "CpuMax", CpuMax)
           && pXmlOperation->Read_XML_Data_Single_Uint32("Monitor", "MemoryMax", MemoryMax);
}

bool xmlThreadInfoAI::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint8("ThreadInfoAI", "AI", AI)
           && pXmlOperation->Read_XML_Data_Single_Uint8("ThreadInfoAI", "ReturnDataType", ReturnDataType)
           && pXmlOperation->Read_XML_Data_Single_Uint16("ThreadInfoAI", "CheckTime", CheckTime)
           && pXmlOperation->Read_XML_Data_Single_Uint16("ThreadInfoAI", "TimeoutCount", TimeoutCount)
           && pXmlOperation->Read_XML_Data_Single_Uint16("ThreadInfoAI", "StopTime", StopTime)
           && pXmlOperation->Read_XML_Data_Single_String("ThreadInfoAI", "ReturnData", ReturnData);
}

bool xmlThreadInfo::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint16("ThreadInfo", "ThreadTimeout", ThreadTimeout)
           && pXmlOperation->Read_XML_Data_Single_Uint16("ThreadInfo", "ThreadTimeCheck", ThreadTimeCheck)
           && pXmlOperation->Read_XML_Data_Single_Uint16("ThreadInfo", "DisposeTimeout", DisposeTimeout)
           && pXmlOperation->Read_XML_Data_Single_Uint16("ThreadInfo", "PutQueueTimeout", PutQueueTimeout);
}

bool xmlConsole::Init(CXmlOpeation* pXmlOperation)
{
    bool bKet = true;

    if (pXmlOperation->Read_XML_Data_Single_Uint8("Console", "support", support)
        && pXmlOperation->Read_XML_Data_Single_String("Console", "sip", sip)
        && pXmlOperation->Read_XML_Data_Single_Uint16("Console", "sport", sport))
    {
        bKet = Check_IPType(sip, ipType);
    }

    return bKet;
}

bool xmlConsoleKeys::Init(CXmlOpeation* pXmlOperation)
{
    TiXmlElement* pKey = NULL;
    _ConsoleKey consolekey;

    while (pXmlOperation->Read_XML_Data_Multiple_String("ConsoleKey", "Key", consolekey.Key, pKey))
    {
        vec.push_back(consolekey);
    }

    return true;
}

bool xmlConsoleClients::Init(CXmlOpeation* pXmlOperation)
{
    TiXmlElement* pCip = NULL;
    _ConsoleClient consoleclient;

    while (pXmlOperation->Read_XML_Data_Multiple_String("ConsoleClient", "cip", consoleclient.cip, pCip))
    {
        vec.push_back(consoleclient);
    }

    return true;
}

bool xmlAceDebug::Init(CXmlOpeation* pXmlOperation)
{
    bool bKet = true;

    if (pXmlOperation->Read_XML_Data_Single_Uint8("AceDebug", "TrunOn", TrunOn)
        && pXmlOperation->Read_XML_Data_Single_String("AceDebug", "DebugPath", DebugPath)
        && pXmlOperation->Read_XML_Data_Single_String("AceDebug", "DebugName", DebugName)
        && pXmlOperation->Read_XML_Data_Single_Uint32("AceDebug", "LogFileMaxSize", LogFileMaxSize)
        && pXmlOperation->Read_XML_Data_Single_String("AceDebug", "Level", Level))
    {
        //��չ���ֽ�
        LogFileMaxSize = LogFileMaxSize * 1024;
    }

    return bKet;
}

bool xmlCommandAccount::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint8("CommandAccount", "Account", Account)
           && pXmlOperation->Read_XML_Data_Single_Uint8("CommandAccount", "FlowAccount", FlowAccount)
           && pXmlOperation->Read_XML_Data_Single_Uint32("CommandAccount", "MaxCommandCount", MaxCommandCount);
}

bool xmlCoreSetting::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint8("CoreSetting", "CoreNeed", CoreNeed)
           && pXmlOperation->Read_XML_Data_Single_String("CoreSetting", "Script", Script);
}

bool xmlServerType::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint8("ServerType", "Type", Type)
           && pXmlOperation->Read_XML_Data_Single_String("ServerType", "name", name)
           && pXmlOperation->Read_XML_Data_Single_String("ServerType", "displayname", displayname)
           && pXmlOperation->Read_XML_Data_Single_Uint8("ServerType", "Debug", Debug)
           && pXmlOperation->Read_XML_Data_Single_Uint32("ServerType", "DebugSize", DebugSize)
           && pXmlOperation->Read_XML_Data_Single_Uint8("ServerType", "IsClose", IsClose);
}

bool xmlServerID::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint32("ServerID", "id", id);
}

bool xmlServerName::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_String("ServerName", "name", name);
}

bool xmlServerVersion::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_String("ServerVersion", "Version", Version);
}

bool xmlPacketParses::Init(CXmlOpeation* pXmlOperation)
{
    TiXmlElement* pParseID = NULL;
    TiXmlElement* pPath = NULL;
    TiXmlElement* pName = NULL;
    TiXmlElement* pType = NULL;
    TiXmlElement* pOrg = NULL;

    _PacketParse packetparse;
    string Type;

    while (pXmlOperation->Read_XML_Data_Multiple_Uint16("PacketParse", "ParseID", packetparse.ParseID, pParseID)
           && pXmlOperation->Read_XML_Data_Multiple_String("PacketParse", "ModulePath", packetparse.ModulePath, pPath)
           && pXmlOperation->Read_XML_Data_Multiple_String("PacketParse", "ModuleName", packetparse.ModuleName, pName)
           && pXmlOperation->Read_XML_Data_Multiple_String("PacketParse", "Type", Type, pType)
           && pXmlOperation->Read_XML_Data_Multiple_Uint16("PacketParse", "OrgLength", packetparse.OrgLength, pOrg))
    {
        if (Type.compare("STREAM") == 0)
        {
            //��ģʽ
            packetparse.Type = (uint8)PACKET_WITHSTREAM;
        }
        else
        {
            //����ͷ��ģʽ
            packetparse.Type = (uint8)PACKET_WITHHEAD;
        }

        vec.push_back(packetparse);
    }

    return true;
}

bool xmlBuffPacket::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint32("BuffPacket", "Count", Count);
}

bool xmlMessage::Init(CXmlOpeation* pXmlOperation)
{
    bool bKet = false;

    if (pXmlOperation->Read_XML_Data_Single_Uint32("Message", "Msg_Low_mark", Msg_Low_mark)
        && pXmlOperation->Read_XML_Data_Single_Uint32("Message", "Msg_Buff_Max_Size", Msg_Buff_Max_Size)
        && pXmlOperation->Read_XML_Data_Single_Uint16("Message", "Msg_Thread", Msg_Thread)
        && pXmlOperation->Read_XML_Data_Single_Uint32("Message", "Msg_MaxQueue", Msg_MaxQueue)
        && pXmlOperation->Read_XML_Data_Single_Uint32("Message", "Msg_Pool", Msg_Pool))
    {
        pXmlOperation->Read_XML_Data_Single_Uint16("Message", "Msg_Process", Msg_Process);
        bKet = true;
    }

    return bKet;
}

bool xmlTTyClientManagerInfo::Init(CXmlOpeation* pXmlOperation)
{
    bool bKet = false;

    if (pXmlOperation->Read_XML_Data_Single_Uint16("TTYClient", "MaxDevCount", MaxTTyDevCount)
        && pXmlOperation->Read_XML_Data_Single_Uint16("TTYClient", "TimeCheck", TimeCheck))
    {
        bKet = true;
    }

    return bKet;
}


/******************************alert.xml***************************************/
bool xmlAlertConnect::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint8("AlertConnect", "ConnectMin", ConnectMin)
           && pXmlOperation->Read_XML_Data_Single_Uint32("AlertConnect", "ConnectMax", ConnectMax)
           && pXmlOperation->Read_XML_Data_Single_Uint16("AlertConnect", "DisConnectMin", DisConnectMin)
           && pXmlOperation->Read_XML_Data_Single_Uint32("AlertConnect", "DisConnectMax", DisConnectMax)
           && pXmlOperation->Read_XML_Data_Single_Uint32("AlertConnect", "ConnectAlert", ConnectAlert)
           && pXmlOperation->Read_XML_Data_Single_Uint16("AlertConnect", "MailID", MailID);
}

bool xmlIP::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint8("IP", "IPMax", IPMax)
           && pXmlOperation->Read_XML_Data_Single_Uint32("IP", "Timeout", Timeout);
}

bool xmlClientData::Init(CXmlOpeation* pXmlOperation)
{
    return pXmlOperation->Read_XML_Data_Single_Uint16("ClientData", "RecvPacketCount", RecvPacketCount)
           && pXmlOperation->Read_XML_Data_Single_Uint32("ClientData", "RecvDataMax", RecvDataMax)
           && pXmlOperation->Read_XML_Data_Single_Uint16("ClientData", "SendPacketCount", SendPacketCount)
           && pXmlOperation->Read_XML_Data_Single_Uint32("ClientData", "SendDataMax", SendDataMax)
           && pXmlOperation->Read_XML_Data_Single_Uint32("ClientData", "MailID", MailID);
}

bool xmlCommandInfos::Init(CXmlOpeation* pXmlOperation)
{
    TiXmlElement* pCommandID = NULL;
    TiXmlElement* pCommandCount = NULL;
    TiXmlElement* pMailID = NULL;
    _CommandInfo commandInfo;

    while (pXmlOperation->Read_XML_Data_Multiple_Uint16("CommandInfo", "CommandID", commandInfo.CommandID, pCommandID)
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("CommandInfo", "CommandCount", commandInfo.CommandCount, pCommandCount)
           && pXmlOperation->Read_XML_Data_Multiple_Uint16("CommandInfo", "MailID", commandInfo.MailID, pMailID))
    {
        vec.push_back(commandInfo);
    }

    return true;
}

bool xmlMails::Init(CXmlOpeation* pXmlOperation)
{
    TiXmlElement* pMailID = NULL;
    TiXmlElement* pfromMailAddr = NULL;
    TiXmlElement* ptoMailAddr = NULL;
    TiXmlElement* pMailPass = NULL;
    TiXmlElement* pMailUrl = NULL;
    TiXmlElement* pMailPort = NULL;
    _Mail mail;

    while (pXmlOperation->Read_XML_Data_Multiple_Uint16("Mail", "MailID", mail.MailID, pMailID)
           && pXmlOperation->Read_XML_Data_Multiple_String("Mail", "fromMailAddr", mail.fromMailAddr, pfromMailAddr)
           && pXmlOperation->Read_XML_Data_Multiple_String("Mail", "toMailAddr", mail.toMailAddr, ptoMailAddr)
           && pXmlOperation->Read_XML_Data_Multiple_String("Mail", "MailPass", mail.MailPass, pMailPass)
           && pXmlOperation->Read_XML_Data_Multiple_String("Mail", "MailUrl", mail.MailUrl, pMailUrl)
           && pXmlOperation->Read_XML_Data_Multiple_Uint16("Mail", "MailPort", mail.MailPort, pMailPort))
    {
        vec.push_back(mail);
    }

    return true;
}

xmlMails::_Mail* xmlMails::GetMailAlert(uint16 MailID)
{
    for(_Mail& mail : vec)
    {
        if (mail.MailID == MailID)
        {
            return &mail;
        }
    }

    return NULL;
}

bool xmlTcpRedirection::Init(CXmlOpeation* pXmlOperation)
{
    TiXmlElement* pSrcPort         = NULL;
    TiXmlElement* pRedirectionIP   = NULL;
    TiXmlElement* pRedirectionPort = NULL;
    TiXmlElement* pMode            = NULL;
    TiXmlElement* pConnectState    = NULL;
    _RedirectionInfo redirecttioninfo;

    while (pXmlOperation->Read_XML_Data_Multiple_Uint32("TcpRedirection", "SrcPort", redirecttioninfo.SrcPort, pSrcPort)
           && pXmlOperation->Read_XML_Data_Multiple_String("TcpRedirection", "RedirectionIP", redirecttioninfo.RedirectionIP, pRedirectionIP)
           && pXmlOperation->Read_XML_Data_Multiple_Uint32("TcpRedirection", "RedirectionPort", redirecttioninfo.RedirectionPort, pRedirectionPort)
           && pXmlOperation->Read_XML_Data_Multiple_Uint8("TcpRedirection", "Mode", redirecttioninfo.Mode, pMode)
           && pXmlOperation->Read_XML_Data_Multiple_Uint8("TcpRedirection", "ConnectState", redirecttioninfo.ConnectState, pConnectState))
    {
        vec.push_back(redirecttioninfo);
    }

    return true;
}

bool xmlCommandsTimeout::Init(CXmlOpeation* pXmlOperation)
{
    TiXmlElement* pCommandID = NULL;
    TiXmlElement* pTimeout = NULL;

    _CommandsTimeout CommandsTimeout;

    while (pXmlOperation->Read_XML_Data_Multiple_Uint16("CommandTimeout", "CommandID", CommandsTimeout.CommandID, pCommandID)
           && pXmlOperation->Read_XML_Data_Multiple_Uint16("CommandTimeout", "Timeout", CommandsTimeout.Timeout, pTimeout))
    {
        vec.push_back(CommandsTimeout);
    }

    return true;
}

xmlCommandsTimeout::_CommandsTimeout* xmlCommandsTimeout::GetCommandAlert(uint16 CommandID)
{
    for (_CommandsTimeout& commandstimeout : vec)
    {
        if (commandstimeout.CommandID == CommandID)
        {
            return &commandstimeout;
        }
    }

    return NULL;
}
