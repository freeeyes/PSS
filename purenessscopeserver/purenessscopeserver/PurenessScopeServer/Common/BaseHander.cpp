#include "BaseHander.h"

void Recovery_Message(bool blDelete, char*& pMessage)
{
    if (true == blDelete)
    {
        SAFE_DELETE_ARRAY(pMessage);
    }
}

bool Udp_Common_Send_Message(uint32 u4PacketParseInfoID, ACE_INET_Addr& AddrRemote, char*& pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead, uint16 u2CommandID, bool blDlete, ACE_Message_Block*& pMbData)
{
    int nErr = AddrRemote.set(nPort, szIP);

    if (nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[Udp_Common_Send_Message]set_address error[%d].\n", errno));
        Recovery_Message(blDlete, pMessage);
        return false;
    }

    if (blHead == true)
    {
        //如果需要拼接包头，则拼接包头
        CPacketParse PacketParse;

        uint32 u4SendLength = App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Make_Send_Packet_Length(0, u4Len, u2CommandID);
        pMbData = App_MessageBlockManager::instance()->Create(u4SendLength);

        if (NULL == pMbData)
        {
            OUR_DEBUG((LM_INFO, "Udp_Common_Send_Message]pMbData is NULL.\n"));
            Recovery_Message(blDlete, pMessage);
            return false;
        }

        if (false == App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Make_Send_Packet(0, pMessage, u4Len, pMbData, u2CommandID))
        {
            OUR_DEBUG((LM_INFO, "Udp_Common_Send_Message]Make_Send_Packet is false.\n"));
            Recovery_Message(blDlete, pMessage);
            return false;
        }

        Recovery_Message(blDlete, pMessage);
        return true;
    }
    else
    {
        //不需要拼接包头，直接发送
        pMbData = App_MessageBlockManager::instance()->Create(u4Len);

        if (NULL == pMbData)
        {
            OUR_DEBUG((LM_INFO, "Udp_Common_Send_Message]pMbData is NULL.\n"));
            Recovery_Message(blDlete, pMessage);
            return false;
        }

        //将数据拷贝到ACE_Message_Block对象中
        memcpy_safe(pMessage, u4Len, pMbData->wr_ptr(), u4Len);
        pMbData->wr_ptr(u4Len);

        Recovery_Message(blDlete, pMessage);
        return true;
    }
}
