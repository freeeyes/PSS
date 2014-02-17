#ifndef _MODULEMESSAGEMANAGER_H
#define _MODULEMESSAGEMANAGER_H

#include "IModuleMessageManager.h"
#include "BuffPacketManager.h"
#include "LoadModule.h"

class CModuleMessageManager : public IModuleMessageManager
{
public:
	CModuleMessageManager();
	virtual ~CModuleMessageManager();

	int SendModuleMessage(const char* pModuleName, uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);
};

typedef ACE_Singleton<CModuleMessageManager, ACE_Null_Mutex> App_ModuleMessageManager;

#endif
