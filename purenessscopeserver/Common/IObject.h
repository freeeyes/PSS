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
#include "IModuleMessageManager.h"
#include "IControlListen.h"
#include "IModuleInfo.h"
#include "IServerManager.h"
#include "IMessageBlockManager.h"
#include "IFrameCommand.h"
#include "ITTyClientManager.h"
#include "IEchartlog.h"

class CServerObject
{
public:
    CServerObject()
    {
        m_pIMessageManager      = nullptr;
        m_pLogManager           = nullptr;
        m_pConnectManager       = nullptr;
        m_pPacketManager        = nullptr;
        m_pClientManager        = nullptr;
        m_pModuleMessageManager = nullptr;
        m_pContorlListen        = nullptr;
        m_pIServerManager       = nullptr;
        m_pMessageBlockManager  = nullptr;
        m_pIModuleInfo          = nullptr;
        m_pFrameCommand         = nullptr;
        m_pTTyClientManager     = nullptr;
        m_pEchartlog            = nullptr;
    }

    virtual ~CServerObject() {}
    void SetMessageManager(IMessageManager* pIMessageManager)
    {
        m_pIMessageManager = pIMessageManager;
    }
    void SetLogManager(ILogManager* pLogManager)
    {
        m_pLogManager = pLogManager;
    }
    void SetConnectManager(IConnectManager* pConnectManager)
    {
        m_pConnectManager = pConnectManager;
    }
    void SetPacketManager(IPacketManager*  pPacketManager)
    {
        m_pPacketManager = pPacketManager;
    }
    void SetClientManager(IClientManager* pClientManager)
    {
        m_pClientManager = pClientManager;
    }
    void SetModuleMessageManager(IModuleMessageManager* pModuleMessageManager)
    {
        m_pModuleMessageManager = pModuleMessageManager;
    }
    void SetControlListen(IControlListen* pControlListen)
    {
        m_pContorlListen = pControlListen;
    }
    void SetModuleInfo(IModuleInfo* pModuleInfo)
    {
        m_pIModuleInfo = pModuleInfo;
    }
    void SetServerManager(IServerManager* pIServerManager)
    {
        m_pIServerManager = pIServerManager;
    }
    void SetMessageBlockManager(IMessageBlockManager* pMessageBlockManager)
    {
        m_pMessageBlockManager = pMessageBlockManager;
    }

    void SetFrameCommand(IFrameCommand* pFrameCommand)
    {
        m_pFrameCommand = pFrameCommand;
    }

    void SetTTyClientManager(ITTyClientManager* pTTyClientManager)
    {
        m_pTTyClientManager = pTTyClientManager;
    }

    void SetEchartlog(IEchartlog* pEchartlog)
    {
        m_pEchartlog = pEchartlog;
    }

    IMessageManager*       GetMessageManager()
    {
        return m_pIMessageManager;
    }
    ILogManager*           GetLogManager()
    {
        return m_pLogManager;
    }
    IConnectManager*       GetConnectManager()
    {
        return m_pConnectManager;
    }
    IPacketManager*        GetPacketManager()
    {
        return m_pPacketManager;
    }
    IClientManager*        GetClientManager()
    {
        return m_pClientManager;
    }
    IModuleMessageManager* GetModuleMessageManager()
    {
        return m_pModuleMessageManager;
    }
    IControlListen*        GetControlListen()
    {
        return m_pContorlListen;
    }
    IModuleInfo*           GetModuleInfo()
    {
        return m_pIModuleInfo;
    }
    IServerManager*        GetServerManager()
    {
        return m_pIServerManager;
    }
    IMessageBlockManager*  GetMessageBlockManager()
    {
        return m_pMessageBlockManager;
    }

    IFrameCommand* GetFrameCommand()
    {
        return m_pFrameCommand;
    }

    ITTyClientManager* GetTTyClientManager()
    {
        return m_pTTyClientManager;
    }

    IEchartlog* GetEchartlog()
    {
        return m_pEchartlog;
    }

private:
    IMessageManager*       m_pIMessageManager;
    ILogManager*           m_pLogManager;
    IConnectManager*       m_pConnectManager;
    IPacketManager*        m_pPacketManager;
    IClientManager*        m_pClientManager;
    IModuleMessageManager* m_pModuleMessageManager;
    IControlListen*        m_pContorlListen;
    IModuleInfo*           m_pIModuleInfo;
    IServerManager*        m_pIServerManager;
    IMessageBlockManager*  m_pMessageBlockManager;
    IFrameCommand*         m_pFrameCommand;
    ITTyClientManager*     m_pTTyClientManager;
    IEchartlog*            m_pEchartlog;
};

using App_ServerObject = PSS_singleton<CServerObject>;

#endif
