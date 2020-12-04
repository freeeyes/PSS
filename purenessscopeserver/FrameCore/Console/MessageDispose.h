#ifndef _MESSAGEDISPOSE_H
#define _MESSAGEDISPOSE_H

//这里处理所有的接收数据命令函数
//这些命令处理以静态函数的方式暴露出来
//这样有助于代码的阅读和消息的映射
//这么做的原因是，避免大量的if else 命令处理语句
//优化代码结构，完善sonar代码检查结果
//add by freeeyes

#include "MessageDefine.h"
#include "IBuffPacket.h"
#include "LoadModule.h"
#include "MessageManager.h"
#include "MessageService.h"
#include "MakePacket.h"
#include "ace/Message_Block.h"
#include "IPAccount.h"
#include "IObject.h"
#include "FileTestManager.h"
#include "ConsolePromiss.h"
#include "XmlConfig.h"
#include "HandlerManager.h"
#include "UDPConnectManager.h"

#if PSS_PLATFORM == PLATFORM_WIN
#include "ClientProConnectManager.h"
#include "WindowsCPU.h"
#include "ProControlListen.h"
#else
#include "ClientReConnectManager.h"
#include "LinuxCPU.h"
#include "ControlListen.h"
#endif

//设置对应返回命令名称，用于接收端包解析
const uint16 CONSOLE_COMMAND_UNKNOW             = 0x1000;
const uint16 CONSOLE_COMMAND_LOADMOUDLE         = 0x1001;
const uint16 CONSOLE_COMMAND_UNLOADMOUDLE       = 0x1002;
const uint16 CONSOLE_COMMAND_RELOADMOUDLE       = 0x1003;
const uint16 CONSOLE_COMMAND_SHOWMOUDLE         = 0x1004;
const uint16 CONSOLE_COMMAND_CLIENTCOUNT        = 0x1005;
const uint16 CONSOLE_COMMAND_COMMANDINFO        = 0x1006;
const uint16 CONSOLE_COMMAND_COMMANDTIMEOUT     = 0x1007;
const uint16 CONSOLE_COMMAND_COMMANDTIMEOUTCLR  = 0x1008;
const uint16 CONSOLE_COMMAND_COMMANDDATALOG     = 0x1009;
const uint16 CONSOLE_COMMAND_THREADINFO         = 0x100A;
const uint16 CONSOLE_COMMAND_CLIENTINFO         = 0x100B;
const uint16 CONSOLE_COMMAND_UDPCONNECTINFO     = 0x100C;
const uint16 CONSOLE_COMMAND_COLSECLIENT        = 0x100D;
const uint16 CONSOLE_COMMAND_FORBIDDENIP        = 0x100E;
const uint16 CONSOLE_COMMAND_FORBIDDENIPSHOW    = 0x100F;
const uint16 CONSOLE_COMMAND_LIFTED             = 0x1010;
const uint16 CONSOLE_COMMAND_SERVERCONNECT_TCP  = 0x1011;
const uint16 CONSOLE_COMMAND_SERVERCONNECT_UDP  = 0x1012;
const uint16 CONSOLE_COMMAND_PROCESSINFO        = 0x1013;
const uint16 CONSOLE_COMMAND_CLIENTHISTORY      = 0x1014;
const uint16 CONSOLE_COMMAND_ALLCOMMANDINFO     = 0x1015;
const uint16 CONSOLE_COMMAND_SERVERINFO         = 0x1016;
const uint16 CONSOLE_COMMAND_SERVERRECONNECT    = 0x1017;
const uint16 CONSOLE_COMMAND_SETDEBUG           = 0x1018;
const uint16 CONSOLE_COMMAND_SHOWDEBUG          = 0x1019;
const uint16 CONSOLE_COMMAND_SETTRACKIP         = 0x101A;
const uint16 CONSOLE_COMMAND_SETTRACECOMMAND    = 0x101B;
const uint16 CONSOLE_COMMAND_GETTRACKCOMMAND    = 0x101C;
const uint16 CONSOLE_COMMAND_GETCONNECTIPINFO   = 0x101D;
const uint16 CONSOLE_COMMAND_GETLOGINFO         = 0x101E;
const uint16 CONSOLE_COMMAND_SETLOGLEVEL        = 0x101F;
const uint16 CONSOLE_COMMAND_GETWTAI            = 0x1020;
const uint16 CONSOLE_COMMAND_GETWTTIMEOUT       = 0x1021;
const uint16 CONSOLE_COMMAND_SETWTAI            = 0x1022;
const uint16 CONSOLE_COMMAND_GETNICKNAMEINFO    = 0x1023;
const uint16 CONSOLE_COMMAND_SETCONNECTLOG      = 0x1024;
const uint16 CONSOLE_COMMAND_SETMAXCONNECTCOUNT = 0x1025;
const uint16 CONSOLE_COMMAND_ADD_LISTEN         = 0x1026;
const uint16 CONSOLE_COMMAND_DEL_LISTEN         = 0x1027;
const uint16 CONSOLE_COMMAND_SHOW_LISTEN        = 0x1028;
const uint16 CONSOLE_COMMAND_MONITOR_INFO       = 0x1029;
const uint16 CONSOLE_COMMAND_CLOSE_SERVER       = 0x1030;
const uint16 CONSOLE_COMMAND_FILE_TEST_START    = 0x1031;
const uint16 CONSOLE_COMMAND_FILE_TEST_STOP     = 0x1032;
const uint16 CONSOLE_COMMAND_PORT_FLOW          = 0x1033;
const uint16 CONSOLE_COMMAND_PACKET_STATE       = 0x1034;
const uint16 CONSOLE_COMMAND_POOL_SET           = 0x1035;

//目前支持的命令
static const char CONSOLEMESSAHE_LOADMODULE[] = "LoadModule";          //加载模块
static const char CONSOLEMESSAHE_UNLOADMODULE[] = "UnLoadModule";        //卸载模块
static const char CONSOLEMESSAHE_RELOADMODULE[] = "ReLoadModule";        //重新加载模块
static const char CONSOLEMESSAHE_SHOWMOUDLE[] = "ShowModule";          //显示所有现在已经加载的模块
static const char CONSOLEMESSAHE_CLIENTCOUNT[] = "ClientCount";         //当前客户端连接数
static const char CONSOLEMESSAHE_COMMANDINFO[] = "CommandInfo";         //当前某一个信令的状态信息
static const char CONSOLEMESSAHE_COMMANDTIMEOUT[] = "CommandTimeout";      //所有超时的信令列表
static const char CONSOLEMESSAHE_COMMANDTIMEOUTCLR[] = "CommandTimeoutclr";   //清除超时的信令列表
static const char CONSOLEMESSAGE_COMMANDDATALOG[] = "CommandDataLog";     //存储CommandDataLog
static const char CONSOLEMESSAHE_THREADINFO[] = "WorkThreadState";     //当前解析线程和工作线程状态
static const char CONSOLEMESSAHE_CLIENTINFO[] = "ConnectClient";       //当前客户端链接的信息
static const char CONSOLEMESSAHE_UDPCONNECTINFO[] = "UDPConnectClient";    //当前UDP客户端的链接信息
static const char CONSOLEMESSAHE_COLSECLIENT[] = "CloseClient";         //关闭客户端
static const char CONSOLEMESSAHE_FORBIDDENIP[] = "ForbiddenIP";         //禁止IP访问
static const char CONSOLEMESSAHE_FORBIDDENIPSHOW[] = "ShowForbiddenIP";     //查看禁止访问IP列表
static const char CONSOLEMESSAHE_LIFTED[] = "LiftedIP";            //解禁某IP
static const char CONSOLEMESSAHE_SERVERCONNECT_TCP[] = "ServerConnectTCP";    //服务器间通讯(TCP)
static const char CONSOLEMESSAHE_SERVERCONNECT_UDP[] = "ServerConnectUDP";    //服务器间通讯(UDP)
static const char CONSOLEMESSAGE_PROCESSINFO[] = "ShowCurrProcessInfo"; //查看当前服务器的运行状态ShowServerInfo
static const char CONSOLEMESSAGE_CLIENTHISTORY[] = "ShowConnectHistory";  //查看服务器历史链接状态
static const char CONSOLEMESSAGE_ALLCOMMANDINFO[] = "ShowAllCommand";      //查看服务器所有注册模块信令信息
static const char CONSOLEMESSAGE_SERVERINFO[] = "ShowServerInfo";      //查看服务器基本信息
static const char CONSOLEMESSAGE_SERVERRECONNECT[] = "ReConnectServer";     //远端控制重练某一个远端服务器
static const char CONSOLEMESSAGE_SETDEBUG[] = "SetDebug";            //设置当前DEBUG状态
static const char CONSOLEMESSAGE_SHOWDEBUG[] = "ShowDebug";           //查看当前DEBUG状态
static const char CONSOLEMESSAGE_SETTRACKIP[] = "SetTrackIP";          //设置要染色的IP
static const char CONSOLEMESSAGE_SETTRACECOMMAND[] = "SetTrackCommand";    //设置要染色的CommandID
static const char CONSOLEMESSAGE_GETTRACKIPINFO[] = "GetTrackCommandInfo"; //得到染色的CommandID信息
static const char CONSOLEMESSAGE_GETCONNECTIPINFO[] = "GetConnectIPInfo";    //通过ConnectID获得相关的IP信息
static const char CONSOLEMESSAGE_GETLOGINFO[] = "GetLogInfo";          //得到日志等级
static const char CONSOLEMESSAGE_SETLOGLEVEL[] = "SetLogLevel";         //设置日志等级
static const char CONSOLEMESSAGE_GETWTAI[] = "GetWorkThreadAI";     //得到Thread的AI配置信息
static const char CONSOLEMESSAGE_GETWTTIMEOUT[] = "GetWorkThreadTO";     //得到Thread的所有超时数据包信息
static const char CONSOLEMESSAGE_SETWTAI[] = "SetWorkThreadAI";     //设置ThreadAI的配置信息
static const char CONSOLEMESSAGE_GETNICKNAMEINFO[] = "GetNickNameInfo";     //得到别名信息
static const char CONSOLEMESSAGE_SETCONNECTLOG[] = "SetConnectLog";       //设置连接日志开启状态
static const char CONSOLEMESSAGE_SETMAXCONNECTCOUNT[] = "SetMaxConnectCount";  //设置最大连接数
static const char CONSOLEMESSAGE_ADD_LISTEN[] = "AddListen";           //添加一个新的监听端口
static const char CONSOLEMESSAGE_DEL_LISTEN[] = "DelListen";           //删除一个新的监听端口
static const char CONSOLEMESSATE_SHOW_LISTEN[] = "ShowListen";          //查看正在打开的监听端口
static const char CONSOLEMESSATE_MONITOR_INFO[] = "Monitor";             //监控相关运行参数接口
static const char CONSOLEMESSATE_SERVER_CLOSE[] = "ServerClose";         //关闭当前服务器
static const char CONSOLEMESSATE_FILE_TEST_START[] = "TestFileStart";       //启动服务器文件测试用例
static const char CONSOLEMESSATE_FILE_TEST_STOP[] = "TestFileStop";        //停止服务器文件测试用例
static const char CONSOLEMESSATE_PORT_FLOW[] = "PortFlow";            //得到当前时间的端口流量数据
static const char CONSOLEMESSATE_PACKET_STATE[] = "BuffPacketState";     //获得当前正在使用的BuffPacket情况
static const char CONSOLEMESSATE_POOL_SET[] = "PoolSet";             //设置内存池创建信息标记位是否开启

//统一返回数据拼装接口
void Combo_Common_Return_Data(uint8 u1OutputType, uint8 u1State, const char* pMessage, shared_ptr<IBuffPacket> pBuffPacket);
void Combo_Common_Head_Data(uint8 u1OutputType, uint32 u4Count, const char* pMessage, shared_ptr<IBuffPacket> pBuffPacket);
void Combo_Common_vecClientConnectInfo(uint8 u1OutputType, const vecClientConnectInfo VecClientConnectInfo, shared_ptr<IBuffPacket> pBuffPacket);
void Combo_Common_VecForbiddenIP(uint8 u1OutputType, const VecForbiddenIP* pIPList, shared_ptr<IBuffPacket> pBuffPacket);

//协议信息处理函数集合
void DoMessage_LoadModule(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_UnLoadModule(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ReLoadModule(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ClientMessageCount(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ShowModule(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_CommandInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_WorkThreadState(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ClientInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_CloseClient(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ForbiddenIP(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ShowForbiddenList(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_LifedIP(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_UDPClientInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ServerConnectTCP(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ServerConnectUDP(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ShowProcessInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ShowClientHisTory(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ShowAllCommandInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ShowServerInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ReConnectServer(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_CommandTimeout(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_CommandTimeoutclr(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_CommandDataLog(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_SetDebug(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ShowDebug(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_SetTrackIP(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_SetTraceCommand(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_GetTrackCommand(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_GetConnectIPInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_GetLogLevelInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_SetLogLevelInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_GetThreadAI(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_GetWorkThreadTO(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_SetWorkThreadAI(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_GetNickNameInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_SetConnectLog(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_SetMaxConnectCount(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_AddListen(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_DelListen(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ShowListen(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_MonitorInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_ServerClose(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_TestFileStart(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_TestFileStop(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void DoMessage_PortList(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void Do_Message_BuffPacket(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
void Do_Message_PoolSet(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);

#endif
