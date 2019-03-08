#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
	m_pServerObject = NULL;
	m_nCount        = 0;
}

CBaseCommand::~CBaseCommand(void)
{
}

void CBaseCommand::SetServerObject(CServerObject* pServerObject)
{
	m_pServerObject = pServerObject;
}

int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{	
	if(m_pServerObject == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] m_pServerObject is NULL(%d).\n", bDeleteFlag));
		return -1;
	}

	if(pMessage == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pMessage is NULL.\n"));
		return -1;
	}

	//�������ӽ�����Ϣ
	if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CONNECT)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CONNECT OK.\n"));
	}

	//�������ӶϿ���Ϣ
	if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CDISCONNET)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));
	}

	//����������Ϣ
	if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_BASE)
	{
		uint16     u2CommandID = 0;
		uint64     u8ClientTime = 0;

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
		(*pBodyPacket) >> u8ClientTime;

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		//ACE_Time_Value tvSleep(10);
		//ACE_OS::sleep(tvSleep);

		if(NULL != m_pServerObject->GetLogManager())
		{
			m_pServerObject->GetLogManager()->WriteLog(LOG_SYSTEM,  "[CBaseCommand::DoMessage] Get CommandID = %d", u2CommandID);
		}
		else
		{
			OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pLogManager = NULL"));
		}

		//���Է�������(UDP)
		if(NULL != m_pServerObject->GetUDPConnectManager())
		{
			char* pReturn = new char[MAX_BUFF_100];
			sprintf_safe(pReturn, MAX_BUFF_100, "Hello  friend.");
			m_pServerObject->GetUDPConnectManager()->SendMessage(0, pReturn, (uint32)ACE_OS::strlen(pReturn), pMessage->GetMessageBase()->m_szIP, 20002, false, 0x0021);
		}
	}


	return 0;
}


