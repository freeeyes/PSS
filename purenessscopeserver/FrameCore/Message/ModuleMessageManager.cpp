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

int CModuleMessageManager::SendFrameMessage(uint16 u2CommandID, uint32 u4WorkThreadID, IBuffPacket* pHeadPacket, IBuffPacket* pBodyBuffPacket)
{
    //向框架重发送消息，通过注册的插件进行消费
    ACE_Time_Value tvNow = ACE_OS::gettimeofday();
    _MakePacket objMakePacket;
    CPacketParse objPacketParse;

    objPacketParse.SetPacket_CommandID(u2CommandID);

    if(nullptr != pHeadPacket)
    {
        ACE_Message_Block* pMb = App_MessageBlockManager::instance()->Create(pHeadPacket->GetPacketLen());
        memcpy_safe(pHeadPacket->GetData(), pHeadPacket->GetPacketLen(), pMb->wr_ptr(), pHeadPacket->GetPacketLen());
        pMb->wr_ptr(pHeadPacket->GetPacketLen());
        objPacketParse.SetPacket_Head_Curr_Length(pHeadPacket->GetPacketLen());
        objPacketParse.SetPacket_Head_Src_Length(pHeadPacket->GetPacketLen());
        objPacketParse.SetPacket_Head_Message(pMb);
    }

    if(nullptr != pBodyBuffPacket)
    {
        ACE_Message_Block* pMb = App_MessageBlockManager::instance()->Create(pBodyBuffPacket->GetPacketLen());
        memcpy_safe(pBodyBuffPacket->GetData(), pBodyBuffPacket->GetPacketLen(), pMb->wr_ptr(), pBodyBuffPacket->GetPacketLen());
        pMb->wr_ptr(pBodyBuffPacket->GetPacketLen());
        objPacketParse.SetPacket_Body_Curr_Length(pBodyBuffPacket->GetPacketLen());
        objPacketParse.SetPacket_Body_Src_Length(pBodyBuffPacket->GetPacketLen());
        objPacketParse.SetPacket_Body_Message(pMb);
    }

    objMakePacket.m_emPacketType = EM_CONNECT_IO_TYPE::CONNECT_IO_FRAME;
    objMakePacket.m_u4ConnectID = u4WorkThreadID;
    objMakePacket.m_pPacketParse = &objPacketParse;
    objMakePacket.m_u1Option = PACKET_PARSE;

    if (true == App_MakePacket::instance()->PutMessageBlock(objMakePacket, tvNow))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
