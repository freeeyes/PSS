#ifndef _ITTYMESSAGE_H
#define _ITTYMESSAGE_H

#include <stdio.h>
#include "define.h"

enum EM_TTY_EVENT_TYPE
{
    EM_TTY_EVENT_RW_ERROR = 0,
    EM_TTY_EVENT_CONNECT,
};

//文件描述符消息处理类
class ITTyMessage
{
public:
    virtual ~ITTyMessage() {};
    virtual bool RecvData(uint16 u2ConnectID, const char* pData, uint32 u4len) = 0;
    virtual bool ReportMessage(uint16 u2ConnectID, uint32 u4ErrorNo, EM_TTY_EVENT_TYPE em_type) = 0;
};

#endif
