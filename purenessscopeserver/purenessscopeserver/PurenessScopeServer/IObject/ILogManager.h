#ifndef _ILOGMANAGER_H
#define _ILOGMANAGER_H

#include "ILogObject.h"

class ILogManager
{
public:
    virtual ~ILogManager() {}
    //文本日志记录
    virtual int WriteLog(int nLogType, const char* fmt, ...)              = 0;
    //二进制日志记录
    virtual int WriteLogBinary(int nLogType, const char* pData, int nLen) = 0;
    //发送邮件记录
    virtual int WriteToMail(int nLogType, uint32 u4MailID, char* pTitle, const char* fmt, ...) = 0;
};

#endif
