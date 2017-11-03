#include "MainConfig.h"

CMainConfig::CMainConfig(void)
{
    m_szError[0] = '\0';

    m_u1ServerClose           = 0;
    m_u4MsgHighMark           = 0;
    m_u4MsgLowMark            = 0;
    m_u4MsgThreadCount        = 0;
    m_u1MsgProcessCount       = 1;
    m_u4MsgMaxBuffSize        = MAX_PACKET_SIZE;
    m_u4MsgMaxQueue           = 0;
    m_nEncryptFlag            = 0;
    m_nEncryptOutFlag         = 0;
    m_u4ReactorCount          = 3;
    m_u4SendTimeout           = MAX_QUEUE_TIMEOUT;
    m_u4RecvBuffSize          = 0;
    m_u2ThreadTimuOut         = 0;
    m_u2ThreadTimeCheck       = 0;
    m_u2PacketTimeOut         = 0;
    m_u2SendAliveTime         = 0;
    m_u2HandleCount           = 0;
    m_u2MaxHanderCount        = 0;
    m_u2MaxConnectTime        = 0;
    m_u1CommandAccount        = 0;
    m_u4MaxCommandCount       = 2000;
    m_u2MaxModuleCount        = 100;
    m_u4ServerConnectCount    = 100;
    m_nServerID               = 0;
    m_u2SendQueueMax          = 0;
    m_szConsoleIP[0]          = '\0';

    m_u1ServerType            = 0;
    m_u1Debug                 = DEBUG_OFF;
    m_u4DebugSize             = MAX_BUFF_1024;
    m_u1Monitor               = 0;
    m_u4SendDatamark          = MAX_BLOCK_SIZE;
    m_u4BlockSize             = MAX_BLOCK_SIZE;
    m_u4CoreFileSize          = 0;
    m_u2TcpNodelay            = TCP_NODELAY_ON;

    m_u1ConsoleSupport        = 0;
    m_nConsolePort            = 0;
    m_u1ConsoleIPType         = TYPE_IPV4;
    m_u4ConnectServerTimerout = 0;
    m_u2ConnectServerCheck    = CONNECT_LIMIT_RETRY;
    m_u4ConnectServerRecvBuff = MAX_BUFF_1024;
    m_u1ConnectServerRunType  = 0;
    m_u4ServerRecvBuff        = MAX_BUFF_1024;

    m_u2RecvQueueTimeout    = MAX_QUEUE_TIMEOUT;
    m_u2SendQueueTimeout    = MAX_QUEUE_TIMEOUT;
    m_u2SendQueueCount      = SENDQUEUECOUNT;
    m_u2SendQueuePutTime    = (uint16)MAX_MSG_PUTTIMEOUT;
    m_u2WorkQueuePutTime    = (uint16)MAX_MSG_PUTTIMEOUT;
    m_u2Backlog             = (uint16)MAX_ASYNCH_BACKLOG;
    m_u4TrackIPCount        = (uint32)MAX_ASYNCH_BACKLOG;
    m_u4SendBlockCount      = 10;

    //默认的CPU监控和内存监控的上限
    m_u4MaxCpu              = 90;
    m_u4MaxMemory           = 2000;

    m_u1CommandFlow         = 0;

    m_u1NetworkMode         = (uint8)NETWORKMODE_PRO_IOCP;

    m_szServerName[0]         = '\0';
    m_szServerVersion[0]      = '\0';
    m_szPacketVersion[0]      = '\0';
    m_szEncryptPass[0]        = '\0';

    m_szWindowsServiceName[0] = '\0';
    m_szDisplayServiceName[0] = '\0';

    m_u1WTAI                  = (uint8)0;   //AI默认为关闭
    m_u4WTCheckTime           = 0;
    m_u4WTTimeoutCount        = 0;
    m_u4WTStopTime            = 0;
    m_u1WTReturnDataType      = 0;
    m_szWTReturnData[0]       = '\0';
    m_blByteOrder             = false;      //默认为主机字序

    m_u1DebugTrunOn           = 0;          //0为关闭，1为打开
    m_szDeubgFileName[0]      = '\0';
    m_u4ChkInterval           = 600;
    m_u4LogFileMaxSize        = MAX_BUFF_1024*10;
    m_u4LogFileMaxCnt         = 3;
    m_szDebugLevel[0]         = '\0';

    //判定字节序
    if(O32_HOST_ORDER == O32_LITTLE_ENDIAN)
    {
        m_u1CharOrder = SYSTEM_LITTLE_ORDER;
        OUR_DEBUG((LM_INFO, "[CMainConfig::CMainConfig]SYSYTEM SYSTEM_LITTLE_ORDER.\n"));
    }
    else
    {
        m_u1CharOrder = SYSTEM_BIG_ORDER;
        OUR_DEBUG((LM_INFO, "[CMainConfig::CMainConfig]SYSYTEM SYSTEM_BIG_ORDER.\n"));
    }
}

CMainConfig::~CMainConfig(void)
{
    OUR_DEBUG((LM_INFO, "[CMainConfig::~CMainConfig].\n"));
    m_vecServerInfo.clear();
    m_vecUDPServerInfo.clear();
    OUR_DEBUG((LM_INFO, "[CMainConfig::~CMainConfig]End.\n"));
}

const char* CMainConfig::GetError()
{
    return m_szError;
}

bool CMainConfig::Init()
{
    //获得数据解析包的版本号
    CPacketParse objPacketParse;
    sprintf_safe(m_szPacketVersion, MAX_BUFF_20, "%s", objPacketParse.GetPacketVersion());

    //读取主配置文件
    bool blState = Init_Main(MAINCONFIG);

    if(blState == false)
    {
        return false;
    }

    //读取报警相关配置文件
    blState = Init_Alert(ALERTCONFIG);

    if(blState == false)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool CMainConfig::Init_Alert(const char* szConfigPath)
{
    char* pData = NULL;
    OUR_DEBUG((LM_INFO, "[CMainConfig::Init_Alert]Filename = %s.\n", szConfigPath));

    if(false == m_MainConfig.Init(szConfigPath))
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::Init_Alert]File Read Error = %s.\n", szConfigPath));
        return false;
    }

    //连接相关信息
    pData = m_MainConfig.GetData("AlertConnect", "ConnectMin");

    if(NULL != pData)
    {
        m_ConnectAlert.m_u4ConnectMin = (uint16)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("AlertConnect", "ConnectMax");

    if(NULL != pData)
    {
        m_ConnectAlert.m_u4ConnectMax = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("AlertConnect", "DisConnectMin");

    if(NULL != pData)
    {
        m_ConnectAlert.m_u4DisConnectMin = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("AlertConnect", "DisConnectMax");

    if(NULL != pData)
    {
        m_ConnectAlert.m_u4DisConnectMax = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("AlertConnect", "ConnectAlert");

    if(NULL != pData)
    {
        m_ConnectAlert.m_u4ConnectAlert = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("AlertConnect", "MailID");

    if(NULL != pData)
    {
        m_ConnectAlert.m_u4MailID = (uint32)ACE_OS::atoi(pData);
    }

    //IP相关信息
    pData = m_MainConfig.GetData("IP", "IPMax");

    if(NULL != pData)
    {
        m_IPAlert.m_u4IPMaxCount = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("IP", "Timeout");

    if(NULL != pData)
    {
        m_IPAlert.m_u4IPTimeout = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("IP", "MailID");

    if(NULL != pData)
    {
        m_IPAlert.m_u4MailID = (uint32)ACE_OS::atoi(pData);
    }

    //单链接相关阀值配置
    pData = m_MainConfig.GetData("ClientData", "RecvPacketCount");

    if(NULL != pData)
    {
        m_ClientDataAlert.m_u4RecvPacketCount = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ClientData", "RecvDataMax");

    if(NULL != pData)
    {
        m_ClientDataAlert.m_u4RecvDataMax = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ClientData", "SendPacketCount");

    if(NULL != pData)
    {
        m_ClientDataAlert.m_u4SendPacketCount = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ClientData", "SendDataMax");

    if(NULL != pData)
    {
        m_ClientDataAlert.m_u4SendDataMax = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ClientData", "MailID");

    if(NULL != pData)
    {
        m_ClientDataAlert.m_u4MailID = (uint32)ACE_OS::atoi(pData);
    }

    //命令监控相关配置
    m_vecCommandAlert.clear();
    TiXmlElement* pNextTiXmlElementCommand     = NULL;
    TiXmlElement* pNextTiXmlElementCount       = NULL;
    TiXmlElement* pNextTiXmlElementMailID      = NULL;
    _CommandAlert objCommandAlert;

    while(true)
    {
        pData = m_MainConfig.GetData("CommandInfo", "CommandID", pNextTiXmlElementCommand);

        if(pData != NULL)
        {
            objCommandAlert.m_u2CommandID = (uint16)ACE_OS::atoi(pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("CommandInfo", "CommandCount", pNextTiXmlElementCount);

        if(pData != NULL)
        {
            objCommandAlert.m_u4CommandCount = (uint32)ACE_OS::atoi(pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("CommandInfo", "MailID", pNextTiXmlElementMailID);

        if(pData != NULL)
        {
            objCommandAlert.m_u4MailID = (uint32)ACE_OS::atoi(pData);
        }
        else
        {
            break;
        }

        m_vecCommandAlert.push_back(objCommandAlert);
    }

    //报警邮件相关设置
    m_vecMailAlert.clear();
    pNextTiXmlElementMailID      = NULL;
    TiXmlElement* pNextTiXmlElementFormAddr    = NULL;
    TiXmlElement* pNextTiXmlElementToAddr      = NULL;
    TiXmlElement* pNextTiXmlElementMailPass    = NULL;
    TiXmlElement* pNextTiXmlElementMailUrl     = NULL;
    TiXmlElement* pNextTiXmlElementMailPort    = NULL;
    _MailAlert objMailAlert;

    while(true)
    {
        pData = m_MainConfig.GetData("Mail", "MailID", pNextTiXmlElementMailID);

        if(pData != NULL)
        {
            objMailAlert.m_u4MailID = (uint32)ACE_OS::atoi(pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("Mail", "fromMailAddr", pNextTiXmlElementFormAddr);

        if(pData != NULL)
        {
            sprintf_safe(objMailAlert.m_szFromMailAddr, MAX_BUFF_200, "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("Mail", "toMailAddr", pNextTiXmlElementToAddr);

        if(pData != NULL)
        {
            sprintf_safe(objMailAlert.m_szToMailAddr, MAX_BUFF_200, "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("Mail", "MailPass", pNextTiXmlElementMailPass);

        if(pData != NULL)
        {
            sprintf_safe(objMailAlert.m_szMailPass, MAX_BUFF_200, "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("Mail", "MailUrl", pNextTiXmlElementMailUrl);

        if(pData != NULL)
        {
            sprintf_safe(objMailAlert.m_szMailUrl, MAX_BUFF_200, "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("Mail", "MailPort", pNextTiXmlElementMailPort);

        if(pData != NULL)
        {
            objMailAlert.m_u4MailPort = (uint32)ACE_OS::atoi(pData);
        }
        else
        {
            break;
        }

        m_vecMailAlert.push_back(objMailAlert);
    }

    m_MainConfig.Close();

    return true;
}

bool CMainConfig::Init_Main(const char* szConfigPath)
{
    char* pData = NULL;
    OUR_DEBUG((LM_INFO, "[CMainConfig::Init_Main]Filename = %s.\n", szConfigPath));

    if(false == m_MainConfig.Init(szConfigPath))
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::Init_Main]File Read Error = %s.\n", szConfigPath));
        return false;
    }

    //获得当前网络模型
    pData = m_MainConfig.GetData("NetWorkMode", "Mode");

    if(NULL != pData)
    {
        if(ACE_OS::strcmp(pData, "Iocp") == 0)
        {
            m_u1NetworkMode = (uint8)NETWORKMODE_PRO_IOCP;
        }
        else if(ACE_OS::strcmp(pData, "Select") == 0)
        {
            m_u1NetworkMode = (uint8)NETWORKMODE_RE_SELECT;
        }
        else if(ACE_OS::strcmp(pData, "Poll") == 0)
        {
            m_u1NetworkMode = (uint8)NETWORKMODE_RE_TPSELECT;
        }
        else if(ACE_OS::strcmp(pData, "Epoll") == 0)
        {
            m_u1NetworkMode = (uint8)NETWORKMODE_RE_EPOLL;
        }
        else if(ACE_OS::strcmp(pData, "Epoll_et") == 0)
        {
            m_u1NetworkMode = (uint8)NETWORKMODE_RE_EPOLL_ET;
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CMainConfig::Init_Main]NetworkMode is Invalid!!, please read main.xml desc.\n", szConfigPath));
            return false;
        }
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::Init_Main]NetworkMode is Invalid!!, please read main.xml desc.\n", szConfigPath));
        return false;
    }

    pData = m_MainConfig.GetData("NetWorkMode", "BackLog");

    if(NULL != pData)
    {
        m_u2Backlog = (uint16)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("NetWorkMode", "ByteOrder");

    if(NULL != pData)
    {
        if(ACE_OS::strcmp(pData, "NET_ORDER") == 0)
        {
            m_blByteOrder = true;
        }
        else
        {
            m_blByteOrder = false;
        }
    }

    //获得服务器基础属性
    pData = m_MainConfig.GetData("ServerType", "Type");

    if(NULL != pData)
    {
        m_u1ServerType = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ServerType", "Debug");

    if(NULL != pData)
    {
        m_u1Debug = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ServerType", "DebugSize");

    if (NULL != pData)
    {
        m_u4DebugSize = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ServerType", "IsClose");

    if(NULL != pData)
    {
        m_u1ServerClose = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ServerType", "name");

    if(NULL != pData)
    {
        sprintf_safe(m_szWindowsServiceName, MAX_BUFF_50, "%s", pData);
    }

    pData = m_MainConfig.GetData("ServerType", "displayname");

    if(NULL != pData)
    {
        sprintf_safe(m_szDisplayServiceName, MAX_BUFF_50, "%s", pData);
    }

    pData = m_MainConfig.GetData("ServerID", "id");

    if(NULL != pData)
    {
        m_nServerID = ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ServerName", "name");

    if(NULL != pData)
    {
        sprintf_safe(m_szServerName, MAX_BUFF_20, "%s", pData);
    }

    pData = m_MainConfig.GetData("ServerVersion", "Version");

    if(NULL != pData)
    {
        sprintf_safe(m_szServerVersion, MAX_BUFF_20, "%s", pData);
    }

    //获得监听端口信息
    _ServerInfo serverinfo;

    m_vecServerInfo.clear();
    TiXmlElement* pNextTiXmlElementIP       = NULL;
    TiXmlElement* pNextTiXmlElementPort     = NULL;
    TiXmlElement* pNextTiXmlElementIpType   = NULL;
    TiXmlElement* pNextTiXmlElementPacketID = NULL;

    while(true)
    {
        pData = m_MainConfig.GetData("TCPServerIP", "ip", pNextTiXmlElementIP);

        if(pData != NULL)
        {
            sprintf_safe(serverinfo.m_szServerIP, MAX_BUFF_50, "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("TCPServerIP", "port", pNextTiXmlElementPort);

        if(pData != NULL)
        {
            serverinfo.m_nPort = ACE_OS::atoi(pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("TCPServerIP", "ipType", pNextTiXmlElementIpType);

        if(pData != NULL)
        {
            if(ACE_OS::strcmp(pData, "IPV6") == 0)
            {
                serverinfo.m_u1IPType = TYPE_IPV6;
            }
            else
            {
                serverinfo.m_u1IPType = TYPE_IPV4;
            }
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("TCPServerIP", "ParseID", pNextTiXmlElementPacketID);

        if (pData != NULL)
        {
            serverinfo.m_u4PacketParseInfoID = (uint32)ACE_OS::atoi(pData);
        }
        else
        {
            serverinfo.m_u4PacketParseInfoID = 1;
        }

        m_vecServerInfo.push_back(serverinfo);
    }

    //开始获得UDP服务器相关参数
    m_vecUDPServerInfo.clear();
    pNextTiXmlElementIP = NULL;
    pNextTiXmlElementPort = NULL;
    pNextTiXmlElementIpType = NULL;
    pNextTiXmlElementPacketID = NULL;

    while (true)
    {
        pData = m_MainConfig.GetData("UDPServerIP", "uip", pNextTiXmlElementIP);

        if (pData != NULL)
        {
            sprintf_safe(serverinfo.m_szServerIP, MAX_BUFF_20, "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("UDPServerIP", "uport", pNextTiXmlElementPort);

        if (pData != NULL)
        {
            serverinfo.m_nPort = ACE_OS::atoi(pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("UDPServerIP", "uipType", pNextTiXmlElementIpType);

        if (pData != NULL)
        {
            if (ACE_OS::strcmp(pData, "IPV6") == 0)
            {
                serverinfo.m_u1IPType = TYPE_IPV6;
            }
            else
            {
                serverinfo.m_u1IPType = TYPE_IPV4;
            }
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("UDPServerIP", "ParseID", pNextTiXmlElementPacketID);

        if (pData != NULL)
        {
            serverinfo.m_u4PacketParseInfoID = (uint32)ACE_OS::atoi(pData);
        }
        else
        {
            serverinfo.m_u4PacketParseInfoID = 0;
        }

        m_vecUDPServerInfo.push_back(serverinfo);
    }

    //开始获得数据解析模块相关信息
    _PacketParseInfo objPacketParseInfo;

    TiXmlElement* pNextTiXmlElementPacketParseID    = NULL;
    TiXmlElement* pNextTiXmlElementPacketParsePath  = NULL;
    TiXmlElement* pNextTiXmlElementPacketParseName  = NULL;
    TiXmlElement* pNextTiXmlElementPacketParseType  = NULL;
    TiXmlElement* pNextTiXmlElementPacketParseLen   = NULL;

    m_vecPacketParseInfo.clear();

    while (true)
    {
        pData = m_MainConfig.GetData("PacketParse", "ParseID", pNextTiXmlElementPacketParseID);

        if (pData != NULL)
        {
            objPacketParseInfo.m_u4PacketID = (uint32)atoi(pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("PacketParse", "ModulePath", pNextTiXmlElementPacketParsePath);

        if (pData != NULL)
        {
            sprintf_safe(objPacketParseInfo.m_szPacketParsePath, MAX_BUFF_200, "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("PacketParse", "ModuleName", pNextTiXmlElementPacketParseName);

        if (pData != NULL)
        {
            sprintf_safe(objPacketParseInfo.m_szPacketParseName, MAX_BUFF_100, "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("PacketParse", "Type", pNextTiXmlElementPacketParseType);

        if (pData != NULL)
        {
            if (ACE_OS::strcmp(pData, "STREAM") == 0)
            {
                //流模式
                objPacketParseInfo.m_u1Type = (uint8)PACKET_WITHSTREAM;
            }
            else
            {
                //数据头体模式
                objPacketParseInfo.m_u1Type = (uint8)PACKET_WITHHEAD;
            }
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("PacketParse", "OrgLength", pNextTiXmlElementPacketParseLen);

        if (pData != NULL)
        {
            objPacketParseInfo.m_u4OrgLength = (uint32)ACE_OS::atoi(pData);
        }
        else
        {
            break;
        }

        m_vecPacketParseInfo.push_back(objPacketParseInfo);
    }

    //开始设置默认PacketParseID(TCP)
    for (int32 i = 0; i < (int32)m_vecServerInfo.size(); i++)
    {
        if (m_vecServerInfo[i].m_u4PacketParseInfoID == 0)
        {
            m_vecServerInfo[i].m_u4PacketParseInfoID = m_vecPacketParseInfo[0].m_u4PacketID;
        }
    }

    //开始设置默认PacketParseID(UDP)
    for (int32 i = 0; i < (int32)m_vecUDPServerInfo.size(); i++)
    {
        if (m_vecUDPServerInfo[i].m_u4PacketParseInfoID == 0)
        {
            m_vecUDPServerInfo[i].m_u4PacketParseInfoID = m_vecPacketParseInfo[0].m_u4PacketID;
        }
    }

    //开始获得消息处理线程参数
    pData = m_MainConfig.GetData("Message", "Msg_High_mark");

    if(pData != NULL)
    {
        m_u4MsgHighMark = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("Message", "Msg_Low_mark");

    if(pData != NULL)
    {
        m_u4MsgLowMark = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("Message", "Msg_Buff_Max_Size");

    if (pData != NULL)
    {
        m_u4MsgMaxBuffSize = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("Message", "Msg_Thread");

    if(pData != NULL)
    {
        m_u4MsgThreadCount = (uint32)ACE_OS::atoi(pData);
        m_u4ReactorCount   = m_u4ReactorCount + (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("Message", "Msg_Process");

    if (pData != NULL)
    {
        m_u1MsgProcessCount = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("Message", "Msg_MaxQueue");

    if(pData != NULL)
    {
        m_u4MsgMaxQueue = (uint32)ACE_OS::atoi(pData);
    }

    //开始获得加载模块参数
    pData = m_MainConfig.GetData("ModuleManager", "MaxCount");

    if(pData != NULL)
    {
        m_u2MaxModuleCount = (uint16)ACE_OS::atoi(pData);
    }

    //开始获得单例模块相关参数
    m_vecModuleConfig.clear();
    TiXmlElement* pNextTiXmlElementModulePath  = NULL;
    TiXmlElement* pNextTiXmlElementModuleName  = NULL;
    TiXmlElement* pNextTiXmlElementModuleParam = NULL;

    while(true)
    {
        _ModuleConfig objModuleConfig;
        pData = m_MainConfig.GetData("ModuleInfo", "ModuleSPath", pNextTiXmlElementModulePath);

        if(pData != NULL)
        {
            sprintf_safe(objModuleConfig.m_szModulePath, MAX_BUFF_200, "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("ModuleInfo", "ModuleSName", pNextTiXmlElementModuleName);

        if(pData != NULL)
        {
            sprintf_safe(objModuleConfig.m_szModuleName, MAX_BUFF_100, "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("ModuleInfo", "ModuleSParam", pNextTiXmlElementModuleParam);

        if(pData != NULL)
        {
            sprintf_safe(objModuleConfig.m_szModuleParam, MAX_BUFF_200, "%s", pData);
        }
        else
        {
            break;
        }

        m_vecModuleConfig.push_back(objModuleConfig);
    }

    //开始获得Core相关设定(目前仅限Linux)
    pData = m_MainConfig.GetData("CoreSetting", "CoreNeed");

    if(NULL != pData)
    {
        m_u4CoreFileSize = (uint16)ACE_OS::atoi(pData);
    }

    //开始获得发送和接受阀值
    pData = m_MainConfig.GetData("SendInfo", "SendTimeout");

    if(pData != NULL)
    {
        m_u4SendTimeout = (int32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("SendInfo", "TcpNodelay");

    if(pData != NULL)
    {
        if((uint16)ACE_OS::atoi(pData) != TCP_NODELAY_ON)
        {
            m_u2TcpNodelay = TCP_NODELAY_OFF;
        }
    }

    pData = m_MainConfig.GetData("RecvInfo", "RecvBuffSize");

    if(pData != NULL)
    {
        m_u4RecvBuffSize = (int32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("SendInfo", "SendQueueMax");

    if(pData != NULL)
    {
        m_u2SendQueueMax = (uint16)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ThreadInfo", "ThreadTimeout");

    if(pData != NULL)
    {
        m_u2ThreadTimuOut = (uint16)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ThreadInfo", "ThreadTimeCheck");

    if(pData != NULL)
    {
        m_u2ThreadTimeCheck = (uint16)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ThreadInfo", "DisposeTimeout");

    if(pData != NULL)
    {
        m_u2PacketTimeOut = (uint16)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ClientInfo", "CheckAliveTime");

    if(pData != NULL)
    {
        m_u2SendAliveTime = (uint16)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("SendInfo", "PutQueueTimeout");

    if(pData != NULL)
    {
        m_u2SendQueuePutTime = (uint16)ACE_OS::atoi(pData);
    }

    //pData = m_MainConfig.GetData("SendInfo", "MaxSendMask");
    //if(pData != NULL)
    //{
    //  m_u4SendDatamark = (int32)ACE_OS::atoi(pData);
    //}
    pData = m_MainConfig.GetData("SendInfo", "MaxBlockSize");

    if(pData != NULL)
    {
        m_u4BlockSize    = (int32)ACE_OS::atoi(pData);
        //保持m_u4SendDatamark和m_u4BlockSize一致，不必在单独分开
        m_u4SendDatamark = m_u4BlockSize;
    }

    pData = m_MainConfig.GetData("SendInfo", "BlockCount");

    if(pData != NULL)
    {
        m_u4SendBlockCount = (uint32)ACE_OS::atoi(pData);
    }

    //线程相关
    pData = m_MainConfig.GetData("ThreadInfo", "PutQueueTimeout");

    if(pData != NULL)
    {
        m_u2WorkQueuePutTime = (uint16)ACE_OS::atoi(pData);
    }

    //连接对象缓冲相关
    pData = m_MainConfig.GetData("ClientInfo", "HandlerCount");

    if(pData != NULL)
    {
        m_u2HandleCount = (uint16)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ClientInfo", "MaxHandlerCount");

    if(pData != NULL)
    {
        m_u2MaxHanderCount = (uint16)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ClientInfo", "MaxConnectTime");

    if(pData != NULL)
    {
        m_u2MaxConnectTime = (uint16)ACE_OS::atoi((char*)pData);

        //这里增加一个判定，如果最大超时时间小于等于时间间隔参数，自动设置为时间间隔的2倍。
        if (m_u2MaxConnectTime <= m_u2SendAliveTime)
        {
            m_u2MaxConnectTime = m_u2SendAliveTime * 2;
        }
    }

    pData = m_MainConfig.GetData("ClientInfo", "MaxBuffRecv");

    if(pData != NULL)
    {
        m_u4ServerRecvBuff = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ClientInfo", "TrackIPCount");

    if(NULL != pData)
    {
        m_u4TrackIPCount = (uint16)ACE_OS::atoi(pData);
    }

    //接收客户端信息相关配置
    pData = m_MainConfig.GetData("RecvInfo", "RecvQueueTimeout");

    if(pData != NULL)
    {
        m_u2RecvQueueTimeout = (uint16)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("SendInfo", "SendQueueTimeout");

    if(pData != NULL)
    {
        m_u2SendQueueTimeout = (uint16)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("SendInfo", "SendQueueCount");

    if(pData != NULL)
    {
        m_u2SendQueueCount = (uint16)ACE_OS::atoi(pData);
    }

    //开始获得Console服务器相关配置信息
    pData = m_MainConfig.GetData("Console", "support");

    if(pData != NULL)
    {
        m_u1ConsoleSupport = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("Console", "sip");

    if(pData != NULL)
    {
        sprintf_safe(m_szConsoleIP, MAX_BUFF_100, "%s", pData);
    }

    pData = m_MainConfig.GetData("Console", "sport");

    if(pData != NULL)
    {
        m_nConsolePort = (int32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("Console", "sipType");

    if(pData != NULL)
    {
        if(ACE_OS::strcmp(pData, "IPV6") == 0)
        {
            m_u1ConsoleIPType = TYPE_IPV6;
        }
        else
        {
            m_u1ConsoleIPType = TYPE_IPV4;
        }
    }

    //获得Console可接受的客户端IP
    m_vecConsoleClientIP.clear();

    pNextTiXmlElementIP   = NULL;

    while(true)
    {
        _ConsoleClientIP ConsoleClientIP;
        pData = m_MainConfig.GetData("ConsoleClient", "cip", pNextTiXmlElementIP);

        if(NULL != pData)
        {
            sprintf_safe(ConsoleClientIP.m_szServerIP, MAX_BUFF_20, "%s", pData);
        }
        else
        {
            break;
        }

        m_vecConsoleClientIP.push_back(ConsoleClientIP);
    }

    //获得允许的Console链接key值
    m_vecConsoleKey.clear();
    TiXmlElement* pNextTiXmlElementKey  = NULL;

    while(true)
    {
        _ConsoleKey objConsoleKey;
        pData = m_MainConfig.GetData("ConsoleKey", "Key", pNextTiXmlElementKey);

        if(NULL != pData)
        {
            sprintf_safe(objConsoleKey.m_szKey, MAX_BUFF_100, "%s", pData);
        }
        else
        {
            break;
        }

        m_vecConsoleKey.push_back(objConsoleKey);
    }

    //开始获得ConnectServer相关信息
    pData = m_MainConfig.GetData("ConnectServer", "TimeInterval");

    if(pData != NULL)
    {
        m_u4ConnectServerTimerout = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ConnectServer", "TimeCheck");

    if(pData != NULL)
    {
        m_u2ConnectServerCheck = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ConnectServer", "Recvbuff");

    if(pData != NULL)
    {
        m_u4ConnectServerRecvBuff = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ConnectServer", "RunType");

    if(pData != NULL)
    {
        m_u1ConnectServerRunType = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ConnectServer", "Count");

    if(pData != NULL)
    {
        m_u4ServerConnectCount = (uint32)ACE_OS::atoi(pData);
    }

    //开始获得监控数据
    pData = m_MainConfig.GetData("Monitor", "CpuAndMemory");

    if(pData != NULL)
    {
        m_u1Monitor = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("Monitor", "CpuMax");

    if(pData != NULL)
    {
        m_u4MaxCpu = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("Monitor", "MemoryMax");

    if(pData != NULL)
    {
        m_u4MaxMemory = (uint32)ACE_OS::atoi(pData) * 1024 *1024;
    }

    //开始得到命令统计相关开关
    pData = m_MainConfig.GetData("CommandAccount", "Account");

    if(pData != NULL)
    {
        m_u1CommandAccount = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("CommandAccount", "FlowAccount");

    if(pData != NULL)
    {
        m_u1CommandFlow = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("CommandAccount", "MaxCommandCount");

    if(pData != NULL)
    {
        m_u4MaxCommandCount = (uint8)ACE_OS::atoi(pData);
    }

    //开始获得工作线程监控相关
    pData = m_MainConfig.GetData("ThreadInfoAI", "AI");

    if(pData != NULL)
    {
        m_u1WTAI = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ThreadInfoAI", "CheckTime");

    if(pData != NULL)
    {
        m_u4WTCheckTime = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ThreadInfoAI", "TimeoutCount");

    if(pData != NULL)
    {
        m_u4WTTimeoutCount = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ThreadInfoAI", "StopTime");

    if(pData != NULL)
    {
        m_u4WTStopTime = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ThreadInfoAI", "ReturnDataType");

    if(pData != NULL)
    {
        m_u1WTReturnDataType = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("ThreadInfoAI", "ReturnData");

    if(pData != NULL)
    {
        sprintf_safe(m_szWTReturnData, MAX_BUFF_1024, "%s", pData);
    }

    //ACE_DEBUG相关设置信息
    pData = m_MainConfig.GetData("AceDebug", "TrunOn");

    if(pData != NULL)
    {
        m_u1DebugTrunOn = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("AceDebug", "DebugName");

    if(pData != NULL)
    {
        sprintf_safe(m_szDeubgFileName, MAX_BUFF_100, "%s", pData);
    }

    pData = m_MainConfig.GetData("AceDebug", "ChkInterval");

    if(pData != NULL)
    {
        m_u4ChkInterval = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("AceDebug", "LogFileMaxSize");

    if(pData != NULL)
    {
        m_u4LogFileMaxSize = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("AceDebug", "LogFileMaxCnt");

    if(pData != NULL)
    {
        m_u4LogFileMaxCnt = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("AceDebug", "Level");

    if(pData != NULL)
    {
        sprintf_safe(m_szDebugLevel, MAX_BUFF_100, "%s", pData);
    }

    //集群相关配置
    pData = m_MainConfig.GetData("GroupListen", "GroupNeed");

    if(pData != NULL)
    {
        m_GroupListenInfo.m_u1GroupNeed = (uint8)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("GroupListen", "GroupIP");

    if(pData != NULL)
    {
        sprintf_safe(m_GroupListenInfo.m_szGroupIP, MAX_BUFF_50, "%s", pData);
    }

    pData = m_MainConfig.GetData("GroupListen", "GroupPort");

    if(pData != NULL)
    {
        m_GroupListenInfo.m_u4GroupPort = (uint32)ACE_OS::atoi(pData);
    }

    pData = m_MainConfig.GetData("GroupListen", "GroupIpType");

    if(pData != NULL)
    {
        if(ACE_OS::strcmp(pData, "IPV6") == 0)
        {
            m_GroupListenInfo.m_u1IPType = TYPE_IPV6;
        }
        else
        {
            m_GroupListenInfo.m_u1IPType = TYPE_IPV4;
        }
    }

    m_MainConfig.Close();

    return true;
}

void CMainConfig::Display()
{
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_nServerID = %d.\n", m_nServerID));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_szServerName = %s.\n", m_szServerName));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_szServerVersion = %s.\n", m_szServerVersion));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_szPacketVersion = %s.\n", m_szPacketVersion));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u1Debug = %d.\n", m_u1Debug));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u1ServerClose = %d.\n", m_u1ServerClose));

    for(int32 i = 0; i < (int32)m_vecServerInfo.size(); i++)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]ServerIP%d = %s.\n", i, m_vecServerInfo[i].m_szServerIP));
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]ServerPort%d = %d.\n", i, m_vecServerInfo[i].m_nPort));
    }

    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4MsgHighMark = %d.\n", m_u4MsgHighMark));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4MsgLowMark = %d.\n", m_u4MsgLowMark));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4MsgThreadCount = %d.\n", m_u4MsgThreadCount));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u1MsgProcessCount = %d.\n", m_u1MsgProcessCount));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4MsgMaxQueue = %d.\n", m_u4MsgMaxQueue));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_nEncryptFlag = %d.\n", m_nEncryptFlag));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_szEncryptPass = %s.\n", m_szEncryptPass));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_nEncryptOutFlag = %d.\n", m_nEncryptOutFlag));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4SendTimeout = %d.\n", m_u4SendTimeout));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4RecvBuffSize = %d.\n", m_u4RecvBuffSize));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2SendQueueMax = %d.\n", m_u2SendQueueMax));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2ThreadTimuOut = %d.\n", m_u2ThreadTimuOut));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2ThreadTimeCheck = %d.\n", m_u2ThreadTimeCheck));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2PacketTimeOut = %d.\n", m_u2PacketTimeOut));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2SendAliveTime = %d.\n", m_u2SendAliveTime));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2HandleCount = %d.\n", m_u2HandleCount));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2MaxHanderCount = %d.\n", m_u2MaxHanderCount));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2RecvQueueTimeout = %d.\n", m_u2RecvQueueTimeout));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2SendQueueTimeout = %d.\n", m_u2SendQueueTimeout));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2SendQueueCount = %d.\n", m_u2SendQueueCount));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2SendQueuePutTime = %d.\n", m_u2SendQueuePutTime));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2WorkQueuePutTime = %d.\n", m_u2WorkQueuePutTime));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u1ServerType = %d.\n", m_u1ServerType));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4SendDatamark = %d.\n", m_u4SendDatamark));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2TcpNodelay = %d.\n", m_u2TcpNodelay));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4TrackIPCount = %d.\n", m_u4TrackIPCount));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4BlockSize = %d.\n", m_u4BlockSize));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4SendBlockCount = %d.\n", m_u4SendBlockCount));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4MaxCommandCount = %d.\n", m_u4MaxCommandCount));

    for(int32 i = 0; i < (int32)m_vecUDPServerInfo.size(); i++)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]ServerIP%d = %s.\n", i, m_vecUDPServerInfo[i].m_szServerIP));
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]ServerPort%d = %d.\n", i, m_vecUDPServerInfo[i].m_nPort));
    }

    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u1ConsoleSupport = %d.\n", m_u1ConsoleSupport));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_szConsoleIP = %s.\n", m_szConsoleIP));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_nConsolePort = %d.\n", m_nConsolePort));

    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4ConnectServerTimerout = %d.\n", m_u4ConnectServerTimerout));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2ConnectServerCheck = %d.\n", m_u2ConnectServerCheck));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u1ConnectServerRunType = %d.\n", m_u1ConnectServerRunType));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4ConnectServerRecvBuff = %d.\n", m_u4ConnectServerRecvBuff));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u1CommandFlow = %d.\n", m_u1CommandFlow));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u1CommandAccount = %d.\n", m_u1CommandAccount));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4CoreFileSize = %d.\n", m_u4CoreFileSize));
    OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u2Backlog = %d.\n", m_u2Backlog));

    for(int32 i = 0; i < (int32)m_vecModuleConfig.size(); i++)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_szModuleName%d = %s.\n", i, m_vecModuleConfig[i].m_szModuleName));
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_szModulePath%d = %s.\n", i, m_vecModuleConfig[i].m_szModulePath));
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_szModuleParam%d = %s.\n", i, m_vecModuleConfig[i].m_szModuleParam));
    }

    for (int32 i = 0; i < (int32)m_vecPacketParseInfo.size(); i++)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_szPacketParseID = %d.\n", m_vecPacketParseInfo[i].m_u4PacketID));
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_szPacketParsePath = %s.\n", m_vecPacketParseInfo[i].m_szPacketParsePath));
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_szPacketParseName = %s.\n", m_vecPacketParseInfo[i].m_szPacketParseName));
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u1Type = %d.\n", m_vecPacketParseInfo[i].m_u1Type));
        OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4OrgLength = %d.\n", m_vecPacketParseInfo[i].m_u4OrgLength));
    }
}

const char* CMainConfig::GetServerName()
{
    return m_szServerName;
}

uint16 CMainConfig::GetServerID()
{
    return (uint16)m_nServerID;
}

uint16 CMainConfig::GetServerPortCount()
{
    return (uint16)m_vecServerInfo.size();
}

uint16 CMainConfig::GetHandleCount()
{
    return m_u2HandleCount;
}

_ServerInfo* CMainConfig::GetServerPort(int32 nIndex)
{
    if(nIndex > (uint16)m_vecServerInfo.size())
    {
        return NULL;
    }

    return &m_vecServerInfo[nIndex];
}

uint32 CMainConfig::GetMgsHighMark()
{
    return m_u4MsgHighMark;
}

uint32 CMainConfig::GetMsgLowMark()
{
    return m_u4MsgLowMark;
}

uint32 CMainConfig::GetMsgMaxBuffSize()
{
    return m_u4MsgMaxBuffSize;
}

uint32 CMainConfig::GetThreadCount()
{
    return m_u4MsgThreadCount;
}

uint8 CMainConfig::GetProcessCount()
{
    return m_u1MsgProcessCount;
}

uint32 CMainConfig::GetMsgMaxQueue()
{
    return m_u4MsgMaxQueue;
}

int32 CMainConfig::GetEncryptFlag()
{
    return m_nEncryptFlag;
}

const char* CMainConfig::GetEncryptPass()
{
    return m_szEncryptPass;
}

int32 CMainConfig::GetEncryptOutFlag()
{
    return m_nEncryptOutFlag;
}

uint32 CMainConfig::GetReactorCount()
{
    return m_u4ReactorCount;
}

uint16 CMainConfig::GetUDPServerPortCount()
{
    return (uint16)m_vecUDPServerInfo.size();
}

_ServerInfo* CMainConfig::GetUDPServerPort(int32 nIndex)
{
    if(nIndex > (uint16)m_vecUDPServerInfo.size())
    {
        return NULL;
    }

    return &m_vecUDPServerInfo[nIndex];
}

uint32 CMainConfig::GetSendTimeout()
{
    return m_u4SendTimeout;
}

uint32 CMainConfig::GetRecvBuffSize()
{
    return m_u4RecvBuffSize;
}

uint16 CMainConfig::GetSendQueueMax()
{
    return m_u2SendQueueMax;
}

uint16 CMainConfig::GetThreadTimuOut()
{
    return m_u2ThreadTimuOut;
}

uint16 CMainConfig::GetThreadTimeCheck()
{
    return m_u2ThreadTimeCheck;
}

uint16 CMainConfig::GetPacketTimeOut()
{
    return m_u2PacketTimeOut;
}

uint16 CMainConfig::GetCheckAliveTime()
{
    return m_u2SendAliveTime;
};

uint16 CMainConfig::GetMaxHandlerCount()
{
    return m_u2MaxHanderCount;
}

uint16 CMainConfig::GetMaxConnectTime()
{
    return m_u2MaxConnectTime;
}

uint8 CMainConfig::GetConsoleSupport()
{
    return m_u1ConsoleSupport;
}

int32 CMainConfig::GetConsolePort()
{
    return m_nConsolePort;
}

const char* CMainConfig::GetConsoleIP()
{
    return m_szConsoleIP;
}

uint16 CMainConfig::GetRecvQueueTimeout()
{
    return m_u2RecvQueueTimeout;
}

uint16 CMainConfig::GetSendQueueTimeout()
{
    return m_u2SendQueueTimeout;
}

uint16 CMainConfig::GetSendQueueCount()
{
    return m_u2SendQueueCount;
}

bool CMainConfig::CompareConsoleClinetIP(const char* pConsoleClientIP)
{
    //如果没有配置IP，则默认全部可以
    if((int32)m_vecConsoleClientIP.size() == 0)
    {
        return true;
    }

    for(int32 i = 0; i < (int32)m_vecConsoleClientIP.size(); i++)
    {
        if(ACE_OS::strcmp(m_vecConsoleClientIP[i].m_szServerIP, pConsoleClientIP) == 0)
        {
            return true;
        }
    }

    return false;
}

uint8 CMainConfig::GetCommandAccount()
{
    return m_u1CommandAccount;
}

const char* CMainConfig::GetServerVersion()
{
    return m_szServerVersion;
}

const char* CMainConfig::GetPacketVersion()
{
    return m_szPacketVersion;
}

vecConsoleKey* CMainConfig::GetConsoleKey()
{
    return &m_vecConsoleKey;
}

uint32 CMainConfig::GetConnectServerTimeout()
{
    return m_u4ConnectServerTimerout;
}

uint16 CMainConfig::GetConnectServerCheck()
{
    return m_u2ConnectServerCheck;
}

uint8 CMainConfig::GetConnectServerRunType()
{
    return m_u1ConnectServerRunType;
}

uint16 CMainConfig::GetSendQueuePutTime()
{
    return m_u2SendQueuePutTime;
}

uint16 CMainConfig::GetWorkQueuePutTime()
{
    return m_u2WorkQueuePutTime;
}

uint8 CMainConfig::GetServerType()
{
    return m_u1ServerType;
}

const char* CMainConfig::GetWindowsServiceName()
{
    return m_szWindowsServiceName;
}

const char* CMainConfig::GetDisplayServiceName()
{
    return m_szDisplayServiceName;
}

uint8 CMainConfig::GetDebug()
{
    return m_u1Debug;
}

NAMESPACE::uint32 CMainConfig::GetDebugSize()
{
    return m_u4DebugSize;
}

void CMainConfig::SetDebug(uint8 u1Debug)
{
    m_u1Debug = u1Debug;
}

uint8 CMainConfig::GetNetworkMode()
{
    return m_u1NetworkMode;
}

uint32 CMainConfig::GetConnectServerRecvBuffer()
{
    return m_u4ConnectServerRecvBuff;
}

uint8 CMainConfig::GetMonitor()
{
    return m_u1Monitor;
}

uint32 CMainConfig::GetServerRecvBuff()
{
    return m_u4ServerRecvBuff;
}

uint8 CMainConfig::GetConsoleIPType()
{
    return m_u1ConsoleIPType;
}

uint8 CMainConfig::GetCommandFlow()
{
    return m_u1CommandFlow;
}

uint32 CMainConfig::GetSendDataMask()
{
    return m_u4SendDatamark;
}

uint32 CMainConfig::GetCoreFileSize()
{
    return m_u4CoreFileSize;
}

uint16 CMainConfig::GetTcpNodelay()
{
    return m_u2TcpNodelay;
}

uint16 CMainConfig::GetBacklog()
{
    return m_u2Backlog;
}

ENUM_CHAR_ORDER CMainConfig::GetCharOrder()
{
    return m_u1CharOrder;
}

uint16 CMainConfig::GetTrackIPCount()
{
    return m_u4TrackIPCount;
}

uint32 CMainConfig::GetCpuMax()
{
    return m_u4MaxCpu;
}

uint32 CMainConfig::GetMemoryMax()
{
    return m_u4MaxMemory;
}

uint8 CMainConfig::GetWTAI()
{
    return m_u1WTAI;
}

uint32 CMainConfig::GetWTCheckTime()
{
    return m_u4WTCheckTime;
}

uint32 CMainConfig::GetWTTimeoutCount()
{
    return m_u4WTTimeoutCount;
}

uint32 CMainConfig::GetWTStopTime()
{
    return m_u4WTStopTime;
}

uint8 CMainConfig::GetWTReturnDataType()
{
    return m_u1WTReturnDataType;
}

char* CMainConfig::GetWTReturnData()
{
    return (char* )m_szWTReturnData;
}

bool CMainConfig::GetByteOrder()
{
    return m_blByteOrder;
}

uint8 CMainConfig::GetDebugTrunOn()
{
    return m_u1DebugTrunOn;
}

char* CMainConfig::GetDebugFileName()
{
    return m_szDeubgFileName;
}

uint32 CMainConfig::GetChkInterval()
{
    return m_u4ChkInterval;
}

uint32 CMainConfig::GetLogFileMaxSize()
{
    return m_u4LogFileMaxSize;
}

uint32 CMainConfig::GetLogFileMaxCnt()
{
    return m_u4LogFileMaxCnt;
}

char* CMainConfig::GetDebugLevel()
{
    return m_szDebugLevel;
}

_ConnectAlert* CMainConfig::GetConnectAlert()
{
    return &m_ConnectAlert;
}

_IPAlert* CMainConfig::GetIPAlert()
{
    return &m_IPAlert;
}

_ClientDataAlert* CMainConfig::GetClientDataAlert()
{
    return &m_ClientDataAlert;
}

_CommandAlert* CMainConfig::GetCommandAlert(int32 nIndex)
{
    if(nIndex < 0 || nIndex >= (int32)m_vecCommandAlert.size())
    {
        return NULL;
    }
    else
    {
        return (_CommandAlert*)&m_vecCommandAlert[nIndex];
    }
}

uint32 CMainConfig::GetCommandAlertCount()
{
    return (uint32)m_vecCommandAlert.size();
}

_MailAlert* CMainConfig::GetMailAlert(uint32 u4MailID)
{
    for(uint32 i = 0; i < m_vecMailAlert.size(); i++)
    {
        if(m_vecMailAlert[i].m_u4MailID == u4MailID)
        {
            return (_MailAlert* )&m_vecMailAlert[i];
        }
    }

    return NULL;
}

void CMainConfig::SetMaxHandlerCount(uint16 u2MaxHandlerCount)
{
    m_u2MaxHanderCount = u2MaxHandlerCount;
}

_GroupListenInfo* CMainConfig::GetGroupListenInfo()
{
    return (_GroupListenInfo* )&m_GroupListenInfo;
}

_PacketParseInfo* CMainConfig::GetPacketParseInfo(uint8 u1Index)
{
    if(u1Index < (uint8)m_vecPacketParseInfo.size())
    {
        return &m_vecPacketParseInfo[u1Index];
    }
    else
    {
        return NULL;
    }
}

uint8 CMainConfig::GetPacketParseCount()
{
    return (uint8)m_vecPacketParseInfo.size();
}

uint16 CMainConfig::GetModuleInfoCount()
{
    return (uint16)m_vecModuleConfig.size();
}

_ModuleConfig* CMainConfig::GetModuleInfo(uint16 u2Index)
{
    if(u2Index >= (uint16)m_vecModuleConfig.size())
    {
        return NULL;
    }
    else
    {
        return (_ModuleConfig* )&m_vecModuleConfig[u2Index];
    }
}

uint32 CMainConfig::GetBlockSize()
{
    return m_u4BlockSize;
}

uint32 CMainConfig::GetBlockCount()
{
    return m_u4SendBlockCount;
}

uint8 CMainConfig::GetServerClose()
{
    return m_u1ServerClose;
}

uint32 CMainConfig::GetMaxCommandCount()
{
    return m_u4MaxCommandCount;
}

uint32 CMainConfig::GetServerConnectCount()
{
    return m_u4ServerConnectCount;
}

uint16 CMainConfig::GetMaxModuleCount()
{
    return m_u2MaxModuleCount;
}