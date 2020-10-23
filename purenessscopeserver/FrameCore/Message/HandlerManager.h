#ifndef _PSS_HANDLER_MANAGER_H
#define _PSS_HANDLER_MANAGER_H

#include "IConnectManager.h"
#include "MessageService.h"

class CPSSHandlerManager : public IConnectManager
{
public:
	virtual bool PostMessage(uint32 u4ConnectID, uint16 u2CommandID, IBuffPacket*& pBuffPacket, CSend_Param objSendParam);
	
	virtual bool CloseConnect(uint32 u4ConnectID);

	virtual _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);

	virtual _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID);

	virtual uint32 GetCount();

	virtual bool SetIsLog(uint32 u4ConnectID, bool blIsLog);

	virtual EM_Client_Connect_status GetConnectState(uint32 u4ConnectID);
};

typedef ACE_Singleton<CPSSHandlerManager, ACE_Null_Mutex> App_HandlerManager;

#endif
