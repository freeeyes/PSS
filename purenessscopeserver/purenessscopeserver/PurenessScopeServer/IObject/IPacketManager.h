#ifndef _IPACKETMANAGER_H
#define _IPACKETMANAGER_H

#include "IBuffPacket.h"

class IPacketManager
{
public:
	virtual ~IPacketManager() {};

	virtual IBuffPacket* Create()                 = 0;
	virtual bool Delete(IBuffPacket* pBuffPacket) = 0;
};

#endif
