#ifndef _MODULEMESSAGEMANAGER_H
#define _MODULEMESSAGEMANAGER_H

#include "IModuleMessageManager.h"
#include "BuffPacketManager.h"
#include "LoadModule.h"
#include "MakePacket.h"

class CModuleMessageManager : public IModuleMessageManager
{
public:
    CModuleMessageManager() = default;
    ~CModuleMessageManager() final = default;

    int SendModuleMessage(const char* pModuleName, uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket) final;
    int SendFrameMessage(uint16 u2CommandID, uint32 u4WorkThreadID, shared_ptr<IBuffPacket> pHeadPacket, shared_ptr<IBuffPacket> pBodyBuffPacket) final;
};

using App_ModuleMessageManager = ACE_Singleton<CModuleMessageManager, ACE_Null_Mutex>;

#endif
