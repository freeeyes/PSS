#include "BaseClientConnectManager.h"

void Common_Send_ConnectError(ACE_Message_Block* pmblk, const ACE_INET_Addr& objAddrServer, IClientMessage* pClientMessage)
{
    if (NULL != pmblk)
    {
        App_MessageBlockManager::instance()->Close(pmblk);
    }

    //�����Ϣ�д���ӿڣ��򷵻�ʧ�ܽӿ�
    if (NULL != pClientMessage)
    {
        //�������Ѿ��Ͽ�����Ҫ�ȴ��������ӵĽ��
        _ClientIPInfo objServerIPInfo;
        objServerIPInfo.m_strClientIP = objAddrServer.get_host_addr();
        objServerIPInfo.m_u2Port = objAddrServer.get_port_number();
        pClientMessage->ConnectError(101, objServerIPInfo);
    }
}

bool Common_Send_Data(ACE_Message_Block* pmblk, IClientMessage* pClientMessage, ACE_Message_Block*& pSend)
{
    bool blRet = pClientMessage->Send_Format_data(pmblk->rd_ptr(), (uint32)pmblk->length(), App_MessageBlockManager::instance(), pSend);

    if (false == blRet)
    {
        App_MessageBlockManager::instance()->Close(pmblk);
        App_MessageBlockManager::instance()->Close(pSend);
        return false;
    }
    else
    {
        App_MessageBlockManager::instance()->Close(pmblk);
    }

    return true;
}
