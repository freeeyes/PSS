// IObject.h
// 虚接口化所有DLL用到的类
// add by freeeyes
// 2008-04-06

#ifndef _IOBJECT_H
#define _IOBJECT_H

#include "IMessage.h"
#include "IMessageManager.h"
#include "ILogManager.h"
#include "IConnectManager.h"
#include "IPacketManager.h"
#include "IClientManager.h"
#include "IUDPConnectManager.h"
#include "ITimerManager.h"
#include "IModuleMessageManager.h"

class CServerObject
{
public:
	CServerObject() 
	{ 
		m_pIMessageManager      = NULL;
		m_pLogManager           = NULL;
		m_pConnectManager       = NULL;
		m_pPacketManager        = NULL;
		m_pClientManager        = NULL;
		m_pUDPConnectManager    = NULL;
		m_pModuleMessageManager = NULL;
	}

	~CServerObject() {};
	void SetMessageManager(IMessageManager* pIMessageManager) { m_pIMessageManager = pIMessageManager; };
	void SetLogManager(ILogManager* pLogManager) { m_pLogManager = pLogManager; };
	void SetConnectManager(IConnectManager* pConnectManager) { m_pConnectManager = pConnectManager; };
	void SetPacketManager(IPacketManager*  pPacketManager) { m_pPacketManager = pPacketManager; };
	void SetClientManager(IClientManager* pClientManager) { m_pClientManager = pClientManager; }
	void SetUDPConnectManager(IUDPConnectManager* pUDPConnectManager) { m_pUDPConnectManager = pUDPConnectManager; };
	void SetTimerManager(ActiveTimer* pTimerManager) { m_pTimerManager = pTimerManager; }
	void SetModuleMessageManager(IModuleMessageManager* pModuleMessageManager) { m_pModuleMessageManager = pModuleMessageManager; }

	IMessageManager*       GetMessageManager() { return m_pIMessageManager; };
	ILogManager*           GetLogManager() { return m_pLogManager; };
	IConnectManager*       GetConnectManager() { return m_pConnectManager; };
	IPacketManager*        GetPacketManager() { return m_pPacketManager; };
	IClientManager*        GetClientManager() { return m_pClientManager; }
	IUDPConnectManager*    GetUDPConnectManager() { return m_pUDPConnectManager; };
	ActiveTimer*           GetTimerManager() { return m_pTimerManager; }
	IModuleMessageManager* GetModuleMessageManager() { return m_pModuleMessageManager; }

private:
	IMessageManager*       m_pIMessageManager;
	ILogManager*           m_pLogManager;
	IConnectManager*       m_pConnectManager;
	IPacketManager*        m_pPacketManager;
	IClientManager*        m_pClientManager;
	IUDPConnectManager*    m_pUDPConnectManager;
	ActiveTimer*           m_pTimerManager;
	IModuleMessageManager* m_pModuleMessageManager;
};

typedef ACE_Singleton<CServerObject, ACE_Null_Mutex> App_ServerObject; 

#endif
