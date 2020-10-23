#include "MakePacket.h"

CMakePacket::CMakePacket(void)
{
    m_PerformanceCounter.init("PutThread");
}

bool CMakePacket::Init() const
{
    return true;
}

bool CMakePacket::PutMessageBlock(_MakePacket const& objMakePacket, const ACE_Time_Value& tvNow)
{   
    //根据操作OP，调用相应的方法。
    CWorkThreadMessage* pMessage = App_MessageServiceGroup::instance()->CreateMessage(objMakePacket.m_u4ConnectID, 
        objMakePacket.m_emPacketType);

    if(NULL == pMessage)
    {
        //回收PacketParse中的包头包体内存
        if(NULL != objMakePacket.m_pPacketParse)
        {
            if(NULL != objMakePacket.m_pPacketParse->GetMessageHead())
            {
                App_MessageBlockManager::instance()->Close(objMakePacket.m_pPacketParse->GetMessageHead());
            }

            if(NULL != objMakePacket.m_pPacketParse->GetMessageBody())
            {
                App_MessageBlockManager::instance()->Close(objMakePacket.m_pPacketParse->GetMessageBody());
            }

            objMakePacket.m_pPacketParse->Clear();
        }

        OUR_DEBUG((LM_ERROR,"[CMakePacket::PutMessageBlock] pMessage is NULL.\n"));
        return false;
    }

    SetMessage(objMakePacket, pMessage, tvNow);

    //将要处理的消息放入消息处理线程
    if(false == App_MessageServiceGroup::instance()->PutMessage(pMessage))
    {
        OUR_DEBUG((LM_ERROR, "[CMakePacket::ProcessMessageBlock] App_MessageServiceGroup::instance()->PutMessage Error.\n"));
        App_MessageServiceGroup::instance()->DeleteMessage(pMessage);
        return false;
    }

    /*
    if (PACKET_PARSE == u1Option)
    {
        m_PerformanceCounter.counter();
    }

    if (PACKET_CDISCONNECT == u1Option)
    {
        m_PerformanceCounter.reset();
    }
    */

    return true;
}

void CMakePacket::SetMessage(_MakePacket const& objMakePacket, CWorkThreadMessage* pWorkThreadMessage, const ACE_Time_Value& tvNow) const
{
    if(nullptr != pWorkThreadMessage)
    {
        if(PACKET_PARSE == objMakePacket.m_u1Option)
        {
            pWorkThreadMessage->m_u2Cmd = objMakePacket.m_pPacketParse->GetPacketCommandID();
        }
        else if(objMakePacket.m_u1Option == PACKET_CONNECT)
        {
            pWorkThreadMessage->m_u2Cmd          = CLIENT_LINK_CONNECT;
            pWorkThreadMessage->m_pHandler       = objMakePacket.m_pHandler;
            pWorkThreadMessage->m_AddrListen     = objMakePacket.m_AddrListen;
            pWorkThreadMessage->m_AddrListen     = objMakePacket.m_AddrListen;
        }
        else if(objMakePacket.m_u1Option == PACKET_CDISCONNECT)
        {
            pWorkThreadMessage->m_u2Cmd         = CLIENT_LINK_CDISCONNET;
        }
        else if(objMakePacket.m_u1Option == PACKET_SDISCONNECT)
        {
            pWorkThreadMessage->m_u2Cmd         = CLIENT_LINK_SDISCONNET;
        }
        else if(objMakePacket.m_u1Option == PACKET_SEND_TIMEOUT)
        {
            pWorkThreadMessage->m_u2Cmd         = CLINET_LINK_SENDTIMEOUT;
        }
        else if(objMakePacket.m_u1Option == PACKET_CHEK_TIMEOUT)
        {
            pWorkThreadMessage->m_u2Cmd         = CLINET_LINK_CHECKTIMEOUT;
        }
        else if(objMakePacket.m_u1Option == PACKET_SEND_OK)
        {
            pWorkThreadMessage->m_u2Cmd         = CLIENT_LINK_SENDOK;
        }
        else if (objMakePacket.m_u1Option == PACKET_TTY_CONNECT)
        {
            pWorkThreadMessage->m_u2Cmd         = CLINET_LINK_TTY_CONNECT;
        }
        else if (objMakePacket.m_u1Option == PACKET_TTY_DISCONNECT)
        {
            pWorkThreadMessage->m_u2Cmd         = CLINET_LINK_TTY_DISCONNECT;
        }
        else if (objMakePacket.m_u1Option == PACKET_SERVER_TCP_CONNECT)
        {
            pWorkThreadMessage->m_u2Cmd         = CLINET_LINK_ST_CONNECT;
        }
        else if (objMakePacket.m_u1Option == PACKET_SERVER_TCP_DISCONNECT)
        {
            pWorkThreadMessage->m_u2Cmd         = CLINET_LINK_ST_DISCONNECT;
        }
        else if (objMakePacket.m_u1Option == PACKET_SERVER_UDP_CONNECT)
        {
            pWorkThreadMessage->m_u2Cmd         = CLINET_LINK_SU_DISCONNECT;
        }
        else if (objMakePacket.m_u1Option == PACKET_SERVER_UDP_DISCONNECT)
        {
            pWorkThreadMessage->m_u2Cmd         = CLINET_LINK_SU_DISCONNECT;
        }

        if (NULL != objMakePacket.m_pPacketParse)
        {
            pWorkThreadMessage->m_pmbRecvHead = objMakePacket.m_pPacketParse->GetMessageHead();
            pWorkThreadMessage->m_pmbRecvBody = objMakePacket.m_pPacketParse->GetMessageBody();
        }
        else
        {
            pWorkThreadMessage->m_pmbRecvHead = nullptr;
            pWorkThreadMessage->m_pmbRecvBody = nullptr;
        }

        pWorkThreadMessage->m_u4ConnectID     = objMakePacket.m_u4ConnectID;
        pWorkThreadMessage->m_u4PacketParseID = objMakePacket.m_u4PacketParseID;
        pWorkThreadMessage->m_emPacketType    = objMakePacket.m_emPacketType;
        pWorkThreadMessage->m_tvMessage       = tvNow;
        pWorkThreadMessage->m_emDirect        = EM_WORKTHREAD_DIRECT::EM_WORKTHREAD_DIRECT_INPUT;
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", objMakePacket.m_u4ConnectID));
    }
}

void CMakePacket::SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, CWorkThreadMessage* pWorkThreadMessage, const ACE_Time_Value& tvNow) const
{
    //开始组装数据
    pWorkThreadMessage->m_u4ConnectID   = u4ConnectID;
    pWorkThreadMessage->m_u2Cmd         = (uint16)CLINET_LINK_SENDERROR;
    pWorkThreadMessage->m_tvMessage     = tvNow;
    pWorkThreadMessage->m_pmbRecvBody   = pBodyMessage;
    pWorkThreadMessage->m_emDirect      = EM_WORKTHREAD_DIRECT::EM_WORKTHREAD_DIRECT_INPUT;
}

bool CMakePacket::PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, const ACE_Time_Value& tvNow) const
{
    CWorkThreadMessage* pMessage = App_MessageServiceGroup::instance()->CreateMessage(u4ConnectID, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

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
        App_MessageServiceGroup::instance()->DeleteMessage(pMessage);
        return false;
    }

    return true;
}

