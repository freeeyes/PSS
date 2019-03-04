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

bool CMakePacket::PutMessageBlock(_MakePacket* pMakePacket, ACE_Time_Value& tvNow)
{
    if(NULL == pMakePacket)
    {
        OUR_DEBUG((LM_ERROR,"[CMakePacket::PutMessageBlock] pMakePacket is NULL.\n"));
        return false;
    }

    //根据操作OP，调用相应的方法。
    CMessage* pMessage = App_MessageServiceGroup::instance()->CreateMessage(pMakePacket->m_u4ConnectID, pMakePacket->m_PacketType);

    if(NULL == pMessage)
    {
        //回收PacketParse中的包头包体内存
        if(NULL != pMakePacket->m_pPacketParse)
        {
            if(NULL != pMakePacket->m_pPacketParse->GetMessageHead())
            {
                App_MessageBlockManager::instance()->Close(pMakePacket->m_pPacketParse->GetMessageHead());
            }

            if(NULL != pMakePacket->m_pPacketParse->GetMessageBody())
            {
                App_MessageBlockManager::instance()->Close(pMakePacket->m_pPacketParse->GetMessageBody());
            }

            pMakePacket->m_pPacketParse->Clear();
        }

        OUR_DEBUG((LM_ERROR,"[CMakePacket::PutMessageBlock] pMessage is NULL.\n"));
        return false;
    }

    SetMessage(pMakePacket, pMessage, tvNow);

    //将要处理的消息放入消息处理线程
    if(false == App_MessageServiceGroup::instance()->PutMessage(pMessage))
    {
        OUR_DEBUG((LM_ERROR, "[CMakePacket::ProcessMessageBlock] App_MessageServiceGroup::instance()->PutMessage Error.\n"));
        App_MessageServiceGroup::instance()->DeleteMessage(pMakePacket->m_u4ConnectID, pMessage);
        return false;
    }

    return true;
}

void CMakePacket::SetMessage(_MakePacket* pMakePacket, CMessage* pMessage, ACE_Time_Value& tvNow)
{
    if(NULL != pMessage->GetMessageBase())
    {
        if(PACKET_PARSE == pMakePacket->m_u1Option)
        {
            if (NULL != pMakePacket->m_pPacketParse)
            {
                pMessage->GetMessageBase()->m_u2Cmd = pMakePacket->m_pPacketParse->GetPacketCommandID();
            }
        }
        else if(pMakePacket->m_u1Option == PACKET_CONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd  = CLIENT_LINK_CONNECT;
        }
        else if(pMakePacket->m_u1Option == PACKET_CDISCONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_CDISCONNET;
        }
        else if(pMakePacket->m_u1Option == PACKET_SDISCONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_SDISCONNET;
        }
        else if(pMakePacket->m_u1Option == PACKET_SEND_TIMEOUT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_SENDTIMEOUT;
        }
        else if(pMakePacket->m_u1Option == PACKET_CHEK_TIMEOUT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_CHECKTIMEOUT;
        }
        else if(pMakePacket->m_u1Option == PACKET_SEND_OK)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_SENDOK;
        }

        pMessage->GetMessageBase()->m_u4ConnectID     = pMakePacket->m_u4ConnectID;
        pMessage->GetMessageBase()->m_tvRecvTime      = tvNow;
        pMessage->GetMessageBase()->m_u4PacketParseID = pMakePacket->m_u4PacketParseID;

        if(NULL != pMakePacket->m_pPacketParse)
        {
            pMessage->GetMessageBase()->m_u4HeadSrcSize = pMakePacket->m_pPacketParse->GetPacketHeadSrcLen();
            pMessage->GetMessageBase()->m_u4BodySrcSize = pMakePacket->m_pPacketParse->GetPacketBodySrcLen();
            pMessage->SetPacketHead(pMakePacket->m_pPacketParse->GetMessageHead());
            pMessage->SetPacketBody(pMakePacket->m_pPacketParse->GetMessageBody());
        }
        else
        {
            pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
            pMessage->GetMessageBase()->m_u4BodySrcSize = 0;
            pMessage->SetPacketHead(NULL);
            pMessage->SetPacketBody(NULL);
        }

        pMessage->GetMessageBase()->m_u1PacketType  = pMakePacket->m_PacketType;

        sprintf_safe(pMessage->GetMessageBase()->m_szIP, MAX_BUFF_20, "%s", pMakePacket->m_AddrRemote.get_host_addr());
        pMessage->GetMessageBase()->m_u4Port = (uint32)pMakePacket->m_AddrRemote.get_port_number();
        sprintf_safe(pMessage->GetMessageBase()->m_szListenIP, MAX_BUFF_20, "%s", pMakePacket->m_AddrListen.get_host_addr());
        pMessage->GetMessageBase()->m_u4ListenPort = (uint32)pMakePacket->m_AddrListen.get_port_number();
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", pMakePacket->m_u4ConnectID));
    }
}

void CMakePacket::SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, CMessage* pMessage, ACE_Time_Value& tvNow)
{
    if(NULL != pMessage->GetMessageBase())
    {
        //开始组装数据
        pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
        pMessage->GetMessageBase()->m_u2Cmd         = (uint16)CLINET_LINK_SENDERROR;
        pMessage->GetMessageBase()->m_tvRecvTime    = tvNow;
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

    //将要处理的消息放入消息处理线程
    if(false == App_MessageServiceGroup::instance()->PutMessage(pMessage))
    {
        OUR_DEBUG((LM_ERROR, "[CMakePacket::PutSendErrorMessage] App_MessageServiceGroup::instance()->PutMessage Error.\n"));
        App_MessageBlockManager::instance()->Close(pBodyMessage);
        App_MessageServiceGroup::instance()->DeleteMessage(u4ConnectID, pMessage);
        return false;
    }

    return true;
}

