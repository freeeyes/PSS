#include "ConnectHandler.h"

CConnectHandler::CConnectHandler(void)
{
}

const char* CConnectHandler::GetError() const
{
    return m_szError;
}

void CConnectHandler::Close()
{
    //�������ӶϿ���Ϣ
    App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->DisConnect(GetConnectID());

    if (CONNECTSTATE::CONNECT_SERVER_CLOSE == m_u1ConnectState)
    {
        //�����������Ͽ�
        _MakePacket objMakePacket;

		objMakePacket.m_u4ConnectID     = GetConnectID();
		objMakePacket.m_pPacketParse    = NULL;
		objMakePacket.m_u1Option        =  PACKET_SDISCONNECT;
		objMakePacket.m_AddrRemote      = m_addrRemote;
		objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

        Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);
    }
    else
    {
		_MakePacket objMakePacket;

		objMakePacket.m_u4ConnectID     = GetConnectID();
		objMakePacket.m_pPacketParse    = NULL;
		objMakePacket.m_u1Option        = PACKET_CDISCONNECT;
		objMakePacket.m_AddrRemote      = m_addrRemote;
		objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

        //�ͻ������ӶϿ�
        Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);
    }

    shutdown();
    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, m_u8RecvQueueTimeCost = %dws, m_u4RecvQueueCount = %d, m_u8SendQueueTimeCost = %dws.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, (uint32)m_u8RecvQueueTimeCost, m_u4RecvQueueCount, (uint32)m_u8SendQueueTimeCost);

    //�������PacketParse����
    ClearPacketParse();

    OUR_DEBUG((LM_ERROR, "[CConnectHandler::Close](0x%08x)Close(ConnectID=%d) OK.\n", this, GetConnectID()));

    //ɾ�������б��еĶ�������,�����һ���ж��������0˵����ǰ������δ���Manager��ӡ�
    if (GetConnectID() > 0)
    {
        App_ConnectManager::instance()->CloseConnectByClient(GetConnectID());
    }

    //����ת���ӿ�
    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->DisConnectRegedit(m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), ENUM_FORWARD_TCP_CLINET);
        m_strDeviceName = "";
    }

    //�ع��ù���ָ��
    App_ConnectHandlerPool::instance()->Delete(this);
}

void CConnectHandler::CloseFinally()
{
    this->closing_ = true;
    SAFE_DELETE_ARRAY(m_pPacketDebugData);
    m_u4PacketDebugSize = 0;
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
    m_nBlockCount         = 0;
    m_u8SendQueueTimeCost = 0;
    m_blIsLog             = false;
    m_szConnectName[0]    = '\0';
    m_u1IsActive          = 1;

    //���û�����

    if (NULL == App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID))
    {
        //��������������ڣ���ֱ�ӶϿ�����
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open](%s)can't find PacketParseInfo.\n", m_addrRemote.get_host_addr()));
        return -1;
    }

    //���Զ�����ӵ�ַ�Ͷ˿�
    if(this->peer().get_remote_addr(m_addrRemote) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]this->peer().get_remote_addr error.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::open]this->peer().get_remote_addr error.");
        return -1;
    }

    if(App_ForbiddenIP::instance()->CheckIP(m_addrRemote.get_host_addr()) == false)
    {
        //�ڽ�ֹ�б��У����������
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]IP Forbidden(%s).\n", m_addrRemote.get_host_addr()));
        return -1;
    }

    //��鵥λʱ�����Ӵ����Ƿ�ﵽ����
    if(false == App_IPAccount::instance()->AddIP((string)m_addrRemote.get_host_addr()))
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]IP connect frequently(%s).\n", m_addrRemote.get_host_addr()));
        App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), GetXmlConfigAttribute(xmlIP)->Timeout);

        //���͸澯�ʼ�
        AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECT,
                GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                "Alert IP",
                "[CConnectHandler::open]IP is more than IP Max(%s).", m_addrRemote.get_host_addr());

        return -1;
    }

    //��ʼ�������
    m_TimeConnectInfo.Init(GetXmlConfigAttribute(xmlClientData)->RecvPacketCount,
                           GetXmlConfigAttribute(xmlClientData)->RecvDataMax,
                           GetXmlConfigAttribute(xmlClientData)->SendPacketCount,
                           GetXmlConfigAttribute(xmlClientData)->SendDataMax);

    int nRet = 0;

    //��������Ϊ������ģʽ
    if (this->peer().enable(ACE_NONBLOCK) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]this->peer().enable  = ACE_NONBLOCK error.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::open]this->peer().enable  = ACE_NONBLOCK error.");
        return -1;
    }

    //�鿴�Ƿ����ת������
    m_strDeviceName = App_ForwardManager::instance()->ConnectRegedit(m_addrRemote.get_host_addr(),
                      m_addrRemote.get_port_number(),
                      ENUM_FORWARD_TCP_CLINET,
                      dynamic_cast<IDeviceHandler*>(this));

    //��ʼ����������
    if (-1 == Init_Open_Connect())
    {
        return -1;
    }

    if(m_pCurrMessage == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]pmb new is NULL.\n"));
        return -1;
    }

    //��������ӷ������ӿ�
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

    m_emIOType = EM_IO_TYPE::NET_INPUT;
    return nRet;
}

//��������
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

    //�ж����ݰ��ṹ�Ƿ�ΪNULL
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

        //����ǶϿ�ָ���ִ�����ӶϿ�����
        if (pmbSendData->msg_type() == ACE_Message_Block::MB_STOP)
        {
            App_MessageBlockManager::instance()->Close(pmbSendData);
            return -1;
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CConnectHandler::handle_output]ConnectID=%d unknow command.\n", GetConnectID()));
        }
    }

    int nWakeupRet =  reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);

    if (-1 == nWakeupRet)
    {
        OUR_DEBUG((LM_INFO, "[CConnectHandler::handle_output]ConnectID=%d,nWakeupRet=%d, errno=%d.\n", GetConnectID(), nWakeupRet, errno));
    }

    return 0;
}

//����������ݴ���
int CConnectHandler::RecvData()
{
    return Dispose_Recv_Data();
}

int CConnectHandler::Dispose_Recv_Data()
{
    ACE_Time_Value nowait(0, MAX_QUEUE_TIMEOUT);

    //�����ݰ���С
    uint32 u4NewPacketSize = 0;

    //�жϻ����Ƿ�ΪNULL
    if (m_pCurrMessage == NULL)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData]m_pCurrMessage == NULL.\n"));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::RecvData]m_pCurrMessage == NULL.");

        return -1;
    }

    //����Ӧ�ý��յ����ݳ���
    int nCurrCount = 0;

    Get_Recv_length(nCurrCount);

    //������Ҫ��m_u4CurrSize���м�顣
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

        //�����-1 ��Ϊ11�Ĵ��󣬺���֮
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

    //�����DEBUG״̬����¼��ǰ���ܰ��Ķ���������
    Output_Debug_Data(m_pCurrMessage, LOG_SYSTEM_DEBUG_CLIENTRECV);

    //�鿴�Ƿ���Ҫת��
    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->SendData(m_strDeviceName, m_pCurrMessage);
        m_pCurrMessage->reset();
        return 0;
    }

    if (App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        //���û�ж��꣬�̶�
        if (nCurrCount - nDataLen > 0)
        {
            //û���꣬������
            return 0;
        }

        if (m_pCurrMessage->length() == App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength
            && m_pPacketParse->GetIsHandleHead())
        {
            //����Head��Ϣͷ
            return Dispose_Paceket_Parse_Head();
        }
        else
        {
            //��������������ɣ���ʼ�����������ݰ�
            if (-1 == Dispose_Paceket_Parse_Body() || false == CheckMessage())
            {
                return -1;
            }

            m_u4CurrSize = 0;

            //�����µİ�
            m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

            if (NULL == m_pPacketParse)
            {
                OUR_DEBUG((LM_DEBUG, "[%t|CConnectHandle::RecvData] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
                return -1;
            }

            //������һ�����Ĵ�С
            u4NewPacketSize = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength;
        }
    }
    else
    {
        //����ģʽ����
        if (-1 == Dispose_Paceket_Parse_Stream(m_pCurrMessage))
        {
            return -1;
        }

        App_MessageBlockManager::instance()->Close(m_pCurrMessage);
        m_u4CurrSize = 0;

        //������һ�����Ĵ�С
        u4NewPacketSize = GetXmlConfigAttribute(xmlClientInfo)->MaxBuffRecv;
    }

    //����ͷ�Ĵ�С��Ӧ��mb
    m_pCurrMessage = App_MessageBlockManager::instance()->Create(u4NewPacketSize);

    if (m_pCurrMessage == NULL)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, m_u8RecvQueueTimeCost = %dws, m_u4RecvQueueCount = %d, m_u8SendQueueTimeCost = %dws.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, (uint32)m_u8RecvQueueTimeCost, m_u4RecvQueueCount, (uint32)m_u8SendQueueTimeCost);
        OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvData] pmb new is NULL.\n"));
        return -1;
    }

    return 0;
}

int CConnectHandler::Init_Open_Connect()
{
    //����Ĭ�ϱ���
    SetConnectName(m_addrRemote.get_host_addr());

    //��ʼ����ǰ���ӵ�ĳЩ����
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
        //��������˽���Nagle�㷨��������Ҫ���á�
        int nOpt = 1;
        ACE_OS::setsockopt(this->get_handle(), IPPROTO_TCP, TCP_NODELAY, (char*)&nOpt, sizeof(int));
    }

    m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

    if (NULL == m_pPacketParse)
    {
        OUR_DEBUG((LM_DEBUG, "[CConnectHandler::open]Open(%d) m_pPacketParse new error.\n", GetConnectID()));
        return -1;
    }

    //����ͷ�Ĵ�С��Ӧ��mb
    if ("" != m_strDeviceName)
    {
        m_pCurrMessage = App_MessageBlockManager::instance()->Create(GetXmlConfigAttribute(xmlClientInfo)->MaxBuffRecv);
    }
    else if (App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        m_pCurrMessage = App_MessageBlockManager::instance()->Create(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength);
    }
    else
    {
        m_pCurrMessage = App_MessageBlockManager::instance()->Create(GetXmlConfigAttribute(xmlClientInfo)->MaxBuffRecv);
    }

    return 0;
}

//�ر�����
int CConnectHandler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
    if(h == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_DEBUG,"[CConnectHandler::handle_close] h is NULL mask=%d.\n", (int)mask));
    }

    //���÷�����Ϣ���в����ٷ����κ���Ϣ
    if(m_u1ConnectState != CONNECTSTATE::CONNECT_SERVER_CLOSE)
    {
        m_u1ConnectState = CONNECTSTATE::CONNECT_CLIENT_CLOSE;
    }

    //��ȡ���Ͷ����ڲ����������ݣ����Ϊ��Ч�������ڴ�
    ACE_Message_Block* pmbSendData = NULL;
    ACE_Time_Value nowait(ACE_OS::gettimeofday());

    while (-1 != this->getq(pmbSendData, &nowait))
    {
        App_MessageBlockManager::instance()->Close(pmbSendData);
    }

    Close();

    return 0;
}

bool CConnectHandler::Device_Send_Data(const char* pData, ssize_t nLen)
{
    uint16 u2CommandID = 0x0000;

    IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);
    pBuffPacket->WriteStream(pData, (uint32)nLen);

    uint8 u1State = PACKET_SEND_IMMEDIATLY;
    uint8 u1SendState = SENDMESSAGE_JAMPNOMAL;
    uint32 u4PacketSize = 0;

    return SendMessage(u2CommandID, pBuffPacket, u1State, u1SendState, u4PacketSize, true, 0);
}

uint32 CConnectHandler::file_open(IFileTestManager* pFileTest)
{
    m_blBlockState = false;
    m_nBlockCount = 0;
    m_u8SendQueueTimeCost = 0;
    m_blIsLog = false;
    m_szConnectName[0] = '\0';
    m_u1IsActive = 1;

    //���û�����

    if (NULL == App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID))
    {
        //��������������ڣ���ֱ�ӶϿ�����
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open](%s)can't find PacketParseInfo.\n", m_addrRemote.get_host_addr()));
        return -1;
    }

    //��ʼ�������
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

    //��������ӷ������ӿ�
    if (false == App_ConnectManager::instance()->AddConnect(this))
    {
        OUR_DEBUG((LM_ERROR, "%s.\n", App_ConnectManager::instance()->GetError()));
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", App_ConnectManager::instance()->GetError());
        return 0;
    }

    ConnectOpen();

    m_emIOType       = EM_IO_TYPE::FILE_INPUT;
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

        memcpy_safe(pData, u4PacketHead, pMbHead->wr_ptr(), u4PacketHead);
        pMbHead->wr_ptr(u4PacketHead);

        //������Ϣͷ
        _Head_Info objHeadInfo;
        bool blStateHead = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Head_Info(GetConnectID(), pMbHead, App_MessageBlockManager::instance(), &objHeadInfo, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

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

        //������Ϣ��
        ACE_Message_Block* pMbBody = App_MessageBlockManager::instance()->Create(u4Size - u4PacketHead);

        memcpy_safe(&pData[u4PacketHead], u4Size - u4PacketHead, pMbBody->wr_ptr(), u4Size - u4PacketHead);
        pMbBody->wr_ptr(u4Size - u4PacketHead);

        //�������ݰ���
        _Body_Info obj_Body_Info;
        bool blStateBody = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Body_Info(GetConnectID(), pMbBody, App_MessageBlockManager::instance(), &obj_Body_Info, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

        if (false == blStateBody)
        {
            //������ݰ���Ƿ����Ͽ�����
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_write_file_stream]Parse_Packet_Body_Info is illegal.\n"));

            //����PacketParse
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
        //��ģʽ�����ļ�����
        ACE_Message_Block* pCurrMessage = App_MessageBlockManager::instance()->Create(u4Size);
        memcpy_safe(pData, u4Size, pCurrMessage->wr_ptr(), u4Size);
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
    //���������ֵ�����¼����־��ȥ
    if((uint32)(m_u8RecvQueueTimeout * 1000) <= u4TimeCost)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_RECVQUEUEERROR, "[TCP]IP=%s,Prot=%d, m_u8RecvQueueTimeout=[%d], Timeout=[%d].", GetClientIPInfo().m_szClientIP, GetClientIPInfo().m_u2Port, (uint32)m_u8RecvQueueTimeout, u4TimeCost);
    }

    m_u4RecvQueueCount++;
}

void CConnectHandler::SetSendQueueTimeCost(uint32 u4TimeCost) const
{
    //���������ֵ�����¼����־��ȥ
    if((uint32)(m_u8SendQueueTimeout) <= u4TimeCost)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_SENDQUEUEERROR, "[TCP]IP=%s,Prot=%d,m_u8SendQueueTimeout = [%d], Timeout=[%d].", GetClientIPInfo().m_szClientIP, GetClientIPInfo().m_u2Port, (uint32)m_u8SendQueueTimeout, u4TimeCost);

		_MakePacket objMakePacket;

		objMakePacket.m_u4ConnectID     = GetConnectID();
		objMakePacket.m_pPacketParse    = NULL;
		objMakePacket.m_u1Option        = PACKET_SEND_TIMEOUT;
		objMakePacket.m_AddrRemote      = m_addrRemote;
		objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

        Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);
    }
}

CONNECTSTATE CConnectHandler::GetConnectState() const
{
    return m_u1ConnectState;
}

CONNECTSTATE CConnectHandler::GetSendBuffState() const
{
    return m_u1SendBuffState;
}

bool CConnectHandler::SendMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, uint8 u1State, uint8 u1SendType, uint32& u4PacketSize, bool blDelete, int nMessageID)
{
    //�����ǰ�����ѱ�����̹߳رգ������ﲻ������ֱ���˳�
    if (m_u1IsActive == 0)
    {
        //������Ӳ������ˣ������ﷵ��ʧ�ܣ��ص���ҵ���߼�ȥ����
        Tcp_Common_Send_Message_Error(GetConnectID(), u2CommandID, blDelete, pBuffPacket);

        return false;
    }

    if (EM_IO_TYPE::NET_INPUT == m_emIOType)
    {
        if (CONNECTSTATE::CONNECT_SERVER_CLOSE == m_u1ConnectState 
            || CONNECTSTATE::CONNECT_CLIENT_CLOSE == m_u1ConnectState)
        {
            //�ڶ������Ѿ����ڹر�����ָ�֮�����������д����ȫ�����跢�͡�
            ACE_Message_Block* pSendMessage = App_MessageBlockManager::instance()->Create(pBuffPacket->GetPacketLen());
            memcpy_safe(pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pSendMessage->wr_ptr(), pBuffPacket->GetPacketLen());
            pSendMessage->wr_ptr(pBuffPacket->GetPacketLen());
            ACE_Time_Value tvNow = ACE_OS::gettimeofday();
            App_MakePacket::instance()->PutSendErrorMessage(0, pSendMessage, tvNow);

            Recovery_Common_BuffPacket(blDelete, pBuffPacket);

            return false;
        }

        //�������ֱ�ӷ������ݣ���ƴ�����ݰ�
        if (u1State == PACKET_SEND_CACHE)
        {
            return Send_Input_To_Cache(u1SendType, u4PacketSize, u2CommandID, blDelete, pBuffPacket);
        }
        else
        {
            //Ҫ�������ݵ��Զ�
            return Send_Input_To_TCP(u1SendType, u4PacketSize, u2CommandID, u1State, nMessageID, blDelete, pBuffPacket);
        }
    }
    else
    {
        //�ļ���ڣ�ֱ��д����־
        return Write_SendData_To_File(blDelete, pBuffPacket);
    }
}

bool CConnectHandler::SendCloseMessage()
{
    if (EM_IO_TYPE::NET_INPUT == m_emIOType)
    {
        //��������Ϣ�Ͽ�����outputȥִ�У������Ͳ���Ҫͬ�������ˡ�
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

        m_u1ConnectState = CONNECTSTATE::CONNECT_SERVER_CLOSE;
    }
    else
    {
        //�رջ��յ�ǰ����
        Close();
    }

    return true;
}

bool CConnectHandler::SendTimeoutMessage() const
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

bool CConnectHandler::PutSendPacket(ACE_Message_Block* pMbData)
{
	ACE_Message_Block* pmbSend = App_MessageBlockManager::instance()->Create(pMbData->length());
	pmbSend->copy(pMbData->rd_ptr(), pMbData->length());

    //�����DEBUG״̬����¼��ǰ���Ͱ��Ķ���������
    Output_Debug_Data(pmbSend, LOG_SYSTEM_DEBUG_CLIENTSEND);

    //ͳ�Ʒ�������
    ACE_Date_Time dtNow;

    if(false == m_TimeConnectInfo.SendCheck((uint8)dtNow.minute(), 1, (uint32)pmbSend->length()))
    {
        //�������޶��ķ�ֵ����Ҫ�ر����ӣ�����¼��־
        AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECTABNORMAL,
                GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                "Alert",
                "[TCP]IP=%s,Prot=%d,SendPacketCount=%d, SendSize=%d.",
                m_addrRemote.get_host_addr(),
                m_addrRemote.get_port_number(),
                m_TimeConnectInfo.m_u4SendPacketCount,
                m_TimeConnectInfo.m_u4SendSize);

        //���÷��ʱ��
        App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), GetXmlConfigAttribute(xmlIP)->Timeout);
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, Send Data is more than limit.\n", GetConnectID()));

        ACE_Time_Value tvNow = ACE_OS::gettimeofday();
        App_MakePacket::instance()->PutSendErrorMessage(GetConnectID(), pmbSend, tvNow);

        return false;
    }

    //���ͳ�ʱʱ������
    ACE_Time_Value  nowait(0, m_u4SendThresHold*MAX_BUFF_1000);

    if(NULL == pmbSend)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID()));
        return false;
    }

    if(get_handle() == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID()));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::PutSendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID());
        ACE_Time_Value tvNow = ACE_OS::gettimeofday();
        App_MakePacket::instance()->PutSendErrorMessage(GetConnectID(), pmbSend, tvNow);
        App_MessageBlockManager::instance()->Close(pmbSend);
        return false;
    }

    //��������
    int nSendPacketLen = (int)pmbSend->length();
    int nIsSendSize    = 0;

    //ѭ�����ͣ�ֱ�����ݷ�����ɡ�
    while(true)
    {
        if(nSendPacketLen <= 0)
        {
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, nCurrSendSize error is %d.\n", GetConnectID(), nSendPacketLen));
            App_MessageBlockManager::instance()->Close(pmbSend);
            return false;
        }

        int nDataLen = (int)this->peer().send(pmbSend->rd_ptr(), nSendPacketLen - nIsSendSize, &nowait);

        if(nDataLen <= 0)
        {
            int nErrno = errno;
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, error = %d.\n", GetConnectID(), nErrno));

            AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "WriteError [%s:%d] nErrno = %d  result.bytes_transferred() = %d, ",
                                                  m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), nErrno,
                                                  nIsSendSize);

            //������Ϣ�ص�
            pmbSend->rd_ptr((size_t)0);
            ACE_Time_Value tvNow = ACE_OS::gettimeofday();
            App_MakePacket::instance()->PutSendErrorMessage(GetConnectID(), pmbSend, tvNow);

            OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID=%d send cancel.\n", GetConnectID()));

            return false;
        }
        else if(nDataLen >= nSendPacketLen - nIsSendSize)   //�����ݰ�ȫ��������ϣ���ա�
        {
            m_u4AllSendCount    += 1;
            m_u4AllSendSize     += (uint32)pmbSend->length();
            m_atvOutput         = ACE_OS::gettimeofday();

            int nMessageID = pmbSend->msg_type() - ACE_Message_Block::MB_USER;

            if(nMessageID > 0)
            {
                //��Ҫ�ص����ͳɹ���ִ
                CPacketParse objPacketParse;
                ACE_Message_Block* pSendOKData = App_MessageBlockManager::instance()->Create(sizeof(int));
                memcpy_safe((char*)&nMessageID, sizeof(int), pSendOKData->wr_ptr(), sizeof(int));
                pSendOKData->wr_ptr(sizeof(int));
                objPacketParse.SetPacket_Head_Message(pSendOKData);
                objPacketParse.SetPacket_Head_Curr_Length((uint32)pSendOKData->length());

				_MakePacket objMakePacket;

				objMakePacket.m_u4ConnectID     = GetConnectID();
				objMakePacket.m_pPacketParse    = &objPacketParse;
				objMakePacket.m_u1Option        = PACKET_SEND_OK;
				objMakePacket.m_AddrRemote      = m_addrRemote;
				objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

                Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

                //��ԭ��Ϣ����
                pmbSend->msg_type(ACE_Message_Block::MB_DATA);
            }

            App_MessageBlockManager::instance()->Close(pmbSend);

            return true;
        }
        else
        {
            pmbSend->rd_ptr(nDataLen);
            nIsSendSize      += nDataLen;
            m_atvOutput      = ACE_OS::gettimeofday();
        }
    }
}

void CConnectHandler::ConnectOpen()
{
    //д��������־
    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Connection from [%s:%d]ConnectID=%d, GetHandlerID=%d.",
                                          m_addrRemote.get_host_addr(),
                                          m_addrRemote.get_port_number(),
                                          GetConnectID(),
                                          GetHandlerID());

    //����PacketParse����Ӧ����
    App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Connect(GetConnectID(), GetClientIPInfo(), GetLocalIPInfo());

	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID     = GetConnectID();
	objMakePacket.m_pPacketParse    = NULL;
	objMakePacket.m_u1Option        = PACKET_CONNECT;
	objMakePacket.m_AddrRemote      = m_addrRemote;
	objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

    Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

    OUR_DEBUG((LM_DEBUG, "[CConnectHandler::open]Open(%d) Connection from [%s:%d](0x%08x).\n", GetConnectID(), m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), this));

    m_u1ConnectState = CONNECTSTATE::CONNECT_OPEN;
}

void CConnectHandler::Get_Recv_length(int& nCurrCount) const
{
    if("" != m_strDeviceName)
    {
        nCurrCount = GetXmlConfigAttribute(xmlClientInfo)->MaxBuffRecv - (uint32)m_pCurrMessage->length();
    }
    else if (App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        if (m_pPacketParse->GetIsHandleHead())
        {
            nCurrCount = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength - (uint32)m_pCurrMessage->length();
        }
        else
        {
            nCurrCount = m_pPacketParse->GetPacketBodySrcLen() - (uint32)m_pCurrMessage->length();
        }
    }
    else
    {
        nCurrCount = GetXmlConfigAttribute(xmlClientInfo)->MaxBuffRecv - (uint32)m_pCurrMessage->length();
    }
}

void CConnectHandler::Output_Debug_Data(const ACE_Message_Block* pMbData, int nLogType)
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

        const char* pData = pMbData->rd_ptr();

        for (uint32 i = 0; i < u4DebugSize; i++)
        {
            sprintf_safe(szLog, 10, "0x%02X ", (unsigned char)pData[i]);
            sprintf_safe(m_pPacketDebugData + 5 * i, MAX_BUFF_1024 - 5 * i, "%s", szLog);
        }

        m_pPacketDebugData[5 * u4DebugSize] = '\0';

        if (blblMore == true)
        {
            AppLogManager::instance()->WriteLog_i(nLogType, "[(%s)%s:%d]%s.(���ݰ�����ֻ��¼ǰ200�ֽ�)", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_pPacketDebugData);
        }
        else
        {
            AppLogManager::instance()->WriteLog_i(nLogType, "[(%s)%s:%d]%s.", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_pPacketDebugData);
        }
    }
}

int CConnectHandler::Dispose_Paceket_Parse_Head()
{
    _Head_Info objHeadInfo;
    bool blStateHead = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Head_Info(GetConnectID(), m_pCurrMessage, App_MessageBlockManager::instance(), &objHeadInfo, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

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

    //�������ֻ�����ͷ������
    //�������ֻ�а�ͷ������Ҫ���壬�����������һЩ����������ֻ�����ͷ���ӵ�DoMessage()
    if (u4PacketBodyLen == 0)
    {
        //ֻ�����ݰ�ͷ
        if (false == CheckMessage())
        {
            return -1;
        }

        m_u4CurrSize = 0;

        //�����µİ�
        m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

        if (NULL == m_pPacketParse)
        {
            OUR_DEBUG((LM_DEBUG, "[%t|CConnectHandle::RecvData] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
            return -1;
        }

        //����ͷ�Ĵ�С��Ӧ��mb
        m_pCurrMessage = App_MessageBlockManager::instance()->Create(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength);

        if (m_pCurrMessage == NULL)
        {
            AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, m_u8RecvQueueTimeCost = %dws, m_u4RecvQueueCount = %d, m_u8SendQueueTimeCost = %dws.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, (uint32)m_u8RecvQueueTimeCost, m_u4RecvQueueCount, (uint32)m_u8SendQueueTimeCost);
            OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvData] pmb new is NULL.\n"));
            return -1;
        }
    }
    else
    {
        //����������������ȣ�Ϊ�Ƿ�����
        if (u4PacketBodyLen >= m_u4MaxPacketSize)
        {
            m_u4CurrSize = 0;
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData]u4PacketHeadLen(%d) more than %d.\n", u4PacketBodyLen, m_u4MaxPacketSize));

            return -1;
        }
        else
        {
            //����ͷ�Ĵ�С��Ӧ��mb
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
    bool blStateBody = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Body_Info(GetConnectID(), m_pCurrMessage, App_MessageBlockManager::instance(), &obj_Body_Info, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

    if (false == blStateBody)
    {
        //������ݰ����Ǵ���ģ���Ͽ�����
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
        //����һ���ݿ�
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
        //�������޶��ķ�ֵ����Ҫ�ر����ӣ�����¼��־
        AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECTABNORMAL,
                GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                "Alert",
                "[TCP]IP=%s,Prot=%d,PacketCount=%d, RecvSize=%d.",
                m_addrRemote.get_host_addr(),
                m_addrRemote.get_port_number(),
                m_TimeConnectInfo.m_u4RecvPacketCount,
                m_TimeConnectInfo.m_u4RecvSize);

        App_PacketParsePool::instance()->Delete(m_pPacketParse, true);
        m_pPacketParse = NULL;

        //���÷��ʱ��
        App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), GetXmlConfigAttribute(xmlIP)->Timeout);
        OUR_DEBUG((LM_ERROR, "[CConnectHandle::CheckMessage] ConnectID = %d, PutMessageBlock is check invalid.\n", GetConnectID()));
        return false;
    }

	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID     = GetConnectID();
	objMakePacket.m_pPacketParse    = m_pPacketParse;
	objMakePacket.m_u1Option        = PACKET_PARSE;
	objMakePacket.m_AddrRemote      = m_addrRemote;
	objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

    Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

    //����ʱ������
    App_ConnectManager::instance()->SetConnectTimeWheel(this);

    App_PacketParsePool::instance()->Delete(m_pPacketParse);

    return true;
}

_ClientConnectInfo CConnectHandler::GetClientInfo() const
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

_ClientIPInfo  CConnectHandler::GetClientIPInfo() const
{
    _ClientIPInfo ClientIPInfo;
    sprintf_safe(ClientIPInfo.m_szClientIP, MAX_BUFF_50, "%s", m_addrRemote.get_host_addr());
    ClientIPInfo.m_u2Port = m_addrRemote.get_port_number();
    return ClientIPInfo;
}

_ClientIPInfo  CConnectHandler::GetLocalIPInfo()
{
    _ClientIPInfo ClientIPInfo;
    sprintf_safe(ClientIPInfo.m_szClientIP, MAX_BUFF_50, "%s", m_szLocalIP);
    ClientIPInfo.m_u2Port = m_u2LocalPort;
    return ClientIPInfo;
}

bool CConnectHandler::CheckSendMask(uint32 u4PacketLen)
{
    m_u4ReadSendSize += u4PacketLen;

    //Linux�²��ü�⣬ֱ�ӷ���true
    return true;
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
    //����Ϣ������У���output�ڷ�Ӧ���̷߳��͡�
    ACE_Time_Value xtime = ACE_OS::gettimeofday();

    //���������������ٷŽ�ȥ��,�Ͳ��Ž�ȥ��
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
    //�ļ���ڣ�ֱ��д����־
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
    ACE_UNUSED_ARG(nMessageID);

    _Send_Packet_Param obj_Send_Packet_Param;
    obj_Send_Packet_Param.m_blDelete            = blDelete;
    obj_Send_Packet_Param.m_u1SendType          = u1SendType;
    obj_Send_Packet_Param.m_u2CommandID         = u2CommandID;
    obj_Send_Packet_Param.m_u4ConnectID         = GetConnectID();
    obj_Send_Packet_Param.m_u4PacketParseInfoID = m_u4PacketParseInfoID;
    obj_Send_Packet_Param.m_u4SendMaxBuffSize   = m_u4SendMaxBuffSize;

    //ƴװ����
    if (false == Tcp_Common_Make_Send_Packet(obj_Send_Packet_Param,
                   pBuffPacket,
                   m_pBlockMessage,
                   u4PacketSize))
    {
        return false;
    }

	//�����ڴ�
	Recovery_Common_BuffPacket(blDelete, pBuffPacket);

    //ֱ�ӷ������ݣ����ڷŵ������������ѹ������д���̫��
	bool blRet = PutSendPacket(m_pBlockMessage);

	if (true == blRet)
	{
		if (m_u4ReadSendSize >= m_u4SuccessSendSize + m_pBlockMessage->length())
		{
			//��¼�ɹ������ֽ�
			m_u4SuccessSendSize += m_pBlockMessage->length();
		}

		m_pBlockMessage->reset();
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CConnectHandler::handle_output]ConnectID=%d write is close.\n", GetConnectID()));
		return -1;
	}

    //�����Ҫ������ɺ�ɾ���������ñ��λ
    if (PACKET_SEND_FIN_CLOSE == u1State)
    {
        m_u1ConnectState = CONNECTSTATE::CONNECT_SERVER_CLOSE;

        //��ӹر�socketָ��
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

        //�����µİ�
        m_pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

        if (NULL == m_pPacketParse)
        {
            OUR_DEBUG((LM_DEBUG, "[%t|CConnectHandle::RecvData] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
            return -1;
        }

        //�����Ƿ���������
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

        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, m_u8RecvQueueTimeCost = %dws, m_u4RecvQueueCount = %d, m_u8SendQueueTimeCost = %dws.",
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
    sprintf_safe(m_szLocalIP, MAX_BUFF_50, "%s", pLocalIP);
    m_u2LocalPort = u2LocalPort;
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
    //�����µİ�
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
CConnectManager::CConnectManager(void) : m_cond(m_mutex)
{
    m_tvCheckConnect     = ACE_OS::gettimeofday();

    //��ʼ�����Ͷ����
    m_SendMessagePool.Init();
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

    //��ʼ�ر���������
    for(auto* pConnectHandler : vecCloseConnectHandler)
    {
        pConnectHandler->Close();
    }

    //ɾ��ʱ������
    m_TimeWheelLink.Close();

    //ɾ��hash��ռ�
    m_objHashConnectList.Close();

    //ɾ���������
    m_SendCacheManager.Close();
}

bool CConnectManager::Close(uint32 u4ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if (false == DelConnectTimeWheel(m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID)))
    {
        OUR_DEBUG((LM_INFO, "[CConnectManager::Close]DelConnectTimeWheel error.\n"));
    }

    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "[CConnectManager::Close]ConnectID=%d.",
                                          u4ConnectID);

    int nPos = m_objHashConnectList.Del_Hash_Data_By_Unit32(u4ConnectID);

    if(0 > nPos)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "[CConnectManager::Close]ConnectID=%d FAIL.",
                                              u4ConnectID);

        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectManager::Close] ConnectID[%d] is not find.", u4ConnectID);
    }

    //���շ��ͻ���
    m_SendCacheManager.FreeCacheData(u4ConnectID);
    m_u4TimeDisConnect++;

    //��������ͳ�ƹ���
    App_ConnectAccount::instance()->AddDisConnect();

    return true;
}

bool CConnectManager::CloseUnLock(uint32 u4ConnectID)
{
    //���ӹرգ����ʱ������
    if (false == DelConnectTimeWheel(m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID)))
    {
        OUR_DEBUG((LM_INFO, "[CConnectManager::CloseUnLock]DelConnectTimeWheel error.\n"));
    }

    int nPos = m_objHashConnectList.Del_Hash_Data_By_Unit32(u4ConnectID);

    if(0 < nPos)
    {
        //���շ��ͻ���
        m_SendCacheManager.FreeCacheData(u4ConnectID);
        m_u4TimeDisConnect++;

        //��������ͳ�ƹ���
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
        //�ŵ���Ӧ���߳���ȥ��
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
    //���뷢�Ͷ���
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

    const CConnectHandler* pCurrConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if(NULL != pCurrConnectHandler)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectManager::AddConnect]ConnectID=%d is find.\n", u4ConnectID));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectManager::AddConnect] ConnectID[%d] is exist.", u4ConnectID);
        return false;
    }

    pConnectHandler->SetConnectID(u4ConnectID);
    pConnectHandler->SetSendCacheManager(&m_SendCacheManager);
    //����Hash����
    m_objHashConnectList.Add_Hash_Data_By_Key_Unit32(u4ConnectID, pConnectHandler);
    m_u4TimeConnect++;

    //��������ͳ�ƹ���
    App_ConnectAccount::instance()->AddConnect();

    //����ʱ������
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

bool CConnectManager::SendMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint16 u2CommandID, uint8 u1SendState, uint8 u1SendType, const ACE_Time_Value& tvSendBegin, bool blDelete, int nMessageID)
{
    //��Ϊ�Ƕ��е��ã��������ﲻ��Ҫ�����ˡ�
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

        //��¼��Ϣ��������ʱ��
        ACE_Time_Value tvInterval = ACE_OS::gettimeofday() - tvSendBegin;
        uint32 u4SendCost = (uint32)(tvInterval.msec());
        pConnectHandler->SetSendQueueTimeCost(u4SendCost);

        _ClientIPInfo objClientIP = pConnectHandler->GetLocalIPInfo();
        m_CommandAccount.SaveCommandData(u2CommandID, objClientIP.m_u2Port, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP, u4PacketSize, COMMAND_TYPE_OUT);
        return true;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CConnectManager::SendMessage] ConnectID[%d] is not find, PacketSize=%d.", u4ConnectID, pBuffPacket->GetPacketLen()));
        //������Ӳ������ˣ������ﷵ��ʧ�ܣ��ص���ҵ���߼�ȥ����
        Tcp_Common_Send_Message_Error(u4ConnectID, u2CommandID, blDelete, pBuffPacket);

        if(true == blDelete)
        {
            App_BuffPacketManager::instance()->Delete(pBuffPacket);
        }

        return true;
    }
}

bool CConnectManager::PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    //ֱ�ӷ��ͣ����������з��ͣ������塣
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

const char* CConnectManager::GetError() const
{
    return m_szError;
}

bool CConnectManager::StartTimer()
{
    //���������߳�
    if(0 != open())
    {
        OUR_DEBUG((LM_ERROR, "[CConnectManager::StartTimer]Open() is error.\n"));
        return false;
    }

    //���ⶨʱ���ظ�����
    if (false == KillTimer())
    {
        OUR_DEBUG((LM_ERROR, "[CConnectManager::StartTimer]KillTimer error.\n"));
    }

    OUR_DEBUG((LM_ERROR, "[CConnectManager::StartTimer]begin....\n"));
    //�õ��ڶ���Reactor
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

    if (GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime > 0)
    {
        long lTimeCheckID = pReactor->schedule_timer(this,
                            (const void*)m_pTCTimeSendCheck,
                            ACE_Time_Value(GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime, 0),
                            ACE_Time_Value(GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime, 0));

        if (-1 == lTimeCheckID)
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

    return true;
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

    const _TimerCheckID* pTimerCheckID = (const _TimerCheckID*)arg;

    if(NULL != pTimerCheckID && pTimerCheckID->m_u2TimerCheckID == PARM_CONNECTHANDLE_CHECK)
    {
        //��ʱ��ⷢ�ͣ����ｫ��ʱ��¼������Ϣ�������У�����һ����ʱ��
        m_TimeWheelLink.Tick();

        //�ж��Ƿ�Ӧ�ü�¼������־
        Tcp_Common_Manager_Timeout_CheckInfo(GetCount());
    }

    ACE_Time_Value tvEnd = ACE_OS::gettimeofday();

    //�õ�ʱ��ִ���˶����룬����¼��־
    ACE_Time_Value tvCost = tvEnd - tvNow;
    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_WORKTHREAD, "[CConnectManager::handle_timeout]TimeCost=%d.", tvCost.msec());

    return 0;
}

void CConnectManager::TimeWheel_Timeout_Callback(void* pArgsContext, vector<CConnectHandler*> vecConnectHandle)
{
    OUR_DEBUG((LM_INFO, "[CConnectManager::TimeWheel_Timeout_Callback]Timeout Count(%d).\n", vecConnectHandle.size()));

    for (const CConnectHandler* pConnectHandler : vecConnectHandle)
    {
        //�Ͽ���ʱ������
        CConnectManager* pManager = reinterpret_cast<CConnectManager*>(pArgsContext);
        OUR_DEBUG((LM_INFO, "[CConnectManager::TimeWheel_Timeout_Callback]ConnectID(%d).\n", pConnectHandler->GetConnectID()));

        //֪ͨҵ��������ʱ��Ϣ
        pConnectHandler->SendTimeoutMessage();

        if (NULL != pManager && false == pManager->CloseConnect_By_Queue(pConnectHandler->GetConnectID()))
        {
            OUR_DEBUG((LM_INFO, "[CConnectManager::TimeWheel_Timeout_Callback]CloseConnect_By_Queue error.\n"));
        }
    }
}

int CConnectManager::GetCount()
{
    return m_objHashConnectList.Get_Used_Count();
}

int CConnectManager::open()
{
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
        //������Ϣ������Ϣ
        if (false == Dispose_Queue())
        {
            break;
        }
    }

    //���շ��ͻ����
    m_SendMessagePool.Close();

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

    for(const auto* pConnectHandler : vecConnectHandler)
    {
        if(pConnectHandler != NULL)
        {
            VecClientConnectInfo.push_back(pConnectHandler->GetClientInfo());
        }
    }
}

_ClientIPInfo CConnectManager::GetClientIPInfo(uint32 u4ConnectID)
{
    const CConnectHandler* pConnectHandler = m_objHashConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

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

    for(const auto* pConnectHandler : objvecConnectManager)
    {
        IBuffPacket* pCurrBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

        if(NULL == pCurrBuffPacket)
        {
            OUR_DEBUG((LM_INFO, "[CConnectManager::PostMessage](%d)pCurrBuffPacket is NULL.\n", pConnectHandler->GetConnectID()));

            if(true == blDelete)
            {
                App_BuffPacketManager::instance()->Delete(pBuffPacket);
            }

            return false;
        }

        pCurrBuffPacket->WriteStream(pBuffPacket->GetData(), pBuffPacket->GetPacketLen());

        //���뷢�Ͷ���
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

    for(auto* pConnectHandler : vecConnectHandler)
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

_CommandData* CConnectManager::GetCommandData( uint16 u2CommandID )
{
    return m_CommandAccount.GetCommandData(u2CommandID);
}

void CConnectManager::Init( uint16 u2Index )
{
    //��ʼ�������Ĳ���
    Tcp_Common_Manager_Init(u2Index, m_CommandAccount, m_u2SendQueueMax, m_SendCacheManager);

    //��ʼ��Hash��
    uint32 u4PoolSize = GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount;
    m_objHashConnectList.Init(u4PoolSize);

    //��ʼ��ʱ������
    m_TimeWheelLink.Init(GetXmlConfigAttribute(xmlClientInfo)->MaxConnectTime,
                         GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime,
                         (int)u4PoolSize,
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
            //����������
            if (false == SendMessage(msg->m_u4ConnectID, msg->m_pBuffPacket, msg->m_u2CommandID, msg->m_u1SendState, msg->m_nEvents, msg->m_tvSend, msg->m_blDelete, msg->m_nMessageID))
            {
                OUR_DEBUG((LM_INFO, "[CConnectManager::Dispose_Queue]SendMessage error.\n"));
				//���շ��ͻ���
				App_BuffPacketManager::instance()->Delete(msg->m_pBuffPacket);
            }
        }
        else
        {
            //�������ӷ����������ر�
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
}

void CConnectHandlerPool::Init(int nObjcetCount)
{
    Close();

    //��ʼ��HashTable
    m_objHandlerList.Init((uint32)nObjcetCount);
    m_objHashHandleList.Init((uint32)nObjcetCount);

    for(uint16 i = 0; i < (uint16)nObjcetCount; i++)
    {
        CConnectHandler* pHandler = m_objHandlerList.GetObject(i);

        if(NULL != pHandler)
        {
            //��ID��Handlerָ��Ĺ�ϵ����hashTable
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
    OUR_DEBUG((LM_INFO, "[CConnectHandlerPool::Close]Begin.\n"));
    //���������Ѵ��ڵ�ָ��
    m_u4CurrMaxCount  = 1;

    //ɾ��hash��ռ�
    m_objHashHandleList.Close();

    //�����ڴ�
    for (uint32 i = 0; i < m_objHandlerList.GetCount(); i++)
    {
        m_objHandlerList.GetObject(i)->CloseFinally();
    }

    m_objHashHandleList.Close();

    OUR_DEBUG((LM_INFO, "[CConnectHandlerPool::Close]End.\n"));
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

    //��Hash���е���һ����ʹ�õ�����
    CConnectHandler* pHandler = m_objHashHandleList.Pop();

    //û�ҵ������
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
}

void CConnectManagerGroup::Close()
{
    if(NULL != m_objConnnectManagerList)
    {
        for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
        {
            CConnectManager* pConnectManager = m_objConnnectManagerList[i];
            pConnectManager->CloseAll();

            ACE_Time_Value tvSleep(0, 100000);
            ACE_OS::sleep(tvSleep);

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

        //��ʼ��ͳ����
        pConnectManager->Init((uint16)i);
        //��������
        m_objConnnectManagerList[i] = pConnectManager;
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::Init]Creat %d SendQueue OK.\n", i));
    }

    m_u2ThreadQueueCount = u2SendQueueCount;
}

uint32 CConnectManagerGroup::GetGroupIndex()
{
    //�������ӻ�����У��������������㷨��
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    m_u4CurrMaxCount++;
    return m_u4CurrMaxCount;
}

bool CConnectManagerGroup::AddConnect(CConnectHandler* pConnectHandler)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

    uint32 u4ConnectID = GetGroupIndex();

    //�ж����е���һ���߳�������ȥ
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

    //�ж����е���һ���߳�������ȥ
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

    //�ж����е���һ���߳�������ȥ
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
    if (0 >= pBuffPacket->GetPacketLen())
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage]send data len is zero.\n"));
        return false;
    }

    //�ж����е���һ���߳�������ȥ
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
    if (0 == nDataLen)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage]send data len is zero.\n"));
        return false;
    }

    //�ж����е���һ���߳�������ȥ
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
    if (0 >= pBuffPacket->GetPacketLen())
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage]send data len is zero.\n"));
        return false;
    }

    for(const auto& u4ConnectID : vecConnectID)
    {
        //�ж����е���һ���߳�������ȥ
        uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

        CConnectManager* pConnectManager =  m_objConnnectManagerList[u2ThreadIndex];

        if(NULL == pConnectManager)
        {
            continue;
        }

        //Ϊÿһ��Connect���÷��Ͷ������ݰ�
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
    if (0 == nDataLen)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage]send data len is zero.\n"));
        return false;
    }

    for(const auto& u4ConnectID : vecConnectID)
    {
        //�ж����е���һ���߳�������ȥ
        uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

        CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

        if(NULL == pConnectManager)
        {
            OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessage]No find send Queue object.\n"));
            continue;
        }

        //Ϊÿһ��Connect���÷��Ͷ������ݰ�
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
    //�ж����е���һ���߳�������ȥ
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::CloseConnect]No find send Queue object.\n"));
        return false;
    }

    //ͨ����Ϣ����ȥʵ�ֹر�
    return pConnectManager->CloseConnect_By_Queue(u4ConnectID);
}

bool CConnectManagerGroup::CloseConnectByClient(uint32 u4ConnectID) const
{
    //�ж����е���һ���߳�������ȥ
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
    //�ж����е���һ���߳�������ȥ
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
    //�ж����е���һ���߳�������ȥ
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::GetLocalIPInfo]No find send Queue object.\n"));
        return objClientIPInfo;
    }

    return pConnectManager->GetLocalIPInfo(u4ConnectID);
}


void CConnectManagerGroup::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo) const
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
    Close();
}

bool CConnectManagerGroup::StartTimer() const
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

bool CConnectManagerGroup::Close(uint32 u4ConnectID) const
{
    //�ж����е���һ���߳�������ȥ
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->Close(u4ConnectID);
}

bool CConnectManagerGroup::CloseUnLock(uint32 u4ConnectID) const
{
    //�ж����е���һ���߳�������ȥ
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
        return false;
    }

    return pConnectManager->CloseUnLock(u4ConnectID);
}

const char* CConnectManagerGroup::GetError() const
{
    return NULL;
}

void CConnectManagerGroup::SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost) const
{
    //�ж����е���һ���߳�������ȥ
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
        return;
    }

    pConnectManager->SetRecvQueueTimeCost(u4ConnectID, u4TimeCost);
}

uint16 CConnectManagerGroup::GetConnectCheckTime()
{
    return GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime;
}

bool CConnectManagerGroup::PostMessageAll(IBuffPacket*& pBuffPacket, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    if (0 >= pBuffPacket->GetPacketLen())
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessageAll]send data len is zero.\n"));
        return false;
    }

    //ȫ��Ⱥ��
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

    //�����˾�ɾ��
    if(true == blDelete)
    {
        App_BuffPacketManager::instance()->Delete(pBuffPacket);
    }

    return true;
}

bool CConnectManagerGroup::PostMessageAll( char*& pData, uint32 nDataLen, uint8 u1SendType, uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID)
{
    if (0 == nDataLen)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::PostMessageAll]send data len is zero.\n"));
        return false;
    }

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

    //ȫ��Ⱥ��
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

    //�����˾�ɾ��
    if(true == blDelete)
    {
        SAFE_DELETE_ARRAY(pData);
    }

    return true;
}

bool CConnectManagerGroup::SetConnectName(uint32 u4ConnectID, const char* pName)
{
    //�ж����е���һ���߳�������ȥ
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
    //�ж����е���һ���߳�������ȥ
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

    //ȫ������
    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
        CConnectManager* pConnectManager = m_objConnnectManagerList[i];

        if(NULL != pConnectManager)
        {
            pConnectManager->GetClientNameInfo(pName, objClientNameInfo);
        }
    }
}

void CConnectManagerGroup::GetCommandData( uint16 u2CommandID, _CommandData& objCommandData ) const
{
	for (uint16 i = 0; i < m_u2ThreadQueueCount; i++)
	{
		CConnectManager* pConnectManager = m_objConnnectManagerList[i];

		if (NULL != pConnectManager)
		{
			const _CommandData* pCommandData = pConnectManager->GetCommandData(u2CommandID);

			if (pCommandData != NULL)
			{
				objCommandData += (*pCommandData);
			}
		}
	}
}

void CConnectManagerGroup::GetFlowInfo(uint32& u4UdpFlowIn, uint32& u4UdpFlowOut) const
{
	uint32 u4ConnectFlowIn = 0;
	uint32 u4ConnectFlowOut = 0;

    for(uint16 i = 0; i < m_u2ThreadQueueCount; i++)
    {
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
    //�ж����е���һ���߳�������
    uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

    CConnectManager* pConnectManager = m_objConnnectManagerList[u2ThreadIndex];

    if(NULL == pConnectManager)
    {
        OUR_DEBUG((LM_INFO, "[CConnectManagerGroup::GetConnectState]No find send Queue object.\n"));
        return CLIENT_CONNECT_NO_EXIST;
    }

    return pConnectManager->GetConnectState(u4ConnectID);
}

int CConnectManagerGroup::handle_write_file_stream(uint32 u4ConnectID, const char* pData, uint32 u4Size, uint8 u1ParseID) const
{
    //�ж����е���һ���߳�������
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
