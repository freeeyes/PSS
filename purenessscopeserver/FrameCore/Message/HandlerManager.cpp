#include "HandlerManager.h"

bool CPSSHandlerManager::PostMessage(uint32 u4ConnectID, uint16 u2CommandID, IBuffPacket*& pBuffPacket, CSend_Param objSendParam)
{
	if (u4ConnectID > 0)
	{
		//这个是TCP链接，找到这个Handler所在的处理线程
		CSendMessageInfo objSendMessageInfo;

		objSendMessageInfo.blDelete    = objSendParam.m_blDlete;
		objSendMessageInfo.nMessageID  = objSendParam.m_nMessageID;
		objSendMessageInfo.emSendType  = objSendParam.m_emSendType;
		objSendMessageInfo.emSendState = objSendParam.m_emSendState;
		objSendMessageInfo.pBuffPacket = pBuffPacket;
		objSendMessageInfo.u2CommandID = u2CommandID;
		objSendMessageInfo.u4ConnectID = u4ConnectID;

		return App_MessageServiceGroup::instance()->Send_Post_Message(objSendMessageInfo);
	}
	else
	{
		//这个是UDP链接

		return true;
	}
}

bool CPSSHandlerManager::CloseConnect(uint32 u4ConnectID)
{
	//待实现
	return true;
}

_ClientIPInfo CPSSHandlerManager::GetClientIPInfo(uint32 u4ConnectID)
{
	return App_MessageServiceGroup::instance()->GetClientIPInfo(u4ConnectID);
}

_ClientIPInfo CPSSHandlerManager::GetLocalIPInfo(uint32 u4ConnectID)
{
	return App_MessageServiceGroup::instance()->GetLocalIPInfo(u4ConnectID);
}

uint32 CPSSHandlerManager::GetCount()
{
	return App_MessageServiceGroup::instance()->GetHandlerCount();
}

bool CPSSHandlerManager::SetIsLog(uint32 u4ConnectID, bool blIsLog)
{
	//待实现
	return true;
}

EM_Client_Connect_status CPSSHandlerManager::GetConnectState(uint32 u4ConnectID)
{
	//待实现
	return EM_Client_Connect_status::CLIENT_CONNECT_EXIST;
}
