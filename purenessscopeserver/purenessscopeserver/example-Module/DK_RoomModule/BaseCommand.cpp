#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
	m_pServerObject = NULL;
}

CBaseCommand::~CBaseCommand(void)
{
}

bool CBaseCommand::Init(ActiveTimer* pActiveTimer)
{
	m_objSenceManager.Init(pActiveTimer, this);

	return true;
}

void CBaseCommand::SetServerObject(CServerObject* pServerObject)
{
	m_pServerObject = pServerObject;
}

int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{	
	if(m_pServerObject == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] m_pServerObject is NULL.\n"));
		return -1;
	}

	if(pMessage == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pMessage is NULL.\n"));
		return -1;
	}

	if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_INITROOM)
	{
		uint16     u2CommandID  = 0;
		uint32     u4PlayerID   = PLAYER_INVALID_ID;
		char       szPlayerNick[MAX_BUFF_20] = {'\0'};
		
		//OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));
		
		IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();
		if(NULL == pBodyPacket)
		{
			OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
			return -1;
		}
		
		_PacketInfo BodyPacket;
		pMessage->GetPacketBody(BodyPacket);

		pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

		(*pBodyPacket) >> u2CommandID;
		(*pBodyPacket) >> u4PlayerID;
		
		_CommandInfo objCommandInfo;

		objCommandInfo.u4PlayerID = u4PlayerID; 
		App_SenceManager::instance()->PutMessage(pMessage->GetMessageBase()->m_u2Cmd, objCommandInfo);		
	}
	else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_SETROOM)
	{
		uint16     u2CommandID  = 0;
		uint32     u4PlayerID   = PLAYER_INVALID_ID;

		IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();
		if(NULL == pBodyPacket)
		{
			OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
			return -1;
		}

		_PacketInfo BodyPacket;
		pMessage->GetPacketBody(BodyPacket);

		pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

		(*pBodyPacket) >> u2CommandID;
		(*pBodyPacket) >> u4PlayerID;
		 _CommandInfo objCommandInfo;

		 objCommandInfo.u4PlayerID = u4PlayerID; 
		 App_SenceManager::instance()->PutMessage(pMessage->GetMessageBase()->m_u2Cmd, objCommandInfo);
	}


	return 0;
}

bool CBaseCommand::Module_Login(uint32 u4Pos, uint32 u4PlayerID, uint32 u4ConnectID)
{
	mapConnectPlayer::iterator f = m_mapOnlineUserPlayer.find(u4PlayerID);
	if(f == m_mapOnlineUserPlayer.end())
	{
		m_mapOnlineUserPlayer.insert(mapConnectPlayer::value_type(u4PlayerID, u4ConnectID));
	}

	//场景添加在线人物
	App_SenceManager::instance()->AddSencePlayer(u4PlayerID, u4Pos);

	return true;
}

bool CBaseCommand::Module_Logout(uint32 u4PlayerID, uint32 u4ConnectID)
{
	mapConnectPlayer::iterator f = m_mapOnlineUserPlayer.find(u4PlayerID);
	if(f != m_mapOnlineUserPlayer.end())
	{
		uint32 u4ServerPlayerID = (uint32)f->second;
		if(u4PlayerID == u4ServerPlayerID)
		{
			m_mapOnlineUserPlayer.erase(f);
		}
	}

	//场景删除在线人物
	App_SenceManager::instance()->DelSencePlayer(u4PlayerID);

	return true;
}

bool CBaseCommand::SendRoomState(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomInfo& objSendRoomInfo)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

	bool blState = objSendRoomInfo.SetStream(u2CommandID, pResponsesPacket);
	if(true == blState)
	{
		mapConnectPlayer::iterator f = m_mapOnlineUserPlayer.find(u4PlayerID);
		if(f != m_mapOnlineUserPlayer.end())
		{
			if(NULL != m_pServerObject->GetConnectManager())
			{
				//发送全部数据
				m_pServerObject->GetConnectManager()->PostMessage((uint32)f->second, pResponsesPacket, SENDMESSAGE_NOMAL, u2CommandID, true);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
	}

	return true;
}

bool CBaseCommand::SendRoomSitdown(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomPlayerInfo& objSendRoomPlayerInfo)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

	bool blState = objSendRoomPlayerInfo.SetStream(u2CommandID, pResponsesPacket);
	if(true == blState)
	{
		mapConnectPlayer::iterator f = m_mapOnlineUserPlayer.find(u4PlayerID);
		if(f != m_mapOnlineUserPlayer.end())
		{
			if(NULL != m_pServerObject->GetConnectManager())
			{
				//发送全部数据
				m_pServerObject->GetConnectManager()->PostMessage((uint32)f->second, pResponsesPacket, SENDMESSAGE_NOMAL, u2CommandID, true);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
	}

	return true;
}

bool CBaseCommand::SendRoomTimer(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomTimer& objSendRoomTimer)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

	bool blState = objSendRoomTimer.SetStream(u2CommandID, pResponsesPacket);
	if(true == blState)
	{
		mapConnectPlayer::iterator f = m_mapOnlineUserPlayer.find(u4PlayerID);
		if(f != m_mapOnlineUserPlayer.end())
		{
			if(NULL != m_pServerObject->GetConnectManager())
			{
				//发送全部数据
				m_pServerObject->GetConnectManager()->PostMessage((uint32)f->second, pResponsesPacket, SENDMESSAGE_NOMAL, u2CommandID, true);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
	}

	return true;
}

bool CBaseCommand::SendRoomPlayerTimer(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomPlayerTimer objSendRoomPlayerTimer)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

	bool blState = objSendRoomPlayerTimer.SetStream(u2CommandID, pResponsesPacket);
	if(true == blState)
	{
		mapConnectPlayer::iterator f = m_mapOnlineUserPlayer.find(u4PlayerID);
		if(f != m_mapOnlineUserPlayer.end())
		{
			if(NULL != m_pServerObject->GetConnectManager())
			{
				//发送全部数据
				m_pServerObject->GetConnectManager()->PostMessage((uint32)f->second, pResponsesPacket, SENDMESSAGE_NOMAL, u2CommandID, true);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
	}

	return true;
}

bool CBaseCommand::SendRoomPlayerMoney(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomMoney objSendRoomMoney)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

	bool blState = objSendRoomMoney.SetStream(u2CommandID, pResponsesPacket);
	if(true == blState)
	{
		mapConnectPlayer::iterator f = m_mapOnlineUserPlayer.find(u4PlayerID);
		if(f != m_mapOnlineUserPlayer.end())
		{
			if(NULL != m_pServerObject->GetConnectManager())
			{
				//发送全部数据
				m_pServerObject->GetConnectManager()->PostMessage((uint32)f->second, pResponsesPacket, SENDMESSAGE_NOMAL, u2CommandID, true);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
	}

	return true;
}

bool CBaseCommand::SendRoomTableCard(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomCodeInfo objSendRoomCodeInfo)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

	bool blState = objSendRoomCodeInfo.SetStream(u2CommandID, pResponsesPacket);
	if(true == blState)
	{
		mapConnectPlayer::iterator f = m_mapOnlineUserPlayer.find(u4PlayerID);
		if(f != m_mapOnlineUserPlayer.end())
		{
			if(NULL != m_pServerObject->GetConnectManager())
			{
				//发送全部数据
				m_pServerObject->GetConnectManager()->PostMessage((uint32)f->second, pResponsesPacket, SENDMESSAGE_NOMAL, u2CommandID, true);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
	}

	return true;
}


