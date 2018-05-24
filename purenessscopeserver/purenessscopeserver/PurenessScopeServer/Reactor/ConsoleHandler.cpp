#include "ConsoleHandler.h"

CConsoleHandler::CConsoleHandler(void)
{
    m_szError[0]       = '\0';
    m_u4ConnectID      = 0;
    m_u2SendCount      = 0;
    m_u4AllRecvCount   = 0;
    m_u4AllSendCount   = 0;
    m_u4AllRecvSize    = 0;
    m_u4AllSendSize    = 0;
    m_nIOCount         = 1;
    m_u4SendThresHold  = MAX_MSG_SNEDTHRESHOLD;
    m_u2SendQueueMax   = MAX_MSG_SENDPACKET;
    m_u1ConnectState   = CONNECT_INIT;
    m_u1SendBuffState  = CONNECT_SENDNON;
    m_pTCClose         = NULL;
    m_u1IsClosing      = HANDLE_ISCLOSE_NO;
    m_pCurrMessage     = NULL;
    m_pPacketParse     = NULL;
    m_u4CurrSize       = 0;
    m_u4HandlerID      = 0;
    m_u2MaxConnectTime = 0;
    m_u4SendCheckTime  = 0;
}

CConsoleHandler::~CConsoleHandler(void)
{
    OUR_DEBUG((LM_ERROR, "[CConsoleHandler::~CConsoleHandler].\n"));
    SAFE_DELETE(m_pTCClose);
    OUR_DEBUG((LM_ERROR, "[CConsoleHandler::~CConsoleHandler]End.\n"));
}

const char* CConsoleHandler::GetError()
{
    return m_szError;
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
        if (m_pCurrMessage != NULL)
        {
            App_MessageBlockManager::instance()->Close(m_pCurrMessage);
        }

        SAFE_DELETE(m_pPacketParse);
        shutdown();
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::Close]Close(%d) OK.\n", GetConnectID()));
        delete this;
    }
}

bool CConsoleHandler::ServerClose()
{
    OUR_DEBUG((LM_ERROR, "[CConsoleHandler::ServerClose]Close(%d) OK.\n", GetConnectID()));
    shutdown();
    m_u1ConnectState = CONNECT_SERVER_CLOSE;
    return true;
}

void CConsoleHandler::SetConnectID(uint32 u4ConnectID)
{
    m_u4ConnectID = u4ConnectID;
}

uint32 CConsoleHandler::GetConnectID()
{
    return m_u4ConnectID;
}

int CConsoleHandler::open(void*)
{
    m_nIOCount = 1;
    //初始化key值列表
    App_ConsoleManager::instance()->SetConsoleKey(GetXmlConfigAttribute(xmlConsoleKeys)->vec);
    int nRet = ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>::open();

    if (nRet != 0)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::open]ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>::open() error [%d].\n", nRet));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::open]ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>::open() error [%d].", nRet);
        return -1;
    }

    //设置链接为非阻塞模式
    if (this->peer().enable(ACE_NONBLOCK) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::open]this->peer().enable  = ACE_NONBLOCK error.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::open]this->peer().enable  = ACE_NONBLOCK error.");
        return -1;
    }

    //获得远程链接地址和端口
    if (this->peer().get_remote_addr(m_addrRemote) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::open]this->peer().get_remote_addr error.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::open]this->peer().get_remote_addr error.");
        return -1;
    }

    //判断是否在服务器允许的IP范围内
    if (App_MainConfig::instance()->CompareConsoleClinetIP(m_addrRemote.get_host_addr()) == false)
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
    m_pPacketParse = new CConsolePacketParse();

    if (NULL == m_pPacketParse)
    {
        OUR_DEBUG((LM_DEBUG, "[%t|CConnectHandle::open] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
        return -1;
    }

    m_pPacketParse->Init();

    //申请头的大小对应的mb
    m_pCurrMessage = App_MessageBlockManager::instance()->Create(CONSOLE_PACKET_MAX_SIZE);

    if (m_pCurrMessage == NULL)
    {
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
        OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvClinetPacket] pmb new is NULL.\n"));
        return -1;
    }

    m_u1ConnectState = CONNECT_OPEN;
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

    if (fd == ACE_INVALID_HANDLE || NULL == m_pPacketParse)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input]fd == ACE_INVALID_HANDLE or m_pPacketParse is NULL.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::handle_input]fd == ACE_INVALID_HANDLE or m_pPacketParse is NULL.");
        return -1;
    }

    //判断缓冲是否为NULL
    if (m_pCurrMessage == NULL)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input]m_pCurrMessage == NULL.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::handle_input]m_pCurrMessage == NULL.");

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

    int nDataLen = (int)this->peer().recv(m_pCurrMessage->wr_ptr(), nCurrCount, MSG_NOSIGNAL, &nowait);

    if (nDataLen <= 0)
    {
        m_u4CurrSize = 0;
        uint32 u4Error = (uint32)errno;
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input] ConnectID = %d, recv data is error nDataLen = [%d] errno = [%d].\n", GetConnectID(), nDataLen, u4Error));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::handle_input] ConnectID = %d, recv data is error[%d].\n", GetConnectID(), nDataLen);

        Clear_PacketParse();
        return -1;
    }

    m_u4CurrSize += nDataLen;
    m_pCurrMessage->wr_ptr(nDataLen);

    const char* pData = m_pCurrMessage->rd_ptr();
    uint32 u4Len = (uint32)m_pCurrMessage->length();

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

        if (NULL != pmbHead)
        {
            memcpy_safe(pmbHead->wr_ptr(), sizeof(int), (char*)&u4Len, sizeof(int));
            pmbHead->wr_ptr(sizeof(int));
            m_pPacketParse->SetPacketHead(GetConnectID(), pmbHead, App_MessageBlockManager::instance());
            //组装包体
            m_pPacketParse->SetPacketBody(GetConnectID(), m_pCurrMessage, App_MessageBlockManager::instance());

            if (false == CheckMessage())
            {
                OUR_DEBUG((LM_INFO, "[CConsoleHandler::handle_input]CheckMessage error.\n"));

                Clear_PacketParse();
                return -1;
            }

        }

        Clear_PacketParse();

        m_pPacketParse = new CConsolePacketParse();

        //申请头的大小对应的mb
        m_pCurrMessage = App_MessageBlockManager::instance()->Create(CONSOLE_PACKET_MAX_SIZE);

        if (m_pCurrMessage == NULL)
        {
            AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
            OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvClinetPacket] pmb new is NULL.\n"));
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
        OUR_DEBUG((LM_DEBUG, "[CConsoleHandler::handle_close] h is NULL mask=%d.\n", (int)mask));
    }

    OUR_DEBUG((LM_DEBUG, "[CConsoleHandler::handle_close]Connectid=[%d] begin(%d)...\n", GetConnectID(), errno));
    Close(2);
    return 0;
}

uint8 CConsoleHandler::GetConnectState()
{
    return m_u1ConnectState;
}

uint8 CConsoleHandler::GetSendBuffState()
{
    return m_u1SendBuffState;
}

uint8 CConsoleHandler::GetIsClosing()
{
    return m_u1IsClosing;
}

bool CConsoleHandler::SendMessage(IBuffPacket* pBuffPacket, uint8 u1OutputType)
{
    m_ThreadLock.acquire();
    m_nIOCount++;
    m_ThreadLock.release();

    ACE_Message_Block* pMbData = NULL;

    if (false == Console_Common_SendMessage_Data_Check(GetConnectID(), pBuffPacket, u1OutputType, pMbData))
    {
        Close();
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

    if (NULL == pMbData)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID()));
        Close();
        return false;
    }

    if (get_handle() == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID()));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::SendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID());
        App_MessageBlockManager::instance()->Close(pMbData);
        Close();
        return false;
    }

    //发送数据
    char* pData = pMbData->rd_ptr();

    if (NULL == pData)
    {
        OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, pData is NULL.\n", GetConnectID()));
        App_MessageBlockManager::instance()->Close(pMbData);
        Close();
        return false;
    }

    int nIsSendSize = 0;

    //循环发送，直到数据发送完成。
    while (true)
    {
        int nCurrSendSize = (int)(pMbData->length() - nIsSendSize);

        if (nCurrSendSize <= 0)
        {
            OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, nCurrSendSize error is %d.\n", GetConnectID(), nCurrSendSize));
            App_MessageBlockManager::instance()->Close(pMbData);
            return false;
        }

        int nDataLen = (int)this->peer().send(pMbData->rd_ptr(), nCurrSendSize, &nowait);

        if (nDataLen <= 0)
        {
            OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, error = %d.\n", GetConnectID(), errno));
            App_MessageBlockManager::instance()->Close(pMbData);
            m_atvOutput      = ACE_OS::gettimeofday();
            Close();
            return false;
        }
        else if (nDataLen >= nCurrSendSize)  //当数据包全部发送完毕，清空。
        {
            m_u4AllSendCount += 1;
            m_u4AllSendSize  += (uint32)pMbData->length();
            App_MessageBlockManager::instance()->Close(pMbData);
            m_atvOutput      = ACE_OS::gettimeofday();
            Close();
            return true;
        }
        else
        {
            pMbData->rd_ptr(nDataLen);
            nIsSendSize      += nDataLen;
            m_atvOutput      = ACE_OS::gettimeofday();
        }
    }

    return true;
}

void CConsoleHandler::Clear_PacketParse()
{
    if (m_pPacketParse->GetMessageHead() != NULL)
    {
        App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
    }

    if (m_pPacketParse->GetMessageBody() != NULL)
    {
        App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
    }

    if (m_pCurrMessage != NULL && m_pPacketParse->GetMessageBody() != m_pCurrMessage && m_pPacketParse->GetMessageHead() != m_pCurrMessage)
    {
        App_MessageBlockManager::instance()->Close(m_pCurrMessage);
    }

    m_pCurrMessage = NULL;

    SAFE_DELETE(m_pPacketParse);
}

bool CConsoleHandler::CheckMessage()
{
    uint8 u1Output = 0;
    IBuffPacket* pBuffPacket = NULL;
    bool blRet = Console_Common_CheckMessage_Data(m_u4AllRecvSize, m_u4AllRecvCount, m_pPacketParse, u1Output, pBuffPacket);

    if (true == blRet && false == SendMessage(dynamic_cast<IBuffPacket*>(pBuffPacket), u1Output))
    {
        OUR_DEBUG((LM_INFO, "[CConsoleHandler::CheckMessage]SendMessage error.\n"));
    }

    return true;
}
