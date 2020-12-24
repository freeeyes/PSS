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

    //�ӷ�Ӧ��ע���¼�
    if (m_nIOCount == 0)
    {
        shutdown();
        PSS_LOGGER_DEBUG("[CConnectClient::Close]Close({0}:{1}) OK.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number());

        //ɾ�����Ӷ���
        App_ClientReConnectManager::instance()->CloseByClient(m_nServerID);

        if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
        {
            //����packetParse�Ͽ���Ϣ
            _ClientIPInfo objClientIPInfo;
            _ClientIPInfo objLocalIPInfo;
            App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Connect(m_nServerID,
                    objClientIPInfo,
                    objLocalIPInfo);

            //���Ϳ����Ϣ
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

        //ת���ӿڹر�
        if ("" != m_strDeviceName)
        {
            App_ForwardManager::instance()->DisConnectRegedit(m_addrRemote.get_host_addr(), 
                m_addrRemote.get_port_number(), 
                ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_S2S);
            m_strDeviceName = "";
        }

        //�ع��ù���ָ��
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
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadLock);
    //��������Ϣ�Ͽ�����outputȥִ�У������Ͳ���Ҫͬ�������ˡ�
    ACE_Message_Block* pMbData = App_MessageBlockManager::instance()->Create(sizeof(int));

    if (nullptr == pMbData)
    {
        PSS_LOGGER_DEBUG("[CConnectClient::ClientClose] Connectid={0}, pMbData is nullptr.", GetServerID());
        return;
    }

    ACE_Message_Block::ACE_Message_Type objType = ACE_Message_Block::MB_STOP;
    pMbData->msg_type(objType);

    //���������رգ�����Ҫ��ص����ӶϿ���Ϣ
    SetClientMessage(nullptr);

    Close();
}

int CConnectClient::open(void* p)
{
    if (p != nullptr)
    {
        PSS_LOGGER_DEBUG("[CConnectClient::open]p is not nullptr.");
    }

    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadLock);

    msg_queue()->high_water_mark(MAX_MSG_MASK);
    msg_queue()->low_water_mark(MAX_MSG_MASK);

    //�������ļ���ȡ����
    m_u4MaxPacketSize  = GetXmlConfigAttribute(xmlRecvInfo)->RecvBuffSize;

    ACE_Time_Value nowait(MAX_MSG_PACKETTIMEOUT);
    m_nIOCount = 1;

    //��������Ϊ������ģʽ
    if (this->peer().enable(ACE_NONBLOCK) == -1)
    {
        PSS_LOGGER_DEBUG("[CConnectHandler::open]this->peer().enable  = ACE_NONBLOCK error.");
        m_strError = "[CConnectClient::open]this->peer().enable  = ACE_NONBLOCK error.";
        return -1;
    }

    //���Զ�����ӵ�ַ�Ͷ˿�
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
    //���뵱ǰ��MessageBlock
    m_pCurrMessage = App_MessageBlockManager::instance()->Create(GetXmlConfigAttribute(xmlConnectServer)->Recvbuff);

    if (m_pCurrMessage == nullptr)
    {
        PSS_LOGGER_DEBUG("[CConnectClient::RecvClinetPacket] pmb new is nullptr.");
        return -1;
    }

    m_u1ConnectState = CONNECTSTATE::CONNECT_OPEN;

    PSS_LOGGER_DEBUG("[CConnectClient::open] Connection from [{0}:{1}].", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number());

    if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
    {
        //����packetParse�Ͽ���Ϣ
        _ClientIPInfo objClientIPInfo;
        _ClientIPInfo objLocalIPInfo;
        App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Connect(m_nServerID,
                objClientIPInfo,
                objLocalIPInfo);

        //���Ϳ����Ϣ
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

    //�жϻ����Ƿ�Ϊnullptr
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

    //�鿴�Ƿ���Ҫת��
    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->SendData(m_strDeviceName, m_pCurrMessage);
    }

    //�����DEBUG״̬����¼��ǰ���ܰ��Ķ���������
    Output_Debug_Data(m_pCurrMessage, LOG_SYSTEM_DEBUG_SERVERRECV, m_addrRemote);

    m_pCurrMessage->wr_ptr(nDataLen);

    Dispose_Recv_Data(m_pCurrMessage);

    return 0;
}

int CConnectClient::SendMessageGroup(uint16 u2CommandID, ACE_Message_Block* pmblk) const
{
    //��֯����
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

                //������Ϣ����Ϣ���
                auto pPacketParse = std::make_shared<CPacketParse>();
                pPacketParse->SetPacket_Head_Message(obj_Packet_Info.m_pmbHead);
                pPacketParse->SetPacket_Body_Message(obj_Packet_Info.m_pmbBody);
                pPacketParse->SetPacket_CommandID(obj_Packet_Info.m_u2PacketCommandID);
                pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4HeadSrcLen);
                pPacketParse->SetPacket_Head_Curr_Length(obj_Packet_Info.m_u4HeadCurrLen);
                pPacketParse->SetPacket_Body_Src_Length(obj_Packet_Info.m_u4BodySrcLen);
                pPacketParse->SetPacket_Body_Curr_Length(obj_Packet_Info.m_u4BodyCurrLen);

                //���Ϳ����Ϣ
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
                // ���ݰ��������󣬶Ͽ�����
                Close();
                return 0;
            }
        }
    }
    else
    {
        //�������ݣ����ظ��߼��㣬�Լ������������������ж�
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
                //ֱ�ӵ��ò����ע��Ķ���������
                Recv_Common_Dispose_Client_Message(u2CommandID, pRecvFinish, objServerIPInfo, m_pClientMessage);
            }
            else
            {
                //�����ݷŻص���Ϣ�߳�
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
        //�����Ѿ�����ر����̣����ڽ��ܷ������ݡ�
        App_MessageBlockManager::instance()->Close(pmblk);
        return false;
    }

    if (nullptr == pmblk)
    {
        PSS_LOGGER_DEBUG("[CConnectClient::SendData] ConnectID = {0}, get_handle() == ACE_INVALID_HANDLE.", GetServerID());
        return false;
    }

    //�����DEBUG״̬����¼��ǰ���ܰ��Ķ���������
    Output_Debug_Data(pmblk, LOG_SYSTEM_DEBUG_CLIENTSEND, m_addrRemote);

    auto nSendLen = (int)pmblk->length();   //�������ݵ��ܳ���
    int nIsSendSize = 0;

    //ѭ�����ͣ�ֱ�����ݷ�����ɡ�
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
        else if (nDataLen + nIsSendSize >= nSendLen)  //�����ݰ�ȫ��������ϣ���ա�
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
        //�������ݴ����Ѿ���ʱ���������ӡ����
        PSS_LOGGER_DEBUG("[CConnectClient::GetTimeout]***({0})recv dispose is timeout({1})!***.", m_nServerID, tvIntval.sec());
        return false;
    }
    else
    {
        return true;
    }
}


