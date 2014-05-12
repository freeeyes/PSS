#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
	m_pServerObject = NULL;
	m_nCount        = 0;
}

CBaseCommand::~CBaseCommand(void)
{
	delete m_pPostServerData1;
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

	//处理链接建立信息
	if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CONNECT)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CONNECT OK.\n"));
	}

	//处理链接断开信息
	if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CDISCONNET)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));
	}

	//处理正常信息
	if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_BASE)
	{
		uint32     u4PacketLen = 0;
		uint16     u2CommandID = 0;
		//VCHARS_STR strsName;
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
		//(*pBodyPacket) >> strsName;
		//strName.assign(strsName.text, strsName.u1Len);
		
		//往中间服务器发送消息(测试数据透传)
		char szPostData[2048] = {'\0'};
		//sprintf_safe(szPostData, MAX_BUFF_100, "hello world");
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CommandID= 0x%04x, m_nDataLen=%d.\n", u2CommandID, BodyPacket.m_nDataLen));
		int nSendSize = BodyPacket.m_nDataLen + sizeof(uint32);
		ACE_OS::memcpy(szPostData, &BodyPacket.m_nDataLen, sizeof(uint32));
		ACE_OS::memcpy(&szPostData[sizeof(uint32)], BodyPacket.m_pData, BodyPacket.m_nDataLen);		

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		//设置当前接收数据的ConnectID，用于收到远程回应信息返回
		m_pPostServerData1->SetConnectID(pMessage->GetMessageBase()->m_u4ConnectID);
		if(false == m_pServerObject->GetClientManager()->SendData(1, szPostData, nSendSize, false))
		{
			OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] Send Post Data Error.\n"));
			return 0;
		}
	}


	return 0;
}

void CBaseCommand::InitServer()
{
	m_pPostServerData1 = new CPostServerData();

	//设置返回客户端需要的发送对象
	m_pPostServerData1->SetServerObject(m_pServerObject);

	//初始化连接关系
	m_pServerObject->GetClientManager()->Connect(1, "172.21.1.200", 10040, TYPE_IPV4, (IClientMessage* )m_pPostServerData1);

}


