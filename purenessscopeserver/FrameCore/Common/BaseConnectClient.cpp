#include "BaseConnectClient.h"

bool Recv_Common_Dispose_Client_Message(uint16 u2CommandID, ACE_Message_Block* pRecvFinish, const _ClientIPInfo objServerIPInfo, IClientMessage* pClientMessage)
{
    //有数据需要处理，则处理
    if (GetXmlConfigAttribute(xmlConnectServer)->RunType == 0)
    {
        //调用数据包处理
        pClientMessage->RecvData(u2CommandID, pRecvFinish, objServerIPInfo);
        //回收处理包
        App_MessageBlockManager::instance()->Close(pRecvFinish);
    }
    else
    {
        //异步消息处理
        _Server_Message_Info* pServer_Message_Info = App_ServerMessageInfoPool::instance()->Create();
        pServer_Message_Info->m_pClientMessage = pClientMessage;
        pServer_Message_Info->m_objServerIPInfo = objServerIPInfo;
        pServer_Message_Info->m_pRecvFinish = pRecvFinish;
        pServer_Message_Info->m_u2CommandID = u2CommandID;
        App_ServerMessageTask::instance()->PutMessage(pServer_Message_Info);
    }

    return true;
}

int Make_Common_Dispose_Client_WorkTread_Message(uint16 u2CommandID, uint32 u4ServerID, ACE_Message_Block* pmblk, const ACE_INET_Addr AddrRemote)
{
    //组织数据
    CMessage* pMessage = App_MessageServiceGroup::instance()->CreateMessage(u4ServerID, CONNECT_IO_TCP);

    if (NULL == pMessage)
    {
        //放入消息框架失败
        OUR_DEBUG((LM_ERROR, "[CConnectClient::SendMessageGroup] ConnectID = %d CreateMessage fail.\n", u4ServerID));
        App_MessageBlockManager::instance()->Close(pmblk);
        return -1;
    }
    else
    {
        ACE_Message_Block* pMBBHead = App_MessageBlockManager::instance()->Create(sizeof(uint32));

        if (NULL == pMBBHead)
        {
            OUR_DEBUG((LM_ERROR, "[CConnectClient::SendMessageGroup] ConnectID = %d pMBBHead fail.\n", u4ServerID));
            App_MessageBlockManager::instance()->Close(pmblk);
            return -1;
        }

        //添加消息包头
        uint32 u4PacketLen = (uint32)pmblk->length();
        memcpy_safe((char*)&u4PacketLen, sizeof(uint32), pMBBHead->wr_ptr(), sizeof(uint32));
        pMBBHead->wr_ptr(sizeof(uint32));

        sprintf_safe(pMessage->GetMessageBase()->m_szListenIP, MAX_BUFF_20, "%s", AddrRemote.get_host_addr());
        sprintf_safe(pMessage->GetMessageBase()->m_szIP, MAX_BUFF_20, "127.0.0.1");
        pMessage->GetMessageBase()->m_u2Cmd = u2CommandID;
        pMessage->GetMessageBase()->m_u4ConnectID = u4ServerID;
        pMessage->GetMessageBase()->m_u4ListenPort = (uint32)AddrRemote.get_port_number();
        pMessage->GetMessageBase()->m_tvRecvTime = ACE_OS::gettimeofday();
        pMessage->GetMessageBase()->m_u1ResouceType = RESOUCE_FROM_SERVER;
        pMessage->GetMessageBase()->m_u4HeadSrcSize = sizeof(uint32);
        pMessage->GetMessageBase()->m_u4BodySrcSize = u4PacketLen;
        pMessage->SetPacketHead(pMBBHead);
        pMessage->SetPacketBody(pmblk);

        //将要处理的消息放入消息处理线程
        if (false == App_MessageServiceGroup::instance()->PutMessage(pMessage))
        {
            OUR_DEBUG((LM_ERROR, "[CConnectClient::SendMessageGroup] App_MessageServiceGroup::instance()->PutMessage Error.\n"));
            App_MessageServiceGroup::instance()->DeleteMessage(u4ServerID, pMessage);
            return -1;
        }
    }

    return 0;
}
