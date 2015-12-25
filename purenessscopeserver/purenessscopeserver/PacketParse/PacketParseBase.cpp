#include "PacketParseBase.h"

CPacketParseBase::CPacketParseBase(void) 
{
	//在这里设置包头的长度，因为大部分的包头长度是固定的。包体长度是可变的。
	m_u4PacketHead      = PACKET_HEAD;
	m_u2PacketCommandID = 0;
	m_u4PacketData      = 0;
	m_u4HeadSrcSize     = 0;
	m_u4BodySrcSize     = 0;
	m_u1Sort            = 0;

	//这里修改属于你的包解析版本号
	sprintf_safe(m_szPacketVersion, MAX_BUFF_20, "0.90");

	//这里设置你的包模式
	m_u1PacketMode      = PACKET_WITHHEAD;

	m_blIsHandleHead    = true;

	m_pmbHead           = NULL;
	m_pmbBody           = NULL;
}

CPacketParseBase::~CPacketParseBase(void)
{
}

void CPacketParseBase::Clear()
{
	m_pmbHead = NULL;
	m_pmbBody = NULL;

	m_blIsHandleHead = true;

	m_u4PacketData      = 0;
	m_u4HeadSrcSize     = 0;
	m_u4BodySrcSize     = 0;
	m_u2PacketCommandID = 0;

	m_objPacketHeadInfo.Clear();
}

void CPacketParseBase::Close()
{
	if(m_pmbHead != NULL)
	{
		m_pmbHead->release();
		m_pmbHead = NULL;
	}

	if(m_pmbBody != NULL)
	{
		m_pmbBody->release();
		m_pmbBody = NULL;
	}

	m_blIsHandleHead = true;
}

const char* CPacketParseBase::GetPacketVersion()
{
	return m_szPacketVersion;
}

uint8 CPacketParseBase::GetPacketMode()
{
	return (uint8)m_u1PacketMode;
}

uint32 CPacketParseBase::GetPacketHeadLen()
{
	return m_u4PacketHead;
}

uint32 CPacketParseBase::GetPacketBodyLen()
{
	return m_u4PacketData;
}

uint16 CPacketParseBase::GetPacketCommandID()
{
	return m_u2PacketCommandID;
}

bool CPacketParseBase::GetIsHandleHead()
{
	return m_blIsHandleHead;
}

uint32 CPacketParseBase::GetPacketHeadSrcLen()
{
	return m_u4HeadSrcSize;
}

uint32 CPacketParseBase::GetPacketBodySrcLen()
{
	return m_u4BodySrcSize;
}

ACE_Message_Block* CPacketParseBase::GetMessageHead()
{
	return m_pmbHead;
};

ACE_Message_Block* CPacketParseBase::GetMessageBody()
{
	return m_pmbBody;
};

void CPacketParseBase::SetSort(uint8 u1Sort)
{
	m_u1Sort = u1Sort;
}

void CPacketParseBase::Check_Recv_Unit16(uint16& u2Data)
{
	if(m_u1Sort == 1)
	{
		//网序转换为主机字序
		u2Data = ACE_NTOHS(u2Data);
	}
}

void CPacketParseBase::Check_Recv_Unit32(uint32& u4Data)
{
	if(m_u1Sort == 1)
	{
		//网序转换为主机字序
		u4Data = ACE_NTOHL(u4Data);
	}
}

void CPacketParseBase::Check_Recv_Unit64(uint64& u8Data)
{
	if(m_u1Sort == 1)
	{
		//网序转换为主机字序
		u8Data = ntohl64(u8Data);
	}
}

void CPacketParseBase::Check_Send_Unit16(uint16& u2Data)
{
	if(m_u1Sort == 1)
	{
		//主机序到网序
		u2Data = ACE_HTONS(u2Data);
	}
}

void CPacketParseBase::Check_Send_Unit32(uint32& u4Data)
{
	if(m_u1Sort == 1)
	{
		//主机序到网序
		u4Data = ACE_HTONL(u4Data);
	}
}

void CPacketParseBase::Check_Send_Unit64(uint64& u8Data)
{
	if(m_u1Sort == 1)
	{
		//主机序到网序
		u8Data = hl64ton(u8Data);
	}
}
