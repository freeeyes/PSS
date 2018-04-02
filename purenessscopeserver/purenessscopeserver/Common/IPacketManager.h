#ifndef _IPACKETMANAGER_H
#define _IPACKETMANAGER_H

#include "IBuffPacket.h"

class IPacketManager
{
public:
    virtual ~IPacketManager() {}

    virtual uint32 GetBuffPacketUsedCount() = 0;
    virtual uint32 GetBuffPacketFreeCount() = 0;
    virtual void OutputCreateInfo() = 0;
    virtual void SetCreateFlag(bool blTagCreateInfo) = 0;

    virtual IBuffPacket* Create(const char* pFileName = __FILE__, uint32 u4Line = __LINE__) = 0;
    virtual bool Delete(IBuffPacket* pBuffPacket)     = 0;
    virtual void GetCreateInfoList(vector<_Object_Create_Info>& objCreateList) = 0;
};

#endif
