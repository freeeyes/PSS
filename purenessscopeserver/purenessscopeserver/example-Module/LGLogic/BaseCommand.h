#pragma once

#include "ClientCommand.h"
#include "LSServerManager.h"

#include <string>

//定义客户端信令(TCP)
#define COMMAND_LOGIC_CLIENAT_LOGIN   0x1001      //客户端数据
#define COMMAND_LOGIC_CLIENAT_LOGIN_R 0xf001      //客户端数据

#define LG_SERVER_KEY  "TESTSERVER1"

//定时定时器时间间隔，单位秒
#define MAX_TIMER_INTERVAL   30

#define MESSAGE_FUNCTION_BEGIN(x) switch(x) {
#define MESSAGE_FUNCTION(x,y,z) case x: { y(z); break; }
#define MESSAGE_FUNCTION_END }

using namespace std;

class CBaseCommand : public CClientCommand, public ACE_Task<ACE_MT_SYNCH>
{
public:
    CBaseCommand(void);
    ~CBaseCommand(void);

    //发送心跳包
    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);

    int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
    void SetServerObject(CServerObject* pServerObject);

    void Init(const char* pFileName);

private:
    int Do_Connect(IMessage* pMessage);
    int Do_DisConnect(IMessage* pMessage);
    int Do_ClientSendTimeout(IMessage* pMessage);
    int Do_Logic_Client_Login(IMessage* pMessage);

    void ReadIniFile(const char* pIniFileName);

private:
    CServerObject*    m_pServerObject;
    int               m_nCount;
    CLSServerManager  m_objLSServer;
    uint32            m_u4TimeID;

    uint32            m_u4LSServerID;
    char              m_szLSIP[50];
    uint32            m_u4LSPort;
};
