#ifndef _PSS_HANDLER_MANAGER_H
#define _PSS_HANDLER_MANAGER_H

#include "IConnectManager.h"
#include "MessageService.h"

class CPSSHandlerManager : public IConnectManager
{
public:
	bool PostMessage(uint32 u4ConnectID, uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, CSend_Param objSendParam) final;
	
	bool CloseConnect(uint32 u4ConnectID) final;

	_ClientIPInfo GetClientIPInfo(uint32 u4ConnectID) final;

	_ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID) final;

	uint32 GetCount() final;

	bool SetIsLog(uint32 u4ConnectID, bool blIsLog) final;

	 EM_Client_Connect_status GetConnectState(uint32 u4ConnectID) final;
};

using App_HandlerManager = PSS_singleton<CPSSHandlerManager>;

#endif
