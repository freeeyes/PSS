#include "ConnectHandler.h"

void CConnectHandler::Close(uint32 u4ConnectID)
{
    //�������ӶϿ���Ϣ
    m_pPacketParseInfo->DisConnect(u4ConnectID);

    
	OUR_DEBUG((LM_ERROR, "[CProConnectHandler::Close](0x%08x)Close(ConnectID=%d), Recv=%d,Send=%d OK.\n",
		this,
		GetConnectID(),
		m_u4AllRecvSize,
		m_u4AllSendSize));

    shutdown();

    //�������PacketParse����
    ClearPacketParse();

    //����ת���ӿ�
    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->DisConnectRegedit(m_addrRemote.get_host_addr(), 
            m_addrRemote.get_port_number(), 
            ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_CLINET);
        m_strDeviceName = "";
    }

    //�ع��ù���ָ��
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

    //��ý�����ָ��
    m_pPacketParseInfo = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID);
    if (nullptr == m_pPacketParseInfo)
    {
        //��������������ڣ���ֱ�ӶϿ�����
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open](%s)can't find PacketParseInfo.\n", m_addrRemote.get_host_addr()));
        return -1;
    }

    //���Զ�����ӵ�ַ�Ͷ˿�
    if(this->peer().get_remote_addr(m_addrRemote) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]this->peer().get_remote_addr error.\n"));
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
        string strLog = fmt::format("[CConnectHandler::open]IP is more than IP Max({0}).",
            m_addrRemote.get_host_addr());

        AppLogManager::instance()->WriteToMail_r(LOG_SYSTEM_CONNECT,
            GetXmlConfigAttribute(xmlAlertConnect)->MailID,
            "Alert IP",
            strLog);

        return -1;
    }

    int nRet = 0;

    //��������Ϊ������ģʽ
    if (this->peer().enable(ACE_NONBLOCK) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]this->peer().enable  = ACE_NONBLOCK error.\n"));
        return -1;
    }

    //�鿴�Ƿ����ת������
    m_strDeviceName = App_ForwardManager::instance()->ConnectRegedit(m_addrRemote.get_host_addr(),
        m_addrRemote.get_port_number(),
        ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_CLINET,
        dynamic_cast<IDeviceHandler*>(this));

    //��ʼ����������
    if (-1 == Init_Open_Connect())
    {
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
        return -1;
    }

    return Dispose_Recv_Data();

}

int CConnectHandler::handle_output(ACE_HANDLE fd /*= ACE_INVALID_HANDLE*/)
{
    if (fd == ACE_INVALID_HANDLE)
    {
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_output]fd == ACE_INVALID_HANDLE.\n"));
        return -1;
    }

    return 0;
}

int CConnectHandler::Dispose_Recv_Data()
{
    ACE_Time_Value nowait(0, MAX_QUEUE_TIMEOUT);

    //����Ӧ�ý��յ����ݳ���
    uint32 u4CurrCount = Get_Recv_length();

    //������Ҫ��m_u4CurrSize���м�顣
    if (u4CurrCount == 0)
    {
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData][%d] nCurrCount < 0 m_u4CurrSize = %d.\n", GetConnectID(), m_u4CurrSize));

		//����PacketParse
		ClearPacketParse();

        return -1;
    }

    auto nDataLen = (int)this->peer().recv(m_pBlockRecv->wr_ptr(), u4CurrCount, MSG_NOSIGNAL, &nowait);

    if (nDataLen <= 0)
    {
        m_u4CurrSize = 0;
        auto u4Error = (uint32)errno;

        //�����-1 ��Ϊ11�Ĵ��󣬺���֮
        if (nDataLen == -1 && u4Error == EAGAIN)
        {
            return 0;
        }

        OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData] ConnectID=%d, recv data is error nDataLen = [%d] errno = [%d].\n", GetConnectID(), nDataLen, u4Error));

        return -1;
    }

    m_u4CurrSize += nDataLen;

    m_pBlockRecv->wr_ptr(nDataLen);

    //�����DEBUG״̬����¼��ǰ���ܰ��Ķ���������
    Output_Debug_Data(m_pBlockRecv, LOG_SYSTEM_DEBUG_CLIENTRECV, m_addrRemote);

    //�鿴�Ƿ���Ҫת��
    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->SendData(m_strDeviceName, m_pBlockRecv);
        m_pBlockRecv->reset();
        return 0;
    }

    //���������а�
	if (false == Dispose_Recv_buffer())
	{
		//����PacketParse
		ClearPacketParse();

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
    m_atvInput            = m_atvConnect;
    m_atvOutput           = m_atvConnect;

    m_u4AllRecvSize       = 0;
    m_u4AllSendSize       = 0;
    m_u4CurrSize          = 0;

    if (m_u2TcpNodelay == TCP_NODELAY_OFF)
    {
        //��������˽���Nagle�㷨��������Ҫ���á�
        int nOpt = 1;
        ACE_OS::setsockopt(this->get_handle(), IPPROTO_TCP, TCP_NODELAY, (char*)&nOpt, sizeof(int));
    }

    return 0;
}

//�ر�����
int CConnectHandler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
    if(h == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_DEBUG,"[CConnectHandler::handle_close] h is nullptr mask=%d.\n", (int)mask));
    }

    //���÷�����Ϣ���в����ٷ����κ���Ϣ
    if(m_u1ConnectState != CONNECTSTATE::CONNECT_SERVER_CLOSE)
    {
        m_u1ConnectState = CONNECTSTATE::CONNECT_CLIENT_CLOSE;
    }

	//�������ӹر���Ϣ
	Send_Hander_Event(PACKET_CDISCONNECT);

    return 0;
}

bool CConnectHandler::Device_Send_Data(const char* pData, ssize_t nLen)
{
    uint16 u2CommandID = 0x0000;

    IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);
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
    //�Ȳ�ʵ��
    ACE_UNUSED_ARG(pFileTest);
    return 0;
}

int CConnectHandler::handle_write_file_stream(const char* pData, uint32 u4Size, uint8 u1ParseID) const
{
    //�ݲ�ʵ��
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

bool CConnectHandler::SendMessage(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize)
{
    //��������
    if (EM_IO_TYPE::NET_INPUT == m_emIOType)
    {
        //�������ֱ�ӷ������ݣ���ƴ�����ݰ�
        if (EM_Client_Send_Status::CLIENT_SEND_CACHE == objSendMessageInfo.emSendState)
        {
            return Send_Input_To_Cache(objSendMessageInfo, u4PacketSize);
        }
        else
        {
            //Ҫ�������ݵ��Զ�
            return Send_Input_To_TCP(objSendMessageInfo, u4PacketSize);
        }
    }
    else
    {
        //�ļ���ڣ�ֱ��д����־
        return Write_SendData_To_File(objSendMessageInfo.blDelete, objSendMessageInfo.pBuffPacket);
    }
}

bool CConnectHandler::SendTimeoutMessage() const
{
	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID     = GetConnectID();
	objMakePacket.m_pPacketParse    = nullptr;
	objMakePacket.m_u1Option        = PACKET_CHEK_TIMEOUT;
	objMakePacket.m_AddrRemote      = m_addrRemote;
	objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
    objMakePacket.m_AddrListen      = m_addrLocal;

    Send_MakePacket_Queue(objMakePacket);

    return true;
}

bool CConnectHandler::PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const ACE_Time_Value& tvSend)
{
	ACE_Message_Block* pmbSend = App_MessageBlockManager::instance()->Create(u4Size);
	memcpy_safe(pMbData->rd_ptr(),
		u4Size,
		pmbSend->rd_ptr(),
		u4Size);

	pmbSend->wr_ptr(u4Size);

    //�����DEBUG״̬����¼��ǰ���Ͱ��Ķ���������
    Output_Debug_Data(pmbSend, LOG_SYSTEM_DEBUG_CLIENTSEND, m_addrRemote);


    //���ͳ�ʱʱ������
    ACE_Time_Value  nowait(0, MAX_BUFF_1000 * MAX_BUFF_1000);

    //��������
    auto nSendPacketLen = (int)pmbSend->length();
    int nIsSendSize    = 0;

    //ѭ�����ͣ�ֱ�����ݷ�����ɡ�
    while(true)
    {
        if(nSendPacketLen <= 0)
        {
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, nCurrSendSize error is %d.\n", 
                u4ConnectID,
                nSendPacketLen));
            App_MessageBlockManager::instance()->Close(pmbSend);
            return false;
        }

        auto nDataLen = (int)this->peer().send(pmbSend->rd_ptr(), size_t(nSendPacketLen - nIsSendSize), &nowait);

        if(nDataLen <= 0)
        {
            int nErrno = errno;
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID = %d, error = %d.\n", u4ConnectID, nErrno));

            string strLog = fmt::format("WriteError [{0}:{1}] nErrno = {2}  result.bytes_transferred() = {3},",
                m_addrRemote.get_host_addr(), 
                m_addrRemote.get_port_number(), 
                nErrno,
                nIsSendSize);

            AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, strLog);

            //������Ϣ�ص�
            pmbSend->rd_ptr((size_t)0);
            ACE_Time_Value tvNow = ACE_OS::gettimeofday();
            App_MakePacket::instance()->PutSendErrorMessage(GetConnectID(), pmbSend, tvNow);

            OUR_DEBUG((LM_ERROR, "[CConnectHandler::PutSendPacket] ConnectID=%d send cancel.\n", GetConnectID()));

            return false;
        }
        else if(nDataLen >= nSendPacketLen - nIsSendSize)   //�����ݰ�ȫ��������ϣ���ա�
        {
            m_u4AllSendSize     += (uint32)pmbSend->length();
            m_atvOutput         = tvSend;

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
                objMakePacket.m_AddrListen      = m_addrLocal;

                Send_MakePacket_Queue(objMakePacket);

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
            m_atvOutput      = tvSend;
        }
    }
}

bool CConnectHandler::Dispose_Recv_buffer()
{
	bool blRet = true;

	//���������а�
	while (true)
	{
		if (m_pPacketParseInfo->m_u1PacketParseType == PACKET_WITHHEAD)
		{
			//����ǰ�ͷģʽ
			if (m_pBlockRecv->length() < m_pPacketParseInfo->m_u4OrgLength)
			{
				//û����ȫ��ͷ����������
				Move_Recv_buffer();
				blRet = true;
				break;
			}
			else
			{
				//�����ͷ
				ACE_Message_Block* pHead = App_MessageBlockManager::instance()->Create(m_pPacketParseInfo->m_u4OrgLength);
				memcpy_safe(m_pBlockRecv->rd_ptr(),
					m_pPacketParseInfo->m_u4OrgLength,
					pHead->wr_ptr(),
					m_pPacketParseInfo->m_u4OrgLength);
				pHead->wr_ptr(m_pPacketParseInfo->m_u4OrgLength);

				if (0 != Dispose_Paceket_Parse_Head(pHead))
				{
					blRet = false;
					break;
				}

				//�жϴ��������ݰ��峤���Ƿ����
				uint32 u4BodyLength = m_objPacketParse.GetPacketBodySrcLen();
				uint32 u4AllPacketLength = u4BodyLength + m_pPacketParseInfo->m_u4OrgLength;
				if (u4AllPacketLength <= (uint32)m_pBlockRecv->length())
				{
					ACE_Message_Block* pBody = App_MessageBlockManager::instance()->Create(u4BodyLength);
					memcpy_safe(m_pBlockRecv->rd_ptr() + m_pPacketParseInfo->m_u4OrgLength,
						u4BodyLength,
						pBody->wr_ptr(),
						u4BodyLength);
					pBody->wr_ptr(u4BodyLength);

					if (0 != Dispose_Paceket_Parse_Body(pBody, u4BodyLength))
					{
						blRet = false;
						break;
					}

					//���������һ�������İ�������ƫ��һ��
					m_pBlockRecv->rd_ptr(u4AllPacketLength);
				}
				else
				{
					//û������ȫ����������
                    //������յİ�ͷ�ṹ��Ļ���,��һ�λ�������֯��ͷ
                    pHead->release();

					Move_Recv_buffer();
					blRet = true;
					break;
				}
			}
		}
		else
		{
			//�������ģʽ
			Dispose_Paceket_Parse_Stream(m_pBlockRecv);
			m_pBlockRecv->reset();
		}
	}

	return blRet;
}

void CConnectHandler::Move_Recv_buffer()
{
	if (m_pBlockRecv->rd_ptr() != m_pBlockRecv->base() && m_pBlockRecv->length() > 0)
	{
		//�ƶ���ǰ��ȥ
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
	else if (m_pBlockRecv->length() == 0) //���ȫ��������ɲ�����
	{
		//ȫ������
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

	Send_MakePacket_Queue(objMakePacket);
}

void CConnectHandler::ConnectOpen()
{
	//����PacketParse����Ӧ����
	m_pPacketParseInfo->Connect(GetConnectID(), GetClientIPInfo(), GetLocalIPInfo());

	//�������ӽ�����Ϣ
	Send_Hander_Event(PACKET_CONNECT);

    OUR_DEBUG((LM_DEBUG, "[CConnectHandler::open]Open(%d) Connection from [%s:%d](0x%08x).\n", GetConnectID(), m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), this));

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
        OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData]SetPacketHead is false.\n"));

        return -1;
    }
    else
    {
        if (nullptr == objHeadInfo.m_pmbHead)
        {
            OUR_DEBUG((LM_ERROR, "[CConnectHandler::RecvData]ConnectID=%d, objHeadInfo.m_pmbHead is nullptr.\n", GetConnectID()));
        }

        m_objPacketParse.SetPacket_IsHandleHead(false);
        m_objPacketParse.SetPacket_Head_Curr_Length(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength);
        m_objPacketParse.SetPacket_Head_Message(objHeadInfo.m_pmbHead);
        m_objPacketParse.SetPacket_Head_Curr_Length(objHeadInfo.m_u4HeadCurrLen);
        m_objPacketParse.SetPacket_Body_Src_Length(objHeadInfo.m_u4BodySrcLen);
        m_objPacketParse.SetPacket_CommandID(objHeadInfo.m_u2PacketCommandID);
    }

    uint32 u4PacketBodyLen = m_objPacketParse.GetPacketBodySrcLen();
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
        //������ݰ����Ǵ���ģ���Ͽ�����
        m_u4CurrSize = 0;
        OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvData]SetPacketBody is false.\n"));

        return -1;
    }
    else
    {
        m_objPacketParse.SetPacket_Body_Message(obj_Body_Info.m_pmbBody);
        m_objPacketParse.SetPacket_Body_Curr_Length(obj_Body_Info.m_u4BodyCurrLen);

        if (obj_Body_Info.m_u2PacketCommandID > 0)
        {
            m_objPacketParse.SetPacket_CommandID(obj_Body_Info.m_u2PacketCommandID);
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
    if(m_objPacketParse.GetMessageBody() == nullptr)
    {
        m_u4AllRecvSize += (uint32)m_objPacketParse.GetMessageHead()->length();
    }
    else
    {
        m_u4AllRecvSize += (uint32)(m_objPacketParse.GetMessageHead()->length() + m_objPacketParse.GetMessageBody()->length());
    }

	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID     = GetConnectID();
	objMakePacket.m_pPacketParse    = &m_objPacketParse;
	objMakePacket.m_u1Option        = PACKET_PARSE;
	objMakePacket.m_AddrRemote      = m_addrRemote;
	objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
    objMakePacket.m_AddrListen      = m_addrLocal;
    objMakePacket.m_tvRecv          = m_atvInput;

    Send_MakePacket_Queue(objMakePacket);

    m_objPacketParse.Clear();

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

    //Linux�²��ü�⣬ֱ�ӷ���true
    return true;
}

void CConnectHandler::ClearPacketParse()
{
    m_pBlockRecv->reset();
}

bool CConnectHandler::Write_SendData_To_File(bool blDelete, IBuffPacket* pBuffPacket)
{
    //�ļ���ڣ�ֱ��д����־
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

bool CConnectHandler::Send_Input_To_Cache(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize)
{
    _Input_To_Cache_Param obj_Input_To_Cache_Param;
    obj_Input_To_Cache_Param.m_blDelete            = objSendMessageInfo.blDelete;
    obj_Input_To_Cache_Param.m_emSendType          = objSendMessageInfo.emSendType;
    obj_Input_To_Cache_Param.m_u2CommandID         = objSendMessageInfo.u2CommandID;
    obj_Input_To_Cache_Param.m_u4ConnectID         = m_u4ConnectID;
    obj_Input_To_Cache_Param.m_u4PacketParseInfoID = m_u4PacketParseInfoID;
    obj_Input_To_Cache_Param.m_u4SendMaxBuffSize   = m_u4SendMaxBuffSize;

    return Tcp_Common_Send_Input_To_Cache(obj_Input_To_Cache_Param,
        m_pBlockMessage,
        u4PacketSize,
        objSendMessageInfo.pBuffPacket);
}

bool CConnectHandler::Send_Input_To_TCP(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize)
{
    _Send_Packet_Param obj_Send_Packet_Param;
    obj_Send_Packet_Param.m_blDelete            = objSendMessageInfo.blDelete;
    obj_Send_Packet_Param.m_emSendType          = objSendMessageInfo.emSendType;
    obj_Send_Packet_Param.m_u2CommandID         = objSendMessageInfo.u2CommandID;
    obj_Send_Packet_Param.m_u4ConnectID         = GetConnectID();
    obj_Send_Packet_Param.m_u4PacketParseInfoID = m_u4PacketParseInfoID;
    obj_Send_Packet_Param.m_u4SendMaxBuffSize   = m_u4SendMaxBuffSize;

    //ƴװ����
    if (false == Tcp_Common_Make_Send_Packet(obj_Send_Packet_Param,
        objSendMessageInfo.pBuffPacket,
        m_pBlockMessage,
        u4PacketSize))
    {
        return false;
    }

	//�����ڴ�
	Recovery_Common_BuffPacket(objSendMessageInfo.blDelete, objSendMessageInfo.pBuffPacket);

    //ֱ�ӷ������ݣ����ڷŵ������������ѹ������д���̫��
	bool blRet = PutSendPacket(GetConnectID(),
        m_pBlockMessage, 
        (uint32)m_pBlockMessage->length(),
        objSendMessageInfo.tvSendBegin);

	if (true == blRet)
	{
		m_pBlockMessage->reset();
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CConnectHandler::handle_output]ConnectID=%d write is close.\n", GetConnectID()));
		return false;
	}

    return true;
}

int CConnectHandler::Dispose_Paceket_Parse_Stream_Single(ACE_Message_Block* pCurrMessage)
{
    uint8 n1Ret = Tcp_Common_Recv_Stream(GetConnectID(), pCurrMessage, &m_objPacketParse, m_u4PacketParseInfoID);

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
        //��������ʧ�ܣ�����ʧ��
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

    //��ʼ��HashTable
    m_objHandlerList.Init((uint32)nObjcetCount);
    m_objHashHandleList.Init((uint32)nObjcetCount);

    for(uint16 i = 0; i < (uint16)nObjcetCount; i++)
    {
        CConnectHandler* pHandler = m_objHandlerList.GetObject(i);

        if(nullptr != pHandler)
        {
            //��ID��Handlerָ��Ĺ�ϵ����hashTable

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
    OUR_DEBUG((LM_INFO, "[CConnectHandlerPool::Close]Begin.\n"));
    //���������Ѵ��ڵ�ָ��

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

	//�����µ�ConnectID
	pHandler->SetConnectID(App_ConnectCounter::instance()->CreateCounter());

    //û�ҵ������
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
        OUR_DEBUG((LM_INFO, "[CProConnectHandlerPool::Delete]szHandlerID=%s(0x%08x).\n", strHandlerID.c_str(), pObject));
    }

    return true;
}

