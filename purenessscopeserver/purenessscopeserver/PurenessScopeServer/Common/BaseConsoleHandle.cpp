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

    //回收IBuffPacket发送数据的内存缓冲
    App_BuffPacketManager::instance()->Delete(pBuffPacket);

    return true;
}

bool Console_Common_CheckMessage_Data(uint32& u4AllRecvSize, uint32& u4AllRecvCount, CConsolePacketParse* pPacketParse, uint8& u1Output, IBuffPacket*& pBuffPacket)
{
    u4AllRecvSize += (uint32)pPacketParse->GetMessageHead()->length() + (uint32)pPacketParse->GetMessageBody()->length();
    u4AllRecvCount++;

    //发送回复信息
    pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    //将数据Buff放入消息体中，传递给处理类。
    uint32 u4Return = (uint32)App_ConsoleManager::instance()->Dispose(pPacketParse->GetMessageBody(), pBuffPacket, u1Output);

    if (CONSOLE_MESSAGE_SUCCESS == u4Return)
    {
        //需要发送返回包
        return true;
    }
    else if (CONSOLE_MESSAGE_FAIL == u4Return)
    {
        OUR_DEBUG((LM_INFO, "[CProConsoleHandle::CheckMessage]Dispose CONSOLE_MESSAGE_FAIL.\n"));
        App_BuffPacketManager::instance()->Delete(pBuffPacket);
        return false;
    }
    else
    {
        App_BuffPacketManager::instance()->Delete(pBuffPacket);
        return false;
    }
}

bool check_console_ip(const char* pConsoleIP)
{
    for (int i = 0; i < (int)GetXmlConfigAttribute(xmlConsoleClients)->vec.size(); i++)
    {
        if (ACE_OS::strcmp(GetXmlConfigAttribute(xmlConsoleClients)->vec[i].cip.c_str(), pConsoleIP) == 0)
        {
            return true;
        }
    }

    return false;
}
