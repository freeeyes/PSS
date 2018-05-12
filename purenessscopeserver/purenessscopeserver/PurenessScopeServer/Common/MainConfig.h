#ifndef _MAINCONFIG_H
#define _MAINCONFIG_H

#include "define.h"
#include "XmlOpeation.h"

#include "ace/Singleton.h"
#include <vector>

#include "PacketParse.h"
#include "IpCheck.h"

//main.xml配置枚举

/*1.枚举值以复数s结束的可能该类型有多个，并作为数组元素
  2.需要增加枚举必须从 MIN到MAX区间范围添加
  3.同文件的枚举必须相连*/

enum XmlStart
{
	MIN = 0
};

enum XmlConfig
{
/*******************main.xml************************/

	/********网络设置********/
	RecvInfo = MIN,
	SendInfo,
	NetWorkMode,
	/********连接信息********/
	ConnectServer,
	ClientInfo,
	/********模块设置********/
	ModuleInfos,
	ModuleMangager,
	/********监控信息********/
	Monitor,
	ThreadInfoAI,
	ThreadInfo,
	Console,
	ConsoleKey,
	/********文件记录********/
	AceDebug,
	CommandAccount,
	CoreSetting,
	/********服务器属性********/
	ServerType,
	ServerID,
	ServerName,
	ServerVersion,
	/********消息设置********/
	PacketParses,
	BuffPacket,
	Message,

/****************alert.xml*****************/
	AlertConnect,
	IP,
	ClientData,
	CommandInfo,
	Mail,
	WorkThreadChart,
	ConnectChart,
	CommandChart,

	/*枚举结束*/
	End
};

enum XmlEnd
{
	MAX = XmlConfig::End
};

class IConfigOpeation;

//MianConfig主类
class XMainConfig
{
public:
	bool Init();
	template<class T>
	T* GetXmlConfig(XmlConfig config) { return dynamic_cast<T*>(IConfigOpeation::_array[config]); }
private:
	bool Init(const char* pFileName, XmlConfig start, XmlConfig end);
	CXmlOpeation m_XmlOpeation;
};

typedef ACE_Singleton<XMainConfig, ACE_Null_Mutex> App_XmlConfig;


class IConfigOpeation
{
	friend class XMainConfig;
protected:
	IConfigOpeation(XmlConfig config) { _array[config] = this;}
	virtual bool Init(CXmlOpeation* pXmlOpeation) = 0;
private:
	static IConfigOpeation* _array[XmlEnd::MAX];
};


//接收数据包连接相关设置
class xmlRecvInfo : public IConfigOpeation
{
public:
	uint32 RecvBuffSize;
	uint16 RecvQueueTimeout;
	xmlRecvInfo() : IConfigOpeation(RecvInfo), RecvBuffSize(0), RecvQueueTimeout(0){}
	bool Init(CXmlOpeation* pXmlOpeation);
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
	xmlSendInfo() : IConfigOpeation(SendInfo), SendQueueMax(0), TcpNodelay(TCP_NODELAY_OFF), MaxBlockSize(0),
		SendDatamark(0), BlockCount(0), SendTimeout(0), SendQueueTimeout(0), SendQueueCount(0), PutQueueTimeout(0){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

//网络模型
class xmlNetWorkMode : public IConfigOpeation
{
public:
	uint8 Mode;
	uint16 BackLog;
	uint8 ByteOrder;
	xmlNetWorkMode() : IConfigOpeation(NetWorkMode), Mode(0), BackLog(0), ByteOrder(false){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlConnectServer : public IConfigOpeation
{
public:
	uint32 TimeInterval;
	uint32 Recvbuff;
	uint8 RunType;
	uint16 TimeCheck;
	uint16 Count;
	xmlConnectServer() : IConfigOpeation(ConnectServer), TimeInterval(500), Recvbuff(1024), RunType(0), Count(100){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlClientInfo : public IConfigOpeation
{
public:
	uint32 HandlerCount;
	uint32 MaxHandlerCount;
	uint16 MaxConnectTime;
	uint16 CheckAliveTime;
	uint32 MaxBuffRecv;
	xmlClientInfo() : IConfigOpeation(ClientInfo), HandlerCount(5000), MaxHandlerCount(10000), MaxConnectTime(120), 
		CheckAliveTime(60), MaxBuffRecv(1024){}
	bool Init(CXmlOpeation* pXmlOpeation);
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
		_ModuleInfo() : szModuleName("TcpTest.dll"), szModulePath("./"), szModuleParam(""){}
	};
	std::vector<_ModuleInfo> vec;
	xmlModuleInfos() : IConfigOpeation(ModuleInfos) {}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlModuleMangager : public IConfigOpeation
{
public:
	uint32 MaxCount;
	xmlModuleMangager() : IConfigOpeation(ModuleMangager), MaxCount(100) {}
	bool Init(CXmlOpeation* pXmlOpeation);
};
class xmlMonitor : public IConfigOpeation
{
public:
	uint8 CpuAndMemory;
	uint16 CpuMax;
	uint32 MemoryMax;
	xmlMonitor() : IConfigOpeation(Monitor), CpuAndMemory(1), CpuMax(50), MemoryMax(1000){}
	bool Init(CXmlOpeation* pXmlOpeation);
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
	xmlThreadInfoAI() : IConfigOpeation(ThreadInfoAI), AI(0), ReturnDataType(1), CheckTime(30), 
		TimeoutCount(1), StopTime(30), ReturnData("ff ff"){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlThreadInfo : public IConfigOpeation
{
public:
	uint16 ThreadTimeout;
	uint16 ThreadTimeCheck;
	uint16 DisposeTimeout;
	uint16 PutQueueTimeout;
	xmlThreadInfo() : IConfigOpeation(ThreadInfo), ThreadTimeout(30), ThreadTimeCheck(60), 
		DisposeTimeout(40), PutQueueTimeout(100){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlConsole : public IConfigOpeation
{
public:
	uint8 support;
	std::string sip;
	uint16 sport;
	xmlConsole() : IConfigOpeation(Console), support(1), sip("INADDR_ANY"), sport(10010){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlConsoleKeys : public IConfigOpeation
{
public:
	class _ConsoleKey
	{
	public:
		std::string Key;
		_ConsoleKey() : Key("not"){}
	};
	std::vector<_ConsoleKey> vec;
	xmlConsoleKeys() : IConfigOpeation(ConsoleKey) {}
	bool Init(CXmlOpeation* pXmlOpeation);
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
	xmlAceDebug() : IConfigOpeation(AceDebug), TrunOn(0), DebugName("./serverdebug.log"), ChkInterval(600), 
		LogFileMaxSize(10240), LogFileMaxCnt(3), Level(""){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlCommandAccount : public IConfigOpeation
{
public:
	uint8 Account;
	uint8 FlowAccount;
	uint32 MaxCommandCount;
	xmlCommandAccount() : IConfigOpeation(CommandAccount), Account(1), FlowAccount(1), MaxCommandCount(2000) {}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlCoreSetting : public IConfigOpeation
{
public:
	uint8 CoreNeed;
	std::string Script;
	xmlCoreSetting() : IConfigOpeation(CoreSetting), CoreNeed(0), Script(""){}
	bool Init(CXmlOpeation* pXmlOpeation);
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
	xmlServerType() : IConfigOpeation(ServerType), Type(0), name("Pss Service"),
		displayname("PssService"), Debug(0), DebugSize(1000), IsClose(0) {}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlServerID : public IConfigOpeation
{
public:
	uint32 id;
	xmlServerID() : IConfigOpeation(ServerID), id(1001) {}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlServerName : public IConfigOpeation
{
public:
	std::string name;
	xmlServerName() : IConfigOpeation(ServerName), name("Freeeyes") {}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlServerVersion : public IConfigOpeation
{
public:
	std::string Version;
	xmlServerVersion() : IConfigOpeation(ServerVersion), Version("1.00") {}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlPacketParses : public IConfigOpeation
{
public:
	class _PacketParse
	{
	public:
		uint16 uParseID;
		std::string szModulePath;
		std::string szModuleName;
		std::string szType;
		uint16 uOrgLength;
		_PacketParse() : uParseID(1), szModulePath("./"), 
			szModuleName("PacketParse_Interface.dll"), szType("STREAM"), uOrgLength(40){}
	};
	std::vector<_PacketParse> _vec;
	xmlPacketParses() : IConfigOpeation(PacketParses){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlBuffPacket : public IConfigOpeation
{
public:
	uint32 Count;
	xmlBuffPacket() : IConfigOpeation(BuffPacket), Count(5000) {}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlMessage : public IConfigOpeation
{
public:
	uint32 Msg_High_mark;
	uint32 Msg_Low_mark;
	uint32 Msg_Buff_Max_Size;
	uint16 Msg_Thread;
	uint32 Msg_MaxQueue;
	xmlMessage() : IConfigOpeation(Message), Msg_High_mark(64000), Msg_Low_mark(64000), Msg_Buff_Max_Size(20480),
		Msg_Thread(5), Msg_MaxQueue(10000){}
	bool Init(CXmlOpeation* pXmlOpeation);
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
	xmlAlertConnect() : IConfigOpeation(AlertConnect), ConnectMin(0), ConnectMax(0), DisConnectMin(0),
		DisConnectMax(0), ConnectAlert(20000), MailID(0){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlIP : public IConfigOpeation
{
public:
	uint8 IPMax;
	uint32 Timeout;
	xmlIP() : IConfigOpeation(IP), IPMax(0), Timeout(300){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlClientData : public IConfigOpeation
{
public:
	uint16 RecvPacketCount;
	uint32 RecvDataMax;
	uint16 SendPacketCount;
	uint32 SendDataMax;
	uint32 MailID;
	xmlClientData() : IConfigOpeation(ClientData), RecvPacketCount(0), RecvDataMax(0), SendPacketCount(0),
		SendDataMax(0), MailID(0){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlCommandInfo : public IConfigOpeation
{
public:
	uint32 CommandID;
	uint32 CommandCount;
	uint32 MailID;
	xmlCommandInfo() : IConfigOpeation(CommandInfo), CommandID(4096), CommandCount(0), MailID(0){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlMail : public IConfigOpeation
{
public:
	uint16 MailID;
	std::string fromMailAddr;
	std::string toMailAddr;
	uint32 MailPass;
	std::string MailUrl;
	uint16 MailPort;
	xmlMail() : IConfigOpeation(Mail), MailID(1), fromMailAddr("local@163.com"), toMailAddr("freeeyes@163.com"),
		MailPass(123456), MailUrl("smtp.163.com"), MailPort(25){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlWorkThreadChart : public IConfigOpeation
{
public:
	uint16 JsonOutput;
	uint32 Count;
	std::string File;
	xmlWorkThreadChart() : IConfigOpeation(WorkThreadChart), JsonOutput(1), Count(10),
		File("./Log/WorkThread.json"){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlConnectChart : public IConfigOpeation
{
public:
	uint16 JsonOutput;
	uint32 Count;
	std::string File;
	xmlConnectChart() : IConfigOpeation(ConnectChart), JsonOutput(1), Count(10),
		File("./Log/ConnnectInfo.json"){}
	bool Init(CXmlOpeation* pXmlOpeation);
};

class xmlCommandChart : public IConfigOpeation
{
public:
	uint16 JsonOutput;
	uint32 Count;
	uint32 CommandID;
	std::string File;
	xmlCommandChart() : IConfigOpeation(CommandChart), JsonOutput(1), Count(10), CommandID(4096),
		File("./Log/Command4096.json"){}
	bool Init(CXmlOpeation* pXmlOpeation);
};


/*********************************以下为旧的********************************/

//PacketParse相关信息
class _PacketParseInfo
{
public:
    uint32 m_u4PacketID;
    uint32 m_u4OrgLength;
    uint8  m_u1Type;
    char   m_szPacketParsePath[MAX_BUFF_200];
    char   m_szPacketParseName[MAX_BUFF_100];

    _PacketParseInfo()
    {
        m_u4PacketID           = 0;
        m_szPacketParsePath[0] = '\0';
        m_szPacketParseName[0] = '\0';
        m_u1Type               = (uint8)PACKET_WITHHEAD;
        m_u4OrgLength          = 0;
    }
};

//服务器信息
//增加对IPv4和IPv6的支持
class _ServerInfo
{
public:
    uint32 m_u4PacketParseInfoID;
    int32  m_nPort;
    uint8  m_u1IPType;
    char   m_szServerIP[MAX_BUFF_50];

    _ServerInfo()
    {
        m_szServerIP[0]       = '\0';
        m_nPort               = 0;
        m_u1IPType            = TYPE_IPV4;
        m_u4PacketParseInfoID = 0;
    }
};

//单个插件加载相关信息
class _ModuleConfig
{
public:
    char m_szModuleName[MAX_BUFF_100];
    char m_szModulePath[MAX_BUFF_200];
    char m_szModuleParam[MAX_BUFF_200];

    _ModuleConfig()
    {
        m_szModuleName[0]  = '\0';
        m_szModulePath[0]  = '\0';
        m_szModuleParam[0] = '\0';
    }
};

typedef vector<_ModuleConfig> vecModuleConfig;

//远程管理器支持
//记录允许远程维护接口进入的数据key的内容。
class _ConsoleKey
{
public:
    char m_szKey[MAX_BUFF_100];

    _ConsoleKey()
    {
        m_szKey[0] = '\0';
    }
};

typedef vector<_ConsoleKey> vecConsoleKey;

//Console管理后台允许IP信息
class _ConsoleClientIP
{
public:
    char m_szServerIP[MAX_BUFF_20];

    _ConsoleClientIP()
    {
        m_szServerIP[0] =  '\0';
    }
};

//连接告警阀值相关配置
class _ConnectAlert
{
public:
    uint32 m_u4ConnectMin;
    uint32 m_u4ConnectMax;
    uint32 m_u4DisConnectMin;
    uint32 m_u4DisConnectMax;
    uint32 m_u4ConnectAlert;
    uint32 m_u4MailID;

    _ConnectAlert()
    {
        m_u4ConnectMin    = 0;
        m_u4ConnectMax    = 0;
        m_u4DisConnectMin = 0;
        m_u4DisConnectMax = 0;
        m_u4ConnectAlert  = 0;
        m_u4MailID        = 0;
    }
};

//连接IP告警阀值相关配置
class _IPAlert
{
public:
    uint32 m_u4IPMaxCount;
    uint32 m_u4IPTimeout;
    uint32 m_u4MailID;

    _IPAlert()
    {
        m_u4IPMaxCount = 0;
        m_u4IPTimeout  = 0;
        m_u4MailID     = 0;
    }
};

//单链接告警阀值相关配置
class _ClientDataAlert
{
public:
    uint32 m_u4RecvPacketCount;
    uint32 m_u4RecvDataMax;
    uint32 m_u4SendPacketCount;
    uint32 m_u4SendDataMax;
    uint32 m_u4MailID;

    _ClientDataAlert()
    {
        m_u4RecvPacketCount = 0;
        m_u4RecvDataMax     = 0;
        m_u4SendPacketCount = 0;
        m_u4SendDataMax     = 0;
        m_u4MailID          = 0;
    }
};

//命令行告警阀值计算
class _CommandAlert
{
public:
    uint32 m_u4CommandCount;
    uint32 m_u4MailID;
    uint16 m_u2CommandID;

    _CommandAlert()
    {
        m_u2CommandID    = 0;
        m_u4CommandCount = 0;
        m_u4MailID       = 0;
    }
};
typedef vector<_CommandAlert> vecCommandAlert;

//邮件配置相关信息
class _MailAlert
{
public:
    uint32 m_u4MailPort;
    uint32 m_u4MailID;
    char   m_szFromMailAddr[MAX_BUFF_200];
    char   m_szToMailAddr[MAX_BUFF_200];
    char   m_szMailPass[MAX_BUFF_200];
    char   m_szMailUrl[MAX_BUFF_200];

    _MailAlert()
    {
        m_u4MailID          = 0;
        m_szFromMailAddr[0] = '\0';
        m_szToMailAddr[0]   = '\0';
        m_szMailPass[0]     = '\0';
        m_szMailUrl[0]      = '\0';
        m_u4MailPort        = 0;
    }
};
typedef vector<_MailAlert> vecMailAlert;

//集群配置相关信息
class _GroupListenInfo
{
public:
    uint32 m_u4GroupPort;
    uint8  m_u1GroupNeed;
    uint8  m_u1IPType;
    char   m_szGroupIP[MAX_BUFF_50];

    _GroupListenInfo()
    {
        m_u1GroupNeed  = 0;
        m_szGroupIP[0] = '\0';
        m_u4GroupPort  = 0;
        m_u1IPType     = TYPE_IPV4;
    }
};

//输出图表信息
class _ChartInfo
{
public:
    bool   m_blJsonOutput;
    uint16 m_u2Count;
    uint32 m_u4CommandID;
    char   m_szJsonFile[MAX_BUFF_200];

    _ChartInfo()
    {
        m_blJsonOutput  = false;
        m_u2Count       = 0;
        m_u4CommandID   = 0;
        m_szJsonFile[0] = '\0';
    }
};

enum ENUM_CHAR_ORDER
{
    SYSTEM_LITTLE_ORDER = 0,   //小端字序
    SYSTEM_BIG_ORDER,          //大端字序
};

class CMainConfig
{
public:
    CMainConfig(void);
    ~CMainConfig(void);

    bool Init();
    bool Init_Alert(const char* szConfigPath);
    bool Init_Main(const char* szConfigPath);
    void Display();
    bool CheckAllIP();
    const char* GetError();

    const char* GetServerName();
    const char* GetServerVersion();
    const char* GetPacketVersion();
    const char* GetWindowsServiceName();
    const char* GetDisplayServiceName();
    uint16 GetServerID();
    uint16 GetServerPortCount();
    _ServerInfo* GetServerPort(int32 nIndex);

    uint32 GetMgsHighMark();
    uint32 GetMsgLowMark();
    uint32 GetMsgMaxBuffSize();
    uint32 GetThreadCount();
    uint8  GetProcessCount();
    uint32 GetMsgMaxQueue();
    uint16 GetHandleCount();

    int32 GetEncryptFlag();
    const char* GetEncryptPass();
    int32 GetEncryptOutFlag();

    uint32 GetSendTimeout();

    uint32 GetRecvBuffSize();
    uint16 GetSendQueueMax();
    uint16 GetThreadTimuOut();
    uint16 GetThreadTimeCheck();
    uint16 GetPacketTimeOut();
    uint16 GetCheckAliveTime();
    uint16 GetMaxHandlerCount();
    void   SetMaxHandlerCount(uint16 u2MaxHandlerCount);
    uint16 GetMaxConnectTime();
    uint8  GetConsoleSupport();
    int32  GetConsolePort();
    uint8  GetConsoleIPType();
    const char* GetConsoleIP();
    vecConsoleKey* GetConsoleKey();
    uint16 GetRecvQueueTimeout();
    uint16 GetSendQueueTimeout();
    uint16 GetSendQueueCount();

    bool CompareConsoleClinetIP(const char* pConsoleClientIP);

    _ServerInfo* GetUDPServerPort(int32 nIndex);

    uint16 GetUDPServerPortCount();
    uint32 GetReactorCount();
    uint8  GetCommandAccount();
    uint32 GetConnectServerTimeout();
    uint16 GetConnectServerCheck();
    uint8  GetConnectServerRunType();
    uint16 GetSendQueuePutTime();
    uint16 GetWorkQueuePutTime();
    uint8  GetServerType();
    uint8  GetDebug();
    uint32 GetDebugSize();
    void   SetDebug(uint8 u1Debug);
    uint8  GetNetworkMode();
    uint32 GetConnectServerRecvBuffer();
    uint8  GetMonitor();
    uint32 GetServerRecvBuff();
    uint8  GetCommandFlow();
    uint32 GetSendDataMask();
    uint32 GetCoreFileSize();
    uint16 GetTcpNodelay();
    uint16 GetBacklog();
    uint16 GetTrackIPCount();
    ENUM_CHAR_ORDER GetCharOrder();
    uint32 GetCpuMax();
    uint32 GetMemoryMax();
    uint8  GetWTAI();
    uint32 GetWTCheckTime();
    uint32 GetWTTimeoutCount();
    uint32 GetWTStopTime();
    uint8  GetWTReturnDataType();
    char*  GetWTReturnData();
    bool   GetByteOrder();
    uint8  GetDebugTrunOn();
    char*  GetDebugFileName();
    uint32 GetChkInterval();
    uint32 GetLogFileMaxSize();
    uint32 GetLogFileMaxCnt();
    char*  GetDebugLevel();
    uint32 GetBlockSize();
    uint32 GetBlockCount();
    uint8  GetServerClose();
    uint32 GetMaxCommandCount();
    uint32 GetServerConnectCount();
    uint16 GetMaxModuleCount();

    uint16 GetModuleInfoCount();
    _ModuleConfig* GetModuleInfo(uint16 u2Index);

    _ConnectAlert*    GetConnectAlert();
    _IPAlert*         GetIPAlert();
    _ClientDataAlert* GetClientDataAlert();
    uint32            GetCommandAlertCount();
    _CommandAlert*    GetCommandAlert(int32 nIndex);
    _MailAlert*       GetMailAlert(uint32 u4MailID);
    _GroupListenInfo* GetGroupListenInfo();
    _PacketParseInfo* GetPacketParseInfo(uint8 u1Index = 0);
    uint8             GetPacketParseCount();
    char*             GetCoreScript();
    _ChartInfo*       GetWorkThreadChart();
    _ChartInfo*       GetConnectChart();
    uint32            GetCommandChartCount();
    _ChartInfo*       GetCommandChart(uint32 u4Index);
    uint32            GetBuffPacketPoolCount();

private:
    uint32     m_u4MsgHighMark;                        //消息的高水位阀值
    uint32     m_u4MsgLowMark;                         //消息的低水位阀值
    uint32     m_u4MsgMaxBuffSize;                     //消息的最大大小
    uint32     m_u4MsgThreadCount;                     //允许的工作线程个数
    uint32     m_u4MsgMaxQueue;                        //消息队列的最大个数
    uint32     m_u4DebugSize;                          //设置当前记录数据包长度的最大缓冲大小
    uint32     m_u4SendTimeout;                        //发送超时时间
    uint32     m_u4RecvBuffSize;                       //接收数据缓冲池的大小
    uint32     m_u4ServerConnectCount;                 //服务器间连接缓冲对象个数
    uint32     m_u4MaxCommandCount;                    //当前框架最大命令数量
    uint32     m_u4ReactorCount;                       //系统中遇到的反应器的个数
    uint32     m_u4ConnectServerTimerout;              //连接远程服务器间隔时间
    uint32     m_u4ConnectServerRecvBuff;              //服务器间的数据包接收缓冲大小
    uint32     m_u4ServerRecvBuff;                     //接收从客户端到达的数据块的最大大小，只有PacketPrase流模式才会生效
    uint32     m_u4SendDatamark;                       //发送差值的水位标（目前只有Proactor模式用这个）
    uint32     m_u4BlockSize;                          //发送缓冲块大小设置
    uint32     m_u4CoreFileSize;                       //Core文件的尺寸大小
    uint32     m_u4TrackIPCount;                       //监控IP的最大历史记录数
    uint32     m_u4SendBlockCount;                     //初始化发送缓冲个数
    uint32     m_u4MaxCpu;                             //监控CPU的最高阀值
    uint32     m_u4MaxMemory;                          //监控内存的峰值
    uint32     m_u4WTCheckTime;                        //工作线程超时包的时间范围，单位是秒
    uint32     m_u4WTTimeoutCount;                     //工作线程超时包的单位时间内的超时次数上限
    uint32     m_u4WTStopTime;                         //停止此命令服务的时间
    uint32     m_u4ChkInterval;                        //检测文件时间
    uint32     m_u4LogFileMaxSize;                     //输出文件最大尺寸
    uint32     m_u4LogFileMaxCnt;                      //输出文件最大个数，当达到最大个数自动循环
    uint32     m_u4BuffPacketPoolCount;                //BuffPacket内存池大小
    int32      m_nServerID;                            //服务器ID
    int32      m_nEncryptFlag;                         //0，加密方式关闭，1为加密方式开启
    int32      m_nEncryptOutFlag;                      //回应数据包，0，为不加密，1为加密
    int32      m_nConsolePort;                         //Console服务器的端口
    uint16     m_u2SendQueueMax;                       //发送队列中最长的数据包个数
    uint16     m_u2ThreadTimuOut;                      //线程超时时间判定
    uint16     m_u2ThreadTimeCheck;                    //线程自检时间
    uint16     m_u2PacketTimeOut;                      //处理数据超时时间
    uint16     m_u2SendAliveTime;                      //发送存活包的时间
    uint16     m_u2HandleCount;                        //handle对象池的个数
    uint16     m_u2MaxHanderCount;                     //最大同时处理Handler的数量
    uint16     m_u2MaxConnectTime;                     //最长等待连接链接时间（此时间内，如果接收和发送都没有发生，则由服务器关闭这个链接）
    uint16     m_u2RecvQueueTimeout;                   //接收队列处理超时时间限定
    uint16     m_u2SendQueueTimeout;                   //发送队列处理超时时间限定
    uint16     m_u2SendQueueCount;                     //框架发送线程数
    uint16     m_u2SendQueuePutTime;                   //设置发送队列的入队超时时间
    uint16     m_u2WorkQueuePutTime;                   //设置工作队列的入队超时时间
    uint16     m_u2MaxModuleCount;                     //当前框架允许的最大模块数量
    uint16     m_u2ConnectServerCheck;                 //服务器间连接单位检查时间
    uint16     m_u2TcpNodelay;                         //TCP的Nagle算法开关，0为打开，1为关闭
    uint16     m_u2Backlog;                            //设置的Backlog值
    bool       m_blByteOrder;                          //当前框架使用字序，false为主机序，true为网络序
    uint8      m_u1MsgProcessCount;                    //当前的多进程数量(仅Linux支持)
    uint8      m_u1Debug;                              //是否开启Debug模式，1是开启，0是关闭
    uint8      m_u1ServerClose;                        //服务器是否允许远程关闭
    uint8      m_u1CommandAccount;                     //是否需要统计命令出入服务器的信息，0是关闭，1是打开。打开后会生成相应的报表
    uint8      m_u1ServerType;                         //设置服务器工作状态
    uint8      m_u1ConsoleSupport;                     //是否支持Console服务，如果是1则是支持，0是不支持
    uint8      m_u1ConsoleIPType;                      //Console的IPType
    uint8      m_u1CommandFlow;                        //命令调用统计，0为不统计，1为统计
    uint8      m_u1ConnectServerRunType;               //服务器间返回包处理模式，0为同步，1为异步
    uint8      m_u1NetworkMode;                        //当前可以设置的网络模式
    uint8      m_u1Monitor;                            //设置当前的监控开关是否打开，0是关闭，1是打开
    uint8      m_u1WTAI;                               //工作线程AI开关，0为关闭，1为打开
    uint8      m_u1WTReturnDataType;                   //返回错误数据的类型，1为二进制，2为文本
    uint8      m_u1DebugTrunOn;                        //ACE_DEBUG文件输出开关，0为关闭，1为打开


    char       m_szError[MAX_BUFF_500];
    char       m_szServerName[MAX_BUFF_20];            //服务器名称
    char       m_szServerVersion[MAX_BUFF_20];         //服务器版本
    char       m_szWindowsServiceName[MAX_BUFF_50];    //windows服务名称
    char       m_szDisplayServiceName[MAX_BUFF_50];    //windows服务显示名称
    char       m_szPacketVersion[MAX_BUFF_20];         //数据解析包模块的版本号
    char       m_szEncryptPass[MAX_BUFF_9];            //最长8位的加密密码，3DES算法
    char       m_szConsoleIP[MAX_BUFF_100];            //Console服务器IP
    char       m_szWTReturnData[MAX_BUFF_1024];        //返回的数据体，最多1K
    char       m_szDeubgFileName[MAX_BUFF_100];        //输出文件名
    char       m_szDebugLevel[MAX_BUFF_100];           //输出文件级别
    char       m_szCoreScript[MAX_BUFF_200];           //当程序Core掉的时候，调用的执行脚本位置

    CXmlOpeation     m_MainConfig;
    _ConnectAlert    m_ConnectAlert;                   //连接告警相关配置信息
    _IPAlert         m_IPAlert;                        //IP告警阀值相关配置
    _ClientDataAlert m_ClientDataAlert;                //单链接客户端告警阀值相关配置
    _GroupListenInfo m_GroupListenInfo;                //集群相关服务器地址配置
    _ChartInfo       m_WorkThreadChart;                //工作线程图表信息
    _ChartInfo       m_ConnectChart;                   //客户端连接图表信息

    typedef vector<_ChartInfo> vecCommandChart;       //信令图表信息
    vecCommandChart m_vecCommandChart;                //命令图表信息

    typedef vector<_PacketParseInfo> vecPacketParseInfo;
    vecPacketParseInfo m_vecPacketParseInfo;

    ENUM_CHAR_ORDER m_u1CharOrder;                 //当前字节序

    typedef vector<_ServerInfo> vecServerInfo;
    vecServerInfo m_vecServerInfo;
    vecServerInfo m_vecUDPServerInfo;

    typedef vector<_ConsoleClientIP> vecConsoleClientIP;
    vecConsoleClientIP m_vecConsoleClientIP;                  //服务器后台允许的IP
    vecConsoleKey      m_vecConsoleKey;                       //服务器允许的key值
    vecCommandAlert    m_vecCommandAlert;                     //命令行告警阀值相关配置
    vecMailAlert       m_vecMailAlert;                        //报警邮件相关设置
    vecModuleConfig    m_vecModuleConfig;                     //单个模块加载相关信息
};

typedef ACE_Singleton<CMainConfig, ACE_Null_Mutex> App_MainConfig;

#endif


