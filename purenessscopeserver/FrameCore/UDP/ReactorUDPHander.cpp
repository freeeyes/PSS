#include "ReactorUDPHander.h"

void CReactorUDPHander::CloseFinaly()
{
    OUR_DEBUG((LM_INFO, "[CReactorUDPHander::~CloseFinaly]Begin.\n"));
    if (nullptr != m_pBlockMessage)
    {
        m_pBlockMessage->release();
    }

    if (nullptr != m_pBlockRecv)
    {
        m_pBlockRecv->release();
    }

    ACE_Reactor_Mask close_mask = ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;
    reactor()->remove_handler(this, close_mask);
    m_skRemote.close();
    OUR_DEBUG((LM_INFO, "[CReactorUDPHander::~CloseFinaly]End.\n"));
}

int CReactorUDPHander::OpenAddress(const ACE_INET_Addr& AddrRemote, ACE_Reactor* pReactor)
{
    if (m_skRemote.open(AddrRemote) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorUDPHander::OpenAddress]Open error(%d).\n", errno));
        return -1;
    }

    reactor(pReactor);

	if (-1 == this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK))
	{
		OUR_DEBUG((LM_ERROR, "[CReactorUDPHander::OpenAddress] Addr is register_handler error(%d).\n", errno));
		return -1;
	}

    m_pPacketParse = std::make_shared<CPacketParse>();

    return Init_Open_Address(AddrRemote);
}

void CReactorUDPHander::Close(uint32 u4ConnectID)
{
    App_UDPConnectIDManager::instance()->DeleteConnectID(u4ConnectID);
}

ACE_HANDLE CReactorUDPHander::get_handle(void) const
{
    return m_skRemote.get_handle();
}

int CReactorUDPHander::handle_input(ACE_HANDLE fd)
{
    if(fd == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorUDPHander::handle_input]fd is ACE_INVALID_HANDLE.\n"));
        return -1;
    }

    ACE_INET_Addr addrRemote;
    auto nDataLen = (int)m_skRemote.recv(m_pBlockRecv->wr_ptr(), m_pBlockRecv->size(), addrRemote);

    if(nDataLen > 0)
    {
        //��õ�ǰConnectID
		bool blNew = false;
		uint32 u4ConnectID = App_UDPConnectIDManager::instance()->GetConnetID(addrRemote.get_host_addr(),
            addrRemote.get_port_number(),
			blNew);

		if (true == blNew)
		{
			//������µ����ӣ����������ӽ����¼�
            Send_Hander_Event(u4ConnectID, PACKET_CONNECT, addrRemote);
		}

        //������հ�������
        if (false == CheckMessage(u4ConnectID, m_pBlockRecv->rd_ptr(), (uint32)nDataLen, addrRemote))
        {
            OUR_DEBUG((LM_INFO, "[CReactorUDPHander::handle_input]CheckMessage fail.\n"));
        }

        m_pBlockRecv->reset();
    }

    return 0;
}

int CReactorUDPHander::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
    if(handle == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorUDPHander::handle_close]close_mask = %d.\n", (uint32)close_mask));
    }

    m_skRemote.close();
    return 0;
}

void CReactorUDPHander::SetPacketParseInfoID(uint32 u4PacketParseInfoID)
{
    m_u4PacketParseInfoID = u4PacketParseInfoID;
}

bool CReactorUDPHander::SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize)
{
    ACE_Message_Block* pMbData = nullptr;

	_ClientIPInfo objClientIPInfo = App_UDPConnectIDManager::instance()->GetConnectIP(objSendMessageInfo.u4ConnectID);
	if (objClientIPInfo.m_u2Port == 0)
	{
		//û���ҵ�Ҫ���͵Ķ˿ڣ����ڷ���
		OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::SendMessage]no find ConnectID=%d.\n", objSendMessageInfo.u4ConnectID));
		return false;
	}

	_Send_Message_Param obj_Send_Message_Param;
	obj_Send_Message_Param.m_u4PacketParseInfoID = m_u4PacketParseInfoID;
	obj_Send_Message_Param.m_blDlete             = objSendMessageInfo.blDelete;
	obj_Send_Message_Param.m_u2Port              = objClientIPInfo.m_u2Port;
	obj_Send_Message_Param.m_strClientIP         = objClientIPInfo.m_strClientIP;
	obj_Send_Message_Param.m_u2CommandID         = objSendMessageInfo.u2CommandID;
	obj_Send_Message_Param.m_emSendType          = objSendMessageInfo.emSendType;

    u4PacketSize = objSendMessageInfo.pBuffPacket->GetPacketLen();

	bool blState = Udp_Common_Send_Message(obj_Send_Message_Param,
		objSendMessageInfo.pBuffPacket,
		m_skRemote,
		m_pPacketParseInfo,
		m_pBlockMessage);

    if (false == blState)
    {
        //�ͷŷ�����
        App_MessageBlockManager::instance()->Close(pMbData);

        return false;
    }

	if (true == blState)
	{
        SaveSendInfo(objSendMessageInfo.pBuffPacket->GetPacketLen());
	}

    return true;
}

bool CReactorUDPHander::PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend)
{
	//��������
	_ClientIPInfo objClientIPInfo = App_UDPConnectIDManager::instance()->GetConnectIP(u4ConnectID);
	if (objClientIPInfo.m_u2Port == 0)
	{
		//û���ҵ�Ҫ���͵Ķ˿ڣ����ڷ���
		OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::PutSendPacket]no find ConnectID=%d.\n", u4ConnectID));
		return false;
	}

	ACE_INET_Addr AddrRemote;
	int nErr = AddrRemote.set(objClientIPInfo.m_u2Port, objClientIPInfo.m_strClientIP.c_str());

	if (nErr != 0)
	{
		OUR_DEBUG((LM_INFO, "[PutSendPacket]set_address error[%d].\n", errno));
		return false;
	}

	auto u4SendSize = (uint32)m_skRemote.send(pMbData->rd_ptr(), pMbData->length(), AddrRemote);

	if (u4SendSize != u4Size)
	{
		return false;
	}
	else
	{
        SaveSendInfo(u4SendSize);
        m_atvOutput = tvSend;
	}

	//������ɻ���
	App_MessageBlockManager::instance()->Close(pMbData);

	return true;
}

void CReactorUDPHander::SetIsLog(bool blIsLog)
{
    ACE_UNUSED_ARG(blIsLog);
}

bool CReactorUDPHander::Device_Send_Data(const char* pData, ssize_t nLen)
{
    OUR_DEBUG((LM_INFO, "[CReactorUDPHander::Device_Send_Data]0x%08x, size=%d", pData, nLen));
    return true;
}

_ClientConnectInfo CReactorUDPHander::GetClientConnectInfo() const
{
    _ClientConnectInfo ClientConnectInfo;
    ClientConnectInfo.m_blValid       = true;
    ClientConnectInfo.m_u4ConnectID   = 0;
    ClientConnectInfo.m_u4AliveTime   = 0;
    ClientConnectInfo.m_u4BeginTime   = (uint32)CTimeStamp::Get_Time_use_second(m_atvInput);
    ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
    ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
    ClientConnectInfo.m_u4RecvCount   = m_u4RecvPacketCount;
    ClientConnectInfo.m_u4SendCount   = m_u4SendPacketCount;
    return ClientConnectInfo;
}

bool CReactorUDPHander::CheckMessage(uint32 u4ConnectID, const char* pData, uint32 u4Len, const ACE_INET_Addr& addrRemote)
{
    m_atvInput = CTimeStamp::Get_Time_Stamp();

    if(m_pPacketParseInfo->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        m_pPacketParse->SetPacket_Head_Src_Length(m_pPacketParseInfo->m_u4OrgLength);

        if(u4Len < m_pPacketParse->GetPacketHeadSrcLen())
        {
            return false;
        }

        //�����������ݰ�ת��ΪPacketParse����
        ACE_Message_Block* pMBHead = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketHeadSrcLen());
        memcpy_safe(pData, m_pPacketParse->GetPacketHeadSrcLen(), pMBHead->wr_ptr(), m_pPacketParse->GetPacketHeadSrcLen());
        pMBHead->wr_ptr(m_pPacketParse->GetPacketHeadLen());

        bool blRet = Udp_Common_Recv_Head(u4ConnectID, pMBHead, m_pPacketParse, m_pPacketParseInfo, u4Len);

        if (false == blRet)
        {
            App_MessageBlockManager::instance()->Close(pMBHead);
            return false;
        }

        //�����������
        if(m_pPacketParse->GetPacketBodySrcLen() > 0)
        {
            const char* pBody = pData + m_pPacketParse->GetPacketHeadSrcLen();
            ACE_Message_Block* pMBBody = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketBodySrcLen());
            memcpy_safe(pBody, m_pPacketParse->GetPacketBodySrcLen(), pMBBody->wr_ptr(), m_pPacketParse->GetPacketBodySrcLen());
            pMBBody->wr_ptr(m_pPacketParse->GetPacketBodySrcLen());

            bool blStateBody = Udp_Common_Recv_Body(u4ConnectID, pMBBody, m_pPacketParse, m_pPacketParseInfo);

            if (false == blStateBody)
            {
                App_MessageBlockManager::instance()->Close(pMBHead);
                App_MessageBlockManager::instance()->Close(pMBBody);
                return false;
            }
        }
    }
    else
    {
        ACE_Message_Block* pMbData = App_MessageBlockManager::instance()->Create(u4Len);
        memcpy_safe(pData, u4Len, pMbData->wr_ptr(), u4Len);
        pMbData->wr_ptr(u4Len);

        //������������
        if (false == Udp_Common_Recv_Stream(u4ConnectID, pMbData, m_pPacketParse, m_pPacketParseInfo))
        {
            return false;
        }

        App_MessageBlockManager::instance()->Close(pMbData);
    }

	//�������ݰ�
	if (false == Udp_Common_Send_WorkThread(m_MakePacket, u4ConnectID, m_pPacketParse, addrRemote, m_addrLocal, m_atvInput))
	{
		return false;
	}

    m_u4RecvSize += u4Len;
    m_u4RecvPacketCount++;

    return true;
}

int CReactorUDPHander::Init_Open_Address(const ACE_INET_Addr& AddrRemote)
{
    m_addrLocal = AddrRemote;

	m_pBlockMessage = App_MessageBlockManager::instance()->Create(GetXmlConfigAttribute(xmlSendInfo)->MaxBlockSize);
	m_pBlockRecv    = App_MessageBlockManager::instance()->Create(GetXmlConfigAttribute(xmlRecvInfo)->RecvBuffSize);

    //���÷��ͳ�ʱʱ�䣨��ΪUDP����ͻ��˲����ڵĻ���sendto������һ��recv����
    //����������һ����ʱ���ø�recv�������޵���ȥ
    struct timeval timeout = { MAX_RECV_UDP_TIMEOUT, 0 };
    ACE_OS::setsockopt(m_skRemote.get_handle(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    //���PacketParse������
    m_pPacketParseInfo = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID);

    return 0;
}

void CReactorUDPHander::SaveSendInfo(uint32 u4Len)
{
    m_atvOutput = CTimeStamp::Get_Time_Stamp();
    m_u4SendSize += u4Len;
    m_u4SendPacketCount++;
}

void CReactorUDPHander::Send_Hander_Event(uint32 u4ConnandID, uint8 u1Option, const ACE_INET_Addr& addrRemote)
{
	_MakePacket objMakePacket;

	objMakePacket.m_u4ConnectID     = u4ConnandID;
	objMakePacket.m_pPacketParse    = nullptr;
	objMakePacket.m_u1Option        = u1Option;
	objMakePacket.m_AddrRemote      = addrRemote;
	objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
	objMakePacket.m_pHandler        = this;
	objMakePacket.m_tvRecv          = m_atvInput;
	objMakePacket.m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_UDP;
    objMakePacket.m_AddrListen      = m_addrLocal;

	Send_MakePacket_Queue(m_MakePacket, objMakePacket);
}

void CReactorUDPHander::GetFlowInfo(uint32& u4FlowIn, uint32& u4FlowOut) const
{
    u4FlowIn  = 0;
    u4FlowOut = 0;
}
