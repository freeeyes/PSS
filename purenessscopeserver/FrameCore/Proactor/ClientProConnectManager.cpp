#include "ClientProConnectManager.h"

CProactorClientInfo::CProactorClientInfo()
{
    m_pProConnectClient = nullptr;
    m_pProAsynchConnect = nullptr;
    m_pClientMessage    = nullptr;
    m_nServerID         = 0;
    m_u4PacketParseID   = 0;
    m_emConnectState    = EM_Server_Connect_State::SERVER_CONNECT_READY;
    m_blIsLocal         = false;
}

CProactorClientInfo::~CProactorClientInfo()
{
}

bool CProactorClientInfo::Init(const char* pIP, uint16 u2Port, uint8 u1IPType, int nServerID, CProAsynchConnect* pProAsynchConnect, IClientMessage* pClientMessage, uint32 u4PacketParseID)
{
    OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Init]SetAddrServer(%s:%d) Begin.\n", pIP, u2Port));

    int nRet = 0;

    if(u1IPType == TYPE_IPV4)
    {
        nRet = m_AddrServer.set(u2Port, pIP);
    }
    else
    {
        nRet = m_AddrServer.set(u2Port, pIP, 1, PF_INET6);
    }

    if(-1 == nRet)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Init]nServerID = %d, adrClient(%s:%d) error.\n", nServerID, pIP, u2Port));
        return false;
    }

    m_pProAsynchConnect = pProAsynchConnect;
    m_pClientMessage    = pClientMessage;
    m_nServerID         = nServerID;
    m_u4PacketParseID   = u4PacketParseID;

    return true;
}

bool CProactorClientInfo::Run(bool blIsReadly, EM_Server_Connect_State emState)
{
    if(nullptr != m_pProConnectClient)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]Connect is exist.\n"));
        return false;
    }

    if(nullptr == m_pProAsynchConnect)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]m_pAsynchConnect is nullptr.\n"));
        return false;
    }

    if(true == blIsReadly 
        && EM_Server_Connect_State::SERVER_CONNECT_FIRST != m_emConnectState 
        && EM_Server_Connect_State::SERVER_CONNECT_RECONNECT != m_emConnectState)
    {
        m_pProAsynchConnect->SetConnectState(true);

        //����һ�����ݲ���������Զ��
        _ProConnectState_Info* pProConnectInfo = new _ProConnectState_Info();
        pProConnectInfo->m_nServerID = m_nServerID;

        m_emConnectState = emState;

        if (true == m_blIsLocal)
        {
            if (m_pProAsynchConnect->connect(m_AddrServer, (const ACE_INET_Addr&)m_AddrLocal, 1, (const void*)pProConnectInfo) == -1)
            {
                OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]m_pAsynchConnect open error(%d).\n", ACE_OS::last_error()));
                return false;
            }
        }
        else
        {
            if (m_pProAsynchConnect->connect(m_AddrServer, (ACE_INET_Addr&)ACE_Addr::sap_any, 1, (const void*)pProConnectInfo) == -1)
            {
                OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]m_pAsynchConnect open error(%d).\n", ACE_OS::last_error()));
                return false;
            }
        }
    }

    return true;
}

bool CProactorClientInfo::SendData(ACE_Message_Block* pmblk)
{
    if(nullptr == m_pProConnectClient)
    {
        //����������ڽ��������У��ȴ�5���룬���
        if(EM_Server_Connect_State::SERVER_CONNECT_FIRST == m_emConnectState 
            || EM_Server_Connect_State::SERVER_CONNECT_RECONNECT == m_emConnectState)
        {
            return false;
        }

        if(nullptr == m_pProConnectClient)
        {
            //�������ӽ�������Ϣ
            Common_Send_ConnectError(pmblk, m_AddrServer, m_pClientMessage);

            if (EM_Server_Connect_State::SERVER_CONNECT_FIRST != m_emConnectState
                && EM_Server_Connect_State::SERVER_CONNECT_RECONNECT != m_emConnectState
                && false == Run(true, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
            {
                //������Ӳ����ڣ��������ӡ�
                OUR_DEBUG((LM_INFO, "[CProactorClientInfo::SendData]Run error.\n"));
            }

            return false;
        }
    }

    if(true == m_pClientMessage->Need_Send_Format())
    {
        //�������ݷ�����װ
        ACE_Message_Block* pSend = nullptr;

        if(false == Common_Send_Data(pmblk, m_pClientMessage, pSend))
        {
            return false;
        }

        //��������
        return m_pProConnectClient->SendData(pSend);
    }
    else
    {
        //��������
        return m_pProConnectClient->SendData(pmblk);
    }
}

int CProactorClientInfo::GetServerID()
{
    if(nullptr == m_pProConnectClient)
    {
        return 0;
    }
    else
    {
        return m_pProConnectClient->GetServerID();
    }
}

bool CProactorClientInfo::Close()
{
    if(nullptr != m_pProConnectClient)
    {
        SetProConnectClient(nullptr);
    }

    return true;
}

void CProactorClientInfo::SetProConnectClient(CProConnectClient* pProConnectClient)
{
    m_pProConnectClient = pProConnectClient;
}

CProConnectClient* CProactorClientInfo::GetProConnectClient()
{
    return m_pProConnectClient;
}

IClientMessage* CProactorClientInfo::GetClientMessage()
{
    //���������Ƿ��������������ж��Լ��Ƿ��ǵ�һ�����ӵĻص�
    if((m_emConnectState == EM_Server_Connect_State::SERVER_CONNECT_RECONNECT || m_emConnectState == EM_Server_Connect_State::SERVER_CONNECT_FIRST) && nullptr != m_pClientMessage)
    {
        m_emConnectState = EM_Server_Connect_State::SERVER_CONNECT_OK;
        //֪ͨ�ϲ�ĳһ�������Ѿ��ָ������ѽ���
        m_pClientMessage->ReConnect(m_nServerID);
    }

    return m_pClientMessage;
}

ACE_INET_Addr CProactorClientInfo::GetServerAddr()
{
    return m_AddrServer;
}

EM_Server_Connect_State CProactorClientInfo::GetServerConnectState()
{
    return m_emConnectState;
}

void CProactorClientInfo::SetServerConnectState( EM_Server_Connect_State objState )
{
    m_emConnectState = objState;
}

void CProactorClientInfo::SetPacketParseID(uint32 u4PacketParseID)
{
    m_u4PacketParseID = u4PacketParseID;
}

NAMESPACE::uint32 CProactorClientInfo::GetPacketParseID()
{
    return m_u4PacketParseID;
}

void CProactorClientInfo::SetLocalAddr( const char* pIP, uint16 u2Port, uint8 u1IPType )
{
    if(u1IPType == TYPE_IPV4)
    {
        m_AddrLocal.set(u2Port, pIP);
    }
    else
    {
        m_AddrLocal.set(u2Port, pIP, 1, PF_INET6);
    }

    m_blIsLocal = true;
}

CClientProConnectManager::CClientProConnectManager(void)
{
    m_nTaskID                = -1;
    m_blProactorFinish       = false;
    m_u4MaxPoolCount         = 0;
    m_u4ConnectServerTimeout = 0;
    m_emS2SRunState          = S2S_Run_State_Init;
}

CClientProConnectManager::~CClientProConnectManager(void)
{
    Close();
}

bool CClientProConnectManager::Init(ACE_Proactor* pProactor)
{
    if(pProactor == nullptr)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Init]pProactor is nullptr.\n"));
        return false;
    }

    m_u4ConnectServerTimeout = GetXmlConfigAttribute(xmlConnectServer)->TimeInterval * 1000; //ת��Ϊ΢��

    if(m_u4ConnectServerTimeout == 0)
    {
        m_u4ConnectServerTimeout = PRO_CONNECT_SERVER_TIMEOUT;
    }

    //��¼����ص��������
    m_u4MaxPoolCount = GetXmlConfigAttribute(xmlConnectServer)->Count;

    //��ʼ��Hash����(TCP)
    m_objClientTCPList.Init((int)m_u4MaxPoolCount);

    //��ʼ��Hash����(UDP)
    m_objClientUDPList.Init((int)m_u4MaxPoolCount);

    if(-1 == m_ProAsynchConnect.open(false, pProactor, true))
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Init]m_ProAsynchConnect open error(%d).\n", ACE_OS::last_error()));
        return false;
    }

    //���Proactor�Ѿ����ӳɹ�
    m_blProactorFinish = true;

    m_emS2SRunState = S2S_Run_State_Run;

    //��֮ǰ�л����������Ϣ��������
    for (CS2SConnectGetRandyInfo f : m_GetReadyInfoList)
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

bool CClientProConnectManager::Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CProactorClientInfo* pClientInfo = nullptr;

    if (S2S_Run_State_Init == m_emS2SRunState)
    {
        //�����Ӧ����û��ʼ������ӵ�һ���б��У��ȴ���Ӧ����ʼ�������
        CS2SConnectGetRandyInfo objS2SConnectGetRandyInfo;

        objS2SConnectGetRandyInfo.m_nServerID      = nServerID;
        sprintf_safe(objS2SConnectGetRandyInfo.m_szServerIP, MAX_BUFF_100, "%s", pIP);
        objS2SConnectGetRandyInfo.m_u2ServerPort    = u2Port;
        objS2SConnectGetRandyInfo.m_u1Type         = u1IPType;
        objS2SConnectGetRandyInfo.m_pClientMessage = pClientMessage;

        m_GetReadyInfoList.push_back(objS2SConnectGetRandyInfo);

        return true;
    }

    //���ӳ�ʼ������
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, nullptr, 0, u1IPType, pClientMessage, pClientInfo))
    {
        return false;
    }

    //��һ�ο�ʼ����
    if(false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    //�����Ч��pClientMessage
    if (GetXmlConfigAttribute(xmlConnectServer)->RunType == 1)
    {
        App_ServerMessageTask::instance()->AddClientMessage(pClientMessage);
    }

    OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) connect is OK.\n", nServerID));

    return true;
}

bool CClientProConnectManager::Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CProactorClientInfo* pClientInfo = nullptr;

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
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, pLocalIP, u2LocalPort, u1LocalIPType, pClientMessage, pClientInfo))
    {
        return false;
    }

    //��һ�ο�ʼ����
    if(false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    //�����Ч��pClientMessage
    if (GetXmlConfigAttribute(xmlConnectServer)->RunType == 1)
    {
        App_ServerMessageTask::instance()->AddClientMessage(pClientMessage);
    }

    OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) connect is OK.\n", nServerID));

    return true;
}

bool CClientProConnectManager::ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, uint32 u4PacketParse)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CProactorClientInfo* pClientInfo = nullptr;

    //���ӳ�ʼ������
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, nullptr, 0, u1IPType, nullptr, pClientInfo, u4PacketParse))
    {
        return false;
    }

    //��һ�ο�ʼ����
    if (false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) connect is OK.\n", nServerID));

    return true;
}

bool CClientProConnectManager::ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, uint32 u4PacketParse)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CProactorClientInfo* pClientInfo = nullptr;

    //���ӳ�ʼ������
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, pLocalIP, u2LocalPort, u1LocalIPType, nullptr, pClientInfo, u4PacketParse))
    {
        return false;
    }

    //��һ�ο�ʼ����
    if (false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) connect is OK.\n", nServerID));

    return true;
}

bool CClientProConnectManager::ConnectUDP(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, EM_UDP_TYPE emType, IClientUDPMessage* pClientUDPMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CProactorUDPClient* pProactorUDPClient = nullptr;

    //���ӳ�ʼ��
    if (false == ConnectUdpInit(nServerID, pProactorUDPClient))
    {
        return false;
    }

    //����UDP���ӵ�ַ
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
        OUR_DEBUG((LM_INFO, "[CClientProConnectManager::ConnectUDP](%d)UDP set_address error[%d].\n", nServerID, errno));
        SAFE_DELETE(pProactorUDPClient);
        return false;
    }

    //����UDP
    if(0 != pProactorUDPClient->OpenAddress(AddrLocal, emType, App_ProactorManager::instance()->GetAce_Proactor(), pClientUDPMessage))
    {
        OUR_DEBUG((LM_INFO, "[CClientProConnectManager::ConnectUDP](%d)UDP OpenAddress error.\n", nServerID));
        SAFE_DELETE(pProactorUDPClient);
        return false;
    }

    return true;
}

bool CClientProConnectManager::SetHandler(int nServerID, CProConnectClient* pProConnectClient)
{
    if(nullptr == pProConnectClient)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SetHandler]pProConnectClient is nullptr.\n"));
        return false;
    }

    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(nullptr != pClientInfo)
    {
        pClientInfo->SetProConnectClient(pProConnectClient);
    }
    else
    {
        //���������Ӳ�����
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SetHandler]nServerID =(%d) is not exist.\n", nServerID));
        return false;
    }

    return true;
}

bool CClientProConnectManager::Close(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //�������Ϊ�������Ͽ�����ֻɾ��ProConnectClient��ָ��
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(nullptr == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) pClientInfo is nullptr.\n", nServerID));
        return false;
    }

    EM_s2s ems2s = EM_s2s::S2S_INNEED_CALLBACK;

    //�ر����Ӷ���
    if(nullptr != pClientInfo->GetProConnectClient())
    {
        pClientInfo->GetProConnectClient()->ClientClose(ems2s);
        SAFE_DELETE(pClientInfo);
    }

    //��Hash������ɾ��
    m_objClientTCPList.Del_Hash_Data(szServerID);

    return true;
}

bool CClientProConnectManager::CloseUDP(int nServerID)
{
    //�������Ϊ�������Ͽ�����ֻɾ��ProConnectClient��ָ��
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorUDPClient* pClientInfo = m_objClientUDPList.Get_Hash_Box_Data(szServerID);

    if(nullptr == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::CloseUDP]nServerID =(%d) pClientInfo is nullptr.\n", nServerID));
        return false;
    }

    pClientInfo->Close();
    SAFE_DELETE(pClientInfo);
    //��Hash����ɾ����ǰ���ڵĶ���
    m_objClientUDPList.Del_Hash_Data(szServerID);
    return true;
}

bool CClientProConnectManager::ConnectErrorClose(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(nullptr == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::ConnectErrorClose]nServerID =(%d) pClientInfo is nullptr.\n", nServerID));
        return false;
    }

    //��Hash����ɾ����ǰ���ڵĶ���
    SAFE_DELETE(pClientInfo);
    m_objClientTCPList.Del_Hash_Data(szServerID);

    return true;
}

IClientMessage* CClientProConnectManager::GetClientMessage(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(nullptr != pClientInfo)
    {
        return pClientInfo->GetClientMessage();
    }

    return nullptr;
}

uint32 CClientProConnectManager::GetPacketParseID(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = { '\0' };
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr != pClientInfo)
    {
        return pClientInfo->GetPacketParseID();
    }

    return 0;
}

bool CClientProConnectManager::SendData(int nServerID, char*& pData, int nSize, bool blIsDelete)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(nullptr == pClientInfo)
    {
        //���������Ӳ����ڣ��򲻴����µ�����
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SendData]nServerID =(%d) is not exist.\n", nServerID));

        if(true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

    ACE_Message_Block* pmblk = App_MessageBlockManager::instance()->Create(nSize);

    if(nullptr == pmblk)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SendData]nServerID =(%d) pmblk is nullptr.\n", nServerID));

        if(true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

    memcpy_safe((char* )pData, (uint32)nSize, (char* )pmblk->wr_ptr(), (uint32)nSize);
    pmblk->wr_ptr(nSize);

    if(true == blIsDelete && nullptr != pData)
    {
        SAFE_DELETE_ARRAY(pData);
    }

    //��������
    return pClientInfo->SendData(pmblk);
}

bool CClientProConnectManager::SendDataUDP(int nServerID,const char* pIP, uint16 u2Port, char*& pMessage, uint32 u4Len, bool blIsDelete)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorUDPClient* pClientInfo = m_objClientUDPList.Get_Hash_Box_Data(szServerID);

    if(nullptr == pClientInfo)
    {
        //���������Ӳ����ڣ��򲻴����µ�����
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SendDataUDP]nServerID =(%d) is not exist.\n", nServerID));

        if(true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pMessage);
        }

        return false;
    }

    //��������
    bool blSendRet = pClientInfo->SendMessage(pMessage, u4Len, pIP, u2Port);

    if(true == blIsDelete)
    {
        SAFE_DELETE_ARRAY(pMessage);
    }

    return blSendRet;
}

bool CClientProConnectManager::StartConnectTask(int nIntervalTime)
{
    CancelConnectTask();
    m_nTaskID = App_TimerManager::instance()->schedule(this, (void* )nullptr, ACE_OS::gettimeofday() + ACE_Time_Value(nIntervalTime), ACE_Time_Value(nIntervalTime));

    if(m_nTaskID == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::StartConnectTask].StartConnectTask is fail, time is (%d).\n", nIntervalTime));
        return false;
    }

    return true;
}

void CClientProConnectManager::CancelConnectTask()
{
    if(m_nTaskID != -1)
    {
        //ɱ��֮ǰ�Ķ�ʱ�������¿����µĶ�ʱ��
        App_TimerManager::instance()->cancel(m_nTaskID);
        m_nTaskID = -1;
    }
}

void CClientProConnectManager::Close()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //����ж�ʱ�����رն�ʱ��
    CancelConnectTask();

    //�ر������Ѵ��ڵ�����
    vector<CProactorClientInfo*> vecProactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecProactorClientInfo);

    for(auto* pClientInfo : vecProactorClientInfo)
    {
        EM_s2s ems2s = EM_s2s::S2S_INNEED_CALLBACK;

        if(nullptr != pClientInfo)
        {
            pClientInfo->GetProConnectClient()->ClientClose(ems2s);
            SAFE_DELETE(pClientInfo);
        }
    }

    m_objClientTCPList.Close();

    vector<CProactorUDPClient*> vecProactorUDPClient;
    m_objClientUDPList.Get_All_Used(vecProactorUDPClient);

    for(auto* pClientInfo : vecProactorUDPClient)
    {
        if(nullptr != pClientInfo)
        {
            pClientInfo->Close();
            SAFE_DELETE(pClientInfo);
        }
    }

    m_objClientUDPList.Close();
    m_u4MaxPoolCount = 0;
}

int CClientProConnectManager::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    ACE_UNUSED_ARG(arg);
    ACE_UNUSED_ARG(tv);

    if(m_ProAsynchConnect.GetConnectState() == true)
    {
        return 0;
    }

    m_ThreadWritrLock.acquire();
    vector<CProactorClientInfo*> vecProactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecProactorClientInfo);
    m_ThreadWritrLock.release();

    for(auto* pClientInfo : vecProactorClientInfo)
    {
        if(nullptr != pClientInfo)
        {
            if(nullptr == pClientInfo->GetProConnectClient())
            {
                //������Ӳ����ڣ������½�������
                if (false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
                {
                    OUR_DEBUG((LM_DEBUG, "[CClientProConnectManager::handle_timeout]Run is fail.\n"));
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
    }

    return 0;
}

bool CClientProConnectManager::ConnectTcpInit(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, int nLocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage, CProactorClientInfo*& pClientInfo, uint32 u4PacketParseID)
{
    char szServerID[10] = { '\0' };
    sprintf_safe(szServerID, 10, "%d", nServerID);
    pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr != pClientInfo)
    {
        //�����������Ѿ����ڣ��򲻴����µ�����
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) is exist.\n", nServerID));
        return false;
    }

    //������Ѿ����ˣ�����������������
    if (m_objClientTCPList.Get_Used_Count() == m_objClientTCPList.Get_Count())
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) m_objClientTCPList is full.\n", nServerID));
        return false;
    }

    //��ʼ��������Ϣ
    pClientInfo = new CProactorClientInfo();

    if (false == pClientInfo->Init(pIP, u2Port, u1IPType, nServerID, &m_ProAsynchConnect, pClientMessage, u4PacketParseID))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    //���ñ���IP�Ͷ˿�
    if (nullptr != pLocalIP && nLocalPort > 0)
    {
        pClientInfo->SetLocalAddr(pLocalIP, nLocalPort, u1LocalIPType);
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

bool CClientProConnectManager::ConnectUdpInit(int nServerID, CProactorUDPClient*& pProactorUDPClient)
{
    char szServerID[10] = { '\0' };
    sprintf_safe(szServerID, 10, "%d", nServerID);
    pProactorUDPClient = m_objClientUDPList.Get_Hash_Box_Data(szServerID);

    if (nullptr != pProactorUDPClient)
    {
        //�����������Ѿ����ڣ��򲻴����µ�����
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::ConnectUDP]nServerID =(%d) is exist.\n", nServerID));
        return false;
    }

    //������Ѿ����ˣ�����������������
    if (m_objClientUDPList.Get_Used_Count() == m_objClientUDPList.Get_Count())
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::ConnectUDP]nServerID =(%d) m_objClientTCPList is full.\n", nServerID));
        return false;
    }

    pProactorUDPClient = new CProactorUDPClient();

    if (nullptr == pProactorUDPClient)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::ConnectUDP]nServerID =(%d) pProactorUDPClient is nullptr.\n", nServerID));
        return false;
    }

    //�����Ѿ���������ӽ�hash
    if (-1 == m_objClientUDPList.Add_Hash_Data(szServerID, pProactorUDPClient))
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) add m_objClientTCPList is fail.\n", nServerID));
        SAFE_DELETE(pProactorUDPClient);
        return false;
    }

    return true;
}

void CClientProConnectManager::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    VecClientConnectInfo.clear();
    vector<CProactorClientInfo*> vecProactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecProactorClientInfo);

    for(auto* pClientInfo : vecProactorClientInfo)
    {
        if(nullptr != pClientInfo)
        {
            if(nullptr != pClientInfo->GetProConnectClient())
            {
                //�����Ѿ�����
                _ClientConnectInfo ClientConnectInfo = pClientInfo->GetProConnectClient()->GetClientConnectInfo();
                ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
                VecClientConnectInfo.push_back(ClientConnectInfo);
            }
            else
            {
                //����δ����
                _ClientConnectInfo ClientConnectInfo;
                ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
                ClientConnectInfo.m_blValid    = false;
                VecClientConnectInfo.push_back(ClientConnectInfo);
            }
        }
    }
}

void CClientProConnectManager::GetUDPConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    vector<CProactorUDPClient*> vecProactorUDPClient;
    m_objClientUDPList.Get_All_Used(vecProactorUDPClient);

    for(auto* pClientInfo : vecProactorUDPClient)
    {
        if(nullptr != pClientInfo)
        {
            _ClientConnectInfo ClientConnectInfo = pClientInfo->GetClientConnectInfo();
            VecClientConnectInfo.push_back(ClientConnectInfo);
        }
    }

}

bool CClientProConnectManager::CloseByClient(int nServerID)
{
    //�������ΪԶ�̿ͻ��˶Ͽ�����ֻɾ��ProConnectClient��ָ��
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(nullptr != pClientInfo)
    {
        pClientInfo->SetProConnectClient(nullptr);
        pClientInfo->SetServerConnectState(EM_Server_Connect_State::SERVER_CONNECT_FAIL);
    }

    return true;
}

EM_Server_Connect_State CClientProConnectManager::GetConnectState(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(nullptr == pClientInfo)
    {
        //���������Ӳ����ڣ��򲻴����µ�����
        return EM_Server_Connect_State::SERVER_CONNECT_FAIL;
    }
    else
    {
        return pClientInfo->GetServerConnectState();
    }
}

bool CClientProConnectManager::ReConnect(int nServerID)
{
    //��鵱ǰ�����Ƿ��ǻ�Ծ��
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(nullptr == pClientInfo)
    {
        return false;
    }

    if(nullptr == pClientInfo->GetProConnectClient())
    {
        //������Ӳ����ڣ������½�������
        if (false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
        {
            OUR_DEBUG((LM_INFO, "[CClientProConnectManager::ReConnect]Run is fail.\n"));
            return false;
        }

        return true;
    }
    else
    {
        return true;
    }
}

ACE_INET_Addr CClientProConnectManager::GetServerAddr(int nServerID)
{
    ACE_INET_Addr remote_addr;
    //��鵱ǰ�����Ƿ��ǻ�Ծ��
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(nullptr != pClientInfo)
    {
        remote_addr = pClientInfo->GetServerAddr();
        return remote_addr;
    }
    else
    {
        return remote_addr;
    }
}

bool CClientProConnectManager::SetServerConnectState(int nServerID, EM_Server_Connect_State objState)
{
    //��鵱ǰ�����Ƿ��ǻ�Ծ��
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(nullptr != pClientInfo)
    {
        pClientInfo->SetServerConnectState(objState);
        return true;
    }
    else
    {
        return false;
    }
}

bool CClientProConnectManager::GetServerIPInfo(int nServerID, _ClientIPInfo& objServerIPInfo)
{
    //��鵱ǰ�����Ƿ��ǻ�Ծ��
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(nullptr != pClientInfo)
    {
        ACE_INET_Addr remote_addr = pClientInfo->GetServerAddr();
        objServerIPInfo.m_strClientIP = remote_addr.get_host_addr();
        objServerIPInfo.m_u2Port = remote_addr.get_port_number();
        return true;
    }
    else
    {
        return  false;
    }
}

bool CClientProConnectManager::DeleteIClientMessage(IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //���첽�ص���Ч�����д�pClientMessage����Ϊ��Ч
    App_ServerMessageTask::instance()->DelClientMessage(pClientMessage);

    //һһѰ����֮��Ӧ�������Լ������Ϣ��ɾ��֮
    vector<CProactorClientInfo*> vecProactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecProactorClientInfo);

    for(auto* pClientInfo : vecProactorClientInfo)
    {
        if(nullptr != pClientInfo && pClientInfo->GetClientMessage() == pClientMessage)
        {
            //�ر����ӣ���ɾ������
            //�ر����Ӷ���
            if (nullptr != pClientInfo->GetClientMessage())
            {
                EM_s2s ems2s = EM_s2s::S2S_INNEED_CALLBACK;
                pClientInfo->GetProConnectClient()->ClientClose(ems2s);
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
