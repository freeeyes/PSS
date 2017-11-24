#ifndef _IUDPCONNECTMANAGER_H
#define _IUDPCONNECTMANAGER_H

class IUDPConnectManager
{
public:
    virtual ~IUDPConnectManager() {}
    virtual bool SendMessage(int nConnectID, const char* pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead = true, uint16 u2CommandID = 0, bool blDlete = true) = 0;
};

#endif
