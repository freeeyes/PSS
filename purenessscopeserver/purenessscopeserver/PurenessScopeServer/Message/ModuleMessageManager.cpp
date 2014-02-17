#include "ModuleMessageManager.h"

CModuleMessageManager::CModuleMessageManager()
{

}


CModuleMessageManager::~CModuleMessageManager()
{

}

int CModuleMessageManager::SendModuleMessage(const char* pModuleName, uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket)
{
	return App_ModuleLoader::instance()->SendModuleMessage(pModuleName, u2CommandID, pBuffPacket, pReturnBuffPacket);
}
