#include "PacketParseBase.h"

CPacketParseBase::CPacketParseBase(void)
{
    //在这里设置包头的长度，因为大部分的包头长度是固定的。包体长度是可变的。
    m_u4PacketHead      = 0;
    m_u2PacketCommandID = 0;
    m_u4PacketBody      = 0;
    m_u4HeadSrcSize     = 0;
    m_u4BodySrcSize     = 0;
    m_u1Sort            = 0;

    //这里修改属于你的包解析版本号
    sprintf_safe(m_szPacketVersion, MAX_BUFF_20, "0.99");

    m_blIsHandleHead    = true;

    m_pmbHead           = NULL;
    m_pmbBody           = NULL;
    m_pPacketHeadInfo   = NULL;
}

CPacketParseBase::~CPacketParseBase(void)
{
    SAFE_DELETE(m_pPacketHeadInfo);
}

void CPacketParseBase::Clear()
{
    m_pmbHead = NULL;
    m_pmbBody = NULL;

    m_blIsHandleHead    = true;

    m_u4PacketHead      = 0;
    m_u4PacketBody      = 0;
    m_u4BodySrcSize     = 0;
    m_u2PacketCommandID = 0;
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

uint32 CPacketParseBase::GetPacketHeadLen()
{
    return m_u4PacketHead;
}

uint32 CPacketParseBase::GetPacketBodyLen()
{
    return m_u4PacketBody;
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
}

ACE_Message_Block* CPacketParseBase::GetMessageBody()
{
    return m_pmbBody;
}

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

IPacketHeadInfo* CPacketParseBase::GetPacketHeadInfo()
{
    return m_pPacketHeadInfo;
}

void CPacketParseBase::SetPacketHeadInfo(IPacketHeadInfo* pPacketHeadInfo)
{
    m_pPacketHeadInfo = pPacketHeadInfo;
}

void CPacketParseBase::SetPacket_Head_Curr_Length(uint32 u4CurrLength)
{
    m_u4PacketHead = u4CurrLength;
}

void CPacketParseBase::SetPacket_Body_Curr_Length(uint32 u4CurrLength)
{
    m_u4PacketBody = u4CurrLength;
}

void CPacketParseBase::SetPacket_Head_Src_Length(uint32 u4SrcLength)
{
    m_u4HeadSrcSize = u4SrcLength;
}

void CPacketParseBase::SetPacket_Body_Src_Length(uint32 u4SrcLength)
{
    m_u4BodySrcSize = u4SrcLength;
}

void CPacketParseBase::SetPacket_CommandID(uint16 u2PacketCommandID)
{
    m_u2PacketCommandID = u2PacketCommandID;
}

void CPacketParseBase::SetPacket_IsHandleHead(bool blState)
{
    m_blIsHandleHead = blState;
}

void CPacketParseBase::SetPacket_Head_Message(ACE_Message_Block* pmbHead)
{
    m_pmbHead = pmbHead;
}

void CPacketParseBase::SetPacket_Body_Message(ACE_Message_Block* pmbHead)
{
    m_pmbBody = pmbHead;
}
