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
    m_u1ConnectState     = CONNECTSTATE::CONNECT_INIT;
    m_u1SendBuffState    = CONNECTSTATE::CONNECT_SENDNON;
    m_pPacketParse       = nullptr;
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

    m_u1ConnectState = CONNECTSTATE::CONNECT_SERVER_CLOSE;

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

    //AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Connection from [%s:%d] DisposeTime = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), tvOpen.msec());

    this->handle(h);

    if(this->m_Reader.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor()) == -1 ||
       this->m_Writer.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor()) == -1)
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

    m_u1ConnectState = CONNECTSTATE::CONNECT_OPEN;

    OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::open] Open(%d).\n", GetConnectID()));

    m_pPacketParse = new CConsolePacketParse();

    if(nullptr == m_pPacketParse)
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
        //���ӶϿ�
        if(m_pPacketParse->GetMessageHead() != nullptr)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
        }

        if(m_pPacketParse->GetMessageBody() != nullptr)
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
        //��Ϊ��Ҫ�ر����ӣ�����Ҫ��ر�һ��IO����ӦOpen���õ�1�ĳ�ʼֵ

        Close(2);

        return;
    }

    m_atvInput = ACE_OS::gettimeofday();


    const char* pData = mb.rd_ptr();
    uint32 u4Len = (uint32)mb.length();

    //�ж��������һ���ǲ���һ��\n
    if (pData[u4Len - 1] != '&' && pData[u4Len - 1] != 0x0d && pData[u4Len - 1] != 0x0a)
    {
        //�̶� ������
        int nRead = (int)result.bytes_to_read() - (int)result.bytes_transferred();

        if (-1 == m_Reader.read(mb, nRead))
        {
            if (m_pPacketParse->GetMessageHead() != nullptr)
            {
                App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
            }

            if (m_pPacketParse->GetMessageBody() != nullptr)
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
            //��Ϊ��Ҫ�ر����ӣ�����Ҫ��ر�һ��IO����ӦOpen���õ�1�ĳ�ʼֵ

            Close(2);
            return;
        }
    }
    else
    {
        //���������
        ACE_Message_Block* pmbHead = App_MessageBlockManager::instance()->Create(sizeof(int));

        if (nullptr != pmbHead)
        {
            //��װ��ͷ
            memcpy_safe(pmbHead->wr_ptr(), sizeof(int), (char* )&u4Len, sizeof(int));
            pmbHead->wr_ptr(sizeof(int));
            m_pPacketParse->SetPacketHead(GetConnectID(), pmbHead, App_MessageBlockManager::instance());
            //��װ����
            m_pPacketParse->SetPacketBody(GetConnectID(), &mb, App_MessageBlockManager::instance());

            if (false == CheckMessage())
            {
                OUR_DEBUG((LM_INFO, "[CProConsoleHandle::handle_input]CheckMessage error.\n"));

                if (m_pPacketParse->GetMessageHead() != nullptr)
                {
                    App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
                }

                if (m_pPacketParse->GetMessageBody() != nullptr)
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

        if (m_pPacketParse->GetMessageHead() != nullptr)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
        }

        if (m_pPacketParse->GetMessageBody() != nullptr)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
        }

        SAFE_DELETE(m_pPacketParse);
        m_pPacketParse = new CConsolePacketParse();

        Close();

        //������һ�����ݰ�
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
        //���ӶϿ�
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
        //�̶�������
        m_blCanWrite = true;
        m_atvOutput = ACE_OS::gettimeofday();
        App_MessageBlockManager::instance()->Close(&result.message_block());
        m_u4AllSendSize += (uint32)result.bytes_to_write();
        Close();
        return;
    }
}

CONNECTSTATE CProConsoleHandle::GetConnectState()
{
    return m_u1ConnectState;
}

CONNECTSTATE CProConsoleHandle::GetSendBuffState()
{
    return m_u1SendBuffState;
}

bool CProConsoleHandle::GetIsClosing()
{
    return m_blTimeClose;
}

bool CProConsoleHandle::SendMessage(IBuffPacket* pBuffPacket, uint8 u1OutputType)
{
    ACE_Message_Block* pMbData = nullptr;

    if (false == Console_Common_SendMessage_Data_Check(GetConnectID(), pBuffPacket, u1OutputType, pMbData))
    {
        Close();
        return false;
    }

    if (false == PutSendPacket(pMbData))
    {
        OUR_DEBUG((LM_INFO, "[CProConsoleHandle::SendMessage]PutSendPacket error.\n"));
    }

    return true;
}

bool CProConsoleHandle::PutSendPacket(ACE_Message_Block* pMbData)
{
    m_ThreadWriteLock.acquire();
    m_nIOCount++;
    m_ThreadWriteLock.release();

    //�첽���ͷ���
    if(nullptr != pMbData)
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
        OUR_DEBUG ((LM_ERROR,"[CConnectHandler::PutSendPacket] Connectid=%d mb is nullptr!\n", GetConnectID()));
        Close();
        return false;
    }
}

bool CProConsoleHandle::RecvClinetPacket(uint32 u4PackeLen)
{
    m_ThreadWriteLock.acquire();
    m_nIOCount++;
    m_ThreadWriteLock.release();

    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(u4PackeLen);

    if(pmb == nullptr)
    {
        OUR_DEBUG((LM_ERROR, "[CProConsoleHandle::RecvClinetPacket] pmb new is nullptr.\n"));

        if(m_pPacketParse->GetMessageHead() != nullptr)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
        }

        if(m_pPacketParse->GetMessageBody() != nullptr)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
        }

        SAFE_DELETE(m_pPacketParse);
        Close(2);
        return false;
    }

    if(m_Reader.read(*pmb, u4PackeLen) == -1)
    {
        //�����ʧ�ܣ���ر����ӡ�
        OUR_DEBUG((LM_ERROR, "[CProConsoleHandle::RecvClinetPacket] m_reader.read is error(%d)(%d).\n", GetConnectID(), errno));

        App_MessageBlockManager::instance()->Close(pmb);

        if(m_pPacketParse->GetMessageHead() != nullptr)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
        }

        if(m_pPacketParse->GetMessageBody() != nullptr)
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
    uint8 u1Output           = 0;
    IBuffPacket* pBuffPacket = nullptr;
    bool blRet = Console_Common_CheckMessage_Data(m_u4AllRecvSize, m_u4AllRecvCount, m_pPacketParse, u1Output, pBuffPacket);

    if (true == blRet && false == SendMessage(dynamic_cast<IBuffPacket*>(pBuffPacket), u1Output))
    {
        OUR_DEBUG((LM_INFO, "[CProConsoleHandle::CheckMessage]SendMessage error.\n"));
    }

    return true;
}
