#include "ConnectHandler.h"

CConnectHandler::CConnectHandler(void)
{
    m_szError[0]          = '\0';
    m_u4ConnectID         = 0;
    m_u2SendCount         = 0;
    m_u4AllRecvCount      = 0;
    m_u4AllSendCount      = 0;
    m_u4AllRecvSize       = 0;
    m_u4AllSendSize       = 0;
    m_u4SendThresHold     = MAX_MSG_SNEDTHRESHOLD;
    m_u1ConnectState      = CONNECT_INIT;
    m_u1SendBuffState     = CONNECT_SENDNON;
    m_pCurrMessage        = NULL;
    m_pBlockMessage       = NULL;
    m_pPacketParse        = NULL;
    m_u4CurrSize          = 0;
    m_u4HandlerID         = 0;
    m_u2MaxConnectTime    = 0;
    m_u1IsActive          = 0;
    m_u4MaxPacketSize     = MAX_MSG_PACKETLENGTH;
    m_blBlockState        = false;
    m_nBlockCount         = 0;
    m_nBlockSize          = MAX_BLOCK_SIZE;
    m_nBlockMaxCount      = MAX_BLOCK_COUNT;
    m_u8RecvQueueTimeCost = 0;
    m_u4RecvQueueCount    = 0;
    m_u8SendQueueTimeCost = 0;
    m_u4ReadSendSize      = 0;
    m_u4SuccessSendSize   = 0;
    m_nHashID             = 0;
    m_u8SendQueueTimeout  = MAX_QUEUE_TIMEOUT * 1000 * 1000;  //目前因为记录的是纳秒
    m_u8RecvQueueTimeout  = MAX_QUEUE_TIMEOUT * 1000 * 1000;  //目前因为记录的是纳秒
    m_u2TcpNodelay        = TCP_NODELAY_ON;
    m_u4SendMaxBuffSize   = 5*MAX_BUFF_1024;
    m_szLocalIP[0]        = '\0';
    m_szConnectName[0]    = '\0';
    m_blIsLog             = false;
    m_u4PacketParseInfoID = 0;
    m_u4PacketDebugSize   = 0;
    m_pPacketDebugData    = NULL;
    m_emIOType            = NET_INPUT;
    m_pFileTest           = NULL;
    m_u4SendCheckTime     = 0;
    m_u4LocalPort         = 0;
}

CConnectHandler::~CConnectHandler(void)
{
    this->closing_ = true;
    SAFE_DELETE_ARRAY(m_pPacketDebugData);
    m_u4PacketDebugSize = 0;
}

const char* CConnectHandler::GetError()
{
    return m_szError;
}

void CConnectHandler::Close()
{
    //调用连接断开消息
    App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->DisConnect(GetConnectID());

    if (CONNECT_SERVER_CLOSE == m_u1ConnectState)
    {
        //服务器主动断开
        Send_MakePacket_Queue(GetConnectID(), m_u4PacketParseInfoID, NULL, PACKET_SDISCONNECT, m_addrRemote, m_szLocalIP, m_u4LocalPort);
    }
    else
    {
        //客户端连接断开
        Send_MakePacket_Queue(GetConnectID(), m_u4PacketParseInfoID, NULL, PACKET_CDISCONNECT, m_addrRemote, m_szLocalIP, m_u4LocalPort);
    }

    shutdown();
    AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, m_u8RecvQueueTimeCost = %dws, m_u4RecvQueueCount = %d, m_u8SendQueueTimeCost = %dws.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, (uint32)m_u8RecvQueueTimeCost, m_u4RecvQueueCount, (uint32)m_u8SendQueueTimeCost);

    //清理缓冲的PacketParse对象
    ClearPacketParse();

    //查看是否需要转发数据
    App_TcpRedirection::instance()->CloseRedirect(GetConnectID());

    OUR_DEBUG((LM_ERROR, "[CConnectHandler::Close](0x%08x)Close(ConnectID=%d) OK.\n", this, GetConnectID()));

    //删除存在列表中的对象引用,这里加一个判定，如果是0说明当前连接尚未完成Manager添加。
    if (GetConnectID() > 0)
    {
        App_ConnectManager::instance()->CloseConnectByClient(GetConnectID());
    }

    //回归用过的指针
    App_ConnectHandlerPool::instance()->Delete(this);
}

void CConnectHandler::Init(uint16 u2HandlerID)
{
    m_u4HandlerID      = u2HandlerID;
    m_u2MaxConnectTime = GetXmlConfigAttribute(xmlClientInfo)->MaxConnectTime;
    m_u4SendThresHold  = GetXmlConfigAttribute(xmlSendInfo)->SendTimeout;
    m_u4MaxPacketSize  = GetXmlConfigAttribute(xmlRecvInfo)->RecvBuffSize;
    m_u2TcpNodelay     = GetXmlConfigAttribute(xmlSendInfo)->TcpNodelay;

    m_u8SendQueueTimeout = (uint64)(GetXmlConfigAttribute(xmlSendInfo)->SendQueueTimeout) * 1000 * 1000;

    if(m_u8SendQueueTimeout == 0)
    {
        m_u8SendQueueTimeout = MAX_QUEUE_TIMEOUT * 1000 * 1000;
    }

    m_u8RecvQueueTimeout = (uint64)(GetXmlConfigAttribute(xmlRecvInfo)->RecvQueueTimeout) * 1000 * 1000;

    if(m_u8RecvQueueTimeout == 0)
    {
        m_u8RecvQueueTimeout = MAX_QUEUE_TIMEOUT * 1000 * 1000;
    }

    m_u4SendMaxBuffSize  = GetXmlConfigAttribute(xmlSendInfo)->MaxBlockSize;
    m_pBlockMessage      = NULL;

    m_pPacketDebugData   = new char[GetXmlConfigAttribute(xmlServerType)->DebugSize];
    m_u4PacketDebugSize  = GetXmlConfigAttribute(xmlServerType)->DebugSize / 5;
}

void CConnectHandler::SetPacketParseInfoID(uint32 u4PacketParseInfoID)
{
    m_u4PacketParseInfoID = u4PacketParseInfoID;
}

uint32 CConnectHandler::GetPacketParseInfoID()
{
    return m_u4PacketParseInfoID;
}

uint32 CConnectHandler::GetHandlerID()
{
    return m_u4HandlerID;
}

void CConnectHandler::SetConnectID(uint32 u4ConnectID)
{
    m_u4ConnectID = u4ConnectID;
}

uint32 CConnectHandler::GetConnectID()
{
    return m_u4ConnectID;
}

int CConnectHandler::open(void*)
{
    m_blBlockState        = false;
    m_nBlockCount         = 0;
    m_u8SendQueueTimeCost = 0;
    m_blIsLog             = false;
    m_szConnectName[0]    = '\0';
    m_u1IsActive          = 1;

    //重置缓冲区

    if (NULL == App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID))
    {
        //如果解析器不存在，则直接断开连接
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open](%s)can't find PacketParseInfo.\n", m_addrRemote.get_host_addr()));
        return -1;
    }

    //获得远程链接地址和端口
    if(this->peer().get_remote_addr(m_addrRemote) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]this->peer().get_remote_addr error.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::open]this->peer().get_remote_addr error.");
        return -1;
    }

    if(App_ForbiddenIP::instance()->CheckIP(m_addrRemote.get_host_addr()) == false)
    {
        //在禁止列表中，不允许访问
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]IP Forbidden(%s).\n", m_addrRemote.get_host_addr()));
        return -1;
    }

    //检查单位时间链接次数是否达到上限
    if(false == App_IPAccount::instance()->AddIP((string)m_addrRemote.get_host_addr()))
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]IP connect frequently.\n", m_addrRemote.get_host_addr()));
        App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), GetXmlConfigAttribute(xmlIP)->Timeout);

        //发送告警邮件
        AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT,
                                               GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                                               (char* )"Alert IP",
                                               "[CConnectHandler::open] IP is more than IP Max,");

        return -1;
    }

    //初始化检查器
    m_TimeConnectInfo.Init(GetXmlConfigAttribute(xmlClientData)->RecvPacketCount,
                           GetXmlConfigAttribute(xmlClientData)->RecvDataMax,
                           GetXmlConfigAttribute(xmlClientData)->SendPacketCount,
                           GetXmlConfigAttribute(xmlClientData)->SendDataMax);

    int nRet = 0;

    //设置链接为非阻塞模式
    if (this->peer().enable(ACE_NONBLOCK) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]this->peer().enable  = ACE_NONBLOCK error.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::open]this->peer().enable  = ACE_NONBLOCK error.");
        return -1;
    }

    //初始化参数设置
    if (-1 == Init_Open_Connect())
    {
        return -1;
    }

    if(m_pCurrMessage == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]pmb new is NULL.\n"));
        return -1;
    }

    //将这个链接放入链接库
    if(false == App_ConnectManager::instance()->AddConnect(this))
    {
        OUR_DEBUG((LM_ERROR, "%s.\n", App_ConnectManager::instance()->GetError()));
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", App_ConnectManager::instance()->GetError());
        return -1;
    }

    ConnectOpen();

    nRet = this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK|ACE_Event_Handler::WRITE_MASK);

    int nWakeupRet = reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);

    if (-1 == nWakeupRet)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]ConnectID=%d, nWakeupRet=%d, errno=%d.\n", GetConnectID(), nWakeupRet, errno));
    }

    //查看是否需要转发数据
    App_TcpRedirection::instance()->ConnectRedirect(m_u4LocalPort, GetConnectID());

    m_emIOType = NET_INPUT;
    return nRet;
}

//接受数据
int CConnectHandler::handle_input(ACE_HANDLE fd)
{
    m_atvInput = ACE_OS::gettimeofday();

    if(fd == ACE_INVALID_HANDLE)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_input]fd == ACE_INVALID_HANDLE.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::handle_input]fd == ACE_INVALID_HANDLE.");

        return -1;
    }

    //判断数据包结构是否为NULL
    if(m_pPacketParse == NULL)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_input]ConnectID=%d, m_pPacketParse == NULL.\n", GetConnectID()));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::handle_input]m_pPacketParse == NULL.");

        return -1;
    }

    return RecvData();

}

int CConnectHandler::handle_output(ACE_HANDLE fd /*= ACE_INVALID_HANDLE*/)
{
    if (fd == ACE_INVALID_HANDLE)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_output]fd == ACE_INVALID_HANDLE.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::handle_output]fd == ACE_INVALID_HANDLE.");

        return -1;
    }

    ACE_Message_Block* pmbSendData = NULL;
    ACE_Time_Value nowait(ACE_OS::gettimeofday());

    while (-1 != this->getq(pmbSendData, &nowait))
    {
        if (NULL == pmbSendData)
        {
            OUR_DEBUG((LM_INFO, "[CConnectHandler::handle_output]ConnectID=%d pmbSendData is NULL.\n", GetConnectID()));
            break;
        }

        //如果是断开指令，则执行连接断开操作
        if (pmbSendData->msg_type() == ACE_Message_Block::MB_STOP)
        {
            App_MessageBlockManager::instance()->Close(pmbSendData);
            return -1;
        }

        uint32 u4SendSuc = (uint32)pmbSendData->length();
        bool blRet = PutSendPacket(pmbSendData);

        if (true == blRet)
        {
            if (m_u4ReadSendSize >= m_u4SuccessSendSize + u4SendSuc)
            {
                //记录成功发送字节
                m_u4SuccessSendSize += u4SendSuc;
            }
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CConnectHandler::handle_output]ConnectID=%d write is close.\n", GetConnectID()));
            return -1;
        }
    }

    int nWakeupRet =  reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);

    if (-1 == nWakeupRet)
    {
        OUR_DEBUG((LM_INFO, "[CConnectHandler::handle_output]ConnectID=%d,nWakeupRet=%d, errno=%d.\n", GetConnectID(), nWakeupRet, errno));
    }

    return 0;
}

//剥离接收数据代码
int CConnectHandler::RecvData()
{
    return Dispose_Recv_Data();
}

int CConnectHandler::Dispose_Recv_Data()
{
    ACE_Time_Value nowait(0, MAX_QUEUE_TIMEOUT);

    //新数据包大小
    uint32 u4NewPacketSize = 0;

    //判断缓冲是否为NULL
    if (m_pCurrMessage == NULL)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData]m_pCurrMessage == NULL.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::RecvData]m_pCurrMessage == NULL.");

        return -1;
    }

    //计算应该接收的数据长度
    int nCurrCount = 0;

    Get_Recv_length(nCurrCount);

    //这里需要对m_u4CurrSize进行检查。
    if (nCurrCount <= 0)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData][%d] nCurrCount < 0 m_u4CurrSize = %d.\n", GetConnectID(), m_u4CurrSize));
        m_u4CurrSize = 0;

        return -1;
    }

    int nDataLen = (int)this->peer().recv(m_pCurrMessage->wr_ptr(), nCurrCount, MSG_NOSIGNAL, &nowait);

    if (nDataLen <= 0)
    {
        m_u4CurrSize = 0;
        uint32 u4Error = (uint32)errno;

        //如果是-1 且为11的错误，忽略之
        if (nDataLen == -1 && u4Error == EAGAIN)
        {
            return 0;
        }

        OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData] ConnectID=%d, recv data is error nDataLen = [%d] errno = [%d].\n", GetConnectID(), nDataLen, u4Error));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::RecvData] ConnectID = %d, recv data is error[%d].\n", GetConnectID(), nDataLen);

        return -1;
    }

    m_u4CurrSize += nDataLen;

    m_pCurrMessage->wr_ptr(nDataLen);

    //如果是DEBUG状态，记录当前接受包的二进制数据
    Output_Debug_Data(m_pCurrMessage, LOG_SYSTEM_DEBUG_CLIENTRECV);

    //查看是否需要转发数据
    App_TcpRedirection::instance()->DataRedirect(GetConnectID(), m_pCurrMessage);

    if (App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        //如果没有读完，短读
        if (nCurrCount - nDataLen > 0)
        {
            //没读完，继续读
            return 0;
        }

        if (m_pCurrMessage->length() == App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength
            && m_pPacketParse->GetIsHandleHead())
        {
            //处理Head消息头
            return Dispose_Paceket_Parse_Head();
        }
        else
        {
            //接受完整数据完成，开始分析完整数据包
            if (-1 == Dispose_Paceket_Parse_Body() || false == CheckMessage())
            {
                return -1;
            }

            m_u4CurrSize = 0;

            //申请新的包
            m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

            if (NULL == m_pPacketParse)
            {
                OUR_DEBUG((LM_DEBUG, "[%t|CConnectHandle::RecvData] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
                return -1;
            }

            //计算下一个包的大小
            u4NewPacketSize = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength;
        }
    }
    else
    {
        //以流模式解析
        if (-1 == Dispose_Paceket_Parse_Stream(m_pCurrMessage))
        {
            return -1;
        }

        App_MessageBlockManager::instance()->Close(m_pCurrMessage);
        m_u4CurrSize = 0;

        //计算下一个包的大小
        u4NewPacketSize = GetXmlConfigAttribute(xmlClientInfo)->MaxBuffRecv;
    }

    //申请头的大小对应的mb
    m_pCurrMessage = App_MessageBlockManager::instance()->Create(u4NewPacketSize);

    if (m_pCurrMessage == NULL)
    {
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, m_u8RecvQueueTimeCost = %dws, m_u4RecvQueueCount = %d, m_u8SendQueueTimeCost = %dws.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, (uint32)m_u8RecvQueueTimeCost, m_u4RecvQueueCount, (uint32)m_u8SendQueueTimeCost);
        OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvData] pmb new is NULL.\n"));
        return -1;
    }

    return 0;
}

int CConnectHandler::Init_Open_Connect()
{
    //设置默认别名
    SetConnectName(m_addrRemote.get_host_addr());

    //初始化当前链接的某些参数
    m_atvConnect          = ACE_OS::gettimeofday();
    m_atvInput            = ACE_OS::gettimeofday();
    m_atvOutput           = ACE_OS::gettimeofday();
    m_atvSendAlive        = ACE_OS::gettimeofday();

    m_u4ConnectID         = 0;
    m_u4AllRecvCount      = 0;
    m_u4AllSendCount      = 0;
    m_u4AllRecvSize       = 0;
    m_u4AllSendSize       = 0;
    m_u8RecvQueueTimeCost = 0;
    m_u4RecvQueueCount    = 0;
    m_u8SendQueueTimeCost = 0;
    m_u4CurrSize          = 0;

    m_u4ReadSendSize      = 0;
    m_u4SuccessSendSize   = 0;

    if (m_u2TcpNodelay == TCP_NODELAY_OFF)
    {
        //如果设置了禁用Nagle算法，则这里要禁用。
        int nOpt = 1;
        ACE_OS::setsockopt(this->get_handle(), IPPROTO_TCP, TCP_NODELAY, (char*)&nOpt, sizeof(int));
    }

    m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

    if (NULL == m_pPacketParse)
    {
        OUR_DEBUG((LM_DEBUG, "[CConnectHandler::open]Open(%d) m_pPacketParse new error.\n", GetConnectID()));
        return -1;
    }

    //申请头的大小对应的mb
    if (App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        m_pCurrMessage = App_MessageBlockManager::instance()->Create(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength);
    }
    else
    {
        m_pCurrMessage = App_MessageBlockManager::instance()->Create(GetXmlConfigAttribute(xmlClientInfo)->MaxBuffRecv);
    }

    return 0;
}

//关闭链接
int CConnectHandler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
    if(h == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_DEBUG,"[CConnectHandler::handle_close] h is NULL mask=%d.\n", (int)mask));
    }

    //设置发送消息队列不能再发送任何消息
    if(m_u1ConnectState != CONNECT_SERVER_CLOSE)
    {
        m_u1ConnectState = CONNECT_CLIENT_CLOSE;
    }

    //读取发送队列内部的所有数据，标记为无效并回收内存
    ACE_Message_Block* pmbSendData = NULL;
    ACE_Time_Value nowait(ACE_OS::gettimeofday());

    while (-1 != this->getq(pmbSendData, &nowait))
    {
        App_MessageBlockManager::instance()->Close(pmbSendData);
    }

    Close();

    return 0;
}

uint32 CConnectHandler::file_open(IFileTestManager* pFileTest)
{
    m_blBlockState = false;
    m_nBlockCount = 0;
    m_u8SendQueueTimeCost = 0;
    m_blIsLog = false;
    m_szConnectName[0] = '\0';
    m_u1IsActive = 1;

    //重置缓冲区

    if (NULL == App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID))
    {
        //如果解析器不存在，则直接断开连接
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open](%s)can't find PacketParseInfo.\n", m_addrRemote.get_host_addr()));
        return -1;
    }

    //初始化检查器
    m_TimeConnectInfo.Init(GetXmlConfigAttribute(xmlClientData)->RecvPacketCount,
                           GetXmlConfigAttribute(xmlClientData)->RecvDataMax,
                           GetXmlConfigAttribute(xmlClientData)->SendPacketCount,
                           GetXmlConfigAttribute(xmlClientData)->SendDataMax);

    if (-1 == Init_Open_Connect())
    {
        return 0;
    }

    if (m_pCurrMessage == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]pmb new is NULL.\n"));
        return 0;
    }

    //将这个链接放入链接库
    if (false == App_ConnectManager::instance()->AddConnect(this))
    {
        OUR_DEBUG((LM_ERROR, "%s.\n", App_ConnectManager::instance()->GetError()));
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", App_ConnectManager::instance()->GetError());
        return 0;
    }

    ConnectOpen();

    m_emIOType       = FILE_INPUT;
    m_pFileTest      = pFileTest;

    return GetConnectID();
}

int CConnectHandler::handle_write_file_stream(const char* pData, uint32 u4Size, uint8 u1ParseID)
{
    m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

    if (NULL == m_pPacketParse)
    {
        OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_write_file_stream](%d) m_pPacketParse new error.\n", GetConnectID()));
        return -1;
    }

    if (App_PacketParseLoader::instance()->GetPacketParseInfo(u1ParseID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        uint32 u4PacketHead = App_PacketParseLoader::instance()->GetPacketParseInfo(u1ParseID)->m_u4OrgLength;
        ACE_Message_Block* pMbHead = App_MessageBlockManager::instance()->Create(u4PacketHead);

        memcpy_safe((char*)pData, u4PacketHead, pMbHead->wr_ptr(), u4PacketHead);
        pMbHead->wr_ptr(u4PacketHead);

        //解析消息头
        _Head_Info objHeadInfo;
        bool blStateHead = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Head_Info(GetConnectID(), pMbHead, App_MessageBlockManager::instance(), &objHeadInfo);

        if (false == blStateHead)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_write_file_stream](%d) Parse_Packet_Head_Info is illegal.\n", GetConnectID()));
            ClearPacketParse();
            return -1;
        }

        m_pPacketParse->SetPacket_IsHandleHead(false);
        m_pPacketParse->SetPacket_Head_Message(objHeadInfo.m_pmbHead);
        m_pPacketParse->SetPacket_Head_Curr_Length(objHeadInfo.m_u4HeadCurrLen);
        m_pPacketParse->SetPacket_Body_Src_Length(objHeadInfo.m_u4BodySrcLen);
        m_pPacketParse->SetPacket_CommandID(objHeadInfo.m_u2PacketCommandID);

        //解析消息体
        ACE_Message_Block* pMbBody = App_MessageBlockManager::instance()->Create(u4Size - u4PacketHead);

        memcpy_safe((char*)&pData[u4PacketHead], u4Size - u4PacketHead, pMbBody->wr_ptr(), u4Size - u4PacketHead);
        pMbBody->wr_ptr(u4Size - u4PacketHead);

        //解析数据包体
        _Body_Info obj_Body_Info;
        bool blStateBody = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Body_Info(GetConnectID(), pMbBody, App_MessageBlockManager::instance(), &obj_Body_Info);

        if (false == blStateBody)
        {
            //如果数据包体非法，断开连接
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_write_file_stream]Parse_Packet_Body_Info is illegal.\n"));

            //清理PacketParse
            ClearPacketParse();
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
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_write_file_stream]CheckMessage is false.\n"));
            return -1;
        }
    }
    else
    {
        //流模式处理文件数据
        ACE_Message_Block* pCurrMessage = App_MessageBlockManager::instance()->Create(u4Size);
        memcpy_safe((char*)pData, u4Size, pCurrMessage->wr_ptr(), u4Size);
        pCurrMessage->wr_ptr(u4Size);

        if (-1 == Dispose_Paceket_Parse_Stream(pCurrMessage))
        {
            return -1;
        }
    }

    return 0;
}

void CConnectHandler::SetRecvQueueTimeCost(uint32 u4TimeCost)
{
    //如果超过阀值，则记录到日志中去
    if((uint32)(m_u8RecvQueueTimeout * 1000) <= u4TimeCost)
    {
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_RECVQUEUEERROR, "[TCP]IP=%s,Prot=%d, m_u8RecvQueueTimeout=[%d], Timeout=[%d].", GetClientIPInfo().m_szClientIP, GetClientIPInfo().m_nPort, (uint32)m_u8RecvQueueTimeout, u4TimeCost);
    }

    m_u4RecvQueueCount++;
}

void CConnectHandler::SetSendQueueTimeCost(uint32 u4TimeCost)
{
    //如果超过阀值，则记录到日志中去
    if((uint32)(m_u8SendQueueTimeout) <= u4TimeCost)
    {
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_SENDQUEUEERROR, "[TCP]IP=%s,Prot=%d,m_u8SendQueueTimeout = [%d], Timeout=[%d].", GetClientIPInfo().m_szClientIP, GetClientIPInfo().m_nPort, (uint32)m_u8SendQueueTimeout, u4TimeCost);

        Send_MakePacket_Queue(GetConnectID(), m_u4PacketParseInfoID, NULL, PACKET_SEND_TIMEOUT, m_addrRemote, m_szLocalIP, m_u4LocalPort);
    }
}

uint8 CConnectHandler::GetConnectState()
{
    return m_u1ConnectState;
}

uint8 CConnectHandler::GetSendBuffState()
{
    return m_u1SendBuffState;
}

bool CConnectHandler::SendMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, uint8 u1State, uint8 u1SendType, uint32& u4PacketSize, bool blDelete, int nMessageID)
{
    //如果当前连接已被别的线程关闭，则这里不做处理，直接退出
    if (m_u1IsActive == 0)
    {
        //如果连接不存在了，在这里返回失败，回调给业务逻辑去处理
        Tcp_Common_Send_Message_Error(blDelete, pBuffPacket);

        return false;
    }

    if (NET_INPUT == m_emIOType)
    {
        if (CONNECT_SERVER_CLOSE == m_u1ConnectState || CONNECT_CLIENT_CLOSE == m_u1ConnectState)
        {
            //在队列里已经存在关闭连接指令，之后的所有数据写请求全部不予发送。
            ACE_Message_Block* pSendMessage = App_MessageBlockManager::instance()->Create(pBuffPacket->GetPacketLen());
            memcpy_safe((char*)pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), (char*)pSendMessage->wr_ptr(), pBuffPacket->GetPacketLen());
            pSendMessage->wr_ptr(pBuffPacket->GetPacketLen());
            ACE_Time_Value tvNow = ACE_OS::gettimeofday();
            App_MakePacket::instance()->PutSendErrorMessage(0, pSendMessage, tvNow);

            Recovery_Common_BuffPacket(blDelete, pBuffPacket);

            return false;
        }

        //如果不是直接发送数据，则拼接数据包
        if (u1State == PACKET_SEND_CACHE)
        {
            return Send_Input_To_Cache(u1SendType, u4PacketSize, u2CommandID, blDelete, pBuffPacket);
        }
        else
        {
            //要发送数据到对端
            return Send_Input_To_TCP(u1SendType, u4PacketSize, u2CommandID, u1State, nMessageID, blDelete, pBuffPacket);
        }
    }
    else
    {
        //文件入口，直接写入日志
        return Write_SendData_To_File(blDelete, pBuffPacket);
    }
}

bool CConnectHandler::SendCloseMessage()
{
    if (NET_INPUT == m_emIOType)
    {
        //将连接消息断开放在output去执行，这样就不需要同步加锁了。
        ACE_Message_Block* pMbData = App_MessageBlockManager::instance()->Create(sizeof(int));

        if (NULL == pMbData)
        {
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::SendCloseMessage] Connectid=%d, pMbData is NULL.\n", GetConnectID()));
            return false;
        }

        ACE_Message_Block::ACE_Message_Type objType = ACE_Message_Block::MB_STOP;
        pMbData->msg_type(objType);

        if (false == Send_Block_Queue(pMbData))
        {
            return false;
        }

        m_u1ConnectState = CONNECT_SERVER_CLOSE;
    }
    else
    {
        //关闭回收当前连接
        Close();
    }

    return true;
}

bool CConnectHandler::PutSendPacket(ACE_Message_Block* pMbData)
{
    if(NULL == pMbData)
    {
        return false;
    }

    //如果是DEBUG状态，记录当前发送包的二进制数据
    Output_Debug_Data(pMbData, LOG_SYSTEM_DEBUG_CLIENTSEND);

    //统计发送数量
    ACE_Date_Time dtNow;

    if(false == m_TimeConnectInfo.SendCheck((uint8)dtNow.minute(), 1, (uint32)pMbData->length()))
    {
        //超过了限定的阀值，需要关闭链接，并记录日志
        AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECTABNORMAL,
                                               GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                                               (char* )"Alert",
                                               "[TCP]IP=%s,Prot=%d,SendPacketCount=%d, SendSize=%d.",
                                               m_addrRemote.get_host_addr(),
                                               m_addrRemote.get_port_number(),
                                               m_TimeConnectInfo.m_u4SendPacketCount,
                                               m_TimeConnectInfo.m_u4SendSize);

        //设置封禁时间
        App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), GetXmlConfigAttribute(xmlIP)->Timeout);
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, Send Data is more than limit.\n", GetConnectID()));

        ACE_Time_Value tvNow = ACE_OS::gettimeofday();
        App_MakePacket::instance()->PutSendErrorMessage(GetConnectID(), pMbData, tvNow);

        return false;
    }

    //发送超时时间设置
    ACE_Time_Value  nowait(0, m_u4SendThresHold*MAX_BUFF_1000);

    if(NULL == pMbData)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID()));
        return false;
    }

    if(get_handle() == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID()));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::PutSendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID());
        ACE_Time_Value tvNow = ACE_OS::gettimeofday();
        App_MakePacket::instance()->PutSendErrorMessage(GetConnectID(), pMbData, tvNow);
        App_MessageBlockManager::instance()->Close(pMbData);
        return false;
    }

    //发送数据
    int nSendPacketLen = (int)pMbData->length();
    int nIsSendSize    = 0;

    //循环发送，直到数据发送完成。
    while(true)
    {
        if(nSendPacketLen <= 0)
        {
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, nCurrSendSize error is %d.\n", GetConnectID(), nSendPacketLen));
            App_MessageBlockManager::instance()->Close(pMbData);
            return false;
        }

        int nDataLen = (int)this->peer().send(pMbData->rd_ptr(), nSendPacketLen - nIsSendSize, &nowait);

        if(nDataLen <= 0)
        {
            int nErrno = errno;
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, error = %d.\n", GetConnectID(), nErrno));

            AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "WriteError [%s:%d] nErrno = %d  result.bytes_transferred() = %d, ",
                                                m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), nErrno,
                                                nIsSendSize);

            //错误消息回调
            pMbData->rd_ptr((size_t)0);
            ACE_Time_Value tvNow = ACE_OS::gettimeofday();
            App_MakePacket::instance()->PutSendErrorMessage(GetConnectID(), pMbData, tvNow);

            OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID=%d send cancel.\n", GetConnectID()));

            return false;
        }
        else if(nDataLen >= nSendPacketLen - nIsSendSize)   //当数据包全部发送完毕，清空。
        {
            m_u4AllSendCount    += 1;
            m_u4AllSendSize     += (uint32)pMbData->length();
            m_atvOutput         = ACE_OS::gettimeofday();

            int nMessageID = pMbData->msg_type() - ACE_Message_Block::MB_USER;

            if(nMessageID > 0)
            {
                //需要回调发送成功回执
                CPacketParse objPacketParse;
                ACE_Message_Block* pSendOKData = App_MessageBlockManager::instance()->Create(sizeof(int));
                memcpy_safe((char*)&nMessageID, sizeof(int), pSendOKData->wr_ptr(), sizeof(int));
                pSendOKData->wr_ptr(sizeof(int));
                objPacketParse.SetPacket_Head_Message(pSendOKData);
                objPacketParse.SetPacket_Head_Curr_Length((uint32)pSendOKData->length());

                Send_MakePacket_Queue(GetConnectID(), m_u4PacketParseInfoID, &objPacketParse, PACKET_SEND_OK, m_addrRemote, m_szLocalIP, m_u4LocalPort);

                //还原消息类型
                pMbData->msg_type(ACE_Message_Block::MB_DATA);
            }

            App_MessageBlockManager::instance()->Close(pMbData);

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

void CConnectHandler::ConnectOpen()
{
    //写入连接日志
    AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Connection from [%s:%d]ConnectID=%d, GetHandlerID=%d.",
                                        m_addrRemote.get_host_addr(),
                                        m_addrRemote.get_port_number(),
                                        GetConnectID(),
                                        GetHandlerID());

    //告诉PacketParse连接应建立
    App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Connect(GetConnectID(), GetClientIPInfo(), GetLocalIPInfo());

    Send_MakePacket_Queue(GetConnectID(), m_u4PacketParseInfoID, NULL, PACKET_CONNECT, m_addrRemote, m_szLocalIP, m_u4LocalPort);

    OUR_DEBUG((LM_DEBUG, "[CConnectHandler::open]Open(%d) Connection from [%s:%d](0x%08x).\n", GetConnectID(), m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), this));

    m_u1ConnectState = CONNECT_OPEN;
}

void CConnectHandler::Get_Recv_length(int& nCurrCount)
{
    if (App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        if (m_pPacketParse->GetIsHandleHead())
        {
            nCurrCount = (uint32)App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength - (uint32)m_pCurrMessage->length();
        }
        else
        {
            nCurrCount = (uint32)m_pPacketParse->GetPacketBodySrcLen() - (uint32)m_pCurrMessage->length();
        }
    }
    else
    {
        nCurrCount = (uint32)GetXmlConfigAttribute(xmlClientInfo)->MaxBuffRecv - (uint32)m_pCurrMessage->length();
    }
}

void CConnectHandler::Output_Debug_Data(ACE_Message_Block* pMbData, int nLogType)
{
    if (GetXmlConfigAttribute(xmlServerType)->Debug == DEBUG_ON || m_blIsLog == true)
    {
        int nDataLen = (int)pMbData->length();
        char szLog[10] = { '\0' };
        uint32 u4DebugSize = 0;
        bool blblMore = false;

        if ((uint32)nDataLen >= m_u4PacketDebugSize)
        {
            u4DebugSize = m_u4PacketDebugSize - 1;
            blblMore = true;
        }
        else
        {
            u4DebugSize = (uint32)nDataLen;
        }

        char* pData = pMbData->rd_ptr();

        for (uint32 i = 0; i < u4DebugSize; i++)
        {
            sprintf_safe(szLog, 10, "0x%02X ", (unsigned char)pData[i]);
            sprintf_safe(m_pPacketDebugData + 5 * i, MAX_BUFF_1024 - 5 * i, "%s", szLog);
        }

        m_pPacketDebugData[5 * u4DebugSize] = '\0';

        if (blblMore == true)
        {
            AppLogManager::instance()->WriteLog(nLogType, "[(%s)%s:%d]%s.(数据包过长只记录前200字节)", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_pPacketDebugData);
        }
        else
        {
            AppLogManager::instance()->WriteLog(nLogType, "[(%s)%s:%d]%s.", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_pPacketDebugData);
        }
    }
}

int CConnectHandler::Dispose_Paceket_Parse_Head()
{
    _Head_Info objHeadInfo;
    bool blStateHead = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Head_Info(GetConnectID(), m_pCurrMessage, App_MessageBlockManager::instance(), &objHeadInfo);

    if (false == blStateHead)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData]SetPacketHead is false.\n"));

        return -1;
    }
    else
    {
        if (NULL == objHeadInfo.m_pmbHead)
        {
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData]ConnectID=%d, objHeadInfo.m_pmbHead is NULL.\n", GetConnectID()));
        }

        m_pPacketParse->SetPacket_IsHandleHead(false);
        m_pPacketParse->SetPacket_Head_Curr_Length(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength);
        m_pPacketParse->SetPacket_Head_Message(objHeadInfo.m_pmbHead);
        m_pPacketParse->SetPacket_Head_Curr_Length(objHeadInfo.m_u4HeadCurrLen);
        m_pPacketParse->SetPacket_Body_Src_Length(objHeadInfo.m_u4BodySrcLen);
        m_pPacketParse->SetPacket_CommandID(objHeadInfo.m_u2PacketCommandID);
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

        //申请新的包
        m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

        if (NULL == m_pPacketParse)
        {
            OUR_DEBUG((LM_DEBUG, "[%t|CConnectHandle::RecvData] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
            return -1;
        }

        //申请头的大小对应的mb
        m_pCurrMessage = App_MessageBlockManager::instance()->Create(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength);

        if (m_pCurrMessage == NULL)
        {
            AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, m_u8RecvQueueTimeCost = %dws, m_u4RecvQueueCount = %d, m_u8SendQueueTimeCost = %dws.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, (uint32)m_u8RecvQueueTimeCost, m_u4RecvQueueCount, (uint32)m_u8SendQueueTimeCost);
            OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvData] pmb new is NULL.\n"));
            return -1;
        }
    }
    else
    {
        //如果超过了最大包长度，为非法数据
        if (u4PacketBodyLen >= m_u4MaxPacketSize)
        {
            m_u4CurrSize = 0;
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData]u4PacketHeadLen(%d) more than %d.\n", u4PacketBodyLen, m_u4MaxPacketSize));

            return -1;
        }
        else
        {
            //申请头的大小对应的mb
            m_pCurrMessage = App_MessageBlockManager::instance()->Create(u4PacketBodyLen);

            if (m_pCurrMessage == NULL)
            {
                m_u4CurrSize = 0;
                OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvData] pmb new is NULL.\n"));

                return -1;
            }
        }
    }

    return 0;
}

int CConnectHandler::Dispose_Paceket_Parse_Body()
{
    _Body_Info obj_Body_Info;
    bool blStateBody = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Body_Info(GetConnectID(), m_pCurrMessage, App_MessageBlockManager::instance(), &obj_Body_Info);

    if (false == blStateBody)
    {
        //如果数据包体是错误的，则断开连接
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvData]SetPacketBody is false.\n"));

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
    if (m_pPacketParse->GetMessageHead() == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::CheckMessage](%d)head is NULL.\n", GetConnectID()));
        return false;
    }

    if(m_pPacketParse->GetMessageBody() == NULL)
    {
        m_u4AllRecvSize += (uint32)m_pPacketParse->GetMessageHead()->length();
    }
    else
    {
        m_u4AllRecvSize += (uint32)m_pPacketParse->GetMessageHead()->length() + (uint32)m_pPacketParse->GetMessageBody()->length();
    }

    m_u4AllRecvCount++;

    ACE_Time_Value tvCheck = ACE_OS::gettimeofday();
    ACE_Date_Time dtNow(tvCheck);

    if(false == m_TimeConnectInfo.RecvCheck((uint8)dtNow.minute(), 1, m_u4AllRecvSize))
    {
        //超过了限定的阀值，需要关闭链接，并记录日志
        AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECTABNORMAL,
                                               GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                                               (char* )"Alert",
                                               "[TCP]IP=%s,Prot=%d,PacketCount=%d, RecvSize=%d.",
                                               m_addrRemote.get_host_addr(),
                                               m_addrRemote.get_port_number(),
                                               m_TimeConnectInfo.m_u4RecvPacketCount,
                                               m_TimeConnectInfo.m_u4RecvSize);

        App_PacketParsePool::instance()->Delete(m_pPacketParse, true);
        m_pPacketParse = NULL;

        //设置封禁时间
        App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), GetXmlConfigAttribute(xmlIP)->Timeout);
        OUR_DEBUG((LM_ERROR, "[CConnectHandle::CheckMessage] ConnectID = %d, PutMessageBlock is check invalid.\n", GetConnectID()));
        return false;
    }

    Send_MakePacket_Queue(GetConnectID(), m_u4PacketParseInfoID, m_pPacketParse, PACKET_PARSE, m_addrRemote, m_szLocalIP, m_u4LocalPort);

    //更新时间轮盘
    App_ConnectManager::instance()->SetConnectTimeWheel(this);

    App_PacketParsePool::instance()->Delete(m_pPacketParse);

    return true;
}

_ClientConnectInfo CConnectHandler::GetClientInfo()
{
    _ClientConnectInfo_Param obj_ClientConnectInfo_Param;
    obj_ClientConnectInfo_Param.m_addrRemote = m_addrRemote;
    obj_ClientConnectInfo_Param.m_atvConnect = m_atvConnect;
    obj_ClientConnectInfo_Param.m_u4AllRecvCount = m_u4AllRecvCount;
    obj_ClientConnectInfo_Param.m_u4AllRecvSize = m_u4AllRecvSize;
    obj_ClientConnectInfo_Param.m_u4AllSendCount = m_u4AllSendCount;
    obj_ClientConnectInfo_Param.m_u4AllSendSize = m_u4AllSendSize;
    obj_ClientConnectInfo_Param.m_u4ConnectID = GetConnectID();
    obj_ClientConnectInfo_Param.m_u4RecvQueueCount = m_u4RecvQueueCount;
    obj_ClientConnectInfo_Param.m_u8RecvQueueTimeCost = m_u8RecvQueueTimeCost;
    obj_ClientConnectInfo_Param.m_u8SendQueueTimeCost = m_u8SendQueueTimeCost;

    return Tcp_Common_ClientInfo(obj_ClientConnectInfo_Param);
}

_ClientIPInfo  CConnectHandler::GetClientIPInfo()
{
    _ClientIPInfo ClientIPInfo;
    sprintf_safe(ClientIPInfo.m_szClientIP, MAX_BUFF_50, "%s", m_addrRemote.get_host_addr());
    ClientIPInfo.m_nPort = (int)m_addrRemote.get_port_number();
    return ClientIPInfo;
}

_ClientIPInfo  CConnectHandler::GetLocalIPInfo()
{
    _ClientIPInfo ClientIPInfo;
    sprintf_safe(ClientIPInfo.m_szClientIP, MAX_BUFF_50, "%s", m_szLocalIP);
    ClientIPInfo.m_nPort = (int)m_u4LocalPort;
    return ClientIPInfo;
}

bool CConnectHandler::CheckSendMask(uint32 u4PacketLen)
{
    m_u4ReadSendSize += u4PacketLen;

    if(m_u4ReadSendSize - m_u4SuccessSendSize >= GetXmlConfigAttribute(xmlSendInfo)->MaxBlockSize)
    {
        OUR_DEBUG ((LM_ERROR, "[CConnectHandler::CheckSendMask]ConnectID = %d, SingleConnectMaxSendBuffer is more than DataMask(%d)(%d:%d)!\n", GetConnectID(), GetXmlConfigAttribute(xmlSendInfo)->MaxBlockSize, m_u4ReadSendSize, m_u4SuccessSendSize));
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_SENDQUEUEERROR, "]Connection from [%s:%d], SingleConnectMaxSendBuffer is more than(%d)!.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4ReadSendSize - m_u4SuccessSendSize);
        return false;
    }
    else
    {
        return true;
    }
}

void CConnectHandler::ClearPacketParse()
{
    if(NULL != m_pPacketParse)
    {
        if (m_pPacketParse->GetMessageHead() != NULL)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
        }

        if (m_pPacketParse->GetMessageBody() != NULL)
        {
            App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
        }

        if(m_pCurrMessage != NULL && m_pPacketParse->GetMessageBody() != m_pCurrMessage && m_pPacketParse->GetMessageHead() != m_pCurrMessage)
        {
            App_MessageBlockManager::instance()->Close(m_pCurrMessage);
        }

        m_pCurrMessage = NULL;

        App_PacketParsePool::instance()->Delete(m_pPacketParse);
        m_pPacketParse = NULL;
    }
}

bool CConnectHandler::Send_Block_Queue(ACE_Message_Block* pMb)
{
    //将消息放入队列，让output在反应器线程发送。
    ACE_Time_Value xtime = ACE_OS::gettimeofday();

    //队列已满，不能再放进去了,就不放进去了
    if (msg_queue()->is_full() == true)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::SendCloseMessage] Connectid=%d,putq is full(%d).\n", GetConnectID(), msg_queue()->message_count()));
        App_MessageBlockManager::instance()->Close(pMb);
        return false;
    }

    if (this->putq(pMb, &xtime) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::SendCloseMessage] Connectid=%d,putq(%d) output errno = [%d].\n", GetConnectID(), msg_queue()->message_count(), errno));
        App_MessageBlockManager::instance()->Close(pMb);
    }
    else
    {
        int nWakeupRet = reactor()->schedule_wakeup(this, ACE_Event_Handler::WRITE_MASK);

        if (-1 == nWakeupRet)
        {
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::SendCloseMessage] Connectid=%d, nWakeupRet(%d) output errno = [%d].\n", GetConnectID(), nWakeupRet, errno));
        }
    }

    return true;
}

bool CConnectHandler::Write_SendData_To_File(bool blDelete, IBuffPacket* pBuffPacket)
{
    //文件入口，直接写入日志
    _File_Message_Param obj_File_Message_Param;
    obj_File_Message_Param.m_addrRemote        = m_addrRemote;
    obj_File_Message_Param.m_blDelete          = blDelete;
    obj_File_Message_Param.m_pFileTest         = m_pFileTest;
    obj_File_Message_Param.m_pPacketDebugData  = m_pPacketDebugData;
    obj_File_Message_Param.m_u4ConnectID       = GetConnectID();
    obj_File_Message_Param.m_u4PacketDebugSize = m_u4PacketDebugSize;

    return Tcp_Common_File_Message(obj_File_Message_Param,
                                   pBuffPacket,
                                   m_szConnectName);
}

bool CConnectHandler::Send_Input_To_Cache(uint8 u1SendType, uint32& u4PacketSize, uint16 u2CommandID, bool blDelete, IBuffPacket* pBuffPacket)
{
    _Input_To_Cache_Param obj_Input_To_Cache_Param;
    obj_Input_To_Cache_Param.m_blDelete            = blDelete;
    obj_Input_To_Cache_Param.m_u1SendType          = u1SendType;
    obj_Input_To_Cache_Param.m_u2CommandID         = u2CommandID;
    obj_Input_To_Cache_Param.m_u4ConnectID         = m_u4ConnectID;
    obj_Input_To_Cache_Param.m_u4PacketParseInfoID = m_u4PacketParseInfoID;
    obj_Input_To_Cache_Param.m_u4SendMaxBuffSize   = m_u4SendMaxBuffSize;

    return Tcp_Common_Send_Input_To_Cache(obj_Input_To_Cache_Param,
                                          m_pBlockMessage,
                                          u4PacketSize,
                                          pBuffPacket);
}

bool CConnectHandler::Send_Input_To_TCP(uint8 u1SendType, uint32& u4PacketSize, uint16 u2CommandID, uint8 u1State, int nMessageID, bool blDelete, IBuffPacket* pBuffPacket)
{
    ACE_Message_Block* pMbData = NULL;

    _Send_Packet_Param obj_Send_Packet_Param;
    obj_Send_Packet_Param.m_blDelete            = blDelete;
    obj_Send_Packet_Param.m_u1SendType          = u1SendType;
    obj_Send_Packet_Param.m_u2CommandID         = u2CommandID;
    obj_Send_Packet_Param.m_u4ConnectID         = GetConnectID();
    obj_Send_Packet_Param.m_u4PacketParseInfoID = m_u4PacketParseInfoID;
    obj_Send_Packet_Param.m_u4SendMaxBuffSize   = m_u4SendMaxBuffSize;

    //拼装数据
    bool blState = Tcp_Common_Make_Send_Packet(obj_Send_Packet_Param,
                   pBuffPacket,
                   m_pBlockMessage,
                   pMbData,
                   u4PacketSize);

    if (false == blState)
    {
        return false;
    }

    //回收内存
    Recovery_Common_BuffPacket(blDelete, pBuffPacket);

    //判断是否超过阈值
    if (false == CheckSendMask((uint32)pMbData->length()))
    {
        App_MessageBlockManager::instance()->Close(pMbData);
        return false;
    }

    //将消息ID放入MessageBlock
    ACE_Message_Block::ACE_Message_Type objType = ACE_Message_Block::MB_USER + nMessageID;
    pMbData->msg_type(objType);

    //将消息放入队列，让output在反应器线程发送。

    if (false == Send_Block_Queue(pMbData))
    {
        return false;
    }

    //如果需要发送完成后删除，则配置标记位
    if (PACKET_SEND_FIN_CLOSE == u1State)
    {
        m_u1ConnectState = CONNECT_SERVER_CLOSE;
        ACE_Message_Block* pMbData = App_MessageBlockManager::instance()->Create(sizeof(int));

        if (NULL == pMbData)
        {
            OUR_DEBUG((LM_DEBUG, "[CConnectHandler::SendMessage] Connectid=[%d] pMbData is NULL.\n", GetConnectID()));
            return false;
        }

        //添加关闭socket指令
        SendCloseMessage();
    }
    else
    {
        int nWakeupRet = reactor()->schedule_wakeup(this, ACE_Event_Handler::WRITE_MASK);

        if (-1 == nWakeupRet)
        {
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::SendMessage] Connectid=%d, nWakeupRet(%d) output errno = [%d].\n", GetConnectID(), nWakeupRet, errno));
        }
    }

    return true;
}

int CConnectHandler::Dispose_Paceket_Parse_Stream_Single(ACE_Message_Block* pCurrMessage)
{
    uint8 n1Ret = Tcp_Common_Recv_Stream(GetConnectID(), pCurrMessage, m_pPacketParse, m_u4PacketParseInfoID);

    if (PACKET_GET_NO_ENOUGTH == n1Ret)
    {
        return 0;
    }
    else if (PACKET_GET_ENOUGTH == n1Ret)
    {
        if (false == CheckMessage())
        {
            return -1;
        }

        m_u4CurrSize = 0;

        //申请新的包
        m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

        if (NULL == m_pPacketParse)
        {
            OUR_DEBUG((LM_DEBUG, "[%t|CConnectHandle::RecvData] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
            return -1;
        }

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
        m_pPacketParse->Clear();

        AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, m_u8RecvQueueTimeCost = %dws, m_u4RecvQueueCount = %d, m_u8SendQueueTimeCost = %dws.",
                                            m_addrRemote.get_host_addr(),
                                            m_addrRemote.get_port_number(),
                                            m_u4AllRecvSize, m_u4AllRecvCount,
                                            m_u4AllSendSize, m_u4AllSendCount,
                                            (uint32)m_u8RecvQueueTimeCost,
                                            m_u4RecvQueueCount,
                                            (uint32)m_u8SendQueueTimeCost);
        OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvData] pmb new is NULL.\n"));
        return -1;
    }
}

void CConnectHandler::SetConnectName(const char* pName)
{
    sprintf_safe(m_szConnectName, MAX_BUFF_100, "%s", pName);
}

void CConnectHandler::SetIsLog(bool blIsLog)
{
    m_blIsLog = blIsLog;
}

char* CConnectHandler::GetConnectName()
{
    return m_szConnectName;
}

bool CConnectHandler::GetIsLog()
{
    return m_blIsLog;
}

void CConnectHandler::SetHashID(int nHashID)
{
    m_nHashID = nHashID;
}

int CConnectHandler::GetHashID()
{
    return m_nHashID;
}

void CConnectHandler::SetLocalIPInfo(const char* pLocalIP, uint32 u4LocalPort)
{
    sprintf_safe(m_szLocalIP, MAX_BUFF_50, "%s", pLocalIP);
    m_u4LocalPort = u4LocalPort;
}

void CConnectHandler::SetSendCacheManager(CSendCacheManager* pSendCacheManager)
{
    if(NULL != pSendCacheManager)
    {
        m_pBlockMessage = pSendCacheManager->GetCacheData(GetConnectID());
    }
}

bool CConnectHandler::Test_Paceket_Parse_Stream(ACE_Message_Block* pmb)
{
    //申请新的包
    m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

    if (NULL == m_pPacketParse)
    {
        OUR_DEBUG((LM_DEBUG, "[%t|CConnectHandle::Test_Paceket_Parse_Stream] (%d) m_pPacketParse new error.\n", GetConnectID()));
        return false;
    }

    if (0 == Dispose_Paceket_Parse_Stream_Single(pmb))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//***************************************************************************
CConnectManager::CConnectManager(void):m_mutex(), m_cond(m_mutex)
{
    m_u4TimeCheckID      = 0;
    m_szError[0]         = '\0';

    m_pTCTimeSendCheck   = NULL;
    m_tvCheckConnect     = ACE_OS::gettimeofday();
    m_blRun              = false;

    m_u4TimeConnect      = 0;
    m_u4TimeDisConnect   = 0;
    m_u4SendQueuePutTime = 0;
    m_u2SendQueueMax     = MAX_MSG_SENDPACKET;

    //初始化发送对象池
    m_SendMessagePool.Init();
}

CConnectManager::~CConnectManager(void)
{
    OUR_DEBUG((LM_INFO, "[CConnectManager::~CConnectManager].\n"));
}

void CConnectManager::CloseAll()
{
    if(m_blRun)
    {
        if (false == this->CloseMsgQueue())
        {
            OUR_DEBUG((LM_INFO, "[CConnectManager::CloseAll]CloseMsgQueue error.\n"));
        }
    }
    else
    {
        msg_queue()->deactivate();
    }

    if (false == KillTimer())
    {
        OUR_DEBUG((LM_INFO, "[CConnectManager::CloseAll]KillTimer error.\n"));
    }

    vector<CConnectHandler*> vecCloseConnectHandler;
    m_objHashConnectList.Get_All_Used(vecCloseConnectHandler);

    //开始关闭所有连接
    for(int i = 0; i < (int)vecCloseConnectHandler.size(); i++)
    {
        CConnectHandler* pConnectHandler = vecCloseConnectHandler[i];
        pConnectHandler->Close();
    }

    //删除hash表空间
    m_objHashConnectList.Close();

    //删除缓冲对象
    m_SendCacheManager.Close();
}

bool CConnectManager::Close(uint32 u4ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if (false == DelConnectTimeWheel(m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID)))
    {
        OUR_DEBUG((LM_INFO, "[CConnectManager::Close]DelConnectTimeWheel error.\n"));
    }

    AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "[CConnectManager::Close]ConnectID=%d.",
                                        u4ConnectID);

    int nPos = m_objHashConnectList.Del_Hash_Data_By_Unit32(u4ConnectID);

    if(0 > nPos)
    {
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "[CConnectManager::Close]ConnectID=%d FAIL.",
                                            u4ConnectID);

        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectManager::Close] ConnectID[%d] is not find.", u4ConnectID);
    }

    //回收发送缓冲
    m_SendCacheManager.FreeCacheData(u4ConnectID);
    m_u4TimeDisConnect++;

    //加入链接统计功能
    App_ConnectAccount::instance()->AddDisConnect();

    return true;
}

bool CConnectManager::CloseUnLock(uint32 u4ConnectID)
{
    //连接关闭，清除时间轮盘
    if (false == DelConnectTimeWheel(m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID)))
    {
        OUR_DEBUG((LM_INFO, "[CConnectManager::CloseUnLock]DelConnectTimeWheel error.\n"));
    }

    int nPos = m_objHashConnectList.Del_Hash_Data_By_Unit32(u4ConnectID);

    if(0 < nPos)
    {
        //回收发送缓冲
        m_SendCacheManager.FreeCacheData(u4ConnectID);
        m_u4TimeDisConnect++;

        //加入链接统计功能
        App_ConnectAccount::instance()->AddDisConnect();

        OUR_DEBUG((LM_ERROR, "[CConnectManager::CloseUnLock]ConnectID=%d End.\n", u4ConnectID));
        return true;
    }
    else
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectManager::Close] ConnectID[%d] is not find.", u4ConnectID);
        return true;
    }
}

bool CConnectManager::CloseConnect(uint32 u4ConnectID)
{
    m_ThreadWriteLock.acquire();
    CConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);
    m_ThreadWriteLock.release();

    if(NULL != pConnectHandler)
    {
        //放到反应器线程里去做
        if (false == pConnectHandler->SendCloseMessage())
        {
            OUR_DEBUG((LM_INFO, "[CConnectManager::CloseConnect]SendCloseMessage error.\n"));
        }

        return true;
    }
    else
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectManager::CloseConnect] ConnectID[%d] is not find.", u4ConnectID);
        return false;
    }
}

bool CConnectManager::CloseConnect_By_Queue(uint32 u4ConnectID)
{
    //放入发送队列
    return Tcp_Common_CloseConnect_By_Queue(u4ConnectID,
                                            m_SendMessagePool,
                                            m_u4SendQueuePutTime,
                                            dynamic_cast<ACE_Task<ACE_MT_SYNCH>*>(this));
}

bool CConnectManager::AddConnect(uint32 u4ConnectID, CConnectHandler* pConnectHandler)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if(pConnectHandler == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectManager::AddConnect]ConnectID=%d, pConnectHandler is NULL.\n", u4ConnectID));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectManager::AddConnect] pConnectHandler is NULL.");
        return false;
    }

    CConnectHandler* pCurrConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if(NULL != pCurrConnectHandler)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectManager::AddConnect]ConnectID=%d is find.\n", u4ConnectID));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectManager::AddConnect] ConnectID[%d] is exist.", u4ConnectID);
        return false;
    }

    pConnectHandler->SetConnectID(u4ConnectID);
    pConnectHandler->SetSendCacheManager(&m_SendCacheManager);
    //加入Hash数组
    m_objHashConnectList.Add_Hash_Data_By_Key_Unit32(u4ConnectID, pConnectHandler);
    m_u4TimeConnect++;

    //加入链接统计功能
    App_ConnectAccount::instance()->AddConnect();

    //加入时间轮盘
    if (false == SetConnectTimeWheel(pConnectHandler))
    {
        OUR_DEBUG((LM_INFO, "[CConnectManager::AddConnect]SetConnectTimeWheel error.\n"));
    }

    return true;
}

bool CConnectManager::SetConnectTimeWheel(CConnectHandler* pConnectHandler)
{
    bool bAddResult = m_TimeWheelLink.Add_TimeWheel_Object(pConnectHandler);

    if(!bAddResult)
    {
        OUR_DEBUG((LM_ERROR,"[CConnectManager::SetConnectTimeWheel]Fail to set pConnectHandler(0x%08x).\n", pConnectHandler));
    }

    return bAddResult;
}

bool CConnectManager::DelConnectTimeWheel(CConnectHandler* pConnectHandler)
{
    m_TimeWheelLink.Del_TimeWheel_Object(pConnectHandler);
    return true;
}

bool CConnectManager::SendMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint16 u2CommandID, uint8 u1SendState, uint8 u1SendType, ACE_Time_Value& tvSendBegin, bool blDelete, int nMessageID)
{
    //因为是队列调用，所以这里不需要加锁了。
    if(NULL == pBuffPacket)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectManager::SendMessage] ConnectID[%d] pBuffPacket is NULL.", u4ConnectID);
        return false;
    }

    m_ThreadWriteLock.acquire();
    CConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);
    m_ThreadWriteLock.release();

    if(NULL != pConnectHandler)
    {
        uint32 u4PacketSize = 0;

        if (false == pConnectHandler->SendMessage(u2CommandID, pBuffPacket, u1SendState, u1SendType, u4PacketSize, blDelete, nMessageID))
        {
            OUR_DEBUG((LM_ERROR, "[CConnectManager::SendMessage](%d) SendMessage error.\n", u4ConnectID));
        }

        //记录消息发送消耗时间
        ACE_Time_Value tvInterval = ACE_OS::gettimeofday() - tvSendBegin;
        uint32 u4SendCost = (uint32)(tvInterval.msec());
        pConnectHandler->SetSendQueueTimeCost(u4SendCost);

        _ClientIPInfo objClientIP = pConnectHandler->GetLocalIPInfo();
        m_CommandAccount.SaveCommandData(u2CommandID, (uint32)objClientIP.m_nPort, PACKET_TCP, u4PacketSize, COMMAND_TYPE_OUT);
        return true;
    }
    else
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectManager::SendMessage] ConnectID[%d] is not find.", u4ConnectID);
        //如果连接不存在了，在这里返回失败，回调给业务逻辑去处理
        Tcp_Common_Send_Message_Error(blDelete, pBuffPacket);

        if(true == blDelete)
        {
            App_BuffPacketManager::instance()->Delete(pBuffPacket);
        }

        return true;
    }
}

bool CConnectManager::PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    _Post_Message_Param obj_Post_Message_Param;
    obj_Post_Message_Param.m_blDelete = blDelete;
    obj_Post_Message_Param.m_nMessageID = nMessageID;
    obj_Post_Message_Param.m_u1SendState = u1SendState;
    obj_Post_Message_Param.m_u1SendType = u1SendType;
    obj_Post_Message_Param.m_u2CommandID = u2CommandID;
    obj_Post_Message_Param.m_u2SendQueueMax = m_u2SendQueueMax;
    obj_Post_Message_Param.m_u4ConnectID = u4ConnectID;
    obj_Post_Message_Param.m_u4SendQueuePutTime = m_u4SendQueuePutTime;

    return Tcp_Common_Manager_Post_Message(obj_Post_Message_Param,
                                           pBuffPacket,
                                           m_SendMessagePool,
                                           dynamic_cast<ACE_Task<ACE_MT_SYNCH>*>(this));
}

const char* CConnectManager::GetError()
{
    return m_szError;
}

bool CConnectManager::StartTimer()
{
    //启动发送线程
    if(0 != open())
    {
        OUR_DEBUG((LM_ERROR, "[CConnectManager::StartTimer]Open() is error.\n"));
        return false;
    }

    //避免定时器重复启动
    if (false == KillTimer())
    {
        OUR_DEBUG((LM_ERROR, "[CConnectManager::StartTimer]KillTimer error.\n"));
    }

    OUR_DEBUG((LM_ERROR, "[CConnectManager::StartTimer]begin....\n"));
    //得到第二个Reactor
    ACE_Reactor* pReactor = App_ReactorManager::instance()->GetAce_Reactor(REACTOR_POSTDEFINE);

    if(NULL == pReactor)
    {
        OUR_DEBUG((LM_ERROR, "CConnectManager::StartTimer() -->GetAce_Reactor(REACTOR_POSTDEFINE) is NULL.\n"));
        return false;
    }

    m_pTCTimeSendCheck = new _TimerCheckID();

    if(NULL == m_pTCTimeSendCheck)
    {
        OUR_DEBUG((LM_ERROR, "CConnectManager::StartTimer() m_pTCTimeSendCheck is NULL.\n"));
        return false;
    }

    m_pTCTimeSendCheck->m_u2TimerCheckID = PARM_CONNECTHANDLE_CHECK;
    long lTimeCheckID = pReactor->schedule_timer(this,
                        (const void*)m_pTCTimeSendCheck,
                        ACE_Time_Value(GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime, 0),
                        ACE_Time_Value(GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime, 0));

    if(-1 == lTimeCheckID)
    {
        OUR_DEBUG((LM_ERROR, "CConnectManager::StartTimer()--> Start thread m_u4TimeCheckID error.\n"));
        return false;
    }
    else
    {
        m_u4TimeCheckID = (uint32)lTimeCheckID;
        OUR_DEBUG((LM_ERROR, "CConnectManager::StartTimer()--> Start thread time OK.\n"));
        return true;
    }
}

bool CConnectManager::KillTimer()
{
    if(m_u4TimeCheckID > 0)
    {
        App_ReactorManager::instance()->GetAce_Reactor(REACTOR_POSTDEFINE)->cancel_timer(m_u4TimeCheckID);
        m_u4TimeCheckID = 0;
    }

    SAFE_DELETE(m_pTCTimeSendCheck);
    return true;
}

int CConnectManager::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    ACE_UNUSED_ARG(arg);

    if(arg == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectManager::handle_timeout]arg is not NULL, tv = %d.\n", tv.sec()));
    }

    ACE_Time_Value tvNow = tv;

    _TimerCheckID* pTimerCheckID = (_TimerCheckID*)arg;

    if(NULL != pTimerCheckID && pTimerCheckID->m_u2TimerCheckID == PARM_CONNECTHANDLE_CHECK)
    {
        //定时检测发送，这里将定时记录链接信息放入其中，减少一个定时器
        m_TimeWheelLink.Tick();

        //判定是否应该记录链接日志
        Tcp_Common_Manager_Timeout_CheckInfo(GetCount());
    }

    ACE_Time_Value tvEnd = ACE_OS::gettimeofday();

    //得到时间执行了多少秒，并记录日志
    ACE_Time_Value tvCost = tvEnd - tvNow;
    AppLogManager::instance()->WriteLog(LOG_SYSTEM_WORKTHREAD, "[CConnectManager::handle_timeout]TimeCost=%d.", tvCost.msec());

    return 0;
}

void CConnectManager::TimeWheel_Timeout_Callback(void* pArgsContext, vector<CConnectHandler*> vecConnectHandle)
{
    OUR_DEBUG((LM_INFO, "[CConnectManager::TimeWheel_Timeout_Callback]Timeout Count(%d).\n", vecConnectHandle.size()));

    for (int i = 0; i < (int)vecConnectHandle.size(); i++)
    {
        //断开超时的链接
        CConnectManager* pManager = reinterpret_cast<CConnectManager*>(pArgsContext);
        OUR_DEBUG((LM_INFO, "[CConnectManager::TimeWheel_Timeout_Callback]ConnectID(%d).\n", vecConnectHandle[i]->GetConnectID()));

        if (NULL != pManager && false == pManager->CloseConnect_By_Queue(vecConnectHandle[i]->GetConnectID()))
        {
            OUR_DEBUG((LM_INFO, "[CConnectManager::TimeWheel_Timeout_Callback]CloseConnect_By_Queue error.\n"));
        }
    }
}

int CConnectManager::GetCount()
{
    return m_objHashConnectList.Get_Used_Count();
}

int CConnectManager::open(void* args)
{
    if(args != NULL)
    {
        OUR_DEBUG((LM_INFO,"[CConnectManager::open]args is not NULL.\n"));
    }

    m_blRun = true;
    msg_queue()->high_water_mark(MAX_MSG_MASK);
    msg_queue()->low_water_mark(MAX_MSG_MASK);

    OUR_DEBUG((LM_INFO,"[CConnectManager::open] m_u4HighMask = [%d] m_u4LowMask = [%d]\n", MAX_MSG_MASK, MAX_MSG_MASK));

    if(activate(THREAD_PARAM, MAX_MSG_THREADCOUNT) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectManager::open] activate error ThreadCount = [%d].", MAX_MSG_THREADCOUNT));
        m_blRun = false;
        return -1;
    }

    m_u4SendQueuePutTime = GetXmlConfigAttribute(xmlSendInfo)->PutQueueTimeout * 1000;

    resume();

    return 0;
}

int CConnectManager::svc (void)
{
    while(true)
    {
        //处理消息队列信息
        if (false == Dispose_Queue())
        {
            break;
        }
    }

    OUR_DEBUG((LM_INFO,"[CConnectManager::svc] svc finish!\n"));
    return 0;
}

int CConnectManager::close(u_long)
{
    OUR_DEBUG((LM_INFO,"[CConnectManager::close] close().\n"));
    return 0;
}

void CConnectManager::SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost)
{
    CConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if(NULL != pConnectHandler)
    {
        pConnectHandler->SetRecvQueueTimeCost(u4TimeCost);
    }
}

void CConnectManager::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
    vector<CConnectHandler*> vecConnectHandler;
    m_objHashConnectList.Get_All_Used(vecConnectHandler);

    for(int i = 0; i < (int)vecConnectHandler.size(); i++)
    {
        CConnectHandler* pConnectHandler = vecConnectHandler[i];

        if(pConnectHandler != NULL)
        {
            VecClientConnectInfo.push_back(pConnectHandler->GetClientInfo());
        }
    }
}

_ClientIPInfo CConnectManager::GetClientIPInfo(uint32 u4ConnectID)
{
    CConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if(NULL != pConnectHandler)
    {
        return pConnectHandler->GetClientIPInfo();
    }
    else
    {
        _ClientIPInfo ClientIPInfo;
        return ClientIPInfo;
    }
}

_ClientIPInfo CConnectManager::GetLocalIPInfo(uint32 u4ConnectID)
{
    CConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if(NULL != pConnectHandler)
    {
        return pConnectHandler->GetLocalIPInfo();
    }
    else
    {
        _ClientIPInfo ClientIPInfo;
        return ClientIPInfo;
    }
}

bool CConnectManager::PostMessageAll(IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    ACE_UNUSED_ARG(nMessageID);

    m_ThreadWriteLock.acquire();
    vector<CConnectHandler*> objvecConnectManager;
    m_objHashConnectList.Get_All_Used(objvecConnectManager);
    m_ThreadWriteLock.release();

    uint32 u4ConnectID = 0;

    for(uint32 i = 0; i < (uint32)objvecConnectManager.size(); i++)
    {
        IBuffPacket* pCurrBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

        if(NULL == pCurrBuffPacket)
        {
            OUR_DEBUG((LM_INFO, "[CConnectManager::PostMessage]pCurrBuffPacket is NULL.\n"));

            if(true == blDelete)
            {
                App_BuffPacketManager::instance()->Delete(pBuffPacket);
            }

            return false;
        }

        pCurrBuffPacket->WriteStream(pBuffPacket->GetData(), pBuffPacket->GetPacketLen());

        //放入发送队列
        PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, u1SendState, true, 0);
    }

    return true;
}

bool CConnectManager::SetConnectName(uint32 u4ConnectID, const char* pName)
{
    CConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if(NULL != pConnectHandler)
    {
        pConnectHandler->SetConnectName(pName);
        return true;
    }
    else
    {
        return false;
    }
}

bool CConnectManager::SetIsLog(uint32 u4ConnectID, bool blIsLog)
{
    CConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if(NULL != pConnectHandler)
    {
        pConnectHandler->SetIsLog(blIsLog);
        return true;
    }
    else
    {
        return false;
    }
}

void CConnectManager::GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo)
{
    vector<CConnectHandler*> vecConnectHandler;
    m_objHashConnectList.Get_All_Used(vecConnectHandler);

    for(int i = 0; i < (int)vecConnectHandler.size(); i++)
    {
        CConnectHandler* pConnectHandler = vecConnectHandler[i];

        if(NULL != pConnectHandler && ACE_OS::strcmp(pConnectHandler->GetConnectName(), pName) == 0)
        {
            _ClientNameInfo ClientNameInfo = Tcp_Common_ClientNameInfo((int)pConnectHandler->GetConnectID(),
                                             pConnectHandler->GetConnectName(),
                                             pConnectHandler->GetClientIPInfo().m_szClientIP,
                                             pConnectHandler->GetClientIPInfo().m_nPort,
                                             pConnectHandler->GetIsLog());

            objClientNameInfo.push_back(ClientNameInfo);
        }
    }
}

_CommandData* CConnectManager::GetCommandData( uint16 u2CommandID )
{
    return m_CommandAccount.GetCommandData(u2CommandID);
}

void CConnectManager::Init( uint16 u2Index )
{
    //初始化公共的部分
    Tcp_Common_Manager_Init(u2Index, m_CommandAccount, m_u2SendQueueMax, m_SendCacheManager);

    //初始化Hash表
    uint16 u2PoolSize = GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount;
    m_objHashConnectList.Init((int)u2PoolSize);

    //初始化时间轮盘
    m_TimeWheelLink.Init(GetXmlConfigAttribute(xmlClientInfo)->MaxConnectTime,
                         GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime,
                         (int)u2PoolSize,
                         CConnectManager::TimeWheel_Timeout_Callback, (void*)this);
}

void CConnectManager::GetFlowInfo(uint32& u4UdpFlowIn, uint32& u4UdpFlowOut)
{
    u4UdpFlowIn  = m_CommandAccount.GetFlowIn();
    u4UdpFlowOut = m_CommandAccount.GetFlowOut();
}

EM_Client_Connect_status CConnectManager::GetConnectState(uint32 u4ConnectID)
{
    if(NULL == m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID))
    {
        return CLIENT_CONNECT_NO_EXIST;
    }
    else
    {
        return CLIENT_CONNECT_EXIST;
    }
}

int CConnectManager::handle_write_file_stream(uint32 u4ConnectID, const char* pData, uint32 u4Size, uint8 u1ParseID)
{
    CConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if (NULL == pConnectHandler)
    {
        return -1;
    }
    else
    {
        return pConnectHandler->handle_write_file_stream(pData, u4Size, u1ParseID);
    }
}

int CConnectManager::CloseMsgQueue()
{
    return Task_Common_CloseMsgQueue((ACE_Task<ACE_MT_SYNCH>*)this, m_cond, m_mutex);
}

bool CConnectManager::Dispose_Queue()
{
    ACE_Message_Block* mb = NULL;
    ACE_OS::last_error(0);

    if (getq(mb, 0) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectManager::Dispose_Queue] get error errno = [%d].\n", ACE_OS::last_error()));
        m_blRun = false;
        return false;
    }
    else
    {
        if ((0 == mb->size()) && (mb->msg_type() == ACE_Message_Block::MB_STOP))
        {
            m_mutex.acquire();
            mb->release();
            this->msg_queue()->deactivate();
            m_cond.signal();
            m_mutex.release();
            m_blRun = false;
            return false;
        }

        _SendMessage* msg = *((_SendMessage**)mb->base());

        if (0 == msg->m_u1Type)
        {
            //处理发送数据
            if (false == SendMessage(msg->m_u4ConnectID, msg->m_pBuffPacket, msg->m_u2CommandID, msg->m_u1SendState, msg->m_nEvents, msg->m_tvSend, msg->m_blDelete, msg->m_nMessageID))
            {
                OUR_DEBUG((LM_INFO, "[CConnectManager::Dispose_Queue]SendMessage error.\n"));
            }
        }
        else
        {
            //处理连接服务器主动关闭
            if (false == CloseConnect(msg->m_u4ConnectID))
            {
                OUR_DEBUG((LM_INFO, "[CConnectManager::Dispose_Queue]CloseConnect error.\n"));
            }
        }

        m_SendMessagePool.Delete(msg);
    }

    return true;
}

//*********************************************************************************

CConnectHandlerPool::CConnectHandlerPool(void)
{
    //ConnectID计数器从1开始
    m_u4CurrMaxCount = 1;
}

CConnectHandlerPool::CConnectHandlerPool(const CConnectHandlerPool& ar)
{
    (*this) = ar;
}

CConnectHandlerPool::~CConnectHandlerPool(void)
{
    OUR_DEBUG((LM_INFO, "[CConnectHandlerPool::~CConnectHandlerPool].\n"));
    Close();
    OUR_DEBUG((LM_INFO, "[CConnectHandlerPool::~CConnectHandlerPool]End.\n"));
}

void CConnectHandlerPool::Init(int nObjcetCount)
{
    Close();

    //初始化HashTable
    m_objHandlerList.Init((uint32)nObjcetCount);
    m_objHashHandleList.Init((int)nObjcetCount);

    for(int i = 0; i < nObjcetCount; i++)
    {
        CConnectHandler* pHandler = m_objHandlerList.GetObject(i);

        if(NULL != pHandler)
        {
            //将ID和Handler指针的关系存入hashTable
            char szHandlerID[10] = {'\0'};
            sprintf_safe(szHandlerID, 10, "%d", m_u4CurrMaxCount);
            int nHashPos = m_objHashHandleList.Add_Hash_Data(szHandlerID, pHandler);

            if(-1 != nHashPos)
            {
                pHandler->Init(i);
                pHandler->SetHashID(i);
            }

            m_u4CurrMaxCount++;
        }
    }
}

void CConnectHandlerPool::Close()
{
    //清理所有已存在的指针
    m_u4CurrMaxCount  = 1;

    //删除hash表空间
    m_objHashHandleList.Close();
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

    //没找到空余的
    return pHandler;
}

bool CConnectHandlerPool::Delete(CConnectHandler* pObject)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if(NULL == pObject)
    {
        return false;
    }

    char szHandlerID[10] = {'\0'};
    sprintf_safe(szHandlerID, 10, "%d", pObject->GetHandlerID());
    bool blState = m_objHashHandleList.Push(szHandlerID, pObject);

    if(false == blState)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectHandlerPool::Delete]szHandlerID=%s(0x%08x).\n", szHandlerID, pObject));
    }

    return true;
}

//==============================================================
CConnectManagerGroup::CConnectManagerGroup()
{
    m_objConnnectManagerList = NULL;
    m_u4CurrMaxCount         = 0;
    m_u2ThreadQueueCount     = SENDQUEUECOUNT;
}

CConnectManagerGroup::CConnectManagerGroup(const CConnectManagerGroup& ar)
{
    (*this) = ar;
}

CConnectManagerGroup::~CConnectManagerGroup()
{
    OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::~CConnectManagerGroup].\n"));

    Close();
}

void CConnectManagerGroup::Close()
{
    if(NULL != m_objConnnectManagerList)
    {
        for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
        {
            CConnectManager* pConnectManager = m_objConnnectManagerList[i];
            SAFE_DELETE(pConnectManager);
        }
    }

    SAFE_DELETE_ARRAY(m_objConnnectManagerList);
    m_u2ThreadQueueCount = 0;
}

void CConnectManagerGroup::Init(uint16 u2SendQueueCount)
{
    Close();

    m_objConnnectManagerList = new CConnectManager*[u2SendQueueCount];
    memset(m_objConnnectManagerList, 0, sizeof(CConnectManager*)*u2SendQueueCount);

    for(int i = 0; i < (int)u2SendQueueCount; i++)
    {
        CConnectManager* pConnectManager = new CConnectManager();

        //初始化统计器
        pConnectManager->Init((uint16)i);
        //加入数组
        m_objConnnectManagerList[i] = pConnectManager;
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::Init]Creat %d SendQueue OK.\n", i));
    }

    m_u2ThreadQueueCount = u2SendQueueCount;
}

uint32 CConnectManagerGroup::GetGroupIndex()
{
    //根据链接获得命中，（简单球形命中算法）
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    m_u4CurrMaxCount++;
    return m_u4CurrMaxCount;
}

bool CConnectManagerGroup::AddConnect(CConnectHandler* pConnectHandler)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

    uint32 u4ConnectID = GetGroupIndex();

    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::AddConnect]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->AddConnect(u4ConnectID, pConnectHandler);
}

bool CConnectManagerGroup::SetConnectTimeWheel(CConnectHandler* pConnectHandler)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

    uint32 u4ConnectID = pConnectHandler->GetConnectID();

    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if (NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::AddConnect]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->SetConnectTimeWheel(pConnectHandler);
}

bool CConnectManagerGroup::DelConnectTimeWheel(CConnectHandler* pConnectHandler)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

    uint32 u4ConnectID = pConnectHandler->GetConnectID();

    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if (NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::AddConnect]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->DelConnectTimeWheel(pConnectHandler);
}

bool CConnectManagerGroup::PostMessage(uint32 u4ConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nServerID)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager =  m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, u1SendState, blDelete, nServerID);
}

bool CConnectManagerGroup::PostMessage( uint32 u4ConnectID, char*& pData, uint32 nDataLen, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nServerID)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager =  m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage]No find send Queue object.\n"));

        if(true == blDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

    IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    if(NULL != pBuffPacket)
    {
        bool bWriteResult = pBuffPacket->WriteStream(pData, nDataLen);

        if(true == blDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        if(bWriteResult)
        {
            return pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, u1SendState, true, nServerID);
        }
        else
        {
            return false;
        }
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage]pBuffPacket is NULL.\n"));

        if(true == blDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }
}

bool CConnectManagerGroup::PostMessage( vector<uint32> vecConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nServerID)
{
    uint32 u4ConnectID = 0;

    for(uint32 i = 0; i < (uint32)vecConnectID.size(); i++)
    {
        //判断命中到哪一个线程组里面去
        u4ConnectID = vecConnectID[i];
        uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

        CConnectManager* pConnectManager =  m_objConnnectManagerList[u2ThreadIndex];

        if(NULL == pConnectManager)
        {
            continue;
        }

        //为每一个Connect设置发送对象数据包
        IBuffPacket* pCurrBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

        if(NULL == pCurrBuffPacket)
        {
            continue;
        }

        pCurrBuffPacket->WriteStream(pBuffPacket->GetData(), pBuffPacket->GetWriteLen());

        if (false == pConnectManager->PostMessage(u4ConnectID, pCurrBuffPacket, u1SendType, u2CommandID, u1SendState, true, nServerID))
        {
            OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage](%d)PostMessage error.\n", u4ConnectID));
        }
    }

    if(true == blDelete)
    {
        App_BuffPacketManager::instance()->Delete(pBuffPacket);
    }

    return true;
}

bool CConnectManagerGroup::PostMessage( vector<uint32> vecConnectID, char*& pData, uint32 nDataLen, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nServerID)
{
    uint32 u4ConnectID = 0;

    for(uint32 i = 0; i < (uint32)vecConnectID.size(); i++)
    {
        //判断命中到哪一个线程组里面去
        u4ConnectID = vecConnectID[i];
        uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

        CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

        if(NULL == pConnectManager)
        {
            OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage]No find send Queue object.\n"));
            continue;
        }

        //为每一个Connect设置发送对象数据包
        IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

        if(NULL == pBuffPacket)
        {
            continue;
        }

        pBuffPacket->WriteStream(pData, nDataLen);

        if (false == pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, u1SendState, true, nServerID))
        {
            OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage](%d)PostMessage error.\n", u4ConnectID));
        }
    }

    if(true == blDelete)
    {
        SAFE_DELETE_ARRAY(pData);
    }

    return true;
}

bool CConnectManagerGroup::CloseConnect(uint32 u4ConnectID)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::CloseConnect]No find send Queue object.\n"));
        return false;
    }

    //通过消息队列去实现关闭
    return pConnectManager->CloseConnect_By_Queue(u4ConnectID);
}

bool CConnectManagerGroup::CloseConnectByClient(uint32 u4ConnectID)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::CloseConnectByClient]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->Close(u4ConnectID);
}

_ClientIPInfo CConnectManagerGroup::GetClientIPInfo(uint32 u4ConnectID)
{
    _ClientIPInfo objClientIPInfo;
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
        return objClientIPInfo;
    }

    return pConnectManager->GetClientIPInfo(u4ConnectID);
}

_ClientIPInfo CConnectManagerGroup::GetLocalIPInfo(uint32 u4ConnectID)
{
    _ClientIPInfo objClientIPInfo;
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::GetLocalIPInfo]No find send Queue object.\n"));
        return objClientIPInfo;
    }

    return pConnectManager->GetLocalIPInfo(u4ConnectID);
}


void CConnectManagerGroup::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CConnectManager* pConnectManager = m_objConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            pConnectManager->GetConnectInfo(VecClientConnectInfo);
        }
    }
}

int CConnectManagerGroup::GetCount()
{
    uint32 u4Count = 0;

    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CConnectManager* pConnectManager = m_objConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            u4Count += pConnectManager->GetCount();
        }
    }

    return u4Count;
}

void CConnectManagerGroup::CloseAll()
{
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CConnectManager* pConnectManager = m_objConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            pConnectManager->CloseAll();
        }
    }
}

bool CConnectManagerGroup::StartTimer()
{
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CConnectManager* pConnectManager = m_objConnnectManagerList[i];

        if(NULL != pConnectManager && false == pConnectManager->StartTimer())
        {
            OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::StartTimer]StartTimer error.\n"));
        }
    }

    return true;
}

bool CConnectManagerGroup::Close(uint32 u4ConnectID)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->Close(u4ConnectID);
}

bool CConnectManagerGroup::CloseUnLock(uint32 u4ConnectID)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->CloseUnLock(u4ConnectID);
}

const char* CConnectManagerGroup::GetError()
{
    return (char* )"";
}

void CConnectManagerGroup::SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
        return;
    }

    pConnectManager->SetRecvQueueTimeCost(u4ConnectID, u4TimeCost);
}

bool CConnectManagerGroup::PostMessageAll( IBuffPacket*& pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    //全部群发
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CConnectManager* pConnectManager = m_objConnnectManagerList[i];

        if(NULL == pConnectManager)
        {
            OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage]No find send Queue object.\n"));
            continue;
        }

        if (false == pConnectManager->PostMessageAll(pBuffPacket, u1SendType, u2CommandID, u1SendState, false, nMessageID))
        {
            OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessageAll](u2CommandID=0x%04x)PostMessageAll error.\n", u2CommandID));
        }
    }

    //用完了就删除
    if(true == blDelete)
    {
        App_BuffPacketManager::instance()->Delete(pBuffPacket);
    }

    return true;
}

bool CConnectManagerGroup::PostMessageAll( char*& pData, uint32 nDataLen, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    if(NULL == pBuffPacket)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessageAll]pBuffPacket is NULL.\n"));

        if(true == blDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }
    else
    {
        pBuffPacket->WriteStream(pData, nDataLen);
    }

    //全部群发
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CConnectManager* pConnectManager = m_objConnnectManagerList[i];

        if(NULL == pConnectManager)
        {
            OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage]No find send Queue object.\n"));
            continue;
        }

        if (false == pConnectManager->PostMessageAll(pBuffPacket, u1SendType, u2CommandID, u1SendState, false, nMessageID))
        {
            OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessageAll](u2CommandID=0x%04x)PostMessageAll error.\n", u2CommandID));
        }
    }

    App_BuffPacketManager::instance()->Delete(pBuffPacket);

    //用完了就删除
    if(true == blDelete)
    {
        SAFE_DELETE_ARRAY(pData);
    }

    return true;
}

bool CConnectManagerGroup::SetConnectName(uint32 u4ConnectID, const char* pName)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::SetConnectName]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->SetConnectName(u4ConnectID, pName);
}

bool CConnectManagerGroup::SetIsLog(uint32 u4ConnectID, bool blIsLog)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::SetIsLog]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->SetIsLog(u4ConnectID, blIsLog);
}

void CConnectManagerGroup::GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo)
{
    objClientNameInfo.clear();

    //全部查找
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CConnectManager* pConnectManager = m_objConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            pConnectManager->GetClientNameInfo(pName, objClientNameInfo);
        }
    }
}

void CConnectManagerGroup::GetCommandData( uint16 u2CommandID, _CommandData& objCommandData )
{
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CConnectManager* pConnectManager = m_objConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            _CommandData* pCommandData = pConnectManager->GetCommandData(u2CommandID);

            if(pCommandData != NULL)
            {
                objCommandData += (*pCommandData);
            }
        }
    }
}

void CConnectManagerGroup::GetFlowInfo(uint32& u4UdpFlowIn, uint32& u4UdpFlowOut)
{
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        uint32 u4ConnectFlowIn  = 0;
        uint32 u4ConnectFlowOut = 0;
        CConnectManager* pConnectManager = m_objConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            pConnectManager->GetFlowInfo(u4ConnectFlowIn, u4ConnectFlowOut);
            u4UdpFlowIn  += u4ConnectFlowIn;
            u4UdpFlowOut += u4ConnectFlowOut;
        }
    }
}

EM_Client_Connect_status CConnectManagerGroup::GetConnectState(uint32 u4ConnectID)
{
    //判断命中到哪一个线程组里面
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::GetConnectState]No find send Queue object.\n"));
        return CLIENT_CONNECT_NO_EXIST;
    }

    return pConnectManager->GetConnectState(u4ConnectID);
}

int CConnectManagerGroup::handle_write_file_stream(uint32 u4ConnectID, const char* pData, uint32 u4Size, uint8 u1ParseID)
{
    //判断命中到哪一个线程组里面
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if (NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::handle_write_file_stream]No find send Queue object.\n"));
        return CLIENT_CONNECT_NO_EXIST;
    }

    return pConnectManager->handle_write_file_stream(u4ConnectID, pData, u4Size, u1ParseID);
}

CConnectManager* CConnectManagerGroup::GetManagerFormList(int nIndex)
{
    if (nIndex < 0 || nIndex >= m_u2ThreadQueueCount)
    {
        return NULL;
    }
    else
    {
        return m_objConnnectManagerList[nIndex];
    }
}
