#pragma once

#include "ProxyManager.h"

//定义客户端信令(TCP)
#define COMMAND_BASE            0x1000
#define COMMAND_RETURN_ALIVE    0xf001

#define MESSAGE_FUNCTION_BEGIN(x) switch(x) {
#define MESSAGE_FUNCTION(x,y,z) case x: { y(z); break; }
#define MESSAGE_FUNCTION_END }

class CBaseCommand : public CClientCommand
{
public:
    CBaseCommand(void);
    ~CBaseCommand(void);

    int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
    void SetServerObject(CServerObject* pServerObject);
    void InitServer(const char* pModuleName);

private:
    int Do_Connect(IMessage* pMessage);
    int Do_DisConnect(IMessage* pMessage);
    int Do_ClientSendTimeout(IMessage* pMessage);
    int Do_PostBase(IMessage* pMessage);

    void AddClient2Server(uint32 u4ClientID);
    void DelClient2Server(uint32 u4ClientID);
    CPostServerData* GetClient2Server_ServerID(uint32 u4ClientID);

private:
    CServerObject*   m_pServerObject;
    int              m_nCount;
    CProxyManager    m_objProxyManager;        //客户端和服务器映射关系
};
