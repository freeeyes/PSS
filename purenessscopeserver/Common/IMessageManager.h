#ifndef _IMESSAGEMANAGER_H
#define _IMESSAGEMANAGER_H

#include "ClientCommand.h"
#include "IMessage.h"

class IMessageManager
{
public:
    virtual ~IMessageManager() {}

    virtual bool AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName, _ClientIPInfo objListenInfo) = 0;
    virtual bool AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName)                              = 0;
    virtual bool DelClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand)                                                       = 0;

    virtual uint32 GetWorkThreadCount()                 = 0;
    virtual uint32 GetWorkThreadByIndex(uint32 u4Index) = 0;
};

#endif
