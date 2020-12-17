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
        //������ɷ������ݰ�
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

    //���ͻظ���Ϣ
    pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    //������Buff������Ϣ���У����ݸ������ࡣ
    auto emReturn = App_ConsoleManager::instance()->Dispose(pPacketParse->GetMessageBody(), pBuffPacket, u1Output);

    if (EM_CONSOLE_MESSAGE::CONSOLE_MESSAGE_SUCCESS == emReturn)
    {
        //��Ҫ���ͷ��ذ�
        return true;
    }
    else if (EM_CONSOLE_MESSAGE::CONSOLE_MESSAGE_FAIL == emReturn)
    {
        OUR_DEBUG((LM_INFO, "[CProConsoleHandle::CheckMessage]Dispose CONSOLE_MESSAGE_FAIL.\n"));
        return false;
    }
    else
    {
        return false;
    }
}

bool check_console_ip(const char* pConsoleIP)
{
    //�����0�����ã������������е�Ip���ӷ���
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
