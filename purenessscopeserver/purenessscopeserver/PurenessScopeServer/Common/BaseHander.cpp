#include "BaseHander.h"

void Recovery_Message(bool blDelete, char*& pMessage)
{
    if (true == blDelete)
    {
        SAFE_DELETE_ARRAY(pMessage);
    }
}

bool Udp_Common_Send_Message(uint32 u4PacketParseInfoID, ACE_INET_Addr& AddrRemote, char*& pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead, uint16 u2CommandID, bool blDlete, ACE_Message_Block*& pMbData)
{
    int nErr = AddrRemote.set(nPort, szIP);

    if (nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[Udp_Common_Send_Message]set_address error[%d].\n", errno));
        Recovery_Message(blDlete, pMessage);
        return false;
    }

    if (blHead == true)
    {
        //如果需要拼接包头，则拼接包头
        CPacketParse PacketParse;

        uint32 u4SendLength = App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Make_Send_Packet_Length(0, u4Len, u2CommandID);
        pMbData = App_MessageBlockManager::instance()->Create(u4SendLength);

        if (NULL == pMbData)
        {
            OUR_DEBUG((LM_INFO, "Udp_Common_Send_Message]pMbData is NULL.\n"));
            Recovery_Message(blDlete, pMessage);
            return false;
        }

        if (false == App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Make_Send_Packet(0, pMessage, u4Len, pMbData, u2CommandID))
        {
            OUR_DEBUG((LM_INFO, "Udp_Common_Send_Message]Make_Send_Packet is false.\n"));
            Recovery_Message(blDlete, pMessage);
            return false;
        }

        Recovery_Message(blDlete, pMessage);
        return true;
    }
    else
    {
        //不需要拼接包头，直接发送
        pMbData = App_MessageBlockManager::instance()->Create(u4Len);

        if (NULL == pMbData)
        {
            OUR_DEBUG((LM_INFO, "Udp_Common_Send_Message]pMbData is NULL.\n"));
            Recovery_Message(blDlete, pMessage);
            return false;
        }

        //将数据拷贝到ACE_Message_Block对象中
        memcpy_safe(pMessage, u4Len, pMbData->wr_ptr(), u4Len);
        pMbData->wr_ptr(u4Len);

        Recovery_Message(blDlete, pMessage);
        return true;
    }
}

bool Udp_Common_Recv_Head(ACE_Message_Block* pMBHead, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID, uint32 u4Len)
{
    _Head_Info obj_Head_Info;
    bool blStateHead = App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Parse_Packet_Head_Info(0, pMBHead, App_MessageBlockManager::instance(), &obj_Head_Info);

    if (false == blStateHead)
    {
        return false;
    }
    else
    {
        pPacketParse->SetPacket_IsHandleHead(false);
        pPacketParse->SetPacket_Head_Curr_Length(obj_Head_Info.m_u4HeadCurrLen);
        pPacketParse->SetPacket_Body_Src_Length(obj_Head_Info.m_u4BodySrcLen);
        pPacketParse->SetPacket_CommandID(obj_Head_Info.m_u2PacketCommandID);
        pPacketParse->SetPacket_Head_Message(pMBHead);
    }

    if (u4Len != pPacketParse->GetPacketHeadSrcLen() + pPacketParse->GetPacketBodySrcLen())
    {
        return false;
    }

    return true;
}

bool Udp_Common_Recv_Body(ACE_Message_Block* pMBBody, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID)
{
    _Body_Info obj_Body_Info;
    bool blStateBody = App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Parse_Packet_Body_Info(0, pMBBody, App_MessageBlockManager::instance(), &obj_Body_Info);

    if (false == blStateBody)
    {
        return false;
    }
    else
    {
        pPacketParse->SetPacket_Body_Message(pMBBody);
        pPacketParse->SetPacket_Body_Curr_Length(obj_Body_Info.m_u4BodyCurrLen);
    }

    return true;
}

bool Udp_Common_Recv_Stream(ACE_Message_Block* pMbData, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID)
{
    //以数据流处理
    _Packet_Info obj_Packet_Info;

    if (PACKET_GET_ENOUGTH == App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Parse_Packet_Stream(0, pMbData, App_MessageBlockManager::instance(), &obj_Packet_Info))
    {
        pPacketParse->SetPacket_Head_Message(obj_Packet_Info.m_pmbHead);
        pPacketParse->SetPacket_Body_Message(obj_Packet_Info.m_pmbBody);
        pPacketParse->SetPacket_CommandID(obj_Packet_Info.m_u2PacketCommandID);
        pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4HeadSrcLen);
        pPacketParse->SetPacket_Head_Curr_Length(obj_Packet_Info.m_u4HeadCurrLen);
        pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4BodySrcLen);
        pPacketParse->SetPacket_Body_Curr_Length(obj_Packet_Info.m_u4BodyCurrLen);
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[Udp_Common_Recv_Stream]m_pPacketParse GetPacketStream is error.\n"));
        App_PacketParsePool::instance()->Delete(pPacketParse);
        return false;
    }

    return true;
}

bool Udp_Common_Send_WorkThread(CPacketParse*& pPacketParse, ACE_INET_Addr addrRemote, ACE_INET_Addr addrLocal, ACE_Time_Value& tvCheck)
{
    //组织数据包
    _MakePacket objMakePacket;
    objMakePacket.m_u4ConnectID = UDP_HANDER_ID;
    objMakePacket.m_pPacketParse = pPacketParse;
    objMakePacket.m_PacketType = PACKET_UDP;
    objMakePacket.m_AddrRemote = addrRemote;
    objMakePacket.m_AddrListen = addrLocal;
    objMakePacket.m_u1Option = PACKET_PARSE;

    //UDP因为不是面向链接的
    if (false == App_MakePacket::instance()->PutMessageBlock(&objMakePacket, tvCheck))
    {
        OUR_DEBUG((LM_ERROR, "[Udp_Common_Send_WorkThread]PutMessageBlock is error.\n"));
        App_PacketParsePool::instance()->Delete(pPacketParse);
        return false;
    }

    return true;
}
