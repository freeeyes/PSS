#include "PacketConsoleParse.h"


CConsolePacketParse::CConsolePacketParse(void)
{
    //如果是包头模式，这里需要设置包头的长度
    SetPacket_Head_Src_Length(PACKET_CONSOLE_HEAD);
    SetPacket_IsHandleHead(true);
}

void CConsolePacketParse::Init()
{
    SetPacket_IsHandleHead(true);

    SetPacket_Head_Message(nullptr);
    SetPacket_Body_Message(nullptr);
}

bool CConsolePacketParse::SetPacketHead(uint32 u4ConnectID, ACE_Message_Block* pmbHead, const IMessageBlockManager* pMessageBlockManager)
{
    ACE_UNUSED_ARG(u4ConnectID);
    ACE_UNUSED_ARG(pMessageBlockManager);

    //这里添加自己对包头的分析，主要分析出包长度。
    auto u4Len = (uint32)pmbHead->length();

    SetPacket_Head_Curr_Length(u4Len);

    if(u4Len == sizeof(uint32))
    {
        uint32 u4PacketBody = 0;
        memcpy_safe(pmbHead->rd_ptr(), (uint32)sizeof(uint32), (char* )&u4PacketBody, (uint32)sizeof(uint32));

        SetPacket_Head_Message(pmbHead);
        SetPacket_Body_Src_Length(u4PacketBody);
        SetPacket_Body_Curr_Length(u4PacketBody);
        SetPacket_IsHandleHead(false);
        return true;
    }
    else
    {
        return false;
    }
}

bool CConsolePacketParse::SetPacketBody(uint32 u4ConnectID, ACE_Message_Block* pmbBody, const IMessageBlockManager* pMessageBlockManager)
{
    //这里分析出包体内的一些数据，如果包头包含了CommandID，那么包体就不必做解析。
    if(u4ConnectID == 0 && pMessageBlockManager != nullptr)
    {
        //UDP数据包，没有u4ConnectID
    }

    auto u4Len = (uint32)pmbBody->length();

    SetPacket_Body_Src_Length(u4Len);
    SetPacket_Body_Curr_Length(u4Len);

    if(u4Len >= sizeof(uint16))
    {
        SetPacket_Body_Message(pmbBody);
        SetPacket_IsHandleHead(true);
        return true;
    }
    else
    {
        SetPacket_IsHandleHead(true);
        return false;
    }
}


uint32 CConsolePacketParse::MakePacketLength(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID) const
{
    ACE_UNUSED_ARG(u4ConnectID);
    ACE_UNUSED_ARG(u2CommandID);

    return u4DataLen + sizeof(uint32);
}

bool CConsolePacketParse::MakePacket(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID) const
{
    ACE_UNUSED_ARG(u4ConnectID);
    ACE_UNUSED_ARG(u2CommandID);

    if(pMbData == nullptr)
    {
        return false;
    }

    //拼装数据包
    memcpy_safe((const char* )&u4Len, (uint32)sizeof(uint32), pMbData->wr_ptr(), (uint32)sizeof(uint32));
    pMbData->wr_ptr(sizeof(uint32));
    memcpy_safe(pData, u4Len, pMbData->wr_ptr(), u4Len);
    pMbData->wr_ptr(u4Len);

    return true;
}



uint8 CConsolePacketParse::GetPacketStream(uint32 u4ConnectID, const ACE_Message_Block* pCurrMessage, const IMessageBlockManager* pMessageBlockManager) const
{
    ACE_UNUSED_ARG(pMessageBlockManager);
    ACE_UNUSED_ARG(pCurrMessage);
    ACE_UNUSED_ARG(u4ConnectID);

    return PACKET_GET_ENOUGH;
}


bool CConsolePacketParse::Connect( uint32 u4ConnectID, _ClientIPInfo const& objClientIPInfo, _ClientIPInfo const& objLocalIPInfo) const
{
    //这里添加你对连接建立的逻辑处理，如果没有则不用在这里写任何代码
    //返回false，则连接会断开
    if(u4ConnectID == 0)
    {
        //UDP数据包，没有u4ConnectID
    }

    PSS_LOGGER_DEBUG("[CConsolePacketParse::Connect]IP={0},Port={1}.",
        objClientIPInfo.m_strClientIP, 
        objClientIPInfo.m_u2Port);
    PSS_LOGGER_DEBUG("[CConsolePacketParse::Connect]LocalIP={0},Port={1}.",
        objLocalIPInfo.m_strClientIP, 
        objLocalIPInfo.m_u2Port);
    return true;
}

void CConsolePacketParse::DisConnect( uint32 u4ConnectID ) const
{
    //这里添加你对连接断开的逻辑处理
    if(u4ConnectID == 0)
    {
        //UDP数据包，没有u4ConnectID
    }
}


