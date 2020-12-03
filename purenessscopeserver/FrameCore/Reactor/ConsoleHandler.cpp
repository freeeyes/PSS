#include "ConsoleHandler.h"

const char* CConsoleHandler::GetError() const
{
    return m_strError.c_str();
}

void CConsoleHandler::Close(int nIOCount)
{
    m_ThreadLock.acquire();

    if (nIOCount > m_nIOCount)
    {
        m_nIOCount = 0;
    }

    if (m_nIOCount > 0)
    {
        m_nIOCount -= nIOCount;
    }

    m_ThreadLock.release();

    //从反应器注销事件
    if (m_nIOCount == 0)
    {
        //删除对象缓冲的PacketParse
        if (m_pCurrMessage != nullptr)
        {
            App_MessageBlockManager::instance()->Close(m_pCurrMessage);
        }

        shutdown();
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::Close]Close(%d) OK.\n", GetConnectID()));
        SAFE_DELETE(m_pTCClose);
        delete this;
    }
}

bool CConsoleHandler::ServerClose()
{
    OUR_DEBUG((LM_ERROR, "[CConsoleHandler::ServerClose]Close(%d) OK.\n", GetConnectID()));
    shutdown();
    m_u1ConnectState = CONNECTSTATE::CONNECT_SERVER_CLOSE;
    return true;
}

void CConsoleHandler::SetConnectID(uint32 u4ConnectID)
{
    m_u4ConnectID = u4ConnectID;
}

uint32 CConsoleHandler::GetConnectID() const
{
    return m_u4ConnectID;
}

int CConsoleHandler::open(void*)
{
    m_nIOCount = 1;
    int nRet = ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>::open();

    if (nRet != 0)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::open]ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>::open() error [%d].\n", nRet));
        m_strError = fmt::format("[CConsoleHandler::open]ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>::open() error [{0}].", nRet);
        return -1;
    }

    //设置链接为非阻塞模式
    if (this->peer().enable(ACE_NONBLOCK) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::open]this->peer().enable  = ACE_NONBLOCK error.\n"));
        m_strError = "CConsoleHandler::open]this->peer().enable  = ACE_NONBLOCK error.";
        return -1;
    }

    //获得远程链接地址和端口
    if (this->peer().get_remote_addr(m_addrRemote) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::open]this->peer().get_remote_addr error.\n"));
        m_strError = "[CConsoleHandler::open]this->peer().get_remote_addr error.";
        return -1;
    }

    //判断是否在服务器允许的IP范围内
    if (CompareConsoleClinetIP(m_addrRemote.get_host_addr()) == false)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::open]this IP is abort.\n"));
        return -1;
    }

    OUR_DEBUG((LM_INFO, "[CConsoleHandler::open] Connection from [%s:%d]\n", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number()));
    m_atvConnect      = ACE_OS::gettimeofday();
    m_atvInput        = ACE_OS::gettimeofday();
    m_atvOutput       = ACE_OS::gettimeofday();
    m_atvSendAlive    = ACE_OS::gettimeofday();
    m_u4AllRecvCount  = 0;
    m_u4AllSendCount  = 0;
    m_u4AllRecvSize   = 0;
    m_u4AllSendSize   = 0;
    //设置接收缓冲池的大小
    int nTecvBuffSize = MAX_MSG_SOCKETBUFF;
    ACE_OS::setsockopt(this->get_handle(), SOL_SOCKET, SO_SNDBUF, (char*)&nTecvBuffSize, sizeof(nTecvBuffSize));
    m_pPacketParse = std::make_shared<CConsolePacketParse>();

    m_pPacketParse->Init();

    //申请头的大小对应的mb
    m_pCurrMessage = App_MessageBlockManager::instance()->Create(CONSOLE_PACKET_MAX_SIZE);

    if (m_pCurrMessage == nullptr)
    {
        string strLog = fmt::format("Close Connection from [{0}:{1}] RecvSize = {2}, RecvCount = {3}, SendSize = {4}, SendCount = {5}.",
            m_addrRemote.get_host_addr(), 
            m_addrRemote.get_port_number(), 
            m_u4AllRecvSize, 
            m_u4AllRecvCount, 
            m_u4AllSendSize, 
            m_u4AllSendCount);

        AppLogManager::instance()->WriteLog_r(LOG_SYSTEM_CONNECT, strLog);
        OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvClinetPacket] pmb new is nullptr.\n"));
        return -1;
    }

    m_u1ConnectState = CONNECTSTATE::CONNECT_OPEN;
    return nRet;
}

//接受数据
int CConsoleHandler::handle_input(ACE_HANDLE fd)
{
    m_ThreadLock.acquire();
    m_nIOCount++;
    m_ThreadLock.release();
    ACE_Time_Value nowait(MAX_MSG_PACKETTIMEOUT);
    m_atvInput = ACE_OS::gettimeofday();

    if (fd == ACE_INVALID_HANDLE || nullptr == m_pPacketParse)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input]fd == ACE_INVALID_HANDLE or m_pPacketParse is nullptr.\n"));
        m_strError = "[CConsoleHandler::handle_input]fd == ACE_INVALID_HANDLE or m_pPacketParse is nullptr.";
        return -1;
    }

    //判断缓冲是否为nullptr
    if (m_pCurrMessage == nullptr)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input]m_pCurrMessage == nullptr.\n"));
        m_strError = "[CConsoleHandler::handle_input]m_pCurrMessage == nullptr.";

        Clear_PacketParse();
        return -1;
    }

    int nCurrCount = (uint32)CONSOLE_PACKET_MAX_SIZE - m_u4CurrSize;

    //这里需要对m_u4CurrSize进行检查。
    if (nCurrCount < 0)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input][%d] nCurrCount < 0 m_u4CurrSize = %d.\n", GetConnectID(), m_u4CurrSize));
        m_u4CurrSize = 0;

        Clear_PacketParse();
        return -1;
    }

    auto nDataLen = (int)this->peer().recv(m_pCurrMessage->wr_ptr(), nCurrCount, MSG_NOSIGNAL, &nowait);

    if (nDataLen <= 0)
    {
        m_u4CurrSize = 0;
        uint32 u4Error = (uint32)errno;
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input] ConnectID = %d, recv data is error nDataLen = [%d] errno = [%d].\n", GetConnectID(), nDataLen, u4Error));
        m_strError = fmt::format("[CConsoleHandler::handle_input] ConnectID = {0}, recv data is error[{1}].", GetConnectID(), nDataLen);

        Clear_PacketParse();
        return -1;
    }

    m_u4CurrSize += nDataLen;
    m_pCurrMessage->wr_ptr(nDataLen);

    const char* pData = m_pCurrMessage->rd_ptr();
    auto u4Len = (uint32)m_pCurrMessage->length();

    OUR_DEBUG((LM_INFO, "[CConsoleHandler::handle_input]End is(0x%02x).\n", pData[nDataLen - 1]));

    //如果没有读完，短读
    if (pData[u4Len - 1] != '&' && pData[u4Len - 1] != 0x0d && pData[u4Len - 1] != 0x0a)
    {
        Close();
        return 0;
    }
    else
    {
        //完整数据包
        //组装包头
        ACE_Message_Block* pmbHead = App_MessageBlockManager::instance()->Create(sizeof(int));

        if (nullptr != pmbHead)
        {
            memcpy_safe(pmbHead->wr_ptr(), sizeof(int), (char*)&u4Len, sizeof(int));
            pmbHead->wr_ptr(sizeof(int));
            m_pPacketParse->SetPacketHead(GetConnectID(), pmbHead, App_MessageBlockManager::instance());
            //组装包体
            m_pPacketParse->SetPacketBody(GetConnectID(), m_pCurrMessage, App_MessageBlockManager::instance());

            if (false == CheckMessage())
            {
                OUR_DEBUG((LM_INFO, "[CConsoleHandler::handle_input]CheckMessage error.\n"));
                return -1;
            }

        }

        //申请头的大小对应的mb
        m_pCurrMessage = App_MessageBlockManager::instance()->Create(CONSOLE_PACKET_MAX_SIZE);

        if (m_pCurrMessage == nullptr)
        {
            string strLog = fmt::format("Close Connection from [{0}:{1}] RecvSize = {2}, RecvCount = {3}, SendSize = {4}, SendCount = {5}.",
                m_addrRemote.get_host_addr(), 
                m_addrRemote.get_port_number(), 
                m_u4AllRecvSize, 
                m_u4AllRecvCount, 
                m_u4AllSendSize, 
                m_u4AllSendCount);

            AppLogManager::instance()->WriteLog_r(LOG_SYSTEM_CONNECT, strLog);
            OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvClinetPacket] pmb new is nullptr.\n"));
            return -1;
        }

        Close();
    }

    return 0;
}

//关闭链接
int CConsoleHandler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
    if (h == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_DEBUG, "[CConsoleHandler::handle_close] h is nullptr mask=%d.\n", (int)mask));
    }

    OUR_DEBUG((LM_DEBUG, "[CConsoleHandler::handle_close]Connectid=[%d] begin(%d)...\n", GetConnectID(), errno));
    Close(2);
    return 0;
}

CONNECTSTATE CConsoleHandler::GetConnectState() const
{
    return m_u1ConnectState;
}

CONNECTSTATE CConsoleHandler::GetSendBuffState() const
{
    return m_u1SendBuffState;
}

uint8 CConsoleHandler::GetIsClosing() const
{
    return m_u1IsClosing;
}

bool CConsoleHandler::SendMessage(IBuffPacket* pBuffPacket, uint8 u1OutputType)
{
    ACE_Message_Block* pMbData = nullptr;

    if (false == Console_Common_SendMessage_Data_Check(GetConnectID(), pBuffPacket, u1OutputType, pMbData))
    {
        return false;
    }

    if (false == PutSendPacket(pMbData))
    {
        OUR_DEBUG((LM_INFO, "[CConsoleHandler::SendMessage]PutSendPacket error.\n"));
    }

    return true;
}

bool CConsoleHandler::PutSendPacket(ACE_Message_Block* pMbData)
{
    ACE_Time_Value     nowait(MAX_MSG_PACKETTIMEOUT);

    if (nullptr == pMbData)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID()));
        return false;
    }

    if (get_handle() == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID()));
        m_strError = fmt::format("[CConsoleHandler::SendPacket] ConnectID = {0}, get_handle() == ACE_INVALID_HANDLE.", GetConnectID());
        App_MessageBlockManager::instance()->Close(pMbData);
        return false;
    }

    //发送数据
    const char* pData = pMbData->rd_ptr();

    if (nullptr == pData)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, pData is nullptr.\n", GetConnectID()));
        App_MessageBlockManager::instance()->Close(pMbData);
        return false;
    }

    int nIsSendSize = 0;

    //循环发送，直到数据发送完成。
    while (true)
    {
        auto nCurrSendSize = (int)(pMbData->length() - nIsSendSize);

        if (nCurrSendSize <= 0)
        {
            OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, nCurrSendSize error is %d.\n", GetConnectID(), nCurrSendSize));
            App_MessageBlockManager::instance()->Close(pMbData);
            return false;
        }

        auto nDataLen = (int)this->peer().send(pMbData->rd_ptr(), nCurrSendSize, &nowait);

        if (nDataLen <= 0)
        {
            OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, error = %d.\n", GetConnectID(), errno));
            App_MessageBlockManager::instance()->Close(pMbData);
            m_atvOutput      = ACE_OS::gettimeofday();
            return false;
        }
        else if (nDataLen >= nCurrSendSize)  //当数据包全部发送完毕，清空。
        {
            m_u4AllSendCount += 1;
            m_u4AllSendSize  += (uint32)pMbData->length();
            App_MessageBlockManager::instance()->Close(pMbData);
            m_atvOutput      = ACE_OS::gettimeofday();
            return true;
        }
        else
        {
            pMbData->rd_ptr(nDataLen);
            nIsSendSize      += nDataLen;
            m_atvOutput      = ACE_OS::gettimeofday();
        }
    }
}

void CConsoleHandler::Clear_PacketParse()
{
    if (m_pPacketParse->GetMessageHead() != nullptr)
    {
        App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
    }

    if (m_pPacketParse->GetMessageBody() != nullptr)
    {
        App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
    }

    if (m_pCurrMessage != nullptr && m_pPacketParse->GetMessageBody() != m_pCurrMessage && m_pPacketParse->GetMessageHead() != m_pCurrMessage)
    {
        App_MessageBlockManager::instance()->Close(m_pCurrMessage);
    }

    m_pCurrMessage = nullptr;
}

bool CConsoleHandler::CompareConsoleClinetIP(const char* pIP) const
{
    if (GetXmlConfigAttribute(xmlConsoleClients)->vec.empty())
    {
        return true;
    }

    for (xmlConsoleClients::_ConsoleClient consoleclient : GetXmlConfigAttribute(xmlConsoleClients)->vec)
    {
        if (ACE_OS::strcmp(consoleclient.cip.c_str(), pIP) == 0)
        {
            return true;
        }
    }

    return false;
}

bool CConsoleHandler::CheckMessage()
{
    uint8 u1Output = 0;
    IBuffPacket* pBuffPacket = nullptr;
    bool blRet = Console_Common_CheckMessage_Data(m_u4AllRecvSize, m_u4AllRecvCount, m_pPacketParse, u1Output, pBuffPacket);

    //回收内存
    m_pPacketParse->Close();

    if (true == blRet && false == SendMessage(pBuffPacket, u1Output))
    {
        OUR_DEBUG((LM_INFO, "[CConsoleHandler::CheckMessage]SendMessage error.\n"));
        return false;
    }
    else
    {
        return true;
    }
}
