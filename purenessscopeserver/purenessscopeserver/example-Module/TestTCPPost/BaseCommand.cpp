#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
	m_pServerObject = NULL;
	m_nCount        = 0;
}

CBaseCommand::~CBaseCommand(void)
{
	CloseClient2Server();
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

	MESSAGE_FUNCTION_BEGIN(pMessage->GetMessageBase()->m_u2Cmd);
	MESSAGE_FUNCTION(CLIENT_LINK_CONNECT,     Do_Connect,           pMessage);
	MESSAGE_FUNCTION(CLIENT_LINK_CDISCONNET,  Do_DisConnect,        pMessage);
	MESSAGE_FUNCTION(CLINET_LINK_SENDTIMEOUT, Do_ClientSendTimeout, pMessage);
	MESSAGE_FUNCTION(COMMAND_BASE,            Do_PostBase,          pMessage);
	MESSAGE_FUNCTION_END;

	return 0;
}

void CBaseCommand::InitServer()
{
}

void CBaseCommand::AddClient2Server(uint32 u4ClientID)
{
	mapc2s::iterator f = m_mapC2S.find(u4ClientID);
	if(f != m_mapC2S.end())
	{
		return;
	}

	CPostServerData* pPostServerData = new CPostServerData();

	//设置返回客户端需要的发送对象
	pPostServerData->SetServerObject(m_pServerObject);
	pPostServerData->SetConnectID(u4ClientID);
	pPostServerData->SetServerID(u4ClientID);

	//初始化连接关系
	m_pServerObject->GetClientManager()->Connect(u4ClientID, "172.21.0.41", 10040, TYPE_IPV4, (IClientMessage* )pPostServerData);

	m_mapC2S.insert(mapc2s::value_type(u4ClientID, pPostServerData));
}

void CBaseCommand::DelClient2Server( uint32 u4ClientID )
{
	mapc2s::iterator f = m_mapC2S.find(u4ClientID);
	if(f == m_mapC2S.end())
	{
		return;
	}

	CPostServerData* pPostServerData = (CPostServerData* )f->second;
	if(NULL != pPostServerData)
	{
		uint32 u4ServerID = pPostServerData->GetServerID();
		m_pServerObject->GetClientManager()->Close(u4ServerID);
	}
	m_mapC2S.erase(f);

}

CPostServerData* CBaseCommand::GetClient2Server_ServerID( uint32 u4ClientID )
{
	mapc2s::iterator f = m_mapC2S.find(u4ClientID);
	if(f == m_mapC2S.end())
	{
		return NULL;
	}
	else
	{
		return (CPostServerData* )f->second;
	}

}

void CBaseCommand::CloseClient2Server()
{
	for(mapc2s::iterator b = m_mapC2S.begin(); b != m_mapC2S.end(); b++)
	{
		CPostServerData* pPostServerData = (CPostServerData* )b->second;
		SAFE_DELETE(pPostServerData);
	}
	m_mapC2S.clear();
}

int CBaseCommand::Do_Connect(IMessage* pMessage)
{
	AddClient2Server(pMessage->GetMessageBase()->m_u4ConnectID);
	OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CONNECT OK.\n"));
	return 0;
}

int CBaseCommand::Do_DisConnect(IMessage* pMessage)
{
	DelClient2Server(pMessage->GetMessageBase()->m_u4ConnectID);
	OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));

	return 0;
}

int CBaseCommand::Do_ClientSendTimeout(IMessage* pMessage)
{
	return 0;
}

int CBaseCommand::Do_PostBase(IMessage* pMessage)
{
	uint32     u4PacketLen = 0;
	uint16     u2CommandID = 0;

	_PacketInfo objPacketInfo;
	pMessage->GetPacketBody(objPacketInfo);

	//设置当前接收数据的ConnectID，用于收到远程回应信息返回
	//m_pPostServerData1->SetConnectID(pMessage->GetMessageBase()->m_u4ConnectID);
	CPostServerData* pPostServerData = GetClient2Server_ServerID(pMessage->GetMessageBase()->m_u4ConnectID);

	if(NULL != pPostServerData)
	{
		pPostServerData->SendData(objPacketInfo.m_pData, objPacketInfo.m_nDataLen);
	}

	return 0;
}