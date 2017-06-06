#include "ModuleMessageManager.h"

CModuleMessageManager::CModuleMessageManager()
{

}

CModuleMessageManager::~CModuleMessageManager()
{

}

int CModuleMessageManager::SendModuleMessage(const char* pModuleName, uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket)
{
    return App_ModuleLoader::instance()->SendModuleMessage(pModuleName, u2CommandID, pBuffPacket, pReturnBuffPacket);
}

int CModuleMessageManager::SendFrameMessage(uint16 u2CommandID, uint32 u4ConnectID, IBuffPacket* pHeadPacket, IBuffPacket* pBodyBuffPacket)
{
    //向框架重发送消息，通过注册的插件进行消费
    ACE_Time_Value tvNow = ACE_OS::gettimeofday();
    _MakePacket objMakePacket;
    CPacketParse objPacketParse;

    objPacketParse.SetPacket_CommandID(u2CommandID);

    if(NULL != pHeadPacket)
    {
        ACE_Message_Block* pMb = App_MessageBlockManager::instance()->Create(pHeadPacket->GetPacketLen());
        memcpy_safe((char* )pHeadPacket->GetData(), pHeadPacket->GetPacketLen(), pMb->wr_ptr(), pHeadPacket->GetPacketLen());
        pMb->wr_ptr(pHeadPacket->GetPacketLen());
        objPacketParse.SetPacket_Head_Curr_Length(pHeadPacket->GetPacketLen());
        objPacketParse.SetPacket_Head_Src_Length(pHeadPacket->GetPacketLen());
        objPacketParse.SetPacket_Head_Message(pMb);
    }

    if(NULL != pBodyBuffPacket)
    {
        ACE_Message_Block* pMb = App_MessageBlockManager::instance()->Create(pBodyBuffPacket->GetPacketLen());
        memcpy_safe((char* )pBodyBuffPacket->GetData(), pBodyBuffPacket->GetPacketLen(), pMb->wr_ptr(), pBodyBuffPacket->GetPacketLen());
        pMb->wr_ptr(pBodyBuffPacket->GetPacketLen());
        objPacketParse.SetPacket_Body_Curr_Length(pBodyBuffPacket->GetPacketLen());
        objPacketParse.SetPacket_Body_Src_Length(pBodyBuffPacket->GetPacketLen());
        objPacketParse.SetPacket_Body_Message(pMb);
    }

    objMakePacket.m_PacketType   = PACKET_TCP;
    objMakePacket.m_u4ConnectID  = u4ConnectID;
    objMakePacket.m_pPacketParse = &objPacketParse;
    objMakePacket.m_u1Option     = PACKET_PARSE;

    if(true == App_MakePacket::instance()->PutMessageBlock(&objMakePacket, tvNow))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
