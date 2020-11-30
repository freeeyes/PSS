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

    int SendModuleMessage(const char* pModuleName, uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket) final;
    int SendFrameMessage(uint16 u2CommandID, uint32 u4WorkThreadID, IBuffPacket* pHeadPacket, IBuffPacket* pBodyBuffPacket) final;
};

using App_ModuleMessageManager = ACE_Singleton<CModuleMessageManager, ACE_Null_Mutex>;

#endif
