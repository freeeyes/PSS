#include "ProConnectClient.h"
#include "ClientProConnectManager.h"

CProConnectClient::CProConnectClient(void)
{
    m_nIOCount          = 1;
    m_nServerID         = 0;
    m_mbRecv            = nullptr;
    m_pClientMessage    = nullptr;
    m_u4MaxPacketSize   = MAX_MSG_PACKETLENGTH;
    m_ems2s             = EM_s2s::S2S_NEED_CALLBACK;

    m_u4SendSize        = 0;
    m_u4SendCount       = 0;
    m_u4RecvSize        = 0;
    m_u4RecvCount       = 0;
    m_u4CostTime        = 0;

    m_emRecvState       = EM_Server_Recv_State::SERVER_RECV_INIT;
    m_emDispose         = EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN;
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

        if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
        {
            //发送packetParse断开消息
            _ClientIPInfo objClientIPInfo;
            _ClientIPInfo objLocalIPInfo;
            App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Connect(m_nServerID,
                    objClientIPInfo,
                    objLocalIPInfo);

            //发送框架消息
			_MakePacket objMakePacket;

			objMakePacket.m_u4ConnectID     = m_nServerID;
			objMakePacket.m_pPacketParse    = nullptr;
			objMakePacket.m_u1Option        = PACKET_SERVER_TCP_DISCONNECT;
			objMakePacket.m_AddrRemote      = m_AddrRemote;
			objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
            objMakePacket.m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_SERVER_TCP;
            objMakePacket.m_tvRecv          = CTimeStamp::Get_Time_Stamp();

            Send_MakePacket_Queue(m_MakePacket, objMakePacket);
        }

        App_ClientProConnectManager::instance()->CloseByClient(m_nServerID);

        //转发接口关闭
        if ("" != m_strDeviceName)
        {
            App_ForwardManager::instance()->DisConnectRegedit(m_AddrRemote.get_host_addr(), 
                m_AddrRemote.get_port_number(), 
                ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_S2S);
            m_strDeviceName = "";
        }

        PSS_LOGGER_DEBUG("[CProConnectClient::Close]delete OK[{0}], m_ems2s={1}.", fmt::ptr(this), m_ems2s);
        delete this;
    }
}

void CProConnectClient::Close(uint32 u4ConnectID)
{
    PSS_LOGGER_DEBUG("[CProConnectClient::Close]u4ConnectID={0}.", u4ConnectID);
    Close();
}

bool CProConnectClient::SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize)
{
    ACE_UNUSED_ARG(objSendMessageInfo);
    ACE_UNUSED_ARG(u4PacketSize);

    return true;
}

bool CProConnectClient::PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend)
{
    ACE_UNUSED_ARG(u4ConnectID);
    ACE_UNUSED_ARG(pMbData);
    ACE_UNUSED_ARG(u4Size);
    ACE_UNUSED_ARG(tvSend);

    return true;
}

void CProConnectClient::SetIsLog(bool blIsLog)
{
    ACE_UNUSED_ARG(blIsLog);
}

void CProConnectClient::ClientClose(EM_s2s& ems2s)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    if(this->handle() != ACE_INVALID_HANDLE)
    {
        m_ems2s = ems2s;

        //如果对象已经在外面释放，则不需要再次回调
        if(ems2s == EM_s2s::S2S_INNEED_CALLBACK)
        {
            SetClientMessage(nullptr);
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
    m_emDispose           = EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME;
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

    if(this->m_Reader.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor()) == -1||this->m_Writer.open(*this, h,  0, App_ProactorManager::instance()->GetAce_Proactor()) == -1)
    {
        PSS_LOGGER_DEBUG("[CProConnectClient::open] m_reader or m_reader == 0.");
        Close();
        return;
    }

    m_u4SendSize        = 0;
    m_u4SendCount       = 0;
    m_u4RecvSize        = 0;
    m_u4RecvCount       = 0;
    m_u4CostTime        = 0;
    m_atvBegin          = CTimeStamp::Get_Time_Stamp();

    PSS_LOGGER_DEBUG("[CProConnectClient::open] m_nServerID={0}, this={1}.", m_nServerID, fmt::ptr(this));

    if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
    {
        //发送packetParse断开消息
        _ClientIPInfo objClientIPInfo;
        _ClientIPInfo objLocalIPInfo;
        App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Connect(m_nServerID,
                objClientIPInfo,
                objLocalIPInfo);

        //发送框架消息
		_MakePacket objMakePacket;

		objMakePacket.m_u4ConnectID     = m_nServerID;
		objMakePacket.m_pPacketParse    = nullptr;
		objMakePacket.m_u1Option        = PACKET_SERVER_TCP_CONNECT;
		objMakePacket.m_AddrRemote      = m_AddrRemote;
		objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
        objMakePacket.m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_SERVER_TCP;
        objMakePacket.m_tvRecv          = m_atvRecv;

        Send_MakePacket_Queue(m_MakePacket, objMakePacket);
    }

    m_strDeviceName = App_ForwardManager::instance()->ConnectRegedit(m_AddrRemote.get_host_addr(),
        m_AddrRemote.get_port_number(),
        ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_S2S,
        dynamic_cast<IHandler*>(this));

    if (false == RecvData(GetXmlConfigAttribute(xmlConnectServer)->Recvbuff, nullptr))
    {
        PSS_LOGGER_DEBUG("[CProConnectClient::open]({0})GetConnectServerRecvBuffer is error.", m_nServerID);
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

        if (nullptr != m_pClientMessage)
        {
            _ClientIPInfo objServerIPInfo;
            objServerIPInfo.m_strClientIP = m_AddrRemote.get_host_addr();
            objServerIPInfo.m_u2Port = m_AddrRemote.get_port_number();

            //这里只处理远端服务器断开连接的消息，回调ConnectError
            //服务器主动关闭不在回调ConnectError
            if (EM_s2s::S2S_NEED_CALLBACK == m_ems2s)
            {
                m_pClientMessage->ConnectError((int)ACE_OS::last_error(), objServerIPInfo);
            }
        }

        Close();
        return;
    }
    else
    {
        //查看是否需要转发
        if ("" != m_strDeviceName)
        {
            App_ForwardManager::instance()->SendData(m_strDeviceName, &mb);
        }

        //处理接收数据(这里不区分是不是完整包，交给上层逻辑自己去判定)
        if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
        {
            auto pPacketParseInfo = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID);

            if (nullptr != pPacketParseInfo)
            {
                _Packet_Info obj_Packet_Info;
                uint8 n1Ret = pPacketParseInfo->Parse_Packet_Stream(m_nServerID,
                              &mb,
                              dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()),
                              &obj_Packet_Info,
                              EM_CONNECT_IO_TYPE::CONNECT_IO_SERVER_TCP);

                if (PACKET_GET_ENOUGH == n1Ret)
                {

                    //发送消息给消息框架
                    auto pPacketParse = std::make_shared<CPacketParse>();
                    pPacketParse->SetPacket_Head_Message(obj_Packet_Info.m_pmbHead);
                    pPacketParse->SetPacket_Body_Message(obj_Packet_Info.m_pmbBody);
                    pPacketParse->SetPacket_CommandID(obj_Packet_Info.m_u2PacketCommandID);
                    pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4HeadSrcLen);
                    pPacketParse->SetPacket_Head_Curr_Length(obj_Packet_Info.m_u4HeadCurrLen);
                    pPacketParse->SetPacket_Body_Src_Length(obj_Packet_Info.m_u4BodySrcLen);
                    pPacketParse->SetPacket_Body_Curr_Length(obj_Packet_Info.m_u4BodyCurrLen);

                    //发送框架消息
                    _MakePacket objMakePacket;

					objMakePacket.m_u4ConnectID     = m_nServerID;
					objMakePacket.m_pPacketParse    = pPacketParse;
					objMakePacket.m_u1Option        = PACKET_PARSE;
					objMakePacket.m_AddrRemote      = m_AddrRemote;
					objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
                    objMakePacket.m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_SERVER_TCP;
                    objMakePacket.m_tvRecv          = m_atvRecv;

                    Send_MakePacket_Queue(m_MakePacket, objMakePacket);
                }
                else if (PACKET_GET_ERROR == n1Ret)
                {
                    // 数据包解析错误，断开连接
                    Close();
                    return;
                }

                //等待下一个数据
                if (false == RecvData(GetXmlConfigAttribute(xmlConnectServer)->Recvbuff, nullptr))
                {
                    PSS_LOGGER_DEBUG("[CProConnectClient::handle_read_stream]({0})RecvData is fail.", m_nServerID);
                }
            }
        }
        else
        {

            if (nullptr != m_pClientMessage)
            {
                _ClientIPInfo objServerIPInfo;
                objServerIPInfo.m_strClientIP = m_AddrRemote.get_host_addr();
                objServerIPInfo.m_u2Port = m_AddrRemote.get_port_number();
                //m_pClientMessage->RecvData(&mb, objServerIPInfo);

                //这里处理一下是不是完整包
                uint16 u2CommandID = 0;
                ACE_Message_Block* pRecvFinish = nullptr;

                m_atvRecv = CTimeStamp::Get_Time_Stamp();
                m_emRecvState = EM_Server_Recv_State::SERVER_RECV_BEGIN;

                while (true)
                {
                    EM_PACKET_ROUTE em_PacketRoute = EM_PACKET_ROUTE::PACKET_ROUTE_SELF;
                    bool blRet = m_pClientMessage->Recv_Format_data(&mb, App_MessageBlockManager::instance(), u2CommandID, pRecvFinish, em_PacketRoute);

                    if (true == blRet)
                    {
                        if (EM_PACKET_ROUTE::PACKET_ROUTE_SELF == em_PacketRoute)
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

            m_emRecvState = EM_Server_Recv_State::SERVER_RECV_END;

            //如果有剩余数据，放入数据包里面去
            if (mb.length() > 0)
            {
                ACE_Message_Block* pmbSave = App_MessageBlockManager::instance()->Create((uint32)mb.length());

                if (nullptr != pmbSave)
                {
                    memcpy_safe(pmbSave->wr_ptr(), (uint32)mb.length(), mb.rd_ptr(), (uint32)mb.length());
                    pmbSave->wr_ptr(mb.length());

                    if (false == RecvData(GetXmlConfigAttribute(xmlConnectServer)->Recvbuff, pmbSave))
                    {
                        PSS_LOGGER_DEBUG("[CProConnectClient::handle_read_stream]({0})RecvData is fail.", m_nServerID);
                    }
                }
            }
            else
            {
                if (false == RecvData(GetXmlConfigAttribute(xmlConnectServer)->Recvbuff, nullptr))
                {
                    PSS_LOGGER_DEBUG("[CProConnectClient::handle_read_stream]({0})RecvData is fail.", m_nServerID);
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
        PSS_LOGGER_DEBUG("[CProConnectClient::handle_write_stream]Write error({0}).", ACE_OS::last_error());
        mblk.release();

        if(nullptr != m_pClientMessage)
        {
            _ClientIPInfo objServerIPInfo;
            objServerIPInfo.m_strClientIP = m_AddrRemote.get_host_addr();
            objServerIPInfo.m_u2Port = m_AddrRemote.get_port_number();
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

bool CProConnectClient::Device_Send_Data(const char* pData, ssize_t nLen)
{
    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create((uint32)nLen);
    memcpy_safe((char* )pData, (uint32)nLen, pmb->wr_ptr(), (uint32)nLen);
    pmb->wr_ptr(nLen);

    return SendData(pmb);
}

bool CProConnectClient::GetTimeout(PSS_Time_Point const& tvNow)
{
    auto time_interval_millisecond = CTimeStamp::Get_Time_Difference(tvNow, m_atvRecv);
    auto time_interval_second = time_interval_millisecond / 1000;

    if(m_emRecvState == EM_Server_Recv_State::SERVER_RECV_BEGIN && time_interval_second > SERVER_RECV_TIMEOUT)
    {
        //接收数据处理已经超时，在这里打印出来
        PSS_LOGGER_DEBUG("[CProConnectClient::GetTimeout]***({0})recv dispose is timeout({1})!***.", m_nServerID, time_interval_second);
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

    if (nullptr != pmbSave && 0 < pmbSave->length())
    {
        //如果有剩余数据，则直接加进去。
        memcpy_safe(m_mbRecv->wr_ptr(), (uint32)pmbSave->length(), pmbSave->rd_ptr(), (uint32)pmbSave->length());
        m_mbRecv->wr_ptr(pmbSave->length());
        App_MessageBlockManager::instance()->Close(pmbSave);
    }

    if(this->m_Reader.read(*m_mbRecv, u4PacketLen) == -1)
    {
        PSS_LOGGER_DEBUG("[CProConnectClient::open] m_reader is error({0}).", ACE_OS::last_error());
        App_MessageBlockManager::instance()->Close(m_mbRecv);

        if(nullptr != m_pClientMessage)
        {
            _ClientIPInfo objServerIPInfo;
            objServerIPInfo.m_strClientIP = m_AddrRemote.get_host_addr();
            objServerIPInfo.m_u2Port = m_AddrRemote.get_port_number();

            if(EM_s2s::S2S_NEED_CALLBACK == m_ems2s)
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
    string strHexChar;     //单个十六进制的字符
    string strHexData;     //十六进制的字符串

    //如果是DEBUG状态，记录当前接受包的二进制数据
    if(GetXmlConfigAttribute(xmlServerType)->Debug == DEBUG_ON)
    {
        char szDebugData[MAX_BUFF_1024] = {'\0'};
        char szLog[10]  = {'\0'};
        uint32 u4DebugSize = 0;
        bool blblMore   = false;

        if(pmblk->length() >= MAX_BUFF_200)
        {
            u4DebugSize = MAX_BUFF_200;
            blblMore   = true;
        }
        else
        {
            u4DebugSize = (int)pmblk->length();
        }

        const char* pData = pmblk->rd_ptr();

        for (uint32 i = 0; i < u4DebugSize; i++)
        {
            std::stringstream ss_format;

            ss_format << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)pData[i] << " ";
            strHexData += ss_format.str();
        }

        if(blblMore == true)
        {
            string strLog = fmt::format("[{0}:{1}]{2}.(packet is more than 200)",
                m_AddrRemote.get_host_addr(),
                m_AddrRemote.get_port_number(),
                strHexData);
            AppLogManager::instance()->WriteLog_r(LOG_SYSTEM_DEBUG_SERVERSEND, strLog);
        }
        else
        {
            string strLog = fmt::format("[{0}:{1}]{2}.",
                m_AddrRemote.get_host_addr(),
                m_AddrRemote.get_port_number(),
                strHexData);
            AppLogManager::instance()->WriteLog_r(LOG_SYSTEM_DEBUG_SERVERSEND, strLog);
        }
    }

    //发送信息
    int nLen = (int)pmblk->length();

    if (m_Writer.write(*pmblk, pmblk->length()) == -1)
    {
        PSS_LOGGER_DEBUG("[CProConnectClient::SendData] Send Error({0}).", ACE_OS::last_error());
        App_MessageBlockManager::instance()->Close(pmblk);

        if(nullptr != m_pClientMessage)
        {
            _ClientIPInfo objServerIPInfo;
            objServerIPInfo.m_strClientIP = m_AddrRemote.get_host_addr();
            objServerIPInfo.m_u2Port = m_AddrRemote.get_port_number();
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
    ClientConnectInfo.m_u4AliveTime   = (uint32)(CTimeStamp::Get_Time_Difference(CTimeStamp::Get_Time_Stamp(), m_atvBegin));
    ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
    ClientConnectInfo.m_u4RecvCount   = m_u4RecvCount;
    ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
    ClientConnectInfo.m_u4SendCount   = m_u4SendCount;
    ClientConnectInfo.m_u4ConnectID   = m_nServerID;
    ClientConnectInfo.m_u4BeginTime   = (uint32)CTimeStamp::Get_Time_use_second(m_atvBegin);
    return ClientConnectInfo;
}

