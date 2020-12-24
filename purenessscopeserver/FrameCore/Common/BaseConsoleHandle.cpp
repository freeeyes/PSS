#include "BaseConsoleHandle.h"


bool Console_Common_SendMessage_Data_Check(uint32 u4ConnectID, shared_ptr<IBuffPacket> pBuffPacket, uint8 u1OutputType, ACE_Message_Block*& pMbData)
{
    CConsolePacketParse PacketParse;

    if (nullptr == pBuffPacket)
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
        memcpy_safe(pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pMbData->wr_ptr(), pBuffPacket->GetPacketLen());
        pMbData->wr_ptr(pBuffPacket->GetPacketLen());
    }

    PacketParse.Close();

    return true;
}

bool Console_Common_CheckMessage_Data(uint32& u4AllRecvSize, uint32& u4AllRecvCount, shared_ptr<CConsolePacketParse> pPacketParse, uint8& u1Output, shared_ptr<IBuffPacket>& pBuffPacket)
{
    u4AllRecvSize += (uint32)pPacketParse->GetMessageHead()->length() + (uint32)pPacketParse->GetMessageBody()->length();
    u4AllRecvCount++;

    //发送回复信息
    pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    //将数据Buff放入消息体中，传递给处理类。
    auto emReturn = App_ConsoleManager::instance()->Dispose(pPacketParse->GetMessageBody(), pBuffPacket, u1Output);

    if (EM_CONSOLE_MESSAGE::CONSOLE_MESSAGE_SUCCESS == emReturn)
    {
        //需要发送返回包
        return true;
    }
    else if (EM_CONSOLE_MESSAGE::CONSOLE_MESSAGE_FAIL == emReturn)
    {
        PSS_LOGGER_DEBUG( "[CProConsoleHandle::CheckMessage]Dispose CONSOLE_MESSAGE_FAIL.");
        return false;
    }
    else
    {
        return false;
    }
}

bool check_console_ip(const char* pConsoleIP)
{
    //如果是0个设置，就是允许所有的Ip连接访问
    if (nullptr == GetXmlConfigAttribute(xmlConsoleClients) || true == GetXmlConfigAttribute(xmlConsoleClients)->vec.empty())
    {
        return true;
    }

    bool blRet = std::all_of(GetXmlConfigAttribute(xmlConsoleClients)->vec.cbegin(), GetXmlConfigAttribute(xmlConsoleClients)->vec.cend(), [pConsoleIP](const xmlConsoleClients::_ConsoleClient& x) {
        if (x.cip == pConsoleIP)
        {
            return false;
        }
        else
        {
            return true;
        }
        });

    return !blRet;
}
