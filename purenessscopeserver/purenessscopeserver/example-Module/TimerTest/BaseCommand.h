#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

#include <string>

//定义客户端信令(TCP)
#define COMMAND_TIMER           0x1100
#define COMMAND_RETURN_TIMER    0xf100

//定时定时器时间间隔，单位秒
#define MAX_TIMER_INTERVAL   10

#define MESSAGE_FUNCTION_BEGIN(x) switch(x) {
#define MESSAGE_FUNCTION(x,y,z) case x: { y(z); break; }
#define MESSAGE_FUNCTION_END }

using namespace std;

class CBaseCommand : public CClientCommand, public ACE_Task<ACE_MT_SYNCH>
{
public:
    CBaseCommand(void);
    ~CBaseCommand(void);

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);

    int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
    void SetServerObject(CServerObject* pServerObject);

    void Init();

    void Fini();

private:
    int Do_Connect(IMessage* pMessage);
    int Do_DisConnect(IMessage* pMessage);
    int Do_Base(IMessage* pMessage);

private:
    CServerObject* m_pServerObject;
    int            m_nCount;
    uint32         m_u4TimeID;
};
