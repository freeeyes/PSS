#pragma once

#include "define.h"
#include "XmlOpeation.h"

//main.xml配置枚举

/*1.枚举值以复数s结束的可能该类型有多个，并作为数组元素
2.需要增加枚举必须从 MIN到MAX区间范围添加
3.同文件的枚举必须相连*/

enum XmlStart
{
    XML_Config_MIN = 0
};

enum XmlConfig
{
    /*******************main.xml************************/

    /********网络设置********/
    XML_Config_RecvInfo = XML_Config_MIN,
    XML_Config_SendInfo,
    XML_Config_NetWorkMode,
    XML_Config_TCPServerIPs,
    XML_Config_UDPServerIPs,
    XML_Config_TTyDrives,
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
    XML_Config_Timer,
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

enum XmlEnd
{
    XML_Config_MAX = XML_Config_End
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

    bool InitIsOk()
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
};

typedef ACE_Singleton<XMainConfig, ACE_Null_Mutex> App_XmlConfig;

#define GetXmlConfigAttribute(XmlClass) App_XmlConfig::instance()->GetXmlConfig<XmlClass>()

class IConfigOpeation
{
    friend class XMainConfig;
public:
    const std::string& ClassName()
    {
        return m_name;
    }
    virtual ~IConfigOpeation() {}
protected:
    IConfigOpeation(XmlConfig config, const char* name)
        : m_name(name)
    {
        _array[config] = this;
    }
    virtual bool Init(CXmlOpeation* pXmlOperation) = 0;
private:
    std::string m_name;
    static IConfigOpeation* _array[XML_Config_MAX];
};



//接收数据包连接相关设置
class xmlRecvInfo : public IConfigOpeation
{
public:
    uint32 RecvBuffSize;
    uint16 RecvQueueTimeout;
    xmlRecvInfo(XmlConfig config, const char* name) : IConfigOpeation(config, name), RecvBuffSize(0), RecvQueueTimeout(0) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//发送数据包连接相关设置
class xmlSendInfo : public IConfigOpeation
{
public:
    uint16 SendQueueMax;//发送队列中最长的数据包个数
    uint16 TcpNodelay;
    uint32 MaxBlockSize;
    uint32 SendDatamark;
    uint32 BlockCount;
    uint16 SendTimeout;
    uint16 SendQueueTimeout;
    uint16 SendQueueCount;
    uint16 PutQueueTimeout;
    xmlSendInfo(XmlConfig config, const char* name) : IConfigOpeation(config, name), SendQueueMax(0), TcpNodelay(TCP_NODELAY_OFF), MaxBlockSize(0),
        SendDatamark(0), BlockCount(0), SendTimeout(0), SendQueueTimeout(0), SendQueueCount(0), PutQueueTimeout(0) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//网络模型
class xmlNetWorkMode : public IConfigOpeation
{
public:
    uint8 Mode;
    uint16 BackLog;
    bool NetByteOrder;
    ENUM_CHAR_ORDER LocalByteOrder;
    xmlNetWorkMode(XmlConfig config, const char* name) : IConfigOpeation(config, name), Mode(0),
        BackLog(0), NetByteOrder(false), LocalByteOrder(SYSTEM_BIG_ORDER) {}
    bool Init(CXmlOpeation* pXmlOperation);
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
        std::string ip;
        uint32 port;
        uint8 ipType;
        uint32 packetparseid;
        _TCPServerIP() : ip(""), port(0), ipType(0), packetparseid(0) {}
    };
    std::vector<_TCPServerIP> vec;
    xmlTCPServerIPs(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//Udp监听端口信息
class xmlUDPServerIPs : public IConfigOpeation
{
public:
    class _UDPServerIP
    {
    public:
        std::string uip;
        uint8 uipType;
        uint32 uport;
        uint32 uMaxRecvSize;
        uint32 uPacketParseID;
        _UDPServerIP() : uip("127.0.0.1"), uipType(0), uport(0), uMaxRecvSize(0), uPacketParseID(0) {}
    };
    std::vector<_UDPServerIP> vec;
    xmlUDPServerIPs(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//TTy设备端口监听
class xmlTTyDrives : public IConfigOpeation
{
public:
    class _TTyDrives
    {
    public:
        uint32 u4TTyID;
        uint32 u4PacketParseID;
        uint32 u4Baud;
        uint32 u4DataBits;
        uint32 u4StopBits;
        std::string strParity;
        std::string strPortName;
        std::string strDesc;
        _TTyDrives() : u4TTyID(0), u4PacketParseID(0), u4Baud(0), u4DataBits(0), u4StopBits(0), strParity(""), strPortName(""), strDesc("") {}
    };
    std::vector<_TTyDrives> vec;
    xmlTTyDrives(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//服务器间连接信息
class xmlConnectServer : public IConfigOpeation
{
public:
    uint32 TimeInterval;
    uint32 Recvbuff;
    uint8 RunType;
    uint16 TimeCheck;
    uint16 Count;
    xmlConnectServer(XmlConfig config, const char* name) : IConfigOpeation(config, name), TimeInterval(500), Recvbuff(1024), RunType(0), Count(100) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlClientInfo : public IConfigOpeation
{
public:
    uint32 HandlerCount;
    uint32 MaxHandlerCount;
    uint16 MaxConnectTime;
    uint16 CheckAliveTime;
    uint32 MaxBuffRecv;
    xmlClientInfo(XmlConfig config, const char* name) : IConfigOpeation(config, name), HandlerCount(5000), MaxHandlerCount(10000), MaxConnectTime(120),
        CheckAliveTime(60), MaxBuffRecv(1024) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlModuleInfos : public IConfigOpeation
{
public:
    class _ModuleInfo
    {
    public:
        std::string szModuleName;
        std::string szModulePath;
        std::string szModuleParam;
        _ModuleInfo() : szModuleName("TcpTest.dll"), szModulePath("./"), szModuleParam("") {}
    };
    std::vector<_ModuleInfo> vec;
    xmlModuleInfos(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlModuleMangager : public IConfigOpeation
{
public:
    uint32 MaxCount;
    xmlModuleMangager(XmlConfig config, const char* name) : IConfigOpeation(config, name), MaxCount(100) {}
    bool Init(CXmlOpeation* pXmlOperation);
};
class xmlMonitor : public IConfigOpeation
{
public:
    uint8 CpuAndMemory;
    uint16 CpuMax;
    uint32 MemoryMax;
    xmlMonitor(XmlConfig config, const char* name) : IConfigOpeation(config, name), CpuAndMemory(1), CpuMax(50), MemoryMax(1000) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlThreadInfoAI : public IConfigOpeation
{
public:
    uint8 AI;
    uint8 ReturnDataType;
    uint16 CheckTime;
    uint16 TimeoutCount;
    uint16 StopTime;
    std::string ReturnData;
    xmlThreadInfoAI(XmlConfig config, const char* name) : IConfigOpeation(config, name), AI(0), ReturnDataType(1), CheckTime(30),
        TimeoutCount(1), StopTime(30), ReturnData("ff ff") {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlThreadInfo : public IConfigOpeation
{
public:
    uint16 ThreadTimeout;
    uint16 ThreadTimeCheck;
    uint16 DisposeTimeout;
    uint16 PutQueueTimeout;
    xmlThreadInfo(XmlConfig config, const char* name) : IConfigOpeation(config, name), ThreadTimeout(30), ThreadTimeCheck(60),
        DisposeTimeout(40), PutQueueTimeout(100) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlConsole : public IConfigOpeation
{
public:
    uint8 support;
    std::string sip;
    uint16 sport;
    uint8 ipType;
    xmlConsole(XmlConfig config, const char* name) : IConfigOpeation(config, name), support(1), sip("INADDR_ANY"), sport(10010), ipType(0) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlConsoleKeys : public IConfigOpeation
{
public:
    class _ConsoleKey
    {
    public:
        std::string Key;
        _ConsoleKey() : Key("not") {}
    };
    std::vector<_ConsoleKey> vec;
    xmlConsoleKeys(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlConsoleClients : public IConfigOpeation
{
public:
    class _ConsoleClient
    {
    public:
        std::string cip;
        _ConsoleClient() : cip("") {}
    };
    std::vector<_ConsoleClient> vec;
    xmlConsoleClients(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlAceDebug : public IConfigOpeation
{
public:
    uint8 TrunOn;
    std::string DebugName;
    uint16 ChkInterval;
    uint32 LogFileMaxSize;
    uint8 LogFileMaxCnt;
    std::string Level;
    xmlAceDebug(XmlConfig config, const char* name) : IConfigOpeation(config, name), TrunOn(0), DebugName("./serverdebug.log"), ChkInterval(600),
        LogFileMaxSize(10240), LogFileMaxCnt(3), Level("") {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlCommandAccount : public IConfigOpeation
{
public:
    uint8 Account;
    uint8 FlowAccount;
    uint32 MaxCommandCount;
    xmlCommandAccount(XmlConfig config, const char* name) : IConfigOpeation(config, name), Account(1), FlowAccount(1), MaxCommandCount(2000) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlCoreSetting : public IConfigOpeation
{
public:
    uint8 CoreNeed;
    std::string Script;
    xmlCoreSetting(XmlConfig config, const char* name) : IConfigOpeation(config, name), CoreNeed(0), Script("") {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlServerType : public IConfigOpeation
{
public:
    uint8 Type;
    std::string name;
    std::string displayname;
    uint8 Debug;
    uint32 DebugSize;
    uint8 IsClose;
    xmlServerType(XmlConfig config, const char* name) : IConfigOpeation(config, name), Type(0), name("Pss Service"),
        displayname("PssService"), Debug(0), DebugSize(1000), IsClose(0) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlServerID : public IConfigOpeation
{
public:
    uint32 id;
    xmlServerID(XmlConfig config, const char* name) : IConfigOpeation(config, name), id(1001) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlServerName : public IConfigOpeation
{
public:
    std::string name;
    xmlServerName(XmlConfig config, const char* name) : IConfigOpeation(config, name), name("Freeeyes") {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlServerVersion : public IConfigOpeation
{
public:
    std::string Version;
    xmlServerVersion(XmlConfig config, const char* name) : IConfigOpeation(config, name), Version("1.00") {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlPacketParses : public IConfigOpeation
{
public:
    class _PacketParse
    {
    public:
        uint16 ParseID;
        std::string ModulePath;
        std::string ModuleName;
        uint8 Type;
        uint16 OrgLength;
        _PacketParse() : ParseID(1), ModulePath("./"),
            ModuleName("PacketParse_Interface.dll"), Type(PACKET_WITHHEAD), OrgLength(40) {}
    };
    std::vector<_PacketParse> vec;
    xmlPacketParses(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlBuffPacket : public IConfigOpeation
{
public:
    uint32 Count;
    xmlBuffPacket(XmlConfig config, const char* name) : IConfigOpeation(config, name), Count(5000) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlMessage : public IConfigOpeation
{
public:
    uint32 Msg_High_mark;
    uint32 Msg_Low_mark;
    uint32 Msg_Buff_Max_Size;
    uint16 Msg_Thread;
    uint16 Msg_Process;
    uint32 Msg_MaxQueue;
    xmlMessage(XmlConfig config, const char* name) : IConfigOpeation(config, name), Msg_High_mark(64000), Msg_Low_mark(64000), Msg_Buff_Max_Size(20480),
        Msg_Thread(5), Msg_Process(1), Msg_MaxQueue(10000) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//TTy相关管理配置
class xmlTTyClientManagerInfo : public IConfigOpeation
{
public:
    uint16 MaxTTyDevCount;
    uint16 TimeCheck;
    xmlTTyClientManagerInfo(XmlConfig config, const char* name) : IConfigOpeation(config, name), MaxTTyDevCount(MAX_BUFF_100), TimeCheck(120) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

/****************************alert.xml*****************************/
class xmlAlertConnect : public IConfigOpeation
{
public:
    uint8 ConnectMin;
    uint32 ConnectMax;
    uint16 DisConnectMin;
    uint32 DisConnectMax;
    uint32 ConnectAlert;
    uint32 MailID;
    xmlAlertConnect(XmlConfig config, const char* name) : IConfigOpeation(config, name), ConnectMin(0), ConnectMax(0), DisConnectMin(0),
        DisConnectMax(0), ConnectAlert(20000), MailID(0) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlIP : public IConfigOpeation
{
public:
    uint8 IPMax;
    uint32 Timeout;
    xmlIP(XmlConfig config, const char* name) : IConfigOpeation(config, name), IPMax(0), Timeout(300) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlClientData : public IConfigOpeation
{
public:
    uint16 RecvPacketCount;
    uint32 RecvDataMax;
    uint16 SendPacketCount;
    uint32 SendDataMax;
    uint32 MailID;
    xmlClientData(XmlConfig config, const char* name) : IConfigOpeation(config, name), RecvPacketCount(0), RecvDataMax(0), SendPacketCount(0),
        SendDataMax(0), MailID(0) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlCommandInfos : public IConfigOpeation
{
public:
    class _CommandInfo
    {
    public:
        uint32 CommandID;
        uint32 CommandCount;
        uint32 MailID;
        _CommandInfo() : CommandID(4096), CommandCount(0), MailID(0) {}
    };
    std::vector<_CommandInfo> vec;
    xmlCommandInfos(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlMails : public IConfigOpeation
{
public:
    class _Mail
    {
    public:
        uint16 MailID;
        std::string fromMailAddr;
        std::string toMailAddr;
        std::string MailPass;
        std::string MailUrl;
        uint16 MailPort;
        _Mail() : MailID(1), fromMailAddr("local@163.com"), toMailAddr("freeeyes@163.com"),
            MailPass("123456"), MailUrl("smtp.163.com"), MailPort(25) {}
    };
    std::vector<_Mail> vec;
    xmlMails(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);

    //根据指定的邮箱ID查找邮箱配置信息
    _Mail* GetMailAlert(uint16 MailID);
};

class xmlCommandsTimeout : public IConfigOpeation
{
public:
    class _CommandsTimeout
    {
    public:
        uint16 CommandID;
        uint16 Timeout;
        _CommandsTimeout() : CommandID(0), Timeout(0)
        {}
    };
    std::vector<_CommandsTimeout> vec;
    xmlCommandsTimeout(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);

    //根据指定的邮箱ID查找邮箱配置信息
    _CommandsTimeout* GetCommandAlert(uint16 CommandID);
};

class xmlTSTimer : public IConfigOpeation
{
public:
    uint16 TimerListCount;
    xmlTSTimer(XmlConfig config, const char* name) : IConfigOpeation(config, name), TimerListCount(10)
    {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlTcpRedirection : public IConfigOpeation
{
public:
    class _RedirectionInfo
    {
    public:
        uint32 SrcPort;
        string RedirectionIP;
        uint32 RedirectionPort;
        uint8  Mode;
        uint8  ConnectState;
        _RedirectionInfo() : SrcPort(0), RedirectionIP(""), RedirectionPort(0), Mode(0), ConnectState(0) {}
    };
    std::vector<_RedirectionInfo> vec;
    xmlTcpRedirection(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};


