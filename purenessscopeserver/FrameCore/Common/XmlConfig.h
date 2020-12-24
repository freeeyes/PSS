#pragma once

#include "define.h"
#include "XmlOpeation.h"
#include <array>

//main.xml配置枚举

/*1.枚举值以复数s结束的可能该类型有多个，并作为数组元素
2.需要增加枚举必须从 MIN到MAX区间范围添加
3.同文件的枚举必须相连*/

enum class XmlStart
{
    XML_Config_MIN = 0
};

enum class XmlConfig : uint8
{
    /*******************main.xml************************/

    /********网络设置********/
    XML_Config_RecvInfo = static_cast<int>(XmlStart::XML_Config_MIN),
    XML_Config_SendInfo,
    XML_Config_NetWorkMode,
    XML_Config_TCPServerIPs,
    XML_Config_UDPServerIPs,
    XML_Config_TTyDrives,
    XML_Config_Server2Servers,
    /********连接信息********/
    XML_Config_ConnectServer,
    XML_Config_ClientInfo,
    XML_Config_TTyClientManager,
    /********模块设置********/
    XML_Config_ModuleInfos,
    XML_Config_ModuleMangager,
    /********监控信息********/
    XML_Config_Monitor,
    XML_Config_ThreadInfoAI,
    XML_Config_ThreadInfo,
    XML_Config_Console,
    XML_Config_ConsoleKeys,
    XML_Config_ConsoleClients,
    /********文件记录********/
    XML_Config_AceDebug,
    XML_Config_CommandAccount,
    XML_Config_CoreSetting,
    /********服务器属性********/
    XML_Config_ServerType,
    XML_Config_ServerID,
    XML_Config_ServerName,
    XML_Config_ServerVersion,
    /********消息设置********/
    XML_Config_PacketParses,
    XML_Config_BuffPacket,
    XML_Config_Message,
    XML_Config_Redirection,

    /****************alert.xml*****************/
    XML_Config_AlertConnect,
    XML_Config_IP,
    XML_Config_ClientData,
    XML_Config_CommandInfos,
    XML_Config_Mails,
    XML_Config_Commands_Timeout,

    /*枚举结束*/
    XML_Config_End
};

class IConfigOpeation;

//MianConfig主类
class XMainConfig
{
public:
    XMainConfig()
    {
        _initIsOk = Init();
    }

    bool InitIsOk() const
    {
        return _initIsOk;
    }
    bool Init();

    template<class T>
    T* GetXmlConfig();
private:
    bool _initIsOk;
    bool InitFile(const char* pFileName, XmlConfig start, XmlConfig end);

    CXmlOpeation m_XmlOperation;
    std::array<IConfigOpeation*, static_cast<int>(XmlConfig::XML_Config_End)> _array;
};

using App_XmlConfig = PSS_singleton<XMainConfig>;

#define GetXmlConfigAttribute(XmlClass) App_XmlConfig::instance()->GetXmlConfig<XmlClass>()

class IConfigOpeation
{
    friend class XMainConfig;
public:
    std::string ClassName() const
    {
        return m_name;
    }

    XmlConfig GetArrayIndex() const
    {
        return m_emconfig;
    }

    virtual ~IConfigOpeation() = default;
protected:
    IConfigOpeation(XmlConfig config, const char* name) : m_name(name), m_emconfig(config)
    {
    }
    virtual bool Init(CXmlOpeation* pXmlOperation) = 0;
private:
    std::string m_name;
    XmlConfig m_emconfig = XmlConfig::XML_Config_End;
};



//接收数据包连接相关设置
class xmlRecvInfo : public IConfigOpeation
{
public:
    uint32 RecvBuffSize     = 0;
    uint16 RecvQueueTimeout = 0;
    xmlRecvInfo(XmlConfig config, const char* name) : IConfigOpeation::IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

//发送数据包连接相关设置
class xmlSendInfo : public IConfigOpeation
{
public:
    uint16 TcpNodelay       = TCP_NODELAY_OFF;
    uint32 MaxBlockSize     = 0;
    xmlSendInfo(XmlConfig config, const char* name) : IConfigOpeation::IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

//网络模型
class xmlNetWorkMode : public IConfigOpeation
{
public:
    NETWORKMODE Mode               = NETWORKMODE::NETWORKMODE_RE_EPOLL;
    uint16 BackLog                 = 0;
    bool NetByteOrder              = false;
    uint16 ThreadCount             = 1;
    ENUM_CHAR_ORDER LocalByteOrder = ENUM_CHAR_ORDER::SYSTEM_BIG_ORDER;
    xmlNetWorkMode(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
private:
    bool SetIOMode(const std::string& pData);
    void SetLocalByteOrder();
    void SetNetByteOrder(const std::string& pData);
};

//Tcp监听服务器列表
class xmlTCPServerIPs : public IConfigOpeation
{
public:
    class _TCPServerIP
    {
    public:
        std::string ip       = "";
        uint16 port          = 0;
        uint8 ipType         = 0;
        uint32 packetparseid = 0;
        _TCPServerIP() = default;
    };
    std::vector<_TCPServerIP> vec;
    xmlTCPServerIPs(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

//Udp监听端口信息
class xmlUDPServerIPs : public IConfigOpeation
{
public:
    class _UDPServerIP
    {
    public:
        std::string uip       = "127.0.0.1";
        uint8 uipType         = 0;
        uint16 uport          = 0;
        uint32 uMaxRecvSize   = 0;
        uint32 uPacketParseID = 0;
        _UDPServerIP() = default;
    };
    std::vector<_UDPServerIP> vec;
    xmlUDPServerIPs(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

//TTy设备端口监听
class xmlTTyDrives : public IConfigOpeation
{
public:
    class _TTyDrives
    {
    public:
        uint32 u4TTyID         = 0;
        uint32 u4PacketParseID = 0;
        uint32 u4Baud          = 0;
        uint32 u4DataBits      = 0;
        uint32 u4StopBits      = 0;
        std::string strParity;
        std::string strPortName;
        std::string strDesc;
        _TTyDrives() = default;
    };
    std::vector<_TTyDrives> vec;
    xmlTTyDrives(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

//服务器间通讯端口设置
class xmlServer2Server : public IConfigOpeation
{
public:
    class _Server2Server
    {
    public:
        uint32 u4ServerID       = 0;
        std::string strServerIP;
        uint16 u2ServerPort     = 0;
        uint32 u4PacketParseID  = 0;
        _Server2Server() = default;
    };
    std::vector<_Server2Server> vec;
    xmlServer2Server(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

//服务器间连接信息
class xmlConnectServer : public IConfigOpeation
{
public:
    uint32 TimeInterval = MAX_BUFF_500;
    uint32 Recvbuff     = MAX_BUFF_1024;
    uint8  RunType      = 0;
    uint16 TimeCheck    = 0;
    uint16 Count        = MAX_BUFF_100;
    xmlConnectServer(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlClientInfo : public IConfigOpeation
{
public:
    uint32 HandlerCount     = 5000;
    uint32 MaxHandlerCount  = 10000;
    uint16 MaxConnectTime   = 120;
    uint16 CheckAliveTime   = 60;
    uint32 MaxBuffRecv      = 1024;
    xmlClientInfo(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlModuleInfos : public IConfigOpeation
{
public:
    class _ModuleInfo
    {
    public:
        std::string szModuleName  = "TcpTest.dll";
        std::string szModulePath  = "./";
        std::string szModuleParam;
        _ModuleInfo() = default;
    };
    std::vector<_ModuleInfo> vec;
    xmlModuleInfos(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlModuleMangager : public IConfigOpeation
{
public:
    uint16 MaxCount = MAX_BUFF_100;
    xmlModuleMangager(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};
class xmlMonitor : public IConfigOpeation
{
public:
    uint8 CpuAndMemory = 1;
    uint16 CpuMax      = 50;
    uint32 MemoryMax   = 1000;
    xmlMonitor(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlThreadInfoAI : public IConfigOpeation
{
public:
    uint8 AI               = 0;
    uint8 ReturnDataType   = 1;
    uint16 CheckTime       = 30;
    uint16 TimeoutCount    = 1;
    uint16 StopTime        = 30;
    std::string ReturnData = "ff ff";
    xmlThreadInfoAI(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlThreadInfo : public IConfigOpeation
{
public:
    uint16 ThreadTimeout   = 30;
    uint16 ThreadTimeCheck = 60;
    uint16 DisposeTimeout  = 40;
    uint16 PutQueueTimeout = 100;
    xmlThreadInfo(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlConsole : public IConfigOpeation
{
public:
    uint8 support   = 0;
    std::string sip = "INADDR_ANY";
    uint16 sport    = 10010;
    uint8 ipType    = 0;
    xmlConsole(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlConsoleKeys : public IConfigOpeation
{
public:
    class _ConsoleKey
    {
    public:
        std::string Key = "not";
        _ConsoleKey() = default;
    };
    std::vector<_ConsoleKey> vec;
    xmlConsoleKeys(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlConsoleClients : public IConfigOpeation
{
public:
    class _ConsoleClient
    {
    public:
        std::string cip;
        _ConsoleClient() = default;
    };
    std::vector<_ConsoleClient> vec;
    xmlConsoleClients(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlAceDebug : public IConfigOpeation
{
public:
    uint8 TrunOn          = 0;
    std::string ConsoleName = "./consoleoutput";
    uint32 LogFileMaxSize = 10240000;
    uint16 FileCount = 3;
    std::string Level = "info";
    xmlAceDebug(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlCommandAccount : public IConfigOpeation
{
public:
    uint8 Account          = 1;
    uint8 FlowAccount      = 1;
    uint32 MaxCommandCount = 2000;
    xmlCommandAccount(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlCoreSetting : public IConfigOpeation
{
public:
    uint8 CoreNeed = 0;
    std::string Script;
    xmlCoreSetting(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlServerType : public IConfigOpeation
{
public:
    uint8 Type              = 0;
    std::string name        = "Pss Service";
    std::string displayname = "PssService";
    uint8 Debug             = 0;
    uint32 DebugSize        = 1000;
    uint8 IsClose           = 0;
    xmlServerType(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlServerID : public IConfigOpeation
{
public:
    uint32 id = 1001;
    xmlServerID(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlServerName : public IConfigOpeation
{
public:
    std::string name = "freeeyes";
    xmlServerName(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlServerVersion : public IConfigOpeation
{
public:
    std::string Version = "1.00";
    xmlServerVersion(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlPacketParses : public IConfigOpeation
{
public:
    class _PacketParse
    {
    public:
        uint16 ParseID         = 1;
        std::string ModulePath = "./";
        std::string ModuleName = "PacketParse_Interface.dll";
        uint8 Type             = PACKET_WITHHEAD;
        uint16 OrgLength       = 40;
        _PacketParse() = default;
    };
    std::vector<_PacketParse> vec;
    xmlPacketParses(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlBuffPacket : public IConfigOpeation
{
public:
    uint32 Count = 5000;
    xmlBuffPacket(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlMessage : public IConfigOpeation
{
public:
    uint32 Msg_Low_mark      = 64000;
    uint32 Msg_Buff_Max_Size = 20480;
    uint16 Msg_Thread        = 3;
    uint16 Msg_Process       = 1;
    uint32 Msg_MaxQueue      = 10000;
    uint8  Packet_Counter    = 1;
    xmlMessage(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

//TTy相关管理配置
class xmlTTyClientManagerInfo : public IConfigOpeation
{
public:
    uint16 MaxTTyDevCount = MAX_BUFF_100;
    uint16 TimeCheck      = 120;
    xmlTTyClientManagerInfo(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

/****************************alert.xml*****************************/
class xmlAlertConnect : public IConfigOpeation
{
public:
    uint8 ConnectMin     = 0;
    uint32 ConnectMax    = 0;
    uint16 DisConnectMin = 0;
    uint32 DisConnectMax = 0;
    uint32 ConnectAlert  = 20000;
    uint16 MailID        = 0;
    xmlAlertConnect(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlIP : public IConfigOpeation
{
public:
    uint8 IPMax    = 0;
    uint32 Timeout = 300;
    xmlIP(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlClientData : public IConfigOpeation
{
public:
    uint16 RecvPacketCount = 0;
    uint32 RecvDataMax     = 0;
    uint16 SendPacketCount = 0;
    uint32 SendDataMax     = 0;
    uint32 MailID          = 0;
    xmlClientData(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlCommandInfos : public IConfigOpeation
{
public:
    class _CommandInfo
    {
    public:
        uint16 CommandID    = 4096;
        uint32 CommandCount = 0;
        uint16 MailID       = 0;
        _CommandInfo() = default;
    };
    std::vector<_CommandInfo> vec;
    xmlCommandInfos(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};

class xmlMails : public IConfigOpeation
{
public:
    class _Mail
    {
    public:
        uint16 MailID            = 1;
        std::string fromMailAddr = "local@163.com";
        std::string toMailAddr = "freeeyes@163.com";
        std::string MailPass     = "123456";
        std::string MailUrl      = "smtp.163.com";
        uint16 MailPort          = 25;
        _Mail() = default;
    };
    std::vector<_Mail> vec;
    xmlMails(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;

    //根据指定的邮箱ID查找邮箱配置信息
    _Mail* GetMailAlert(uint16 MailID);
};

class xmlCommandsTimeout : public IConfigOpeation
{
public:
    class _CommandsTimeout
    {
    public:
        uint16 CommandID = 0;
        uint16 Timeout   = 0;
        _CommandsTimeout() = default;
    };
    std::vector<_CommandsTimeout> vec;
    xmlCommandsTimeout(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;

    //根据指定的邮箱ID查找邮箱配置信息
    _CommandsTimeout* GetCommandAlert(uint16 CommandID);
};

class xmlTcpRedirection : public IConfigOpeation
{
public:
    class _RedirectionInfo
    {
    public:
        uint32 SrcPort         = 0;
        string RedirectionIP;
        uint32 RedirectionPort = 0;
        uint8  Mode            = 0;
        uint8  ConnectState    = 0;
        _RedirectionInfo() = default;
    };
    std::vector<_RedirectionInfo> vec;
    xmlTcpRedirection(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation) final;
};


