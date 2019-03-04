#ifndef _IMESSAGEBLOCKMANAGER_H
#define _IMESSAGEBLOCKMANAGER_H

#include "define.h"
#include "ace/Message_Block.h"

using namespace PSS;

class IMessageBlockManager
{
public:
    virtual ~IMessageBlockManager() {}
    virtual ACE_Message_Block* Create(uint32 u4Size)     = 0;
    virtual bool Close(ACE_Message_Block* pMessageBlock) = 0;
};

#endif
