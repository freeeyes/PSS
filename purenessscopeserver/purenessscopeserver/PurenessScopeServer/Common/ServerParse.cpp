#include "ServerParse.h"

CServerParse::CServerParse()
{
	//在这里设置包头的长度，因为大部分的包头长度是固定的。包体长度是可变的。
	m_u4PacketHead      = PACKET_HEAD;
	m_u2PacketCommandID = 0;
	m_u4PacketData      = 0;

    m_blIsHandleHead          = false;

	m_pmbHead           = NULL;
	m_pmbBody           = NULL;
}

CServerParse::~CServerParse()
{
	OUR_DEBUG((LM_INFO, "[CServerParse::~CServerParse].\n"));
}

void CServerParse::Init()
{
	m_u4PacketHead      = PACKET_HEAD;
	m_u2PacketCommandID = 0;
	m_u4PacketData      = 0;

    m_blIsHandleHead          = false;

	m_pmbHead           = NULL;
	m_pmbBody           = NULL;
}

uint32 CServerParse::GetPacketHeadLen()
{
	return m_u4PacketHead;
}

uint32 CServerParse::GetPacketBodyLen()
{
	return m_u4PacketData;
}

uint16 CServerParse::GetPacketCommandID()
{
	return m_u2PacketCommandID;
}

bool CServerParse::GetIsHandleHead()
{
    return m_blIsHandleHead;
}

bool CServerParse::SetPacketHead(char* pData, uint32 u4Len)
{
	//这里添加自己对包头的分析，主要分析出包长度。
	if(u4Len == sizeof(uint32))
	{
		memcpy_safe(pData, (uint32)sizeof(uint32), (char* )&m_u4PacketData, (uint32)sizeof(uint32));
        m_blIsHandleHead = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool CServerParse::SetPacketBody(char* pData, uint32 u4Len)
{
	//这里分析出包体内的一些数据，如果包头包含了CommandID，那么包体就不必做解析。
	if(u4Len >= sizeof(uint16))
	{
		memcpy_safe(pData, (uint32)sizeof(uint16), (char* )&m_u2PacketCommandID, (uint32)sizeof(uint16));
        m_blIsHandleHead = false;
		return true;
	}
	else
	{
        m_blIsHandleHead = false;
		return false;
	}
}

bool CServerParse::SetMessageHead(ACE_Message_Block* pmbHead)
{
	m_pmbHead = pmbHead;
	return true;
}

bool CServerParse::SetMessageBody(ACE_Message_Block* pmbBody)
{
	m_pmbBody = pmbBody;
	return true;
}

ACE_Message_Block* CServerParse::GetMessageHead()
{
	return m_pmbHead;
}

ACE_Message_Block* CServerParse::GetMessageBody()
{
	return m_pmbBody;
}

uint32 CServerParse::MakePacketLength(uint32 u4DataLen)
{
	return u4DataLen + sizeof(uint32);
}

bool CServerParse::MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData)
{
	if(pMbData == NULL)
	{
		return false;
	}

	//拼装数据包
	memcpy_safe((char* )&u4Len, (uint32)sizeof(uint32), pMbData->wr_ptr(), (uint32)sizeof(uint32));
	pMbData->wr_ptr(sizeof(uint32));
	//NOTICE，这里的pMbData可能会有内存溢出，这里需要完善一下
	memcpy_safe((char* )pData, u4Len, pMbData->wr_ptr(), u4Len);
	pMbData->wr_ptr(u4Len);

	return true;
}

void CServerParse::Close()
{
	if(m_pmbHead != NULL)
	{
		m_pmbHead = NULL;
	}

	if(m_pmbBody != NULL)
	{
		m_pmbBody = NULL;
	}

    m_blIsHandleHead = false;
}

