#include "ProConsoleHandle.h"

CProConsoleHandle::CProConsoleHandle(void)
{
    m_szError[0]         = '\0';
    m_u4ConnectID        = 0;
    m_u4AllRecvCount     = 0;
    m_u4AllSendCount     = 0;
    m_u4AllRecvSize      = 0;
    m_u4AllSendSize      = 0;
    m_nIOCount           = 0;
    m_u4HandlerID        = 0;
    m_u2MaxConnectTime   = 0;
    m_u4SendThresHold    = MAX_MSG_SNEDTHRESHOLD;
    m_u2SendQueueMax     = MAX_MSG_SENDPACKET;
    m_u1ConnectState     = CONNECT_INIT;
    m_u1SendBuffState    = CONNECT_SENDNON;
    m_pPacketParse       = NULL;
    m_blCanWrite         = false;
    m_blTimeClose        = false;
    m_u4RecvPacketCount  = 0;
    m_u4SendCheckTime    = 0;
}

CProConsoleHandle::~CProConsoleHandle(void)
{
}

const char* CProConsoleHandle::GetError()
{
    return m_szError;
}

void CProConsoleHandle::Close(int nIOCount)
{
    m_ThreadWriteLock.acquire();

    if(nIOCount > m_nIOCount)
    {
        m_nIOCount = 0;
    }

    if(m_nIOCount > 0)
    {
        m_nIOCount -= nIOCount;
    }

    m_ThreadWriteLock.release();

    //OUR_DEBUG((LM_DEBUG, "[CProConsoleHandle::Close]ConnectID=%d, m_nIOCount = %d.\n", GetConnectID(), m_nIOCount));

    if(m_nIOCount == 0)
    {
        m_ThreadWriteLock.acquire();

        m_Reader.cancel();
        m_Writer.cancel();

        if(this->handle() != ACE_INVALID_HANDLE)
        {
            ACE_OS::closesocket(this->handle());
            this->handle(ACE_INVALID_HANDLE);
        }

        m_ThreadWriteLock.release();
        SAFE_DELETE(m_pPacketParse);
        OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::Close] Close(%d) delete OK.\n", GetConnectID()));

        delete this;
    }
}

bool CProConsoleHandle::ServerClose()
{
    OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::ServerClose] Close(%d) delete OK.\n", GetConnectID()));

    m_Reader.cancel();
    m_Writer.cancel();

    if(this->handle() != ACE_INVALID_HANDLE)
    {
        ACE_OS::closesocket(this->handle());
        this->handle(ACE_INVALID_HANDLE);
    }

    m_u1ConnectState = CONNECT_SERVER_CLOSE;

    return true;
}

void CProConsoleHandle::SetConnectID(uint32 u4ConnectID)
{
    m_u4ConnectID = u4ConnectID;
}

uint32 CProConsoleHandle::GetConnectID()
{
    return m_u4ConnectID;
}

void CProConsoleHandle::addresses (const ACE_INET_Addr& remote_address, const ACE_INET_Addr& local_address)
{
    m_addrRemote = remote_address;
}

void CProConsoleHandle::open(ACE_HANDLE h, ACE_Message_Block&)
{
    ACE_Time_Value tvOpenBegin(ACE_OS::gettimeofday());

    //初始化key值列表
    App_ConsoleManager::instance()->SetConsoleKey(App_MainConfig::instance()->GetConsoleKey());

    //OUR_DEBUG((LM_INFO, "[CProConsoleHandle::open] [0x%08x]Connection from [%s:%d]\n", this, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number()));

    m_atvConnect      = ACE_OS::gettimeofday();
    m_atvInput        = ACE_OS::gettimeofday();
    m_atvOutput       = ACE_OS::gettimeofday();
    m_atvSendAlive    = ACE_OS::gettimeofday();

    m_u4AllRecvCount    = 0;
    m_u4AllSendCount    = 0;
    m_u4AllRecvSize     = 0;
    m_u4AllSendSize     = 0;
    m_blCanWrite        = true;
    m_blTimeClose       = false;
    m_u4RecvPacketCount = 0;
    m_nIOCount          = 1;

    ACE_Time_Value tvOpenEnd(ACE_OS::gettimeofday());
    ACE_Time_Value tvOpen(tvOpenEnd - tvOpenBegin);

    //AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Connection from [%s:%d] DisposeTime = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), tvOpen.msec());

    this->handle(h);

    if(this->m_Reader.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE)) == -1 ||
       this->m_Writer.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE)) == -1)
    {
        OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::open] m_reader or m_reader == 0.\n"));
        Close();
        return;
    }

    //ACE_Sig_Action writeAction((ACE_SignalHandler)SIG_IGN);
    //writeAction.register_action(SIGPIPE, 0);

    //int nTecvBuffSize = MAX_MSG_SOCKETBUFF;
    //ACE_OS::setsockopt(this->get_handle(), SOL_SOCKET, SO_RCVBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));
    //ACE_OS::setsockopt(h, SOL_SOCKET, SO_SNDBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));

    m_u1ConnectState = CONNECT_OPEN;

    OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::open] Open(%d).\n", GetConnectID()));

    m_pPacketParse = new CConsolePacketParse();

    if(NULL == m_pPacketParse)
    {
        OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::open] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
        Close();
        return;
    }

    if (false == RecvClinetPacket(CONSOLE_PACKET_MAX_SIZE))
    {
        OUR_DEBUG((LM_INFO, "[CProConsoleHandle::open]RecvClinetPacket error.\n"));
    }

    return;
}

void CProConsoleHandle::handle_read_stream(const ACE_Asynch_Read_Stream::Result& result)
{
    ACE_Message_Block& mb = result.message_block();
    uint32 u4PacketLen = (uint32)result.bytes_transferred();
    //int nTran = (int)result.bytes_transferred();

    if(!result.success() || result.bytes_transferred() == 0)
    {
        //链接断开
        if(m_pPacketParse->GetMessageHead() != NULL)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
        }

        if(m_pPacketParse->GetMessageBody() != NULL)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
        }

        if(&mb != m_pPacketParse->GetMessageHead() && &mb != m_pPacketParse->GetMessageBody())
        {
            //OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::handle_read_stream] Message_block release.\n"));
            App_MessageBlockManager::instance()->Close(&mb);
        }

        SAFE_DELETE(m_pPacketParse);

        OUR_DEBUG((LM_DEBUG,"[CConnectHandler::handle_read_stream]Connectid=[%d] error(%d)...\n", GetConnectID(), errno));
        //AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
        //因为是要关闭连接，所以要多关闭一次IO，对应Open设置的1的初始值

        Close(2);

        return;
    }

    m_atvInput = ACE_OS::gettimeofday();


    const char* pData = mb.rd_ptr();
    uint32 u4Len = (uint32)mb.length();

    //OUR_DEBUG((LM_INFO, "[CProConsoleHandle::handle_input]<%d>Data is(%s).\n", u4Len, pData));
    OUR_DEBUG((LM_INFO, "[CProConsoleHandle::handle_input]End is(0x%02x).\n", pData[u4Len - 1]));

    //判断命令最后一个是不是一个\n
    if (pData[u4Len - 1] != '&' && pData[u4Len - 1] != 0x0d && pData[u4Len - 1] != 0x0a)
    {
        //短读 继续读
        int nRead = (int)result.bytes_to_read() - (int)result.bytes_transferred();

        if (-1 == m_Reader.read(mb, nRead))
        {
            if (m_pPacketParse->GetMessageHead() != NULL)
            {
                App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
            }

            if (m_pPacketParse->GetMessageBody() != NULL)
            {
                App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
            }

            if (&mb != m_pPacketParse->GetMessageHead() && &mb != m_pPacketParse->GetMessageBody())
            {
                App_MessageBlockManager::instance()->Close(&mb);
            }

            SAFE_DELETE(m_pPacketParse);

            OUR_DEBUG((LM_ERROR, "[CProConsoleHandle::handle_input]Read Shoter error(%d).", errno));
            //AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
            //因为是要关闭连接，所以要多关闭一次IO，对应Open设置的1的初始值

            Close(2);
            return;
        }
    }
    else
    {
        //完整命令包
        ACE_Message_Block* pmbHead = App_MessageBlockManager::instance()->Create(sizeof(int));

        if (NULL != pmbHead)
        {
            //组装包头
            memcpy_safe(pmbHead->wr_ptr(), sizeof(int), (char* )&u4Len, sizeof(int));
            pmbHead->wr_ptr(sizeof(int));
            m_pPacketParse->SetPacketHead(GetConnectID(), pmbHead, App_MessageBlockManager::instance());
            //组装包体
            m_pPacketParse->SetPacketBody(GetConnectID(), &mb, App_MessageBlockManager::instance());

            if (false == CheckMessage())
            {
                OUR_DEBUG((LM_INFO, "[CProConsoleHandle::handle_input]CheckMessage error.\n"));

                if (m_pPacketParse->GetMessageHead() != NULL)
                {
                    App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
                }

                if (m_pPacketParse->GetMessageBody() != NULL)
                {
                    App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
                }

                if (&mb != m_pPacketParse->GetMessageHead() && &mb != m_pPacketParse->GetMessageBody())
                {
                    App_MessageBlockManager::instance()->Close(&mb);
                }

                SAFE_DELETE(m_pPacketParse);

                Close(2);
                return;
            }
        }

        if (m_pPacketParse->GetMessageHead() != NULL)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
        }

        if (m_pPacketParse->GetMessageBody() != NULL)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
        }

        SAFE_DELETE(m_pPacketParse);
        m_pPacketParse = new CConsolePacketParse();

        Close();

        //接受下一个数据包
        if (false == RecvClinetPacket(CONSOLE_PACKET_MAX_SIZE))
        {
            OUR_DEBUG((LM_INFO, "[CProConsoleHandle::handle_read_stream]RecvClinetPacket error.\n"));
        }
    }

    return;
}

void CProConsoleHandle::handle_write_stream(const ACE_Asynch_Write_Stream::Result& result)
{
    if(!result.success() || result.bytes_transferred()==0)
    {
        //链接断开
        OUR_DEBUG ((LM_DEBUG,"[CConnectHandler::handle_write_stream] Connectid=[%d] begin(%d)...\n",GetConnectID(), errno));

        //AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);

        OUR_DEBUG((LM_DEBUG,"[CConnectHandler::handle_write_stream] Connectid=[%d] finish ok...\n", GetConnectID()));
        m_atvOutput = ACE_OS::gettimeofday();
        App_MessageBlockManager::instance()->Close(&result.message_block());
        Close();
        return;
    }
    else
    {
        //短读不考虑
        m_blCanWrite = true;
        m_atvOutput = ACE_OS::gettimeofday();
        App_MessageBlockManager::instance()->Close(&result.message_block());
        m_u4AllSendSize += (uint32)result.bytes_to_write();
        Close();
        return;
    }
}

uint8 CProConsoleHandle::GetConnectState()
{
    return m_u1ConnectState;
}

uint8 CProConsoleHandle::GetSendBuffState()
{
    return m_u1SendBuffState;
}

bool CProConsoleHandle::GetIsClosing()
{
    return m_blTimeClose;
}

bool CProConsoleHandle::SendMessage(IBuffPacket* pBuffPacket, uint8 u1OutputType)
{
    CConsolePacketParse PacketParse;

    if(NULL == pBuffPacket)
    {
        //OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::SendMessage] Connectid=[%d] pBuffPacket is NULL.\n", GetConnectID()));
        Close();
        return false;
    }

    ACE_Message_Block* pMbData = NULL;

    if (0 == u1OutputType)
    {
        int nSendLength = PacketParse.MakePacketLength(GetConnectID(), pBuffPacket->GetPacketLen());
        pMbData = App_MessageBlockManager::instance()->Create(nSendLength);

        //这里组成返回数据包
        PacketParse.MakePacket(GetConnectID(), pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pMbData);
    }
    else
    {
        pMbData = App_MessageBlockManager::instance()->Create(pBuffPacket->GetPacketLen());
        memcpy_safe((char* )pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pMbData->wr_ptr(), pBuffPacket->GetPacketLen());
        pMbData->wr_ptr(pBuffPacket->GetPacketLen());
    }

    App_BuffPacketManager::instance()->Delete(pBuffPacket);

    if (false == PutSendPacket(pMbData))
    {
        OUR_DEBUG((LM_INFO, "[CProConsoleHandle::SendMessage]PutSendPacket error.\n"));
    }

    return true;
}

bool CProConsoleHandle::PutSendPacket(ACE_Message_Block* pMbData)
{
    //int nSendSize = m_u4AllSendSize;

    m_ThreadWriteLock.acquire();
    m_nIOCount++;
    m_ThreadWriteLock.release();
    //OUR_DEBUG ((LM_ERROR, "[CConnectHandler::PutSendPacket] Connectid=%d, m_nIOCount=%d!\n", GetConnectID(), m_nIOCount));

    //异步发送方法
    if(NULL != pMbData)
    {
        if(0 != m_Writer.write(*pMbData, pMbData->length()))
        {
            OUR_DEBUG ((LM_ERROR, "[CConnectHandler::PutSendPacket] Connectid=%d mb=%d m_writer.write error(%d)!\n", GetConnectID(),  pMbData->length(), errno));
            App_MessageBlockManager::instance()->Close(pMbData);
            Close();
            return false;
        }
        else
        {
            m_u4AllSendCount += 1;
            m_atvOutput      = ACE_OS::gettimeofday();
            return true;
        }
    }
    else
    {
        OUR_DEBUG ((LM_ERROR,"[CConnectHandler::PutSendPacket] Connectid=%d mb is NULL!\n", GetConnectID()));
        Close();
        return false;
    }
}

bool CProConsoleHandle::RecvClinetPacket(uint32 u4PackeLen)
{
    m_ThreadWriteLock.acquire();
    m_nIOCount++;
    m_ThreadWriteLock.release();
    //OUR_DEBUG((LM_ERROR, "[CProConsoleHandle::RecvClinetPacket]Connectid=%d, m_nIOCount=%d.\n", GetConnectID(), m_nIOCount));

    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(u4PackeLen);

    if(pmb == NULL)
    {
        //AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
        OUR_DEBUG((LM_ERROR, "[CProConsoleHandle::RecvClinetPacket] pmb new is NULL.\n"));

        if(m_pPacketParse->GetMessageHead() != NULL)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
        }

        if(m_pPacketParse->GetMessageBody() != NULL)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
        }

        SAFE_DELETE(m_pPacketParse);
        Close(2);
        return false;
    }

    if(m_Reader.read(*pmb, u4PackeLen) == -1)
    {
        //如果读失败，则关闭连接。
        //AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
        OUR_DEBUG((LM_ERROR, "[CProConsoleHandle::RecvClinetPacket] m_reader.read is error(%d)(%d).\n", GetConnectID(), errno));

        App_MessageBlockManager::instance()->Close(pmb);

        if(m_pPacketParse->GetMessageHead() != NULL)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
        }

        if(m_pPacketParse->GetMessageBody() != NULL)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
        }

        SAFE_DELETE(m_pPacketParse);
        Close(2);
        return false;
    }

    return true;
}

bool CProConsoleHandle::CheckMessage()
{
    if(m_pPacketParse->GetMessageHead() != NULL && m_pPacketParse->GetMessageBody() != NULL)
    {
        m_u4AllRecvSize += (uint32)m_pPacketParse->GetMessageHead()->length() + (uint32)m_pPacketParse->GetMessageBody()->length();
        m_u4AllRecvCount++;

        //发送回复信息
        IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create();

        //将数据Buff放入消息体中，传递给处理类。
        uint8 u1Output = 0;
        uint32 u4Return = (uint32)App_ConsoleManager::instance()->Dispose(m_pPacketParse->GetMessageBody(), pBuffPacket, u1Output);

        if(CONSOLE_MESSAGE_SUCCESS == u4Return)
        {
            if(pBuffPacket->GetPacketLen() > 0)
            {
                if (false == SendMessage(dynamic_cast<IBuffPacket*>(pBuffPacket), u1Output))
                {
                    OUR_DEBUG((LM_INFO, "[CProConsoleHandle::CheckMessage]SendMessage error.\n"));
                }
            }
        }
        else if(CONSOLE_MESSAGE_FAIL == u4Return)
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
    else
    {
        OUR_DEBUG((LM_ERROR, "[CProConsoleHandle::CheckMessage] ConnectID = %d, m_pPacketParse is NULL.\n", GetConnectID()));
    }

    return true;
}
