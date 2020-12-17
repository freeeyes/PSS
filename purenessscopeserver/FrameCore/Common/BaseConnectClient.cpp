#include "BaseConnectClient.h"

bool Recv_Common_Dispose_Client_Message(uint16 u2CommandID, ACE_Message_Block* pRecvFinish, const _ClientIPInfo& objServerIPInfo, IClientMessage* pClientMessage)
{
    //�������ݰ�����
    pClientMessage->RecvData(u2CommandID, pRecvFinish, objServerIPInfo);
    //���մ����
    App_MessageBlockManager::instance()->Close(pRecvFinish);

    return true;
}

int Make_Common_Dispose_Client_WorkTread_Message(uint16 u2CommandID, uint32 u4ServerID, ACE_Message_Block* pmblk, const ACE_INET_Addr& AddrRemote)
{
    //��֯����
    auto pWorkThreadMessage = App_MessageServiceGroup::instance()->CreateMessage(u4ServerID, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

    if (nullptr == pWorkThreadMessage)
    {
        //������Ϣ���ʧ��
        OUR_DEBUG((LM_ERROR, "[CConnectClient::SendMessageGroup] ConnectID = %d CreateMessage fail.\n", u4ServerID));
        App_MessageBlockManager::instance()->Close(pmblk);
        return -1;
    }
    else
    {
        ACE_Message_Block* pMBBHead = App_MessageBlockManager::instance()->Create(sizeof(uint32));

        //�����Ϣ��ͷ
        auto u4PacketLen = (uint32)pmblk->length();
        memcpy_safe((char*)&u4PacketLen, sizeof(uint32), pMBBHead->wr_ptr(), sizeof(uint32));
        pMBBHead->wr_ptr(sizeof(uint32));

        pWorkThreadMessage->m_u2Cmd = u2CommandID;
        pWorkThreadMessage->m_AddrRemote = AddrRemote;
        pWorkThreadMessage->m_pmbRecvHead = pMBBHead;
        pWorkThreadMessage->m_pmbRecvBody = pmblk;
        pWorkThreadMessage->m_emResouceType = EM_PACKET_RESOURCE::PACKET_RESOURCE_FROM_SERVER;
        pWorkThreadMessage->m_emDirect = EM_WORKTHREAD_DIRECT::EM_WORKTHREAD_DIRECT_INPUT;

        auto pMessageList = std::make_shared<CWorkThreadMessageList>();
        pMessageList->m_vecList.emplace_back(pWorkThreadMessage);
        pMessageList->m_u4WorkThreadID = pWorkThreadMessage->m_u4WorkThreadID;

        //��Ҫ�������Ϣ������Ϣ�����߳�
        App_MessageServiceGroup::instance()->PutMessage(pMessageList);
    }

    return 0;
}
