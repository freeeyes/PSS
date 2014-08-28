#ifndef _PROCONTROLLISTEN_H
#define _PROCONTROLLISTEN_H

#include "ProConnectAccept.h"
#include "AceProactorManager.h"

//连接管理类，用于创建删除和查看监听
//add by freeeyes

struct _ProControlInfo
{
	char m_szListenIP[MAX_BUFF_20];
	uint32 m_u4Port;

	_ProControlInfo()
	{
		m_szListenIP[0] = '\0';
		m_u4Port        = 0;
	}
};

typedef vector<_ProControlInfo> vecProControlInfo;

class CProControlListen
{
public:
	CProControlListen();
	~CProControlListen();

	bool AddListen(const char* pListenIP, uint32 u4Port, uint8 u1IPType);  //打开一个新的监听端口
	bool DelListen(const char* pListenIP, uint32 u4Port);                  //关闭一个已知的连接
	void ShowListen(vecProControlInfo& objProControlInfo);                 //查看已打开的监听端口
};

typedef ACE_Singleton<CProControlListen, ACE_Null_Mutex> App_ProControlListen;

#endif
