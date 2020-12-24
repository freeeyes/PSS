#include "MakePacket.h"

CMakePacket::CMakePacket(void)
{
    m_PerformanceCounter.init("PutThread");
}

bool CMakePacket::Init() const
{
    return true;
}

bool CMakePacket::PutMessageBlock(_MakePacket const& objMakePacket, const PSS_Time_Point& tvNow)
{   
    //根据操作OP，调用相应的方法。
    auto pMessage = App_MessageServiceGroup::instance()->CreateMessage(objMakePacket.m_u4ConnectID, 
        objMakePacket.m_emPacketType);

    if(nullptr == pMessage)
    {
        //回收PacketParse中的包头包体内存
        if(nullptr != objMakePacket.m_pPacketParse)
        {
            if(nullptr != objMakePacket.m_pPacketParse->GetMessageHead())
            {
                App_MessageBlockManager::instance()->Close(objMakePacket.m_pPacketParse->GetMessageHead());
            }

            if(nullptr != objMakePacket.m_pPacketParse->GetMessageBody())
            {
                App_MessageBlockManager::instance()->Close(objMakePacket.m_pPacketParse->GetMessageBody());
            }

            objMakePacket.m_pPacketParse->Clear();
        }

        PSS_LOGGER_DEBUG("[CMakePacket::PutMessageBlock] pMessage is nullptr.");
        return false;
    }

    SetMessage(objMakePacket, pMessage, tvNow);

    //将消息写入消息集合
    if (nullptr == m_pMessageList)
    {
        m_pMessageList = std::make_shared<CWorkThreadMessageList>();
        m_pMessageList->m_u4WorkThreadID = pMessage->m_u4WorkThreadID;
    }

    m_pMessageList->m_vecList.emplace_back(pMessage);

    return true;
}

void CMakePacket::SetMessage(_MakePacket const& objMakePacket, shared_ptr<CWorkThreadMessage> pWorkThreadMessage, const PSS_Time_Point& tvNow) const
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
            pWorkThreadMessage->m_AddrRemote     = objMakePacket.m_AddrRemote;
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

        if (nullptr != objMakePacket.m_pPacketParse)
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
        PSS_LOGGER_DEBUG("[CMakePacket::SetMessage] ConnectID = {0}, pMessage->GetMessageBase() is nullptr.", objMakePacket.m_u4ConnectID);
    }
}

void CMakePacket::SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, shared_ptr<CWorkThreadMessage> pWorkThreadMessage, const PSS_Time_Point& tvNow) const
{
    //开始组装数据
    pWorkThreadMessage->m_u4ConnectID   = u4ConnectID;
    pWorkThreadMessage->m_u2Cmd         = CLINET_LINK_SENDERROR;
    pWorkThreadMessage->m_tvMessage     = tvNow;
    pWorkThreadMessage->m_pmbRecvBody   = pBodyMessage;
    pWorkThreadMessage->m_emDirect      = EM_WORKTHREAD_DIRECT::EM_WORKTHREAD_DIRECT_INPUT;
}

bool CMakePacket::PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, const PSS_Time_Point& tvNow)
{
    auto pMessage = App_MessageServiceGroup::instance()->CreateMessage(u4ConnectID, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

    if(nullptr == pMessage)
    {
        PSS_LOGGER_DEBUG("[CMakePacket::PutSendErrorMessage] pMessage is nullptr.");
        App_MessageBlockManager::instance()->Close(pBodyMessage);
        return false;
    }

    SetMessageSendError(u4ConnectID, pBodyMessage, pMessage, tvNow);

    //将要处理的消息放入消息处理线程
    //将消息写入消息集合
    if (nullptr == m_pMessageList)
    {
        m_pMessageList = std::make_shared<CWorkThreadMessageList>();
        m_pMessageList->m_u4WorkThreadID = pMessage->m_u4WorkThreadID;
    }

    m_pMessageList->m_vecList.emplace_back(pMessage);
    return true;
}

void CMakePacket::CommitMessageList()
{
    //将要处理的消息放入消息处理线程
    if (nullptr != m_pMessageList)
    {
        App_MessageServiceGroup::instance()->PutMessage(m_pMessageList);
        m_pMessageList = nullptr;
    }
}

