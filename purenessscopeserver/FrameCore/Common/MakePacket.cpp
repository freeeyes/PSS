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
    uint8 u1Option = objMakePacket.m_u1Option;

    
    //根据操作OP，调用相应的方法。
    CMessage* pMessage = App_MessageServiceGroup::instance()->CreateMessage(objMakePacket.m_u4ConnectID, objMakePacket.m_u1PacketType);

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
        App_MessageServiceGroup::instance()->DeleteMessage(objMakePacket.m_u4ConnectID, pMessage);
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

void CMakePacket::SetMessage(_MakePacket const& objMakePacket, CMessage* pMessage, const ACE_Time_Value& tvNow) const
{
    if(NULL != pMessage->GetMessageBase())
    {
        if(PACKET_PARSE == objMakePacket.m_u1Option)
        {
            if (NULL != objMakePacket.m_pPacketParse)
            {
                pMessage->GetMessageBase()->m_u2Cmd = objMakePacket.m_pPacketParse->GetPacketCommandID();
            }
        }
        else if(objMakePacket.m_u1Option == PACKET_CONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd  = CLIENT_LINK_CONNECT;
        }
        else if(objMakePacket.m_u1Option == PACKET_CDISCONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_CDISCONNET;
        }
        else if(objMakePacket.m_u1Option == PACKET_SDISCONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_SDISCONNET;
        }
        else if(objMakePacket.m_u1Option == PACKET_SEND_TIMEOUT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_SENDTIMEOUT;
        }
        else if(objMakePacket.m_u1Option == PACKET_CHEK_TIMEOUT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_CHECKTIMEOUT;
        }
        else if(objMakePacket.m_u1Option == PACKET_SEND_OK)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_SENDOK;
        }
        else if (objMakePacket.m_u1Option == PACKET_TTY_CONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_TTY_CONNECT;
        }
        else if (objMakePacket.m_u1Option == PACKET_TTY_DISCONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_TTY_DISCONNECT;
        }
        else if (objMakePacket.m_u1Option == PACKET_SERVER_TCP_CONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_ST_CONNECT;
        }
        else if (objMakePacket.m_u1Option == PACKET_SERVER_TCP_DISCONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_ST_DISCONNECT;
        }
        else if (objMakePacket.m_u1Option == PACKET_SERVER_UDP_CONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_SU_DISCONNECT;
        }
        else if (objMakePacket.m_u1Option == PACKET_SERVER_UDP_DISCONNECT)
        {
            pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_SU_DISCONNECT;
        }

        pMessage->GetMessageBase()->m_u4ConnectID     = objMakePacket.m_u4ConnectID;
        pMessage->GetMessageBase()->m_tvRecvTime      = tvNow;
        pMessage->GetMessageBase()->m_u4PacketParseID = objMakePacket.m_u4PacketParseID;

        if(NULL != objMakePacket.m_pPacketParse)
        {
            pMessage->GetMessageBase()->m_u4HeadSrcSize = objMakePacket.m_pPacketParse->GetPacketHeadSrcLen();
            pMessage->GetMessageBase()->m_u4BodySrcSize = objMakePacket.m_pPacketParse->GetPacketBodySrcLen();
            pMessage->SetPacketHead(objMakePacket.m_pPacketParse->GetMessageHead());
            pMessage->SetPacketBody(objMakePacket.m_pPacketParse->GetMessageBody());
        }
        else
        {
            pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
            pMessage->GetMessageBase()->m_u4BodySrcSize = 0;
            pMessage->SetPacketHead(NULL);
            pMessage->SetPacketBody(NULL);
        }

        pMessage->GetMessageBase()->m_u1PacketType  = objMakePacket.m_u1PacketType;

        sprintf_safe(pMessage->GetMessageBase()->m_szIP, MAX_BUFF_20, "%s", objMakePacket.m_AddrRemote.get_host_addr());
        pMessage->GetMessageBase()->m_u4Port = (uint32)objMakePacket.m_AddrRemote.get_port_number();
        sprintf_safe(pMessage->GetMessageBase()->m_szListenIP, MAX_BUFF_20, "%s", objMakePacket.m_AddrListen.get_host_addr());
        pMessage->GetMessageBase()->m_u4ListenPort = (uint32)objMakePacket.m_AddrListen.get_port_number();
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", objMakePacket.m_u4ConnectID));
    }
}

void CMakePacket::SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, CMessage* pMessage, const ACE_Time_Value& tvNow) const
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

bool CMakePacket::PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, const ACE_Time_Value& tvNow) const
{
    CMessage* pMessage = App_MessageServiceGroup::instance()->CreateMessage(u4ConnectID, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

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

