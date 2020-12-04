#include "BuffPacketManager.h"

shared_ptr<IBuffPacket> CBuffPacketManager::Create(const char* pFileName, uint32 u4Line)
{
    ACE_UNUSED_ARG(pFileName);
    ACE_UNUSED_ARG(u4Line);

    auto pBuffPacket = std::make_shared<CBuffPacket>();
    pBuffPacket->Init();
    return pBuffPacket;
}

