#include "ProConnectClient.h"
#include "ClientProConnectManager.h"

CProConnectClient::CProConnectClient(void)
{
    m_nIOCount          = 1;
    m_nServerID         = 0;
    m_mbRecv            = NULL;
    m_pClientMessage    = NULL;
    m_u4MaxPacketSize   = MAX_MSG_PACKETLENGTH;
    m_ems2s             = S2S_NEED_CALLBACK;

    m_u4SendSize        = 0;
    m_u4SendCount       = 0;
    m_u4RecvSize        = 0;
    m_u4RecvCount       = 0;
    m_u4CostTime        = 0;

    m_emRecvState       = SERVER_RECV_INIT;
    m_emDispose         = CONNECT_IO_PLUGIN;
}

CProConnectClient::~CProConnectClient(void)
{
}

void CProConnectClient::Close()
{
    m_ThreadWritrLock.acquire();

    if(m_nIOCount > 0)
    {
        m_nIOCount--;
    }

    m_ThreadWritrLock.release();

    if(m_nIOCount == 0)
    {
        m_Reader.cancel();
        m_Writer.cancel();

        if(this->handle() != ACE_INVALID_HANDLE)
        {
            ACE_OS::closesocket(this->handle());
            this->handle(ACE_INVALID_HANDLE);
        }

        if (CONNECT_IO_FRAME == m_emDispose)
        {
            //发送packetParse断开消息
            _ClientIPInfo objClientIPInfo;
            _ClientIPInfo objLocalIPInfo;
            App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Connect(m_nServerID,
                    objClientIPInfo,
                    objLocalIPInfo);

            //发送框架消息
            Send_MakePacket_Queue(m_nServerID, m_u4PacketParseInfoID, NULL, PACKET_SERVER_TCP_DISCONNECT, m_AddrRemote, "127.0.0.1", 0, CONNECT_IO_SERVER_TCP);
        }

        App_ClientProConnectManager::instance()->CloseByClient(m_nServerID);

        OUR_DEBUG((LM_DEBUG, "[CProConnectClient::Close]delete OK[0x%08x], m_ems2s=%d.\n", this, m_ems2s));
        delete this;
    }
}

void CProConnectClient::ClientClose(EM_s2s& ems2s)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    if(this->handle() != ACE_INVALID_HANDLE)
    {
        m_ems2s = ems2s;

        //如果对象已经在外面释放，则不需要再次回调
        if(ems2s == S2S_INNEED_CALLBACK)
        {
            SetClientMessage(NULL);
        }

        ACE_OS::shutdown(this->handle(), SD_SEND);

        //ACE_OS::closesocket(this->handle());
        //this->handle(ACE_INVALID_HANDLE);
    }
}

void CProConnectClient::SetServerID(int nServerID)
{
    m_nServerID = nServerID;
}

void CProConnectClient::SetPacketParseInfoID(uint32 u4PacketParseInfoID)
{
    m_emDispose           = CONNECT_IO_FRAME;
    m_u4PacketParseInfoID = u4PacketParseInfoID;
}

void CProConnectClient::SetClientMessage(IClientMessage* pClientMessage)
{
    m_pClientMessage = pClientMessage;
}

int CProConnectClient::GetServerID()
{
    return m_nServerID;
}

void CProConnectClient::open(ACE_HANDLE h, ACE_Message_Block&)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //从配置文件获取数据
    m_u4MaxPacketSize  = GetXmlConfigAttribute(xmlRecvInfo)->RecvBuffSize;

    m_nIOCount = 1;
    this->handle(h);

    if(this->m_Reader.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_POSTDEFINE)) == -1||this->m_Writer.open(*this, h,  0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_POSTDEFINE)) == -1)
    {
        OUR_DEBUG((LM_DEBUG,"[CProConnectClient::open] m_reader or m_reader == 0.\n"));
        Close();
        return;
    }

    m_u4SendSize        = 0;
    m_u4SendCount       = 0;
    m_u4RecvSize        = 0;
    m_u4RecvCount       = 0;
    m_u4CostTime        = 0;
    m_atvBegin          = ACE_OS::gettimeofday();

    OUR_DEBUG((LM_DEBUG,"[CProConnectClient::open] m_nServerID=%d, this=0x%08x.\n", m_nServerID, this));

    if (CONNECT_IO_FRAME == m_emDispose)
    {
        //发送packetParse断开消息
        _ClientIPInfo objClientIPInfo;
        _ClientIPInfo objLocalIPInfo;
        App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Connect(m_nServerID,
                objClientIPInfo,
                objLocalIPInfo);

        //发送框架消息
        Send_MakePacket_Queue(m_nServerID, m_u4PacketParseInfoID, NULL, PACKET_SERVER_TCP_CONNECT, m_AddrRemote, "127.0.0.1", 0, CONNECT_IO_SERVER_TCP);
    }

    if (false == RecvData(GetXmlConfigAttribute(xmlConnectServer)->Recvbuff, NULL))
    {
        OUR_DEBUG((LM_DEBUG, "[CProConnectClient::open](%d)GetConnectServerRecvBuffer is error.\n", m_nServerID));
    }
}

void CProConnectClient::handle_read_stream(const ACE_Asynch_Read_Stream::Result& result)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    ACE_Message_Block& mb = result.message_block();
    uint32 u4PacketLen = (uint32)result.bytes_transferred();

    if (!result.success() || u4PacketLen == 0)
    {
        mb.release();

        if (NULL != m_pClientMessage)
        {
            _ClientIPInfo objServerIPInfo;
            sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrRemote.get_host_addr());
            objServerIPInfo.m_nPort = m_AddrRemote.get_port_number();

            //这里只处理远端服务器断开连接的消息，回调ConnectError
            //服务器主动关闭不在回调ConnectError
            if (S2S_NEED_CALLBACK == m_ems2s)
            {
                m_pClientMessage->ConnectError((int)ACE_OS::last_error(), objServerIPInfo);
            }
        }

        Close();
        return;
    }
    else
    {
        //处理接收数据(这里不区分是不是完整包，交给上层逻辑自己去判定)
        if (CONNECT_IO_FRAME == m_emDispose)
        {
            _Packet_Parse_Info* pPacketParse = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID);

            if (NULL != pPacketParse)
            {
                _Packet_Info obj_Packet_Info;
                uint8 n1Ret = pPacketParse->Parse_Packet_Stream(m_nServerID,
                              &mb,
                              dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()),
                              &obj_Packet_Info,
                              CONNECT_IO_SERVER_TCP);

                if (PACKET_GET_ENOUGTH == n1Ret)
                {

                    //发送消息给消息框架
                    CPacketParse* pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);
                    pPacketParse->SetPacket_Head_Message(obj_Packet_Info.m_pmbHead);
                    pPacketParse->SetPacket_Body_Message(obj_Packet_Info.m_pmbBody);
                    pPacketParse->SetPacket_CommandID(obj_Packet_Info.m_u2PacketCommandID);
                    pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4HeadSrcLen);
                    pPacketParse->SetPacket_Head_Curr_Length(obj_Packet_Info.m_u4HeadCurrLen);
                    pPacketParse->SetPacket_Body_Src_Length(obj_Packet_Info.m_u4BodySrcLen);
                    pPacketParse->SetPacket_Body_Curr_Length(obj_Packet_Info.m_u4BodyCurrLen);

                    //发送框架消息
                    Send_MakePacket_Queue(m_nServerID, m_u4PacketParseInfoID, pPacketParse, PACKET_PARSE, m_AddrRemote, "127.0.0.1", 0, CONNECT_IO_SERVER_TCP);

                    //清理用完的m_pPacketParse
                    App_PacketParsePool::instance()->Delete(pPacketParse);
                }
            }
        }
        else
        {

            if (NULL != m_pClientMessage)
            {
                _ClientIPInfo objServerIPInfo;
                sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrRemote.get_host_addr());
                objServerIPInfo.m_nPort = m_AddrRemote.get_port_number();
                //m_pClientMessage->RecvData(&mb, objServerIPInfo);

                //这里处理一下是不是完整包
                uint16 u2CommandID = 0;
                ACE_Message_Block* pRecvFinish = NULL;

                m_atvRecv = ACE_OS::gettimeofday();
                m_emRecvState = SERVER_RECV_BEGIN;

                while (true)
                {
                    EM_PACKET_ROUTE em_PacketRoute = PACKET_ROUTE_SELF;
                    bool blRet = m_pClientMessage->Recv_Format_data(&mb, App_MessageBlockManager::instance(), u2CommandID, pRecvFinish, em_PacketRoute);

                    if (true == blRet)
                    {
                        if (PACKET_ROUTE_SELF == em_PacketRoute)
                        {
                            //直接调用插件内注册的对象处理数据
                            Recv_Common_Dispose_Client_Message(u2CommandID, pRecvFinish, objServerIPInfo, m_pClientMessage);
                        }
                        else
                        {
                            //将数据放回到工作消息线程
                            SendMessageGroup(u2CommandID, pRecvFinish);
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }

            m_emRecvState = SERVER_RECV_END;

            //如果有剩余数据，放入数据包里面去
            if (mb.length() > 0)
            {
                ACE_Message_Block* pmbSave = App_MessageBlockManager::instance()->Create((uint32)mb.length());

                if (NULL != pmbSave)
                {
                    memcpy_safe(pmbSave->wr_ptr(), (uint32)mb.length(), mb.rd_ptr(), (uint32)mb.length());
                    pmbSave->wr_ptr(mb.length());

                    if (false == RecvData(GetXmlConfigAttribute(xmlConnectServer)->Recvbuff, pmbSave))
                    {
                        OUR_DEBUG((LM_INFO, "[CProConnectClient::handle_read_stream](%d)RecvData is fail.\n", m_nServerID));
                    }
                }
            }
            else
            {
                if (false == RecvData(GetXmlConfigAttribute(xmlConnectServer)->Recvbuff, NULL))
                {
                    OUR_DEBUG((LM_INFO, "[CProConnectClient::handle_read_stream](%d)RecvData is fail.\n", m_nServerID));
                }
            }

            mb.release();
        }
    }
}

void CProConnectClient::handle_write_stream(const ACE_Asynch_Write_Stream::Result& result)
{
    ACE_Message_Block& mblk = result.message_block();

    if(!result.success() || result.bytes_transferred() == 0)
    {
        //处理数据发送出错
        OUR_DEBUG((LM_DEBUG, "[CProConnectClient::handle_write_stream]Write error(%d).\n", ACE_OS::last_error()));
        mblk.release();

        if(NULL != m_pClientMessage)
        {
            _ClientIPInfo objServerIPInfo;
            sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrRemote.get_host_addr());
            objServerIPInfo.m_nPort = m_AddrRemote.get_port_number();
            m_pClientMessage->ConnectError((int)ACE_OS::last_error(), objServerIPInfo);
        }

        Close();
    }
    else
    {
        mblk.release();
    }
}

void CProConnectClient::addresses(const ACE_INET_Addr& remote_address, const ACE_INET_Addr& local_address)
{
    m_AddrRemote = remote_address;
}

bool CProConnectClient::GetTimeout(ACE_Time_Value tvNow)
{
    ACE_Time_Value tvIntval(tvNow - m_atvRecv);

    if(m_emRecvState == SERVER_RECV_BEGIN && tvIntval.sec() > SERVER_RECV_TIMEOUT)
    {
        //接收数据处理已经超时，在这里打印出来
        OUR_DEBUG((LM_DEBUG,"[CProConnectClient::GetTimeout]***(%d)recv dispose is timeout(%d)!***.\n", m_nServerID, tvIntval.sec()));
        return false;
    }
    else
    {
        return true;
    }
}

bool CProConnectClient::RecvData(uint32 u4PacketLen, ACE_Message_Block* pmbSave)
{
    //ACE_NEW_NORETURN(m_mbRecv, ACE_Message_Block(u4PacketLen));
    m_mbRecv = App_MessageBlockManager::instance()->Create(u4PacketLen);

    if (NULL != pmbSave && 0 < pmbSave->length())
    {
        //如果有剩余数据，则直接加进去。
        memcpy_safe(m_mbRecv->wr_ptr(), (uint32)pmbSave->length(), pmbSave->rd_ptr(), (uint32)pmbSave->length());
        m_mbRecv->wr_ptr(pmbSave->length());
        App_MessageBlockManager::instance()->Close(pmbSave);
    }

    if(this->m_Reader.read(*m_mbRecv, u4PacketLen) == -1)
    {
        OUR_DEBUG((LM_DEBUG,"[CProConnectClient::open] m_reader is error(%d).\n", (int)ACE_OS::last_error()));
        App_MessageBlockManager::instance()->Close(m_mbRecv);

        if(NULL != m_pClientMessage)
        {
            _ClientIPInfo objServerIPInfo;
            sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrRemote.get_host_addr());
            objServerIPInfo.m_nPort = m_AddrRemote.get_port_number();

            if(S2S_NEED_CALLBACK == m_ems2s)
            {
                m_pClientMessage->ConnectError((int)ACE_OS::last_error(), objServerIPInfo);
            }
        }

        Close();
        return false;
    }

    return true;
}

int CProConnectClient::SendMessageGroup(uint16 u2CommandID, ACE_Message_Block* pmblk)
{
    //组织数据
    return Make_Common_Dispose_Client_WorkTread_Message(u2CommandID, (uint32)GetServerID(), pmblk, m_AddrRemote);
}

bool CProConnectClient::SendData(ACE_Message_Block* pmblk)
{
    //OUR_DEBUG((LM_DEBUG, "[CProConnectClient::SendData]Begin.\n"));

    //如果是DEBUG状态，记录当前接受包的二进制数据
    if(GetXmlConfigAttribute(xmlServerType)->Debug == DEBUG_ON)
    {
        char szDebugData[MAX_BUFF_1024] = {'\0'};
        char szLog[10]  = {'\0'};
        int  nDebugSize = 0;
        bool blblMore   = false;

        if(pmblk->length() >= MAX_BUFF_200)
        {
            nDebugSize = MAX_BUFF_200;
            blblMore   = true;
        }
        else
        {
            nDebugSize = (int)pmblk->length();
        }

        char* pData = pmblk->rd_ptr();

        for(int i = 0; i < nDebugSize; i++)
        {
            sprintf_safe(szLog, 10, "0x%02X ", (unsigned char)pData[i]);
            sprintf_safe(szDebugData + 5*i, MAX_BUFF_1024 - 5*i, "%s", szLog);
        }

        if(blblMore == true)
        {
            AppLogManager::instance()->WriteLog(LOG_SYSTEM_DEBUG_SERVERSEND, "[%s:%d]%s.(数据包过长只记录前200字节)", m_AddrRemote.get_host_addr(), m_AddrRemote.get_port_number(), szDebugData);
        }
        else
        {
            AppLogManager::instance()->WriteLog(LOG_SYSTEM_DEBUG_SERVERSEND, "[%s:%d]%s.", m_AddrRemote.get_host_addr(), m_AddrRemote.get_port_number(), szDebugData);
        }
    }

    //发送信息
    int nLen = (int)pmblk->length();

    if (m_Writer.write(*pmblk, pmblk->length()) == -1)
    {
        OUR_DEBUG((LM_DEBUG,"[CProConnectClient::SendData] Send Error(%d).\n", ACE_OS::last_error()));
        App_MessageBlockManager::instance()->Close(pmblk);

        if(NULL != m_pClientMessage)
        {
            _ClientIPInfo objServerIPInfo;
            sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrRemote.get_host_addr());
            objServerIPInfo.m_nPort = m_AddrRemote.get_port_number();
            m_pClientMessage->ConnectError((int)ACE_OS::last_error(), objServerIPInfo);
        }

        return false;
    }

    m_u4SendSize += (uint32)nLen;
    m_u4SendCount++;

    return true;
}

_ClientConnectInfo CProConnectClient::GetClientConnectInfo()
{
    _ClientConnectInfo ClientConnectInfo;
    ClientConnectInfo.m_blValid       = true;
    ClientConnectInfo.m_u4AliveTime   = (uint32)(ACE_OS::gettimeofday().sec() - m_atvBegin.sec());
    ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
    ClientConnectInfo.m_u4RecvCount   = m_u4RecvCount;
    ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
    ClientConnectInfo.m_u4SendCount   = m_u4SendCount;
    ClientConnectInfo.m_u4ConnectID   = m_nServerID;
    ClientConnectInfo.m_u4BeginTime   = (uint32)m_atvBegin.sec();
    return ClientConnectInfo;
}

