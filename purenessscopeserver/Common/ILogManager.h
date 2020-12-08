#ifndef _ILOGMANAGER_H
#define _ILOGMANAGER_H

#include "ILogObject.h"

class ILogManager
{
public:
    virtual ~ILogManager() {}

    //二进制日志记录
    virtual int WriteLogBinary(uint16 u2LogType, const string& strLog) = 0;

    //写入日志接口
    virtual int WriteLog_r(uint16 u2LogType, const string& strLog) = 0;

    //写入邮件接口
    virtual int WriteToMail_r(uint16 u2LogType, uint16 u2MailID, const string& strTitle, const string& strLog) = 0;
};

#endif
