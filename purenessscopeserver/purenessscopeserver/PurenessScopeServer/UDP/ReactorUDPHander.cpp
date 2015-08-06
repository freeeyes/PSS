#include "ReactorUDPHander.h"

CReactorUDPHander::CReactorUDPHander(void)
{
	m_pPacketParse      = NULL;
	m_u4RecvPacketCount = 0;
	m_u4SendPacketCount = 0;
	m_u4RecvSize        = 0;
	m_u4SendSize        = 0;
}

CReactorUDPHander::~CReactorUDPHander(void)
{
}

int CReactorUDPHander::OpenAddress(const ACE_INET_Addr& AddrRemote, ACE_Reactor* pReactor)
{
	if(m_skRemote.open(AddrRemote) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CReactorUDPHander::OpenAddress]Open error(%d).\n", errno));
		return -1;
	}

	reactor(pReactor);

	//按照线程初始化统计模块的名字
	char szName[MAX_BUFF_50] = {'\0'};
	sprintf_safe(szName, MAX_BUFF_50, "发送线程");
	m_CommandAccount.InitName(szName);

	//初始化统计模块功能
	m_CommandAccount.Init(App_MainConfig::instance()->GetCommandAccount(), 
		App_MainConfig::instance()->GetCommandFlow(), 
		App_MainConfig::instance()->GetPacketTimeOut());

	//设置发送超时时间（因为UDP如果客户端不存在的话，sendto会引起一个recv错误）
	//在这里设置一个超时，让个recv不会无限等下去
	struct timeval timeout = {MAX_RECV_UDP_TIMEOUT, 0}; 
	ACE_OS::setsockopt(m_skRemote.get_handle(), SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

	m_pPacketParse = App_PacketParsePool::instance()->Create();

	if(-1 == this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK))
	{
		OUR_DEBUG((LM_ERROR, "[CReactorUDPHander::OpenAddress] Addr is register_handler error(%d).\n", errno));
		return -1;
	}
	return 0;
}

void CReactorUDPHander::Close()
{
	ACE_Reactor_Mask close_mask = ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;
	this->reactor()->remove_handler(this, close_mask);
	m_skRemote.close();
}

ACE_HANDLE CReactorUDPHander::get_handle(void) const
{
	return m_skRemote.get_handle();
}

int CReactorUDPHander::handle_input(ACE_HANDLE fd)
{
	if(fd == ACE_INVALID_HANDLE)
	{
		OUR_DEBUG((LM_ERROR, "[CReactorUDPHander::handle_input]fd is ACE_INVALID_HANDLE.\n"));
		return -1;
	}	
	
	char szBuff[MAX_UDP_PACKET_LEN] = {'\0'};

	int nDataLen = m_skRemote.recv(szBuff, MAX_UDP_PACKET_LEN, m_addrRemote);
	if(nDataLen > 0)
	{
		CheckMessage(szBuff, (uint32)nDataLen);

		m_pPacketParse = App_PacketParsePool::instance()->Create();
	}

	return 0;
}

int CReactorUDPHander::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	if(handle == ACE_INVALID_HANDLE)
	{
		OUR_DEBUG((LM_ERROR, "[CReactorUDPHander::handle_close]close_mask = %d.\n", (uint32)close_mask));
	}	
	
	Close();
	return 0;
}

bool CReactorUDPHander::SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead, uint16 u2CommandID)
{
	ACE_hrtime_t m_tvBegin = ACE_OS::gethrtime();

	ACE_INET_Addr AddrRemote;
	int nErr = AddrRemote.set(nPort, szIP);
	if(nErr != 0)
	{
		OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::SendMessage]set_address error[%d].\n", errno));
		SAFE_DELETE_ARRAY(pMessage);
		return false;
	}

	//如果需要拼接包头，则拼接包头
	if(blHead == true)
	{
		CPacketParse PacketParse;

		ACE_Message_Block* pMbData = NULL;
		uint32 u4SendLength = PacketParse.MakePacketLength(0, u4Len);
		pMbData = App_MessageBlockManager::instance()->Create(u4SendLength);
		if(NULL == pMbData)
		{
			SAFE_DELETE(pMessage);
			return false;
		}

		PacketParse.MakePacket(0, pMessage, u4Len, pMbData);

		int nSize = (int)m_skRemote.send(pMbData->rd_ptr(), pMbData->length(), AddrRemote);
		if((uint32)nSize == u4Len)
		{
			m_atvOutput = ACE_OS::gettimeofday();
			m_u4SendSize += u4Len;
			m_u4SendPacketCount++;
			SAFE_DELETE_ARRAY(pMessage);

			//统计发送信息
			uint32 u4Cost = (uint32)(ACE_OS::gethrtime() - m_tvBegin);
			m_CommandAccount.SaveCommandData(u2CommandID, u4Cost, PACKET_UDP, (uint32)pMbData->length(), u4Len, COMMAND_TYPE_OUT);

			//释放发送体
			pMbData->release();

			return true;
		}
		else
		{
			OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]send error(%d).\n", errno));
			SAFE_DELETE_ARRAY(pMessage);

			//释放发送体
			pMbData->release();

			return false;
		}
	}
	else
	{
		int nSize = (int)m_skRemote.send(pMessage, u4Len, AddrRemote);
		if((uint32)nSize == u4Len)
		{
			m_atvOutput = ACE_OS::gettimeofday();
			m_u4SendSize += u4Len;
			m_u4SendPacketCount++;
			SAFE_DELETE_ARRAY(pMessage);

			//统计发送信息
			uint32 u4Cost = (uint32)(ACE_OS::gethrtime() - m_tvBegin);
			m_CommandAccount.SaveCommandData(u2CommandID, u4Cost, PACKET_UDP, u4Len, u4Len, COMMAND_TYPE_OUT);

			return true;
		}
		else
		{
			OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]send error(%d).\n", errno));
			SAFE_DELETE_ARRAY(pMessage);
			return false;
		}
	}

}

_ClientConnectInfo CReactorUDPHander::GetClientConnectInfo()
{
	_ClientConnectInfo ClientConnectInfo;
	ClientConnectInfo.m_blValid       = true;
	ClientConnectInfo.m_u4ConnectID   = 0;
	ClientConnectInfo.m_u4AliveTime   = 0;
	ClientConnectInfo.m_u4BeginTime   = (uint32)m_atvInput.sec();
	ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
	ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
	ClientConnectInfo.m_u4RecvCount   = m_u4RecvPacketCount;
	ClientConnectInfo.m_u4SendCount   = m_u4SendPacketCount;
	return ClientConnectInfo;
}

bool CReactorUDPHander::CheckMessage(const char* pData, uint32 u4Len)
{
	if(NULL == m_pPacketParse || NULL == pData)
	{
		return false;
	}

	if(m_pPacketParse->GetPacketMode() == PACKET_WITHHEAD)
	{
		if(u4Len <= m_pPacketParse->GetPacketHeadLen())
		{
			return false;
		}

		//将完整的数据包转换为PacketParse对象
		ACE_Message_Block* pMBHead = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketHeadLen());
		memcpy_safe((char* )pData, m_pPacketParse->GetPacketHeadLen(), (char* )pMBHead->wr_ptr(), m_pPacketParse->GetPacketHeadLen());
		pMBHead->wr_ptr(m_pPacketParse->GetPacketHeadLen());
		m_pPacketParse->SetPacketHead(0, pMBHead, App_MessageBlockManager::instance());

		if(u4Len != m_pPacketParse->GetPacketHeadLen() + m_pPacketParse->GetPacketBodyLen())
		{
			pMBHead->release();
			return false;
		}

		//如果包含包体
		if(m_pPacketParse->GetPacketBodyLen() > 0)
		{
			char* pBody = (char* )(&pData[0] + m_pPacketParse->GetPacketHeadLen());
			ACE_Message_Block* pMBBody = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketBodyLen());
			memcpy_safe(pBody, m_pPacketParse->GetPacketBodyLen(), (char* )pMBBody->wr_ptr(), m_pPacketParse->GetPacketBodyLen());
			pMBBody->wr_ptr(m_pPacketParse->GetPacketBodyLen());
			m_pPacketParse->SetPacketBody(0, pMBBody, App_MessageBlockManager::instance());
		}

		//组织数据包
		_MakePacket objMakePacket;
		objMakePacket.m_u4ConnectID       = UDP_HANDER_ID;
		objMakePacket.m_pPacketParse      = m_pPacketParse;
		objMakePacket.m_PacketType        = PACKET_UDP;

		//UDP因为不是面向链接的，所以这里ConnectID设置成-1
		if(false == App_MakePacket::instance()->PutUDPMessageBlock(m_addrRemote, PACKET_PARSE, &objMakePacket))
		{
			App_PacketParsePool::instance()->Delete(m_pPacketParse);
			OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]PutMessageBlock is error.\n"));
			return false;
		}
	}
	else
	{
		ACE_Message_Block* pMbData = App_MessageBlockManager::instance()->Create(u4Len); 
		memcpy_safe((char* )pData, u4Len, (char* )pMbData->wr_ptr(), u4Len);
		pMbData->wr_ptr(u4Len);

		//以数据流处理
		if(PACKET_GET_ENOUGTH == m_pPacketParse->GetPacketStream(0, pMbData, App_MessageBlockManager::instance()))
		{
			//组织数据包
			_MakePacket objMakePacket;
			objMakePacket.m_u4ConnectID       = UDP_HANDER_ID;
			objMakePacket.m_pPacketParse      = m_pPacketParse;
			objMakePacket.m_PacketType        = PACKET_UDP;

			//UDP因为不是面向链接的
			if(false == App_MakePacket::instance()->PutUDPMessageBlock(m_addrRemote, PACKET_PARSE, &objMakePacket))
			{
				App_PacketParsePool::instance()->Delete(m_pPacketParse);
				OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]PutMessageBlock is error.\n"));
				return false;
			}
		}
		else
		{
			OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]m_pPacketParse GetPacketStream is error.\n"));
			App_PacketParsePool::instance()->Delete(m_pPacketParse);
			return false;
		}
		App_MessageBlockManager::instance()->Close(pMbData);
	}

	App_PacketParsePool::instance()->Delete(m_pPacketParse);

	m_atvInput = ACE_OS::gettimeofday();
	m_u4RecvSize += u4Len;
	m_u4RecvPacketCount++;

	return true;
}

void CReactorUDPHander::GetCommandData( uint16 u2CommandID, _CommandData& objCommandData )
{
	_CommandData* pCommandData = m_CommandAccount.GetCommandData(u2CommandID);
	if(pCommandData != NULL)
	{
		objCommandData += (*pCommandData);
	}
}
