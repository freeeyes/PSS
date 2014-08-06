#include "MakePacket.h"


CMakePacketPool::CMakePacketPool()
{
}

CMakePacketPool::~CMakePacketPool()
{
	OUR_DEBUG((LM_INFO, "[CMakePacketPool::~CMakePacketPool].\n"));
	Close();
}

void CMakePacketPool::Init(uint32 u4PacketCount)
{
	Close();

	for(int i = 0; i < (int)u4PacketCount; i++)
	{
		_MakePacket* pPacket = new _MakePacket();
		if(NULL != pPacket)
		{
			//添加到Free map里面
			mapPacket::iterator f = m_mapPacketFree.find(pPacket);
			if(f == m_mapPacketFree.end())
			{
				m_mapPacketFree.insert(mapPacket::value_type(pPacket, pPacket));
			}
		}
	}
}

void CMakePacketPool::Close()
{
	//清理所有已存在的指针
	for(mapPacket::iterator itorFreeB = m_mapPacketFree.begin(); itorFreeB != m_mapPacketFree.end(); itorFreeB++)
	{
		_MakePacket* pPacket = (_MakePacket* )itorFreeB->second;
		SAFE_DELETE(pPacket);
	}

	for(mapPacket::iterator itorUsedB = m_mapPacketUsed.begin(); itorUsedB != m_mapPacketUsed.end(); itorUsedB++)
	{
		_MakePacket* pPacket = (_MakePacket* )itorUsedB->second;
		OUR_DEBUG((LM_ERROR, "[CMakePacketPool::Close]MakePacket has used!!memory address[0x%08x].\n", pPacket));
		SAFE_DELETE(pPacket);
	}

	m_mapPacketFree.clear();
	m_mapPacketUsed.clear();
}

int CMakePacketPool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapPacketUsed.size();
}

int CMakePacketPool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapPacketFree.size();
}

_MakePacket* CMakePacketPool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//如果free池中已经没有了，则添加到free池中。
	if(m_mapPacketFree.size() <= 0)
	{
		_MakePacket* pPacket = new _MakePacket();

		if(pPacket != NULL)
		{
			//添加到Free map里面
			mapPacket::iterator f = m_mapPacketFree.find(pPacket);
			if(f == m_mapPacketFree.end())
			{
				m_mapPacketFree.insert(mapPacket::value_type(pPacket, pPacket));
			}
		}
		else
		{
			return NULL;
		}
	}

	//从free池中拿出一个,放入到used池中
	mapPacket::iterator itorFreeB = m_mapPacketFree.begin();
	_MakePacket* pPacket = (_MakePacket* )itorFreeB->second;
	m_mapPacketFree.erase(itorFreeB);
	//添加到used map里面
	mapPacket::iterator f = m_mapPacketUsed.find(pPacket);
	if(f == m_mapPacketUsed.end())
	{
		m_mapPacketUsed.insert(mapPacket::value_type(pPacket, pPacket));
	}

	return (_MakePacket* )pPacket;
}

bool CMakePacketPool::Delete(_MakePacket* pBuffPacket)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	if(NULL == pBuffPacket)
	{
		return false;
	}
	pBuffPacket->Clear();

	mapPacket::iterator f = m_mapPacketUsed.find(pBuffPacket);
	if(f != m_mapPacketUsed.end())
	{
		m_mapPacketUsed.erase(f);

		//添加到Free map里面
		mapPacket::iterator f = m_mapPacketFree.find(pBuffPacket);
		if(f == m_mapPacketFree.end())
		{
			m_mapPacketFree.insert(mapPacket::value_type(pBuffPacket, pBuffPacket));
		}
	}

	return true;
}


//*********************************************************************

CMakePacket::CMakePacket(void)
{
}

CMakePacket::~CMakePacket(void)
{
	OUR_DEBUG((LM_INFO, "[CMakePacket::~CMakePacket].\n"));
}

bool CMakePacket::Init()
{
	m_MakePacketPool.Init(MAX_PACKET_PARSE);
	return true;
}


bool CMakePacket::PutUDPMessageBlock(const ACE_INET_Addr& AddrRemote, uint8 u1Option, CPacketParse* pPacketParse)
{
	_MakePacket* pMakePacket = m_MakePacketPool.Create();
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::PutUDPMessageBlock] Get pMakePacket is NULL.\n"));
		return false;
	}

	pMakePacket->m_u4ConnectID       = UDP_HANDER_ID;
	pMakePacket->m_u1Option          = u1Option;
	pMakePacket->m_pPacketParse      = pPacketParse;
	pMakePacket->m_AddrRemote        = AddrRemote;
	pMakePacket->m_PacketType        = PACKET_UDP;

	ProcessMessageBlock(pMakePacket);

	return true;
}

bool CMakePacket::PutMessageBlock(uint32 u4ConnectID, uint8 u1Option, CPacketParse* pPacketParse)
{
	_MakePacket* pMakePacket = m_MakePacketPool.Create();
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::PutMessageBlock] Get pMakePacket is NULL.\n"));
		return false;
	}

	pMakePacket->m_u4ConnectID       = u4ConnectID;
	pMakePacket->m_u1Option          = u1Option;
	pMakePacket->m_pPacketParse      = pPacketParse;

	ProcessMessageBlock(pMakePacket);

	return true;
}

bool CMakePacket::ProcessMessageBlock(_MakePacket* pMakePacket)
{
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR,"[CMakePacket::ProcessMessageBlock] pMakePacket is NULL.\n"));
		return false;
	}

	//根据操作OP，调用相应的方法。
	CMessage* pMessage = NULL;
	if(pMakePacket->m_u1Option == PACKET_PARSE)
	{
		if(pMakePacket->m_PacketType == 0)
		{
			//TCP数据包处理方法
			pMessage = SetMessage(pMakePacket->m_pPacketParse, pMakePacket->m_u4ConnectID);
		}
		else
		{
			//UDP数据包处理方法
			pMessage = SetMessage(pMakePacket->m_pPacketParse, pMakePacket->m_AddrRemote);
		}		
	}
	else if(pMakePacket->m_u1Option == PACKET_CONNECT)
	{
		pMessage = SetMessageConnect(pMakePacket->m_u4ConnectID);
	}
	else if(pMakePacket->m_u1Option == PACKET_CDISCONNECT)
	{
		pMessage = SetMessageCDisConnect(pMakePacket->m_u4ConnectID);
	}
	else if(pMakePacket->m_u1Option == PACKET_SDISCONNECT)
	{
		pMessage = SetMessageSDisConnect(pMakePacket->m_u4ConnectID);
	}

	if(NULL != pMessage)
	{
		//将要处理的消息放入消息处理线程
		if(false == App_MessageServiceGroup::instance()->PutMessage(pMessage))
		{
			OUR_DEBUG((LM_ERROR, "[CMakePacket::ProcessMessageBlock] App_MessageServiceGroup::instance()->PutMessage Error.\n"));
			App_MessagePool::instance()->Delete(pMessage);
			App_PacketParsePool::instance()->Delete(pMakePacket->m_pPacketParse);
			m_MakePacketPool.Delete(pMakePacket);
			return false;
		}
	}
	else
	{
			App_MessagePool::instance()->Delete(pMessage);
			App_PacketParsePool::instance()->Delete(pMakePacket->m_pPacketParse);
			m_MakePacketPool.Delete(pMakePacket);
			return false;		
	}

	if(NULL != pMakePacket->m_pPacketParse)
	{
		App_PacketParsePool::instance()->Delete(pMakePacket->m_pPacketParse);
	}

	m_MakePacketPool.Delete(pMakePacket);

	return true;
}

CMessage* CMakePacket::SetMessage(CPacketParse* pPacketParse, uint32 u4ConnectID)
{
	//创建新的Message对象
	CMessage* pMessage = App_MessagePool::instance()->Create();
	if(NULL == pMessage)
	{
		//写入接收包错误
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage is NULL.\n", u4ConnectID));
		return NULL;
	}

	//填充数据包头信息
	_PacketHeadInfo objPacketHeadInfo;
	pPacketParse->GetPacketHeadInfo(objPacketHeadInfo);
	pMessage->SetPacketHeadInfo(objPacketHeadInfo);

	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = pPacketParse->GetPacketCommandID();
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)ACE_OS::gettimeofday().sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = pPacketParse->GetPacketHeadSrcLen();
		pMessage->GetMessageBase()->m_u4BodySrcSize = pPacketParse->GetPacketBodySrcLen();
		//pMessage->GetMessageBase()->m_ProfileTime.Start();

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(pPacketParse->GetMessageHead());
		pMessage->SetPacketBody(pPacketParse->GetMessageBody());

		return pMessage;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
		return NULL;
	}
}

CMessage* CMakePacket::SetMessage(CPacketParse* pPacketParse, const ACE_INET_Addr& AddrRemote)
{
	//创建新的Message对象
	CMessage* pMessage = App_MessagePool::instance()->Create();
	if(NULL == pMessage)
	{
		//写入接收包错误
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] UDP ConnectID, pMessage is NULL.\n"));
		return NULL;
	}

	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = UDP_HANDER_ID;
		pMessage->GetMessageBase()->m_u2Cmd         = pPacketParse->GetPacketCommandID();
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)ACE_OS::gettimeofday().sec();
		pMessage->GetMessageBase()->m_u4Port        = (uint32)AddrRemote.get_port_number();
		pMessage->GetMessageBase()->m_u1PacketType  = PACKET_UDP;
		pMessage->GetMessageBase()->m_u4HeadSrcSize = pPacketParse->GetPacketHeadSrcLen();
		pMessage->GetMessageBase()->m_u4BodySrcSize = pPacketParse->GetPacketBodySrcLen();
		sprintf_safe(pMessage->GetMessageBase()->m_szIP, MAX_BUFF_20, "%s", AddrRemote.get_host_addr());
		//pMessage->GetMessageBase()->m_ProfileTime.Start();

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(pPacketParse->GetMessageHead());
		pMessage->SetPacketBody(pPacketParse->GetMessageBody());
		
		return pMessage;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] UDP ConnectID, pMessage->GetMessageBase() is NULL.\n"));
		return NULL;
	}
}

CMessage* CMakePacket::SetMessageConnect(uint32 u4ConnectID)
{
	//创建新的Message对象
	CMessage* pMessage = App_MessagePool::instance()->Create();
	if(NULL == pMessage)
	{
		//写入接收包错误
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage is NULL.\n", u4ConnectID));
		return NULL;
	}

	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_CONNECT;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)ACE_OS::gettimeofday().sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;
		//pMessage->GetMessageBase()->m_ProfileTime.Start();

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);

		return pMessage;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
		return NULL;
	}
}

CMessage* CMakePacket::SetMessageCDisConnect(uint32 u4ConnectID)
{
	//创建新的Message对象
	CMessage* pMessage = App_MessagePool::instance()->Create();
	if(NULL == pMessage)
	{
		//写入接收包错误
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage is NULL.\n", u4ConnectID));
		return NULL;
	}

	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_CDISCONNET;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)ACE_OS::gettimeofday().sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;
		//pMessage->GetMessageBase()->m_ProfileTime.Start();

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);

		return pMessage;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
		return NULL;
	}
}

CMessage* CMakePacket::SetMessageSDisConnect(uint32 u4ConnectID)
{
	//创建新的Message对象
	CMessage* pMessage = App_MessagePool::instance()->Create();
	if(NULL == pMessage)
	{
		//写入接收包错误
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage is NULL.\n", u4ConnectID));
		return NULL;
	}

	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_SDISCONNET;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)ACE_OS::gettimeofday().sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;
		//pMessage->GetMessageBase()->m_ProfileTime.Start();

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);

		return pMessage;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
		return NULL;
	}
}

