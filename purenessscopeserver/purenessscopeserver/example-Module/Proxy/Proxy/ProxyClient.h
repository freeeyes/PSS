#ifndef _PROXYCLIENT_H
#define _PROXYCLIENT_H

#include "define.h"
#include "IObject.h"

#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"
#include "ace/Reactor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Reactor_Notification_Strategy.h"

#ifndef __LINUX__
#include "ace/Select_Reactor.h"
#else
#include "ace/Dev_Poll_Reactor.h"
#endif
#include "ace/Thread.h" 

#define MAX_RECV_BUFF           4096
#define COMMAND_RETURN_PROXY    0xf030

#include <map>

using namespace std;

class CProxyClient : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	CProxyClient(void);
	~CProxyClient(void);

	bool Close();

	virtual int open(void*);
	virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
	virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);

	void SetWorkThreadID(uint32 u4WorkThreadID);

	uint32 GetWorkThreadID();

	bool SendData(char* pData, int nLen);

	void SetServerObject(uint32 u4ConnectID, CServerObject* pServerObject);



private:
	uint32                     m_u4SendAllSize;      //总发送字节数
	uint32                     m_u4RecvAllSize;      //总接收字节数
	CServerObject*             m_pServerObject;      //记录发送指针
	uint32                     m_u4ConnectID;        //客户端与插件的连接ID

	uint32                     m_u4WorkThreadID;     //记录工作线程ID

	ACE_INET_Addr              m_addrRemote;
	int                        m_nIOCount;           //当前IO操作的个数
	ACE_Recursive_Thread_Mutex m_ThreadLock;
};

typedef ACE_Connector<CProxyClient, ACE_SOCK_CONNECTOR> CProxyClientConnector;

typedef map<CProxyClient*, uint32> mapProxy2ConnectID;
typedef map<uint32, CProxyClient*> mapConnectID2Proxy;

//管理CProxyClient和ConnectID的映射关系
//add by freeeyes
class CProxyClientManager
{
public:
	CProxyClientManager() {};
	~CProxyClientManager() {};

	bool Insert(uint32 u4ConnectID, CProxyClient* pProxyClient)
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
		mapConnectID2Proxy::iterator f = m_mapConnectID2Proxy.find(u4ConnectID);
		if(f != m_mapConnectID2Proxy.end())
		{
			return false;
		}

		mapProxy2ConnectID::iterator fe = m_mapProxy2ConnectID.find(pProxyClient);
		if(fe != m_mapProxy2ConnectID.end())
		{
			return false;
		}

		m_mapConnectID2Proxy.insert(mapConnectID2Proxy::value_type(u4ConnectID, pProxyClient));
		m_mapProxy2ConnectID.insert(mapProxy2ConnectID::value_type(pProxyClient, u4ConnectID));

		return true;
	};

	uint32 FindConnectID(CProxyClient* pProxyClient)
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
		mapProxy2ConnectID::iterator f = m_mapProxy2ConnectID.find(pProxyClient);
		if(f == m_mapProxy2ConnectID.end())
		{
			return 0;
		}
		else
		{
			return (uint32)f->second;
		}
	}

	CProxyClient* FindProxyClient(uint32 u4ConnectID)
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
		mapConnectID2Proxy::iterator f = m_mapConnectID2Proxy.find(u4ConnectID);
		if(f == m_mapConnectID2Proxy.end())
		{
			return NULL;
		}
		else
		{
			return (CProxyClient* )f->second;
		}
	}

	void DeleteByProxyClient(CProxyClient* pProxyClient)
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
		mapProxy2ConnectID::iterator f = m_mapProxy2ConnectID.find(pProxyClient);
		if(f != m_mapProxy2ConnectID.end())
		{
			uint32 u4ConnectID = (uint32)f->second;

			mapConnectID2Proxy::iterator fb = m_mapConnectID2Proxy.find(u4ConnectID);
			if(fb != m_mapConnectID2Proxy.end())
			{
				m_mapConnectID2Proxy.erase(fb);
			}

			m_mapProxy2ConnectID.erase(f);
		}
	}

	void DeleteByConnectID(uint32 u4ConnectID)
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
		mapConnectID2Proxy::iterator f = m_mapConnectID2Proxy.find(u4ConnectID);
		if(f != m_mapConnectID2Proxy.end())
		{
			CProxyClient* pProxyClient = (CProxyClient* )f->second;

			mapProxy2ConnectID::iterator fb = m_mapProxy2ConnectID.find(pProxyClient);
			if(fb != m_mapProxy2ConnectID.end())
			{
				m_mapProxy2ConnectID.erase(fb);
			}

			m_mapConnectID2Proxy.erase(f);
		}
	}



private:
	mapProxy2ConnectID m_mapProxy2ConnectID;
	mapConnectID2Proxy m_mapConnectID2Proxy;
	ACE_Recursive_Thread_Mutex m_ThreadLock;
};

//工作线程对应的连接结构
struct _ProxyClientConnector
{
	CProxyClientConnector    m_objReactorConnect;
	ACE_Reactor*             m_pReactor;
	CProxyClientManager*     m_ProxyClientManager;
	bool                     m_blThreadRun;

	_ProxyClientConnector()
	{
		m_pReactor    = NULL;
		m_blThreadRun = false;
	}
};


class CProxyClientThreadManager
{
public:
	CProxyClientThreadManager()
	{

	};

	~CProxyClientThreadManager() 
	{
		//Close();
	};

	void Close()
	{
		OUR_DEBUG((LM_ERROR, "[CProxyClientThreadManager::Close]Begin.\n"));
		ACE_Time_Value tvSleep(0, 10000); 
		for(int i = 0; i < (int)m_vecProxyClientManager.size(); i++)
		{
			m_vecProxyClientManager[i]->m_blThreadRun = false;
			m_vecProxyClientManager[i]->m_pReactor->end_reactor_event_loop();
			m_vecProxyClientManager[i]->m_pReactor->close();
			OUR_DEBUG((LM_ERROR, "[CProxyClientThreadManager::Close](%d) is close.\n", i));
			ACE_OS::sleep(tvSleep);
			delete m_vecProxyClientManager[i]->m_pReactor;
		}

		m_vecProxyClientManager.clear();
		OUR_DEBUG((LM_ERROR, "[CProxyClientThreadManager::Close]End.\n"));
	}

	void Init(int nThreadCount);

	CProxyClientManager* GetProxyClientManager(uint32 u4ThreadCount)
	{
		if(u4ThreadCount >= (uint32)m_vecProxyClientManager.size())
		{
			return NULL;
		}
		else
		{
			return (CProxyClientManager* )m_vecProxyClientManager[u4ThreadCount]->m_ProxyClientManager;
		}
	}

	ACE_Reactor* GetProxyClientReactor(uint32 u4ThreadCount)
	{
		if(u4ThreadCount >= (uint32)m_vecProxyClientManager.size())
		{
			return NULL;
		}
		else
		{
			return (ACE_Reactor* )m_vecProxyClientManager[u4ThreadCount]->m_pReactor;
		}
	}

	CProxyClientConnector* GetProxyClientConnector(uint32 u4ThreadCount)
	{
		if(u4ThreadCount >= (uint32)m_vecProxyClientManager.size())
		{
			return NULL;
		}
		else
		{
			return (CProxyClientConnector* )&m_vecProxyClientManager[u4ThreadCount]->m_objReactorConnect;
		}
	}

private:
	typedef vector<_ProxyClientConnector*> vecProxyClientManager;
	vecProxyClientManager m_vecProxyClientManager;
};

typedef ACE_Singleton<CProxyClientThreadManager, ACE_Recursive_Thread_Mutex> App_ProxyThreadManager;

#endif
