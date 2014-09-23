#include "ProConnectHandle.h"

Mutex_Allocator _msg_prosend_mb_allocator;

CProConnectHandle::CProConnectHandle(void)
{
	m_szError[0]          = '\0';
	m_u4ConnectID         = 0;
	m_u4AllRecvCount      = 0;
	m_u4AllSendCount      = 0;
	m_u4AllRecvSize       = 0;
	m_u4AllSendSize       = 0;
	m_nIOCount            = 0;
	m_u4HandlerID         = 0;
	m_u2MaxConnectTime    = 0;
	m_u4SendThresHold     = MAX_MSG_SNEDTHRESHOLD;
	m_u2SendQueueMax      = MAX_MSG_SENDPACKET;
	m_u1ConnectState      = CONNECT_INIT;
	m_u1SendBuffState     = CONNECT_SENDNON;
	m_pPacketParse        = NULL;
	m_u4MaxPacketSize     = MAX_MSG_PACKETLENGTH;
	m_u8RecvQueueTimeCost = 0;
	m_u4RecvQueueCount    = 0;
	m_u8SendQueueTimeCost = 0;
	m_u4ReadSendSize      = 0;
	m_u4SuccessSendSize   = 0;
	m_pBlockMessage       = NULL;
	m_u2SendQueueTimeout  = MAX_QUEUE_TIMEOUT * 1000;  //目前因为记录的是微秒，所以这里相应的扩大1000倍
	m_u2RecvQueueTimeout  = MAX_QUEUE_TIMEOUT * 1000;  //目前因为记录的是微秒，所以这里相应的扩大1000倍
	m_u2TcpNodelay        = TCP_NODELAY_ON;
}

CProConnectHandle::~CProConnectHandle(void)
{
	if(NULL != m_pBlockMessage)
	{
		m_pBlockMessage->release();
		m_pBlockMessage = NULL;
	}
}

void CProConnectHandle::Init(uint16 u2HandlerID)
{
	m_u4HandlerID      = u2HandlerID;
	m_u2MaxConnectTime = App_MainConfig::instance()->GetMaxConnectTime();
	m_u4SendThresHold  = App_MainConfig::instance()->GetSendTimeout();
	m_u2SendQueueMax   = App_MainConfig::instance()->GetSendQueueMax();
	m_u4MaxPacketSize  = App_MainConfig::instance()->GetRecvBuffSize();
	m_u2TcpNodelay     = App_MainConfig::instance()->GetTcpNodelay();

	m_u2SendQueueTimeout = App_MainConfig::instance()->GetSendQueueTimeout() * 1000;
	if(m_u2SendQueueTimeout == 0)
	{
		m_u2SendQueueTimeout = MAX_QUEUE_TIMEOUT * 1000;
	}

	m_u2RecvQueueTimeout = App_MainConfig::instance()->GetRecvQueueTimeout() * 1000;
	if(m_u2RecvQueueTimeout <= 0)
	{
		m_u2RecvQueueTimeout = MAX_QUEUE_TIMEOUT * 1000;
	}

	m_pBlockMessage      = new ACE_Message_Block(m_u4MaxPacketSize);
}

const char* CProConnectHandle::GetError()
{
	return m_szError;
}

bool CProConnectHandle::Close(int nIOCount, int nErrno)
{
	m_ThreadWriteLock.acquire();
	if(nIOCount > m_nIOCount)
	{
		m_nIOCount = 0;
	}

	//OUR_DEBUG((LM_DEBUG, "[CProConnectHandle::Close]ConnectID=%d, m_nIOCount = %d, nIOCount = %d.\n", GetConnectID(), m_nIOCount, nIOCount));

	if(m_nIOCount > 0)
	{
		m_nIOCount -= nIOCount;
	}
	m_ThreadWriteLock.release();

	if(m_nIOCount == 0)
	{
		m_ThreadWriteLock.acquire();
		//查看是否是IP追踪信息，是则记录
		App_IPAccount::instance()->CloseIP((string)m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllSendSize);

		//调用连接断开消息，通知PacketParse接口
		CPacketParse objPacketParse;
		objPacketParse.DisConnect(GetConnectID());

		//通知逻辑接口，连接已经断开
		OUR_DEBUG((LM_DEBUG,"[CConnectHandler::Close]Connectid=[%d] error(%d)...\n", GetConnectID(), nErrno));
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64dns, SendQueueTimeCost=%I64dns.", 
			m_addrRemote.get_host_addr(), 
			m_addrRemote.get_port_number(), 
			m_u4AllRecvSize, 
			m_u4AllRecvCount, 
			m_u4AllSendSize, 
			m_u4AllSendCount, 
			m_u4RecvQueueCount, 
			m_u8RecvQueueTimeCost, 
			m_u8SendQueueTimeCost);
		//因为是要关闭连接，所以要多关闭一次IO，对应Open设置的1的初始值

		if(m_u1ConnectState != CONNECT_SERVER_CLOSE)
		{
			//发送客户端链接断开消息。
			if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_CDISCONNECT, NULL))
			{
				OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
			}
		}

		m_Reader.cancel();
		m_Writer.cancel();

		if(this->handle() != ACE_INVALID_HANDLE)
		{
			ACE_OS::closesocket(this->handle());
			this->handle(ACE_INVALID_HANDLE);
		}

		m_ThreadWriteLock.release();

		OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::Close] Close(%d) delete OK.\n", GetConnectID()));

		//删除存在列表中的对象引用
		App_ProConnectManager::instance()->Close(GetConnectID());

		//将对象指针放入空池中
		App_ProConnectHandlerPool::instance()->Delete(this);

		return true;
	}

	return false;
}

bool CProConnectHandle::ServerClose()
{
	OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::ServerClose] Close(%d) delete OK.\n", GetConnectID()));

	//发送服务器端链接断开消息。
	if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_SDISCONNECT, NULL))
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_SDISCONNECT is error.\n", GetConnectID()));
		return false;
	}

	m_Reader.cancel();
	m_Writer.cancel();

	if(this->handle() != ACE_INVALID_HANDLE)
	{
		ACE_OS::closesocket(this->handle());
		this->handle(ACE_INVALID_HANDLE);
	}

	m_u1ConnectState = CONNECT_SERVER_CLOSE;

	return true;
}

void CProConnectHandle::SetConnectID(uint32 u4ConnectID)
{
	m_u4ConnectID = u4ConnectID;
}

uint32 CProConnectHandle::GetConnectID()
{
	return m_u4ConnectID;
}

void CProConnectHandle::addresses (const ACE_INET_Addr &remote_address, const ACE_INET_Addr &local_address)
{
	m_addrRemote = remote_address;
}

void CProConnectHandle::open(ACE_HANDLE h, ACE_Message_Block&)
{
	ACE_Time_Value tvOpenBegin(ACE_OS::gettimeofday());

	OUR_DEBUG((LM_INFO, "[CProConnectHandle::open] [0x%08x]Connection from [%s:%d]\n", this, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number()));

	m_atvConnect      = ACE_OS::gettimeofday();
	m_atvInput        = ACE_OS::gettimeofday();
	m_atvOutput       = ACE_OS::gettimeofday();
	m_atvSendAlive    = ACE_OS::gettimeofday();

	m_u4AllRecvCount      = 0;
	m_u4AllSendCount      = 0;
	m_u4AllRecvSize       = 0;
	m_u4AllSendSize       = 0;
	m_u4RecvPacketCount   = 0;
	m_nIOCount            = 1;
	m_u8RecvQueueTimeCost = 0;
	m_u4RecvQueueCount    = 0;
	m_u8SendQueueTimeCost = 0;
	m_u4SuccessSendSize   = 0;
	m_u4ReadSendSize      = 0;
	m_blIsLog             = false;
	m_szConnectName[0]    = '\0';

	ACE_Time_Value tvOpenEnd(ACE_OS::gettimeofday());
	ACE_Time_Value tvOpen(tvOpenEnd - tvOpenBegin);

	if(App_ForbiddenIP::instance()->CheckIP(m_addrRemote.get_host_addr()) == false)
	{
		//在禁止列表中，不允许访问
		OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]IP Forbidden(%s).\n", m_addrRemote.get_host_addr()));
		return;
	}

	//检查单位时间链接次数是否达到上限
	if(false == App_IPAccount::instance()->AddIP((string)m_addrRemote.get_host_addr(), m_addrRemote.get_port_number()))
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open]IP connect frequently.\n", m_addrRemote.get_host_addr()));
		App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), App_MainConfig::instance()->GetIPAlert()->m_u4IPTimeout);
		
		//发送告警邮件
		AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT, 
			App_MainConfig::instance()->GetIPAlert()->m_u4MailID,
			"Alert IP",
			"[CConnectHandler::open] IP is more than IP Max,");		
		
		Close();
		return;
	}

	if(m_u2TcpNodelay == TCP_NODELAY_OFF)
	{
		//如果设置了禁用Nagle算法，则这里要禁用。
		int nOpt = 1; 
		ACE_OS::setsockopt(h, IPPROTO_TCP, TCP_NODELAY, (char* )&nOpt, sizeof(int)); 
	}

	//初始化检查器
	m_TimeConnectInfo.Init(App_MainConfig::instance()->GetClientDataAlert()->m_u4RecvPacketCount, 
		App_MainConfig::instance()->GetClientDataAlert()->m_u4RecvDataMax, 
		App_MainConfig::instance()->GetClientDataAlert()->m_u4SendPacketCount,
		App_MainConfig::instance()->GetClientDataAlert()->m_u4SendDataMax);

	//写入连接日志
	AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Connection from [%s:%d] DisposeTime = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), tvOpen.msec());

	this->handle(h);

	//默认别名是IP地址
	SetConnectName(m_addrRemote.get_host_addr());

	if(this->m_Reader.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE)) == -1 || 
		this->m_Writer.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE)) == -1)
	{
		OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::open] m_reader or m_reader == 0.\n"));	
		Close();
		return;
	}

	//ACE_Sig_Action writeAction((ACE_SignalHandler)SIG_IGN);
	//writeAction.register_action(SIGPIPE, 0);

	//int nTecvBuffSize = MAX_MSG_SOCKETBUFF;
	//ACE_OS::setsockopt(this->get_handle(), SOL_SOCKET, SO_RCVBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));
	//ACE_OS::setsockopt(h, SOL_SOCKET, SO_SNDBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));

	//将这个链接放入链接库
	if(false == App_ProConnectManager::instance()->AddConnect(this))
	{
		OUR_DEBUG((LM_ERROR, "%s.\n", App_ProConnectManager::instance()->GetError()));
		sprintf_safe(m_szError, MAX_BUFF_500, "%s", App_ProConnectManager::instance()->GetError());
		Close();
		return;
	}

	m_u1ConnectState = CONNECT_OPEN;

	OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::open] Open(%d).\n", GetConnectID()));	

	m_pPacketParse = App_PacketParsePool::instance()->Create();
	if(NULL == m_pPacketParse)
	{
		OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::open] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
		Close();
		return;
	}

	//告诉PacketParse连接应建立
	_ClientIPInfo objClientIPInfo;
	sprintf_safe(objClientIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_addrRemote.get_host_addr());
	objClientIPInfo.m_nPort = m_addrRemote.get_port_number();
	m_pPacketParse->Connect(GetConnectID(), objClientIPInfo);

	//发送链接建立消息。
	if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_CONNECT, NULL))
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
	}

	if(m_pPacketParse->GetPacketMode() == PACKET_WITHHEAD)
	{
		RecvClinetPacket(m_pPacketParse->GetPacketHeadLen());
	}
	else
	{
		RecvClinetPacket(App_MainConfig::instance()->GetServerRecvBuff());
	}

	return;
}

void CProConnectHandle::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
{
	ACE_Message_Block& mb = result.message_block();
	uint32 u4PacketLen = (uint32)result.bytes_transferred();
	int nTran = (int)result.bytes_transferred();

	if(!result.success() || result.bytes_transferred() == 0)
	{
		//链接断开
		//清理PacketParse
		ClearPacketParse(mb);

		//关闭当前连接
		Close(2, errno);

		return;
	}

	m_atvInput = ACE_OS::gettimeofday();

	//如果是DEBUG状态，记录当前接受包的二进制数据
	if(App_MainConfig::instance()->GetDebug() == DEBUG_ON || m_blIsLog == true)
	{
		char szDebugData[MAX_BUFF_1024] = {'\0'};
		char szLog[10]  = {'\0'};
		int  nDebugSize = 0; 
		bool blblMore   = false;

		if(mb.length() >= MAX_BUFF_200)
		{
			nDebugSize = MAX_BUFF_200;
			blblMore   = true;
		}
		else
		{
			nDebugSize = (int)mb.length();
		}

		char* pData = mb.rd_ptr();
		for(int i = 0; i < nDebugSize; i++)
		{
			sprintf_safe(szLog, 10, "0x%02X ", (unsigned char)pData[i]);
			sprintf_safe(szDebugData + 5*i, MAX_BUFF_1024 - 5*i, "%s", szLog);
		}

		if(blblMore == true)
		{
			AppLogManager::instance()->WriteLog(LOG_SYSTEM_DEBUG_CLIENTRECV, "[(%s)%s:%d]%s.(数据包过长只记录前200字节)", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), szDebugData);
		}
		else
		{
			AppLogManager::instance()->WriteLog(LOG_SYSTEM_DEBUG_CLIENTRECV, "[(%s)%s:%d]%s.", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), szDebugData);
		}
	}

	if(m_pPacketParse->GetPacketMode() == PACKET_WITHHEAD)
	{
		if(result.bytes_transferred() < result.bytes_to_read())
		{
			//短读，继续读
			int nRead = (int)result.bytes_to_read() - (int)result.bytes_transferred();
			if(-1 == m_Reader.read(mb, nRead))
			{
				//清理PacketParse
				ClearPacketParse(mb);

				//关闭当前连接
				Close(2, errno);
				return;
			}

		}
		else if(mb.length() == m_pPacketParse->GetPacketHeadLen() && m_pPacketParse->GetIsHead() == false)
		{
			//判断头的合法性
			bool blStateHead = m_pPacketParse->SetPacketHead(GetConnectID(), &mb, App_MessageBlockManager::instance());
			if(false == blStateHead)
			{
				//如果包头是非法的，则返回错误，断开连接。
				OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_read_stream]PacketHead is illegal.\n"));

				//清理PacketParse
				ClearPacketParse(mb);

				//关闭当前连接
				Close(2, errno);
				return;
			}

			//这里添加只处理包头的数据
			//如果数据只有包头，不需要包体，在这里必须做一些处理，让数据只处理包头就扔到DoMessage()
			uint32 u4PacketBodyLen = m_pPacketParse->GetPacketBodyLen();

			if(u4PacketBodyLen == 0)
			{
				//如果只有包头没有包体，则直接丢到逻辑里处理
				if(false == CheckMessage())
				{
					Close(2);
					return;
				}

				m_pPacketParse = App_PacketParsePool::instance()->Create();
				if(NULL == m_pPacketParse)
				{
					OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::handle_read_stream] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
					//因为是要关闭连接，所以要多关闭一次IO，对应Open设置的1的初始值
					//发送服务器端链接断开消息。
					if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_SDISCONNECT, NULL))
					{
						OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
					}

					Close(2);
					return;
				}
				Close();

				//接受下一个数据包
				RecvClinetPacket(m_pPacketParse->GetPacketHeadLen());

			}
			else
			{
				//如果超过了最大包长度，为非法数据
				if(u4PacketBodyLen >= m_u4MaxPacketSize)
				{
					OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_read_stream]u4PacketHeadLen(%d) more than %d.\n", u4PacketBodyLen, m_u4MaxPacketSize));

					//清理PacketParse
					ClearPacketParse(mb);

					//关闭当前连接
					Close(2, errno);
				}
				else
				{
					Close();
					RecvClinetPacket(u4PacketBodyLen);
				}
			}
		}
		else
		{
			//接受完整数据完成，开始分析完整数据包
			bool blStateBody = m_pPacketParse->SetPacketBody(GetConnectID(), &mb, App_MessageBlockManager::instance());
			if(false == blStateBody)
			{
				//如果数据包体非法，断开连接
				OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_read_stream]SetPacketBody is illegal.\n"));

				//清理PacketParse
				ClearPacketParse(mb);

				//关闭当前连接
				Close(2, errno);
				return;
			}

			if(false == CheckMessage())
			{
				Close(2);
				return;
			}

			m_pPacketParse = App_PacketParsePool::instance()->Create();
			if(NULL == m_pPacketParse)
			{
				OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::handle_read_stream] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
				//因为是要关闭连接，所以要多关闭一次IO，对应Open设置的1的初始值
				//发送服务器端链接断开消息。
				if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_SDISCONNECT, NULL))
				{
					OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
				}

				Close(2);
				return;
			}
			Close();

			//接受下一个数据包
			RecvClinetPacket(m_pPacketParse->GetPacketHeadLen());
		}
	}
	else
	{
		//以流模式解析
		while(true)
		{
			uint8 n1Ret = m_pPacketParse->GetPacketStream(GetConnectID(), &mb, (IMessageBlockManager* )App_MessageBlockManager::instance());
			if(PACKET_GET_ENOUGTH == n1Ret)
			{
				//已经接收了完整数据包，扔给工作线程去处理
				if(false == CheckMessage())
				{
					Close(2);
					return;
				}

				m_pPacketParse = App_PacketParsePool::instance()->Create();
				if(NULL == m_pPacketParse)
				{
					OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::handle_read_stream] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
					//因为是要关闭连接，所以要多关闭一次IO，对应Open设置的1的初始值
					//发送服务器端链接断开消息。
					if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_SDISCONNECT, NULL))
					{
						OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
					}

					Close(2);
					return;
				}

				//看看是否接收完成了
				if(mb.length() == 0)
				{
					mb.release();
					break;
				}
				else
				{
					//还有数据，继续分析
					continue;
				}
			}
			else if(PACKET_GET_NO_ENOUGTH == n1Ret)
			{
				//接收的数据不完整，需要继续接收
				break;
			}
			else
			{
				//数据包为错误包，丢弃处理
				m_pPacketParse->Clear();

				Close(2);
				return;
			}
		}

		Close();
		//接受下一个数据包
		RecvClinetPacket(App_MainConfig::instance()->GetServerRecvBuff());
	}

	return;
}

void CProConnectHandle::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
{
	if(!result.success() || result.bytes_transferred()==0)
	{
		//链接断开
		OUR_DEBUG ((LM_DEBUG,"[CConnectHandler::handle_write_stream] Connectid=[%d] begin(%d)...\n",GetConnectID(), errno));

		AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64dns, SendQueueTimeCost=%I64dns.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, m_u4RecvQueueCount, m_u8RecvQueueTimeCost, m_u8SendQueueTimeCost);

		OUR_DEBUG((LM_DEBUG,"[CConnectHandler::handle_write_stream] Connectid=[%d] finish ok...\n", GetConnectID()));
		m_atvOutput = ACE_OS::gettimeofday();
		App_MessageBlockManager::instance()->Close(&result.message_block());
		Close();
		return;
	}
	else
	{
		//发送成功
		m_ThreadWriteLock.acquire();
		m_atvOutput = ACE_OS::gettimeofday();
		App_MessageBlockManager::instance()->Close(&result.message_block());
		m_u4AllSendSize += (uint32)result.bytes_to_write();

		//如果需要统计信息
		App_IPAccount::instance()->UpdateIP((string)m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllSendSize);
		m_ThreadWriteLock.release();
		
		//记录水位标
		m_u4SuccessSendSize += (uint32)result.bytes_to_write();
		Close();
		return;
	}
}

bool CProConnectHandle::SetRecvQueueTimeCost(uint32 u4TimeCost)
{
	m_ThreadWriteLock.acquire();
	m_nIOCount++;
	m_ThreadWriteLock.release();

	//如果超过阀值，则记录到日志中去
	if((uint32)(m_u2RecvQueueTimeout * 1000) <= u4TimeCost)
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_RECVQUEUEERROR, "[TCP]IP=%s,Prot=%d,Timeout=[%d].", GetClientIPInfo().m_szClientIP, GetClientIPInfo().m_nPort, u4TimeCost);
	}

	//OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetRecvQueueTimeCost]m_u4RecvQueueCount=%d.\n", m_u4RecvQueueCount));
	m_u4RecvQueueCount++;

	//测试了几天，感觉这个时间意义，因为获取队列的处理时间片可能很耗时，导致一批数据的阶段性时间增长
	//只要记录超时的数据即可
	//m_u8RecvQueueTimeCost += u4TimeCost;

	Close();
	return true;
}

bool CProConnectHandle::SetSendQueueTimeCost(uint32 u4TimeCost)
{
	m_ThreadWriteLock.acquire();
	m_nIOCount++;
	m_ThreadWriteLock.release();

	//如果超过阀值，则记录到日志中去
	if((uint32)(m_u2SendQueueTimeout * 1000) <= u4TimeCost)
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_SENDQUEUEERROR, "[TCP]IP=%s,Prot=%d,Timeout=[%d].", GetClientIPInfo().m_szClientIP, GetClientIPInfo().m_nPort, u4TimeCost);
	}

	//m_u8SendQueueTimeCost += u4TimeCost;

	Close();
	return true;
}

uint8 CProConnectHandle::GetConnectState()
{
	return m_u1ConnectState;
}

uint8 CProConnectHandle::GetSendBuffState()
{
	return m_u1SendBuffState;
}

bool CProConnectHandle::SendMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, bool blState, uint8 u1SendType, uint32& u4PacketSize, bool blDelete)
{
	m_ThreadWriteLock.acquire();
	m_nIOCount++;
	m_ThreadWriteLock.release();	
	//OUR_DEBUG((LM_DEBUG,"[CConnectHandler::SendMessage]Connectid=%d,m_nIOCount=%d.\n", GetConnectID(), m_nIOCount));

	ACE_Message_Block* pMbData = NULL;

	if(NULL == pBuffPacket)
	{
		OUR_DEBUG((LM_DEBUG,"[CConnectHandler::SendMessage] Connectid=[%d] pBuffPacket is NULL.\n", GetConnectID()));
		Close();
		return false;
	}

	//如果不是直接发送数据，则拼接数据包
	if(blState == PACKET_SEND_CACHE)
	{
		//先判断要发送的数据长度，看看是否可以放入缓冲，缓冲是否已经放满。
		uint32 u4SendPacketSize = 0;
		if(u1SendType == SENDMESSAGE_NOMAL)
		{
			u4SendPacketSize = m_objSendPacketParse.MakePacketLength(GetConnectID(), pBuffPacket->GetPacketLen(), u2CommandID);
		}
		else
		{
			u4SendPacketSize = (uint32)m_pBlockMessage->length();
		}
		u4PacketSize = u4SendPacketSize;

		if(u4SendPacketSize + (uint32)m_pBlockMessage->length() >= m_u4MaxPacketSize)
		{
			OUR_DEBUG((LM_DEBUG,"[CConnectHandler::SendMessage] Connectid=[%d] m_pBlockMessage is not enougth.\n", GetConnectID()));
			if(blState = true)
			{
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
			}
			Close();
			return false;
		}
		else
		{
			//添加进缓冲区
			ACE_Message_Block* pMbBufferData = NULL;

			//SENDMESSAGE_NOMAL是需要包头的时候，否则，不组包直接发送
			if(u1SendType == SENDMESSAGE_NOMAL)
			{
				//这里组成返回数据包
				m_objSendPacketParse.MakePacket(GetConnectID(), pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), m_pBlockMessage, u2CommandID);
			}
			else
			{
				//如果不是SENDMESSAGE_NOMAL，则直接组包
				ACE_OS::memcpy(m_pBlockMessage->wr_ptr(), pBuffPacket->GetData(), pBuffPacket->GetPacketLen());
				m_pBlockMessage->wr_ptr(pBuffPacket->GetPacketLen());
			}
		}

		if(blState = true)
		{
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
		}

		Close();
		return true;
	}
	else
	{
		//先判断是否要组装包头，如果需要，则组装在m_pBlockMessage中
		uint32 u4SendPacketSize = 0;
		if(u1SendType == SENDMESSAGE_NOMAL)
		{
			u4SendPacketSize = m_objSendPacketParse.MakePacketLength(GetConnectID(), pBuffPacket->GetPacketLen(), u2CommandID);
			m_objSendPacketParse.MakePacket(GetConnectID(), pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), m_pBlockMessage, u2CommandID);
			//这里MakePacket已经加了数据长度，所以在这里不再追加
		}
		else
		{
			u4SendPacketSize = (uint32)pBuffPacket->GetPacketLen();
			ACE_OS::memcpy(m_pBlockMessage->wr_ptr(), pBuffPacket->GetData(), pBuffPacket->GetPacketLen());
			m_pBlockMessage->wr_ptr(pBuffPacket->GetPacketLen());
		}

		//如果之前有缓冲数据，则和缓冲数据一起发送
		u4PacketSize = m_pBlockMessage->length();

		u4PacketSize = m_pBlockMessage->length() + pBuffPacket->GetPacketLen();
		//如果之前有缓冲数据，则和缓冲数据一起发送
		if(m_pBlockMessage->length() > 0)
		{
			//因为是异步发送，发送的数据指针不可以立刻释放，所以需要在这里创建一个新的发送数据块，将数据考入
			pMbData = App_MessageBlockManager::instance()->Create((uint32)m_pBlockMessage->length());
			if(NULL == pMbData)
			{
				OUR_DEBUG((LM_DEBUG,"[CConnectHandler::SendMessage] Connectid=[%d] pMbData is NULL.\n", GetConnectID()));
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
				Close();
				return false;
			}

			ACE_OS::memcpy(pMbData->wr_ptr(), m_pBlockMessage->rd_ptr(), m_pBlockMessage->length());
			pMbData->wr_ptr(m_pBlockMessage->length());
			//放入完成，则清空缓存数据，使命完成
			m_pBlockMessage->reset();
		}

		if(blState = true)
		{
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
		}

		return PutSendPacket(pMbData);
	}
}

bool CProConnectHandle::CheckAlive()
{
	ACE_Time_Value tvNow = ACE_OS::gettimeofday();
	ACE_Time_Value tvIntval(tvNow - m_atvInput);
	if(tvIntval.sec() > m_u2MaxConnectTime)
	{
		//如果超过了最大时间，则服务器关闭链接
		OUR_DEBUG ((LM_ERROR, "[CProConnectHandle::CheckAlive] Connectid=%d Server Close!\n", GetConnectID()));

		//调用连接断开消息
		CPacketParse objPacketParse;
		objPacketParse.DisConnect(GetConnectID());		

		//在这里发送一个连接断开消息给逻辑
		if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_CDISCONNECT, NULL))
		{
			OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
		}

		ServerClose();
		return false;
	}
	else
	{
		return true;
	}
}

bool CProConnectHandle::PutSendPacket(ACE_Message_Block* pMbData)
{
	//如果是DEBUG状态，记录当前发送包的二进制数据
	if(App_MainConfig::instance()->GetDebug() == DEBUG_ON || m_blIsLog == true)
	{
		char szDebugData[MAX_BUFF_1024] = {'\0'};
		char szLog[10]  = {'\0'};
		int  nDebugSize = 0; 
		bool blblMore   = false;

		if(pMbData->length() >= MAX_BUFF_200)
		{
			nDebugSize = MAX_BUFF_200;
			blblMore   = true;
		}
		else
		{
			nDebugSize = (int)pMbData->length();
		}

		char* pData = pMbData->rd_ptr();
		for(int i = 0; i < nDebugSize; i++)
		{
			sprintf_safe(szLog, 10, "0x%02X ", (unsigned char)pData[i]);
			sprintf_safe(szDebugData + 5*i, MAX_BUFF_1024 -  5*i, "0x%02X ", (unsigned char)pData[i]);
		}

		if(blblMore == true)
		{
			AppLogManager::instance()->WriteLog(LOG_SYSTEM_DEBUG_CLIENTSEND, "[(%s)%s:%d]%s.(数据包过长只记录前200字节)", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), szDebugData);
		}
		else
		{
			AppLogManager::instance()->WriteLog(LOG_SYSTEM_DEBUG_CLIENTSEND, "[(%s)%s:%d]%s.", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), szDebugData);
		}
	}

	//OUR_DEBUG ((LM_ERROR, "[CConnectHandler::PutSendPacket] Connectid=%d, m_nIOCount=%d!\n", GetConnectID(), m_nIOCount));
	//统计发送数量
	ACE_Date_Time dtNow;
	if(false == m_TimeConnectInfo.SendCheck((uint8)dtNow.minute(), 1, pMbData->length()))
	{
		//超过了限定的阀值，需要关闭链接，并记录日志
		AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECTABNORMAL, 
			App_MainConfig::instance()->GetClientDataAlert()->m_u4MailID, 
			"Alert",
			"[TCP]IP=%s,Prot=%d,SendPacketCount=%d, SendSize=%d.", 
			m_addrRemote.get_host_addr(), 
			m_addrRemote.get_port_number(), 
			m_TimeConnectInfo.m_u4SendPacketCount, 
			m_TimeConnectInfo.m_u4SendSize);
		
		//设置封禁时间
		App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), App_MainConfig::instance()->GetIPAlert()->m_u4IPTimeout);
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::PutSendPacket] ConnectID = %d, Send Data is more than limit.\n", GetConnectID()));
		//断开连接
		Close(2);
		return false;
	}

	//异步发送方法
	if(NULL != pMbData)
	{
		//记录水位标
		m_u4ReadSendSize += pMbData->length();

		//比较水位标，是否超过一定数值，也就是说发快收慢的时候，如果超过一定数值，断开连接
		if(m_u4ReadSendSize - m_u4SuccessSendSize >= App_MainConfig::instance()->GetSendDataMask())
		{
			OUR_DEBUG ((LM_ERROR, "[CProConnectHandle::PutSendPacket]ConnectID = %d, SingleConnectMaxSendBuffer is more than(%d)!\n", GetConnectID(), m_u4ReadSendSize - m_u4SuccessSendSize));
			AppLogManager::instance()->WriteLog(LOG_SYSTEM_SENDQUEUEERROR, "]Connection from [%s:%d], SingleConnectMaxSendBuffer is more than(%d)!.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4ReadSendSize - m_u4SuccessSendSize);
			pMbData->release();
			//断开连接
			Close(2);
			return false;
		}

		//OUR_DEBUG ((LM_ERROR, "[CConnectHandler::PutSendPacket] Connectid=%d, length=%d!\n", GetConnectID(), pMbData->length()));
		if(0 != m_Writer.write(*pMbData, pMbData->length()))
		{
			OUR_DEBUG ((LM_ERROR, "[CProConnectHandle::PutSendPacket] Connectid=%d mb=%d m_writer.write error(%d)!\n", GetConnectID(),  pMbData->length(), errno));
			pMbData->release();
			Close();
			return false;
		}
		else
		{
			m_ThreadWriteLock.acquire();
			m_u4AllSendCount += 1;
			m_atvOutput      = ACE_OS::gettimeofday();
			m_ThreadWriteLock.release();
			//Close();
			return true;
		}
		//pMbData->release();
	}
	else
	{
		OUR_DEBUG ((LM_ERROR,"[CProConnectHandle::PutSendPacket] Connectid=%d mb is NULL!\n", GetConnectID()));
		Close();
		return false;
	}
}

bool CProConnectHandle::RecvClinetPacket(uint32 u4PackeLen)
{
	m_ThreadWriteLock.acquire();
	m_nIOCount++;
	m_ThreadWriteLock.release();
	//OUR_DEBUG((LM_ERROR, "[CProConnectHandle::RecvClinetPacket]Connectid=%d, m_nIOCount=%d.\n", GetConnectID(), m_nIOCount));

	ACE_Message_Block* pmb = NULL;
	pmb = App_MessageBlockManager::instance()->Create(u4PackeLen);
	if(pmb == NULL)
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64d, SendQueueTimeCost=%I64d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, m_u4RecvQueueCount, m_u8RecvQueueTimeCost, m_u8SendQueueTimeCost);
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::RecvClinetPacket] pmb new is NULL.\n"));
		if(m_pPacketParse->GetMessageHead() != NULL)
		{
			m_pPacketParse->GetMessageHead()->release();
		}

		if(m_pPacketParse->GetMessageBody() != NULL)
		{
			m_pPacketParse->GetMessageBody()->release();
		}
		App_PacketParsePool::instance()->Delete(m_pPacketParse);
		Close(2);
		return false;
	}

	if(m_Reader.read(*pmb, u4PackeLen) == -1)
	{
		//如果读失败，则关闭连接。
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64d, SendQueueTimeCost=%I64d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, m_u4RecvQueueCount, m_u8RecvQueueTimeCost, m_u8SendQueueTimeCost);
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::RecvClinetPacket] m_reader.read is error(%d)(%d).\n", GetConnectID(), errno));
		pmb->release();
		if(m_pPacketParse->GetMessageHead() != NULL)
		{
			m_pPacketParse->GetMessageHead()->release();
		}

		if(m_pPacketParse->GetMessageBody() != NULL)
		{
			m_pPacketParse->GetMessageBody()->release();
		}
		App_PacketParsePool::instance()->Delete(m_pPacketParse);
		Close(2);
		return false;
	}

	return true;
}

bool CProConnectHandle::CheckMessage()
{
	if(m_pPacketParse->GetMessageHead() != NULL)
	{
		m_ThreadWriteLock.acquire();
		if(m_pPacketParse->GetMessageBody() == NULL)
		{
			m_u4AllRecvSize += (uint32)m_pPacketParse->GetMessageHead()->length();
		}
		else
		{
			m_u4AllRecvSize += (uint32)m_pPacketParse->GetMessageHead()->length() + (uint32)m_pPacketParse->GetMessageBody()->length();
		}
		m_u4AllRecvCount++;

		//如果有需要监控的IP，则记录字节流信息
		App_IPAccount::instance()->UpdateIP((string)m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllSendSize);
		m_ThreadWriteLock.release();

		ACE_Date_Time dtNow;
		if(false == m_TimeConnectInfo.RecvCheck((uint8)dtNow.minute(), 1, m_u4AllRecvSize))
		{
			//超过了限定的阀值，需要关闭链接，并记录日志
			AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECTABNORMAL, 
				App_MainConfig::instance()->GetClientDataAlert()->m_u4MailID,
				"Alert", 
				"[TCP]IP=%s,Prot=%d,PacketCount=%d, RecvSize=%d.", 
				m_addrRemote.get_host_addr(), 
				m_addrRemote.get_port_number(), 
				m_TimeConnectInfo.m_u4RecvPacketCount, 
				m_TimeConnectInfo.m_u4RecvSize);


			App_PacketParsePool::instance()->Delete(m_pPacketParse);
			//设置封禁时间
			App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), App_MainConfig::instance()->GetIPAlert()->m_u4IPTimeout);
			OUR_DEBUG((LM_ERROR, "[CProConnectHandle::CheckMessage] ConnectID = %d, PutMessageBlock is check invalid.\n", GetConnectID()));
			return false;
		}

		//将数据Buff放入消息体中，传递给MakePacket处理。
		if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_PARSE, m_pPacketParse))
		{
			App_PacketParsePool::instance()->Delete(m_pPacketParse);
			OUR_DEBUG((LM_ERROR, "[CProConnectHandle::CheckMessage] ConnectID = %d, PutMessageBlock is error.\n", GetConnectID()));
		}
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::CheckMessage] ConnectID = %d, m_pPacketParse is NULL.\n", GetConnectID()));
	}

	/*
	//测试代码
	m_pPacketParse->GetMessageHead()->release();
	m_pPacketParse->GetMessageBody()->release();
	App_PacketParsePool::instance()->Delete(m_pPacketParse);
	*/

	return true;
}

_ClientConnectInfo CProConnectHandle::GetClientInfo()
{
	_ClientConnectInfo ClientConnectInfo;

	ClientConnectInfo.m_blValid             = true;
	ClientConnectInfo.m_u4ConnectID         = GetConnectID();
	ClientConnectInfo.m_addrRemote          = m_addrRemote;
	ClientConnectInfo.m_u4RecvCount         = m_u4AllRecvCount;
	ClientConnectInfo.m_u4SendCount         = m_u4AllSendCount;
	ClientConnectInfo.m_u4AllRecvSize       = m_u4AllSendSize;
	ClientConnectInfo.m_u4AllSendSize       = m_u4AllSendSize;
	ClientConnectInfo.m_u4BeginTime         = (uint32)m_atvConnect.sec();
	ClientConnectInfo.m_u4AliveTime         = (uint32)(ACE_OS::gettimeofday().sec()  -  m_atvConnect.sec());
	ClientConnectInfo.m_u4RecvQueueCount    = m_u4RecvQueueCount;
	ClientConnectInfo.m_u8RecvQueueTimeCost = m_u8RecvQueueTimeCost;
	ClientConnectInfo.m_u8SendQueueTimeCost = m_u8SendQueueTimeCost;

	return ClientConnectInfo;
}

_ClientIPInfo CProConnectHandle::GetClientIPInfo()
{
	_ClientIPInfo ClientIPInfo;
	sprintf_safe(ClientIPInfo.m_szClientIP, MAX_BUFF_50, "%s", m_addrRemote.get_host_addr());
	ClientIPInfo.m_nPort = (int)m_addrRemote.get_port_number();
	return ClientIPInfo;
}

_ClientIPInfo CProConnectHandle::GetLocalIPInfo()
{
	_ClientIPInfo ClientIPInfo;
	sprintf_safe(ClientIPInfo.m_szClientIP, MAX_BUFF_50, "%s", m_szLocalIP);
	ClientIPInfo.m_nPort = (int)m_u4LocalPort;
	return ClientIPInfo;
}

void CProConnectHandle::ClearPacketParse(ACE_Message_Block& mbCurrBlock)
{
	//链接断开
	if(m_pPacketParse->GetMessageHead() != NULL)
	{
		App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
	}

	if(m_pPacketParse->GetMessageBody() != NULL)
	{
		App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
	}

	if(&mbCurrBlock != m_pPacketParse->GetMessageHead() && &mbCurrBlock != m_pPacketParse->GetMessageBody())
	{
		//OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::handle_read_stream] Message_block release.\n"));
		App_MessageBlockManager::instance()->Close(&mbCurrBlock);
	}
	App_PacketParsePool::instance()->Delete(m_pPacketParse);
}

char* CProConnectHandle::GetConnectName()
{
	return m_szConnectName;
}

void CProConnectHandle::SetConnectName( const char* pName )
{
	sprintf_safe(m_szConnectName, MAX_BUFF_100, "%s", pName);
}

void CProConnectHandle::SetIsLog(bool blIsLog)
{
	m_blIsLog = blIsLog;
}

bool CProConnectHandle::GetIsLog()
{
	return m_blIsLog;
}

void CProConnectHandle::SetLocalIPInfo(const char* pLocalIP, uint32 u4LocalPort)
{
	sprintf_safe(m_szLocalIP, MAX_BUFF_50, "%s", pLocalIP);
	m_u4LocalPort = u4LocalPort;
}

//***************************************************************************
CProConnectManager::CProConnectManager(void)
{
	m_u4TimeCheckID      = 0;
	m_szError[0]         = '\0';
	m_blRun              = false;

	m_u4TimeConnect      = 0;
	m_u4TimeDisConnect   = 0;

	m_tvCheckConnect     = ACE_OS::gettimeofday();

	m_SendMessagePool.Init();
}

CProConnectManager::~CProConnectManager(void)
{
	CloseAll();
}

void CProConnectManager::CloseAll()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	KillTimer();
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e; b++)
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )b->second;
		if(pConnectHandler != NULL)
		{
			pConnectHandler->Close();
		}

		m_u4TimeDisConnect++;
	}

	m_mapConnectManager.clear();
}

bool CProConnectManager::Close(uint32 u4ConnectID)
{
	//客户端关闭
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		m_mapConnectManager.erase(f);
		m_u4TimeDisConnect++;

		//加入链接统计功能
		App_ConnectAccount::instance()->AddDisConnect();

		return true;
	}
	else
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::Close] ConnectID[%d] is not find.", u4ConnectID);
		return true;
	}
}

bool CProConnectManager::CloseConnect(uint32 u4ConnectID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	//服务器关闭
	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )f->second;
		m_mapConnectManager.erase(f);
		if(pConnectHandler != NULL)
		{
			pConnectHandler->ServerClose();
			m_u4TimeDisConnect++;

			//加入链接统计功能
			App_ConnectAccount::instance()->AddDisConnect();
		}
		return true;
	}
	else
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::CloseConnect] ConnectID[%d] is not find.", u4ConnectID);
		return true;
	}
}

bool CProConnectManager::AddConnect(uint32 u4ConnectID, CProConnectHandle* pConnectHandler)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	if(pConnectHandler == NULL)
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::AddConnect] pConnectHandler is NULL.");
		return false;		
	}

	m_ThreadWriteLock.acquire();
	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);
	if(f != m_mapConnectManager.end())
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::AddConnect] ConnectID[%d] is exist.", u4ConnectID);
		m_ThreadWriteLock.release();
		return false;
	}

	pConnectHandler->SetConnectID(u4ConnectID);
	//加入map
	m_mapConnectManager.insert(mapConnectManager::value_type(u4ConnectID, pConnectHandler));
	m_u4TimeConnect++;
	m_ThreadWriteLock.release();

	//加入链接统计功能
	App_ConnectAccount::instance()->AddConnect();

	return true;
}

bool CProConnectManager::SendMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint16 u2CommandID, bool blSendState, uint8 u1SendType, ACE_Time_Value& tvSendBegin, bool blDelete)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	//OUR_DEBUG((LM_ERROR,"[CProConnectManager::SendMessage]BEGIN.\n"));

	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )f->second;
		uint32 u4CommandSize = pBuffPacket->GetPacketLen();

		if(NULL != pConnectHandler)
		{
			uint32 u4PacketSize  = 0;
			pConnectHandler->SendMessage(u2CommandID, pBuffPacket, blSendState, u1SendType, u4PacketSize, blDelete);

			return true;
		}
		else
		{
			sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::SendMessage] ConnectID[%d] is not find.", u4ConnectID);
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
			return true;
		}
	}
	else
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::SendMessage] ConnectID[%d] is not find.", u4ConnectID);
		//OUR_DEBUG((LM_ERROR,"[CProConnectManager::SendMessage]%s.\n", m_szError));
		App_BuffPacketManager::instance()->Delete(pBuffPacket);
		return true;
	}

	return true;
}

bool CProConnectManager::PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, bool blSendState, bool blDelete)
{
	//OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage]BEGIN.\n"));
	if(NULL == pBuffPacket)
	{
		OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] pBuffPacket is NULL.\n"));
		return false;
	}

	//ACE_Message_Block* mb = App_MessageBlockManager::instance()->Create(sizeof(_SendMessage*));
	ACE_Message_Block* mb = NULL;

	ACE_NEW_MALLOC_NORETURN(mb, 
		static_cast<ACE_Message_Block*>(_msg_prosend_mb_allocator.malloc(sizeof(ACE_Message_Block))),
		ACE_Message_Block(sizeof(_SendMessage*), // size
		ACE_Message_Block::MB_DATA, // type
		0,
		0,
		&_msg_prosend_mb_allocator, // allocator_strategy
		0, // locking strategy
		ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, // priority
		ACE_Time_Value::zero,
		ACE_Time_Value::max_time,
		&_msg_prosend_mb_allocator,
		&_msg_prosend_mb_allocator
		));

	if(NULL != mb)
	{
		//放入发送队列
		_SendMessage* pSendMessage = m_SendMessagePool.Create();

		if(NULL == pSendMessage)
		{
			OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] new _SendMessage is error.\n"));
			if(blDelete == true)
			{
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
			}
			return false;
		}

		pSendMessage->m_u4ConnectID = u4ConnectID;
		pSendMessage->m_pBuffPacket = pBuffPacket;
		pSendMessage->m_nEvents     = u1SendType;
		pSendMessage->m_u2CommandID = u2CommandID;
		pSendMessage->m_blSendState = blSendState;
		pSendMessage->m_blDelete    = blDelete;
		pSendMessage->m_tvSend      = ACE_OS::gettimeofday();

		_SendMessage** ppSendMessage = (_SendMessage **)mb->base();
		*ppSendMessage = pSendMessage;

		//判断队列是否是已经最大
		int nQueueCount = (int)msg_queue()->message_count();
		if(nQueueCount >= (int)MAX_MSG_THREADQUEUE)
		{
			OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));
			if(blDelete == true)
			{
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
			}
			mb->release();
			return false;
		}

		ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, m_u4SendQueuePutTime);
		if(this->putq(mb, &xtime) == -1)
		{
			OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));
			if(blDelete == true)
			{
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
			}
			mb->release();
			return false;
		}
	}
	else
	{
		OUR_DEBUG((LM_ERROR,"[CMessageService::PutMessage] mb new error.\n"));
		if(blDelete == true)
		{
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
		}
		return false;
	}

	return true;
}

const char* CProConnectManager::GetError()
{
	return m_szError;
}

bool CProConnectManager::StartTimer()
{
	//启动发送线程
	if(0 != open())
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectManager::StartTimer]Open() is error.\n"));
		return false;
	}

	//避免定时器重复启动
	KillTimer();
	OUR_DEBUG((LM_ERROR, "CProConnectManager::StartTimer()-->begin....\n"));

	//检测链接发送存活包数
	uint16 u2SendAlive = App_MainConfig::instance()->GetSendAliveTime();
	m_u4TimeCheckID = App_TimerManager::instance()->schedule(this, (void *)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(u2SendAlive), ACE_Time_Value(u2SendAlive));
	if(-1 == m_u4TimeCheckID)
	{
		OUR_DEBUG((LM_ERROR, "CProConnectManager::StartTimer()--> Start thread m_u4TimeCheckID error.\n"));
		return false;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "CProConnectManager::StartTimer()--> Start thread time OK.\n"));
		return true;
	}
}

bool CProConnectManager::KillTimer()
{
	if(m_u4TimeCheckID > 0)
	{
		App_TimerManager::instance()->cancel(m_u4TimeCheckID);
		m_u4TimeCheckID = 0;
	}

	return true;
}

int CProConnectManager::handle_timeout(const ACE_Time_Value &tv, const void *arg)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

	//为了防止多线程下的链接删除问题，先把所有的链接ID读出来，再做遍历操作，减少线程竞争的机会。
	if(m_mapConnectManager.size() != 0)
	{
		for(mapConnectManager::iterator b = m_mapConnectManager.begin(); b != m_mapConnectManager.end();)
		{
			CProConnectHandle* pConnectHandler = (CProConnectHandle* )b->second;
			if(pConnectHandler != NULL)
			{
				if(false == pConnectHandler->CheckAlive())
				{
					//删除释放对象
					m_mapConnectManager.erase(b++);
				}
				else
				{
					b++;
				}
			}
			else
			{
				b++;
			}
		}
	}

	//判定是否应该记录链接日志
	ACE_Time_Value tvNow = ACE_OS::gettimeofday();
	ACE_Time_Value tvInterval(tvNow - m_tvCheckConnect);
	if(tvInterval.sec() >= MAX_MSG_HANDLETIME)
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "[CProConnectManager]CurrConnectCount = %d,TimeInterval=%d, TimeConnect=%d, TimeDisConnect=%d.", 
			GetCount(), MAX_MSG_HANDLETIME, m_u4TimeConnect, m_u4TimeDisConnect);

		//重置单位时间连接数和断开连接数
		m_u4TimeConnect    = 0;
		m_u4TimeDisConnect = 0;
		m_tvCheckConnect   = tvNow;
	}

	//检测连接总数是否超越监控阀值
	if(App_MainConfig::instance()->GetConnectAlert()->m_u4ConnectAlert > 0)
	{
		if(GetCount() > (int)App_MainConfig::instance()->GetConnectAlert()->m_u4ConnectAlert)
		{
			AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT,
				App_MainConfig::instance()->GetConnectAlert()->m_u4MailID,
				(char* )"Alert",
				"[CProConnectManager]active ConnectCount is more than limit(%d > %d).", 
				GetCount(),
				App_MainConfig::instance()->GetConnectAlert()->m_u4ConnectAlert);
		}
	}

	//检测单位时间连接数是否超越阀值
	int nCheckRet = App_ConnectAccount::instance()->CheckConnectCount();
	if(nCheckRet == 1)
	{
		AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT,
			App_MainConfig::instance()->GetConnectAlert()->m_u4MailID,
			"Alert",
			"[CProConnectManager]CheckConnectCount is more than limit(%d > %d).", 
			App_ConnectAccount::instance()->GetCurrConnect(),
			App_ConnectAccount::instance()->GetConnectMax());
	}
	else if(nCheckRet == 2)
	{
		AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT,
			App_MainConfig::instance()->GetConnectAlert()->m_u4MailID,
			"Alert",
			"[CProConnectManager]CheckConnectCount is little than limit(%d < %d).", 
			App_ConnectAccount::instance()->GetCurrConnect(),
			App_ConnectAccount::instance()->Get4ConnectMin());
	}

	//检测单位时间连接断开数是否超越阀值
	nCheckRet = App_ConnectAccount::instance()->CheckDisConnectCount();
	if(nCheckRet == 1)
	{
		AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT,
			App_MainConfig::instance()->GetConnectAlert()->m_u4MailID,
			"Alert",
			"[CProConnectManager]CheckDisConnectCount is more than limit(%d > %d).", 
			App_ConnectAccount::instance()->GetCurrConnect(),
			App_ConnectAccount::instance()->GetDisConnectMax());
	}
	else if(nCheckRet == 2)
	{
		AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT,
			App_MainConfig::instance()->GetConnectAlert()->m_u4MailID,
			"Alert",
			"[CProConnectManager]CheckDisConnectCount is little than limit(%d < %d).", 
			App_ConnectAccount::instance()->GetCurrConnect(),
			App_ConnectAccount::instance()->GetDisConnectMin());
	}

	return 0;
}

int CProConnectManager::GetCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	return (int)m_mapConnectManager.size(); 
}

int CProConnectManager::open(void* args)
{
	m_blRun = true;
	msg_queue()->high_water_mark(MAX_MSG_MASK);
	msg_queue()->low_water_mark(MAX_MSG_MASK);

	OUR_DEBUG((LM_INFO,"[CProConnectManager::open] m_u4HighMask = [%d] m_u4LowMask = [%d]\n", MAX_MSG_MASK, MAX_MSG_MASK));
	if(activate(THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED | THR_SUSPENDED, MAX_MSG_THREADCOUNT) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectManager::open] activate error ThreadCount = [%d].", MAX_MSG_THREADCOUNT));
		m_blRun = false;
		return -1;
	}

	m_u4SendQueuePutTime = App_MainConfig::instance()->GetSendQueuePutTime() * 1000;

	resume();

	return 0;
}

int CProConnectManager::svc (void)
{
	ACE_Message_Block* mb = NULL;
	ACE_Time_Value xtime;

	while(IsRun())
	{
		mb = NULL;
		//xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);
		if(getq(mb, 0) == -1)
		{
			OUR_DEBUG((LM_INFO,"[CProConnectManager::svc] getq is error[%d]!\n", errno));
			m_blRun = false;
			break;
		}
		if (mb == NULL)
		{
			continue;
		}
		_SendMessage* msg = *((_SendMessage**)mb->base());
		if (! msg)
		{
			mb->release();
			continue;
		}

		//处理发送数据
		SendMessage(msg->m_u4ConnectID, msg->m_pBuffPacket, msg->m_u2CommandID, msg->m_blSendState, msg->m_nEvents, msg->m_tvSend, msg->m_blDelete);
		m_SendMessagePool.Delete(msg);

		mb->release();
	}

	OUR_DEBUG((LM_INFO,"[CProConnectManager::svc] svc finish!\n"));
	return 0;
}

bool CProConnectManager::IsRun()
{
	return m_blRun;
}

int CProConnectManager::close(u_long)
{
	m_blRun = false;
	OUR_DEBUG((LM_INFO,"[CProConnectManager::close] close().\n"));
	return 0;
}

void CProConnectManager::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e; b++)
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )b->second;
		if(pConnectHandler != NULL)
		{
			VecClientConnectInfo.push_back(pConnectHandler->GetClientInfo());
		}
	}
}

void CProConnectManager::SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )f->second;
		if(NULL != pConnectHandler)
		{
			pConnectHandler->SetRecvQueueTimeCost(u4TimeCost);
		}
	}
}

_ClientIPInfo CProConnectManager::GetClientIPInfo(uint32 u4ConnectID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )f->second;
		if(NULL != pConnectHandler)
		{
			return pConnectHandler->GetClientIPInfo();
		}
		else
		{
			_ClientIPInfo ClientIPInfo;
			return ClientIPInfo;
		}
	}
	else
	{
		_ClientIPInfo ClientIPInfo;
		return ClientIPInfo;
	}
}

_ClientIPInfo CProConnectManager::GetLocalIPInfo(uint32 u4ConnectID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )f->second;
		if(NULL != pConnectHandler)
		{
			return pConnectHandler->GetLocalIPInfo();
		}
		else
		{
			_ClientIPInfo ClientIPInfo;
			return ClientIPInfo;
		}
	}
	else
	{
		_ClientIPInfo ClientIPInfo;
		return ClientIPInfo;
	}
}

bool CProConnectManager::PostMessageAll( IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, bool blSendState, bool blDelete)
{
	m_ThreadWriteLock.acquire();
	vecConnectManager objveCProConnectManager;
	for(mapConnectManager::iterator b = m_mapConnectManager.begin(); b != m_mapConnectManager.end(); b++)
	{
		objveCProConnectManager.push_back((uint32)b->first);
	}
	m_ThreadWriteLock.release();

	uint32 u4ConnectID = 0;
	for(uint32 i = 0; i < (uint32)objveCProConnectManager.size(); i++)
	{
		IBuffPacket* pCurrBuffPacket = App_BuffPacketManager::instance()->Create();
		if(NULL == pCurrBuffPacket)
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManager::PostMessage]pCurrBuffPacket is NULL.\n"));
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
			return false;
		}

		pCurrBuffPacket->WriteStream(pBuffPacket->GetData(), pBuffPacket->GetPacketLen());

		u4ConnectID = objveCProConnectManager[i];
		ACE_Message_Block* mb = NULL;

		ACE_NEW_MALLOC_NORETURN(mb, 
			static_cast<ACE_Message_Block*>(_msg_prosend_mb_allocator.malloc(sizeof(ACE_Message_Block))),
			ACE_Message_Block(sizeof(_SendMessage*), // size
			ACE_Message_Block::MB_DATA, // type
			0,
			0,
			&_msg_prosend_mb_allocator, // allocator_strategy
			0, // locking strategy
			ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, // priority
			ACE_Time_Value::zero,
			ACE_Time_Value::max_time,
			&_msg_prosend_mb_allocator,
			&_msg_prosend_mb_allocator
			));

		if(NULL != mb)
		{
			//放入发送队列
			_SendMessage* pSendMessage = m_SendMessagePool.Create();

			if(NULL == pSendMessage)
			{
				OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] new _SendMessage is error.\n"));
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
				return false;
			}

			pSendMessage->m_u4ConnectID = u4ConnectID;
			pSendMessage->m_pBuffPacket = pCurrBuffPacket;
			pSendMessage->m_nEvents     = u1SendType;
			pSendMessage->m_u2CommandID = u2CommandID;
			pSendMessage->m_blSendState = blSendState;
			pSendMessage->m_blDelete    = blDelete;
			pSendMessage->m_tvSend      = ACE_OS::gettimeofday();

			_SendMessage** ppSendMessage = (_SendMessage **)mb->base();
			*ppSendMessage = pSendMessage;

			//判断队列是否是已经最大
			int nQueueCount = (int)msg_queue()->message_count();
			if(nQueueCount >= (int)MAX_MSG_THREADQUEUE)
			{
				OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));
				if(blDelete == true)
				{
					App_BuffPacketManager::instance()->Delete(pBuffPacket);
				}
				m_SendMessagePool.Delete(pSendMessage);
				mb->release();
				return false;
			}

			ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);
			if(this->putq(mb, &xtime) == -1)
			{
				OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));
				if(blDelete == true)
				{
					App_BuffPacketManager::instance()->Delete(pBuffPacket);
				}
				m_SendMessagePool.Delete(pSendMessage);
				mb->release();
				return false;
			}
		}
		else
		{
			OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] mb new error.\n"));
			if(blDelete == true)
			{
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
			}
			return false;
		}
	}

	return true;
}

bool CProConnectManager::SetConnectName( uint32 u4ConnectID, const char* pName )
{
	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )f->second;
		if(NULL != pConnectHandler)
		{
			pConnectHandler->SetConnectName(pName);
			return true;
		}
		else
		{
			return false;
		}
	}	
	else
	{
		return false;
	}
}

bool CProConnectManager::SetIsLog( uint32 u4ConnectID, bool blIsLog )
{
	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )f->second;
		if(NULL != pConnectHandler)
		{
			pConnectHandler->SetIsLog(blIsLog);
			return true;
		}
		else
		{
			return false;
		}
	}	
	else
	{
		return false;
	}
}

void CProConnectManager::GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo)
{
	for(mapConnectManager::iterator b = m_mapConnectManager.begin(); b != m_mapConnectManager.end(); b++)
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )b->second;
		if(NULL != pConnectHandler && ACE_OS::strcmp(pConnectHandler->GetConnectName(), pName) == 0)
		{
			_ClientNameInfo ClientNameInfo;
			ClientNameInfo.m_nConnectID = (int)pConnectHandler->GetConnectID();
			sprintf_safe(ClientNameInfo.m_szName, MAX_BUFF_100, "%s", pConnectHandler->GetConnectName());
			sprintf_safe(ClientNameInfo.m_szClientIP, MAX_BUFF_50, "%s", pConnectHandler->GetClientIPInfo().m_szClientIP);
			ClientNameInfo.m_nPort =  pConnectHandler->GetClientIPInfo().m_nPort;
			if(pConnectHandler->GetIsLog() == true)
			{
				ClientNameInfo.m_nLog = 1;
			}
			else
			{
				ClientNameInfo.m_nLog = 0;
			}

			objClientNameInfo.push_back(ClientNameInfo);
		}
	}
}

//*********************************************************************************

CProConnectHandlerPool::CProConnectHandlerPool(void)
{
	//ConnectID计数器从1开始
	m_u4CurrMaxCount = 1;
}

CProConnectHandlerPool::~CProConnectHandlerPool(void)
{
	Close();
}

void CProConnectHandlerPool::Init(int nObjcetCount)
{
	Close();

	for(int i = 0; i < nObjcetCount; i++)
	{
		CProConnectHandle* pPacket = new CProConnectHandle();
		if(NULL != pPacket)
		{
			//添加到Free map里面
			mapHandle::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				pPacket->Init(m_u4CurrMaxCount);
				m_mapMessageFree.insert(mapHandle::value_type(pPacket, pPacket));
				m_u4CurrMaxCount++;
			}
		}
	}
}

void CProConnectHandlerPool::Close()
{
	//清理所有已存在的指针
	mapHandle::iterator itorFreeB = m_mapMessageFree.begin();
	mapHandle::iterator itorFreeE = m_mapMessageFree.end();

	for(itorFreeB; itorFreeB != itorFreeE; itorFreeB++)
	{
		CProConnectHandle* pObject = (CProConnectHandle* )itorFreeB->second;
		SAFE_DELETE(pObject);
	}

	mapHandle::iterator itorUsedB = m_mapMessageUsed.begin();
	mapHandle::iterator itorUsedE = m_mapMessageUsed.end();

	for(itorUsedB; itorUsedB != itorUsedE; itorUsedB++)
	{
		CProConnectHandle* pPacket = (CProConnectHandle* )itorUsedB->second;
		SAFE_DELETE(pPacket);
	}

	m_u4CurrMaxCount = 0;
	m_mapMessageFree.clear();
	m_mapMessageUsed.clear();
}

int CProConnectHandlerPool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageUsed.size();
}

int CProConnectHandlerPool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageFree.size();
}

CProConnectHandle* CProConnectHandlerPool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//如果free池中已经没有了，则添加到free池中。
	if(m_mapMessageFree.size() <= 0)
	{
		CProConnectHandle* pPacket = new CProConnectHandle();

		if(pPacket != NULL)
		{
			//添加到Free map里面
			mapHandle::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				pPacket->Init(m_u4CurrMaxCount);
				m_mapMessageFree.insert(mapHandle::value_type(pPacket, pPacket));
				m_u4CurrMaxCount++;
			}
		}
		else
		{
			return NULL;
		}
	}

	//从free池中拿出一个,放入到used池中
	mapHandle::iterator itorFreeB = m_mapMessageFree.begin();
	CProConnectHandle* pPacket = (CProConnectHandle* )itorFreeB->second;
	m_mapMessageFree.erase(itorFreeB);
	//添加到used map里面
	mapHandle::iterator f = m_mapMessageUsed.find(pPacket);
	if(f == m_mapMessageUsed.end())
	{
		m_mapMessageUsed.insert(mapHandle::value_type(pPacket, pPacket));
	}

	return (CProConnectHandle* )pPacket;
}

bool CProConnectHandlerPool::Delete(CProConnectHandle* pObject)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	if(NULL == pObject)
	{
		return false;
	}

	mapHandle::iterator f = m_mapMessageUsed.find(pObject);
	if(f != m_mapMessageUsed.end())
	{
		m_mapMessageUsed.erase(f);

		//添加到Free map里面
		mapHandle::iterator f = m_mapMessageFree.find(pObject);
		if(f == m_mapMessageFree.end())
		{
			m_mapMessageFree.insert(mapHandle::value_type(pObject, pObject));
		}
	}

	return true;
}

//==============================================================
CProConnectManagerGroup::CProConnectManagerGroup()
{
	m_u4CurrMaxCount     = 0;
	m_u2ThreadQueueCount = SENDQUEUECOUNT;
}

CProConnectManagerGroup::~CProConnectManagerGroup()
{
	OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::~CProConnectManagerGroup].\n"));
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e;b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		SAFE_DELETE(pConnectManager);
	}

	m_mapConnectManager.clear();
}

void CProConnectManagerGroup::Init(uint16 u2SendQueueCount)
{
	for(int i = 0; i < u2SendQueueCount; i++)
	{
		CProConnectManager* pConnectManager = new CProConnectManager();
		if(NULL != pConnectManager)	
		{
			//加入map
			m_mapConnectManager.insert(mapConnectManager::value_type(i, pConnectManager));
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::Init]Creat %d SendQueue OK.\n", i));
		}
	}

	m_u2ThreadQueueCount = (uint16)m_mapConnectManager.size();
}

uint32 CProConnectManagerGroup::GetGroupIndex()
{
	//根据链接获得命中，（简单球形命中算法）
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
	m_u4CurrMaxCount++;
	return m_u4CurrMaxCount;
}

bool CProConnectManagerGroup::AddConnect(CProConnectHandle* pConnectHandler)
{
	uint32 u4ConnectID = GetGroupIndex();

	//判断命中到哪一个线程组里面去
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::AddConnect]Out of range Queue ID.\n"));
		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::AddConnect]No find send Queue object.\n"));
		return false;		
	}

	//OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::Init]u4ConnectID=%d, u2ThreadIndex=%d.\n", u4ConnectID, u2ThreadIndex));

	return pConnectManager->AddConnect(u4ConnectID, pConnectHandler);
}

bool CProConnectManagerGroup::PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, bool blSendState, bool blDelete)
{
	//判断命中到哪一个线程组里面去
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]Out of range Queue ID.\n"));
		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
		return false;		
	}

	//OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]u4ConnectID=%d, u2ThreadIndex=%d.\n", u4ConnectID, u2ThreadIndex));

	return pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, blSendState, blDelete);
}

bool CProConnectManagerGroup::PostMessage( uint32 u4ConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType, uint16 u2CommandID, bool blSendState, bool blDelete)
{
	//判断命中到哪一个线程组里面去
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]Out of range Queue ID.\n"));

		if(blDelete == true)
		{
			SAFE_DELETE_ARRAY(pData);
		}

		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));

		if(blDelete == true)
		{
			SAFE_DELETE_ARRAY(pData);
		}

		return false;		
	}

	//OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]u4ConnectID=%d, u2ThreadIndex=%d.\n", u4ConnectID, u2ThreadIndex));
	IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create();
	if(NULL != pBuffPacket)
	{
		pBuffPacket->WriteStream(pData, nDataLen);

		if(blDelete == true)
		{
			SAFE_DELETE_ARRAY(pData);
		}

		return pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, blSendState, true);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]pBuffPacket is NULL.\n"));
		
		if(blDelete == true)
		{
			SAFE_DELETE_ARRAY(pData);
		}

		return false;
	}

}

bool CProConnectManagerGroup::PostMessage( vector<uint32> vecConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, bool blSendState, bool blDelete)
{
	uint32 u4ConnectID = 0;
	for(uint32 i = 0; i < (uint32)vecConnectID.size(); i++)
	{
		//判断命中到哪一个线程组里面去
		uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

		mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
		if(f == m_mapConnectManager.end())
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]Out of range Queue ID.\n"));
			return false;
		}

		CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
		if(NULL == pConnectManager)
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
			return false;		
		}

		pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, blSendState, blDelete);
	}

	return true;
}

bool CProConnectManagerGroup::PostMessage( vector<uint32> vecConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType, uint16 u2CommandID, bool blSendState, bool blDelete)
{
	uint32 u4ConnectID = 0;
	for(uint32 i = 0; i < (uint32)vecConnectID.size(); i++)
	{
		//判断命中到哪一个线程组里面去
		uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

		mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
		if(f == m_mapConnectManager.end())
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]Out of range Queue ID.\n"));

			if(blDelete == true)
			{
				SAFE_DELETE_ARRAY(pData);
			}

			return false;
		}

		CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
		if(NULL == pConnectManager)
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));

			if(blDelete == true)
			{
				SAFE_DELETE_ARRAY(pData);
			}

			return false;		
		}

		//OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]u4ConnectID=%d, u2ThreadIndex=%d.\n", u4ConnectID, u2ThreadIndex));
		IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create();
		if(NULL != pBuffPacket)
		{
			pBuffPacket->WriteStream(pData, nDataLen);
			
			if(blDelete == true)
			{
				SAFE_DELETE_ARRAY(pData);
			}

			return pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, blSendState, true);
		} 
		else
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]pBuffPacket is NULL.\n"));

			if(blDelete == true)
			{
				SAFE_DELETE_ARRAY(pData);
			}

			return false;
		}
	}

	return false;
}

bool CProConnectManagerGroup::CloseConnect(uint32 u4ConnectID)
{
	//判断命中到哪一个线程组里面去
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::CloseConnect]Out of range Queue ID.\n"));
		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::CloseConnect]No find send Queue object.\n"));
		return false;		
	}	

	return pConnectManager->CloseConnect(u4ConnectID);
}

_ClientIPInfo CProConnectManagerGroup::GetClientIPInfo(uint32 u4ConnectID)
{
	_ClientIPInfo objClientIPInfo;
	//判断命中到哪一个线程组里面去
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]Out of range Queue ID.\n"));
		return objClientIPInfo;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
		return objClientIPInfo;		
	}	

	return pConnectManager->GetClientIPInfo(u4ConnectID);	
}

_ClientIPInfo CProConnectManagerGroup::GetLocalIPInfo(uint32 u4ConnectID)
{
	_ClientIPInfo objClientIPInfo;
	//判断命中到哪一个线程组里面去
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetLocalIPInfo]Out of range Queue ID.\n"));
		return objClientIPInfo;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetLocalIPInfo]No find send Queue object.\n"));
		return objClientIPInfo;		
	}	

	return pConnectManager->GetLocalIPInfo(u4ConnectID);	
}


void CProConnectManagerGroup::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
	VecClientConnectInfo.clear();

	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e; b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL != pConnectManager)
		{
			pConnectManager->GetConnectInfo(VecClientConnectInfo);
		}
	}
}

int CProConnectManagerGroup::GetCount()
{
	uint32 u4Count = 0;
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e; b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL != pConnectManager)
		{
			u4Count += pConnectManager->GetCount();
		}
	}	

	return u4Count;
}

void CProConnectManagerGroup::CloseAll()
{
	uint32 u4Count = 0;
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e; b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL != pConnectManager)
		{
			pConnectManager->CloseAll();
		}
	}	
}

bool CProConnectManagerGroup::StartTimer()
{
	uint32 u4Count = 0;
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();	

	for(b; b != e;b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL != pConnectManager)
		{
			pConnectManager->StartTimer();
		}
	}

	return true;	
}

bool CProConnectManagerGroup::Close(uint32 u4ConnectID)
{
	//判断命中到哪一个线程组里面去
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]Out of range Queue ID.\n"));
		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
		return false;		
	}	

	return pConnectManager->Close(u4ConnectID);
}

const char* CProConnectManagerGroup::GetError()
{
	return (char* )"";
}

void CProConnectManagerGroup::SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost)
{
	//判断命中到哪一个线程组里面去
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]Out of range Queue ID.\n"));
		return;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
		return;		
	}		

	pConnectManager->SetRecvQueueTimeCost(u4ConnectID, u4TimeCost);
}

bool CProConnectManagerGroup::PostMessageAll( IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, bool blSendState, bool blDelete)
{
	//全部群发
	for(mapConnectManager::iterator b = m_mapConnectManager.begin(); b != m_mapConnectManager.end(); b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL == pConnectManager)
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
			return false;		
		}

		pConnectManager->PostMessageAll(pBuffPacket, u1SendType, u2CommandID, blSendState, blDelete);
	}

	//用完了就删除
	App_BuffPacketManager::instance()->Delete(pBuffPacket);

	return true;
}

bool CProConnectManagerGroup::PostMessageAll( const char* pData, uint32 nDataLen, uint8 u1SendType, uint16 u2CommandID, bool blSendState, bool blDelete)
{
	IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create();
	if(NULL == pBuffPacket)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessageAll]pBuffPacket is NULL.\n"));

		if(blDelete == true)
		{
			SAFE_DELETE_ARRAY(pData);
		}

		return false;
	}
	else
	{
		pBuffPacket->WriteStream(pData, nDataLen);

		if(blDelete == true)
		{
			SAFE_DELETE_ARRAY(pData);
		}
	}

	//全部群发
	for(mapConnectManager::iterator b = m_mapConnectManager.begin(); b != m_mapConnectManager.end(); b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL == pConnectManager)
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));

			if(blDelete == true)
			{
				SAFE_DELETE_ARRAY(pData);
			}

			return false;		
		}

		pConnectManager->PostMessageAll(pBuffPacket, u1SendType, u2CommandID, u1SendType, true);
	}

	//用完了就删除
	App_BuffPacketManager::instance()->Delete(pBuffPacket);

	return true;
}

bool CProConnectManagerGroup::SetConnectName( uint32 u4ConnectID, const char* pName )
{
	//判断命中到哪一个线程组里面去
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::CloseConnect]Out of range Queue ID.\n"));
		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::CloseConnect]No find send Queue object.\n"));
		return false;		
	}	

	return pConnectManager->SetConnectName(u4ConnectID, pName);	
}

bool CProConnectManagerGroup::SetIsLog( uint32 u4ConnectID, bool blIsLog )
{
	//判断命中到哪一个线程组里面去
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::CloseConnect]Out of range Queue ID.\n"));
		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::CloseConnect]No find send Queue object.\n"));
		return false;		
	}	

	return pConnectManager->SetIsLog(u4ConnectID, blIsLog);		
}

void CProConnectManagerGroup::GetClientNameInfo( const char* pName, vecClientNameInfo& objClientNameInfo )
{
	objClientNameInfo.clear();
	//全部查找
	for(mapConnectManager::iterator b = m_mapConnectManager.begin(); b != m_mapConnectManager.end(); b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL != pConnectManager)
		{
			pConnectManager->GetClientNameInfo(pName, objClientNameInfo);
		}
	}	
}