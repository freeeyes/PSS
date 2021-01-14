#include "ConnectHandler.h"

void CConnectHandler::Close(uint32 u4ConnectID)
{
    //调用连接断开消息
    m_pPacketParseInfo->DisConnect(u4ConnectID);

    
    PSS_LOGGER_DEBUG("[CProConnectHandler::Close]({0})Close(ConnectID={1}), Recv={2},Send={3} OK.",
		fmt::ptr(this),
		GetConnectID(),
		m_u4AllRecvSize,
		m_u4AllSendSize);

    shutdown();

    //清理缓冲的PacketParse对象
    ClearPacketParse();

    //清理转发接口
    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->DisConnectRegedit(m_addrRemote.get_host_addr(), 
            m_addrRemote.get_port_number(), 
            ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_CLINET);
        m_strDeviceName = "";
    }

    //回归用过的指针
    App_ConnectHandlerPool::instance()->Delete(this);
}

void CConnectHandler::CloseFinally()
{
	if (nullptr != m_pBlockMessage)
	{
		m_pBlockMessage->release();
        m_pBlockMessage = nullptr;
	}

	if (nullptr != m_pBlockRecv)
	{
		m_pBlockRecv->release();
        m_pBlockMessage = nullptr;
	}

    this->closing_ = true;
    m_u4PacketDebugSize = 0;
}

void CConnectHandler::Init(uint16 u2HandlerID)
{
    m_u4HandlerID      = u2HandlerID;
    m_u2MaxConnectTime = GetXmlConfigAttribute(xmlClientInfo)->MaxConnectTime;
    m_u4MaxPacketSize  = GetXmlConfigAttribute(xmlRecvInfo)->RecvBuffSize;
    m_u2TcpNodelay     = GetXmlConfigAttribute(xmlSendInfo)->TcpNodelay;

    m_u4SendMaxBuffSize  = GetXmlConfigAttribute(xmlSendInfo)->MaxBlockSize;
	m_pBlockMessage      = App_MessageBlockManager::instance()->Create(m_u4SendMaxBuffSize);
	m_pBlockRecv         = App_MessageBlockManager::instance()->Create(m_u4MaxPacketSize);

    m_u4PacketDebugSize  = GetXmlConfigAttribute(xmlServerType)->DebugSize / 5;

    m_pPacketParse = std::make_shared<CPacketParse>();
}

void CConnectHandler::SetPacketParseInfoID(uint32 u4PacketParseInfoID)
{
    m_u4PacketParseInfoID = u4PacketParseInfoID;
}

uint32 CConnectHandler::GetPacketParseInfoID() const
{
    return m_u4PacketParseInfoID;
}

uint32 CConnectHandler::GetHandlerID() const
{
    return m_u4HandlerID;
}

void CConnectHandler::SetConnectID(uint32 u4ConnectID)
{
    m_u4ConnectID = u4ConnectID;
}

uint32 CConnectHandler::GetConnectID() const
{
    return m_u4ConnectID;
}

int CConnectHandler::open(void*)
{
    m_blBlockState        = false;
    m_blIsLog             = false;

    //获得解析器指针
    m_pPacketParseInfo = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID);
    if (nullptr == m_pPacketParseInfo)
    {
        //如果解析器不存在，则直接断开连接
        PSS_LOGGER_DEBUG("[CConnectHandler::open]({0})can't find PacketParseInfo.", m_addrRemote.get_host_addr());
        return -1;
    }

    //获得远程链接地址和端口
    if(this->peer().get_remote_addr(m_addrRemote) == -1)
    {
        PSS_LOGGER_DEBUG("[CConnectHandler::open]this->peer().get_remote_addr error.");
        return -1;
    }

    if(App_ForbiddenIP::instance()->CheckIP(m_addrRemote.get_host_addr()) == false)
    {
        //在禁止列表中，不允许访问
        PSS_LOGGER_DEBUG("[CConnectHandler::open]IP Forbidden({0}).", m_addrRemote.get_host_addr());
        return -1;
    }

    //检查单位时间链接次数是否达到上限
    if(false == App_IPAccount::instance()->AddIP((string)m_addrRemote.get_host_addr()))
    {
        PSS_LOGGER_DEBUG("[CConnectHandler::open]IP connect frequently({0}).", m_addrRemote.get_host_addr());
        App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), GetXmlConfigAttribute(xmlIP)->Timeout);

        //发送告警邮件
        string strLog = fmt::format("[CConnectHandler::open]IP is more than IP Max({0}).",
            m_addrRemote.get_host_addr());

        AppLogManager::instance()->WriteToMail_r(LOG_SYSTEM_CONNECT,
            GetXmlConfigAttribute(xmlAlertConnect)->MailID,
            "Alert IP",
            strLog);

        return -1;
    }

    int nRet = 0;

    //设置链接为非阻塞模式
    if (this->peer().enable(ACE_NONBLOCK) == -1)
    {
        PSS_LOGGER_DEBUG("[CConnectHandler::open]this->peer().enable  = ACE_NONBLOCK error.");
        return -1;
    }

    //查看是否存在转发服务
    m_strDeviceName = App_ForwardManager::instance()->ConnectRegedit(m_addrRemote.get_host_addr(),
        m_addrRemote.get_port_number(),
        ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_CLINET,
        dynamic_cast<IHandler*>(this));

    //初始化参数设置
    if (-1 == Init_Open_Connect())
    {
        return -1;
    }

    ConnectOpen();

    nRet = this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK|ACE_Event_Handler::WRITE_MASK);

    int nWakeupRet = reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);

    if (-1 == nWakeupRet)
    {
        PSS_LOGGER_DEBUG("[CConnectHandler::open]ConnectID={0}, nWakeupRet={1}, errno={2}.", GetConnectID(), nWakeupRet, errno);
    }

    m_emIOType = EM_IO_TYPE::NET_INPUT;
    return nRet;
}

//接受数据
int CConnectHandler::handle_input(ACE_HANDLE fd)
{
    m_atvInput = CTimeStamp::Get_Time_Stamp();

    if(fd == ACE_INVALID_HANDLE)
    {
        m_u4CurrSize = 0;
        PSS_LOGGER_DEBUG("[CConnectHandler::handle_input]fd == ACE_INVALID_HANDLE.");
        return -1;
    }

    return Dispose_Recv_Data();

}

int CConnectHandler::handle_output(ACE_HANDLE fd /*= ACE_INVALID_HANDLE*/)
{
    if (fd == ACE_INVALID_HANDLE)
    {
        m_u4CurrSize = 0;
        PSS_LOGGER_DEBUG("[CConnectHandler::handle_output]fd == ACE_INVALID_HANDLE.");
        return -1;
    }

    return 0;
}

int CConnectHandler::Dispose_Recv_Data()
{
    ACE_Time_Value nowait(0, MAX_QUEUE_TIMEOUT);

    //计算应该接收的数据长度
    uint32 u4CurrCount = Get_Recv_length();

    //这里需要对m_u4CurrSize进行检查。
    if (u4CurrCount == 0)
    {
        PSS_LOGGER_DEBUG("[CConnectHandler::RecvData][{0}] nCurrCount < 0 m_u4CurrSize = {1}.", GetConnectID(), m_u4CurrSize);

		//清理PacketParse
		ClearPacketParse();

        return -1;
    }

    auto nDataLen = (int)this->peer().recv(m_pBlockRecv->wr_ptr(), u4CurrCount, MSG_NOSIGNAL, &nowait);

    if (nDataLen <= 0)
    {
        m_u4CurrSize = 0;
        auto u4Error = (uint32)errno;

        //如果是-1 且为11的错误，忽略之
        if (nDataLen == -1 && u4Error == EAGAIN)
        {
            return 0;
        }

        PSS_LOGGER_DEBUG("[CConnectHandler::RecvData] ConnectID={0}, recv data is error nDataLen = [{1}] errno = [{2}].", GetConnectID(), nDataLen, u4Error);

        return -1;
    }

    m_u4CurrSize += nDataLen;

    m_pBlockRecv->wr_ptr(nDataLen);

    //如果是DEBUG状态，记录当前接受包的二进制数据
    Output_Debug_Data(m_pBlockRecv, LOG_SYSTEM_DEBUG_CLIENTRECV, m_addrRemote);

    //查看是否需要转发
    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->SendData(m_strDeviceName, m_pBlockRecv);
        m_pBlockRecv->reset();
        return 0;
    }

    //处理数据切包
	if (false == Dispose_Recv_buffer())
	{
		//清理PacketParse
		ClearPacketParse();

		return -1;
	}

    return 0;
}

int CConnectHandler::Init_Open_Connect()
{
    //设置默认别名
    SetConnectName(m_addrRemote.get_host_addr());

    //初始化当前链接的某些参数
    m_atvConnect          = CTimeStamp::Get_Time_Stamp();
    m_atvInput            = m_atvConnect;
    m_atvOutput           = m_atvConnect;

    m_u4AllRecvSize       = 0;
    m_u4AllSendSize       = 0;
    m_u4CurrSize          = 0;

    if (m_u2TcpNodelay == TCP_NODELAY_OFF)
    {
        //如果设置了禁用Nagle算法，则这里要禁用。
        int nOpt = 1;
        ACE_OS::setsockopt(this->get_handle(), IPPROTO_TCP, TCP_NODELAY, (char*)&nOpt, sizeof(int));
    }

    return 0;
}

//关闭链接
int CConnectHandler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
    if(h == ACE_INVALID_HANDLE)
    {
        PSS_LOGGER_DEBUG("[CConnectHandler::handle_close] h is nullptr mask={0}.", (int)mask);
    }

    //设置发送消息队列不能再发送任何消息
    if(m_u1ConnectState != CONNECTSTATE::CONNECT_SERVER_CLOSE)
    {
        m_u1ConnectState = CONNECTSTATE::CONNECT_CLIENT_CLOSE;
    }

	//发送链接关闭消息
	Send_Hander_Event(PACKET_CDISCONNECT);

    return 0;
}

bool CConnectHandler::Device_Send_Data(const char* pData, ssize_t nLen)
{
    uint16 u2CommandID = 0x0000;

    auto pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);
    pBuffPacket->WriteStream(pData, (uint32)nLen);

	EM_SEND_PACKET_PARSE emState = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
	EM_Client_Send_Status  emSendState = EM_Client_Send_Status::CLIENT_SEND_IMMEDIATLY;
    uint32 u4PacketSize = 0;

	CSendMessageInfo objSendMessageInfo;

	objSendMessageInfo.u2CommandID = u2CommandID;
	objSendMessageInfo.pBuffPacket = pBuffPacket;
	objSendMessageInfo.emSendType  = emState;
	objSendMessageInfo.emSendState = emSendState;
	objSendMessageInfo.blDelete = true;

	return SendMessage(objSendMessageInfo, u4PacketSize);
}

uint32 CConnectHandler::file_open(const IFileTestManager* pFileTest) const
{
    //先不实现
    ACE_UNUSED_ARG(pFileTest);
    return 0;
}

int CConnectHandler::handle_write_file_stream(const char* pData, uint32 u4Size, uint8 u1ParseID) const
{
    //暂不实现
    ACE_UNUSED_ARG(pData);
    ACE_UNUSED_ARG(u4Size);
    ACE_UNUSED_ARG(u1ParseID);
    return 0;
}

CONNECTSTATE CConnectHandler::GetConnectState() const
{
    return m_u1ConnectState;
}

CONNECTSTATE CConnectHandler::GetSendBuffState() const
{
    return m_u1SendBuffState;
}

bool CConnectHandler::SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize)
{
    //发送数据
    if (EM_IO_TYPE::NET_INPUT == m_emIOType)
    {
        //如果不是直接发送数据，则拼接数据包
        if (EM_Client_Send_Status::CLIENT_SEND_CACHE == objSendMessageInfo.emSendState)
        {
            return Send_Input_To_Cache(objSendMessageInfo, u4PacketSize);
        }
        else
        {
            //要发送数据到对端
            return Send_Input_To_TCP(objSendMessageInfo, u4PacketSize);
        }
    }
    else
    {
        //文件入口，直接写入日志
        return Write_SendData_To_File(objSendMessageInfo.blDelete, objSendMessageInfo.pBuffPacket);
    }
}

bool CConnectHandler::SendTimeoutMessage()
{
	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID     = GetConnectID();
	objMakePacket.m_pPacketParse    = nullptr;
	objMakePacket.m_u1Option        = PACKET_CHEK_TIMEOUT;
	objMakePacket.m_AddrRemote      = m_addrRemote;
	objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
    objMakePacket.m_AddrListen      = m_addrLocal;

    Send_MakePacket_Queue(m_MakePacket, objMakePacket);

    return true;
}

bool CConnectHandler::PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend)
{
	ACE_Message_Block* pmbSend = App_MessageBlockManager::instance()->Create(u4Size);
	memcpy_safe(pMbData->rd_ptr(),
		u4Size,
		pmbSend->rd_ptr(),
		u4Size);

	pmbSend->wr_ptr(u4Size);

    //如果是DEBUG状态，记录当前发送包的二进制数据
    Output_Debug_Data(pmbSend, LOG_SYSTEM_DEBUG_CLIENTSEND, m_addrRemote);


    //发送超时时间设置
    ACE_Time_Value  nowait(0, MAX_BUFF_1000 * MAX_BUFF_1000);

    //发送数据
    auto nSendPacketLen = (int)pmbSend->length();
    int nIsSendSize    = 0;

    //循环发送，直到数据发送完成。
    while(true)
    {
        if(nSendPacketLen <= 0)
        {
            PSS_LOGGER_DEBUG("[CConnectHandler::PutSendPacket] ConnectID = {0}, nCurrSendSize error is {1}.",
                u4ConnectID,
                nSendPacketLen);
            App_MessageBlockManager::instance()->Close(pmbSend);
            return false;
        }

        auto nDataLen = (int)this->peer().send(pmbSend->rd_ptr(), size_t(nSendPacketLen - nIsSendSize), &nowait);

        if(nDataLen <= 0)
        {
            int nErrno = errno;
            PSS_LOGGER_DEBUG("[CConnectHandler::PutSendPacket] ConnectID = {0}, error = {1}.", u4ConnectID, nErrno);

            string strLog = fmt::format("WriteError [{0}:{1}] nErrno = {2}  result.bytes_transferred() = {3},",
                m_addrRemote.get_host_addr(), 
                m_addrRemote.get_port_number(), 
                nErrno,
                nIsSendSize);

            AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, strLog);

            //错误消息回调
            pmbSend->rd_ptr((size_t)0);
            auto tvNow = CTimeStamp::Get_Time_Stamp();
            Send_MakePacket_Queue_Error(m_MakePacket, GetConnectID(), pmbSend, tvNow);

            PSS_LOGGER_DEBUG("[CConnectHandler::PutSendPacket] ConnectID={0} send cancel.", GetConnectID());

            return false;
        }
        else if(nDataLen >= nSendPacketLen - nIsSendSize)   //当数据包全部发送完毕，清空。
        {
            m_u4AllSendSize     += (uint32)pmbSend->length();
            m_atvOutput         = tvSend;

            int nMessageID = pmbSend->msg_type() - ACE_Message_Block::MB_USER;

            if(nMessageID > 0)
            {
                //需要回调发送成功回执
                CPacketParse objPacketParse;
                ACE_Message_Block* pSendOKData = App_MessageBlockManager::instance()->Create(sizeof(int));
                memcpy_safe((char*)&nMessageID, sizeof(int), pSendOKData->wr_ptr(), sizeof(int));
                pSendOKData->wr_ptr(sizeof(int));
                objPacketParse.SetPacket_Head_Message(pSendOKData);
                objPacketParse.SetPacket_Head_Curr_Length((uint32)pSendOKData->length());

				_MakePacket objMakePacket;

				objMakePacket.m_u4ConnectID     = GetConnectID();
				objMakePacket.m_pPacketParse    = m_pPacketParse;
				objMakePacket.m_u1Option        = PACKET_SEND_OK;
				objMakePacket.m_AddrRemote      = m_addrRemote;
				objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
                objMakePacket.m_AddrListen      = m_addrLocal;

                Send_MakePacket_Queue(m_MakePacket, objMakePacket);

                //还原消息类型
                pmbSend->msg_type(ACE_Message_Block::MB_DATA);
            }

            App_MessageBlockManager::instance()->Close(pmbSend);

            return true;
        }
        else
        {
            pmbSend->rd_ptr(nDataLen);
            nIsSendSize      += nDataLen;
            m_atvOutput      = tvSend;
        }
    }
}

ENUM_WHILE_STATE CConnectHandler::Recv_Packet_Cut(bool& blRet)
{
    if (m_pPacketParseInfo->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        int nBodyLogic = 0;
        //如果是包头模式
        if (m_pBlockRecv->length() < m_pPacketParseInfo->m_u4OrgLength)
        {
            //没有收全包头，继续接收
            Move_Recv_buffer();
            blRet = true;
            return ENUM_WHILE_STATE::WHILE_STATE_BREAK;
        }
        else
        {
            //处理包头
            ACE_Message_Block* pHead = App_MessageBlockManager::instance()->Create(m_pPacketParseInfo->m_u4OrgLength);
            memcpy_safe(m_pBlockRecv->rd_ptr(),
                m_pPacketParseInfo->m_u4OrgLength,
                pHead->wr_ptr(),
                m_pPacketParseInfo->m_u4OrgLength);
            pHead->wr_ptr(m_pPacketParseInfo->m_u4OrgLength);

            if (0 != Dispose_Paceket_Parse_Head(pHead))
            {
                blRet = false;
                return ENUM_WHILE_STATE::WHILE_STATE_BREAK;
            }

            //判断处理后的数据包体长度是否包含
            uint32 u4BodyLength = m_pPacketParse->GetPacketBodySrcLen();

            //如果包体为0，则继续循环
            if (u4BodyLength == 0)
            {
                //没有包体，处理完成了一个完整的包，整体偏移一下
                m_pBlockRecv->rd_ptr(m_pPacketParseInfo->m_u4OrgLength);
                return ENUM_WHILE_STATE::WHILE_STATE_CONTINUE;
            }

            uint32 u4AllPacketLength = u4BodyLength + m_pPacketParseInfo->m_u4OrgLength;
            if (u4AllPacketLength <= (uint32)m_pBlockRecv->length())
            {
                ACE_Message_Block* pBody = App_MessageBlockManager::instance()->Create(u4BodyLength);
                memcpy_safe(m_pBlockRecv->rd_ptr() + m_pPacketParseInfo->m_u4OrgLength,
                    u4BodyLength,
                    pBody->wr_ptr(),
                    u4BodyLength);
                pBody->wr_ptr(u4BodyLength);

                nBodyLogic = Dispose_Paceket_Parse_Body(pBody, u4BodyLength);

                //处理完成了一个完整的包，整体偏移一下
                m_pBlockRecv->rd_ptr(u4AllPacketLength);
            }
            else
            {
                //没接收完全，继续接收
                //处理接收的包头结构体的回收,下一次会重新组织包头
                pHead->release();

                Move_Recv_buffer();
                blRet = true;
                return ENUM_WHILE_STATE::WHILE_STATE_BREAK;
            }

            if (0 != nBodyLogic)
            {
                blRet = false;
                return ENUM_WHILE_STATE::WHILE_STATE_BREAK;
            }
        }
    }
    else
    {
        //如果是流模式
        Dispose_Paceket_Parse_Stream(m_pBlockRecv);
        m_pBlockRecv->reset();
    }

    return ENUM_WHILE_STATE::WHILE_STATE_CONTINUE;
}

bool CConnectHandler::Dispose_Recv_buffer()
{
	bool blRet = true;

	//处理数据切包
	while (true)
	{
        auto while_State = Recv_Packet_Cut(blRet);
        if (while_State == ENUM_WHILE_STATE::WHILE_STATE_BREAK)
        {
            m_MakePacket.CommitMessageList();
            break;
        }
        else
        {
            continue;
        }
	}

	return blRet;
}

void CConnectHandler::Move_Recv_buffer()
{
	if (m_pBlockRecv->rd_ptr() != m_pBlockRecv->base() && m_pBlockRecv->length() > 0)
	{
		//移动到前面去
        auto u4RemainLength = (uint32)m_pBlockRecv->length();
		ACE_Message_Block* pBlockRemain = App_MessageBlockManager::instance()->Create(u4RemainLength);
        memcpy_safe(m_pBlockRecv->rd_ptr(),
            u4RemainLength,
            pBlockRemain->rd_ptr(),
            u4RemainLength);
        pBlockRemain->wr_ptr(u4RemainLength);
        
        m_pBlockRecv->reset();
		
		memcpy_safe(pBlockRemain->rd_ptr(),
			u4RemainLength,
            m_pBlockRecv->rd_ptr(),
			u4RemainLength);
        m_pBlockRecv->wr_ptr(u4RemainLength);

		App_MessageBlockManager::instance()->Close(pBlockRemain);
	}
	else if (m_pBlockRecv->length() == 0) //如果全部接受完成才清理
	{
		//全部重置
		m_pBlockRecv->reset();
	}
}

void CConnectHandler::Send_Hander_Event(uint8 u1Option)
{
	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID     = GetConnectID();
	objMakePacket.m_pPacketParse    = nullptr;
	objMakePacket.m_u1Option        = u1Option;
	objMakePacket.m_AddrRemote      = m_addrRemote;
	objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
	objMakePacket.m_pHandler        = this;
    objMakePacket.m_tvRecv          = m_atvInput;
    objMakePacket.m_AddrListen      = m_addrLocal;

	Send_MakePacket_Queue(m_MakePacket, objMakePacket);
}

void CConnectHandler::ConnectOpen()
{
	//告诉PacketParse连接应建立
	m_pPacketParseInfo->Connect(GetConnectID(), GetClientIPInfo(), GetLocalIPInfo());

	//发送链接建立消息
	Send_Hander_Event(PACKET_CONNECT);

    PSS_LOGGER_DEBUG("[CConnectHandler::open]Open({0}) Connection from [{1}:{2}]({3}).", GetConnectID(), m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), fmt::ptr(this));

    m_u1ConnectState = CONNECTSTATE::CONNECT_OPEN;
}

uint32 CConnectHandler::Get_Recv_length() const
{
    return (uint32)(m_pBlockRecv->size() - m_pBlockRecv->length());
}

int CConnectHandler::Dispose_Paceket_Parse_Head(ACE_Message_Block* pmbHead)
{
    _Head_Info objHeadInfo;
    bool blStateHead = m_pPacketParseInfo->Parse_Packet_Head_Info(GetConnectID(), 
        pmbHead, 
        App_MessageBlockManager::instance(), 
        &objHeadInfo, 
        EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

    if (false == blStateHead)
    {
        m_u4CurrSize = 0;
        PSS_LOGGER_DEBUG("[CConnectHandler::RecvData]SetPacketHead is false.");

        return -1;
    }
    else
    {
        if (nullptr == objHeadInfo.m_pmbHead)
        {
            PSS_LOGGER_DEBUG("[CConnectHandler::RecvData]ConnectID={0}, objHeadInfo.m_pmbHead is nullptr.", GetConnectID());
        }

        m_pPacketParse->SetPacket_IsHandleHead(false);
        m_pPacketParse->SetPacket_Head_Curr_Length(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength);
        m_pPacketParse->SetPacket_Head_Message(objHeadInfo.m_pmbHead);
        m_pPacketParse->SetPacket_Head_Curr_Length(objHeadInfo.m_u4HeadCurrLen);
        m_pPacketParse->SetPacket_Body_Src_Length(objHeadInfo.m_u4BodySrcLen);
        m_pPacketParse->SetPacket_CommandID(objHeadInfo.m_u2PacketCommandID);
        m_pPacketParse->SetPacket_Body_Message(nullptr);
    }

    uint32 u4PacketBodyLen = m_pPacketParse->GetPacketBodySrcLen();
    m_u4CurrSize = 0;

    //这里添加只处理包头的数据
    //如果数据只有包头，不需要包体，在这里必须做一些处理，让数据只处理包头就扔到DoMessage()
    if (u4PacketBodyLen == 0)
    {
        //只有数据包头
        if (false == CheckMessage())
        {
            return -1;
        }

        m_u4CurrSize = 0;
    }
    else
    {
        //如果超过了最大包长度，为非法数据
        if (u4PacketBodyLen >= m_u4MaxPacketSize)
        {
            m_u4CurrSize = 0;
            PSS_LOGGER_DEBUG("[CConnectHandler::RecvData]u4PacketHeadLen({0}) more than {1}.", u4PacketBodyLen, m_u4MaxPacketSize);

            return -1;
        }
    }

    return 0;
}

int CConnectHandler::Dispose_Paceket_Parse_Body(ACE_Message_Block* pmbBody, uint32 u4SrcBodyLength)
{
    _Body_Info obj_Body_Info;
    obj_Body_Info.m_u4BodySrcLen = u4SrcBodyLength;
    bool blStateBody = m_pPacketParseInfo->Parse_Packet_Body_Info(GetConnectID(), 
        pmbBody, 
        App_MessageBlockManager::instance(), 
        &obj_Body_Info, 
        EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

    if (false == blStateBody)
    {
        //如果数据包体是错误的，则断开连接
        m_u4CurrSize = 0;
        PSS_LOGGER_DEBUG("[CConnectHandle::RecvData]SetPacketBody is false.");

        return -1;
    }
    else
    {
        m_pPacketParse->SetPacket_Body_Message(obj_Body_Info.m_pmbBody);
        m_pPacketParse->SetPacket_Body_Curr_Length(obj_Body_Info.m_u4BodyCurrLen);

        if (obj_Body_Info.m_u2PacketCommandID > 0)
        {
            m_pPacketParse->SetPacket_CommandID(obj_Body_Info.m_u2PacketCommandID);
        }
    }

	if (false == CheckMessage())
	{
		return -1;
	}

    return 0;
}

int CConnectHandler::Dispose_Paceket_Parse_Stream(ACE_Message_Block* pCurrMessage)
{
    int nRet = 0;

    while (true)
    {
        //处理单一数据块
        nRet = Dispose_Paceket_Parse_Stream_Single(pCurrMessage);

        if (1 != nRet)
        {
            break;
        }
    }

    return nRet;
}

bool CConnectHandler::CheckMessage()
{
    if(m_pPacketParse->GetMessageBody() == nullptr)
    {
        m_u4AllRecvSize += (uint32)m_pPacketParse->GetMessageHead()->length();
    }
    else
    {
        m_u4AllRecvSize += (uint32)(m_pPacketParse->GetMessageHead()->length() + m_pPacketParse->GetMessageBody()->length());
    }

	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID     = GetConnectID();
	objMakePacket.m_pPacketParse    = m_pPacketParse;
	objMakePacket.m_u1Option        = PACKET_PARSE;
	objMakePacket.m_AddrRemote      = m_addrRemote;
	objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
    objMakePacket.m_AddrListen      = m_addrLocal;
    objMakePacket.m_tvRecv          = m_atvInput;

    Send_MakePacket_Queue(m_MakePacket, objMakePacket, false);

    m_pPacketParse->Clear();

    return true;
}

_ClientConnectInfo CConnectHandler::GetClientInfo() const
{
    _ClientConnectInfo_Param obj_ClientConnectInfo_Param;
    obj_ClientConnectInfo_Param.m_addrRemote = m_addrRemote;
    obj_ClientConnectInfo_Param.m_atvConnect = m_atvConnect;
    obj_ClientConnectInfo_Param.m_u4AllRecvSize = m_u4AllRecvSize;
    obj_ClientConnectInfo_Param.m_u4AllSendSize = m_u4AllSendSize;
    obj_ClientConnectInfo_Param.m_u4ConnectID = GetConnectID();

    return Tcp_Common_ClientInfo(obj_ClientConnectInfo_Param);
}

_ClientIPInfo  CConnectHandler::GetClientIPInfo() const
{
    _ClientIPInfo ClientIPInfo;
    ClientIPInfo.m_strClientIP = m_addrRemote.get_host_addr();
    ClientIPInfo.m_u2Port = m_addrRemote.get_port_number();
    return ClientIPInfo;
}

_ClientIPInfo  CConnectHandler::GetLocalIPInfo() const
{
    _ClientIPInfo ClientIPInfo;
    ClientIPInfo.m_strClientIP = m_strLocalIP;
    ClientIPInfo.m_u2Port = m_u2LocalPort;
    return ClientIPInfo;
}

bool CConnectHandler::CheckSendMask(uint32 u4PacketLen) const
{
    ACE_UNUSED_ARG(u4PacketLen);

    //Linux下不用检测，直接返回true
    return true;
}

void CConnectHandler::ClearPacketParse()
{
    m_pBlockRecv->reset();
}

bool CConnectHandler::Write_SendData_To_File(bool blDelete, shared_ptr<IBuffPacket> pBuffPacket)
{
    //文件入口，直接写入日志
    _File_Message_Param obj_File_Message_Param;
    obj_File_Message_Param.m_addrRemote        = m_addrRemote;
    obj_File_Message_Param.m_blDelete          = blDelete;
    obj_File_Message_Param.m_pFileTest         = m_pFileTest;
    obj_File_Message_Param.m_u4ConnectID       = GetConnectID();
    obj_File_Message_Param.m_u4PacketDebugSize = m_u4PacketDebugSize;

    return Tcp_Common_File_Message(obj_File_Message_Param,
                                   pBuffPacket,
                                   m_strConnectName.c_str());
}

bool CConnectHandler::Send_Input_To_Cache(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize)
{
    _Input_To_Cache_Param obj_Input_To_Cache_Param;
    obj_Input_To_Cache_Param.m_blDelete            = objSendMessageInfo.blDelete;
    obj_Input_To_Cache_Param.m_emSendType          = objSendMessageInfo.emSendType;
    obj_Input_To_Cache_Param.m_u2CommandID         = objSendMessageInfo.u2CommandID;
    obj_Input_To_Cache_Param.m_u4ConnectID         = m_u4ConnectID;
    obj_Input_To_Cache_Param.m_u4PacketParseInfoID = m_u4PacketParseInfoID;
    obj_Input_To_Cache_Param.m_u4SendMaxBuffSize   = m_u4SendMaxBuffSize;

    return Tcp_Common_Send_Input_To_Cache(m_MakePacket,
        obj_Input_To_Cache_Param,
        m_pBlockMessage,
        u4PacketSize,
        objSendMessageInfo.pBuffPacket);
}

bool CConnectHandler::Send_Input_To_TCP(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize)
{
    _Send_Packet_Param obj_Send_Packet_Param;
    obj_Send_Packet_Param.m_blDelete            = objSendMessageInfo.blDelete;
    obj_Send_Packet_Param.m_emSendType          = objSendMessageInfo.emSendType;
    obj_Send_Packet_Param.m_u2CommandID         = objSendMessageInfo.u2CommandID;
    obj_Send_Packet_Param.m_u4ConnectID         = GetConnectID();
    obj_Send_Packet_Param.m_u4PacketParseInfoID = m_u4PacketParseInfoID;
    obj_Send_Packet_Param.m_u4SendMaxBuffSize   = m_u4SendMaxBuffSize;

    //拼装数据
    if (false == Tcp_Common_Make_Send_Packet(m_MakePacket,
        obj_Send_Packet_Param,
        objSendMessageInfo.pBuffPacket,
        m_pBlockMessage,
        u4PacketSize))
    {
        return false;
    }

	//回收内存
	Recovery_Common_BuffPacket(objSendMessageInfo.blDelete, objSendMessageInfo.pBuffPacket);

    //直接发送数据，不在放到队列里，否则在压测过程中处理太慢
	PutSendPacket(GetConnectID(),
        m_pBlockMessage, 
        (uint32)m_pBlockMessage->length(),
        objSendMessageInfo.tvSendBegin);

    //直接清空缓冲
    m_pBlockMessage->reset();

    return true;
}

int CConnectHandler::Dispose_Paceket_Parse_Stream_Single(ACE_Message_Block* pCurrMessage)
{
    uint8 n1Ret = Tcp_Common_Recv_Stream(GetConnectID(), pCurrMessage, m_pPacketParse, m_u4PacketParseInfoID);

    if (PACKET_GET_NO_ENOUGH == n1Ret)
    {
        return 0;
    }
    else if (PACKET_GET_ENOUGH == n1Ret)
    {
        if (false == CheckMessage())
        {
            return -1;
        }

        m_u4CurrSize = 0;

        //看看是否接收完成了
        if (pCurrMessage->length() == 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        //解析数据失败，返回失败
        return -1;
    }
}

void CConnectHandler::SetConnectName(const char* pName)
{
    m_strConnectName = pName;
}

void CConnectHandler::SetIsLog(bool blIsLog)
{
    m_blIsLog = blIsLog;
}

const char* CConnectHandler::GetConnectName() const
{
    return m_strConnectName.c_str();
}

bool CConnectHandler::GetIsLog() const
{
    return m_blIsLog;
}

void CConnectHandler::SetHashID(int nHashID)
{
    m_nHashID = nHashID;
}

int CConnectHandler::GetHashID() const
{
    return m_nHashID;
}

void CConnectHandler::SetLocalIPInfo(const char* pLocalIP, uint16 u2LocalPort)
{
    m_strLocalIP  = pLocalIP;
    m_u2LocalPort = u2LocalPort;

    if (ACE_OS::strcmp("INADDR_ANY", pLocalIP) == 0)
    {
        m_addrLocal.set(u2LocalPort);
    }
    else
    {
        m_addrLocal.set(u2LocalPort, pLocalIP);
    }
}

//*********************************************************************************

void CConnectHandlerPool::Init(int nObjcetCount)
{
    Close();

    //初始化HashTable
    m_objHandlerList.Init((uint32)nObjcetCount);
    m_objHashHandleList.Init((uint32)nObjcetCount);

    for(uint16 i = 0; i < (uint16)nObjcetCount; i++)
    {
        CConnectHandler* pHandler = m_objHandlerList.GetObject(i);

        if(nullptr != pHandler)
        {
            //将ID和Handler指针的关系存入hashTable

            string strHandlerID = fmt::format("{0}", i);
            int nHashPos = m_objHashHandleList.Add_Hash_Data(strHandlerID.c_str(), pHandler);

            if(-1 != nHashPos)
            {
                pHandler->Init(i);
                pHandler->SetHashID(i);
            }
        }
    }
}

void CConnectHandlerPool::Close()
{
    PSS_LOGGER_DEBUG("[CConnectHandlerPool::Close]Begin.");
    //清理所有已存在的指针

    //删除hash表空间
    m_objHashHandleList.Close();

    //回收内存
    for (uint32 i = 0; i < m_objHandlerList.GetCount(); i++)
    {
        m_objHandlerList.GetObject(i)->CloseFinally();
    }

    m_objHashHandleList.Close();

    PSS_LOGGER_DEBUG("[CConnectHandlerPool::Close]End.");
}

int CConnectHandlerPool::GetUsedCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return m_objHashHandleList.Get_Count() - m_objHashHandleList.Get_Used_Count();
}

int CConnectHandlerPool::GetFreeCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return m_objHashHandleList.Get_Used_Count();
}

CConnectHandler* CConnectHandlerPool::Create()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    //在Hash表中弹出一个已使用的数据
    CConnectHandler* pHandler = m_objHashHandleList.Pop();

	//设置新的ConnectID
	pHandler->SetConnectID(App_ConnectCounter::instance()->CreateCounter());

    //没找到空余的
    return pHandler;
}

bool CConnectHandlerPool::Delete(CConnectHandler* pObject)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if(nullptr == pObject)
    {
        return false;
    }

    string strHandlerID = fmt::format("{0}", pObject->GetHandlerID());
    bool blState = m_objHashHandleList.Push(strHandlerID.c_str(), pObject);

    if(false == blState)
    {
        PSS_LOGGER_DEBUG("[CProConnectHandlerPool::Delete]szHandlerID = {0}({1}).", strHandlerID.c_str(), fmt::ptr(pObject));
    }

    return true;
}

