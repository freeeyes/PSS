#include "ConnectClient.h"
#include "ClientReConnectManager.h"

void CConnectClient::Close()
{
    m_ThreadLock.acquire();

    if (m_nIOCount > 0)
    {
        m_nIOCount--;
    }

    m_ThreadLock.release();

    //从反应器注销事件
    if (m_nIOCount == 0)
    {
        shutdown();
        PSS_LOGGER_DEBUG("[CConnectClient::Close]Close({0}:{1}) OK.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number());

        //删除链接对象
        App_ClientReConnectManager::instance()->CloseByClient(m_nServerID);

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
			objMakePacket.m_AddrRemote      = m_addrRemote;
			objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
            objMakePacket.m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_SERVER_TCP;
            objMakePacket.m_tvRecv          = CTimeStamp::Get_Time_Stamp();

            Send_MakePacket_Queue(m_MakePacket, objMakePacket);
        }

        //转发接口关闭
        if ("" != m_strDeviceName)
        {
            App_ForwardManager::instance()->DisConnectRegedit(m_addrRemote.get_host_addr(), 
                m_addrRemote.get_port_number(), 
                ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_S2S);
            m_strDeviceName = "";
        }

        //回归用过的指针
        delete this;
    }

}

void CConnectClient::Close(uint32 u4ConnectID)
{
    PSS_LOGGER_DEBUG("[CProConnectClient::Close]u4ConnectID={0}.", u4ConnectID);
    Close();
}

bool CConnectClient::SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize)
{
    ACE_UNUSED_ARG(objSendMessageInfo);
    ACE_UNUSED_ARG(u4PacketSize);

    return true;
}

bool CConnectClient::PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend)
{
    ACE_UNUSED_ARG(u4ConnectID);
    ACE_UNUSED_ARG(pMbData);
    ACE_UNUSED_ARG(u4Size);
    ACE_UNUSED_ARG(tvSend);

    return true;
}

void CConnectClient::SetIsLog(bool blIsLog)
{
    ACE_UNUSED_ARG(blIsLog);
}

void CConnectClient::ClientClose()
{
    //这里主动关闭，不在要求回调连接断开消息
    SetClientMessage(nullptr);

    PSS_LOGGER_DEBUG("[CConnectClient::ClientClose]Begin.");
    Close();
    PSS_LOGGER_DEBUG("[CConnectClient::ClientClose]End.");
}

int CConnectClient::open(void* p)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadLock);
    PSS_UNUSED_ARG(p);

    msg_queue()->high_water_mark(MAX_MSG_MASK);
    msg_queue()->low_water_mark(MAX_MSG_MASK);

    //从配置文件获取数据
    m_u4MaxPacketSize  = GetXmlConfigAttribute(xmlRecvInfo)->RecvBuffSize;

    ACE_Time_Value nowait(MAX_MSG_PACKETTIMEOUT);
    m_nIOCount = 1;

    //设置链接为非阻塞模式
    if (this->peer().enable(ACE_NONBLOCK) == -1)
    {
        PSS_LOGGER_DEBUG("[CConnectHandler::open]this->peer().enable  = ACE_NONBLOCK error.");
        m_strError = "[CConnectClient::open]this->peer().enable  = ACE_NONBLOCK error.";
        return -1;
    }

    //获得远程链接地址和端口
    if (this->peer().get_remote_addr(m_addrRemote) == -1)
    {
        PSS_LOGGER_DEBUG("[CConnectHandler::open]this->peer().get_remote_addr error.");
        m_strError = "[CConnectClient::open]this->peer().get_remote_addr error.";
        return -1;
    }

    m_u4SendSize        = 0;
    m_u4SendCount       = 0;
    m_u4RecvSize        = 0;
    m_u4RecvCount       = 0;
    m_u4CostTime        = 0;
    m_atvBegin          = CTimeStamp::Get_Time_Stamp();
    m_u4CurrSize        = 0;
    //申请当前的MessageBlock
    m_pCurrMessage = App_MessageBlockManager::instance()->Create(GetXmlConfigAttribute(xmlConnectServer)->Recvbuff);

    if (m_pCurrMessage == nullptr)
    {
        PSS_LOGGER_DEBUG("[CConnectClient::RecvClinetPacket] pmb new is nullptr.");
        return -1;
    }

    m_u1ConnectState = CONNECTSTATE::CONNECT_OPEN;

    PSS_LOGGER_DEBUG("[CConnectClient::open] Connection from [{0}:{1}] ServerID={3}.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_nServerID);

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

		objMakePacket.m_u4ConnectID = m_nServerID;
		objMakePacket.m_pPacketParse = nullptr;
		objMakePacket.m_u1Option = PACKET_SERVER_TCP_CONNECT;
		objMakePacket.m_AddrRemote = m_addrRemote;
		objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
		objMakePacket.m_emPacketType = EM_CONNECT_IO_TYPE::CONNECT_IO_SERVER_TCP;
        objMakePacket.m_tvRecv = m_atvRecv;

        Send_MakePacket_Queue(m_MakePacket, objMakePacket);
    }

    m_strDeviceName = App_ForwardManager::instance()->ConnectRegedit(m_addrRemote.get_host_addr(),
        m_addrRemote.get_port_number(),
        ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_S2S,
        dynamic_cast<IHandler*>(this));

    int nRet = this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK);

    int nWakeupRet = reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);

    if (-1 == nWakeupRet)
    {
        PSS_LOGGER_DEBUG("[CConnectClient::open]ConnectID={0}, nWakeupRet={1}, errno={2}.", GetServerID(), nWakeupRet, errno);
    }

    return nRet;
}

int CConnectClient::handle_input(ACE_HANDLE fd)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadLock);

    if (fd == ACE_INVALID_HANDLE)
    {
        PSS_LOGGER_DEBUG("[CConnectClient::handle_input]fd == ACE_INVALID_HANDLE.");
        m_strError = "[CConnectHandler::handle_input]fd == ACE_INVALID_HANDLE.";

        if (nullptr != m_pClientMessage)
        {
            _ClientIPInfo objServerIPInfo;
            objServerIPInfo.m_strClientIP = m_addrRemote.get_host_addr();
            objServerIPInfo.m_u2Port = m_addrRemote.get_port_number();

            if(EM_s2s::S2S_NEED_CALLBACK == m_ems2s)
            {
                m_pClientMessage->ConnectError(ACE_OS::last_error(), objServerIPInfo);
            }
        }

        return -1;
    }

    //判断缓冲是否为nullptr
    if (m_pCurrMessage == nullptr)
    {
        m_u4CurrSize = 0;
        PSS_LOGGER_DEBUG("[CConnectClient::handle_input]m_pCurrMessage == nullptr.");
        m_strError = "[CConnectClient::handle_input]m_pCurrMessage == nullptr.";
             
        if (nullptr != m_pClientMessage)
        {
            _ClientIPInfo objServerIPInfo;
            objServerIPInfo.m_strClientIP = m_addrRemote.get_host_addr();
            objServerIPInfo.m_u2Port      = m_addrRemote.get_port_number();

            if(EM_s2s::S2S_NEED_CALLBACK == m_ems2s)
            {
                m_pClientMessage->ConnectError(ACE_OS::last_error(), objServerIPInfo);
            }
        }

        return -1;
    }

    return RecvData();
}

int CConnectClient::RecvData()
{
    ACE_Time_Value nowait(MAX_MSG_PACKETTIMEOUT);

    auto nCurrCount = (uint32)m_pCurrMessage->size();

    auto nDataLen = (int)this->peer().recv(m_pCurrMessage->wr_ptr(), nCurrCount, MSG_NOSIGNAL, &nowait);

    if (nDataLen <= 0)
    {
        m_u4CurrSize = 0;
        auto u4Error = (uint32)errno;
        PSS_LOGGER_DEBUG("[CConnectClient::handle_input] ConnectID = {0}, recv data is error nDataLen = [{1}] errno = [{2}].", GetServerID(), nDataLen, u4Error);
        m_strError = fmt::format("[CConnectClient::handle_input] ConnectID = {0}, recv data is error{1}.", GetServerID(), nDataLen);

        _ClientIPInfo objServerIPInfo;
        objServerIPInfo.m_strClientIP = m_addrRemote.get_host_addr();
        objServerIPInfo.m_u2Port      = m_addrRemote.get_port_number();

        if(EM_s2s::S2S_NEED_CALLBACK == m_ems2s)
        {
            m_pClientMessage->ConnectError(ACE_OS::last_error(), objServerIPInfo);
        }

        return -1;
    }

    //查看是否需要转发
    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->SendData(m_strDeviceName, m_pCurrMessage);
    }

    //如果是DEBUG状态，记录当前接受包的二进制数据
    Output_Debug_Data(m_pCurrMessage, LOG_SYSTEM_DEBUG_SERVERRECV, m_addrRemote);

    m_pCurrMessage->wr_ptr(nDataLen);

    Dispose_Recv_Data(m_pCurrMessage);

    return 0;
}

int CConnectClient::SendMessageGroup(uint16 u2CommandID, ACE_Message_Block* pmblk) const
{
    //组织数据
    return Make_Common_Dispose_Client_WorkTread_Message(u2CommandID, (uint32)GetServerID(), pmblk, m_addrRemote);
}

int CConnectClient::Dispose_Recv_Data(ACE_Message_Block* pCurrMessage)
{
    if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
    {
        auto pPacketParseInfo = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID);

        if (nullptr != pPacketParseInfo)
        {
            _Packet_Info obj_Packet_Info;
            uint8 n1Ret = pPacketParseInfo->Parse_Packet_Stream(m_nServerID,
                          pCurrMessage,
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
				objMakePacket.m_AddrRemote      = m_addrRemote;
				objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
				objMakePacket.m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_SERVER_TCP;
                objMakePacket.m_tvRecv          = m_atvRecv;

                Send_MakePacket_Queue(m_MakePacket, objMakePacket);

                m_emRecvState = EM_Server_Recv_State::SERVER_RECV_END;
                m_pCurrMessage->reset();
            }
            else if (PACKET_GET_ERROR == n1Ret)
            {
                // 数据包解析错误，断开连接
                Close();
                return 0;
            }
        }
    }
    else
    {
        //接收数据，返回给逻辑层，自己不处理整包完整性判定
        _ClientIPInfo objServerIPInfo;
        objServerIPInfo.m_strClientIP = m_addrRemote.get_host_addr();
        objServerIPInfo.m_u2Port      = m_addrRemote.get_port_number();

        uint16 u2CommandID = 0;
        ACE_Message_Block* pRecvFinish = nullptr;

        m_atvRecv = CTimeStamp::Get_Time_Stamp();
        m_emRecvState = EM_Server_Recv_State::SERVER_RECV_BEGIN;
        EM_PACKET_ROUTE em_PacketRoute = EM_PACKET_ROUTE::PACKET_ROUTE_SELF;

        while (true)
        {
            bool blRet = m_pClientMessage->Recv_Format_data(pCurrMessage, App_MessageBlockManager::instance(), u2CommandID, pRecvFinish, em_PacketRoute);

            if (false == blRet)
            {
                break;
            }

            if (EM_PACKET_ROUTE::PACKET_ROUTE_SELF == em_PacketRoute)
            {
                //直接调用插件内注册的对象处理数据
                Recv_Common_Dispose_Client_Message(u2CommandID, pRecvFinish, objServerIPInfo, m_pClientMessage);
            }
            else
            {
                //将数据放回到消息线程
                SendMessageGroup(u2CommandID, pRecvFinish);
            }
        }

		m_emRecvState = EM_Server_Recv_State::SERVER_RECV_END;
		m_pCurrMessage->reset();
    }

    return 0;
}

int CConnectClient::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
    if (h == ACE_INVALID_HANDLE)
    {
        PSS_LOGGER_DEBUG("[CConnectClient::handle_close] h({0}) is nullptr mask={1}.", GetServerID(), (int)mask);
    }

    Close();
    return 0;
}

bool CConnectClient::Device_Send_Data(const char* pData, ssize_t nLen)
{
    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create((uint32)nLen);
    memcpy_safe(pData, (uint32)nLen, pmb->wr_ptr(), (uint32)nLen);
    pmb->wr_ptr(nLen);

    return SendData(pmb);
}

void CConnectClient::SetClientMessage(IClientMessage* pClientMessage)
{
    m_pClientMessage = pClientMessage;
}

void CConnectClient::SetServerID(int nServerID)
{
    m_nServerID = nServerID;
}

int CConnectClient::GetServerID() const
{
    return m_nServerID;
}

void CConnectClient::SetPacketParseInfoID(uint32 u4PacketParseInfoID)
{
    m_u4PacketParseInfoID = u4PacketParseInfoID;
}

bool CConnectClient::SendData(ACE_Message_Block* pmblk)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadLock);

    if (CONNECTSTATE::CONNECT_CLIENT_CLOSE == m_u1ConnectState || CONNECTSTATE::CONNECT_SERVER_CLOSE == m_u1ConnectState)
    {
        //连接已经进入关闭流程，不在接受发送数据。
        App_MessageBlockManager::instance()->Close(pmblk);
        return false;
    }

    if (nullptr == pmblk)
    {
        PSS_LOGGER_DEBUG("[CConnectClient::SendData] ConnectID = {0}, get_handle() == ACE_INVALID_HANDLE.", GetServerID());
        return false;
    }

    //如果是DEBUG状态，记录当前接受包的二进制数据
    Output_Debug_Data(pmblk, LOG_SYSTEM_DEBUG_CLIENTSEND, m_addrRemote);

    auto nSendLen = (int)pmblk->length();   //发送数据的总长度
    int nIsSendSize = 0;

    //循环发送，直到数据发送完成。
    while (true)
    {
        if (nSendLen <= 0)
        {
            PSS_LOGGER_DEBUG("[CConnectClient::SendData] ConnectID = {0}, nCurrSendSize error is {1].", GetServerID(), nSendLen);
            App_MessageBlockManager::instance()->Close(pmblk);
            return false;
        }

        ACE_Time_Value nowait(ACE_OS::gettimeofday());
        auto nDataLen = (int)this->peer().send(pmblk->rd_ptr(), pmblk->length(), &nowait);

        if (nDataLen <= 0)
        {
            _ClientIPInfo objServerIPInfo;
            objServerIPInfo.m_strClientIP = m_addrRemote.get_host_addr();
            objServerIPInfo.m_u2Port = m_addrRemote.get_port_number();
            m_pClientMessage->ConnectError(ACE_OS::last_error(), objServerIPInfo);

            PSS_LOGGER_DEBUG("[CConnectClient::SendData] ConnectID = {0}, error = {1}.", GetServerID(), errno);
            App_MessageBlockManager::instance()->Close(pmblk);
            return false;
        }
        else if (nDataLen + nIsSendSize >= nSendLen)  //当数据包全部发送完毕，清空。
        {
            App_MessageBlockManager::instance()->Close(pmblk);
            m_u4SendSize += (uint32)nSendLen;
            m_u4SendCount++;
            break;
        }
        else
        {
            pmblk->rd_ptr(nDataLen);
            nIsSendSize += nDataLen;
        }
    }

    return true;
}

_ClientConnectInfo CConnectClient::GetClientConnectInfo() const
{
    _ClientConnectInfo ClientConnectInfo;
    ClientConnectInfo.m_blValid       = true;
    ClientConnectInfo.m_addrRemote    = m_addrRemote;
    ClientConnectInfo.m_u4AliveTime   = (uint32)(CTimeStamp::Get_Time_Difference(CTimeStamp::Get_Time_Stamp(), m_atvBegin));
    ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
    ClientConnectInfo.m_u4RecvCount   = m_u4RecvCount;
    ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
    ClientConnectInfo.m_u4SendCount   = m_u4SendCount;
    ClientConnectInfo.m_u4ConnectID   = m_nServerID;
    ClientConnectInfo.m_u4BeginTime   = (uint32)CTimeStamp::Get_Time_use_second(m_atvBegin);
    return ClientConnectInfo;
}

bool CConnectClient::GetTimeout(PSS_Time_Point const& tvNow) const
{
    ACE_Time_Value tvIntval(tvNow - m_atvRecv);

    auto time_interval_millisecond = CTimeStamp::Get_Time_Difference(CTimeStamp::Get_Time_Stamp(), m_atvBegin);
    auto time_interval_second = time_interval_millisecond / 1000;

    if(m_emRecvState == EM_Server_Recv_State::SERVER_RECV_BEGIN && time_interval_second > SERVER_RECV_TIMEOUT)
    {
        //接收数据处理已经超时，在这里打印出来
        PSS_LOGGER_DEBUG("[CConnectClient::GetTimeout]***({0})recv dispose is timeout({1})!***.", m_nServerID, tvIntval.sec());
        return false;
    }
    else
    {
        return true;
    }
}


