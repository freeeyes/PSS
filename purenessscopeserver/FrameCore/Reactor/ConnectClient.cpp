#include "ConnectClient.h"
#include "ClientReConnectManager.h"

CConnectClient::CConnectClient(void)
{
}

CConnectClient::~CConnectClient(void)
{
}

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
        OUR_DEBUG((LM_ERROR, "[CConnectClient::Close]Close(%s:%d) OK.\n", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number()));

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

            Send_MakePacket_Queue(objMakePacket);
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

void CConnectClient::ClientClose()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadLock);
    //将连接消息断开放在output去执行，这样就不需要同步加锁了。
    ACE_Message_Block* pMbData = App_MessageBlockManager::instance()->Create(sizeof(int));

    if (nullptr == pMbData)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectClient::ClientClose] Connectid=%d, pMbData is nullptr.\n", GetServerID()));
        return;
    }

    ACE_Message_Block::ACE_Message_Type objType = ACE_Message_Block::MB_STOP;
    pMbData->msg_type(objType);

    //这里主动关闭，不在要求回调连接断开消息
    SetClientMessage(nullptr);

    //将消息放入队列，让output在反应器线程发送。
    ACE_Time_Value xtime = ACE_OS::gettimeofday();

    //队列已满，不能再放进去了,就不放进去了
    if (msg_queue()->is_full() == true)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectClient::ClientClose] Connectid=%d,putq is full(%d).\n", GetServerID(), msg_queue()->message_count()));
        App_MessageBlockManager::instance()->Close(pMbData);
        return;
    }

    if (this->putq(pMbData, &xtime) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectClient::ClientClose] Connectid=%d,putq(%d) output errno = [%d].\n", GetServerID(), msg_queue()->message_count(), errno));
        App_MessageBlockManager::instance()->Close(pMbData);
    }
    else
    {
        m_u1ConnectState = CONNECTSTATE::CONNECT_CLIENT_CLOSE;
        int nWakeupRet = reactor()->schedule_wakeup(this, ACE_Event_Handler::WRITE_MASK);

        if (-1 == nWakeupRet)
        {
            OUR_DEBUG((LM_ERROR, "CConnectClient::ClientClose] Connectid=%d, nWakeupRet(%d) output errno = [%d].\n", GetServerID(), nWakeupRet, errno));
        }
    }
}

int CConnectClient::open(void* p)
{
    if (p != nullptr)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectClient::open]p is not nullptr.\n"));
    }

    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadLock);

    msg_queue()->high_water_mark(MAX_MSG_MASK);
    msg_queue()->low_water_mark(MAX_MSG_MASK);

    //从配置文件获取数据
    m_u4MaxPacketSize  = GetXmlConfigAttribute(xmlRecvInfo)->RecvBuffSize;

    ACE_Time_Value nowait(MAX_MSG_PACKETTIMEOUT);
    m_nIOCount = 1;

    //设置链接为非阻塞模式
    if (this->peer().enable(ACE_NONBLOCK) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]this->peer().enable  = ACE_NONBLOCK error.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectClient::open]this->peer().enable  = ACE_NONBLOCK error.");
        return -1;
    }

    //获得远程链接地址和端口
    if (this->peer().get_remote_addr(m_addrRemote) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]this->peer().get_remote_addr error.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectClient::open]this->peer().get_remote_addr error.");
        return -1;
    }

    m_u4SendSize        = 0;
    m_u4SendCount       = 0;
    m_u4RecvSize        = 0;
    m_u4RecvCount       = 0;
    m_u4CostTime        = 0;
    m_atvBegin          = ACE_OS::gettimeofday();
    m_u4CurrSize        = 0;
    //申请当前的MessageBlock
    m_pCurrMessage = App_MessageBlockManager::instance()->Create(GetXmlConfigAttribute(xmlConnectServer)->Recvbuff);

    if (m_pCurrMessage == nullptr)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectClient::RecvClinetPacket] pmb new is nullptr.\n"));
        return -1;
    }

    m_u1ConnectState = CONNECTSTATE::CONNECT_OPEN;

    OUR_DEBUG((LM_INFO, "[CConnectClient::open] Connection from [%s:%d]\n", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number()));

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

        Send_MakePacket_Queue(objMakePacket);
    }

    m_strDeviceName = App_ForwardManager::instance()->ConnectRegedit(m_addrRemote.get_host_addr(),
        m_addrRemote.get_port_number(),
        ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_S2S,
        dynamic_cast<IDeviceHandler*>(this));

    int nRet = this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK);

    int nWakeupRet = reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);

    if (-1 == nWakeupRet)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectClient::open]ConnectID=%d, nWakeupRet=%d, errno=%d.\n", GetServerID(), nWakeupRet, errno));
    }

    return nRet;
}

int CConnectClient::handle_input(ACE_HANDLE fd)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadLock);

    if (fd == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectClient::handle_input]fd == ACE_INVALID_HANDLE.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::handle_input]fd == ACE_INVALID_HANDLE.");

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
        OUR_DEBUG((LM_ERROR, "[CConnectClient::handle_input]m_pCurrMessage == nullptr.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectClient::handle_input]m_pCurrMessage == nullptr.");

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

    int nCurrCount = (uint32)m_pCurrMessage->size();

    int nDataLen = (int)this->peer().recv(m_pCurrMessage->wr_ptr(), nCurrCount, MSG_NOSIGNAL, &nowait);

    if (nDataLen <= 0)
    {
        m_u4CurrSize = 0;
        uint32 u4Error = (uint32)errno;
        OUR_DEBUG((LM_ERROR, "[CConnectClient::handle_input] ConnectID = %d, recv data is error nDataLen = [%d] errno = [%d].\n", GetServerID(), nDataLen, u4Error));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectClient::handle_input] ConnectID = %d, recv data is error[%d].\n", GetServerID(), nDataLen);

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
        m_pCurrMessage->reset();
        return 0;
    }

    //如果是DEBUG状态，记录当前接受包的二进制数据
    Output_Debug_Data(m_pCurrMessage, LOG_SYSTEM_DEBUG_SERVERRECV);

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
        const _Packet_Parse_Info* pPacketParseInfo = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID);

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
                CPacketParse* pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);
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

                Send_MakePacket_Queue(objMakePacket);

                //清理用完的m_pPacketParse
                App_PacketParsePool::instance()->Delete(pPacketParse);

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

        m_atvRecv = ACE_OS::gettimeofday();
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

void CConnectClient::Output_Debug_Data(const ACE_Message_Block* pMbData, uint16 u2LogType, bool blLog) const
{
    string strHexChar;     //单个十六进制的字符
    string strHexData;     //十六进制的字符串

    if (GetXmlConfigAttribute(xmlServerType)->Debug == DEBUG_ON || false == blLog)
    {
        int nDataLen = (int)pMbData->length();
        uint32 u4DebugSize = 0;
        bool blblMore = false;

        if ((uint32)nDataLen >= MAX_BUFF_200)
        {
            u4DebugSize = MAX_BUFF_200 - 1;
            blblMore = true;
        }
        else
        {
            u4DebugSize = (uint32)nDataLen;
        }

        const char* pData = pMbData->rd_ptr();

        for (uint32 i = 0; i < u4DebugSize; i++)
        {
            std::stringstream ss_format;

            ss_format << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)pData[i] << " ";
            strHexData += ss_format.str();
        }

        if (blblMore == true)
        {
            string strLog = fmt::format("[{0}:{1}]{2}.(packet is more than 200)",
                m_addrRemote.get_host_addr(),
                m_addrRemote.get_port_number(),
                strHexData);
            AppLogManager::instance()->WriteLog_r(u2LogType, strLog);
        }
        else
        {
            string strLog = fmt::format("[{0}:{1}]{2}.",
                m_addrRemote.get_host_addr(),
                m_addrRemote.get_port_number(),
                strHexData);
            AppLogManager::instance()->WriteLog_r(u2LogType, strLog);
        }
    }
}

int CConnectClient::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
    if (h == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_DEBUG, "[CConnectClient::handle_close] h is nullptr mask=%d.\n", GetServerID(), (int)mask));
    }

    Close();
    return 0;
}

int CConnectClient::handle_output(ACE_HANDLE fd /*= ACE_INVALID_HANDLE*/)
{
    if (fd == ACE_INVALID_HANDLE)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConnectClient::handle_output]fd == ACE_INVALID_HANDLE.\n"));
        return -1;
    }

    ACE_Message_Block* pmbSendData = nullptr;
    ACE_Time_Value nowait(ACE_OS::gettimeofday());

    while (-1 != this->getq(pmbSendData, &nowait))
    {
        if (nullptr == pmbSendData)
        {
            OUR_DEBUG((LM_INFO, "[CConnectClient::handle_output]ConnectID=%d pmbSendData is nullptr.\n", GetServerID()));
            break;
        }

        //如果是断开指令，则执行连接断开操作
        if (pmbSendData->msg_type() == ACE_Message_Block::MB_STOP)
        {
            return -1;
        }

        //发送数据
        const char* pData = pmbSendData->rd_ptr();

        if (nullptr == pData)
        {
            OUR_DEBUG((LM_ERROR, "[CConnectClient::SendData] ConnectID = %d, pData is nullptr.\n", GetServerID()));
            App_MessageBlockManager::instance()->Close(pmbSendData);
            return -1;
        }

        int nSendLen = (int)pmbSendData->length();   //发送数据的总长度
        int nIsSendSize = 0;

        //循环发送，直到数据发送完成。
        while (true)
        {
            if (nSendLen <= 0)
            {
                OUR_DEBUG((LM_ERROR, "[CConnectClient::SendData] ConnectID = %d, nCurrSendSize error is %d.\n", GetServerID(), nSendLen));
                App_MessageBlockManager::instance()->Close(pmbSendData);
                return -1;
            }

            int nDataLen = (int)this->peer().send(pmbSendData->rd_ptr(), nSendLen - nIsSendSize, &nowait);

            if (nDataLen <= 0)
            {
                _ClientIPInfo objServerIPInfo;
                objServerIPInfo.m_strClientIP = m_addrRemote.get_host_addr();
                objServerIPInfo.m_u2Port      = m_addrRemote.get_port_number();
                m_pClientMessage->ConnectError(ACE_OS::last_error(), objServerIPInfo);

                OUR_DEBUG((LM_ERROR, "[CConnectClient::SendData] ConnectID = %d, error = %d.\n", GetServerID(), errno));
                App_MessageBlockManager::instance()->Close(pmbSendData);
                return -1;
            }
            else if (nDataLen + nIsSendSize >= nSendLen)  //当数据包全部发送完毕，清空。
            {
                App_MessageBlockManager::instance()->Close(pmbSendData);
                m_u4SendSize += (uint32)nSendLen;
                m_u4SendCount++;
                break;
            }
            else
            {
                pmbSendData->rd_ptr(nDataLen);
                nIsSendSize += nDataLen;
            }
        }

    }

    int nWakeupRet = reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);

    if (-1 == nWakeupRet)
    {
        OUR_DEBUG((LM_INFO, "[CConnectClient::handle_output]ConnectID=%d,nWakeupRet=%d, errno=%d.\n", GetServerID(), nWakeupRet, errno));
    }

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
        OUR_DEBUG((LM_ERROR, "[CConnectClient::SendData] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetServerID()));
        return false;
    }

    //如果是DEBUG状态，记录当前接受包的二进制数据
    Output_Debug_Data(pmblk, LOG_SYSTEM_DEBUG_CLIENTSEND);

    ACE_Time_Value     nowait(MAX_MSG_PACKETTIMEOUT);

    if (get_handle() == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectClient::SendData] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetServerID()));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectClient::SendData] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetServerID());
        App_MessageBlockManager::instance()->Close(pmblk);
        return false;
    }

    //将消息放入队列，让output在反应器线程发送。
    ACE_Time_Value xtime = ACE_OS::gettimeofday();

    //队列已满，不能再放进去了,就不放进去了
    if (msg_queue()->is_full() == true)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectClient::SendMessage] Connectid=%d,putq is full(%d).\n", GetServerID(), msg_queue()->message_count()));
        App_MessageBlockManager::instance()->Close(pmblk);
        return false;
    }

    if (this->putq(pmblk, &xtime) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectClient::SendMessage] Connectid=%d,putq(%d) output errno = [%d].\n", GetServerID(), msg_queue()->message_count(), errno));
        App_MessageBlockManager::instance()->Close(pmblk);
    }
    else
    {
        int nWakeupRet = reactor()->schedule_wakeup(this, ACE_Event_Handler::WRITE_MASK);

        if (-1 == nWakeupRet)
        {
            OUR_DEBUG((LM_ERROR, "[CConnectClient::SendMessage] Connectid=%d, nWakeupRet(%d) output errno = [%d].\n", GetServerID(), nWakeupRet, errno));
        }
    }

    return true;
}

_ClientConnectInfo CConnectClient::GetClientConnectInfo() const
{
    _ClientConnectInfo ClientConnectInfo;
    ClientConnectInfo.m_blValid       = true;
    ClientConnectInfo.m_addrRemote    = m_addrRemote;
    ClientConnectInfo.m_u4AliveTime   = (uint32)(ACE_OS::gettimeofday().sec() - m_atvBegin.sec());
    ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
    ClientConnectInfo.m_u4RecvCount   = m_u4RecvCount;
    ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
    ClientConnectInfo.m_u4SendCount   = m_u4SendCount;
    ClientConnectInfo.m_u4ConnectID   = m_nServerID;
    ClientConnectInfo.m_u4BeginTime   = (uint32)m_atvBegin.sec();
    return ClientConnectInfo;
}

bool CConnectClient::GetTimeout(ACE_Time_Value const& tvNow) const
{
    ACE_Time_Value tvIntval(tvNow - m_atvRecv);

    if(m_emRecvState == EM_Server_Recv_State::SERVER_RECV_BEGIN && tvIntval.sec() > SERVER_RECV_TIMEOUT)
    {
        //接收数据处理已经超时，在这里打印出来
        OUR_DEBUG((LM_DEBUG,"[CConnectClient::GetTimeout]***(%d)recv dispose is timeout(%d)!***.\n", m_nServerID, tvIntval.sec()));
        return false;
    }
    else
    {
        return true;
    }
}


