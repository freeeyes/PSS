#ifndef _ILOGMANAGER_H
#define _ILOGMANAGER_H

#include "ILogObject.h"

class ILogManager
{
public:
    virtual ~ILogManager() {}
    //�ı���־��¼
    virtual int WriteLog(int nLogType, const char* fmt, ...)              = 0;
    //��������־��¼
    virtual int WriteLogBinary(int nLogType, const char* pData, int nLen) = 0;
    //�����ʼ���¼
    virtual int WriteToMail(int nLogType, uint32 u4MailID, const char* pTitle, const char* fmt, ...) = 0;
};

#endif
