#ifndef _CONSOLEMESSAGE_H
#define _CONSOLEMESSAGE_H

#include <ace/OS_NS_sys_resource.h>

#include "define.h"
#include "IBuffPacket.h"
#include "LoadModule.h"
#include "MessageManager.h"
#include "MessageService.h"
#include "MakePacket.h"
#include "ForbiddenIP.h"
#include "ace/Message_Block.h"
#include "IPAccount.h"
#include "IObject.h"
#include "FileTestManager.h"
#include "ConsolePromiss.h"

#ifdef WIN32
#include "ProConnectHandle.h"
#include "ClientProConnectManager.h"
#include "ProUDPManager.h"
#include "WindowsCPU.h"
#include "ProControlListen.h"
#else
#include "ConnectHandler.h"
#include "ClientReConnectManager.h"
#include "ReUDPManager.h"
#include "LinuxCPU.h"
#include "ControlListen.h"
#endif

//命令处理返回值类型定义
enum
{
    CONSOLE_MESSAGE_SUCCESS = 0,
    CONSOLE_MESSAGE_FAIL    = 1,
    CONSOLE_MESSAGE_CLOSE   = 2,
};

#define COMMAND_SPLIT_STRING " "

//设置对应返回命令名称，用于接收端包解析
#define CONSOLE_COMMAND_UNKNOW             0x1000
#define CONSOLE_COMMAND_LOADMOUDLE         0x1001
#define CONSOLE_COMMAND_UNLOADMOUDLE       0x1002
#define CONSOLE_COMMAND_RELOADMOUDLE       0x1003
#define CONSOLE_COMMAND_SHOWMOUDLE         0x1004
#define CONSOLE_COMMAND_CLIENTCOUNT        0x1005
#define CONSOLE_COMMAND_COMMANDINFO        0x1005
#define CONSOLE_COMMAND_COMMANDTIMEOUT     0x1007
#define CONSOLE_COMMAND_COMMANDTIMEOUTCLR  0x1008
#define CONSOLE_COMMAND_COMMANDDATALOG     0x1009
#define CONSOLE_COMMAND_THREADINFO         0x100A
#define CONSOLE_COMMAND_CLIENTINFO         0x100B
#define CONSOLE_COMMAND_UDPCONNECTINFO     0x100C
#define CONSOLE_COMMAND_COLSECLIENT        0x100D
#define CONSOLE_COMMAND_FORBIDDENIP        0x100E
#define CONSOLE_COMMAND_FORBIDDENIPSHOW    0x100F
#define CONSOLE_COMMAND_LIFTED             0x1010
#define CONSOLE_COMMAND_SERVERCONNECT_TCP  0x1011
#define CONSOLE_COMMAND_SERVERCONNECT_UDP  0x1012
#define CONSOLE_COMMAND_PROCESSINFO        0x1013
#define CONSOLE_COMMAND_CLIENTHISTORY      0x1014
#define CONSOLE_COMMAND_ALLCOMMANDINFO     0x1015
#define CONSOLE_COMMAND_SERVERINFO         0x1016
#define CONSOLE_COMMAND_SERVERRECONNECT    0x1017
#define CONSOLE_COMMAND_SETDEBUG           0x1018
#define CONSOLE_COMMAND_SHOWDEBUG          0x1019
#define CONSOLE_COMMAND_SETTRACKIP         0x101A
#define CONSOLE_COMMAND_SETTRACECOMMAND    0x101B
#define CONSOLE_COMMAND_GETTRACKCOMMAND    0x101C
#define CONSOLE_COMMAND_GETCONNECTIPINFO   0x101D
#define CONSOLE_COMMAND_GETLOGINFO         0x101E
#define CONSOLE_COMMAND_SETLOGLEVEL        0x101F
#define CONSOLE_COMMAND_GETWTAI            0x1020
#define CONSOLE_COMMAND_GETWTTIMEOUT       0x1021
#define CONSOLE_COMMAND_SETWTAI            0x1022
#define CONSOLE_COMMAND_GETNICKNAMEINFO    0x1023
#define CONSOLE_COMMAND_SETCONNECTLOG      0x1024
#define CONSOLE_COMMAND_SETMAXCONNECTCOUNT 0x1025
#define CONSOLE_COMMAND_ADD_LISTEN         0x1026
#define CONSOLE_COMMAND_DEL_LISTEN         0x1027
#define CONSOLE_COMMAND_SHOW_LISTEN        0x1028
#define CONSOLE_COMMAND_MONITOR_INFO       0x1029
#define CONSOLE_COMMAND_CLOSE_SERVER       0x1030
#define CONSOLE_COMMAND_FILE_TEST_START    0x1031
#define CONSOLE_COMMAND_FILE_TEST_STOP     0x1032

//目前支持的命令
#define CONSOLEMESSAHE_LOADMOUDLE         "LoadModule"          //加载模块
#define CONSOLEMESSAHE_UNLOADMOUDLE       "UnLoadModule"        //卸载模块
#define CONSOLEMESSAHE_RELOADMOUDLE       "ReLoadModule"        //重新加载模块
#define CONSOLEMESSAHE_SHOWMOUDLE         "ShowModule"          //显示所有现在已经加载的模块
#define CONSOLEMESSAHE_CLIENTCOUNT        "ClientCount"         //当前客户端连接数
#define CONSOLEMESSAHE_COMMANDINFO        "CommandInfo"         //当前某一个信令的状态信息
#define CONSOLEMESSAHE_COMMANDTIMEOUT     "CommandTimeout"      //所有超时的信令列表
#define CONSOLEMESSAHE_COMMANDTIMEOUTCLR  "CommandTimeoutclr"   //清除超时的信令列表
#define CONSOLEMESSAGE_COMMANDDATALOG     "CommandDataLog"      //存储CommandDataLog
#define CONSOLEMESSAHE_THREADINFO         "WorkThreadState"     //当前解析线程和工作线程状态
#define CONSOLEMESSAHE_CLIENTINFO         "ConnectClient"       //当前客户端链接的信息
#define CONSOLEMESSAHE_UDPCONNECTINFO     "UDPConnectClient"    //当前UDP客户端的链接信息
#define CONSOLEMESSAHE_COLSECLIENT        "CloseClient"         //关闭客户端
#define CONSOLEMESSAHE_FORBIDDENIP        "ForbiddenIP"         //禁止IP访问
#define CONSOLEMESSAHE_FORBIDDENIPSHOW    "ShowForbiddenIP"     //查看禁止访问IP列表
#define CONSOLEMESSAHE_LIFTED             "LiftedIP"            //解禁某IP
#define CONSOLEMESSAHE_SERVERCONNECT_TCP  "ServerConnectTCP"    //服务器间通讯(TCP)
#define CONSOLEMESSAHE_SERVERCONNECT_UDP  "ServerConnectUDP"    //服务器间通讯(UDP)
#define CONSOLEMESSAGE_PROCESSINFO        "ShowCurrProcessInfo" //查看当前服务器的运行状态ShowServerInfo
#define CONSOLEMESSAGE_CLIENTHISTORY      "ShowConnectHistory"  //查看服务器历史链接状态
#define CONSOLEMESSAGE_ALLCOMMANDINFO     "ShowAllCommand"      //查看服务器所有注册模块信令信息
#define CONSOLEMESSAGE_SERVERINFO         "ShowServerInfo"      //查看服务器基本信息
#define CONSOLEMESSAGE_SERVERRECONNECT    "ReConnectServer"     //远端控制重练某一个远端服务器
#define CONSOLEMESSAGE_SETDEBUG           "SetDebug"            //设置当前DEBUG状态
#define CONSOLEMESSAGE_SHOWDEBUG          "ShowDebug"           //查看当前DEBUG状态
#define CONSOLEMESSAGE_SETTRACKIP         "SetTrackIP"          //设置要染色的IP
#define CONSOLEMESSAGE_SETTRACECOMMAND    "SetTrackCommand"     //设置要染色的CommandID
#define CONSOLEMESSAGE_GETTRACKIPINFO     "GetTrackCommandInfo" //得到染色的CommandID信息
#define CONSOLEMESSAGE_GETCONNECTIPINFO   "GetConnectIPInfo"    //通过ConnectID获得相关的IP信息
#define CONSOLEMESSAGE_GETLOGINFO         "GetLogInfo"          //得到日志等级
#define CONSOLEMESSAGE_SETLOGLEVEL        "SetLogLevel"         //设置日志等级
#define CONSOLEMESSAGE_GETWTAI            "GetWorkThreadAI"     //得到Thread的AI配置信息
#define CONSOLEMESSAGE_GETWTTIMEOUT       "GetWorkThreadTO"     //得到Thread的所有超时数据包信息
#define CONSOLEMESSAGE_SETWTAI            "SetWorkThreadAI"     //设置ThreadAI的配置信息
#define CONSOLEMESSAGE_GETNICKNAMEINFO    "GetNickNameInfo"     //得到别名信息
#define CONSOLEMESSAGE_SETCONNECTLOG      "SetConnectLog"       //设置连接日志开启状态 
#define CONSOLEMESSAGE_SETMAXCONNECTCOUNT "SetMaxConnectCount"  //设置最大连接数
#define CONSOLEMESSAGE_ADD_LISTEN         "AddListen"           //添加一个新的监听端口
#define CONSOLEMESSAGE_DEL_LISTEN         "DelListen"           //删除一个新的监听端口
#define CONSOLEMESSATE_SHOW_LISTEN        "ShowListen"          //查看正在打开的监听端口 
#define CONSOLEMESSATE_MONITOR_INFO       "Monitor"             //监控相关运行参数接口
#define CONSOLEMESSATE_SERVER_CLOSE       "ServerClose"         //关闭当前服务器
#define CONSOLEMESSATE_FILE_TEST_START    "TestFileStart"       //启动服务器文件测试用例
#define CONSOLEMESSATE_FILE_TEST_STOP     "TestFileStop"        //停止服务器文件测试用例

//命令处理参数
struct _CommandInfo
{
    uint8 m_u1OutputType;                 //输出类型，0为二进制，1为文本
    char m_szCommandTitle[MAX_BUFF_100];  //处理命令头
    char m_szCommandExp[MAX_BUFF_100];    //处理命令扩展参数
    char m_szUser[MAX_BUFF_50];           //用户信息

    _CommandInfo()
    {
        m_u1OutputType      = 0;
        m_szCommandTitle[0] = '\0';
        m_szCommandExp[0]   = '\0';
        m_szUser[0]         = '\0';
    }
};

//文件名结构
struct _FileInfo
{
    char m_szFilePath[MAX_BUFF_100];
    char m_szFileName[MAX_BUFF_100];
    char m_szFileParam[MAX_BUFF_200];

    _FileInfo()
    {
        m_szFilePath[0]  = '\0';
        m_szFileName[0]  = '\0';
        m_szFileParam[0] = '\0';
    }
};

//监听端口信息
struct _ListenInfo
{
    uint32 m_u4Port;
    uint32 m_u4PacketParseID;
    uint8  m_u1IPType;
    char   m_szListenIP[MAX_BUFF_20];

    _ListenInfo()
    {
        m_szListenIP[0]   = '\0';
        m_u4Port          = 0;
        m_u1IPType        = TYPE_IPV4;
        m_u4PacketParseID = 0;
    }
};

//染色IP信息
struct _DyeIPInfo
{
    char   m_szClientIP[MAX_BUFF_20];   //染色客户端IP
    uint16 m_u2MaxCount;                //最大数量
};

//染色的CommandID
struct _DyeCommandInfo
{
    uint16 m_u2CommandID;               //染色客户端命令
    uint16 m_u2MaxCount;                //最大数量
};

class CConsoleMessage
{
public:
    CConsoleMessage();
    ~CConsoleMessage();

    int Dispose(ACE_Message_Block* pmb, IBuffPacket* pBuffPacket, uint8& u1OutputType);     //要处理的命令字解析, pBuffPacket为返回要发送给客户端的数据
    int ParsePlugInCommand(const char* pCommand, IBuffPacket* pBuffPacket);                 //执行命令

    //初始化部分
    bool SetConsoleKey(vecConsoleKey* pvecConsoleKey);       //添加验证允许的key值

    //公用数据部分
private:
    int  ParseCommand_Plugin(const char* pCommand, IBuffPacket* pBuffPacket, uint8& u1OutputType);            //执行命令(插件内部调用)
    int  ParseCommand(const char* pCommand, IBuffPacket* pBuffPacket, uint8& u1OutputType);                   //执行命令
    int  DoCommand(_CommandInfo& CommandInfo, IBuffPacket* pCurrBuffPacket, IBuffPacket* pReturnBuffPacket);  //处理命令数据
    bool GetCommandInfo(const char* pCommand, _CommandInfo& CommandInfo);                                     //把命令切割成应该有的数据格式
    bool GetFileInfo(const char* pFile, _FileInfo& FileInfo);                                                 //将一个全路径切分成文件名
    bool GetForbiddenIP(const char* pCommand, _ForbiddenIP& ForbiddenIP);                                     //得到禁止的IP列表
    bool GetConnectServerID(const char* pCommand, int& nServerID);                                            //得到一个指定的服务器ID
    bool GetDebug(const char* pCommand, uint8& u1Debug);                                                      //得到当前设置的BUDEG
    bool CheckConsoleKey(const char* pKey);                                                                   //验证key
    bool GetTrackIP(const char* pCommand, _ForbiddenIP& ForbiddenIP);                                         //得到设置的追踪IP
    bool GetLogLevel(const char* pCommand, int& nLogLevel);                                                   //得到日志等级
    bool GetAIInfo(const char* pCommand, int& nAI, int& nDispose, int& nCheck, int& nStop);                   //得到AI设置
    bool GetNickName(const char* pCommand, char* pName);                                                      //得到连接别名
    bool GetConnectID(const char* pCommand, uint32& u4ConnectID, bool& blFlag);                               //得到ConnectID
    bool GetMaxConnectCount(const char* pCommand, uint16& u2MaxConnectCount);                                 //得到最大的连接总数
    bool GetListenInfo(const char* pCommand, _ListenInfo& objListenInfo);                                     //得到监听端口信息
    bool GetTestFileName(const char* pCommand, char* pFileName);                                              //获得加载测试文件名
    bool GetDyeingIP(const char* pCommand, _DyeIPInfo& objDyeIPInfo);                                         //获得染色IP的相关信息
    bool GetDyeingCommand(const char* pCommand, _DyeCommandInfo& objDyeCommandInfo);                          //获得染色Command的相关信息

    //命令具体实现部分
private:
    void DoMessage_LoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_UnLoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ReLoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ClientMessageCount(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ShowModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_CommandInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_WorkThreadState(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ClientInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_CloseClient(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ForbiddenIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ShowForbiddenList(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_LifedIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_UDPClientInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ServerConnectTCP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ServerConnectUDP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ShowProcessInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ShowClientHisTory(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ShowAllCommandInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ShowServerInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ReConnectServer(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_CommandTimeout(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_CommandTimeoutclr(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_CommandDataLog(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_SetDebug(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ShowDebug(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_SetTrackIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_SetTraceCommand(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_GetTrackCommand(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_GetConnectIPInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_GetLogLevelInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_SetLogLevelInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_GetThreadAI(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_GetWorkThreadTO(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_SetWorkThreadAI(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_GetNickNameInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_SetConnectLog(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_SetMaxConnectCount(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_AddListen(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_DelListen(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ShowListen(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_MonitorInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_ServerClose(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_TestFileStart(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    void DoMessage_TestFileStop(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);

private:
    vecConsoleKey*      m_pvecConsoleKey;
    CConsolePromissions m_objConsolePromissions;
};

typedef ACE_Singleton<CConsoleMessage, ACE_Null_Mutex> App_ConsoleManager;
#endif
