#pragma once

#include "define.h"
#include "XmlOpeation.h"

//main.xml����ö��

/*1.ö��ֵ�Ը���s�����Ŀ��ܸ������ж��������Ϊ����Ԫ��
2.��Ҫ����ö�ٱ���� MIN��MAX���䷶Χ���
3.ͬ�ļ���ö�ٱ�������*/

enum class XmlStart
{
    XML_Config_MIN = 0
};

enum class XmlConfig
{
    /*******************main.xml************************/

    /********��������********/
    XML_Config_RecvInfo = static_cast<int>(XmlStart::XML_Config_MIN),
    XML_Config_SendInfo,
    XML_Config_NetWorkMode,
    XML_Config_TCPServerIPs,
    XML_Config_UDPServerIPs,
    XML_Config_TTyDrives,
    XML_Config_Server2Servers,
    /********������Ϣ********/
    XML_Config_ConnectServer,
    XML_Config_ClientInfo,
    XML_Config_TTyClientManager,
    /********ģ������********/
    XML_Config_ModuleInfos,
    XML_Config_ModuleMangager,
    /********�����Ϣ********/
    XML_Config_Monitor,
    XML_Config_ThreadInfoAI,
    XML_Config_ThreadInfo,
    XML_Config_Console,
    XML_Config_ConsoleKeys,
    XML_Config_ConsoleClients,
    /********�ļ���¼********/
    XML_Config_AceDebug,
    XML_Config_CommandAccount,
    XML_Config_CoreSetting,
    /********����������********/
    XML_Config_ServerType,
    XML_Config_ServerID,
    XML_Config_ServerName,
    XML_Config_ServerVersion,
    /********��Ϣ����********/
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

    /*ö�ٽ���*/
    XML_Config_End
};

enum XmlEnd
{
    XML_Config_MAX = static_cast<int>(XmlConfig::XML_Config_End)
};

class IConfigOpeation;

//MianConfig����
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
};

typedef ACE_Singleton<XMainConfig, ACE_Null_Mutex> App_XmlConfig;

#define GetXmlConfigAttribute(XmlClass) App_XmlConfig::instance()->GetXmlConfig<XmlClass>()

class IConfigOpeation
{
    friend class XMainConfig;
public:
    const std::string& ClassName() const
    {
        return m_name;
    }
    virtual ~IConfigOpeation() {}
protected:
    IConfigOpeation(XmlConfig config, const char* name) : m_name(name)
    {
        _array[static_cast<int>(config)] = this;
    }
    virtual bool Init(CXmlOpeation* pXmlOperation) = 0;
private:
    std::string m_name;
    static IConfigOpeation* _array[static_cast<int>(XmlEnd::XML_Config_MAX)];
};



//�������ݰ������������
class xmlRecvInfo : public IConfigOpeation
{
public:
    uint32 RecvBuffSize     = 0;
    uint16 RecvQueueTimeout = 0;
    xmlRecvInfo(XmlConfig config, const char* name) : IConfigOpeation::IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//�������ݰ������������
class xmlSendInfo : public IConfigOpeation
{
public:
    uint16 SendQueueMax     = 0;//���Ͷ�����������ݰ�����
    uint16 TcpNodelay       = TCP_NODELAY_OFF;
    uint32 MaxBlockSize     = 0;
    uint32 SendDatamark     = 0;
    uint32 BlockCount       = 0;
    uint16 SendTimeout      = 0;
    uint16 SendQueueTimeout = 0;
    uint16 SendQueueCount   = 0;
    uint16 PutQueueTimeout  = 0;
    xmlSendInfo(XmlConfig config, const char* name) : IConfigOpeation::IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//����ģ��
class xmlNetWorkMode : public IConfigOpeation
{
public:
    NETWORKMODE Mode               = NETWORKMODE::NETWORKMODE_RE_EPOLL;
    uint16 BackLog                 = 0;
    bool NetByteOrder              = false;
    ENUM_CHAR_ORDER LocalByteOrder = ENUM_CHAR_ORDER::SYSTEM_BIG_ORDER;
    xmlNetWorkMode(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
private:
    bool SetIOMode(const std::string& pData);
    void SetLocalByteOrder();
    void SetNetByteOrder(const std::string& pData);
};

//Tcp�����������б�
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
        _TCPServerIP() {}
    };
    std::vector<_TCPServerIP> vec;
    xmlTCPServerIPs(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//Udp�����˿���Ϣ
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
        _UDPServerIP() {}
    };
    std::vector<_UDPServerIP> vec;
    xmlUDPServerIPs(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//TTy�豸�˿ڼ���
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
        _TTyDrives() {}
    };
    std::vector<_TTyDrives> vec;
    xmlTTyDrives(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//��������ͨѶ�˿�����
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
        _Server2Server() {}
    };
    std::vector<_Server2Server> vec;
    xmlServer2Server(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//��������������Ϣ
class xmlConnectServer : public IConfigOpeation
{
public:
    uint32 TimeInterval = MAX_BUFF_500;
    uint32 Recvbuff     = MAX_BUFF_1024;
    uint8  RunType      = 0;
    uint16 TimeCheck    = 0;
    uint16 Count        = MAX_BUFF_100;
    xmlConnectServer(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
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
    bool Init(CXmlOpeation* pXmlOperation);
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
        _ModuleInfo() {}
    };
    std::vector<_ModuleInfo> vec;
    xmlModuleInfos(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlModuleMangager : public IConfigOpeation
{
public:
    uint16 MaxCount = MAX_BUFF_100;
    xmlModuleMangager(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};
class xmlMonitor : public IConfigOpeation
{
public:
    uint8 CpuAndMemory = 1;
    uint16 CpuMax      = 50;
    uint32 MemoryMax   = 1000;
    xmlMonitor(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
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
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlThreadInfo : public IConfigOpeation
{
public:
    uint16 ThreadTimeout   = 30;
    uint16 ThreadTimeCheck = 60;
    uint16 DisposeTimeout  = 40;
    uint16 PutQueueTimeout = 100;
    xmlThreadInfo(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlConsole : public IConfigOpeation
{
public:
    uint8 support   = 0;
    std::string sip = "INADDR_ANY";
    uint16 sport    = 10010;
    uint8 ipType    = 0;
    xmlConsole(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlConsoleKeys : public IConfigOpeation
{
public:
    class _ConsoleKey
    {
    public:
        std::string Key = "not";
        _ConsoleKey() {}
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
        _ConsoleClient() {}
    };
    std::vector<_ConsoleClient> vec;
    xmlConsoleClients(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlAceDebug : public IConfigOpeation
{
public:
    uint8 TrunOn          = 0;
    std::string DebugPath = "./";
    std::string DebugName = "serverdebug";
    uint32 LogFileMaxSize = 10240;
    std::string Level;
    xmlAceDebug(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlCommandAccount : public IConfigOpeation
{
public:
    uint8 Account          = 1;
    uint8 FlowAccount      = 1;
    uint32 MaxCommandCount = 2000;
    xmlCommandAccount(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlCoreSetting : public IConfigOpeation
{
public:
    uint8 CoreNeed = 0;
    std::string Script;
    xmlCoreSetting(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
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
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlServerID : public IConfigOpeation
{
public:
    uint32 id = 1001;
    xmlServerID(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlServerName : public IConfigOpeation
{
public:
    std::string name = "freeeyes";
    xmlServerName(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlServerVersion : public IConfigOpeation
{
public:
    std::string Version = "1.00";
    xmlServerVersion(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
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
        _PacketParse() {}
    };
    std::vector<_PacketParse> vec;
    xmlPacketParses(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlBuffPacket : public IConfigOpeation
{
public:
    uint32 Count = 5000;
    xmlBuffPacket(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlMessage : public IConfigOpeation
{
public:
    uint32 Msg_Low_mark      = 64000;
    uint32 Msg_Buff_Max_Size = 20480;
    uint16 Msg_Thread        = 3;
    uint16 Msg_Process       = 1;
    uint32 Msg_MaxQueue      = 10000;
    uint32 Msg_Pool          = 5000;
    xmlMessage(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};

//TTy��ع�������
class xmlTTyClientManagerInfo : public IConfigOpeation
{
public:
    uint16 MaxTTyDevCount = MAX_BUFF_100;
    uint16 TimeCheck      = 120;
    xmlTTyClientManagerInfo(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
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
    bool Init(CXmlOpeation* pXmlOperation);
};

class xmlIP : public IConfigOpeation
{
public:
    uint8 IPMax    = 0;
    uint32 Timeout = 300;
    xmlIP(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
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
    bool Init(CXmlOpeation* pXmlOperation);
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
        _CommandInfo() {}
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
        uint16 MailID            = 1;
        std::string fromMailAddr = "local@163.com";
        std::string toMailAddr = "freeeyes@163.com";
        std::string MailPass     = "123456";
        std::string MailUrl      = "smtp.163.com";
        uint16 MailPort          = 25;
        _Mail() {}
    };
    std::vector<_Mail> vec;
    xmlMails(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);

    //����ָ��������ID��������������Ϣ
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
        _CommandsTimeout()
        {}
    };
    std::vector<_CommandsTimeout> vec;
    xmlCommandsTimeout(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);

    //����ָ��������ID��������������Ϣ
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
        _RedirectionInfo() {}
    };
    std::vector<_RedirectionInfo> vec;
    xmlTcpRedirection(XmlConfig config, const char* name) : IConfigOpeation(config, name) {}
    bool Init(CXmlOpeation* pXmlOperation);
};


