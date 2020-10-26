#include "ProConnectHandler.h"

CProConnectHandler::CProConnectHandler(void) : m_u2LocalPort(0), m_u4SendCheckTime(0)
{
    m_u4ConnectID          = 0;
    m_u4AllRecvSize        = 0;
    m_u4AllSendSize        = 0;
    m_u4HandlerID          = 0;
    m_u2MaxConnectTime     = 0;
    m_u4SendThresHold      = MAX_MSG_SNEDTHRESHOLD;
    m_u1ConnectState       = CONNECTSTATE::CONNECT_INIT;
    m_u1SendBuffState      = CONNECTSTATE::CONNECT_SENDNON;
    m_u4MaxPacketSize      = MAX_MSG_PACKETLENGTH;
    m_u4RecvQueueCount     = 0;
    m_pBlockMessage        = nullptr;
    m_u2RecvQueueTimeout   = MAX_QUEUE_TIMEOUT * 1000;  //Ŀǰ��Ϊ��¼����΢�룬����������Ӧ������1000��
    m_u2TcpNodelay         = TCP_NODELAY_ON;
    m_u4SendMaxBuffSize    = 5*1024;
    m_nHashID              = 0;
    m_szConnectName[0]     = '\0';
    m_blIsLog              = false;
    m_szLocalIP[0]         = '\0';
    m_u4PacketParseInfoID  = 0;
    m_u4PacketDebugSize    = 0;
    m_pPacketDebugData     = nullptr;
    m_emIOType             = EM_IO_TYPE::NET_INPUT;
    m_pFileTest            = nullptr;
}

CProConnectHandler::~CProConnectHandler(void)
{
    if (nullptr != m_pBlockMessage)
    {
        m_pBlockMessage->release();
    }

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

    m_u2RecvQueueTimeout = GetXmlConfigAttribute(xmlRecvInfo)->RecvQueueTimeout * 1000;

    if (m_u2RecvQueueTimeout == 0)
    {
        m_u2RecvQueueTimeout = MAX_QUEUE_TIMEOUT * 1000;
    }

    m_u4SendMaxBuffSize = GetXmlConfigAttribute(xmlSendInfo)->MaxBlockSize;

    m_pPacketDebugData = new char[GetXmlConfigAttribute(xmlServerType)->DebugSize];
    m_u4PacketDebugSize = GetXmlConfigAttribute(xmlServerType)->DebugSize / 5;

    m_pBlockMessage = new ACE_Message_Block(GetXmlConfigAttribute(xmlSendInfo)->MaxBlockSize);

    m_RecvCounter.init("RecvThread", 10000);
    m_SendCounter.init("SendThread", 10000);
}


uint32 CProConnectHandler::GetHandlerID()
{
    return m_u4HandlerID;
}

void CProConnectHandler::Close()
{
    //�������ӶϿ���Ϣ��֪ͨPacketParse�ӿ�
    App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->DisConnect(GetConnectID());

    //֪ͨ�߼��ӿڣ������Ѿ��Ͽ�
    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, SendSize = %d, RecvQueueCount=%d.",
                                            m_addrRemote.get_host_addr(),
                                            m_addrRemote.get_port_number(),
                                            m_u4AllRecvSize,
                                            m_u4AllSendSize,
                                            m_u4RecvQueueCount);

    m_Reader.cancel();
    m_Writer.cancel();

    ACE_OS::shutdown(this->handle(), SD_BOTH);

    OUR_DEBUG((LM_ERROR, "[CProConnectHandler::Close](0x%08x)Close(ConnectID=%d), Recv=%d,Send=%d OK.\n", 
        this, 
        GetConnectID(),
        m_u4AllRecvSize,
        m_u4AllSendSize));

    //����ת���ӿ�
    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->DisConnectRegedit(m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), ENUM_FORWARD_TCP_CLINET);
        m_strDeviceName = "";
    }

    //������ָ�����ճ���
    App_ProConnectHandlerPool::instance()->Delete(this);
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
    m_atvConnect = ACE_OS::gettimeofday();
    m_atvInput = ACE_OS::gettimeofday();
    m_atvOutput = ACE_OS::gettimeofday();
    m_atvSendAlive = ACE_OS::gettimeofday();

    m_u4AllRecvSize = 0;
    m_u4AllSendSize = 0;
    m_u4RecvQueueCount = 0;
    m_blIsLog = false;
    m_szConnectName[0] = '\0';
    m_emIOType = EM_IO_TYPE::FILE_INPUT;

    //��֯����
	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID     = GetConnectID();
	objMakePacket.m_pPacketParse    = nullptr;
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
    if (App_PacketParseLoader::instance()->GetPacketParseInfo(u1ParseID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        uint32 u4PacketHead = App_PacketParseLoader::instance()->GetPacketParseInfo(u1ParseID)->m_u4OrgLength;
        ACE_Message_Block* pMbHead = App_MessageBlockManager::instance()->Create(u4PacketHead);

        if (nullptr == pMbHead)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_write_file_stream](%d) pMbHead is nullptr.\n", GetConnectID()));
            return -1;
        }

        memcpy_safe((char*)pData, u4PacketHead, pMbHead->wr_ptr(), u4PacketHead);
        pMbHead->wr_ptr(u4PacketHead);

        //������Ϣͷ
        _Head_Info objHeadInfo;
        bool blStateHead = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Head_Info(GetConnectID(), pMbHead, App_MessageBlockManager::instance(), &objHeadInfo, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

        if (false == blStateHead)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_write_file_stream](%d) Parse_Packet_Head_Info is illegal.\n", GetConnectID()));
            ClearPacketParse(*pMbHead);
            return -1;
        }

        m_objPacketParse.SetPacket_IsHandleHead(false);
        m_objPacketParse.SetPacket_Head_Message(objHeadInfo.m_pmbHead);
        m_objPacketParse.SetPacket_Head_Curr_Length(objHeadInfo.m_u4HeadCurrLen);
        m_objPacketParse.SetPacket_Body_Src_Length(objHeadInfo.m_u4BodySrcLen);
        m_objPacketParse.SetPacket_CommandID(objHeadInfo.m_u2PacketCommandID);

        //������Ϣ��
        ACE_Message_Block* pMbBody = App_MessageBlockManager::instance()->Create(u4Size - u4PacketHead);

        if (nullptr == pMbBody)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_write_file_stream](%d) pMbHead is nullptr.\n", GetConnectID()));
            return -1;
        }

        memcpy_safe((char*)&pData[u4PacketHead], u4Size - u4PacketHead, pMbBody->wr_ptr(), u4Size - u4PacketHead);
        pMbBody->wr_ptr(u4Size - u4PacketHead);

        //�������ݰ���
        _Body_Info obj_Body_Info;
        bool blStateBody = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Body_Info(GetConnectID(),
                           pMbBody,
                           App_MessageBlockManager::instance(),
                           &obj_Body_Info,
                           EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

        if (false == blStateBody)
        {
            //������ݰ���Ƿ����Ͽ�����
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_write_file_stream]Parse_Packet_Body_Info is illegal.\n"));

            //����PacketParse
            ClearPacketParse(*pMbBody);
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
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_write_file_stream]CheckMessage is false.\n"));
            return -1;
        }
    }
    else
    {
        //��ģʽ�����ļ�����
        ACE_Message_Block* pMbStream = App_MessageBlockManager::instance()->Create(u4Size);

        if (nullptr == pMbStream)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectHandler::handle_write_file_stream](%d) pMbHead is nullptr.\n", GetConnectID()));
            return -1;
        }

        memcpy_safe((char*)pData, u4Size, pMbStream->wr_ptr(), u4Size);

        _Packet_Info obj_Packet_Info;
        uint8 n1Ret = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Stream(GetConnectID(), pMbStream, dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()), &obj_Packet_Info, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

        if (PACKET_GET_ENOUGH == n1Ret)
        {
            m_objPacketParse.SetPacket_Head_Message(obj_Packet_Info.m_pmbHead);
            m_objPacketParse.SetPacket_Body_Message(obj_Packet_Info.m_pmbBody);
            m_objPacketParse.SetPacket_CommandID(obj_Packet_Info.m_u2PacketCommandID);
            m_objPacketParse.SetPacket_Head_Src_Length(obj_Packet_Info.m_u4HeadSrcLen);
            m_objPacketParse.SetPacket_Head_Curr_Length(obj_Packet_Info.m_u4HeadCurrLen);
            m_objPacketParse.SetPacket_Body_Src_Length(obj_Packet_Info.m_u4BodySrcLen);
            m_objPacketParse.SetPacket_Body_Curr_Length(obj_Packet_Info.m_u4BodyCurrLen);

            //�Ѿ��������������ݰ����Ӹ������߳�ȥ����
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
    m_atvConnect      = ACE_OS::gettimeofday();
    m_atvInput        = m_atvConnect;
    m_atvOutput       = m_atvConnect;
    m_atvSendAlive    = m_atvConnect;

    m_u4AllRecvSize       = 0;
    m_u4AllSendSize       = 0;
    m_u4RecvQueueCount    = 0;
    m_blIsLog             = false;
    m_szConnectName[0]    = '\0';
    m_emIOType            = EM_IO_TYPE::NET_INPUT;

    m_pPacketParseInfo = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID);

    if (nullptr == m_pPacketParseInfo)
    {
        //��������������ڣ���ֱ�ӶϿ�����
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::open](%s)can't find PacketParseInfo.\n", m_addrRemote.get_host_addr()));
        Close();
        return;
    }

    if(App_ForbiddenIP::instance()->CheckIP(m_addrRemote.get_host_addr()) == false)
    {
        //�ڽ�ֹ�б��У����������
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::open]IP Forbidden(%s).\n", m_addrRemote.get_host_addr()));
        Close();
        return;
    }

    //��鵥λʱ�����Ӵ����Ƿ�ﵽ����
    if(false == App_IPAccount::instance()->AddIP((string)m_addrRemote.get_host_addr()))
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::open]IP(%s) connect frequently.\n", m_addrRemote.get_host_addr()));
        App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), GetXmlConfigAttribute(xmlIP)->Timeout);

        //���͸澯�ʼ�
        AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECT,
                GetXmlConfigAttribute(xmlClientData)->MailID,
                "Alert IP",
                "[CProConnectHandler::open] IP is more than IP Max,");

        Close();
        return;
    }

    if(m_u2TcpNodelay == TCP_NODELAY_OFF)
    {
        //��������˽���Nagle�㷨��������Ҫ���á�
        int nOpt = 1;
        ACE_OS::setsockopt(h, IPPROTO_TCP, TCP_NODELAY, (char* )&nOpt, sizeof(int));
    }

    this->handle(h);

    //Ĭ�ϱ�����IP��ַ
    SetConnectName(m_addrRemote.get_host_addr());

    if(this->m_Reader.open(*this, h, 0, proactor()) == -1 ||
       this->m_Writer.open(*this, h, 0, proactor()) == -1)
    {
        OUR_DEBUG((LM_DEBUG,"[CProConnectHandler::open] m_reader or m_reader == 0.\n"));
        Close();
        return;
    }

    //д��������־
    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Connection from [%s:%d] ConnectID=%d, GetHandlerID=%d.",
                                          m_addrRemote.get_host_addr(),
                                          m_addrRemote.get_port_number(),
                                          GetConnectID(),
                                          GetHandlerID());

    m_u1ConnectState = CONNECTSTATE::CONNECT_OPEN;

    //����PacketParse����Ӧ����
    App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Connect(GetConnectID(), GetClientIPInfo(), GetLocalIPInfo());

    //�������ӽ�����Ϣ
    Send_Hander_Event(PACKET_CONNECT);

    OUR_DEBUG((LM_DEBUG,"[CProConnectHandler::open]Open(%d) Connection from [%s:%d](0x%08x).\n", GetConnectID(), m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), this));

    //�鿴�Ƿ����ת������
    m_strDeviceName = App_ForwardManager::instance()->ConnectRegedit(m_addrRemote.get_host_addr(),
                      m_addrRemote.get_port_number(),
                      ENUM_FORWARD_TCP_CLINET,
                      dynamic_cast<IDeviceHandler*>(this));

    //��ý��ջ�������С
    Get_Recv_length();

    return;
}

void CProConnectHandler::handle_read_stream(const ACE_Asynch_Read_Stream::Result& result)
{
    ACE_Message_Block& mb = result.message_block();
    uint32 u4PacketLen = (uint32)result.bytes_transferred();
    int nTran = (int)result.bytes_transferred();

    if(!result.success() || result.bytes_transferred() == 0 || this->handle() == ACE_INVALID_HANDLE)
    {
        //���ӶϿ�
        //����PacketParse
        ClearPacketParse(mb);

        //�������ӹر���Ϣ
        Send_Hander_Event(PACKET_CDISCONNECT);

        return;
    }

    //�鿴�Ƿ���Ҫת��
    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->SendData(m_strDeviceName, &mb);
        mb.reset();
        return;
    }

    m_atvInput = ACE_OS::gettimeofday();

    Output_Debug_Data(&mb, LOG_SYSTEM_DEBUG_CLIENTRECV);

    if(m_pPacketParseInfo->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        if(result.bytes_transferred() < result.bytes_to_read())
        {
            //�̶���������
            OUR_DEBUG((LM_INFO, "[CProConnectHandler::handle_read_stream]bytes_to_read=%d,bytes_transferred=%d.\n",
                       result.bytes_to_read(),
                       result.bytes_transferred()));
            int nRead = (int)result.bytes_to_read() - (int)result.bytes_transferred();

            if(-1 == m_Reader.read(mb, nRead))
            {
                //����PacketParse
                ClearPacketParse(mb);

				//�������ӹر���Ϣ
				Send_Hander_Event(PACKET_CDISCONNECT);

                return;
            }

            return;
        }
        else if(mb.length() == m_pPacketParseInfo->m_u4OrgLength && m_objPacketParse.GetIsHandleHead())
        {
            //�������ݰ�ͷ
            if (0 != Dispose_Paceket_Parse_Head(&mb))
            {
                return;
            }
        }
        else
        {
            //�������ݰ���
            if (0 != Dispose_Paceket_Parse_Body(&mb))
            {
                return;
            }
        }
    }
    else
    {
        int nStream = Dispose_Paceket_Parse_Stream(&mb);

        //������������ݻ�����
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
        //����ʧ��
        int nErrno = errno;
        OUR_DEBUG ((LM_DEBUG,"[CProConnectHandler::handle_write_stream] Connectid=[%d] begin(%d)...\n",GetConnectID(), nErrno));

        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "WriteError [%s:%d] nErrno = %d  result.bytes_transferred() = %d, ",
                                              m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), nErrno,
                                              result.bytes_transferred());

        OUR_DEBUG((LM_DEBUG,"[CProConnectHandler::handle_write_stream] Connectid=[%d] finish ok...\n", GetConnectID()));
        m_atvOutput = ACE_OS::gettimeofday();

        //������Ϣ�ص�
        App_MakePacket::instance()->PutSendErrorMessage(GetConnectID(), &result.message_block(), m_atvOutput);


        return;
    }
    else
    {
        //���ͳɹ�
        m_atvOutput = ACE_OS::gettimeofday();
        m_u4AllSendSize += (uint32)result.bytes_to_write();

        m_SendCounter.counter();

        App_MessageBlockManager::instance()->Close(&result.message_block());

        return;
    }
}

CONNECTSTATE CProConnectHandler::GetConnectState()
{
    return m_u1ConnectState;
}

CONNECTSTATE CProConnectHandler::GetSendBuffState()
{
    return m_u1SendBuffState;
}

bool CProConnectHandler::SendMessage(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize)
{
    if(nullptr == objSendMessageInfo.pBuffPacket)
    {
        OUR_DEBUG((LM_DEBUG,"[CProConnectHandler::SendMessage] Connectid=[%d] pBuffPacket is nullptr.\n", GetConnectID()));
        return false;
    }

    if (EM_IO_TYPE::NET_INPUT == m_emIOType)
    {
        //�������ֱ�ӷ������ݣ���ƴ�����ݰ�
        if (objSendMessageInfo.emSendState == EM_Client_Send_Status::CLIENT_SEND_CACHE)
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

    return true;
}

bool CProConnectHandler::PutSendPacket(ACE_Message_Block* pMbData, uint32 u4Size, const ACE_Time_Value tvSend)
{
    ACE_Message_Block* pmbSend = App_MessageBlockManager::instance()->Create(u4Size);
    pmbSend->copy(pMbData->rd_ptr(), pMbData->length());

    //�����DEBUG״̬����¼��ǰ���Ͱ��Ķ���������
    Output_Debug_Data(pmbSend, LOG_SYSTEM_DEBUG_CLIENTSEND);

    //ͳ�Ʒ�������
    m_atvOutput = tvSend;

    //�첽���ͷ���
    if(0 != m_Writer.write(*pmbSend, pmbSend->length()))
    {

        OUR_DEBUG ((LM_ERROR, "[CProConnectHandler::PutSendPacket] Connectid=%d mb=%d m_writer.write error(%d)!\n", 
            GetConnectID(), 
            pMbData->length(), 
            errno));

        //�������ʧ�ܣ������ﷵ��ʧ�ܣ��ص���ҵ���߼�ȥ����
        ACE_Time_Value tvNow = tvSend;
        App_MakePacket::instance()->PutSendErrorMessage(GetConnectID(), pmbSend, tvNow);
        return false;
    }

    return true;
}

void CProConnectHandler::Send_Hander_Event(uint8 u1Option)
{
    _MakePacket objMakePacket;

    objMakePacket.m_u4ConnectID = GetConnectID();
    objMakePacket.m_pPacketParse = nullptr;
    objMakePacket.m_u1Option = u1Option;
    objMakePacket.m_AddrRemote = m_addrRemote;
    objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
    objMakePacket.m_pHandler = this;

    Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);
}

void CProConnectHandler::Get_Recv_length()
{
    if ("" != m_strDeviceName)
    {
        RecvClinetPacket(GetXmlConfigAttribute(xmlClientInfo)->MaxBuffRecv);
    }
    else if (App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        if (m_objPacketParse.GetIsHandleHead() == true)
        {
            if (false == RecvClinetPacket(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u4OrgLength))
            {
                OUR_DEBUG((LM_INFO, "[CProConnectHandler::open](%d)RecvClinetPacket error.\n", GetConnectID()));
            }
        }
        else
        {
            if (false == RecvClinetPacket(m_objPacketParse.GetPacketBodySrcLen()))
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
    //�����DEBUG״̬����¼��ǰ���ܰ��Ķ���������
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
            AppLogManager::instance()->WriteLog_i(nLogType, "[(%s)%s:%d]%s.(���ݰ�����ֻ��¼ǰ200�ֽ�)", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_pPacketDebugData);
        }
        else
        {
            AppLogManager::instance()->WriteLog_i(nLogType, "[(%s)%s:%d]%s.", m_szConnectName, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_pPacketDebugData);
        }
    }
}

int CProConnectHandler::Dispose_Paceket_Parse_Head(ACE_Message_Block* pmb)
{
    //�ж�ͷ�ĺϷ���
    _Head_Info objHeadInfo;

    bool blStateHead = m_pPacketParseInfo->Parse_Packet_Head_Info(GetConnectID(), pmb, App_MessageBlockManager::instance(), &objHeadInfo, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

    if (false == blStateHead)
    {
        //�����ͷ�ǷǷ��ģ��򷵻ش��󣬶Ͽ����ӡ�
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_read_stream]PacketHead is illegal.\n"));

        //����PacketParse
        ClearPacketParse(*pmb);

        //�رյ�ǰ����
        Send_Hander_Event(PACKET_CDISCONNECT);

        return -1;
    }
    else
    {
        if (nullptr == objHeadInfo.m_pmbHead)
        {
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::RecvData]ConnectID=%d, objHeadInfo.m_pmbHead is nullptr.\n", GetConnectID()));
        }

        m_objPacketParse.SetPacket_IsHandleHead(false);
        m_objPacketParse.SetPacket_Head_Src_Length(m_pPacketParseInfo->m_u4OrgLength);
        m_objPacketParse.SetPacket_Head_Message(objHeadInfo.m_pmbHead);
        m_objPacketParse.SetPacket_Head_Curr_Length(objHeadInfo.m_u4HeadCurrLen);
        m_objPacketParse.SetPacket_Body_Src_Length(objHeadInfo.m_u4BodySrcLen);
        m_objPacketParse.SetPacket_CommandID(objHeadInfo.m_u2PacketCommandID);
    }

    //�������ֻ�����ͷ������
    //�������ֻ�а�ͷ������Ҫ���壬�����������һЩ����������ֻ�����ͷ���ӵ�DoMessage()
    uint32 u4PacketBodyLen = m_objPacketParse.GetPacketBodySrcLen();

    if (u4PacketBodyLen == 0)
    {
        //���ֻ�а�ͷû�а��壬��ֱ�Ӷ����߼��ﴦ��
        if (false == CheckMessage())
        {
            Send_Hander_Event(PACKET_CDISCONNECT);
            return -1;
        }
    }
    else
    {
        //����������������ȣ�Ϊ�Ƿ�����
        if (u4PacketBodyLen >= m_u4MaxPacketSize)
        {
            OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_read_stream]u4PacketHeadLen(%d) more than %d.\n", u4PacketBodyLen, m_u4MaxPacketSize));

            //����PacketParse
            ClearPacketParse(*pmb);

			//�������ӶϿ��¼�
			Send_Hander_Event(PACKET_SDISCONNECT);

            return -1;
        }
    }

    return 0;
}

int CProConnectHandler::Dispose_Paceket_Parse_Body(ACE_Message_Block* pmb)
{
    //��������������ɣ���ʼ�����������ݰ�
    _Body_Info obj_Body_Info;
    bool blStateBody = m_pPacketParseInfo->Parse_Packet_Body_Info(GetConnectID(),
                       pmb,
                       App_MessageBlockManager::instance(),
                       &obj_Body_Info,
                       EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

    if (false == blStateBody)
    {
        //������ݰ���Ƿ����Ͽ�����
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::handle_read_stream]SetPacketBody is illegal.\n"));

        //����PacketParse
        ClearPacketParse(*pmb);

        //�رյ�ǰ����
		Send_Hander_Event(PACKET_SDISCONNECT);

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
        //�رյ�ǰ����
        Send_Hander_Event(PACKET_SDISCONNECT);

        return -1;
    }

    return 0;
}

int CProConnectHandler::Dispose_Paceket_Parse_Stream(ACE_Message_Block* pCurrMessage)
{
    //����ģʽ����
    while (true)
    {
        //������Ϣ
        uint8 n1Ret = Tcp_Common_Recv_Stream(GetConnectID(), pCurrMessage, &m_objPacketParse, m_u4PacketParseInfoID);

        if (PACKET_GET_NO_ENOUGH == n1Ret)
        {
            //���յ����ݲ���������Ҫ��������
            break;
        }
        else if (PACKET_GET_ENOUGH == n1Ret)
        {
            //�Ѿ��������������ݰ����Ӹ������߳�ȥ����
            if (false == CheckMessage())
            {
                App_MessageBlockManager::instance()->Close(pCurrMessage);

				//�رյ�ǰ����
				Send_Hander_Event(PACKET_SDISCONNECT);

                return -1;
            }

            //�����Ƿ���������
            if (pCurrMessage->length() == 0)
            {
                break;
            }
            else
            {
                //�������ݣ���������
                continue;
            }
        }
        else
        {
            //���ݰ�Ϊ���������������
            m_objPacketParse.Clear();

			//�رյ�ǰ����
			Send_Hander_Event(PACKET_SDISCONNECT);

            return -1;
        }
    }

    return 0;
}

bool CProConnectHandler::Write_SendData_To_File(bool blDelete, IBuffPacket* pBuffPacket)
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

bool CProConnectHandler::Send_Input_To_Cache(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize)
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

bool CProConnectHandler::Send_Input_To_TCP(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize)
{
    //���ж��Ƿ�Ҫ��װ��ͷ�������Ҫ������װ��m_pBlockMessage��
    _Send_Packet_Param obj_Send_Packet_Param;
    obj_Send_Packet_Param.m_blDelete            = objSendMessageInfo.blDelete;
    obj_Send_Packet_Param.m_emSendType          = objSendMessageInfo.emSendType;
    obj_Send_Packet_Param.m_u2CommandID         = objSendMessageInfo.u2CommandID;
    obj_Send_Packet_Param.m_u4ConnectID         = GetConnectID();
    obj_Send_Packet_Param.m_u4PacketParseInfoID = m_u4PacketParseInfoID;
    obj_Send_Packet_Param.m_u4SendMaxBuffSize   = m_u4SendMaxBuffSize;

    //ƴװ����
    bool blState = Tcp_Common_Make_Send_Packet(obj_Send_Packet_Param,
        objSendMessageInfo.pBuffPacket,
        m_pBlockMessage,
        u4PacketSize);

    if (false == blState)
    {
        return false;
    }

    Recovery_Common_BuffPacket(objSendMessageInfo.blDelete, objSendMessageInfo.pBuffPacket);

    //����ϢID����MessageBlock
    ACE_Message_Block::ACE_Message_Type objType = ACE_Message_Block::MB_USER + objSendMessageInfo.nMessageID;
    m_pBlockMessage->msg_type(objType);

    blState = PutSendPacket(m_pBlockMessage, (uint32)m_pBlockMessage->length(), objSendMessageInfo.tvSendBegin);
    if (true == blState)
    {
        m_pBlockMessage->reset();
    }

    return blState;
}

bool CProConnectHandler::RecvClinetPacket(uint32 u4PackeLen)
{
    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(u4PackeLen);

    if(m_Reader.read(*pmb, u4PackeLen) == -1)
    {
        //�����ʧ�ܣ���ر����ӡ�
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::RecvClinetPacket] m_reader.read is error(%d)(%d).\n", GetConnectID(), errno));

        ClearPacketParse(*pmb);

		//�رյ�ǰ����
		Send_Hander_Event(PACKET_SDISCONNECT);

        return false;
    }

    return true;
}

bool CProConnectHandler::CheckMessage()
{
    if(m_objPacketParse.GetMessageHead() != nullptr)
    {
        if(m_objPacketParse.GetMessageBody() == nullptr)
        {
            m_u4AllRecvSize += (uint32)m_objPacketParse.GetMessageHead()->length();
        }
        else
        {
            m_u4AllRecvSize += (uint32)m_objPacketParse.GetMessageHead()->length() + (uint32)m_objPacketParse.GetMessageBody()->length();
        }

        //��֯����
		_MakePacket objMakePacket;

		objMakePacket.m_u4ConnectID     = GetConnectID();
		objMakePacket.m_pPacketParse    = &m_objPacketParse;
		objMakePacket.m_u1Option        = PACKET_PARSE;
		objMakePacket.m_AddrRemote      = m_addrRemote;
		objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;

        Send_MakePacket_Queue(objMakePacket, m_szLocalIP, m_u2LocalPort);

        /*
        //���Դ���
        uint32 u4Size = (uint32)(m_objPacketParse.GetMessageHead()->length() + m_objPacketParse.GetMessageBody()->length());
        ACE_Message_Block* pSendmb = App_MessageBlockManager::instance()->Create(u4Size);

        pSendmb->copy(m_objPacketParse.GetMessageHead()->rd_ptr(), m_objPacketParse.GetMessageHead()->length());
        pSendmb->copy(m_objPacketParse.GetMessageBody()->rd_ptr(), m_objPacketParse.GetMessageBody()->length());

        App_MessageBlockManager::instance()->Close(m_objPacketParse.GetMessageHead());
        App_MessageBlockManager::instance()->Close(m_objPacketParse.GetMessageBody());

        PutSendPacket(pSendmb, pSendmb->length(), m_atvInput);

        App_MessageBlockManager::instance()->Close(pSendmb);
        */

        m_objPacketParse.Clear();

        m_RecvCounter.counter();
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectHandler::CheckMessage] ConnectID = %d, m_pPacketParse is nullptr.\n", GetConnectID()));
    }

    return true;
}

_ClientConnectInfo CProConnectHandler::GetClientInfo()
{
    _ClientConnectInfo_Param obj_ClientConnectInfo_Param;
    obj_ClientConnectInfo_Param.m_addrRemote = m_addrRemote;
    obj_ClientConnectInfo_Param.m_atvConnect = m_atvConnect;
    obj_ClientConnectInfo_Param.m_u4AllRecvSize = m_u4AllRecvSize;
    obj_ClientConnectInfo_Param.m_u4AllSendSize = m_u4AllSendSize;
    obj_ClientConnectInfo_Param.m_u4ConnectID = GetConnectID();
    obj_ClientConnectInfo_Param.m_u4RecvQueueCount = m_u4RecvQueueCount;

    return Tcp_Common_ClientInfo(obj_ClientConnectInfo_Param);
}

_ClientIPInfo CProConnectHandler::GetClientIPInfo()
{
    _ClientIPInfo ClientIPInfo;
    ClientIPInfo.m_strClientIP = m_addrRemote.get_host_addr();
    ClientIPInfo.m_u2Port = m_addrRemote.get_port_number();
    return ClientIPInfo;
}

_ClientIPInfo CProConnectHandler::GetLocalIPInfo()
{
    _ClientIPInfo ClientIPInfo;
    ClientIPInfo.m_strClientIP = (string)m_szLocalIP;
    ClientIPInfo.m_u2Port = m_u2LocalPort;
    return ClientIPInfo;
}

void CProConnectHandler::ClearPacketParse(ACE_Message_Block& mbCurrBlock)
{
	if (nullptr != &mbCurrBlock)
	{
		App_MessageBlockManager::instance()->Close(&mbCurrBlock);
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
	objMakePacket.m_pPacketParse    = nullptr;
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

    EM_SEND_PACKET_PARSE emState       = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    EM_Client_Send_Status  emSendState = EM_Client_Send_Status::CLIENT_SEND_IMMEDIATLY;
    uint32 u4PacketSize = 0;

    CSendMessageInfo objSendMessageInfo;

    objSendMessageInfo.u2CommandID = u2CommandID;
    objSendMessageInfo.pBuffPacket = pBuffPacket;
    objSendMessageInfo.emSendType  = emState;
    objSendMessageInfo.emSendState = emSendState;
    objSendMessageInfo.blDelete    = true;

    return SendMessage(objSendMessageInfo, u4PacketSize);
}

CProConnectHandlerPool::CProConnectHandlerPool(void)
{
}

CProConnectHandlerPool::~CProConnectHandlerPool(void)
{
	Close();
}

void CProConnectHandlerPool::Init(int nObjcetCount)
{
	Close();

	//��ʼ��HashTable
	m_objHandlerList.Init((uint32)nObjcetCount);
	m_objHashHandleList.Init((int)nObjcetCount);

	for (int i = 0; i < nObjcetCount; i++)
	{
		CProConnectHandler* pHandler = m_objHandlerList.GetObject(i);

		pHandler->Init(i);

		//��ID��Handlerָ��Ĺ�ϵ����hashTable
		char szHandlerID[10] = { '\0' };
		sprintf_safe(szHandlerID, 10, "%d", i);
		int nHashPos = m_objHashHandleList.Add_Hash_Data(szHandlerID, pHandler);

		if (-1 != nHashPos)
		{
			pHandler->Init(i);
			pHandler->SetHashID(i);
		}
	}
}

void CProConnectHandlerPool::Close()
{
	//ɾ��hash��ռ�
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

	//��Hash���е���һ����ʹ�õ�����
	CProConnectHandler* pHandler = m_objHashHandleList.Pop();

    //�����µ�ConnectID
    pHandler->SetConnectID(++m_u4CurrCount);

	//û�ҵ������
	return pHandler;
}

bool CProConnectHandlerPool::Delete(CProConnectHandler* pObject)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	if (NULL == pObject)
	{
		return false;
	}

	char szHandlerID[10] = { '\0' };
	sprintf_safe(szHandlerID, 10, "%d", pObject->GetHandlerID());
	//������Ϊ�ڴ��ǹ̶��ģ�ֱ��д��Hashԭ��λ��
	bool blState = m_objHashHandleList.Push(szHandlerID, pObject);

	if (false == blState)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectHandlerPool::Delete]szHandlerID=%s(0x%08x).\n", szHandlerID, pObject));
	}

	return true;
}
