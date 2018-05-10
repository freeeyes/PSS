#include "BaseConsoleHandle.h"


bool Console_Common_SendMessage_Data_Check(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1OutputType, ACE_Message_Block*& pMbData)
{
    CConsolePacketParse PacketParse;

    if (NULL == pBuffPacket)
    {
        return false;
    }

    if (0 == u1OutputType)
    {
        int nSendLength = PacketParse.MakePacketLength(u4ConnectID, pBuffPacket->GetPacketLen());
        pMbData = App_MessageBlockManager::instance()->Create(nSendLength);
        //这里组成返回数据包
        PacketParse.MakePacket(u4ConnectID, pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pMbData);
    }
    else
    {
        pMbData = App_MessageBlockManager::instance()->Create(pBuffPacket->GetPacketLen());
        memcpy_safe((char*)pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pMbData->wr_ptr(), pBuffPacket->GetPacketLen());
        pMbData->wr_ptr(pBuffPacket->GetPacketLen());
    }

    App_BuffPacketManager::instance()->Delete(pBuffPacket);

    return true;
}
