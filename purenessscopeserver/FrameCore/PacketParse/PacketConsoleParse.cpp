#include "PacketConsoleParse.h"


CConsolePacketParse::CConsolePacketParse(void)
{
    //����ǰ�ͷģʽ��������Ҫ���ð�ͷ�ĳ���
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

    //��������Լ��԰�ͷ�ķ�������Ҫ�����������ȡ�
    uint32 u4Len = (uint32)pmbHead->length();

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
    //��������������ڵ�һЩ���ݣ������ͷ������CommandID����ô����Ͳ�����������
    if(u4ConnectID == 0 && pMessageBlockManager != nullptr)
    {
        //UDP���ݰ���û��u4ConnectID
    }

    uint32 u4Len = (uint32)pmbBody->length();

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

    //ƴװ���ݰ�
    memcpy_safe((const char* )&u4Len, (uint32)sizeof(uint32), pMbData->wr_ptr(), (uint32)sizeof(uint32));
    pMbData->wr_ptr(sizeof(uint32));
    memcpy_safe(pData, u4Len, pMbData->wr_ptr(), u4Len);
    pMbData->wr_ptr(u4Len);

    return true;
}



uint8 CConsolePacketParse::GetPacketStream(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, const IMessageBlockManager* pMessageBlockManager) const
{
    ACE_UNUSED_ARG(pMessageBlockManager);
    ACE_UNUSED_ARG(pCurrMessage);
    ACE_UNUSED_ARG(u4ConnectID);

    return PACKET_GET_ENOUGH;
}


bool CConsolePacketParse::Connect( uint32 u4ConnectID, _ClientIPInfo const& objClientIPInfo, _ClientIPInfo const& objLocalIPInfo) const
{
    //�������������ӽ������߼��������û������������д�κδ���
    //����false�������ӻ�Ͽ�
    if(u4ConnectID == 0)
    {
        //UDP���ݰ���û��u4ConnectID
    }

    OUR_DEBUG((LM_INFO, "[CConsolePacketParse::Connect]IP=%s,Port=%d.\n", 
        objClientIPInfo.m_strClientIP.c_str(), 
        objClientIPInfo.m_u2Port));
    OUR_DEBUG((LM_INFO, "[CConsolePacketParse::Connect]LocalIP=%s,Port=%d.\n", 
        objLocalIPInfo.m_strClientIP.c_str(), 
        objLocalIPInfo.m_u2Port));
    return true;
}

void CConsolePacketParse::DisConnect( uint32 u4ConnectID ) const
{
    //�������������ӶϿ����߼�����
    if(u4ConnectID == 0)
    {
        //UDP���ݰ���û��u4ConnectID
    }
}


