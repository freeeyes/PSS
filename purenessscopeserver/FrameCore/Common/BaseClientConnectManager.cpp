#include "BaseClientConnectManager.h"

void Common_Send_ConnectError(ACE_Message_Block* pmblk, const ACE_INET_Addr& objAddrServer, IClientMessage* pClientMessage)
{
    if (nullptr != pmblk)
    {
        App_MessageBlockManager::instance()->Close(pmblk);
    }

    //如果消息有处理接口，则返回失败接口
    if (nullptr != pClientMessage)
    {
        //服务器已经断开，需要等待重新连接的结果
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
