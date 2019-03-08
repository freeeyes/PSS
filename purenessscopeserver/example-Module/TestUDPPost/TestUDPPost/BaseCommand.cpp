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
		VCHARS_STR strsName;
		string     strName;

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
		(*pBodyPacket) >> strsName;
		strName.assign(strsName.text, strsName.u1Len);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		//���м������������Ϣ
		char szPostData[MAX_BUFF_100] = {'\0'};
		sprintf_safe(szPostData, MAX_BUFF_100, "hello");
        char *PostData = szPostData;
		if(false == m_pServerObject->GetClientManager()->SendDataUDP(1, "127.0.0.1", 10006, PostData, (int)ACE_OS::strlen(szPostData)))
		{
			OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] Send Post Data Error.\n"));
			return 0;
		}
	}


	return 0;
}

void CBaseCommand::InitServer()
{
	CPostServerData* pPostServerData = new CPostServerData();
	if(NULL != pPostServerData)
	{
		m_pServerObject->GetClientManager()->ConnectUDP(1, "127.0.0.1", 10005, TYPE_IPV4, UDP_SINGLE, pPostServerData);
	}
}


