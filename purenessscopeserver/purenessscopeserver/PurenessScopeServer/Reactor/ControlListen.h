#ifndef _CONTROLLISTEN_H
#define _CONTROLLISTEN_H

#include "ConnectAccept.h"
#include "AceReactorManager.h"

struct _ControlInfo
{
	char m_szListenIP[MAX_BUFF_20];
	uint32 m_u4Port;

	_ControlInfo()
	{
		m_szListenIP[0] = '\0';
		m_u4Port        = 0;
	}
};

typedef vector<_ControlInfo> vecControlInfo;

class CControlListen
{
public:
	CControlListen();
	~CControlListen();

	bool AddListen(const char* pListenIP, uint32 u4Port, uint8 u1IPType);  //打开一个新的监听端口
	bool DelListen(const char* pListenIP, uint32 u4Port);                  //关闭一个已知的连接
	void ShowListen(vecControlInfo& objControlInfo);                       //查看已打开的监听端口
};

typedef ACE_Singleton<CControlListen, ACE_Null_Mutex> App_ControlListen;

#endif
