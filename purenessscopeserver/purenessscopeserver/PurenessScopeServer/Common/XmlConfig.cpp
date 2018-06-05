#include "XmlConfig.h"
#include "define.h"
#include "IpCheck.h"

/*数组元素对应的元素位置已经在构造时候按枚举确认*/
IConfigOpeation* IConfigOpeation::_array[XML_Config_MAX];


/*定义静态类对象并绑定对应枚举，实现返回模板函数*/
#define DefineClassAndFunc(ClassName, XMLConfig)                            \
static ClassName    this##ClassName(XMLConfig, "##ClassName##" );           \
template<>                                                                  \
ClassName* XMainConfig::GetXmlConfig<ClassName>()                           \
{                                                                           \
    return dynamic_cast<ClassName*>(IConfigOpeation::_array[XMLConfig]);    \
}


/*xml配置文件对应类型配置的静态类，需要增加配置文件标签，需要在此添加静态类对象，所有对象不直接使用,也不允许外部使用*/
/*****************类对象和返回函数一一对应*********************/
DefineClassAndFunc(xmlRecvInfo, XML_Config_RecvInfo)
DefineClassAndFunc(xmlSendInfo, XML_Config_SendInfo)
DefineClassAndFunc(xmlNetWorkMode, XML_Config_NetWorkMode)
DefineClassAndFunc(xmlTCPServerIPs, XML_Config_TCPServerIPs)
DefineClassAndFunc(xmlUDPServerIPs, XML_Config_UDPServerIPs)
DefineClassAndFunc(xmlConnectServer, XML_Config_ConnectServer)
DefineClassAndFunc(xmlClientInfo, XML_Config_ClientInfo)
DefineClassAndFunc(xmlModuleInfos, XML_Config_ModuleInfos)
DefineClassAndFunc(xmlModuleMangager, XML_Config_ModuleMangager)
DefineClassAndFunc(xmlMonitor, XML_Config_Monitor)
DefineClassAndFunc(xmlThreadInfoAI, XML_Config_ThreadInfoAI)
DefineClassAndFunc(xmlThreadInfo, XML_Config_ThreadInfo)
DefineClassAndFunc(xmlConsole, XML_Config_Console)
DefineClassAndFunc(xmlConsoleKeys, XML_Config_ConsoleKeys)
DefineClassAndFunc(xmlConsoleClients, XML_Config_ConsoleClients)
DefineClassAndFunc(xmlAceDebug, XML_Config_AceDebug)
DefineClassAndFunc(xmlCommandAccount, XML_Config_CommandAccount)
DefineClassAndFunc(xmlCoreSetting, XML_Config_CoreSetting)
DefineClassAndFunc(xmlServerType, XML_Config_ServerType)
DefineClassAndFunc(xmlServerID, XML_Config_ServerID)
DefineClassAndFunc(xmlServerName, XML_Config_ServerName)
DefineClassAndFunc(xmlServerVersion, XML_Config_ServerVersion)
DefineClassAndFunc(xmlPacketParses, XML_Config_PacketParses)
DefineClassAndFunc(xmlBuffPacket, XML_Config_BuffPacket)
DefineClassAndFunc(xmlMessage, XML_Config_Message)
DefineClassAndFunc(xmlAlertConnect, XML_Config_AlertConnect)
DefineClassAndFunc(xmlIP, XML_Config_IP)
DefineClassAndFunc(xmlClientData, XML_Config_ClientData)
DefineClassAndFunc(xmlCommandInfos, XML_Config_CommandInfos)
DefineClassAndFunc(xmlMails, XML_Config_Mails)
DefineClassAndFunc(xmlWorkThreadChart, XML_Config_WorkThreadChart)
DefineClassAndFunc(xmlConnectChart, XML_Config_ConnectChart)

bool XMainConfig::Init()
{
    //初始化xml文件
    return InitFile(MAINCONFIG, XML_Config_RecvInfo, XML_Config_Message)
           && InitFile(ALERTCONFIG, XML_Config_AlertConnect, XML_Config_ConnectChart);
}

bool XMainConfig::InitFile(const char* pFileName, XmlConfig start, XmlConfig end)
{
    bool bKet = true;

    if (m_XmlOpeation.Init(pFileName))
    {
        /*
        注意for循环为2个判断条件
        1.范围
        2.init函数返回结果
        */
		int i = start;
        for (; i <= end && bKet; ++i)
        {
            bKet = IConfigOpeation::_array[i]->Init(&m_XmlOpeation);
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

/**********************************对应XmlConfig类各自的初始化行为***************************************/

bool xmlRecvInfo::Init(CXmlOpeation* m_pXmlOpeation)
{
    return m_pXmlOpeation->Read_XML_Data_Single_Uint16("RecvInfo", "RecvQueueTimeout", RecvQueueTimeout)
           && m_pXmlOpeation->Read_XML_Data_Single_Uint32("RecvInfo", "RecvBuffSize", RecvBuffSize);
}

bool xmlSendInfo::Init(CXmlOpeation* m_pXmlOpeation)
{
    bool bKet = false;

    if (m_pXmlOpeation->Read_XML_Data_Single_Uint16("SendInfo", "SendTimeout", SendTimeout)
        && m_pXmlOpeation->Read_XML_Data_Single_Uint16("SendInfo", "SendQueueMax", SendQueueMax)
        && m_pXmlOpeation->Read_XML_Data_Single_Uint16("SendInfo", "PutQueueTimeout", PutQueueTimeout)
        && m_pXmlOpeation->Read_XML_Data_Single_Uint32("SendInfo", "BlockCount", BlockCount)
        && m_pXmlOpeation->Read_XML_Data_Single_Uint32("SendInfo", "MaxBlockSize", MaxBlockSize)
        && m_pXmlOpeation->Read_XML_Data_Single_Uint16("SendInfo", "SendQueueTimeout", SendQueueTimeout)
        && m_pXmlOpeation->Read_XML_Data_Single_Uint16("SendInfo", "SendQueueCount", SendQueueCount))
    {
        SendDatamark = MaxBlockSize;
        bKet = m_pXmlOpeation->Read_XML_Data_Single_Uint16("SendInfo", "TcpNodelay", TcpNodelay);
    }

    return bKet;
}

bool xmlNetWorkMode::Init(CXmlOpeation* m_pXmlOpeation)
{
    bool bKet = false;
    std::string strMode;
    std::string strNetByteOrder;

    if (m_pXmlOpeation->Read_XML_Data_Single_String("NetWorkMode", "Mode", strMode)
        && m_pXmlOpeation->Read_XML_Data_Single_Uint16("NetWorkMode", "BackLog", BackLog)
        && m_pXmlOpeation->Read_XML_Data_Single_String("NetWorkMode", "ByteOrder", strNetByteOrder))
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
        Mode = (uint8)NETWORKMODE_PRO_IOCP;
    }
    else if (pData.compare("Select") == 0)
    {
        Mode = (uint8)NETWORKMODE_RE_SELECT;
    }
    else if (pData.compare("Poll") == 0)
    {
        Mode = (uint8)NETWORKMODE_RE_TPSELECT;
    }
    else if (pData.compare("Epoll") == 0)
    {
        Mode = (uint8)NETWORKMODE_RE_EPOLL;
    }
    else if (pData.compare("Epoll_et") == 0)
    {
        Mode = (uint8)NETWORKMODE_RE_EPOLL_ET;
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
    //判定字节序
    if (O32_HOST_ORDER == O32_LITTLE_ENDIAN)
    {
        LocalByteOrder = SYSTEM_LITTLE_ORDER;
        OUR_DEBUG((LM_INFO, "[XMainConfig::SetLocalByteOrder]SYSYTEM SYSTEM_LITTLE_ORDER.\n"));
    }
    else
    {
        LocalByteOrder = SYSTEM_BIG_ORDER;
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


bool xmlTCPServerIPs::Init(CXmlOpeation* pXmlOpeation)
{
    bool bKet = true;
    TiXmlElement* pIP = NULL;
    TiXmlElement* pPort = NULL;
    TiXmlElement* pPacketParseID = NULL;

    _TCPServerIP tcpServerIP;

    while (bKet
           && pXmlOpeation->Read_XML_Data_Multiple_String("TCPServerIP", "ip", tcpServerIP.ip, pIP)
           && pXmlOpeation->Read_XML_Data_Multiple_Uint32("TCPServerIP", "port", tcpServerIP.port, pPort)
           && pXmlOpeation->Read_XML_Data_Multiple_Uint32("TCPServerIP", "ParseID", tcpServerIP.packetparseid, pPacketParseID))
    {
        bKet = Check_IPType(tcpServerIP.ip, tcpServerIP.ipType);
        vec.push_back(tcpServerIP);
    }

    return bKet;
}

bool xmlUDPServerIPs::Init(CXmlOpeation* pXmlOpeation)
{
    bool bKet = true;
    TiXmlElement* pUIP = NULL;
    TiXmlElement* pUPort = NULL;
    TiXmlElement* pUMaxRecvSize = NULL;
    TiXmlElement* pUPacketParseID = NULL;

    _UDPServerIP udpServerIP;

    while (bKet
           && pXmlOpeation->Read_XML_Data_Multiple_String("UDPServerIP", "uip", udpServerIP.uip, pUIP)
           && pXmlOpeation->Read_XML_Data_Multiple_Uint32("UDPServerIP", "uport", udpServerIP.uport, pUPort)
           && pXmlOpeation->Read_XML_Data_Multiple_Uint32("UDPServerIP", "uMaxRecvSize", udpServerIP.uMaxRecvSize, pUMaxRecvSize)
           && pXmlOpeation->Read_XML_Data_Multiple_Uint32("UDPServerIP", "uParseID", udpServerIP.uPacketParseID, pUPacketParseID))
    {
        bKet = Check_IPType(udpServerIP.uip, udpServerIP.uipType);
        vec.push_back(udpServerIP);
    }

    return bKet;
}



bool xmlConnectServer::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint32("ConnectServer", "TimeInterval", TimeInterval)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("ConnectServer", "Recvbuff", Recvbuff)
           && pXmlOpeation->Read_XML_Data_Single_Uint8("ConnectServer", "RunType", RunType)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("ConnectServer", "TimeCheck", TimeCheck)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("ConnectServer", "Count", Count);
}

bool xmlClientInfo::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint32("ClientInfo", "HandlerCount", HandlerCount)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("ClientInfo", "MaxHandlerCount", MaxHandlerCount)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("ClientInfo", "MaxConnectTime", MaxConnectTime)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("ClientInfo", "CheckAliveTime", CheckAliveTime)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("ClientInfo", "MaxBuffRecv", MaxBuffRecv);
}

bool xmlModuleInfos::Init(CXmlOpeation* pXmlOpeation)
{
    TiXmlElement* pName = NULL;
    TiXmlElement* pPath = NULL;
    TiXmlElement* pParam = NULL;

    _ModuleInfo moduleInfo;

    while (pXmlOpeation->Read_XML_Data_Multiple_String("ModuleInfo", "ModuleSName", moduleInfo.szModuleName, pName)
           && pXmlOpeation->Read_XML_Data_Multiple_String("ModuleInfo", "ModuleSPath", moduleInfo.szModulePath, pPath)
           && pXmlOpeation->Read_XML_Data_Multiple_String("ModuleInfo", "ModuleSParam", moduleInfo.szModuleParam, pParam))
    {
        vec.push_back(moduleInfo);
    }

    return true;
}

bool xmlModuleMangager::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint32("ModuleManager", "MaxCount", MaxCount);
}

bool xmlMonitor::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint8("Monitor", "CpuAndMemory", CpuAndMemory)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("Monitor", "CpuMax", CpuMax)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("Monitor", "MemoryMax", MemoryMax);
}

bool xmlThreadInfoAI::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint8("ThreadInfoAI", "AI", AI)
           && pXmlOpeation->Read_XML_Data_Single_Uint8("ThreadInfoAI", "ReturnDataType", ReturnDataType)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("ThreadInfoAI", "CheckTime", CheckTime)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("ThreadInfoAI", "TimeoutCount", TimeoutCount)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("ThreadInfoAI", "StopTime", StopTime)
           && pXmlOpeation->Read_XML_Data_Single_String("ThreadInfoAI", "ReturnData", ReturnData);
}

bool xmlThreadInfo::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint16("ThreadInfo", "ThreadTimeout", ThreadTimeout)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("ThreadInfo", "ThreadTimeCheck", ThreadTimeCheck)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("ThreadInfo", "DisposeTimeout", DisposeTimeout)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("ThreadInfo", "PutQueueTimeout", PutQueueTimeout);
}

bool xmlConsole::Init(CXmlOpeation* pXmlOpeation)
{
    bool bKet = true;

    if (pXmlOpeation->Read_XML_Data_Single_Uint8("Console", "support", support)
        && pXmlOpeation->Read_XML_Data_Single_String("Console", "sip", sip)
        && pXmlOpeation->Read_XML_Data_Single_Uint16("Console", "sport", sport))
    {
        bKet = Check_IPType(sip, ipType);
    }

    return bKet;
}

bool xmlConsoleKeys::Init(CXmlOpeation* pXmlOpeation)
{
    TiXmlElement* pKey = NULL;
    _ConsoleKey consolekey;

    while (pXmlOpeation->Read_XML_Data_Multiple_String("ConsoleKey", "Key", consolekey.Key, pKey))
    {
        vec.push_back(consolekey);
    }

    return true;
}

bool xmlConsoleClients::Init(CXmlOpeation* pXmlOpeation)
{
    TiXmlElement* pCip = NULL;
    _ConsoleClient consoleclient;

    while (pXmlOpeation->Read_XML_Data_Multiple_String("ConsoleClient", "cip", consoleclient.cip, pCip))
    {
        vec.push_back(consoleclient);
    }

    return true;
}

bool xmlAceDebug::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint8("AceDebug", "TrunOn", TrunOn)
           && pXmlOpeation->Read_XML_Data_Single_String("AceDebug", "DebugName", DebugName)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("AceDebug", "ChkInterval", ChkInterval)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("AceDebug", "LogFileMaxSize", LogFileMaxSize)
           && pXmlOpeation->Read_XML_Data_Single_Uint8("AceDebug", "LogFileMaxCnt", LogFileMaxCnt)
           && pXmlOpeation->Read_XML_Data_Single_String("AceDebug", "Level", Level);
}

bool xmlCommandAccount::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint8("CommandAccount", "Account", Account)
           && pXmlOpeation->Read_XML_Data_Single_Uint8("CommandAccount", "FlowAccount", FlowAccount)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("CommandAccount", "MaxCommandCount", MaxCommandCount);
}

bool xmlCoreSetting::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint8("CoreSetting", "CoreNeed", CoreNeed)
           && pXmlOpeation->Read_XML_Data_Single_String("CoreSetting", "Script", Script);
}

bool xmlServerType::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint8("ServerType", "Type", Type)
           && pXmlOpeation->Read_XML_Data_Single_String("ServerType", "name", name)
           && pXmlOpeation->Read_XML_Data_Single_String("ServerType", "displayname", displayname)
           && pXmlOpeation->Read_XML_Data_Single_Uint8("ServerType", "Debug", Debug)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("ServerType", "DebugSize", DebugSize)
           && pXmlOpeation->Read_XML_Data_Single_Uint8("ServerType", "IsClose", IsClose);
}

bool xmlServerID::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint32("ServerID", "id", id);
}

bool xmlServerName::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_String("ServerName", "name", name);
}

bool xmlServerVersion::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_String("ServerVersion", "Version", Version);
}

bool xmlPacketParses::Init(CXmlOpeation* pXmlOpeation)
{
    TiXmlElement* pParseID = NULL;
    TiXmlElement* pPath = NULL;
    TiXmlElement* pName = NULL;
    TiXmlElement* pType = NULL;
    TiXmlElement* pOrg = NULL;

    _PacketParse packetparse;
    string Type;

    while (pXmlOpeation->Read_XML_Data_Multiple_Uint16("PacketParse", "ParseID", packetparse.ParseID, pParseID)
           && pXmlOpeation->Read_XML_Data_Multiple_String("PacketParse", "ModulePath", packetparse.ModulePath, pPath)
           && pXmlOpeation->Read_XML_Data_Multiple_String("PacketParse", "ModuleName", packetparse.ModuleName, pName)
           && pXmlOpeation->Read_XML_Data_Multiple_String("PacketParse", "Type", Type, pType)
           && pXmlOpeation->Read_XML_Data_Multiple_Uint16("PacketParse", "OrgLength", packetparse.OrgLength, pOrg))
    {
        if (Type.compare("STREAM") == 0)
        {
            //流模式
            packetparse.Type = (uint8)PACKET_WITHSTREAM;
        }
        else
        {
            //数据头体模式
            packetparse.Type = (uint8)PACKET_WITHHEAD;
        }

        vec.push_back(packetparse);
    }

    return true;
}

bool xmlBuffPacket::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint32("BuffPacket", "Count", Count);
}

bool xmlMessage::Init(CXmlOpeation* pXmlOpeation)
{
    bool bKet = false;

    if (pXmlOpeation->Read_XML_Data_Single_Uint32("Message", "Msg_High_mark", Msg_High_mark)
        && pXmlOpeation->Read_XML_Data_Single_Uint32("Message", "Msg_Low_mark", Msg_Low_mark)
        && pXmlOpeation->Read_XML_Data_Single_Uint32("Message", "Msg_Buff_Max_Size", Msg_Buff_Max_Size)
        && pXmlOpeation->Read_XML_Data_Single_Uint16("Message", "Msg_Thread", Msg_Thread)
        && pXmlOpeation->Read_XML_Data_Single_Uint32("Message", "Msg_MaxQueue", Msg_MaxQueue))
    {
        pXmlOpeation->Read_XML_Data_Single_Uint16("Message", "Msg_Process", Msg_Process);
        bKet = true;
    }

    return bKet;
}

/******************************alert.xml***************************************/
bool xmlAlertConnect::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint8("AlertConnect", "ConnectMin", ConnectMin)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("AlertConnect", "ConnectMax", ConnectMax)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("AlertConnect", "DisConnectMin", DisConnectMin)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("AlertConnect", "DisConnectMax", DisConnectMax)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("AlertConnect", "ConnectAlert", ConnectAlert)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("AlertConnect", "MailID", MailID);
}

bool xmlIP::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint8("IP", "IPMax", IPMax)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("IP", "Timeout", Timeout);
}

bool xmlClientData::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint16("ClientData", "RecvPacketCount", RecvPacketCount)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("ClientData", "RecvDataMax", RecvDataMax)
           && pXmlOpeation->Read_XML_Data_Single_Uint16("ClientData", "SendPacketCount", SendPacketCount)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("ClientData", "SendDataMax", SendDataMax)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("ClientData", "MailID", MailID);
}

bool xmlCommandInfos::Init(CXmlOpeation* pXmlOpeation)
{
    TiXmlElement* pCommandID = NULL;
    TiXmlElement* pCommandCount = NULL;
    TiXmlElement* pMailID = NULL;
    _CommandInfo commandInfo;

    while (pXmlOpeation->Read_XML_Data_Multiple_Uint32("CommandInfo", "CommandID", commandInfo.CommandID, pCommandID)
           && pXmlOpeation->Read_XML_Data_Multiple_Uint32("CommandInfo", "CommandCount", commandInfo.CommandCount, pCommandCount)
           && pXmlOpeation->Read_XML_Data_Multiple_Uint32("CommandInfo", "MailID", commandInfo.MailID, pMailID))
    {
        vec.push_back(commandInfo);
    }

    return true;
}

bool xmlMails::Init(CXmlOpeation* pXmlOpeation)
{
    TiXmlElement* pMailID = NULL;
    TiXmlElement* pfromMailAddr = NULL;
    TiXmlElement* ptoMailAddr = NULL;
    TiXmlElement* pMailPass = NULL;
    TiXmlElement* pMailUrl = NULL;
    TiXmlElement* pMailPort = NULL;
    _Mail mail;

    while (pXmlOpeation->Read_XML_Data_Multiple_Uint16("Mail", "MailID", mail.MailID, pMailID)
           && pXmlOpeation->Read_XML_Data_Multiple_String("Mail", "fromMailAddr", mail.fromMailAddr, pfromMailAddr)
           && pXmlOpeation->Read_XML_Data_Multiple_String("Mail", "toMailAddr", mail.toMailAddr, ptoMailAddr)
           && pXmlOpeation->Read_XML_Data_Multiple_String("Mail", "MailPass", mail.MailPass, pMailPass)
           && pXmlOpeation->Read_XML_Data_Multiple_String("Mail", "MailUrl", mail.MailUrl, pMailUrl)
           && pXmlOpeation->Read_XML_Data_Multiple_Uint16("Mail", "MailPort", mail.MailPort, pMailPort))
    {
        vec.push_back(mail);
    }

    return true;
}

xmlMails::_Mail* xmlMails::GetMailAlert(uint16 MailID)
{
    for (int i = 0; i < (int)vec.size(); i++)
    {
        if (vec[i].MailID == MailID)
        {
            return &vec[i];
        }
    }

    return NULL;
}

bool xmlWorkThreadChart::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint16("WorkThreadChart", "JsonOutput", JsonOutput)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("WorkThreadChart", "Count", Count)
           && pXmlOpeation->Read_XML_Data_Single_String("WorkThreadChart", "File", File);
}

bool xmlConnectChart::Init(CXmlOpeation* pXmlOpeation)
{
    return pXmlOpeation->Read_XML_Data_Single_Uint16("ConnectChart", "JsonOutput", JsonOutput)
           && pXmlOpeation->Read_XML_Data_Single_Uint32("ConnectChart", "Count", Count)
           && pXmlOpeation->Read_XML_Data_Single_String("ConnectChart", "File", File);
}
