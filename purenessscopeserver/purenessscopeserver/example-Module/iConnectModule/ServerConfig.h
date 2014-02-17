/*
 * 服务器列表的相关管理功能
 * by w1w
 */
#pragma once

#include "IObject.h"
#include "XmlOpeation.h"
#include "CommandDefine.h"

#include "ace/Singleton.h"
#include <vector>
#include <map>

using namespace std;

//连接信息
struct _icConnectInfo
{
	uint32 m_ConnectID;         //连接ID
	uint16 m_Appid;             //注册此连接的APPid
	ACE_Time_Value m_atvTime;   //活动时间
	_icConnectInfo()
	{
		m_ConnectID=-1;
		m_Appid=-1;
	}
};

typedef map<int, _icConnectInfo> mapConnectInfo;

//应用信息
struct _icAppInfo
{
	uint16 m_Appid;                //APPid
	char m_AppName[MAX_BUFF_20];   //APP名称
	ACE_Time_Value m_atvTime;      //活动时间

	_icAppInfo()
	{
		m_Appid = -1;
		iniChar(m_AppName,MAX_BUFF_20);
	}
};
typedef map<int, _icAppInfo> mapAppInfo;

//通讯服务器信息
class _icServerInfo
{
public:
	uint16  m_Serverid;                      //服务器ID
	char m_szServerIP[MAX_BUFF_20];          //服务器IP
	char m_szServerName[MAX_BUFF_20];        //服务器名称
	char m_szServerVersion[MAX_BUFF_20];     //服务器版本
	char m_szStationID[33];                  //服务器StationID
	uint16  m_nPort;                         //服务器端口
	mapAppInfo m_mapAppInfo;                 //服务器连接着的APP列表
	bool m_NeedRefeshApp;                    //服务器的APP列表是否需要主动更新
	bool m_preonline;                        //服务器上一个在线状态

	_icServerInfo()
	{
		m_Serverid = 0;
		iniChar(m_szServerIP,MAX_BUFF_20);
		iniChar(m_szStationID,33);
		iniChar(m_szServerName,MAX_BUFF_20);
		iniChar(m_szServerVersion,MAX_BUFF_20);
		m_nPort         = 0;
		m_pClientManager = NULL;
		m_NeedRefeshApp = true;
		m_preonline = false;
	}

	bool isonline()
	{
		if (m_pClientManager!=NULL)
			return m_pClientManager->GetConnectState(m_Serverid);
		else
			return false;
	}
	void SetClientManager(IClientManager* pClientManager) { m_pClientManager = pClientManager; }
private:
	IClientManager*        m_pClientManager;
};


typedef map<int, _icServerInfo> mapServerInfo;

class CServerConfig
{
public:
	CServerConfig();
	~CServerConfig();
	_icServerInfo m_Server;          //当前服务器
	mapServerInfo m_mapServerInfo;   //远程服务器列表
	mapConnectInfo m_mapConnectInfo; //当前连接列表
	bool Init();                     //初始化
	void Display();                  //显示远程服务器列表
	const char* GetError();
	void SetClientManager(IClientManager* pClientManager);
private:
	CXmlOpeation m_ServerConfig;
	char       m_szError[MAX_BUFF_500];
};

typedef ACE_Singleton<CServerConfig, ACE_Null_Mutex> icServerConfig;