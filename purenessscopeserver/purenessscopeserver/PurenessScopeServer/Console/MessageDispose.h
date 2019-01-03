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

#ifdef WIN32
#include "ProConnectHandler.h"
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
#define CONSOLE_COMMAND_PORT_FLOW          0x1033
#define CONSOLE_COMMAND_PACKET_STATE       0x1034
#define CONSOLE_COMMAND_POOL_SET           0x1035

//目前支持的命令
#define CONSOLEMESSAHE_LOADMODULE         "LoadModule"          //加载模块
#define CONSOLEMESSAHE_UNLOADMODULE       "UnLoadModule"        //卸载模块
#define CONSOLEMESSAHE_RELOADMODULE       "ReLoadModule"        //重新加载模块
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
#define CONSOLEMESSATE_PORT_FLOW          "PortFlow"            //得到当前时间的端口流量数据  
#define CONSOLEMESSATE_PACKET_STATE       "BuffPacketState"     //获得当前正在使用的BuffPacket情况
#define CONSOLEMESSATE_POOL_SET           "PoolSet"             //设置内存池创建信息标记位是否开启

//统一返回数据拼装接口
void Combo_Common_Return_Data(uint8 u1OutputType, uint8 u1State, const char* pMessage, IBuffPacket* pBuffPacket);
void Combo_Common_Head_Data(uint8 u1OutputType, uint32 u4Count, const char* pMessage, IBuffPacket* pBuffPacket);
void Combo_Common_vecClientConnectInfo(uint8 u1OutputType, vecClientConnectInfo& VecClientConnectInfo, IBuffPacket* pBuffPacket);
void Combo_Common_VecForbiddenIP(uint8 u1OutputType, VecForbiddenIP* pIPList, IBuffPacket* pBuffPacket);

//协议信息处理函数集合
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
void DoMessage_PortList(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
void Do_Message_BuffPacket(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
void Do_Message_PoolSet(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);

#endif
