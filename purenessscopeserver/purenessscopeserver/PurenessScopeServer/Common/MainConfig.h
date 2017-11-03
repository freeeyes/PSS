#ifndef _MAINCONFIG_H
#define _MAINCONFIG_H

#include "define.h"
#include "XmlOpeation.h"

#include "ace/Singleton.h"
#include <vector>

#include "PacketParse.h"

//PacketParse相关信息
struct _PacketParseInfo
{
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
struct _ServerInfo
{
    uint32 m_u4PacketParseInfoID;
    int32    m_nPort;
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
struct _ModuleConfig
{
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
struct _ConsoleKey
{
    char m_szKey[MAX_BUFF_100];

    _ConsoleKey()
    {
        m_szKey[0] = '\0';
    }
};

typedef vector<_ConsoleKey> vecConsoleKey;

//Console管理后台允许IP信息
struct _ConsoleClientIP
{
    char m_szServerIP[MAX_BUFF_20];

    _ConsoleClientIP()
    {
        m_szServerIP[0] =  '\0';
    }
};

//连接告警阀值相关配置
struct _ConnectAlert
{
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
struct _IPAlert
{
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
struct _ClientDataAlert
{
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
struct _CommandAlert
{
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
struct _MailAlert
{
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
struct _GroupListenInfo
{
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
    int32    GetConsolePort();
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

    CXmlOpeation     m_MainConfig;
    _ConnectAlert    m_ConnectAlert;                   //连接告警相关配置信息
    _IPAlert         m_IPAlert;                        //IP告警阀值相关配置
    _ClientDataAlert m_ClientDataAlert;                //单链接客户端告警阀值相关配置
    _GroupListenInfo m_GroupListenInfo;                //集群相关服务器地址配置

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
