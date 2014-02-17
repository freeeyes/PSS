#ifndef _REUDPMANAGER_H
#define _REUDPMANAGER_H

#include "ReactorUDPHander.h"
#include "IUDPConnectManager.h"
#include <vector>

using namespace std;

class CReUDPManager : public IUDPConnectManager
{
public:
	CReUDPManager() {};
	~CReUDPManager() {};

	CReactorUDPHander* Create()
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
		CReactorUDPHander* pReactorUDPHandler = new CReactorUDPHander();
		if(NULL != pReactorUDPHandler)
		{
			m_vecReactorUDPHandler.push_back(pReactorUDPHandler);
			return pReactorUDPHandler;
		}

		return NULL;
	};

	void Close()
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
		for(int i = 0; i < (int)m_vecReactorUDPHandler.size(); i++)
		{
			CReactorUDPHander* pReactorUDPHandler = m_vecReactorUDPHandler[i];
			pReactorUDPHandler->Close();
			SAFE_DELETE(pReactorUDPHandler);
		}
	};

	void GetClientConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
		VecClientConnectInfo.clear();
		for(int i = 0; i < (int)m_vecReactorUDPHandler.size(); i++)
		{
			CReactorUDPHander* pReactorUDPHandler = m_vecReactorUDPHandler[i];
			if(NULL != pReactorUDPHandler)
			{
				VecClientConnectInfo.push_back(pReactorUDPHandler->GetClientConnectInfo());
			}
		}
	}

	bool SendMessage(int nConnectID, const char* pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead = true, uint16 u2CommandID = 0)
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
		if(nConnectID >= (int)m_vecReactorUDPHandler.size())
		{
			OUR_DEBUG((LM_ERROR, "[CProUDPManager::SendMessage]Send (%d) UDP ConnectID is out of range.\n", nConnectID));
			return false;
		}

		CReactorUDPHander* pReactorUDPHandler = (CReactorUDPHander* )m_vecReactorUDPHandler[nConnectID];
		if(NULL != pReactorUDPHandler)
		{
			return pReactorUDPHandler->SendMessage(pMessage, u4Len, szIP, nPort, blHead, u2CommandID);
		}
		else
		{
			return false;
		}
	};

private:
	typedef vector<CReactorUDPHander*> vecReactorUDPHandler;
	vecReactorUDPHandler        m_vecReactorUDPHandler;
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;       //线程锁，保证多线程调用的一致性
};


typedef ACE_Singleton<CReUDPManager, ACE_Null_Mutex> App_ReUDPManager;

#endif
