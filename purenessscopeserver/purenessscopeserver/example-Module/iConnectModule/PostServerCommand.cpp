/*
 * 来自服务器的消息处理
 * by w1w
 */
#include "PostServerCommand.h"
#include "ServerConfig.h"

CPostServerCommand::CPostServerCommand()
{
	m_pServerObject = NULL;
}

CPostServerCommand::~CPostServerCommand()
{
}

void CPostServerCommand::SetServerObject(CServerObject* pServerObject)
{
	m_pServerObject = pServerObject;
}

bool CPostServerCommand::RecvData(ACE_Message_Block* mbRecv)
{
	if(mbRecv == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CPostServerCommand::RecvData] pClientParse is NULL.\n"));
		return false;
	}

	char* pData = mbRecv->rd_ptr();

	uint16 u2CommandID = 0;
	ACE_OS::memcpy(&u2CommandID, pData + sizeof(uint32), sizeof(uint16));

	if(u2CommandID == COMMAND_KEEP_ALIVE_RESPONSE)
	{
		//OUR_DEBUG((LM_ERROR, "[CPostServerCommand::RecvData] 心跳回应包.\n"));
		return true;
	}
	if(u2CommandID == COMMAND_QUERY_APP_RESPONSE)
	{
		OUR_DEBUG((LM_ERROR, "[CPostServerCommand::RecvData] 更新APP.\n"));
		IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();
		if(NULL == pBodyPacket)
		{
			return false;
		}
		//得到数据包的包体
		_PacketInfo BodyPacket;
		BodyPacket.m_pData = pData;
		BodyPacket.m_nDataLen = (int)mbRecv->length();

		//将包体绑定给pBodyPacket流
		pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

		uint16     u2CommandID  = 0;
		uint16     ServerID  = 0;
		uint16 u2AppCount = 0;
		uint16 Appid = 0;
		uint8 isadd = 0;

		(*pBodyPacket) >> u2CommandID;   //从流里面读出u2CommandID数值
		(*pBodyPacket) >> ServerID;   
		(*pBodyPacket) >> u2AppCount; 
		icServerConfig::instance()->m_mapServerInfo[ServerID].m_mapAppInfo.clear();
		for (int i = 0; i<u2AppCount; ++i)
		{
			(*pBodyPacket) >> Appid;   
			(*pBodyPacket) >> isadd;
			_icAppInfo appinfo;
			appinfo.m_Appid = Appid;
			icServerConfig::instance()->m_mapServerInfo[ServerID].m_mapAppInfo[Appid]=appinfo;
		}
		if (u2CommandID == COMMAND_QUERY_APP_RESPONSE) icServerConfig::instance()->m_mapServerInfo[ServerID].m_NeedRefeshApp = false;

		//串行化的类用完了，归还到池中
		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);
		return true;
	}
	if(u2CommandID == COMMAND_SYN_APP)
	{
		OUR_DEBUG((LM_ERROR, "[CPostServerCommand::RecvData] 同步APP.\n"));
		return true;
	}

	OUR_DEBUG((LM_INFO, "[CPostServerCommand::RecvData][未处理的命令]%d", u2CommandID));
	return true;
}

bool CPostServerCommand::ConnectError(int nError)
	{
		OUR_DEBUG((LM_ERROR, "[CPostServerCommand::ConnectError]接收错误(%d).\n", nError));
		return true;
	}