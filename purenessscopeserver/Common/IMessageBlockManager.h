#ifndef _IMESSAGEBLOCKMANAGER_H
#define _IMESSAGEBLOCKMANAGER_H

#include "define.h"
#include "ace/Message_Block.h"

class IMessageBlockManager
{
public:
    virtual ~IMessageBlockManager() {}
    virtual ACE_Message_Block* Create(uint32 u4Size)                            = 0;
    virtual bool Close(ACE_Message_Block* pMessageBlock, uint32 u4BlockLen = 0) = 0;
};

#endif
