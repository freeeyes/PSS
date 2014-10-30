#include "ProConnectClient.h"
#include "ClientProConnectManager.h"
#include "MainConfig.h"

CProConnectClient::CProConnectClient(void)
{
	m_nIOCount          = 1;
	m_nServerID         = 0;
	m_mbRecv            = NULL;
	m_pClientMessage    = NULL;
    m_u4MaxPacketSize   = MAX_MSG_PACKETLENGTH;

	m_u4SendSize        = 0;
	m_u4SendCount       = 0;
	m_u4RecvSize        = 0;
	m_u4RecvCount       = 0;
	m_u4CostTime        = 0;
}

CProConnectClient::~CProConnectClient(void)
{
}

void CProConnectClient::Close()
{
	m_ThreadWritrLock.acquire();
	if(m_nIOCount > 0)
	{
		m_nIOCount--;
	}
	m_ThreadWritrLock.release();

	if(m_nIOCount == 0)
	{
		m_Reader.cancel();
		m_Writer.cancel();

		if(this->handle() != ACE_INVALID_HANDLE)
		{
			ACE_OS::closesocket(this->handle());
			this->handle(ACE_INVALID_HANDLE);
		}

		App_ClientProConnectManager::instance()->CloseByClient(m_nServerID);

		OUR_DEBUG((LM_DEBUG, "[CProConnectClient::Close]delete OK.\n"));
		delete this;
	}
}

void CProConnectClient::ClientClose()
{
	if(this->handle() != ACE_INVALID_HANDLE)
	{
		ACE_OS::shutdown(this->handle(), SD_SEND);
		ACE_OS::closesocket(this->handle());
		this->handle(ACE_INVALID_HANDLE);
	}
}

void CProConnectClient::SetServerID(int nServerID)
{
	m_nServerID = nServerID;
}

void CProConnectClient::SetClientMessage(IClientMessage* pClientMessage)
{
	m_pClientMessage = pClientMessage;
}

int CProConnectClient::GetServerID()
{
	return m_nServerID;
}

void CProConnectClient::open(ACE_HANDLE h, ACE_Message_Block&)
{
    //从配置文件获取数据
    m_u4MaxPacketSize  = App_MainConfig::instance()->GetBuffSize();

	m_nIOCount = 1;
	this->handle(h);
	if(this->m_Reader.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_POSTDEFINE)) == -1||this->m_Writer.open(*this, h,  0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_POSTDEFINE)) == -1)
	{
		OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::open] m_reader or m_reader == 0.\n"));	
		Close();
		return;
	}

	m_u4SendSize        = 0;
	m_u4SendCount       = 0;
	m_u4RecvSize        = 0;
	m_u4RecvCount       = 0;
	m_u4CostTime        = 0;
	m_atvBegin          = ACE_OS::gettimeofday();

	App_ClientProConnectManager::instance()->SetHandler(m_nServerID, this);
	m_pClientMessage = App_ClientProConnectManager::instance()->GetClientMessage(m_nServerID);
	OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::open](%d) open OK.\n", m_nServerID));

	m_emState = MESSAGE_HEAD;
	RecvData(CONSOLE_HEAD_SIZE);
}


void CProConnectClient::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
{
	ACE_Message_Block& mb = result.message_block();
	uint32 u4PacketLen = (uint32)result.bytes_transferred();
	uint32 u4BodySize = 0;
	
	if(!result.success() || u4PacketLen == 0)
	{
		mb.release();
		if(NULL != m_pClientMessage)
		{
			_ClientIPInfo objServerIPInfo;
			sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrRemote.get_host_addr());
			objServerIPInfo.m_nPort = m_AddrRemote.get_port_number();
			m_pClientMessage->ConnectError((int)ACE_OS::last_error(), objServerIPInfo);
		}
		Close();
		return;
	}
	else 
	{
		//处理接收数据
		if(m_emState == MESSAGE_HEAD)
		{
			ACE_OS::memcpy(&u4BodySize, mb.rd_ptr(), sizeof(int));

			mb.release();
			m_emState =MESSAGE_BODY; 

			//接受下一个数据包
			RecvData(u4BodySize);
		}
		else
		{
			if(NULL != m_pClientMessage)
			{
				_ClientIPInfo objServerIPInfo;
				sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrRemote.get_host_addr());
				objServerIPInfo.m_nPort = m_AddrRemote.get_port_number();
				m_pClientMessage->RecvData(&mb, objServerIPInfo);
				mb.release();
			}

			m_emState = MESSAGE_HEAD;
			//接受下一个数据包
			RecvData(CONSOLE_HEAD_SIZE);
		}
		


	}
}

void CProConnectClient::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
{
	ACE_Message_Block& mblk = result.message_block();
	if(!result.success() || result.bytes_transferred() == 0)
	{
		//处理数据发送出错
		OUR_DEBUG((LM_DEBUG, "[CProConnectClient::handle_write_stream]Write error(%d).\n", ACE_OS::last_error()));
		mblk.release();
		if(NULL != m_pClientMessage)
		{
			_ClientIPInfo objServerIPInfo;
			sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrRemote.get_host_addr());
			objServerIPInfo.m_nPort = m_AddrRemote.get_port_number();
			m_pClientMessage->ConnectError((int)ACE_OS::last_error(), objServerIPInfo);
		}
		Close();
	}
	else
	{
		mblk.release();
	}
}

void CProConnectClient::addresses(const ACE_INET_Addr &remote_address, const ACE_INET_Addr &local_address)
{
	m_AddrRemote = remote_address;
}

bool CProConnectClient::RecvData(uint32 u4PacketLen)
{
	ACE_NEW_NORETURN(m_mbRecv, ACE_Message_Block(u4PacketLen));
	if(this->m_Reader.read(*m_mbRecv, m_mbRecv->space()) == -1)
	{
		OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::open] m_reader is error(%d).\n", (int)ACE_OS::last_error()));	
		m_mbRecv->release();
		if(NULL != m_pClientMessage)
		{
			_ClientIPInfo objServerIPInfo;
			sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrRemote.get_host_addr());
			objServerIPInfo.m_nPort = m_AddrRemote.get_port_number();
			m_pClientMessage->ConnectError((int)ACE_OS::last_error(), objServerIPInfo);
		}
		Close();
		return false;
	}

	return true;
}

bool CProConnectClient::SendData(ACE_Message_Block* pmblk)
{
	//OUR_DEBUG((LM_DEBUG, "[CProConnectClient::SendData]Begin.\n"));

	//发送信息
	if(NULL != pmblk)
	{
		int nLen = (int)pmblk->length();
		if (m_Writer.write(*pmblk, pmblk->length()) == -1)
		{
			OUR_DEBUG((LM_DEBUG,"[CProConnectClient::SendData] Send Error(%d).\n", ACE_OS::last_error()));	
			pmblk->release();
			if(NULL != m_pClientMessage)
			{
				_ClientIPInfo objServerIPInfo;
				sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrRemote.get_host_addr());
				objServerIPInfo.m_nPort = m_AddrRemote.get_port_number();
				m_pClientMessage->ConnectError((int)ACE_OS::last_error(), objServerIPInfo);
			}
			Close();
			return false;
		}

		m_u4SendSize += (uint32)nLen;
		m_u4SendCount++;		
	}

	return true;
}

_ClientConnectInfo CProConnectClient::GetClientConnectInfo()
{
	_ClientConnectInfo ClientConnectInfo;
	ClientConnectInfo.m_blValid       = true;
	//ClientConnectInfo.m_addrRemote    = m_AddrRemote;
	ClientConnectInfo.m_u4AliveTime   = (uint32)(ACE_OS::gettimeofday().sec() - m_atvBegin.sec());
	ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
	ClientConnectInfo.m_u4RecvCount   = m_u4RecvCount;
	ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
	ClientConnectInfo.m_u4SendCount   = m_u4SendCount;
	ClientConnectInfo.m_u4ConnectID   = m_nServerID;
	ClientConnectInfo.m_u4BeginTime   = (uint32)m_atvBegin.sec();
	return ClientConnectInfo;
}

