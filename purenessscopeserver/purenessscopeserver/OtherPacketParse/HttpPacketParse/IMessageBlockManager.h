#ifndef _IMESSAGEBLOCKMANAGER_H
#define _IMESSAGEBLOCKMANAGER_H

#include "define.h"
#include "ace/Message_Block.h"

class IMessageBlockManager
{
public:
	virtual ACE_Message_Block* Create(uint32 u4Size) = 0;
};

#endif
