#ifndef _IPACKETMANAGER_H
#define _IPACKETMANAGER_H

#include "IBuffPacket.h"

class IPacketManager
{
public:
    virtual ~IPacketManager() {}

    virtual uint32 GetBuffPacketUsedCount() = 0;
    virtual uint32 GetBuffPacketFreeCount() = 0;

    virtual IBuffPacket* Create()                     = 0;
    virtual bool Delete(IBuffPacket* pBuffPacket)     = 0;
};

#endif
