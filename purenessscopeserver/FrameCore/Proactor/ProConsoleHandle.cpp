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
        PSS_LOGGER_DEBUG("[CProConsoleHandle::Close] Close({0}) delete OK.", GetConnectID());

        delete this;
    }
}

bool CProConsoleHandle::ServerClose()
{
    PSS_LOGGER_DEBUG("[CProConsoleHandle::ServerClose] Close({0}) delete OK.", GetConnectID());

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
    m_atvConnect      = CTimeStamp::Get_Time_Stamp();
    m_atvInput        = CTimeStamp::Get_Time_Stamp();
    m_atvOutput       = CTimeStamp::Get_Time_Stamp();
    m_atvSendAlive    = CTimeStamp::Get_Time_Stamp();

    m_u4AllRecvCount    = 0;
    m_u4AllSendCount    = 0;
    m_u4AllRecvSize     = 0;
    m_u4AllSendSize     = 0;
    m_blCanWrite        = true;
    m_blTimeClose       = false;
    m_u4RecvPacketCount = 0;
    m_nIOCount          = 1;

    this->handle(h);

    if(this->m_Reader.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor()) == -1 ||
       this->m_Writer.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor()) == -1)
    {
        PSS_LOGGER_DEBUG("[CProConsoleHandle::open] m_reader or m_reader == 0.");
        Close();
        return;
    }

    //ACE_Sig_Action writeAction((ACE_SignalHandler)SIG_IGN);
    //writeAction.register_action(SIGPIPE, 0);

    //int nTecvBuffSize = MAX_MSG_SOCKETBUFF;
    //ACE_OS::setsockopt(this->get_handle(), SOL_SOCKET, SO_RCVBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));
    //ACE_OS::setsockopt(h, SOL_SOCKET, SO_SNDBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));

    m_u1ConnectState = CONNECTSTATE::CONNECT_OPEN;

    PSS_LOGGER_DEBUG("[CProConsoleHandle::open] Open({0}).", GetConnectID());

    m_pPacketParse = std::make_shared<CConsolePacketParse>();

    if(nullptr == m_pPacketParse)
    {
        PSS_LOGGER_DEBUG("[CProConsoleHandle::open] Open({0}) m_pPacketParse new error.", GetConnectID());
        Close();
        return;
    }

    if (false == RecvClinetPacket(CONSOLE_PACKET_MAX_SIZE))
    {
        PSS_LOGGER_DEBUG("[CProConsoleHandle::open]RecvClinetPacket error.");
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
            App_MessageBlockManager::instance()->Close(&mb);
        }

        PSS_LOGGER_DEBUG("[CConnectHandler::handle_read_stream]Connectid=[{0}] error({1})...", GetConnectID(), errno);

        Close(2);

        return;
    }

    m_atvInput = CTimeStamp::Get_Time_Stamp();

    const char* pData = mb.rd_ptr();
    uint32 u4Len = (uint32)mb.length();

    //判断命令最后一个是不是一个\n
    if (pData[u4Len - 1] != '&' && pData[u4Len - 1] != 0x0d && pData[u4Len - 1] != 0x0a)
    {
        //短读 继续读
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

            PSS_LOGGER_DEBUG("[CProConsoleHandle::handle_input]Read Shoter error({0}).", errno);

            Close(2);
            return;
        }
    }
    else
    {
        //完整命令包
        ACE_Message_Block* pmbHead = App_MessageBlockManager::instance()->Create(sizeof(int));

        if (nullptr != pmbHead)
        {
            //组装包头
            memcpy_safe(pmbHead->wr_ptr(), sizeof(int), (char* )&u4Len, sizeof(int));
            pmbHead->wr_ptr(sizeof(int));
            m_pPacketParse->SetPacketHead(GetConnectID(), pmbHead, App_MessageBlockManager::instance());
            //组装包体
            m_pPacketParse->SetPacketBody(GetConnectID(), &mb, App_MessageBlockManager::instance());

            if (false == CheckMessage())
            {
                PSS_LOGGER_DEBUG("[CProConsoleHandle::handle_input]CheckMessage error.");

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

        Close();

        //接受下一个数据包
        if (false == RecvClinetPacket(CONSOLE_PACKET_MAX_SIZE))
        {
            PSS_LOGGER_DEBUG("[CProConsoleHandle::handle_read_stream]RecvClinetPacket error.");
        }
    }

    return;
}

void CProConsoleHandle::handle_write_stream(const ACE_Asynch_Write_Stream::Result& result)
{
    if(!result.success() || result.bytes_transferred()==0)
    {
        //链接断开
        PSS_LOGGER_DEBUG("[CConnectHandler::handle_write_stream] Connectid=[{0}] write({1})...",GetConnectID(), errno);
        m_atvOutput = CTimeStamp::Get_Time_Stamp();
        App_MessageBlockManager::instance()->Close(&result.message_block());
        Close();
        return;
    }
    else
    {
        //短读不考虑
        m_blCanWrite = true;
        m_atvOutput = CTimeStamp::Get_Time_Stamp();
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

bool CProConsoleHandle::SendMessage(shared_ptr<IBuffPacket> pBuffPacket, uint8 u1OutputType)
{
    ACE_Message_Block* pMbData = nullptr;

    if (false == Console_Common_SendMessage_Data_Check(GetConnectID(), pBuffPacket, u1OutputType, pMbData))
    {
        Close();
        return false;
    }

    if (false == PutSendPacket(pMbData))
    {
        PSS_LOGGER_DEBUG("[CProConsoleHandle::SendMessage]PutSendPacket error.");
    }

    return true;
}

bool CProConsoleHandle::PutSendPacket(ACE_Message_Block* pMbData)
{
    m_ThreadWriteLock.acquire();
    m_nIOCount++;
    m_ThreadWriteLock.release();

    //异步发送方法
    if(nullptr != pMbData)
    {
        if(0 != m_Writer.write(*pMbData, pMbData->length()))
        {
            PSS_LOGGER_DEBUG("[CConnectHandler::PutSendPacket] Connectid={0} mb={1} m_writer.write error({2})!", GetConnectID(),  pMbData->length(), errno);
            App_MessageBlockManager::instance()->Close(pMbData);
            Close();
            return false;
        }
        else
        {
            m_u4AllSendCount += 1;
            m_atvOutput      = CTimeStamp::Get_Time_Stamp();
            return true;
        }
    }
    else
    {
        PSS_LOGGER_DEBUG("[CConnectHandler::PutSendPacket] Connectid=%d mb is nullptr!", GetConnectID());
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
        PSS_LOGGER_DEBUG("[CProConsoleHandle::RecvClinetPacket] pmb new is nullptr.");

        if(m_pPacketParse->GetMessageHead() != nullptr)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
        }

        if(m_pPacketParse->GetMessageBody() != nullptr)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
        }

        Close(2);
        return false;
    }

    if(m_Reader.read(*pmb, u4PackeLen) == -1)
    {
        //如果读失败，则关闭连接。
        PSS_LOGGER_DEBUG("[CProConsoleHandle::RecvClinetPacket] m_reader.read is error({0})({1}).", GetConnectID(), errno);

        App_MessageBlockManager::instance()->Close(pmb);

        if(m_pPacketParse->GetMessageHead() != nullptr)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
        }

        if(m_pPacketParse->GetMessageBody() != nullptr)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
        }

        Close(2);
        return false;
    }

    return true;
}

bool CProConsoleHandle::CheckMessage()
{
    uint8 u1Output           = 0;
    shared_ptr<IBuffPacket> pBuffPacket = nullptr;
    bool blRet = Console_Common_CheckMessage_Data(m_u4AllRecvSize, m_u4AllRecvCount, m_pPacketParse, u1Output, pBuffPacket);

    m_pPacketParse->Close();

    if (true == blRet && false == SendMessage(pBuffPacket, u1Output))
    {
        PSS_LOGGER_DEBUG("[CProConsoleHandle::CheckMessage]SendMessage error.");
    }

    return true;
}
