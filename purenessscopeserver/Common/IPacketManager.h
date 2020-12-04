#ifndef _IPACKETMANAGER_H
#define _IPACKETMANAGER_H

#include "IBuffPacket.h"

class IPacketManager
{
public:
    virtual ~IPacketManager() {}

    virtual shared_ptr<IBuffPacket> Create(const char* pFileName = __FILE__, uint32 u4Line = __LINE__) = 0;
};

#endif
