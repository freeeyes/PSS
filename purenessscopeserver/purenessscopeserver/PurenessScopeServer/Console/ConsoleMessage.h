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

#ifdef WIN32
#include "ProConnectHandle.h"
#include "ClientProConnectManager.h"
#include "ProUDPManager.h"
#include "WindowsCPU.h"
#else
#include "ConnectHandler.h"
#include "ClientReConnectManager.h"
#include "ReUDPManager.h"
#include "LinuxCPU.h"
#endif

//命令处理返回值类型定义
enum
{
	CONSOLE_MESSAGE_SUCCESS = 0,
	CONSOLE_MESSAGE_FAIL    = -1,
};

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
#define CONSOLEMESSAGE_PROCESSINFO        "ShowCurrProcessInfo" //查看当前服务器的运行状态
#define CONSOLEMESSAGE_CLIENTHISTORY      "ShowConnectHistory"  //查看服务器历史链接状态
#define CONSOLEMESSAGE_ALLCOMMANDINFO     "ShowAllCommand"      //查看服务器所有注册模块信令信息
#define CONSOLEMESSAGE_SERVERINFO         "ShowServerInfo"      //查看服务器基本信息
#define CONSOLEMESSAGE_SERVERRECONNECT    "ReConnectServer"     //远端控制重练某一个远端服务器
#define CONSOLEMESSAGE_SETDEBUG           "SetDebug"            //设置当前DEBUG状态
#define CONSOLEMESSAGE_SHOWDEBUG          "ShowDebug"           //查看当前DEBUG状态
#define CONSOLEMESSAGE_SETTRACKIP         "SetTrackIP"          //设置要监控的IP
#define CONSOLEMESSAGE_DELTRACKIP         "DelTrackIP"          //删除要监控的IP
#define CONSOLEMESSAGE_GETTRACKIPINFO     "GetTrackIPInfo"      //得到监控IP的历史记录
#define CONSOLEMESSAGE_GETCONNECTIPINFO   "GetConnectIPInfo"    //通过COnnectID获得相关的IP信息
#define CONSOLEMESSAGE_GETLOGINF          "GetLogInfo"          //得到日志等级
#define CONSOLEMESSAGE_SETLOGLEVEL        "SetLogLevel"         //设置日志等级
#define CONSOLEMESSAGE_GETWTAI            "GetWorkThreadAI"     //得到Thread的AI配置信息
#define CONSOLEMESSAGE_GETWTTIMEOUT       "GetWorkThreadTO"     //得到Thread的所有超时数据包信息
#define CONSOLEMESSAGE_SETWTAI            "SetWorkThreadAI"     //设置ThreadAI的配置信息
#define CONSOLEMESSAGE_GETNICKNAMEINFO    "GetNickNameInfo"     //得到别名信息
#define CONSOLEMESSAGE_SETCONNECTLOG      "SetConnectLog"       //设置连接日志开启状态 
#define CONSOLEMESSAGE_SETMAXCONNECTCOUNT "SetMaxConnectCount"  //设置最大连接数
#define CONSOLEMESSAGE_ADD_LISTEN         "AddListen"           //添加一个新的监听端口
#define CONSOLEMESSAGE_DEL_LISTEN         "DelListen"           //删除一个新的监听端口

//命令处理参数
struct _CommandInfo
{
	char m_szCommandTitle[MAX_BUFF_100];  //处理命令头
	char m_szCommandExp[MAX_BUFF_100];    //处理命令扩展参数

	_CommandInfo()
	{
		m_szCommandTitle[0] = '\0';
		m_szCommandExp[0]   = '\0';
	}
};

//文件名结构
struct _FileInfo
{
	char m_szFilePath[MAX_BUFF_100];
	char m_szFileName[MAX_BUFF_100];

	_FileInfo()
	{
		m_szFilePath[0] = '\0';
		m_szFileName[0] = '\0';
	}
};

//监听端口信息
struct _ListenInfo
{
	char   m_szListenIP[MAX_BUFF_20];
	uint32 m_u4Port;
	uint8  m_u1IPType;

	_ListenInfo()
	{
		m_szListenIP[0] = '\0';
		m_u4Port        = 0;
		m_u1IPType      = TYPE_IPV4;
	}
};

class CConsoleMessage
{
public:
	CConsoleMessage();
	~CConsoleMessage();

	int Dispose(ACE_Message_Block* pmb, IBuffPacket* pBuffPacket);             //要处理的命令字解析, pBuffPacket为返回要发送给客户端的数据

	//初始化部分
	bool SetConsoleKey(vecConsoleKey* pvecConsoleKey);       //添加验证允许的key值

	//公用数据部分
private:
	int ParseCommand(const char* pCommand, IBuffPacket* pBuffPacket);                        //执行命令
	bool GetCommandInfo(const char* pCommand, _CommandInfo& CommandInfo);                    //把命令切割成应该有的数据格式
	bool GetFileInfo(const char* pFile, _FileInfo& FileInfo);                                //将一个全路径切分成文件名
	bool GetForbiddenIP(const char* pCommand, _ForbiddenIP& ForbiddenIP);                    //得到禁止的IP列表
	bool GetConnectServerID(const char* pCommand, int& nServerID);                           //得到一个指定的服务器ID
	bool GetDebug(const char* pCommand, uint8& u1Debug);                                     //得到当前设置的BUDEG
	bool CheckConsoleKey(const char* pKey);                                                  //验证key
	bool GetTrackIP(const char* pCommand, _ForbiddenIP& ForbiddenIP);                        //得到设置的追踪IP
	bool GetLogLevel(const char* pCommand, int& nLogLevel);                                  //得到日志等级
	bool GetAIInfo(const char* pCommand, int& nAI, int& nDispose, int& nCheck, int& nStop);  //得到AI设置
	bool GetNickName(const char* pCommand, char* pName);                                     //得到连接别名
	bool GetConnectID(const char* pCommand, uint32& u4ConnectID, bool& blFlag);              //得到ConnectID
	bool GetMaxConnectCount(const char* pCommand, uint16& u2MaxConnectCount);                //得到最大的连接总数
	bool GetListenInfo(const char* pCommand, _ListenInfo& objListenInfo);                    //得到监听端口信息

	//命令具体实现部分
private:
	bool DoMessage_LoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_UnLoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ReLoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ClientMessageCount(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_CommandInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_WorkThreadState(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ClientInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessgae_CloseClient(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ForbiddenIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowForbiddenList(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_LifedIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_UDPClientInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ServerConnectTCP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ServerConnectUDP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowProcessInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowClientHisTory(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowAllCommandInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowServerInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ReConnectServer(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_CommandTimeout(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_CommandTimeoutclr(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_CommandDataLog(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_SetDebug(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_ShowDebug(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_SetTrackIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_DelTrackIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_GetTrackIPInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_GetConnectIPInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_GetLogLevelInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_SetLogLevelInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_GetThreadAI(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_GetWorkThreadTO(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_SetWorkThreadAI(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_GetNickNameInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_SetConnectLog(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_SetMaxConnectCount(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_AddListen(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);
	bool DoMessage_DelListen(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket);

private:
	vecConsoleKey* m_pvecConsoleKey;
};

#endif
