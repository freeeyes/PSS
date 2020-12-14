#include "ModuleMessageManager.h"

int CModuleMessageManager::SendModuleMessage(const char* pModuleName, uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket)
{
    return App_ModuleLoader::instance()->SendModuleMessage(pModuleName, u2CommandID, pBuffPacket, pReturnBuffPacket);
}

int CModuleMessageManager::SendFrameMessage(uint16 u2CommandID, uint32 u4WorkThreadID, shared_ptr<IBuffPacket> pHeadPacket, shared_ptr<IBuffPacket> pBodyBuffPacket)
{
    //向框架重发送消息，通过注册的插件进行消费
    auto tvNow = CTimeStamp::Get_Time_Stamp();
    _MakePacket objMakePacket;
    auto pPacketParse = std::make_shared<CPacketParse>();

    pPacketParse->SetPacket_CommandID(u2CommandID);

    if(nullptr != pHeadPacket)
    {
        ACE_Message_Block* pMb = App_MessageBlockManager::instance()->Create(pHeadPacket->GetPacketLen());
        memcpy_safe(pHeadPacket->GetData(), pHeadPacket->GetPacketLen(), pMb->wr_ptr(), pHeadPacket->GetPacketLen());
        pMb->wr_ptr(pHeadPacket->GetPacketLen());
        pPacketParse->SetPacket_Head_Curr_Length(pHeadPacket->GetPacketLen());
        pPacketParse->SetPacket_Head_Src_Length(pHeadPacket->GetPacketLen());
        pPacketParse->SetPacket_Head_Message(pMb);
    }

    if(nullptr != pBodyBuffPacket)
    {
        ACE_Message_Block* pMb = App_MessageBlockManager::instance()->Create(pBodyBuffPacket->GetPacketLen());
        memcpy_safe(pBodyBuffPacket->GetData(), pBodyBuffPacket->GetPacketLen(), pMb->wr_ptr(), pBodyBuffPacket->GetPacketLen());
        pMb->wr_ptr(pBodyBuffPacket->GetPacketLen());
        pPacketParse->SetPacket_Body_Curr_Length(pBodyBuffPacket->GetPacketLen());
        pPacketParse->SetPacket_Body_Src_Length(pBodyBuffPacket->GetPacketLen());
        pPacketParse->SetPacket_Body_Message(pMb);
    }

    objMakePacket.m_emPacketType = EM_CONNECT_IO_TYPE::CONNECT_IO_FRAME;
    objMakePacket.m_u4ConnectID  = u4WorkThreadID;
    objMakePacket.m_pPacketParse = pPacketParse;
    objMakePacket.m_u1Option     = PACKET_PARSE;
    objMakePacket.m_tvRecv       = tvNow;

    Send_MakePacket_Queue(m_MakePacket, objMakePacket);
    return 0;
}
