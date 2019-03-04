#ifndef _PSSMONITOR_H
#define _PSSMONITOR_H

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "Trace.h"
#include "DataManager.h"

#define COMMAND_MONITOR_LOGIN      0x2001    //发送注册命令
#define COMMAND_MONITOR_DATA       0x2002    //发送心跳当前数据吞吐命令
#define COMMAND_MONITOR_LOGIN_ACK  0x8001    //注册命令回应
#define COMMAND_MONITOR_DATA_ACK   0x8002    //当前数据吞吐命令回应

#define MESSAGE_FUNCTION_BEGIN(x) switch(x) {
#define MESSAGE_FUNCTION(x,y,z) case x: { y(z); break; }
#define MESSAGE_FUNCTION_END }

class CPSSMonitorCommand : public CClientCommand
{
public:
    CPSSMonitorCommand();
    ~CPSSMonitorCommand();

    int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
    void SetServerObject(CServerObject* pServerObject);
    CServerObject* GetServerObject();

private:
    int Do_Pss_Login(IMessage* pMessage);
    int Do_Pss_Monitor_Data(IMessage* pMessage);

private:
    CServerObject* m_pServerObject;
};

#endif
