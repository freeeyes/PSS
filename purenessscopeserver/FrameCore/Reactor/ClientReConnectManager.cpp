#include "ClientReConnectManager.h"

CReactorClientInfo::CReactorClientInfo()
{
}

bool CReactorClientInfo::Init(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, CConnectClientConnector* pReactorConnect, IClientMessage* pClientMessage, ACE_Reactor* pReactor, uint32 u4PacketParseID)
{
    int nRet = 0;

    if (u1IPType == TYPE_IPV4)
    {
        nRet = m_AddrServer.set(u2Port, pIP);
    }
    else
    {
        nRet = m_AddrServer.set(u2Port, pIP, 1, PF_INET6);
    }

    if (-1 == nRet)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Init]adrClient(%s:%d) error.\n", pIP, u2Port));
        return false;
    }

    m_pClientMessage    = pClientMessage;
    m_pReactorConnect   = pReactorConnect;
    m_nServerID         = nServerID;
    m_pReactor          = pReactor;
    m_u4PacketParseID   = u4PacketParseID;
    return true;
}

bool CReactorClientInfo::Run(bool blIsReady, EM_Server_Connect_State emState)
{
    if (nullptr != m_pConnectClient)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Run]Connect is exist.\n"));
        return false;
    }

    if (nullptr == m_pReactorConnect)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Run]m_pAsynchConnect is nullptr.\n"));
        return false;
    }

    m_pConnectClient = new CConnectClient();

    if (nullptr == m_pConnectClient)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Run]pConnectClient new is nullptr(%d).\n", emState));
        return false;
    }

    //�������Ӷ������ò���
    m_pConnectClient->SetServerID(m_nServerID);
    m_pConnectClient->reactor(m_pReactor);

    IClientMessage* pClientMessage = App_ClientReConnectManager::instance()->GetClientMessage(m_nServerID);

    uint32 u4PacketParseID = App_ClientReConnectManager::instance()->GetPacketParseID(m_nServerID);

    if (u4PacketParseID > 0)
    {
        m_pConnectClient->SetPacketParseInfoID(u4PacketParseID);
    }
    else
    {
        m_pConnectClient->SetClientMessage(pClientMessage);
    }

    App_ClientReConnectManager::instance()->SetHandler(m_nServerID, m_pConnectClient);

    if (blIsReady == true 
        && EM_Server_Connect_State::SERVER_CONNECT_FIRST != m_emConnectState 
        && EM_Server_Connect_State::SERVER_CONNECT_RECONNECT != m_emConnectState)
    {
        if (true == m_blIsLocal)
        {
            //��Ҫָ������˿�
            if (m_pReactorConnect->connect(m_pConnectClient, m_AddrServer, ACE_Synch_Options::defaults, m_AddrLocal) == -1)
            {
                m_emConnectState = EM_Server_Connect_State::SERVER_CONNECT_FAIL;
                OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Run](%s:%d) connection fails(ServerID=%d) error(%d).\n", m_AddrServer.get_host_addr(), m_AddrServer.get_port_number(), m_nServerID, ACE_OS::last_error()));
                //��������ΪTrue��Ϊ�����Զ�����������
                return true;
            }
        }
        else
        {
            //����Ҫָ������˿�
            if (m_pReactorConnect->connect(m_pConnectClient, m_AddrServer) == -1)
            {
                m_emConnectState = EM_Server_Connect_State::SERVER_CONNECT_FAIL;
                OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Run](%s:%d) connection fails(ServerID=%d) error(%d).\n", m_AddrServer.get_host_addr(), m_AddrServer.get_port_number(), m_nServerID, ACE_OS::last_error()));
                //��������ΪTrue��Ϊ�����Զ�����������
                return true;
            }
            else
            {
                OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Run](%s:%d) connection ok(ServerID=%d) OK.\n", m_AddrServer.get_host_addr(), m_AddrServer.get_port_number(), m_nServerID));
            }
        }

        m_emConnectState = EM_Server_Connect_State::SERVER_CONNECT_OK;
        //֪ͨ�ϲ��Ѿ�����
        m_pClientMessage->ReConnect(m_nServerID);
    }

    return true;
}

bool CReactorClientInfo::SendData(ACE_Message_Block* pmblk)
{
    if (nullptr == m_pConnectClient)
    {
        //����������ڽ��������У��ȴ�5���룬���
        if(EM_Server_Connect_State::SERVER_CONNECT_FIRST == m_emConnectState 
            || EM_Server_Connect_State::SERVER_CONNECT_RECONNECT == m_emConnectState)
        {
            return false;
        }

        if (nullptr == m_pConnectClient)
        {
            //�������ӽ�������Ϣ
            Common_Send_ConnectError(pmblk, m_AddrServer, m_pClientMessage);

            if (EM_Server_Connect_State::SERVER_CONNECT_FIRST != m_emConnectState
                && EM_Server_Connect_State::SERVER_CONNECT_RECONNECT != m_emConnectState
                && false == Run(true, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
            {
                OUR_DEBUG((LM_INFO, "[CReactorClientInfo::SendData]Run error.\n"));
            }

            return false;
        }
    }

    if(true == m_pClientMessage->Need_Send_Format())
    {
        //�������ݷ�����װ
        ACE_Message_Block* pSend = nullptr;

        if (false == Common_Send_Data(pmblk, m_pClientMessage, pSend))
        {
            return false;
        }

        //��������
        return m_pConnectClient->SendData(pSend);
    }
    else
    {
        //��������
        return m_pConnectClient->SendData(pmblk);
    }
}

int CReactorClientInfo::GetServerID() const
{
    return m_nServerID;
}

bool CReactorClientInfo::Close()
{
    if (nullptr != m_pConnectClient)
    {

        SetConnectClient(nullptr);
    }

    m_blIsLocal = false;
    return true;
}

void CReactorClientInfo::SetConnectClient(CConnectClient* pConnectClient)
{
    m_pConnectClient = pConnectClient;
}

CConnectClient* CReactorClientInfo::GetConnectClient()
{
    return m_pConnectClient;
}

IClientMessage* CReactorClientInfo::GetClientMessage()
{
    return m_pClientMessage;
}

ACE_INET_Addr CReactorClientInfo::GetServerAddr() const
{
    return m_AddrServer;
}

EM_Server_Connect_State CReactorClientInfo::GetServerConnectState() const
{
    return m_emConnectState;
}

void CReactorClientInfo::SetServerConnectState(EM_Server_Connect_State objState)
{
    m_emConnectState = objState;
}

void CReactorClientInfo::SetPacketParseID(uint32 u4PacketParseID)
{
    m_u4PacketParseID = u4PacketParseID;
}

uint32 CReactorClientInfo::GetPacketParseID() const
{
    return m_u4PacketParseID;
}

void CReactorClientInfo::SetLocalAddr( const char* pIP, uint16 u2Port, uint8 u1IPType )
{
    int nRet = 0;

    if (u1IPType == TYPE_IPV4)
    {
        nRet = m_AddrLocal.set(u2Port, pIP);
    }
    else
    {
        nRet = m_AddrLocal.set(u2Port, pIP, 1, PF_INET6);
    }

    if (-1 == nRet)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::SetLocalAddr]AddrLocal(%s:%d) error.\n", pIP, u2Port));
        return;
    }

    m_blIsLocal = true;
}

CClientReConnectManager::CClientReConnectManager(void)
{
}

bool CClientReConnectManager::Init(ACE_Reactor* pReactor)
{
    if (-1 == m_ReactorConnect.open(pReactor))
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Init]open error(%d).", ACE_OS::last_error()));
        return false;
    }

    //��¼����ص��������
    m_u4MaxPoolCount = GetXmlConfigAttribute(xmlConnectServer)->Count;

    //��ʼ��Hash����(TCP)
    m_objClientTCPList.Init(m_u4MaxPoolCount);

    //��ʼ��Hash����(UDP)
    m_objClientUDPList.Init(m_u4MaxPoolCount);

    m_u4ConnectServerTimeout = GetXmlConfigAttribute(xmlConnectServer)->TimeInterval * 1000; //ת��Ϊ΢��

    if (m_u4ConnectServerTimeout == 0)
    {
        m_u4ConnectServerTimeout = RE_CONNECT_SERVER_TIMEOUT;
    }

    m_pReactor        = pReactor;

    m_emS2SRunState = S2S_Run_State_Run;

    //��֮ǰ�л����������Ϣ��������
    for(CS2SConnectGetRandyInfo f : m_GetReadyInfoList)
    {
        if (f.m_u2LocalPort == 0)
        {
            Connect(f.m_nServerID,
                    f.m_szServerIP,
                    f.m_u2ServerPort,
                    f.m_u1Type,
                    f.m_pClientMessage);
        }
        else
        {
            Connect(f.m_nServerID,
                    f.m_szServerIP,
                    f.m_u2ServerPort,
                    f.m_u1Type,
                    f.m_szLocalIP,
                    f.m_u2LocalPort,
                    f.m_u1LocalIPType,
                    f.m_pClientMessage);
        }
    }

    m_GetReadyInfoList.clear();

    return true;
}

bool CClientReConnectManager::Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CReactorClientInfo* pClientInfo = nullptr;

    if (S2S_Run_State_Init == m_emS2SRunState)
    {
        //�����Ӧ����û��ʼ������ӵ�һ���б��У��ȴ���Ӧ����ʼ�������
        CS2SConnectGetRandyInfo objS2SConnectGetRandyInfo;

        sprintf_safe(objS2SConnectGetRandyInfo.m_szServerIP, MAX_BUFF_100, "%s", pIP);
        objS2SConnectGetRandyInfo.m_nServerID      = nServerID;
        objS2SConnectGetRandyInfo.m_u2ServerPort   = u2Port;
        objS2SConnectGetRandyInfo.m_u1Type         = u1IPType;
        objS2SConnectGetRandyInfo.m_pClientMessage = pClientMessage;

        m_GetReadyInfoList.push_back(objS2SConnectGetRandyInfo);

        return true;
    }

    //���ӳ�ʼ������
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, nullptr, 0, u1IPType, pClientMessage, pClientInfo, 0))
    {
        return false;
    }

    //��ʼ����
    if (false == pClientInfo->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]Run Error.\n"));
        delete pClientInfo;
        pClientInfo = nullptr;

        if (false == Close(nServerID))
        {
            OUR_DEBUG((LM_INFO, "[CClientReConnectManager::Connect]Close Error"));
        }

        return false;
    }

    OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]nServerID =(%d) connect is OK.\n", nServerID));
    return true;
}

bool CClientReConnectManager::Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CReactorClientInfo* pClientInfo = nullptr;

    if (S2S_Run_State_Init == m_emS2SRunState)
    {
        //�����Ӧ����û��ʼ������ӵ�һ���б��У��ȴ���Ӧ����ʼ�������
        CS2SConnectGetRandyInfo objS2SConnectGetRandyInfo;

        objS2SConnectGetRandyInfo.m_nServerID      = nServerID;
        sprintf_safe(objS2SConnectGetRandyInfo.m_szServerIP, MAX_BUFF_100, "%s", pIP);
        objS2SConnectGetRandyInfo.m_u2ServerPort    = u2Port;
        objS2SConnectGetRandyInfo.m_u1Type         = u1IPType;
        sprintf_safe(objS2SConnectGetRandyInfo.m_szLocalIP, MAX_BUFF_100, "%s", pLocalIP);
        objS2SConnectGetRandyInfo.m_u2LocalPort     = u2LocalPort;
        objS2SConnectGetRandyInfo.m_u1LocalIPType  = u1LocalIPType;
        objS2SConnectGetRandyInfo.m_pClientMessage = pClientMessage;
        m_GetReadyInfoList.push_back(objS2SConnectGetRandyInfo);

        return true;
    }

    //���ӳ�ʼ������
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, pLocalIP, u2LocalPort, u1LocalIPType, pClientMessage, pClientInfo, 0))
    {
        return false;
    }

    //��ʼ����
    if (false == pClientInfo->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]Run Error.\n"));
        delete pClientInfo;
        pClientInfo = nullptr;

        if (false == Close(nServerID))
        {
            OUR_DEBUG((LM_INFO, "[CClientReConnectManager::Connect]Close Error.\n"));
        }

        return false;
    }

    OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]nServerID =(%d) connect is OK.\n", nServerID));
    return true;
}

bool CClientReConnectManager::ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, uint32 u4PacketParseID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CReactorClientInfo* pClientInfo = nullptr;

    //���ӳ�ʼ������
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, nullptr, 0, u1IPType, nullptr, pClientInfo, u4PacketParseID))
    {
        return false;
    }

    //��ʼ����
    if (false == pClientInfo->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::ConnectFrame]Run Error.\n"));
        delete pClientInfo;
        pClientInfo = nullptr;

        if (false == Close(nServerID))
        {
            OUR_DEBUG((LM_INFO, "[CClientReConnectManager::ConnectFrame]Close Error"));
        }

        return false;
    }

    OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::ConnectFrame]nServerID =(%d) connect is OK.\n", nServerID));
    return true;
}

bool CClientReConnectManager::ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, uint32 u4PacketParseID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CReactorClientInfo* pClientInfo = nullptr;

    //���ӳ�ʼ������
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, pLocalIP, u2LocalPort, u1LocalIPType, nullptr, pClientInfo, u4PacketParseID))
    {
        return false;
    }

    //��ʼ����
    if (false == pClientInfo->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::ConnectFrame]Run Error.\n"));
        delete pClientInfo;
        pClientInfo = nullptr;

        if (false == Close(nServerID))
        {
            OUR_DEBUG((LM_INFO, "[CClientReConnectManager::ConnectFrame]Close Error.\n"));
        }

        return false;
    }

    OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::ConnectFrame]nServerID =(%d) connect is OK.\n", nServerID));
    return true;
}

bool CClientReConnectManager::ConnectUDP(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, EM_UDP_TYPE emType, IClientUDPMessage* pClientUDPMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CReactorUDPClient* pReactorUDPClient = nullptr;

    //��ʼ�����Ӷ���
    if (false == ConnectUdpInit(nServerID, pReactorUDPClient))
    {
        return false;
    }

    //��ʼ�����ӵ�ַ
    ACE_INET_Addr AddrLocal;
    int nErr = 0;

    if (emType != EM_UDP_TYPE::UDP_BROADCAST)
    {
        if (u1IPType == TYPE_IPV4)
        {
            nErr = AddrLocal.set(u2Port, pIP);
        }
        else
        {
            nErr = AddrLocal.set(u2Port, pIP, 1, PF_INET6);
        }
    }
    else
    {
        //�����UDP�㲥
        AddrLocal.set(u2Port, (uint32)INADDR_ANY);
    }

    if (nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[CClientReConnectManager::ConnectUDP](%d)UDP set_address error[%d].\n", nServerID, errno));
        SAFE_DELETE(pReactorUDPClient);
        return false;
    }

    //��ʼ����
    if (0 != pReactorUDPClient->OpenAddress(AddrLocal, emType, App_ReactorManager::instance()->GetAce_Reactor(REACTOR_UDPDEFINE), pClientUDPMessage))
    {
        OUR_DEBUG((LM_INFO, "[CClientReConnectManager::ConnectUDP](%d)UDP OpenAddress error.\n", nServerID));
        SAFE_DELETE(pReactorUDPClient);
        return false;
    }

    return true;
}

bool CClientReConnectManager::SetHandler(int nServerID, CConnectClient* pConnectClient)
{
    if (nullptr == pConnectClient)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::SetHandler]pProConnectClient is nullptr.\n"));
        return false;
    }

    //������������
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr == pClientInfo)
    {
        //�����������Ѿ����ڣ�������ӵ��Ѿ����ڵĿͻ���Hash���������
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::SetHandler]nServerID =(%d) is not exist.\n", nServerID));
        return false;
    }

    pClientInfo->SetConnectClient(pConnectClient);
    return true;
}

bool CClientReConnectManager::Close(int nServerID)
{
    //�������Ϊ�������Ͽ�����ֻɾ��ProConnectClient��ָ��
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //������������
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]nServerID =(%d) pClientInfo is nullptr.\n", nServerID));
        return false;
    }

    //�ر����Ӷ���
    if (nullptr != pClientInfo->GetConnectClient())
    {
        pClientInfo->GetConnectClient()->ClientClose();
        SAFE_DELETE(pClientInfo);
    }

    //��Hash����ɾ����ǰ���ڵĶ���
    m_objClientTCPList.Del_Hash_Data(szServerID);

    return true;
}

bool CClientReConnectManager::CloseUDP(int nServerID)
{
    //�������Ϊ�������Ͽ�����ֻɾ��ProConnectClient��ָ��
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //������������
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorUDPClient* pClientInfo = m_objClientUDPList.Get_Hash_Box_Data(szServerID);

    if (nullptr == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::CloseUDP]nServerID =(%d) pClientInfo is nullptr.\n", nServerID));
        return false;
    }

    pClientInfo->Close();
    SAFE_DELETE(pClientInfo);
    //��hash����ɾ����ǰ���ڵĶ���
    m_objClientUDPList.Del_Hash_Data(szServerID);
    return true;
}

bool CClientReConnectManager::ConnectErrorClose(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //������������
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    const CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::ConnectErrorClose]nServerID =(%d) pClientInfo is nullptr.\n", nServerID));
        return false;
    }

    //��Hash����ɾ����ǰ���ڵĶ���

    SAFE_DELETE(pClientInfo);
    m_objClientTCPList.Del_Hash_Data(szServerID);
    return true;
}

IClientMessage* CClientReConnectManager::GetClientMessage(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //������������
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr != pClientInfo)
    {
        return pClientInfo->GetClientMessage();
    }

    return nullptr;
}

bool CClientReConnectManager::SendData(int nServerID, char*& pData, int nSize, bool blIsDelete)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //������������
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr == pClientInfo)
    {
        //�����������Ѿ����ڣ��򲻴����µ�����
        OUR_DEBUG((LM_ERROR, "[CProConnectManager::SendData]nServerID =(%d) is not exist.\n", nServerID));

        if (true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

    ACE_Message_Block* pmblk = App_MessageBlockManager::instance()->Create(nSize);

    if (nullptr == pmblk)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::SendData]nServerID =(%d) pmblk is nullptr.\n", nServerID));

        if (true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

    memcpy_safe(pData, (uint32)nSize, pmblk->wr_ptr(), (uint32)nSize);
    pmblk->wr_ptr(nSize);

    if (true == blIsDelete)
    {
        SAFE_DELETE_ARRAY(pData);
    }

    //��������
    return pClientInfo->SendData(pmblk);
}

bool CClientReConnectManager::SendDataUDP(int nServerID, const char* pIP, uint16 u2Port, char*& pMessage, uint32 u4Len, bool blIsDelete)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorUDPClient* pClientInfo = m_objClientUDPList.Get_Hash_Box_Data(szServerID);

    if (nullptr == pClientInfo)
    {
        //�����������Ѿ����ڣ��򲻴����µ�����
        OUR_DEBUG((LM_ERROR, "[CProConnectManager::Close]nServerID =(%d) is not exist.\n", nServerID));

        if (true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pMessage);
        }

        return false;
    }

    //��������
    bool blSendRet = pClientInfo->SendMessage(pMessage, u4Len, pIP, u2Port);

    if (true == blIsDelete)
    {
        SAFE_DELETE_ARRAY(pMessage);
    }

    return blSendRet;
}

bool CClientReConnectManager::StartConnectTask(int nIntervalTime)
{
    CancelConnectTask();
    m_nTaskID = (int)App_TimerManager::instance()->schedule(this, (void*)nullptr, ACE_OS::gettimeofday() + ACE_Time_Value(nIntervalTime), ACE_Time_Value(nIntervalTime));

    if (m_nTaskID == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectManager::StartConnectTask].StartConnectTask is fail, time is (%d).\n", nIntervalTime));
        return false;
    }

    m_blReactorFinish = true;
    return true;
}

void CClientReConnectManager::CancelConnectTask()
{
    if (m_nTaskID != -1)
    {
        //ɱ��֮ǰ�Ķ�ʱ�������¿����µĶ�ʱ��
        App_TimerManager::instance()->cancel(m_nTaskID);
        m_nTaskID = -1;
    }
}

void CClientReConnectManager::Close()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]Begin.\n"));
    //����ж�ʱ������ɾ����ʱ��
    CancelConnectTask();

    //�ر������Ѵ��ڵ�����
    vector<CReactorClientInfo*> vecReactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecReactorClientInfo);

    for(CReactorClientInfo* pClientInfo : vecReactorClientInfo)
    {
        if(nullptr != pClientInfo)
        {
            pClientInfo->GetConnectClient()->ClientClose();
            SAFE_DELETE(pClientInfo);
        }
    }

    m_objClientTCPList.Close();

    vector<CReactorUDPClient*> vecReactorUDPClient;
    m_objClientUDPList.Get_All_Used(vecReactorUDPClient);

    for(CReactorUDPClient* pClientInfo : vecReactorUDPClient)
    {
        if(nullptr != pClientInfo)
        {
            pClientInfo->Close();
            SAFE_DELETE(pClientInfo);
        }
    }

    m_objClientUDPList.Close();
    m_u4MaxPoolCount = 0;

    //�ȴ����Ե����Ӷ����Լ��رգ���Ϊ�����ڵ�ǰ�̹߳رգ���������Ҫ��һ�¡�
    ACE_Time_Value tvSleep(0, 10000);
    ACE_OS::sleep(tvSleep);

    OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]End.\n"));
}

bool CClientReConnectManager::ConnectTcpInit(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage, CReactorClientInfo*& pClientInfo, uint32 u4PacketParseID)
{
    //������������
    char szServerID[10] = { '\0' };
    sprintf_safe(szServerID, 10, "%d", nServerID);
    pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr != pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect](%d)pClientInfo is exist.\n", nServerID));
        return false;
    }

    //������Ѿ����ˣ�����������������
    if (m_objClientTCPList.Get_Used_Count() == m_objClientTCPList.Get_Count())
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) m_objClientTCPList is full.\n", nServerID));
        return false;
    }

    //��ʼ��������Ϣ
    pClientInfo = new CReactorClientInfo();

    if (nullptr == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]pClientInfo is nullptr.\n"));
        return false;
    }

    //�����Ч��pClientMessage
    if (GetXmlConfigAttribute(xmlConnectServer)->RunType == 1)
    {
        App_ServerMessageTask::instance()->AddClientMessage(pClientMessage);
    }

    if (false == pClientInfo->Init(nServerID, pIP, u2Port, u1IPType, &m_ReactorConnect, pClientMessage, m_pReactor, u4PacketParseID))
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]pClientInfo Init Error.\n"));
        SAFE_DELETE(pClientInfo);

        if (false == Close(nServerID))
        {
            OUR_DEBUG((LM_INFO, "[CClientReConnectManager::ConnectTcpInit]Close error.\n"));
        }

        return false;
    }

    //���ñ���IP�Ͷ˿�
    if (nullptr != pLocalIP && u2LocalPort > 0)
    {
        pClientInfo->SetLocalAddr(pLocalIP, u2LocalPort, u1LocalIPType);
    }

    //��ӽ�hash
    if (-1 == m_objClientTCPList.Add_Hash_Data(szServerID, pClientInfo))
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) add m_objClientTCPList is fail.\n", nServerID));
        SAFE_DELETE(pClientInfo);
        return false;
    }

    return true;
}

bool CClientReConnectManager::ConnectUdpInit(int nServerID, CReactorUDPClient*& pReactorUDPClient)
{
    //������������
    char szServerID[10] = { '\0' };
    sprintf_safe(szServerID, 10, "%d", nServerID);
    pReactorUDPClient = m_objClientUDPList.Get_Hash_Box_Data(szServerID);

    if (nullptr != pReactorUDPClient)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect](%d)pClientInfo is exist.\n", nServerID));
        return false;
    }

    //������Ѿ����ˣ�����������������
    if (m_objClientUDPList.Get_Used_Count() == m_objClientUDPList.Get_Count())
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::ConnectUdpInit]nServerID =(%d) <%d>m_objClientUDPList is full.\n", nServerID, m_objClientUDPList.Get_Count()));
        return false;
    }

    pReactorUDPClient = new CReactorUDPClient();

    if (nullptr == pReactorUDPClient)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::ConnectUDP]nServerID =(%d) pProactorUDPClient is nullptr.\n", nServerID));
        return false;
    }

    //�����Ѿ���������ӽ�hash
    if (-1 == m_objClientUDPList.Add_Hash_Data(szServerID, pReactorUDPClient))
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) add m_objClientTCPList is fail.\n", nServerID));
        SAFE_DELETE(pReactorUDPClient);
        return false;
    }

    return true;
}

int CClientReConnectManager::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    ACE_UNUSED_ARG(arg);
    ACE_UNUSED_ARG(tv);

    m_ThreadWritrLock.acquire();
    vector<CReactorClientInfo*> vecCReactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecCReactorClientInfo);
    m_ThreadWritrLock.release();

    for (CReactorClientInfo* pClientInfo : vecCReactorClientInfo)
    { 
        if (nullptr == pClientInfo)
        {
            continue;
        }

        if (nullptr == pClientInfo->GetConnectClient())
        {
            //������Ӳ����ڣ������½�������
            if (false == pClientInfo->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
            {
                OUR_DEBUG((LM_INFO, "[CClientReConnectManager::handle_timeout]Run error.\n"));
            }
        }
        else
        {
            //��鵱ǰ���ӣ��Ƿ��ѹ��������
            ACE_Time_Value tvNow = ACE_OS::gettimeofday();

            //������첽ģʽ������Ҫ��鴦���߳��Ƿ񱻹���
            if(GetXmlConfigAttribute(xmlConnectServer)->RunType == 1)
            {
                App_ServerMessageTask::instance()->CheckServerMessageThread(tvNow);
            }
        }
    }

    return 0;
}

void CClientReConnectManager::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    VecClientConnectInfo.clear();
    vector<CReactorClientInfo*> vecReactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecReactorClientInfo);

    for (CReactorClientInfo* pClientInfo : vecReactorClientInfo)
    {
        if (nullptr == pClientInfo)
        {
            continue;
        }

		if (nullptr != pClientInfo->GetConnectClient())
		{
			_ClientConnectInfo ClientConnectInfo = pClientInfo->GetConnectClient()->GetClientConnectInfo();
			ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
			VecClientConnectInfo.push_back(ClientConnectInfo);
		}
		else
		{
			_ClientConnectInfo ClientConnectInfo;
			ClientConnectInfo.m_blValid = false;
			ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
			VecClientConnectInfo.push_back(ClientConnectInfo);
		}
    }
}

void CClientReConnectManager::GetUDPConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
    VecClientConnectInfo.clear();
    vector<CReactorUDPClient*> vecReactorUDPClient;
    m_objClientUDPList.Get_All_Used(vecReactorUDPClient);

    for (const CReactorUDPClient* pClientInfo : vecReactorUDPClient)
    {
        if (nullptr == pClientInfo)
        {
            continue;
        }

        _ClientConnectInfo ClientConnectInfo = pClientInfo->GetClientConnectInfo();
        VecClientConnectInfo.push_back(ClientConnectInfo);
    }
}

bool CClientReConnectManager::CloseByClient(int nServerID)
{
    //�������ΪԶ�����ӶϿ�����ֻɾ��ProConnectClient��ָ��
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr != pClientInfo)
    {
        pClientInfo->SetConnectClient(nullptr);
        pClientInfo->SetServerConnectState(EM_Server_Connect_State::SERVER_CONNECT_FAIL);
    }

    return true;
}

EM_Server_Connect_State CClientReConnectManager::GetConnectState( int nServerID )
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    const CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr == pClientInfo)
    {
        //�����������Ѿ����ڣ��򲻴����µ�����
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::GetConnectState]nServerID =(%d) is not exist.\n", nServerID));
        return EM_Server_Connect_State::SERVER_CONNECT_FAIL;
    }

    return pClientInfo->GetServerConnectState();
}

uint32 CClientReConnectManager::GetPacketParseID(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = { '\0' };
    sprintf_safe(szServerID, 10, "%d", nServerID);

    const CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr != pClientInfo)
    {
        return pClientInfo->GetPacketParseID();
    }

    return 0;
}

bool CClientReConnectManager::ReConnect(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]nServerID =(%d) pClientInfo is nullptr.\n", nServerID));
        return false;
    }

    if (nullptr == pClientInfo->GetConnectClient())
    {
        //������Ӳ����ڣ������½�������
        if (false == pClientInfo->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
        {
            OUR_DEBUG((LM_INFO, "[CClientReConnectManager::Close]Run error.\n"));
        }

        return true;
    }
    else
    {
        return true;
    }
}

ACE_INET_Addr CClientReConnectManager::GetServerAddr(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    ACE_INET_Addr remote_addr;
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    const CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr == pClientInfo)
    {
        //���������Ӳ����ڣ��򲻴����µ�����
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]nServerID =(%d) is not exist.\n", nServerID));
        return remote_addr;
    }
    else
    {
        remote_addr = pClientInfo->GetServerAddr();
        return remote_addr;
    }
}

bool CClientReConnectManager::SetServerConnectState(int nServerID, EM_Server_Connect_State objState)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr == pClientInfo)
    {
        //���������Ӳ����ڣ��򲻴����µ�����
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]nServerID =(%d) is not exist.\n", nServerID));
        return false;
    }
    else
    {
        pClientInfo->SetServerConnectState(objState);
        return true;
    }
}

bool CClientReConnectManager::GetServerIPInfo( int nServerID, _ClientIPInfo& objServerIPInfo )
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    const CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr == pClientInfo)
    {
        //���������Ӳ����ڣ��򲻴����µ�����
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]nServerID =(%d) is not exist.\n", nServerID));
        return false;
    }
    else
    {
        ACE_INET_Addr remote_addr     = pClientInfo->GetServerAddr();
        objServerIPInfo.m_strClientIP = remote_addr.get_host_addr();
        objServerIPInfo.m_u2Port      = remote_addr.get_port_number();
        return true;
    }
}

bool CClientReConnectManager::DeleteIClientMessage(IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //���첽�ص���Ч�����д�pClientMessage����Ϊ��Ч
    App_ServerMessageTask::instance()->DelClientMessage(pClientMessage);

    //һһѰ����֮��Ӧ�������Լ������Ϣ��ɾ��֮
    vector<CReactorClientInfo*> vecReactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecReactorClientInfo);

    for(CReactorClientInfo* pClientInfo : vecReactorClientInfo)
    {
        if(nullptr != pClientInfo && pClientInfo->GetClientMessage() == pClientMessage)
        {
            //�ر����ӣ���ɾ������
            //�ر����Ӷ���
            if (nullptr != pClientInfo->GetConnectClient())
            {
                pClientInfo->GetConnectClient()->ClientClose();
            }

            char szServerID[10] = {'\0'};
            sprintf_safe(szServerID, 10, "%d", pClientInfo->GetServerID());

            SAFE_DELETE(pClientInfo);
            m_objClientTCPList.Del_Hash_Data(szServerID);
            return true;
        }
    }

    return true;
}
