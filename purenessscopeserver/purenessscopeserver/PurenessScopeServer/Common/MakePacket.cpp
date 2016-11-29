#include "MakePacket.h"

CMakePacket::CMakePacket(void)
{
}

CMakePacket::~CMakePacket(void)
{
	OUR_DEBUG((LM_INFO, "[CMakePacket::~CMakePacket].\n"));
}

bool CMakePacket::Init()
{
	return true;
}


bool CMakePacket::PutUDPMessageBlock(const ACE_INET_Addr& AddrRemote, uint8 u1Option, _MakePacket* pMakePacket, ACE_Time_Value& tvNow)
{
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::PutUDPMessageBlock] Get pMakePacket is NULL.\n"));
		return false;
	}

	pMakePacket->m_u1Option          = u1Option;
	pMakePacket->m_AddrRemote        = AddrRemote;

	ProcessMessageBlock(pMakePacket, tvNow);

	return true;
}

bool CMakePacket::PutMessageBlock(uint32 u4ConnectID, uint8 u1Option, _MakePacket* pMakePacket, ACE_Time_Value& tvNow)
{
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::PutMessageBlock] Get pMakePacket is NULL.\n"));
		return false;
	}

	pMakePacket->m_u4ConnectID       = u4ConnectID;
	pMakePacket->m_u1Option          = u1Option;
	pMakePacket->m_PacketType        = PACKET_TCP;

	ProcessMessageBlock(pMakePacket, tvNow);

	return true;
}

bool CMakePacket::ProcessMessageBlock(_MakePacket* pMakePacket, ACE_Time_Value& tvNow)
{
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR,"[CMakePacket::ProcessMessageBlock] pMakePacket is NULL.\n"));
		return false;
	}

	//根据操作OP，调用相应的方法。
	CMessage* pMessage = App_MessageServiceGroup::instance()->CreateMessage(pMakePacket->m_u4ConnectID, pMakePacket->m_PacketType);
	if(NULL == pMessage)
	{
		OUR_DEBUG((LM_ERROR,"[CMakePacket::ProcessMessageBlock] pMessage is NULL.\n"));
		return false;
	}
	
	if(pMakePacket->m_u1Option == PACKET_PARSE)
	{
		if(pMakePacket->m_PacketType == 0)
		{
			//TCP数据包处理方法
			//记录IP
			sprintf_safe(pMessage->GetMessageBase()->m_szIP, MAX_BUFF_20, "%s", pMakePacket->m_AddrRemote.get_host_addr());
			pMessage->GetMessageBase()->m_u4Port = (uint32)pMakePacket->m_AddrRemote.get_port_number(); 
			sprintf_safe(pMessage->GetMessageBase()->m_szListenIP, MAX_BUFF_20, "%s", pMakePacket->m_AddrListen.get_host_addr());
			pMessage->GetMessageBase()->m_u4ListenPort = (uint32)pMakePacket->m_AddrListen.get_port_number(); 

			SetMessage(pMakePacket->m_pPacketParse, pMakePacket->m_u4ConnectID, pMessage, tvNow);
		}
		else
		{
			//UDP数据包处理方法
			SetMessage(pMakePacket->m_pPacketParse, pMakePacket->m_AddrRemote, pMessage, tvNow);
		}		
	}
	else if(pMakePacket->m_u1Option == PACKET_CONNECT)
	{
		SetMessageConnect(pMakePacket->m_u4ConnectID, pMessage, tvNow);
	}
	else if(pMakePacket->m_u1Option == PACKET_CDISCONNECT)
	{
		SetMessageCDisConnect(pMakePacket->m_u4ConnectID, pMessage, tvNow);
	}
	else if(pMakePacket->m_u1Option == PACKET_SDISCONNECT)
	{
		SetMessageSDisConnect(pMakePacket->m_u4ConnectID, pMessage, tvNow);
	}
	else if(pMakePacket->m_u1Option == PACKET_SEND_TIMEOUT)
	{
		SetMessageSendTimeout(pMakePacket->m_u4ConnectID, pMessage, tvNow);
	}
	else if(pMakePacket->m_u1Option == PACKET_CHEK_TIMEOUT)
	{
		SetMessageSendTimeout(pMakePacket->m_u4ConnectID, pMessage, tvNow);
	}

	//将要处理的消息放入消息处理线程
	if(false == App_MessageServiceGroup::instance()->PutMessage(pMessage))
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::ProcessMessageBlock] App_MessageServiceGroup::instance()->PutMessage Error.\n"));
		App_MessageServiceGroup::instance()->DeleteMessage(pMakePacket->m_u4ConnectID, pMessage);
		return false;
	}

	return true;
}

void CMakePacket::SetMessage(CPacketParse* pPacketParse, uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = pPacketParse->GetPacketCommandID();
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = pPacketParse->GetPacketHeadSrcLen();
		pMessage->GetMessageBase()->m_u4BodySrcSize = pPacketParse->GetPacketBodySrcLen();

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(pPacketParse->GetMessageHead());
		pMessage->SetPacketBody(pPacketParse->GetMessageBody());
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

void CMakePacket::SetMessage(CPacketParse* pPacketParse, const ACE_INET_Addr& AddrRemote, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = UDP_HANDER_ID;
		pMessage->GetMessageBase()->m_u2Cmd         = pPacketParse->GetPacketCommandID();
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4Port        = (uint32)AddrRemote.get_port_number();
		pMessage->GetMessageBase()->m_u1PacketType  = PACKET_UDP;
		pMessage->GetMessageBase()->m_u4HeadSrcSize = pPacketParse->GetPacketHeadSrcLen();
		pMessage->GetMessageBase()->m_u4BodySrcSize = pPacketParse->GetPacketBodySrcLen();
		sprintf_safe(pMessage->GetMessageBase()->m_szIP, MAX_BUFF_20, "%s", AddrRemote.get_host_addr());

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(pPacketParse->GetMessageHead());
		pMessage->SetPacketBody(pPacketParse->GetMessageBody());
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] UDP ConnectID, pMessage->GetMessageBase() is NULL.\n"));
	}
}

void CMakePacket::SetMessageConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_CONNECT;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageConnect] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

void CMakePacket::SetMessageCDisConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_CDISCONNET;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageCDisConnect] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

void CMakePacket::SetMessageSDisConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_SDISCONNET;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageSDisConnect] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

void CMakePacket::SetMessageSendTimeout(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_SENDTIMEOUT;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageSendTimeout] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

void CMakePacket::SetMessageCheckTimeout(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_CHECKTIMEOUT;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageCheckTimeout] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}


void CMakePacket::SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//开始组装数据
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = (uint16)CLINET_LINK_SENDERROR;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//将接受的数据缓冲放入CMessage对象
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(pBodyMessage);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageSendError] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

bool CMakePacket::PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, ACE_Time_Value& tvNow)
{
	CMessage* pMessage = App_MessageServiceGroup::instance()->CreateMessage(u4ConnectID, (uint8)PACKET_TCP);
	if(NULL == pMessage)
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::PutSendErrorMessage] pMessage is NULL.\n"));
		App_MessageBlockManager::instance()->Close(pBodyMessage);
		return false;
	}
	
	SetMessageSendError(u4ConnectID, pBodyMessage, pMessage, tvNow);
	if(NULL != pMessage)
	{
		//将要处理的消息放入消息处理线程
		if(false == App_MessageServiceGroup::instance()->PutMessage(pMessage))
		{
			OUR_DEBUG((LM_ERROR, "[CMakePacket::PutSendErrorMessage] App_MessageServiceGroup::instance()->PutMessage Error.\n"));
			App_MessageBlockManager::instance()->Close(pBodyMessage);
			App_MessageServiceGroup::instance()->DeleteMessage(u4ConnectID, pMessage);
			return false;
		}
	}
	else
	{
		return false;	
	}

	return true;
}

