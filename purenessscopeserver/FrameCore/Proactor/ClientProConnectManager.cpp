#include "ClientProConnectManager.h"

CProactorClientInfo::CProactorClientInfo()
{
    m_pProConnectClient = NULL;
    m_pProAsynchConnect = NULL;
    m_pClientMessage    = NULL;
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
    if(NULL != m_pProConnectClient)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]Connect is exist.\n"));
        return false;
    }

    if(NULL == m_pProAsynchConnect)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]m_pAsynchConnect is NULL.\n"));
        return false;
    }

    if(true == blIsReadly 
        && EM_Server_Connect_State::SERVER_CONNECT_FIRST != m_emConnectState 
        && EM_Server_Connect_State::SERVER_CONNECT_RECONNECT != m_emConnectState)
    {
        m_pProAsynchConnect->SetConnectState(true);

        //创建一个数据参数，传给远端
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
    if(NULL == m_pProConnectClient)
    {
        //如果连接正在建立过程中，等待5毫秒，如果
        if(EM_Server_Connect_State::SERVER_CONNECT_FIRST == m_emConnectState 
            || EM_Server_Connect_State::SERVER_CONNECT_RECONNECT == m_emConnectState)
        {
            return false;
        }

        if(NULL == m_pProConnectClient)
        {
            //发送连接建立无信息
            Common_Send_ConnectError(pmblk, m_AddrServer, m_pClientMessage);

            if (EM_Server_Connect_State::SERVER_CONNECT_FIRST != m_emConnectState
                && EM_Server_Connect_State::SERVER_CONNECT_RECONNECT != m_emConnectState
                && false == Run(true, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
            {
                //如果连接不存在，则建立链接。
                OUR_DEBUG((LM_INFO, "[CProactorClientInfo::SendData]Run error.\n"));
            }

            return false;
        }
    }

    if(true == m_pClientMessage->Need_Send_Format())
    {
        //调用数据发送组装
        ACE_Message_Block* pSend = NULL;

        if(false == Common_Send_Data(pmblk, m_pClientMessage, pSend))
        {
            return false;
        }

        //发送数据
        return m_pProConnectClient->SendData(pSend);
    }
    else
    {
        //发送数据
        return m_pProConnectClient->SendData(pmblk);
    }
}

int CProactorClientInfo::GetServerID()
{
    if(NULL == m_pProConnectClient)
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
    if(NULL != m_pProConnectClient)
    {
        SetProConnectClient(NULL);
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
    //这里增加是否是连接重练的判定以及是否是第一次连接的回调
    if((m_emConnectState == EM_Server_Connect_State::SERVER_CONNECT_RECONNECT || m_emConnectState == EM_Server_Connect_State::SERVER_CONNECT_FIRST) && NULL != m_pClientMessage)
    {
        m_emConnectState = EM_Server_Connect_State::SERVER_CONNECT_OK;
        //通知上层某一个连接已经恢复或者已建立
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
    if(pProactor == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Init]pProactor is NULL.\n"));
        return false;
    }

    m_u4ConnectServerTimeout = GetXmlConfigAttribute(xmlConnectServer)->TimeInterval * 1000; //转换为微妙

    if(m_u4ConnectServerTimeout == 0)
    {
        m_u4ConnectServerTimeout = PRO_CONNECT_SERVER_TIMEOUT;
    }

    //记录缓冲池的最大上限
    m_u4MaxPoolCount = GetXmlConfigAttribute(xmlConnectServer)->Count;

    //初始化Hash数组(TCP)
    m_objClientTCPList.Init((int)m_u4MaxPoolCount);

    //初始化Hash数组(UDP)
    m_objClientUDPList.Init((int)m_u4MaxPoolCount);

    if(-1 == m_ProAsynchConnect.open(false, pProactor, true))
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Init]m_ProAsynchConnect open error(%d).\n", ACE_OS::last_error()));
        return false;
    }

    //标记Proactor已经连接成功
    m_blProactorFinish = true;

    m_emS2SRunState = S2S_Run_State_Run;

    //将之前有缓冲的连接信息建立连接
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
    CProactorClientInfo* pClientInfo = NULL;

    if (S2S_Run_State_Init == m_emS2SRunState)
    {
        //如果反应器还没初始化，添加到一个列表中，等待反应器初始化后调用
        CS2SConnectGetRandyInfo objS2SConnectGetRandyInfo;

        objS2SConnectGetRandyInfo.m_nServerID      = nServerID;
        sprintf_safe(objS2SConnectGetRandyInfo.m_szServerIP, MAX_BUFF_100, "%s", pIP);
        objS2SConnectGetRandyInfo.m_u2ServerPort    = u2Port;
        objS2SConnectGetRandyInfo.m_u1Type         = u1IPType;
        objS2SConnectGetRandyInfo.m_pClientMessage = pClientMessage;

        m_GetReadyInfoList.push_back(objS2SConnectGetRandyInfo);

        return true;
    }

    //连接初始化动作
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, NULL, 0, u1IPType, pClientMessage, pClientInfo))
    {
        return false;
    }

    //第一次开始链接
    if(false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    //添加有效的pClientMessage
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
    CProactorClientInfo* pClientInfo = NULL;

    if (S2S_Run_State_Init == m_emS2SRunState)
    {
        //如果反应器还没初始化，添加到一个列表中，等待反应器初始化后调用
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

    //连接初始化动作
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, pLocalIP, u2LocalPort, u1LocalIPType, pClientMessage, pClientInfo))
    {
        return false;
    }

    //第一次开始链接
    if(false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    //添加有效的pClientMessage
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
    CProactorClientInfo* pClientInfo = NULL;

    //连接初始化动作
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, NULL, 0, u1IPType, NULL, pClientInfo, u4PacketParse))
    {
        return false;
    }

    //第一次开始链接
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
    CProactorClientInfo* pClientInfo = NULL;

    //连接初始化动作
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, pLocalIP, u2LocalPort, u1LocalIPType, NULL, pClientInfo, u4PacketParse))
    {
        return false;
    }

    //第一次开始链接
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
    CProactorUDPClient* pProactorUDPClient = NULL;

    //链接初始化
    if (false == ConnectUdpInit(nServerID, pProactorUDPClient))
    {
        return false;
    }

    //设置UDP连接地址
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
        //如果是UDP广播
        AddrLocal.set(u2Port, (uint32)INADDR_ANY);
    }

    if (nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[CClientProConnectManager::ConnectUDP](%d)UDP set_address error[%d].\n", nServerID, errno));
        SAFE_DELETE(pProactorUDPClient);
        return false;
    }

    //连接UDP
    if(0 != pProactorUDPClient->OpenAddress(AddrLocal, emType, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_UDPDEFINE), pClientUDPMessage))
    {
        OUR_DEBUG((LM_INFO, "[CClientProConnectManager::ConnectUDP](%d)UDP OpenAddress error.\n", nServerID));
        SAFE_DELETE(pProactorUDPClient);
        return false;
    }

    return true;
}

bool CClientProConnectManager::SetHandler(int nServerID, CProConnectClient* pProConnectClient)
{
    if(NULL == pProConnectClient)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SetHandler]pProConnectClient is NULL.\n"));
        return false;
    }

    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(NULL != pClientInfo)
    {
        pClientInfo->SetProConnectClient(pProConnectClient);
    }
    else
    {
        //如果这个链接不存在
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SetHandler]nServerID =(%d) is not exist.\n", nServerID));
        return false;
    }

    return true;
}

bool CClientProConnectManager::Close(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //如果是因为服务器断开，则只删除ProConnectClient的指针
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(NULL == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
        return false;
    }

    EM_s2s ems2s = EM_s2s::S2S_INNEED_CALLBACK;

    //关闭链接对象
    if(NULL != pClientInfo->GetProConnectClient())
    {
        pClientInfo->GetProConnectClient()->ClientClose(ems2s);
        SAFE_DELETE(pClientInfo);
    }

    //从Hash表里面删除
    m_objClientTCPList.Del_Hash_Data(szServerID);

    return true;
}

bool CClientProConnectManager::CloseUDP(int nServerID)
{
    //如果是因为服务器断开，则只删除ProConnectClient的指针
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorUDPClient* pClientInfo = m_objClientUDPList.Get_Hash_Box_Data(szServerID);

    if(NULL == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::CloseUDP]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
        return false;
    }

    pClientInfo->Close();
    SAFE_DELETE(pClientInfo);
    //从Hash里面删除当前存在的对象
    m_objClientUDPList.Del_Hash_Data(szServerID);
    return true;
}

bool CClientProConnectManager::ConnectErrorClose(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(NULL == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::ConnectErrorClose]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
        return false;
    }

    //从Hash里面删除当前存在的对象
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

    if(NULL != pClientInfo)
    {
        return pClientInfo->GetClientMessage();
    }

    return NULL;
}

uint32 CClientProConnectManager::GetPacketParseID(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = { '\0' };
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL != pClientInfo)
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

    if(NULL == pClientInfo)
    {
        //如果这个链接不存在，则不创建新的链接
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SendData]nServerID =(%d) is not exist.\n", nServerID));

        if(true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

    ACE_Message_Block* pmblk = App_MessageBlockManager::instance()->Create(nSize);

    if(NULL == pmblk)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SendData]nServerID =(%d) pmblk is NULL.\n", nServerID));

        if(true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

    memcpy_safe((char* )pData, (uint32)nSize, (char* )pmblk->wr_ptr(), (uint32)nSize);
    pmblk->wr_ptr(nSize);

    if(true == blIsDelete && NULL != pData)
    {
        SAFE_DELETE_ARRAY(pData);
    }

    //发送数据
    return pClientInfo->SendData(pmblk);
}

bool CClientProConnectManager::SendDataUDP(int nServerID,const char* pIP, uint16 u2Port, char*& pMessage, uint32 u4Len, bool blIsDelete)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorUDPClient* pClientInfo = m_objClientUDPList.Get_Hash_Box_Data(szServerID);

    if(NULL == pClientInfo)
    {
        //如果这个链接不存在，则不创建新的链接
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SendDataUDP]nServerID =(%d) is not exist.\n", nServerID));

        if(true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pMessage);
        }

        return false;
    }

    //发送数据
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
    m_nTaskID = App_TimerManager::instance()->schedule(this, (void* )NULL, ACE_OS::gettimeofday() + ACE_Time_Value(nIntervalTime), ACE_Time_Value(nIntervalTime));

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
        //杀死之前的定时器，重新开启新的定时器
        App_TimerManager::instance()->cancel(m_nTaskID);
        m_nTaskID = -1;
    }
}

void CClientProConnectManager::Close()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //如果有定时器，关闭定时器
    CancelConnectTask();

    //关闭所有已存在的链接
    vector<CProactorClientInfo*> vecProactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecProactorClientInfo);

    for(auto* pClientInfo : vecProactorClientInfo)
    {
        EM_s2s ems2s = EM_s2s::S2S_INNEED_CALLBACK;

        if(NULL != pClientInfo)
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
        if(NULL != pClientInfo)
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
        if(NULL != pClientInfo)
        {
            if(NULL == pClientInfo->GetProConnectClient())
            {
                //如果连接不存在，则重新建立连接
                if (false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
                {
                    OUR_DEBUG((LM_DEBUG, "[CClientProConnectManager::handle_timeout]Run is fail.\n"));
                }
            }
            else
            {
                //检查当前连接，是否已挂起或死锁
                ACE_Time_Value tvNow = ACE_OS::gettimeofday();

                //如果是异步模式，则需要检查处理线程是否被挂起
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

    if (NULL != pClientInfo)
    {
        //如果这个链接已经存在，则不创建新的链接
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) is exist.\n", nServerID));
        return false;
    }

    //如果池已经满了，则不能在申请新连接
    if (m_objClientTCPList.Get_Used_Count() == m_objClientTCPList.Get_Count())
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) m_objClientTCPList is full.\n", nServerID));
        return false;
    }

    //初始化链接信息
    pClientInfo = new CProactorClientInfo();

    if (false == pClientInfo->Init(pIP, u2Port, u1IPType, nServerID, &m_ProAsynchConnect, pClientMessage, u4PacketParseID))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    //设置本地IP和端口
    if (NULL != pLocalIP && nLocalPort > 0)
    {
        pClientInfo->SetLocalAddr(pLocalIP, nLocalPort, u1LocalIPType);
    }

    //添加进hash
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

    if (NULL != pProactorUDPClient)
    {
        //如果这个链接已经存在，则不创建新的链接
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::ConnectUDP]nServerID =(%d) is exist.\n", nServerID));
        return false;
    }

    //如果池已经满了，则不能在申请新连接
    if (m_objClientUDPList.Get_Used_Count() == m_objClientUDPList.Get_Count())
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::ConnectUDP]nServerID =(%d) m_objClientTCPList is full.\n", nServerID));
        return false;
    }

    pProactorUDPClient = new CProactorUDPClient();

    if (NULL == pProactorUDPClient)
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::ConnectUDP]nServerID =(%d) pProactorUDPClient is NULL.\n", nServerID));
        return false;
    }

    //链接已经建立，添加进hash
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
        if(NULL != pClientInfo)
        {
            if(NULL != pClientInfo->GetProConnectClient())
            {
                //链接已经建立
                _ClientConnectInfo ClientConnectInfo = pClientInfo->GetProConnectClient()->GetClientConnectInfo();
                ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
                VecClientConnectInfo.push_back(ClientConnectInfo);
            }
            else
            {
                //连接未建立
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
        if(NULL != pClientInfo)
        {
            _ClientConnectInfo ClientConnectInfo = pClientInfo->GetClientConnectInfo();
            VecClientConnectInfo.push_back(ClientConnectInfo);
        }
    }

}

bool CClientProConnectManager::CloseByClient(int nServerID)
{
    //如果是因为远程客户端断开，则只删除ProConnectClient的指针
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(NULL != pClientInfo)
    {
        pClientInfo->SetProConnectClient(NULL);
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

    if(NULL == pClientInfo)
    {
        //如果这个链接不存在，则不创建新的链接
        return EM_Server_Connect_State::SERVER_CONNECT_FAIL;
    }
    else
    {
        return pClientInfo->GetServerConnectState();
    }
}

bool CClientProConnectManager::ReConnect(int nServerID)
{
    //检查当前连接是否是活跃的
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(NULL == pClientInfo)
    {
        return false;
    }

    if(NULL == pClientInfo->GetProConnectClient())
    {
        //如果连接不存在，则重新建立连接
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
    //检查当前连接是否是活跃的
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(NULL != pClientInfo)
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
    //检查当前连接是否是活跃的
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(NULL != pClientInfo)
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
    //检查当前连接是否是活跃的
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CProactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if(NULL != pClientInfo)
    {
        ACE_INET_Addr remote_addr = pClientInfo->GetServerAddr();
        sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_50, remote_addr.get_host_addr());
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

    //将异步回调有效队列中此pClientMessage设置为无效
    App_ServerMessageTask::instance()->DelClientMessage(pClientMessage);

    //一一寻找与之对应的连接以及相关信息并删除之
    vector<CProactorClientInfo*> vecProactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecProactorClientInfo);

    for(auto* pClientInfo : vecProactorClientInfo)
    {
        if(NULL != pClientInfo && pClientInfo->GetClientMessage() == pClientMessage)
        {
            //关闭连接，并删除对象。
            //关闭链接对象
            if (NULL != pClientInfo->GetClientMessage())
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
