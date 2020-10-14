#include "ProConnectHandler.h"

CProConnectHandler::CProConnectHandler(void) : m_u2LocalPort(0), m_u4SendCheckTime(0)
{
    m_szError[0]           = '\0';
    m_u4ConnectID          = 0;
    m_u4AllRecvCount       = 0;
    m_u4AllSendCount       = 0;
    m_u4AllRecvSize        = 0;
    m_u4AllSendSize        = 0;
    m_nIOCount             = 0;
    m_u4HandlerID          = 0;
    m_u2MaxConnectTime     = 0;
    m_u4SendThresHold      = MAX_MSG_SNEDTHRESHOLD;
    m_u1ConnectState       = CONNECTSTATE::CONNECT_INIT;
    m_u1SendBuffState      = CONNECTSTATE::CONNECT_SENDNON;
    m_pPacketParse         = NULL;
    m_u4MaxPacketSize      = MAX_MSG_PACKETLENGTH;
    m_u8RecvQueueTimeCost  = 0;
    m_u4RecvQueueCount     = 0;
    m_u8SendQueueTimeCost  = 0;
    m_u4ReadSendSize       = 0;
    m_u4SuccessSendSize    = 0;
    m_u1IsActive           = 0;
    m_pBlockMessage        = NULL;
    m_u2SendQueueTimeout   = MAX_QUEUE_TIMEOUT * 1000;  //目前因为记录的是微秒，所以这里相应的扩大1000倍
    m_u2RecvQueueTimeout   = MAX_QUEUE_TIMEOUT * 1000;  //目前因为记录的是微秒，所以这里相应的扩大1000倍
    m_u2TcpNodelay         = TCP_NODELAY_ON;
    m_u4SendMaxBuffSize    = 5*1024;
    m_nHashID              = 0;
    m_szConnectName[0]     = '\0';
    m_blIsLog              = false;
    m_szLocalIP[0]         = '\0';
    m_u4RecvPacketCount    = 0;
    m_u4PacketParseInfoID  = 0;
    m_u4PacketDebugSize    = 0;
    m_pPacketDebugData     = NULL;
    m_emIOType             = EM_IO_TYPE::NET_INPUT;
    m_pFileTest            = NULL;
}

CProConnectHandler::~CProConnectHandler(void)
{
    SAFE_DELETE_ARRAY(m_pPacketDebugData);
    m_u4PacketDebugSize = 0;
}

void CProConnectHandler::Init(uint16 u2HandlerID)
{
    m_u4HandlerID = u2HandlerID;
    m_u2MaxConnectTime = GetXmlConfigAttribute(xmlClientInfo)->MaxConnectTime;
    m_u4SendThresHold = GetXmlConfigAttribute(xmlSendInfo)->SendTimeout;
    m_u4MaxPacketSize = GetXmlConfigAttribute(xmlRecvInfo)->RecvBuffSize;
    m_u2TcpNodelay = GetXmlConfigAttribute(xmlSendInfo)->TcpNodelay;

    m_u2SendQueueTimeout = GetXmlConfigAttribute(xmlSendInfo)->SendQueueTimeout * 1000;

    if (m_u2SendQueueTimeout == 0)
    {
        m_u2SendQueueTimeout = MAX_QUEUE_TIMEOUT * 1000;
    }

    m_u2RecvQueueTimeout = GetXmlConfigAttribute(xmlRecvInfo)->RecvQueueTimeout * 1000;

    if (m_u2RecvQueueTimeout == 0)
    {
        m_u2RecvQueueTimeout = MAX_QUEUE_TIMEOUT * 1000;
    }

    m_u4SendMaxBuffSize = GetXmlConfigAttribute(xmlSendInfo)->MaxBlockSize;

    m_pPacketDebugData = new char[GetXmlConfigAttribute(xmlServerType)->DebugSize];
    m_u4PacketDebugSize = GetXmlConfigAttribute(xmlServerType)->DebugSize / 5;

    m_PerformanceCounter.init("SendThread");
}


uint32 CProConnectHandler::GetHandlerID()
{
    return m_u4HandlerID;
}

const char* CProConnectHandler::GetError()
{
    return m_szError;
}

void CProConnectHandler::Close(int nIOCount, int nErrno)
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

    if(m_nIOCount == 0)
    {
        m_u1IsActive = 0;
    }

    m_ThreadWriteLock.release();

    if(m_nIOCount == 0)
    {
        m_ThreadWriteLock.acquire();

        //调用连接断开消息，通知PacketParse接口
        App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->DisConnect(GetConnectID());

        //通知逻辑接口，连接已经断开
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64dns, SendQueueTimeCost=%I64dns.",
                                              m_addrRemote.get_host_addr(),
                                              m_addrRemote.get_port_number(),
                                              m_u4AllRecvSize,
                                              m_u4AllRecvCount,
                                              m_u4AllSendSize,
                                              m_u4AllSendCount,
                                              m_u4RecvQueueCount,
                                              m_u8RecvQueueTimeCost,
                                              m_u8SendQueueTimeCost);




        //如果是服务器关闭，则不理，因为连接已经清理掉了
        if (CONNECTSTATE::CONNECT_SERVER_CLOSE != m_u1ConnectState)
        {
            //组织数据
			_MakePacket objMakePacket;

			objMakePacket.m_u4ConnectID     = GetConnectID();
			objMakePacket.m_pPacketParse    = NULL;
			objMakePacket.m_u1Option        = PACKET_CDISCONNECT;
			objMakePacket.m_AddrRemote      = m_addrRemote;
			objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

            Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

            m_Reader.cancel();
            m_Writer.cancel();

            ACE_OS::shutdown(this->handle(), SD_BOTH);

            if (this->handle() != ACE_INVALID_HANDLE)
            {
                ACE_OS::closesocket(this->handle());
                this->handle(ACE_INVALID_HANDLE);
            }
        }

        m_ThreadWriteLock.release();

        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::Close](0x%08x)Close(ConnectID=%d) OK.\n", this, GetConnectID()));

        //删除存在列表中的对象引用,这里加一个判定，如果是0说明当前连接尚未完成Manager添加。
        if (GetConnectID() > 0)
        {
            App_ProConnectManager::instance()->Close(GetConnectID());
        }

        //清理转发接口
        if ("" != m_strDeviceName)
        {
            App_ForwardManager::instance()->DisConnectRegedit(m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), ENUM_FORWARD_TCP_CLINET);
            m_strDeviceName = "";
        }

        //将对象指针放入空池中
        App_ProConnectHandlerPool::instance()->Delete(this);
    }
}

bool CProConnectHandler::ServerClose(EM_Client_Close_status emStatus, uint8 u1OptionEvent)
{
    if (EM_IO_TYPE::NET_INPUT == m_emIOType)
    {
        if (CLIENT_CLOSE_IMMEDIATLY == emStatus)
        {
            //组织数据
			_MakePacket objMakePacket;

			objMakePacket.m_u4ConnectID     = GetConnectID();
			objMakePacket.m_pPacketParse    = NULL;
			objMakePacket.m_u1Option        = u1OptionEvent;
			objMakePacket.m_AddrRemote      = m_addrRemote;
			objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

            Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

            if (PACKET_SDISCONNECT == u1OptionEvent)
            {
                OUR_DEBUG((LM_ERROR, "[CProConnectHandler::ServerClose]ConnectID = %d, u1OptionEvent = PACKET_SDISCONNECT.\n", GetConnectID()));
            }
            else
            {
                OUR_DEBUG((LM_ERROR, "[CProConnectHandler::ServerClose]ConnectID = %d, u1OptionEvent = PACKET_CDISCONNECT.\n", GetConnectID()));
            }

            m_Reader.cancel();
            m_Writer.cancel();

            ACE_OS::shutdown(this->handle(), SD_BOTH);

            if (this->handle() != ACE_INVALID_HANDLE)
            {
                ACE_OS::closesocket(this->handle());
                this->handle(ACE_INVALID_HANDLE);
            }

            m_u1ConnectState = CONNECTSTATE::CONNECT_SERVER_CLOSE;

        }
    }

    return true;
}

void CProConnectHandler::SetConnectID(uint32 u4ConnectID)
{
    m_u4ConnectID = u4ConnectID;
}

uint32 CProConnectHandler::GetConnectID()
{
    return m_u4ConnectID;
}

void CProConnectHandler::addresses (const ACE_INET_Addr& remote_address, const ACE_INET_Addr& local_address)
{
    m_addrRemote = remote_address;
}

uint32 CProConnectHandler::file_open(IFileTestManager* pFileTest)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    m_atvConnect = ACE_OS::gettimeofday();
    m_atvInput = ACE_OS::gettimeofday();
    m_atvOutput = ACE_OS::gettimeofday();
    m_atvSendAlive = ACE_OS::gettimeofday();

    m_u4AllRecvCount = 0;
    m_u4AllSendCount = 0;
    m_u4AllRecvSize = 0;
    m_u4AllSendSize = 0;
    m_u4RecvPacketCount = 0;
    m_nIOCount = 1;
    m_u8RecvQueueTimeCost = 0;
    m_u4RecvQueueCount = 0;
    m_u8SendQueueTimeCost = 0;
    m_u4SuccessSendSize = 0;
    m_u4ReadSendSize = 0;
    m_blIsLog = false;
    m_szConnectName[0] = '\0';
    m_u1IsActive = 1;
    m_emIOType = EM_IO_TYPE::FILE_INPUT;

    //初始化检查器
    m_TimeConnectInfo.Init(GetXmlConfigAttribute(xmlClientData)->RecvPacketCount,
                           GetXmlConfigAttribute(xmlClientData)->RecvDataMax,
                           GetXmlConfigAttribute(xmlClientData)->SendPacketCount,
                           GetXmlConfigAttribute(xmlClientData)->SendDataMax);

    //将这个链接放入链接库
    if (false == App_ProConnectManager::instance()->AddConnect(this))
    {
        OUR_DEBUG((LM_ERROR, "%s.\n", App_ProConnectManager::instance()->GetError()));
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", App_ProConnectManager::instance()->GetError());
        return 0;
    }

    //组织数据
	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID     = GetConnectID();
	objMakePacket.m_pPacketParse    = NULL;
	objMakePacket.m_u1Option        = PACKET_CONNECT;
	objMakePacket.m_AddrRemote      = m_addrRemote;
	objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

    Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

    OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::file_open]Open(%d) Connection from [%s:%d](0x%08x).\n", GetConnectID(), m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), this));
    m_pFileTest = pFileTest;

    return GetConnectID();
}

int CProConnectHandler::handle_write_file_stream(const char* pData, uint32 u4Size, uint8 u1ParseID)
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

        if (NULL == pMbHead)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_write_file_stream](%d) pMbHead is NULL.\n", GetConnectID()));
            return -1;
        }

        memcpy_safe((char*)pData, u4PacketHead, pMbHead->wr_ptr(), u4PacketHead);
        pMbHead->wr_ptr(u4PacketHead);

        //解析消息头
        _Head_Info objHeadInfo;
        bool blStateHead = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Head_Info(GetConnectID(), pMbHead, App_MessageBlockManager::instance(), &objHeadInfo, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

        if (false == blStateHead)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_write_file_stream](%d) Parse_Packet_Head_Info is illegal.\n", GetConnectID()));
            ClearPacketParse(*pMbHead);
            return -1;
        }

        m_pPacketParse->SetPacket_IsHandleHead(false);
        m_pPacketParse->SetPacket_Head_Message(objHeadInfo.m_pmbHead);
        m_pPacketParse->SetPacket_Head_Curr_Length(objHeadInfo.m_u4HeadCurrLen);
        m_pPacketParse->SetPacket_Body_Src_Length(objHeadInfo.m_u4BodySrcLen);
        m_pPacketParse->SetPacket_CommandID(objHeadInfo.m_u2PacketCommandID);

        //解析消息体
        ACE_Message_Block* pMbBody = App_MessageBlockManager::instance()->Create(u4Size - u4PacketHead);

        if (NULL == pMbBody)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_write_file_stream](%d) pMbHead is NULL.\n", GetConnectID()));
            return -1;
        }

        memcpy_safe((char*)&pData[u4PacketHead], u4Size - u4PacketHead, pMbBody->wr_ptr(), u4Size - u4PacketHead);
        pMbBody->wr_ptr(u4Size - u4PacketHead);

        //解析数据包体
        _Body_Info obj_Body_Info;
        bool blStateBody = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Body_Info(GetConnectID(),
                           pMbBody,
                           App_MessageBlockManager::instance(),
                           &obj_Body_Info,
                           EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

        if (false == blStateBody)
        {
            //如果数据包体非法，断开连接
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_write_file_stream]Parse_Packet_Body_Info is illegal.\n"));

            //清理PacketParse
            ClearPacketParse(*pMbBody);
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
        ACE_Message_Block* pMbStream = App_MessageBlockManager::instance()->Create(u4Size);

        if (NULL == pMbStream)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_write_file_stream](%d) pMbHead is NULL.\n", GetConnectID()));
            return -1;
        }

        memcpy_safe((char*)pData, u4Size, pMbStream->wr_ptr(), u4Size);

        _Packet_Info obj_Packet_Info;
        uint8 n1Ret = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Stream(GetConnectID(), pMbStream, dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()), &obj_Packet_Info, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

        if (PACKET_GET_ENOUGH == n1Ret)
        {
            m_pPacketParse->SetPacket_Head_Message(obj_Packet_Info.m_pmbHead);
            m_pPacketParse->SetPacket_Body_Message(obj_Packet_Info.m_pmbBody);
            m_pPacketParse->SetPacket_CommandID(obj_Packet_Info.m_u2PacketCommandID);
            m_pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4HeadSrcLen);
            m_pPacketParse->SetPacket_Head_Curr_Length(obj_Packet_Info.m_u4HeadCurrLen);
            m_pPacketParse->SetPacket_Body_Src_Length(obj_Packet_Info.m_u4BodySrcLen);
            m_pPacketParse->SetPacket_Body_Curr_Length(obj_Packet_Info.m_u4BodyCurrLen);

            //已经接收了完整数据包，扔给工作线程去处理
            if (false == CheckMessage())
            {
                OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_write_file_stream]CheckMessage is false.\n"));
                return -1;
            }

        }
        else if (PACKET_GET_NO_ENOUGH == n1Ret)
        {
            return 0;
        }
        else
        {
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_write_file_stream]Parse_Packet_Stream is PACKET_GET_ERROR.\n"));
            ClearPacketParse(*pMbStream);
            return -1;
        }
    }

    return 0;
}

void CProConnectHandler::open(ACE_HANDLE h, ACE_Message_Block&)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    m_atvConnect      = ACE_OS::gettimeofday();
    m_atvInput        = ACE_OS::gettimeofday();
    m_atvOutput       = ACE_OS::gettimeofday();
    m_atvSendAlive    = ACE_OS::gettimeofday();

    m_u4ConnectID         = 0;
    m_u4AllRecvCount      = 0;
    m_u4AllSendCount      = 0;
    m_u4AllRecvSize       = 0;
    m_u4AllSendSize       = 0;
    m_u4RecvPacketCount   = 0;
    m_nIOCount            = 1;
    m_u8RecvQueueTimeCost = 0;
    m_u4RecvQueueCount    = 0;
    m_u8SendQueueTimeCost = 0;
    m_u4SuccessSendSize   = 0;
    m_u4ReadSendSize      = 0;
    m_blIsLog             = false;
    m_szConnectName[0]    = '\0';
    m_u1IsActive          = 1;
    m_emIOType            = EM_IO_TYPE::NET_INPUT;

    if (NULL == App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID))
    {
        //如果解析器不存在，则直接断开连接
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::open](%s)can't find PacketParseInfo.\n", m_addrRemote.get_host_addr()));
        Close();
        return;
    }

    if(App_ForbiddenIP::instance()->CheckIP(m_addrRemote.get_host_addr()) == false)
    {
        //在禁止列表中，不允许访问
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::open]IP Forbidden(%s).\n", m_addrRemote.get_host_addr()));
        Close();
        return;
    }

    //检查单位时间链接次数是否达到上限
    if(false == App_IPAccount::instance()->AddIP((string)m_addrRemote.get_host_addr()))
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::open]IP(%s) connect frequently.\n", m_addrRemote.get_host_addr()));
        App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), GetXmlConfigAttribute(xmlIP)->Timeout);

        //发送告警邮件
        AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECT,
                GetXmlConfigAttribute(xmlClientData)->MailID,
                "Alert IP",
                "[CProConnectHandler::open] IP is more than IP Max,");

        Close();
        return;
    }

    if(m_u2TcpNodelay == TCP_NODELAY_OFF)
    {
        //如果设置了禁用Nagle算法，则这里要禁用。
        int nOpt = 1;
        ACE_OS::setsockopt(h, IPPROTO_TCP, TCP_NODELAY, (char* )&nOpt, sizeof(int));
    }

    //初始化检查器
    m_TimeConnectInfo.Init(GetXmlConfigAttribute(xmlClientData)->RecvPacketCount,
                           GetXmlConfigAttribute(xmlClientData)->RecvDataMax,
                           GetXmlConfigAttribute(xmlClientData)->SendPacketCount,
                           GetXmlConfigAttribute(xmlClientData)->SendDataMax);

    this->handle(h);

    //默认别名是IP地址
    SetConnectName(m_addrRemote.get_host_addr());

    if(this->m_Reader.open(*this, h, 0, proactor()) == -1 ||
       this->m_Writer.open(*this, h, 0, proactor()) == -1)
    {
        OUR_DEBUG((LM_DEBUG,"[CProConnectHandler::open] m_reader or m_reader == 0.\n"));
        Close();
        return;
    }

    //将这个链接放入链接库
    if(false == App_ProConnectManager::instance()->AddConnect(this))
    {
        OUR_DEBUG((LM_ERROR, "%s.\n", App_ProConnectManager::instance()->GetError()));
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", App_ProConnectManager::instance()->GetError());
        Close();
        return;
    }

    //写入连接日志
    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Connection from [%s:%d] ConnectID=%d, GetHandlerID=%d.",
                                          m_addrRemote.get_host_addr(),
                                          m_addrRemote.get_port_number(),
                                          GetConnectID(),
                                          GetHandlerID());

    m_u1ConnectState = CONNECTSTATE::CONNECT_OPEN;

    m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

    if(NULL == m_pPacketParse)
    {
        OUR_DEBUG((LM_DEBUG,"[CProConnectHandler::open] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
        Close();
        return;
    }

    //告诉PacketParse连接应建立
    App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Connect(GetConnectID(), GetClientIPInfo(), GetLocalIPInfo());

    //组织数据
	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID = GetConnectID();
	objMakePacket.m_pPacketParse = NULL;
	objMakePacket.m_u1Option = PACKET_CONNECT;
	objMakePacket.m_AddrRemote = m_addrRemote;
	objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

    Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

    OUR_DEBUG((LM_DEBUG,"[CProConnectHandler::open]Open(%d) Connection from [%s:%d](0x%08x).\n", GetConnectID(), m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), this));

    //查看是否存在转发服务
    m_strDeviceName = App_ForwardManager::instance()->ConnectRegedit(m_addrRemote.get_host_addr(),
                      m_addrRemote.get_port_number(),
                      ENUM_FORWARD_TCP_CLINET,
                      dynamic_cast<IDeviceHandler*>(this));

    //获得接收缓冲区大小
    Get_Recv_length();

    return;
}

void CProConnectHandler::handle_read_stream(const ACE_Asynch_Read_Stream::Result& result)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    ACE_Message_Block& mb = result.message_block();
    uint32 u4PacketLen = (uint32)result.bytes_transferred();
    int nTran = (int)result.bytes_transferred();

    if(!result.success() || result.bytes_transferred() == 0 || this->handle() == ACE_INVALID_HANDLE)
    {
        //链接断开
        //清理PacketParse
        ClearPacketParse(mb);

        //关闭当前连接
        Close(2, errno);

        return;
    }

    //查看是否需要转发
    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->SendData(m_strDeviceName, &mb);
        mb.reset();
        return;
    }

    m_atvInput = ACE_OS::gettimeofday();

    Output_Debug_Data(&mb, LOG_SYSTEM_DEBUG_CLIENTRECV);

    char szData[10] = { '\0' };

    if (m_pPacketParse->GetIsHandleHead() == true)
    {
        sprintf_safe(szData, 10, "true");
    }
    else
    {
        sprintf_safe(szData, 10, "false");
    }

    if(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        if(result.bytes_transferred() < result.bytes_to_read())
        {
            //短读，继续读
            OUR_DEBUG((LM_INFO, "[CProConnectHandler::handle_read_stream]bytes_to_read=%d,bytes_transferred=%d.\n",
                       result.bytes_to_read(),
                       result.bytes_transferred()));
            int nRead = (int)result.bytes_to_read() - (int)result.bytes_transferred();

            if(-1 == m_Reader.read(mb, nRead))
            {
                //清理PacketParse
                ClearPacketParse(mb);

                //关闭当前连接
                Close(2, errno);
                return;
            }

            return;
        }
        else if(mb.length() == App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength && m_pPacketParse->GetIsHandleHead())
        {
            //处理数据包头
            if (0 != Dispose_Paceket_Parse_Head(&mb))
            {
                return;
            }
        }
        else
        {
            //处理数据包体
            if (0 != Dispose_Paceket_Parse_Body(&mb))
            {
                return;
            }
        }
    }
    else
    {
        int nStream = Dispose_Paceket_Parse_Stream(&mb);

        //清理处理完的数据缓冲区
        App_MessageBlockManager::instance()->Close(&mb);

        if (0 != nStream)
        {
            return;
        }
    }

    Get_Recv_length();

    return;
}

void CProConnectHandler::handle_write_stream(const ACE_Asynch_Write_Stream::Result& result)
{
    if(!result.success() || result.bytes_transferred()==0)
    {
        //发送失败
        int nErrno = errno;
        OUR_DEBUG ((LM_DEBUG,"[CProConnectHandler::handle_write_stream] Connectid=[%d] begin(%d)...\n",GetConnectID(), nErrno));

        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "WriteError [%s:%d] nErrno = %d  result.bytes_transferred() = %d, ",
                                              m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), nErrno,
                                              result.bytes_transferred());

        OUR_DEBUG((LM_DEBUG,"[CProConnectHandler::handle_write_stream] Connectid=[%d] finish ok...\n", GetConnectID()));
        m_atvOutput = ACE_OS::gettimeofday();

        //错误消息回调
        App_MakePacket::instance()->PutSendErrorMessage(GetConnectID(), &result.message_block(), m_atvOutput);


        return;
    }
    else
    {
        //发送成功
        m_ThreadWriteLock.acquire();
        m_atvOutput = ACE_OS::gettimeofday();
        m_u4AllSendSize += (uint32)result.bytes_to_write();

        int nMessageID = result.message_block().msg_type() - ACE_Message_Block::MB_USER;

        if(nMessageID > 0)
        {
            //需要回调发送成功回执
            CPacketParse objPacketParse;
            ACE_Message_Block* pMbData = App_MessageBlockManager::instance()->Create(sizeof(int));
            memcpy_safe((char* )&nMessageID, sizeof(int), pMbData->wr_ptr(), sizeof(int));
            pMbData->wr_ptr(sizeof(int));
            objPacketParse.SetPacket_Head_Message(pMbData);
            objPacketParse.SetPacket_Head_Curr_Length((uint32)pMbData->length());

			_MakePacket objMakePacket;

			objMakePacket.m_u4ConnectID    = GetConnectID();
			objMakePacket.m_pPacketParse    = &objPacketParse;
			objMakePacket.m_u1Option        = PACKET_SEND_OK;
			objMakePacket.m_AddrRemote      = m_addrRemote;
			objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

            Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

            //还原消息类型
            result.message_block().msg_type(ACE_Message_Block::MB_DATA);
        }

        App_MessageBlockManager::instance()->Close(&result.message_block());
        m_ThreadWriteLock.release();

        //记录发送字节数
        if(m_u4ReadSendSize >= m_u4SuccessSendSize + (uint32)result.bytes_to_write())
        {
            m_u4SuccessSendSize += (uint32)result.bytes_to_write();
        }

        return;
    }
}

void CProConnectHandler::SetRecvQueueTimeCost(uint32 u4TimeCost)
{
    m_ThreadWriteLock.acquire();
    m_nIOCount++;
    m_ThreadWriteLock.release();

    //如果超过阀值，则记录到日志中去
    if((uint32)(m_u2RecvQueueTimeout * 1000) <= u4TimeCost)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_RECVQUEUEERROR, "[TCP]IP=%s,Prot=%d,Timeout=[%d].", GetClientIPInfo().m_szClientIP, GetClientIPInfo().m_u2Port, u4TimeCost);
    }

    m_u4RecvQueueCount++;

    //测试了几天，感觉这个时间意义，因为获取队列的处理时间片可能很耗时，导致一批数据的阶段性时间增长
    //只要记录超时的数据即可

    Close();
}

void CProConnectHandler::SetSendQueueTimeCost(uint32 u4TimeCost)
{
    m_ThreadWriteLock.acquire();
    m_nIOCount++;
    m_ThreadWriteLock.release();

    //如果超过阀值，则记录到日志中去
    if((uint32)(m_u2SendQueueTimeout * 1000) <= u4TimeCost)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_SENDQUEUEERROR, "[TCP]IP=%s,Prot=%d,Timeout=[%d].", GetClientIPInfo().m_szClientIP, GetClientIPInfo().m_u2Port, u4TimeCost);

        //组织数据
		_MakePacket objMakePacket;

		objMakePacket.m_u4ConnectID     = GetConnectID();
		objMakePacket.m_pPacketParse    = NULL;
		objMakePacket.m_u1Option        = PACKET_SEND_TIMEOUT;
		objMakePacket.m_AddrRemote      = m_addrRemote;
		objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

        Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);
    }

    Close();
}

CONNECTSTATE CProConnectHandler::GetConnectState()
{
    return m_u1ConnectState;
}

CONNECTSTATE CProConnectHandler::GetSendBuffState()
{
    return m_u1SendBuffState;
}

bool CProConnectHandler::SendMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, uint8 u1State, uint8 u1SendType, uint32& u4PacketSize, bool blDelete, int nMessageID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if(NULL == pBuffPacket)
    {
        OUR_DEBUG((LM_DEBUG,"[CProConnectHandler::SendMessage] Connectid=[%d] pBuffPacket is NULL.\n", GetConnectID()));
        return false;
    }

    //如果当前连接已被别的线程关闭，则这里不做处理，直接退出
    if(m_u1IsActive == 0)
    {
        //如果连接不存在了，在这里返回失败，回调给业务逻辑去处理
        OUR_DEBUG((LM_INFO, "[CProConnectHandler::SendMessage]m_u1IsActive == 0.\n"))
        Tcp_Common_Send_Message_Error(GetConnectID(), u2CommandID, blDelete, pBuffPacket);

        return false;
    }

    if (EM_IO_TYPE::NET_INPUT == m_emIOType)
    {
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

    return true;
}

bool CProConnectHandler::PutSendPacket(ACE_Message_Block* pMbData, uint8 u1State)
{
    //ACE_Message_Block* pmbSend = new ACE_Message_Block(pMbData->length());
    ACE_Message_Block* pmbSend = App_MessageBlockManager::instance()->Create(pMbData->length());
    pmbSend->copy(pMbData->rd_ptr(), pMbData->length());

    //如果是DEBUG状态，记录当前发送包的二进制数据
    Output_Debug_Data(pmbSend, LOG_SYSTEM_DEBUG_CLIENTSEND);

    //统计发送数量

    //异步发送方法
    //因为是异步的，所以可能会接收到上次成功的数据包累计，这里需要注意一下。
    if(m_u4SuccessSendSize > m_u4ReadSendSize)
    {
        m_u4SuccessSendSize = m_u4ReadSendSize;
    }

    //记录水位标
    m_u4ReadSendSize += (uint32)pmbSend->length();

    if(0 != m_Writer.write(*pmbSend, pmbSend->length()))
    {

        OUR_DEBUG ((LM_ERROR, "[CProConnectHandler::PutSendPacket] Connectid=%d mb=%d m_writer.write error(%d)!\n", GetConnectID(),  pMbData->length(), errno));
        //如果发送失败，在这里返回失败，回调给业务逻辑去处理
        ACE_Time_Value tvNow = ACE_OS::gettimeofday();
        App_MakePacket::instance()->PutSendErrorMessage(GetConnectID(), pmbSend, tvNow);
        return false;
    }
    else
    {
        m_u4AllSendCount += 1;
        //m_atvOutput      = ACE_OS::gettimeofday();

        if (PACKET_SEND_FIN_CLOSE == u1State)
        {
            this->ServerClose(CLIENT_CLOSE_IMMEDIATLY);
        }

        //m_PerformanceCounter.counter();
        return true;
    }

    m_PerformanceCounter.counter();
    return true;
}

void CProConnectHandler::Get_Recv_length()
{
    if ("" != m_strDeviceName)
    {
        RecvClinetPacket(GetXmlConfigAttribute(xmlClientInfo)->MaxBuffRecv);
    }
    else if (App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        if (m_pPacketParse->GetIsHandleHead() == true)
        {
            if (false == RecvClinetPacket(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength))
            {
                OUR_DEBUG((LM_INFO, "[CProConnectHandler::open](%d)RecvClinetPacket error.\n", GetConnectID()));
            }
        }
        else
        {
            if (false == RecvClinetPacket(m_pPacketParse->GetPacketBodySrcLen()))
            {
                OUR_DEBUG((LM_INFO, "[CProConnectHandler::open](%d)RecvClinetPacket error.\n", GetConnectID()));
            }
        }
    }
    else
    {
        if (false == RecvClinetPacket(GetXmlConfigAttribute(xmlClientInfo)->MaxBuffRecv))
        {
            OUR_DEBUG((LM_INFO, "[CProConnectHandler::open](%d)RecvClinetPacket error.\n", GetConnectID()));
        }
    }
}

void CProConnectHandler::Output_Debug_Data(ACE_Message_Block* pMbData, int nLogType)
{
    //如果是DEBUG状态，记录当前接受包的二进制数据
    if (GetXmlConfigAttribute(xmlServerType)->Debug == DEBUG_ON || m_blIsLog == true)
    {
        char szLog[10] = { '\0' };
        int  nDebugSize = 0;
        bool blblMore = false;

        if (pMbData->length() >= m_u4PacketDebugSize)
        {
            nDebugSize = m_u4PacketDebugSize - 1;
            blblMore = true;
        }
        else
        {
            nDebugSize = (int)pMbData->length();
        }

        char* pData = pMbData->rd_ptr();

        for (int i = 0; i < nDebugSize; i++)
        {
            sprintf_safe(szLog, 10, "0x%02X ", (unsigned char)pData[i]);
            sprintf_safe(m_pPacketDebugData + 5 * i, MAX_BUFF_1024 - 5 * i, "%s", szLog);
        }

        m_pPacketDebugData[5 * nDebugSize] = '\0';

        if (blblMore == true)
        {
            AppLogManager::instance()->WriteLog_i(nLogType, "[(%s)%s:%d]%s.(数据包过长只记录前200字节)", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_pPacketDebugData);
        }
        else
        {
            AppLogManager::instance()->WriteLog_i(nLogType, "[(%s)%s:%d]%s.", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_pPacketDebugData);
        }
    }
}

int CProConnectHandler::Dispose_Paceket_Parse_Head(ACE_Message_Block* pmb)
{
    //判断头的合法性
    _Head_Info objHeadInfo;

    bool blStateHead = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Head_Info(GetConnectID(), pmb, App_MessageBlockManager::instance(), &objHeadInfo, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

    if (false == blStateHead)
    {
        //如果包头是非法的，则返回错误，断开连接。
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_read_stream]PacketHead is illegal.\n"));

        //清理PacketParse
        ClearPacketParse(*pmb);

        //关闭当前连接
        Close(2, errno);
        return -1;
    }
    else
    {
        if (NULL == objHeadInfo.m_pmbHead)
        {
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::RecvData]ConnectID=%d, objHeadInfo.m_pmbHead is NULL.\n", GetConnectID()));
        }

        m_pPacketParse->SetPacket_IsHandleHead(false);
        m_pPacketParse->SetPacket_Head_Src_Length(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength);
        m_pPacketParse->SetPacket_Head_Message(objHeadInfo.m_pmbHead);
        m_pPacketParse->SetPacket_Head_Curr_Length(objHeadInfo.m_u4HeadCurrLen);
        m_pPacketParse->SetPacket_Body_Src_Length(objHeadInfo.m_u4BodySrcLen);
        m_pPacketParse->SetPacket_CommandID(objHeadInfo.m_u2PacketCommandID);
    }

    //这里添加只处理包头的数据
    //如果数据只有包头，不需要包体，在这里必须做一些处理，让数据只处理包头就扔到DoMessage()
    uint32 u4PacketBodyLen = m_pPacketParse->GetPacketBodySrcLen();

    if (u4PacketBodyLen == 0)
    {
        //如果只有包头没有包体，则直接丢到逻辑里处理
        if (false == CheckMessage())
        {
            Close(2);
            return -1;
        }

        m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

        if (NULL == m_pPacketParse)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_read_stream] Open(%d) m_pPacketParse new error.\n", GetConnectID()));

            //组织数据
			_MakePacket objMakePacket;

			objMakePacket.m_u4ConnectID     = GetConnectID();
			objMakePacket.m_pPacketParse    = NULL;
			objMakePacket.m_u1Option        = PACKET_SDISCONNECT;
			objMakePacket.m_AddrRemote      = m_addrRemote;
			objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

            Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

            Close(2);
            return -1;
        }

        Close();
        OUR_DEBUG((LM_INFO, "[CProConnectHandler::Dispose_Paceket_Parse_Head]m_nIOCount=%d.\n", m_nIOCount));
    }
    else
    {
        //如果超过了最大包长度，为非法数据
        if (u4PacketBodyLen >= m_u4MaxPacketSize)
        {
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_read_stream]u4PacketHeadLen(%d) more than %d.\n", u4PacketBodyLen, m_u4MaxPacketSize));

            //清理PacketParse
            ClearPacketParse(*pmb);

            //关闭当前连接
            Close(2, errno);
            return -1;
        }
        else
        {
            Close();
        }
    }

    return 0;
}

int CProConnectHandler::Dispose_Paceket_Parse_Body(ACE_Message_Block* pmb)
{
    //接受完整数据完成，开始分析完整数据包
    _Body_Info obj_Body_Info;
    bool blStateBody = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Body_Info(GetConnectID(),
                       pmb,
                       App_MessageBlockManager::instance(),
                       &obj_Body_Info,
                       EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

    if (false == blStateBody)
    {
        //如果数据包体非法，断开连接
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_read_stream]SetPacketBody is illegal.\n"));

        //清理PacketParse
        ClearPacketParse(*pmb);

        //关闭当前连接
        Close(2, errno);
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
        Close(2);
        return -1;
    }

    m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

    if (NULL == m_pPacketParse)
    {
        OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_read_stream] Open(%d) m_pPacketParse new error.\n", GetConnectID()));

        //组织数据
		_MakePacket objMakePacket;

		objMakePacket.m_u4ConnectID     = GetConnectID();
		objMakePacket.m_pPacketParse    = NULL;
		objMakePacket.m_u1Option        = PACKET_SDISCONNECT;
		objMakePacket.m_AddrRemote      = m_addrRemote;
		objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

        Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

        Close(2);
        return -1;
    }

    Close();
    return 0;
}

int CProConnectHandler::Dispose_Paceket_Parse_Stream(ACE_Message_Block* pCurrMessage)
{
    //以流模式解析
    while (true)
    {
        //处理消息
        uint8 n1Ret = Tcp_Common_Recv_Stream(GetConnectID(), pCurrMessage, m_pPacketParse, m_u4PacketParseInfoID);

        if (PACKET_GET_NO_ENOUGH == n1Ret)
        {
            //接收的数据不完整，需要继续接收
            break;
        }
        else if (PACKET_GET_ENOUGH == n1Ret)
        {
            //已经接收了完整数据包，扔给工作线程去处理
            if (false == CheckMessage())
            {
                App_MessageBlockManager::instance()->Close(pCurrMessage);

                Close(2);
                return -1;
            }

            m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

            if (NULL == m_pPacketParse)
            {
                OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_read_stream](%d) m_pPacketParse new error.\n", GetConnectID()));

                //组织数据
				_MakePacket objMakePacket;

				objMakePacket.m_u4ConnectID     = GetConnectID();
				objMakePacket.m_pPacketParse    = NULL;
				objMakePacket.m_u1Option        = PACKET_SDISCONNECT;
				objMakePacket.m_AddrRemote      = m_addrRemote;
				objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

                Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

                Close(2);
                return -1;
            }

            //看看是否接收完成了
            if (pCurrMessage->length() == 0)
            {
                break;
            }
            else
            {
                //还有数据，继续分析
                continue;
            }
        }
        else
        {
            //数据包为错误包，丢弃处理
            m_pPacketParse->Clear();

            Close(2);
            return -1;
        }
    }

    Close();

    return 0;
}

bool CProConnectHandler::Write_SendData_To_File(bool blDelete, IBuffPacket* pBuffPacket)
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

bool CProConnectHandler::Send_Input_To_Cache(uint8 u1SendType, uint32& u4PacketSize, uint16 u2CommandID, bool blDelete, IBuffPacket* pBuffPacket)
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

bool CProConnectHandler::Send_Input_To_TCP(uint8 u1SendType, uint32& u4PacketSize, uint16 u2CommandID, uint8 u1State, int nMessageID, bool blDelete, IBuffPacket* pBuffPacket)
{
    //先判断是否要组装包头，如果需要，则组装在m_pBlockMessage中
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
                   u4PacketSize);

    if (false == blState)
    {
        return false;
    }

    Recovery_Common_BuffPacket(blDelete, pBuffPacket);

    //判断是否发送完成后关闭连接

    //将消息ID放入MessageBlock
    ACE_Message_Block::ACE_Message_Type objType = ACE_Message_Block::MB_USER + nMessageID;
    m_pBlockMessage->msg_type(objType);

    blState = PutSendPacket(m_pBlockMessage, u1State);
    if (true == blState)
    {
        m_pBlockMessage->reset();
    }

    return blState;
}

bool CProConnectHandler::RecvClinetPacket(uint32 u4PackeLen)
{
    m_nIOCount++;

    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(u4PackeLen);

    if(NULL == pmb)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64d, SendQueueTimeCost=%I64d.",
                                              m_addrRemote.get_host_addr(),
                                              m_addrRemote.get_port_number(),
                                              m_u4AllRecvSize,
                                              m_u4AllRecvCount,
                                              m_u4AllSendSize,
                                              m_u4AllSendCount,
                                              m_u4RecvQueueCount,
                                              m_u8RecvQueueTimeCost,
                                              m_u8SendQueueTimeCost);
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::RecvClinetPacket] pmb new is NULL.\n"));
        Close(2);
        return false;
    }

    if(m_Reader.read(*pmb, u4PackeLen) == -1)
    {
        //如果读失败，则关闭连接。
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::RecvClinetPacket] m_reader.read is error(%d)(%d).\n", GetConnectID(), errno));
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64d, SendQueueTimeCost=%I64d.",
                                              m_addrRemote.get_host_addr(),
                                              m_addrRemote.get_port_number(),
                                              m_u4AllRecvSize,
                                              m_u4AllRecvCount,
                                              m_u4AllSendSize,
                                              m_u4AllSendCount,
                                              m_u4RecvQueueCount,
                                              m_u8RecvQueueTimeCost,
                                              m_u8SendQueueTimeCost);
        ClearPacketParse(*pmb);
        Close(2);
        return false;
    }

    return true;
}

bool CProConnectHandler::CheckMessage()
{
    if(m_pPacketParse->GetMessageHead() != NULL)
    {
        if(m_pPacketParse->GetMessageBody() == NULL)
        {
            m_u4AllRecvSize += (uint32)m_pPacketParse->GetMessageHead()->length();
        }
        else
        {
            m_u4AllRecvSize += (uint32)m_pPacketParse->GetMessageHead()->length() + (uint32)m_pPacketParse->GetMessageBody()->length();
        }

        m_u4AllRecvCount++;

        ACE_Date_Time dtNow;

        if(false == m_TimeConnectInfo.RecvCheck((uint8)dtNow.minute(), 1, m_u4AllRecvSize))
        {
            //超过了限定的阀值，需要关闭链接，并记录日志
            AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECTABNORMAL,
                    GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                    "Alert",
                    "[TCP]IP=%s,Prot=%d,PacketCount=%d, RecvSize=%d.",
                    m_addrRemote.get_host_addr(),
                    m_addrRemote.get_port_number(),
                    m_TimeConnectInfo.m_u4RecvPacketCount,
                    m_TimeConnectInfo.m_u4RecvSize);


            App_PacketParsePool::instance()->Delete(m_pPacketParse, true);
            //设置封禁时间
            App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), GetXmlConfigAttribute(xmlIP)->Timeout);
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::CheckMessage] ConnectID = %d, PutMessageBlock is check invalid.\n", GetConnectID()));
            return false;
        }

        //组织数据
		_MakePacket objMakePacket;

		objMakePacket.m_u4ConnectID     = GetConnectID();
		objMakePacket.m_pPacketParse    = m_pPacketParse;
		objMakePacket.m_u1Option        = PACKET_PARSE;
		objMakePacket.m_AddrRemote      = m_addrRemote;
		objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

        Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

        //更新时间轮盘
        App_ProConnectManager::instance()->SetConnectTimeWheel(this);

        //清理用完的m_pPacketParse
        App_PacketParsePool::instance()->Delete(m_pPacketParse);
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::CheckMessage] ConnectID = %d, m_pPacketParse is NULL.\n", GetConnectID()));
    }

    return true;
}

_ClientConnectInfo CProConnectHandler::GetClientInfo()
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

_ClientIPInfo CProConnectHandler::GetClientIPInfo()
{
    _ClientIPInfo ClientIPInfo;
    sprintf_safe(ClientIPInfo.m_szClientIP, MAX_BUFF_50, "%s", m_addrRemote.get_host_addr());
    ClientIPInfo.m_u2Port = m_addrRemote.get_port_number();
    return ClientIPInfo;
}

_ClientIPInfo CProConnectHandler::GetLocalIPInfo()
{
    _ClientIPInfo ClientIPInfo;
    sprintf_safe(ClientIPInfo.m_szClientIP, MAX_BUFF_50, "%s", m_szLocalIP);
    ClientIPInfo.m_u2Port = m_u2LocalPort;
    return ClientIPInfo;
}

void CProConnectHandler::ClearPacketParse(ACE_Message_Block& mbCurrBlock)
{
    if(NULL != m_pPacketParse && m_pPacketParse->GetMessageHead() != NULL)
    {
        App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
    }

    if(NULL != m_pPacketParse && m_pPacketParse->GetMessageBody() != NULL)
    {
        App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
    }

    if(NULL != m_pPacketParse)
    {
        if(NULL != &mbCurrBlock && &mbCurrBlock != m_pPacketParse->GetMessageHead() && &mbCurrBlock != m_pPacketParse->GetMessageBody())
        {
            App_MessageBlockManager::instance()->Close(&mbCurrBlock);
        }

        App_PacketParsePool::instance()->Delete(m_pPacketParse);
        m_pPacketParse = NULL;
    }
    else
    {
        if(NULL != &mbCurrBlock)
        {
            App_MessageBlockManager::instance()->Close(&mbCurrBlock);
        }
    }
}

char* CProConnectHandler::GetConnectName()
{
    return m_szConnectName;
}

void CProConnectHandler::SetConnectName( const char* pName )
{
    sprintf_safe(m_szConnectName, MAX_BUFF_100, "%s", pName);
}

void CProConnectHandler::SetIsLog(bool blIsLog)
{
    m_blIsLog = blIsLog;
}

bool CProConnectHandler::GetIsLog()
{
    return m_blIsLog;
}

void CProConnectHandler::SetHashID(int nHashID)
{
    m_nHashID = nHashID;
}

int CProConnectHandler::GetHashID()
{
    return m_nHashID;
}

void CProConnectHandler::SetLocalIPInfo(const char* pLocalIP, uint16 u2LocalPort)
{
    sprintf_safe(m_szLocalIP, MAX_BUFF_50, "%s", pLocalIP);
    m_u2LocalPort = u2LocalPort;
}

void CProConnectHandler::PutSendPacketError(ACE_Message_Block* pMbData)
{

}

void CProConnectHandler::SetSendCacheManager(ISendCacheManager* pSendCacheManager)
{
    m_pBlockMessage = pSendCacheManager->GetCacheData(GetConnectID());

    if(NULL == m_pBlockMessage)
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::SetSendCacheManager] ConnectID = %d, m_pBlockMessage is NULL.\n", GetConnectID()));
    }
}

void CProConnectHandler::SetPacketParseInfoID(uint32 u4PacketParseInfoID)
{
    m_u4PacketParseInfoID = u4PacketParseInfoID;
}

uint32 CProConnectHandler::GetPacketParseInfoID()
{
    return m_u4PacketParseInfoID;
}

bool CProConnectHandler::SendTimeoutMessage()
{
	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID     = GetConnectID();
	objMakePacket.m_pPacketParse    = NULL;
	objMakePacket.m_u1Option        = PACKET_CHEK_TIMEOUT;
	objMakePacket.m_AddrRemote      = m_addrRemote;
	objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

    Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

    return true;
}

bool CProConnectHandler::Device_Send_Data(const char* pData, ssize_t nLen)
{
    uint16 u2CommandID = 0x0000;

    IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);
    pBuffPacket->WriteStream(pData, (uint32)nLen);

    uint8 u1State       = PACKET_SEND_IMMEDIATLY;
    uint8 u1SendState   = SENDMESSAGE_JAMPNOMAL;
    uint32 u4PacketSize = 0;

    return SendMessage(u2CommandID, pBuffPacket, u1State, u1SendState, u4PacketSize, true, 0);
}

//***************************************************************************
CProConnectManager::CProConnectManager(void):m_mutex(), m_cond(m_mutex), m_u4SendQueuePutTime(0)
{
    m_u4TimeCheckID      = 0;
    m_szError[0]         = '\0';
    m_blRun              = false;

    m_u4TimeConnect      = 0;
    m_u4TimeDisConnect   = 0;

    m_u2SendQueueMax     = MAX_MSG_SENDPACKET;

    m_tvCheckConnect     = ACE_OS::gettimeofday();

    m_SendMessagePool.Init();
}

CProConnectManager::~CProConnectManager(void)
{
    CloseAll();
}

void CProConnectManager::CloseAll()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

    if(m_blRun)
    {
        if (false == this->CloseMsgQueue())
        {
            OUR_DEBUG((LM_INFO, "[CProConnectManager::CloseAll]CloseMsgQueue fail.\n"));
        }
    }
    else
    {
        msg_queue()->deactivate();
    }

    if (false == KillTimer())
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManager::CloseAll]KillTimer fail.\n"));
    }

    vector<CProConnectHandler*> vecCloseConnectHandler;
    m_objHashConnectList.Get_All_Used(vecCloseConnectHandler);

    //开始关闭所有连接
    for(auto* pConnectHandler : vecCloseConnectHandler)
    {
        pConnectHandler->Close();
    }

    //删除hash表空间
    m_objHashConnectList.Close();

    //删除缓冲对象
    m_SendCacheManager.Close();
}

bool CProConnectManager::Close(uint32 u4ConnectID)
{
    //客户端关闭
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

    //连接关闭，清除时间轮盘
    CProConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if (false == DelConnectTimeWheel(pConnectHandler))
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::Close]DelConnectTimeWheel ConnectID=%d fail.\n", u4ConnectID));
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::Close]DelConnectTimeWheel ConnectID=%d.\n", u4ConnectID));
    }

    int32 nPos = m_objHashConnectList.Del_Hash_Data_By_Unit32(u4ConnectID);

    if (0 > nPos)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "[CConnectManager::Close]ConnectID=%d FAIL.",
                                              u4ConnectID);

        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectManager::Close] ConnectID[%d] is not find.", u4ConnectID);
    }

    m_u4TimeDisConnect++;

    //回收发送内存块
    m_SendCacheManager.FreeCacheData(u4ConnectID);

    //加入链接统计功能
    App_ConnectAccount::instance()->AddDisConnect();

    return true;
}

bool CProConnectManager::CloseConnect(uint32 u4ConnectID, EM_Client_Close_status emStatus)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
    //服务器关闭
    CProConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if(pConnectHandler != NULL)
    {
        //从时间轮盘中清除
        if (false == DelConnectTimeWheel(pConnectHandler))
        {
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::Close]DelConnectTimeWheel ConnectID=%d fail.\n", u4ConnectID));
        }

        pConnectHandler->ServerClose(emStatus);

        m_u4TimeDisConnect++;

        //回收发送内存块
        m_SendCacheManager.FreeCacheData(u4ConnectID);

        //加入链接统计功能
        if (false == App_ConnectAccount::instance()->AddDisConnect())
        {
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::Close]AddDisConnect ConnectID=%d fail.\n", u4ConnectID));
        }

        if (-1 == m_objHashConnectList.Del_Hash_Data_By_Unit32(u4ConnectID))
        {
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::Close]Del_Hash_Data ConnectID=%d fail.\n", u4ConnectID));
        }

        return true;
    }
    else
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::CloseConnect] ConnectID[%d] is not find.", u4ConnectID);
        return true;
    }
}

bool CProConnectManager::CloseConnect_By_Queue(uint32 u4ConnectID)
{
    //放入发送队列
    return Tcp_Common_CloseConnect_By_Queue(u4ConnectID,
                                            m_SendMessagePool,
                                            m_u4SendQueuePutTime,
                                            dynamic_cast<ACE_Task<ACE_MT_SYNCH>*>(this));
}

bool CProConnectManager::AddConnect(uint32 u4ConnectID, CProConnectHandler* pConnectHandler)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

    if(pConnectHandler == NULL)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::AddConnect] pConnectHandler is NULL.");
        return false;
    }

    CProConnectHandler* pCurrConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if(NULL != pCurrConnectHandler)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::AddConnect] ConnectID[%d] is exist.", u4ConnectID);
        return false;
    }

    pConnectHandler->SetConnectID(u4ConnectID);
    pConnectHandler->SetSendCacheManager((ISendCacheManager* )&m_SendCacheManager);

    //加入Hash数组
    if (-1 == m_objHashConnectList.Add_Hash_Data_By_Key_Unit32(u4ConnectID, pConnectHandler))
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManager::AddConnect]add fail.\n"));
    }

    m_u4TimeConnect++;

    //加入链接统计功能
    App_ConnectAccount::instance()->AddConnect();

    //加入时间轮盘
    if (false == SetConnectTimeWheel(pConnectHandler))
    {
        OUR_DEBUG((LM_INFO, "[CProConnectHandler::AddConnect](%d)SetConnectTimeWheel is fail", u4ConnectID));
    }

    return true;
}

bool CProConnectManager::SetConnectTimeWheel(CProConnectHandler* pConnectHandler)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
    bool bAddResult = m_TimeWheelLink.Add_TimeWheel_Object(pConnectHandler);

    if(!bAddResult)
    {
        OUR_DEBUG((LM_ERROR,"[CProConnectManager::SetConnectTimeWheel]Fail to set pConnectHandler(0x%08x).\n", pConnectHandler));
    }

    return bAddResult;
}

bool CProConnectManager::DelConnectTimeWheel(CProConnectHandler* pConnectHandler)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

    m_TimeWheelLink.Del_TimeWheel_Object(pConnectHandler);
    return true;
}

bool CProConnectManager::SendMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint16 u2CommandID, uint8 u1SendState, uint8 u1SendType, ACE_Time_Value& tvSendBegin, bool blDelete, int nMessageID)
{
    m_ThreadWriteLock.acquire();
    CProConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);
    m_ThreadWriteLock.release();

    uint32 u4CommandSize = pBuffPacket->GetPacketLen();

    if(NULL != pConnectHandler)
    {
        uint32 u4PacketSize  = 0;

        if (false == pConnectHandler->SendMessage(u2CommandID, pBuffPacket, u1SendState, u1SendType, u4PacketSize, blDelete, nMessageID))
        {
            OUR_DEBUG((LM_INFO, "[CProConnectManager::SendMessage]ConnectID=%d, CommandID=%d, SendMessage error.\n", u4ConnectID, u2CommandID));
        }
        
        _ClientIPInfo objClientIP = pConnectHandler->GetLocalIPInfo();

        if (false == m_CommandAccount.SaveCommandData(u2CommandID, objClientIP.m_u2Port, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP, u4CommandSize, COMMAND_TYPE_OUT))
        {
            OUR_DEBUG((LM_INFO, "[CProConnectManager::SendMessage]ConnectID=%d, CommandID=%d, SaveCommandData error.\n", u4ConnectID, u2CommandID));
        }

        return true;
    }
    else
    {
        //如果连接不存在了，在这里返回失败，回调给业务逻辑去处理
        //OUR_DEBUG((LM_INFO, "[CProConnectManager::SendMessage]pConnectHandler is NULL.\n"));
        Tcp_Common_Send_Message_Error(u4ConnectID, u2CommandID, blDelete, pBuffPacket);

        if(true == blDelete)
        {
            App_BuffPacketManager::instance()->Delete(pBuffPacket);
        }

        return true;
    }

    return true;
}

bool CProConnectManager::PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    _Post_Message_Param obj_Post_Message_Param;
    obj_Post_Message_Param.m_blDelete           = blDelete;
    obj_Post_Message_Param.m_nMessageID         = nMessageID;
    obj_Post_Message_Param.m_u1SendState        = u1SendState;
    obj_Post_Message_Param.m_u1SendType         = u1SendType;
    obj_Post_Message_Param.m_u2CommandID        = u2CommandID;
    obj_Post_Message_Param.m_u2SendQueueMax     = m_u2SendQueueMax;
    obj_Post_Message_Param.m_u4ConnectID        = u4ConnectID;
    obj_Post_Message_Param.m_u4SendQueuePutTime = m_u4SendQueuePutTime;

    ACE_Time_Value tvSend = ACE_OS::gettimeofday();

    return SendMessage(u4ConnectID, 
        pBuffPacket, 
        u2CommandID, 
        u1SendState,
        u1SendType, 
        tvSend,
        blDelete, 
        nMessageID);
}

const char* CProConnectManager::GetError()
{
    return m_szError;
}

bool CProConnectManager::StartTimer()
{
    //启动发送线程
    if(0 != open())
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectManager::StartTimer]Open() is error.\n"));
        return false;
    }

    //避免定时器重复启动
    if (false == KillTimer())
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectManager::StartTimer]KillTimer() is error.\n"));
        return false;
    }

    OUR_DEBUG((LM_ERROR, "CProConnectManager::StartTimer()-->begin....\n"));

    //检测链接发送存活包数
    uint16 u2CheckAlive = GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime;

    if (u2CheckAlive > 0)
    {
        long lTimeCheckID = App_TimerManager::instance()->schedule(this, (void*)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(u2CheckAlive), ACE_Time_Value(u2CheckAlive));

        if (-1 == lTimeCheckID)
        {
            OUR_DEBUG((LM_ERROR, "CProConnectManager::StartTimer()--> Start thread m_u4TimeCheckID error.\n"));
            return false;
        }
        else
        {
            m_u4TimeCheckID = (uint32)lTimeCheckID;
            OUR_DEBUG((LM_ERROR, "CProConnectManager::StartTimer()--> Start thread time OK.\n"));
            return true;
        }
    }

    return true;
}

bool CProConnectManager::KillTimer()
{
    if(m_u4TimeCheckID > 0)
    {
        App_TimerManager::instance()->cancel(m_u4TimeCheckID);
        m_u4TimeCheckID = 0;
    }

    return true;
}

int CProConnectManager::handle_write_file_stream(uint32 u4ConnectID, const char* pData, uint32 u4Size, uint8 u1ParseID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
    CProConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if (NULL != pConnectHandler)
    {
        return pConnectHandler->handle_write_file_stream(pData, u4Size, u1ParseID);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManager::handle_write_file_stream]m_objHashConnectList not find.\n"));
        return -1;
    }
}

int CProConnectManager::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

    ACE_UNUSED_ARG(arg);

    ACE_Time_Value tvNow = tv;

    //转动时间轮盘
    m_TimeWheelLink.Tick();

    //判定是否应该记录链接日志
    Tcp_Common_Manager_Timeout_CheckInfo(GetCount());

    ACE_Time_Value tvEnd = ACE_OS::gettimeofday();

    //得到时间执行了多少秒，并记录日志
    ACE_Time_Value tvCost = tvEnd - tvNow;
    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_WORKTHREAD, "[CProConnectManager::handle_timeout]TimeCost=%d.", tvCost.msec());

    return 0;
}

int CProConnectManager::GetCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
    return (int)m_objHashConnectList.Get_Used_Count();
}

int CProConnectManager::open(void* args)
{
    m_blRun = true;
    msg_queue()->high_water_mark(MAX_MSG_MASK);
    msg_queue()->low_water_mark(MAX_MSG_MASK);

    OUR_DEBUG((LM_INFO,"[CProConnectManager::open] m_u4HighMask = [%d] m_u4LowMask = [%d]\n", MAX_MSG_MASK, MAX_MSG_MASK));

    if(activate(THREAD_PARAM, MAX_MSG_THREADCOUNT) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectManager::open] activate error ThreadCount = [%d].", MAX_MSG_THREADCOUNT));
        m_blRun = false;
        return -1;
    }

    m_u4SendQueuePutTime = GetXmlConfigAttribute(xmlSendInfo)->PutQueueTimeout * 1000;

    resume();

    return 0;
}

int CProConnectManager::svc (void)
{
    ACE_Time_Value xtime;

    while(true)
    {
        ACE_Message_Block* mb = NULL;
        ACE_OS::last_error(0);

        if(getq(mb, 0) == -1)
        {
            OUR_DEBUG((LM_INFO,"[CProConnectManager::svc] getq is error[%d]!\n", ACE_OS::last_error()));
            m_blRun = false;
            break;
        }
        else
        {
            if (mb == NULL)
            {
                continue;
            }

            if ((0 == mb->size ()) && (mb->msg_type () == ACE_Message_Block::MB_STOP))
            {
                m_mutex.acquire();
                mb->release ();
                this->msg_queue ()->deactivate ();
                m_cond.signal();
                m_mutex.release();
                m_blRun = false;
                break;
            }

            _SendMessage* msg = *((_SendMessage**)mb->base());

            if (!msg)
            {
                continue;
            }

            if (0 == msg->m_u1Type)
            {
                //处理发送数据
                if (false == SendMessage(msg->m_u4ConnectID, msg->m_pBuffPacket, msg->m_u2CommandID, msg->m_u1SendState, msg->m_nEvents, msg->m_tvSend, msg->m_blDelete, msg->m_nMessageID))
                {
                    OUR_DEBUG((LM_INFO, "[CProConnectManager::svc]ConnectID=%d, m_u2CommandID=%d, SendMessage error.\n", msg->m_u4ConnectID, msg->m_u2CommandID));
                    //回收发送缓冲
                    App_BuffPacketManager::instance()->Delete(msg->m_pBuffPacket);
                }
            }
            else if (1 == msg->m_u1Type)
            {
                //处理连接服务器主动关闭
                if (false == CloseConnect(msg->m_u4ConnectID, CLIENT_CLOSE_IMMEDIATLY))
                {
                    OUR_DEBUG((LM_INFO, "[CProConnectManager::svc]ConnectID=%d CloseConnect error.\n", msg->m_u4ConnectID));
                }
            }

            m_SendMessagePool.Delete(msg);
        }
    }

    //回收发送数据池
    m_SendMessagePool.Close();

    OUR_DEBUG((LM_INFO,"[CProConnectManager::svc] svc finish!\n"));
    return 0;
}

int CProConnectManager::close(u_long)
{
    m_blRun = false;
    OUR_DEBUG((LM_INFO,"[CProConnectManager::close] close().\n"));
    return 0;
}

void CProConnectManager::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
    vector<CProConnectHandler*> vecProConnectHandle;
    m_objHashConnectList.Get_All_Used(vecProConnectHandle);

    for(auto* pConnectHandler : vecProConnectHandle)
    {
        if(pConnectHandler != NULL)
        {
            VecClientConnectInfo.push_back(pConnectHandler->GetClientInfo());
        }
    }
}

void CProConnectManager::SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
    CProConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if(NULL != pConnectHandler)
    {
        pConnectHandler->SetRecvQueueTimeCost(u4TimeCost);
    }
}

_ClientIPInfo CProConnectManager::GetClientIPInfo(uint32 u4ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
    CProConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

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

_ClientIPInfo CProConnectManager::GetLocalIPInfo(uint32 u4ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
    CProConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

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

bool CProConnectManager::PostMessageAll( IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    m_ThreadWriteLock.acquire();
    vector<CProConnectHandler*> objvecProConnectManager;
    m_objHashConnectList.Get_All_Used(objvecProConnectManager);
    m_ThreadWriteLock.release();

    uint32 u4ConnectID = 0;

    for(auto* pProConnectManager : objvecProConnectManager)
    {
        IBuffPacket* pCurrBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

        if(NULL == pCurrBuffPacket)
        {
            OUR_DEBUG((LM_INFO, "[CProConnectManager::PostMessage]pCurrBuffPacket is NULL.\n"));
            App_BuffPacketManager::instance()->Delete(pBuffPacket);
            return false;
        }

        pCurrBuffPacket->WriteStream(pBuffPacket->GetData(), pBuffPacket->GetPacketLen());

        //放入发送队列
        _SendMessage* pSendMessage = m_SendMessagePool.Create();

        if(NULL == pSendMessage)
        {
            OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] new _SendMessage is error.\n"));
            App_BuffPacketManager::instance()->Delete(pBuffPacket);
            return false;
        }

        ACE_Message_Block* mb = pSendMessage->GetQueueMessage();

        if(NULL != mb)
        {
            pSendMessage->m_u4ConnectID = pProConnectManager->GetConnectID();
            pSendMessage->m_pBuffPacket = pCurrBuffPacket;
            pSendMessage->m_nEvents     = u1SendType;
            pSendMessage->m_u2CommandID = u2CommandID;
            pSendMessage->m_u1SendState = u1SendState;
            pSendMessage->m_blDelete    = PACKET_IS_FRAMEWORK_RECYC;
            pSendMessage->m_nMessageID  = nMessageID;
            pSendMessage->m_u1Type      = 0;
            pSendMessage->m_tvSend      = ACE_OS::gettimeofday();

            //判断队列是否是已经最大
            int nQueueCount = (int)msg_queue()->message_count();

            if(nQueueCount >= (int)m_u2SendQueueMax)
            {
                OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));

                if(true == blDelete)
                {
                    App_BuffPacketManager::instance()->Delete(pBuffPacket);
                }

                m_SendMessagePool.Delete(pSendMessage);
                return false;
            }

            ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);

            if(this->putq(mb, &xtime) == -1)
            {
                OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));

                if(true == blDelete)
                {
                    App_BuffPacketManager::instance()->Delete(pBuffPacket);
                }

                m_SendMessagePool.Delete(pSendMessage);
                return false;
            }
        }
        else
        {
            OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] mb new error.\n"));

            if(true == blDelete)
            {
                App_BuffPacketManager::instance()->Delete(pBuffPacket);
            }

            m_SendMessagePool.Delete(pSendMessage);
            return false;
        }
    }

    return true;
}

bool CProConnectManager::SetConnectName( uint32 u4ConnectID, const char* pName )
{
    CProConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

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

bool CProConnectManager::SetIsLog( uint32 u4ConnectID, bool blIsLog )
{
    CProConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

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

void CProConnectManager::GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo)
{
    vector<CProConnectHandler*> vecProConnectHandle;
    m_objHashConnectList.Get_All_Used(vecProConnectHandle);

    for(auto* pConnectHandler : vecProConnectHandle)
    {
        if(NULL != pConnectHandler && ACE_OS::strcmp(pConnectHandler->GetConnectName(), pName) == 0)
        {
            _ClientNameInfo ClientNameInfo = Tcp_Common_ClientNameInfo((int)pConnectHandler->GetConnectID(),
                                             pConnectHandler->GetConnectName(),
                                             pConnectHandler->GetClientIPInfo().m_szClientIP,
                                             pConnectHandler->GetClientIPInfo().m_u2Port,
                                             pConnectHandler->GetIsLog());

            objClientNameInfo.push_back(ClientNameInfo);
        }
    }
}

void CProConnectManager::Init(uint16 u2Index)
{
    //初始化公共的部分
    Tcp_Common_Manager_Init(u2Index, m_CommandAccount, m_u2SendQueueMax, m_SendCacheManager);

    //初始化Hash表
    uint16 u2PoolSize = GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount;
    m_objHashConnectList.Init((int)u2PoolSize);

    //初始化时间轮盘
    m_TimeWheelLink.Init(GetXmlConfigAttribute(xmlClientInfo)->MaxConnectTime,
                         GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime,
                         (int)u2PoolSize, CProConnectManager::TimeWheel_Timeout_Callback, (void* )this);
}

void CProConnectManager::TimeWheel_Timeout_Callback(void* pArgsContext, vector<CProConnectHandler*> vecProConnectHandle)
{
    OUR_DEBUG((LM_INFO, "[CProConnectManager::TimeWheel_Timeout_Callback]Timeout Count(%d).\n", vecProConnectHandle.size()));

    for (int i = 0; i < (int)vecProConnectHandle.size(); i++)
    {
        //断开超时的链接
        if (NULL != vecProConnectHandle[i])
        {
            CProConnectManager* pManager = reinterpret_cast<CProConnectManager*>(pArgsContext);
            OUR_DEBUG((LM_INFO, "[CProConnectManager::TimeWheel_Timeout_Callback]ConnectID(%d).\n", vecProConnectHandle[i]->GetConnectID()));

            //通知业务插件，超时信息
            vecProConnectHandle[i]->SendTimeoutMessage();

            if (NULL != pManager && false == pManager->CloseConnect_By_Queue(vecProConnectHandle[i]->GetConnectID()))
            {
                OUR_DEBUG((LM_INFO, "[CProConnectManager::TimeWheel_Timeout_Callback]CloseConnect_By_Queue error.\n"));
            }
        }
    }
}

_CommandData* CProConnectManager::GetCommandData(uint16 u2CommandID)
{
    return m_CommandAccount.GetCommandData(u2CommandID);
}

void CProConnectManager::GetFlowInfo(uint32& u4FlowIn, uint32& u4FlowOut)
{
    u4FlowIn  = m_CommandAccount.GetFlowIn();
    u4FlowOut = m_CommandAccount.GetFlowOut();
}

EM_Client_Connect_status CProConnectManager::GetConnectState(uint32 u4ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
    CProConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if(NULL != pConnectHandler)
    {
        return CLIENT_CONNECT_EXIST;
    }
    else
    {
        return CLIENT_CONNECT_NO_EXIST;
    }
}

CSendCacheManager* CProConnectManager::GetSendCacheManager()
{
    return &m_SendCacheManager;
}

int CProConnectManager::CloseMsgQueue()
{
    return Task_Common_CloseMsgQueue((ACE_Task<ACE_MT_SYNCH>*)this, m_cond, m_mutex);
}

//*********************************************************************************

CProConnectHandlerPool::CProConnectHandlerPool(void)
{
    //ConnectID计数器从1开始
    m_u4CurrMaxCount = 1;
}

CProConnectHandlerPool::~CProConnectHandlerPool(void)
{
    Close();
}

void CProConnectHandlerPool::Init(int nObjcetCount)
{
    Close();

    //初始化HashTable
    m_objHandlerList.Init((uint32)nObjcetCount);
    m_objHashHandleList.Init((int)nObjcetCount);

    for(int i = 0; i < nObjcetCount; i++)
    {
        CProConnectHandler* pHandler = m_objHandlerList.GetObject(i);

        if(NULL != pHandler)
        {
            pHandler->Init(m_u4CurrMaxCount);

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

void CProConnectHandlerPool::Close()
{
    //清理所有已存在的指针
    m_u4CurrMaxCount  = 1;

    //删除hash表空间
    m_objHashHandleList.Close();
}

int CProConnectHandlerPool::GetUsedCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return m_objHashHandleList.Get_Count() - m_objHashHandleList.Get_Used_Count();
}

int CProConnectHandlerPool::GetFreeCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return (int)m_objHashHandleList.Get_Used_Count();
}

CProConnectHandler* CProConnectHandlerPool::Create()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    //在Hash表中弹出一个已使用的数据
    CProConnectHandler* pHandler = m_objHashHandleList.Pop();

    //没找到空余的
    return pHandler;
}

bool CProConnectHandlerPool::Delete(CProConnectHandler* pObject)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if(NULL == pObject)
    {
        return false;
    }

    char szHandlerID[10] = {'\0'};
    sprintf_safe(szHandlerID, 10, "%d", pObject->GetHandlerID());
    //这里因为内存是固定的，直接写会Hash原有位置
    bool blState = m_objHashHandleList.Push(szHandlerID, pObject);

    if(false == blState)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectHandlerPool::Delete]szHandlerID=%s(0x%08x).\n", szHandlerID, pObject));
    }

    return true;
}

//==============================================================
CProConnectManagerGroup::CProConnectManagerGroup()
{
    m_objProConnnectManagerList = NULL;
    m_u4CurrMaxCount            = 1;
    m_u2ThreadQueueCount        = SENDQUEUECOUNT;
}

CProConnectManagerGroup::~CProConnectManagerGroup()
{
    OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::~CProConnectManagerGroup].\n"));
    Close();
}

void CProConnectManagerGroup::Close()
{
    if(NULL != m_objProConnnectManagerList)
    {
        for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
        {
            CProConnectManager* pConnectManager = m_objProConnnectManagerList[i];
            SAFE_DELETE(pConnectManager);
        }
    }

    SAFE_DELETE_ARRAY(m_objProConnnectManagerList);
    m_u2ThreadQueueCount = 0;
}

void CProConnectManagerGroup::Init(uint16 u2SendQueueCount)
{
    Close();

    m_objProConnnectManagerList = new CProConnectManager*[u2SendQueueCount];
    memset(m_objProConnnectManagerList, 0, sizeof(CProConnectManager*)*u2SendQueueCount);

    for(int i = 0; i < u2SendQueueCount; i++)
    {
        CProConnectManager* pConnectManager = new CProConnectManager();

        if(NULL != pConnectManager)
        {
            //初始化统计器
            pConnectManager->Init((uint16)i);

            //加入数组
            m_objProConnnectManagerList[i] = pConnectManager;
            OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::Init]Creat %d SendQueue OK.\n", i));
        }
    }

    m_u2ThreadQueueCount = u2SendQueueCount;
}

uint32 CProConnectManagerGroup::GetGroupIndex()
{
    //根据链接获得命中，（简单球形命中算法）
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    m_u4CurrMaxCount++;
    return m_u4CurrMaxCount;
}

bool CProConnectManagerGroup::AddConnect(CProConnectHandler* pConnectHandler)
{
    uint32 u4ConnectID = GetGroupIndex();

    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::AddConnect]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->AddConnect(u4ConnectID, pConnectHandler);
}

bool CProConnectManagerGroup::SetConnectTimeWheel(CProConnectHandler* pConnectHandler)
{
    uint32 u4ConnectID = pConnectHandler->GetConnectID();

    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if (NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::AddConnect]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->SetConnectTimeWheel(pConnectHandler);
}

bool CProConnectManagerGroup::DelConnectTimeWheel(CProConnectHandler* pConnectHandler)
{
    uint32 u4ConnectID = pConnectHandler->GetConnectID();

    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if (NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::AddConnect]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->DelConnectTimeWheel(pConnectHandler);
}

bool CProConnectManagerGroup::PostMessage(uint32 u4ConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    if (0 >= pBuffPacket->GetPacketLen())
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]send data len is zero.\n"));
        return false;
    }

    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, u1SendState, blDelete, nMessageID);
}

bool CProConnectManagerGroup::PostMessage( uint32 u4ConnectID, char*& pData, uint32 nDataLen, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    //判断发送的数据长度是否大于0
    if (0 == nDataLen)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]send data len is zero.\n"));
        return false;
    }

    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));

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
            return pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, u1SendState, true, nMessageID);
        }
        else
        {
            return false;
        }
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]pBuffPacket is NULL.\n"));

        if(true == blDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

}

bool CProConnectManagerGroup::PostMessage( vector<uint32> vecConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    if (0 >= pBuffPacket->GetPacketLen())
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]send data len is zero.\n"));
        return false;
    }

    uint32 u4ConnectID = 0;

    for(auto& u4ConnectID : vecConnectID)
    {
        //判断命中到哪一个线程组里面去
        uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

        CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

        if(NULL == pConnectManager)
        {
            OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
            continue;
        }

        //为每一个Connect设置发送对象数据包
        IBuffPacket* pCurrBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

        if(NULL == pCurrBuffPacket)
        {
            continue;
        }

        pCurrBuffPacket->WriteStream(pBuffPacket->GetData(), pBuffPacket->GetWriteLen());

        if (false == pConnectManager->PostMessage(u4ConnectID, pCurrBuffPacket, u1SendType, u2CommandID, u1SendState, true, nMessageID))
        {
            OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]PostMessage(%d) is error.\n", u4ConnectID));
        }
    }

    if(true == blDelete)
    {
        App_BuffPacketManager::instance()->Delete(pBuffPacket);
    }


    return true;
}

bool CProConnectManagerGroup::PostMessage( vector<uint32> vecConnectID, char*& pData, uint32 nDataLen, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    if (0 == nDataLen)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]send data len is zero.\n"));
        return false;
    }

    for(auto& u4ConnectID : vecConnectID)
    {
        //判断命中到哪一个线程组里面去
        uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

        CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

        if(NULL == pConnectManager)
        {
            OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
            continue;
        }

        //为每一个Connect设置发送对象数据包
        IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

        if(NULL == pBuffPacket)
        {
            continue;
        }

        pBuffPacket->WriteStream(pData, nDataLen);

        if (false == PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, u1SendState, true, nMessageID))
        {
            OUR_DEBUG((LM_INFO, "[ CProConnectManagerGroup::PostMessage]PostMessage(%d) error.\n", u4ConnectID));
        }
    }

    if(true == blDelete)
    {
        SAFE_DELETE_ARRAY(pData);
    }

    return true;
}

bool CProConnectManagerGroup::CloseConnect(uint32 u4ConnectID)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::CloseConnect]No find send Queue object.\n"));
        return false;
    }

    //通过消息队列去实现关闭
    return pConnectManager->CloseConnect_By_Queue(u4ConnectID);
}

_ClientIPInfo CProConnectManagerGroup::GetClientIPInfo(uint32 u4ConnectID)
{
    _ClientIPInfo objClientIPInfo;
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
        return objClientIPInfo;
    }

    return pConnectManager->GetClientIPInfo(u4ConnectID);
}

_ClientIPInfo CProConnectManagerGroup::GetLocalIPInfo(uint32 u4ConnectID)
{
    _ClientIPInfo objClientIPInfo;
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetLocalIPInfo]No find send Queue object.\n"));
        return objClientIPInfo;
    }

    return pConnectManager->GetLocalIPInfo(u4ConnectID);
}


void CProConnectManagerGroup::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
    VecClientConnectInfo.clear();

    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CProConnectManager* pConnectManager = m_objProConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            pConnectManager->GetConnectInfo(VecClientConnectInfo);
        }
    }
}

int CProConnectManagerGroup::GetCount()
{
    uint32 u4Count = 0;

    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CProConnectManager* pConnectManager = m_objProConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            u4Count += pConnectManager->GetCount();
        }
    }

    return u4Count;
}

void CProConnectManagerGroup::CloseAll()
{
    uint32 u4Count = 0;

    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CProConnectManager* pConnectManager = m_objProConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            pConnectManager->CloseAll();
        }
    }
}

bool CProConnectManagerGroup::StartTimer()
{
    uint32 u4Count = 0;

    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CProConnectManager* pConnectManager = m_objProConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            if (false == pConnectManager->StartTimer())
            {
                OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::StartTimer]StartTimer error.\n"));
            }
        }
    }

    return true;
}

bool CProConnectManagerGroup::Close(uint32 u4ConnectID)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->Close(u4ConnectID);
}

const char* CProConnectManagerGroup::GetError()
{
    return (char* )"";
}

void CProConnectManagerGroup::SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
        return;
    }

    pConnectManager->SetRecvQueueTimeCost(u4ConnectID, u4TimeCost);
}

bool CProConnectManagerGroup::PostMessageAll( IBuffPacket*& pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    if (0 >= pBuffPacket->GetPacketLen())
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessageAll]send data len is zero.\n"));
        return false;
    }

    //全部群发
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CProConnectManager* pConnectManager = m_objProConnnectManagerList[i];

        if(NULL == pConnectManager)
        {
            OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessageAll]No find send Queue object.\n"));
            continue;
        }

        if (false == pConnectManager->PostMessageAll(pBuffPacket, u1SendType, u2CommandID, u1SendState, false, nMessageID))
        {
            OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessageAll]PostMessageAll error.\n"));
        }
    }

    if(true == blDelete)
    {
        //用完了就删除
        App_BuffPacketManager::instance()->Delete(pBuffPacket);
    }

    return true;
}

bool CProConnectManagerGroup::PostMessageAll( char*& pData, uint32 nDataLen, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    if (0 == nDataLen)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessageAll]send data len is zero.\n"));
        return false;
    }

    IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    if(NULL == pBuffPacket)
    {
        OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessageAll]pBuffPacket is NULL.\n"));

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
        CProConnectManager* pConnectManager = m_objProConnnectManagerList[i];

        if(NULL == pConnectManager)
        {
            OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
            continue;
        }

        if (false == pConnectManager->PostMessageAll(pBuffPacket, u1SendType, u2CommandID, u1SendState, false, nMessageID))
        {
            OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessageAll]PostMessageAll error.\n"));
        }
    }

    //用完了就删除
    App_BuffPacketManager::instance()->Delete(pBuffPacket);

    //用完了就删除
    if(true == blDelete)
    {
        SAFE_DELETE_ARRAY(pData);
    }

    return true;
}

bool CProConnectManagerGroup::SetConnectName( uint32 u4ConnectID, const char* pName )
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::CloseConnect]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->SetConnectName(u4ConnectID, pName);
}

bool CProConnectManagerGroup::SetIsLog( uint32 u4ConnectID, bool blIsLog )
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::CloseConnect]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->SetIsLog(u4ConnectID, blIsLog);
}

void CProConnectManagerGroup::GetClientNameInfo( const char* pName, vecClientNameInfo& objClientNameInfo )
{
    objClientNameInfo.clear();

    //全部查找
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CProConnectManager* pConnectManager = m_objProConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            pConnectManager->GetClientNameInfo(pName, objClientNameInfo);
        }
    }
}

uint16 CProConnectManagerGroup::GetConnectCheckTime()
{
    return GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime;
}

void CProConnectManagerGroup::GetCommandData(uint16 u2CommandID, _CommandData& objCommandData)
{
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CProConnectManager* pConnectManager = m_objProConnnectManagerList[i];

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

void CProConnectManagerGroup::GetFlowInfo(uint32& u4FlowIn, uint32& u4FlowOut)
{
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CProConnectManager* pConnectManager = m_objProConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            pConnectManager->GetFlowInfo(u4FlowIn, u4FlowOut);
        }
    }
}

EM_Client_Connect_status CProConnectManagerGroup::GetConnectState(uint32 u4ConnectID)
{
    //判断命中到哪一个线程组里面去
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::CloseConnect]No find send Queue object.\n"));
        return CLIENT_CONNECT_NO_EXIST;
    }

    return pConnectManager->GetConnectState(u4ConnectID);
}

int CProConnectManagerGroup::handle_write_file_stream(uint32 u4ConnectID, const char* pData, uint32 u4Size, uint8 u1ParseID)
{
    //文件入口 指定一个线程组
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CProConnectManager* pConnectManager = m_objProConnnectManagerList[u2ThreadIndex];

    if (NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::file_open]No find.\n"));
        return 0;
    }

    return pConnectManager->handle_write_file_stream(u4ConnectID, pData, u4Size, u1ParseID);
}

CProConnectManager* CProConnectManagerGroup::GetManagerFormList(int nIndex)
{
    if (nIndex < 0 || nIndex >= m_u2ThreadQueueCount)
    {
        return NULL;
    }
    else
    {
        return m_objProConnnectManagerList[nIndex];
    }
}
