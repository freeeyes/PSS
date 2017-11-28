#include "ClientReConnectManager.h"

CReactorClientInfo::CReactorClientInfo()
{
    m_pConnectClient         = NULL;
    m_pClientMessage         = NULL;
    m_pReactorConnect        = NULL;
    m_pReactor               = NULL;
    m_nServerID              = 0;
    m_emConnectState         = SERVER_CONNECT_READY;
    m_AddrLocal              = (ACE_INET_Addr&)ACE_Addr::sap_any;
}

CReactorClientInfo::~CReactorClientInfo()
{
    OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::~CReactorClientInfo].\n"));
}

bool CReactorClientInfo::Init(int nServerID, const char* pIP, int nPort, uint8 u1IPType, CConnectClientConnector* pReactorConnect, IClientMessage* pClientMessage, ACE_Reactor* pReactor)
{
    int nRet = 0;

    if (u1IPType == TYPE_IPV4)
    {
        nRet = m_AddrServer.set(nPort, pIP);
    }
    else
    {
        nRet = m_AddrServer.set(nPort, pIP, 1, PF_INET6);
    }

    if (-1 == nRet)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Init]adrClient(%s:%d) error.\n", pIP, nPort));
        return false;
    }

    m_pClientMessage    = pClientMessage;
    m_pReactorConnect   = pReactorConnect;
    m_nServerID         = nServerID;
    m_pReactor          = pReactor;
    return true;
}

bool CReactorClientInfo::Run(bool blIsReady, EM_Server_Connect_State emState)
{
    if (NULL != m_pConnectClient)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Run]Connect is exist.\n"));
        return false;
    }

    if (NULL == m_pReactorConnect)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Run]m_pAsynchConnect is NULL.\n"));
        return false;
    }

    m_pConnectClient = new CConnectClient();

    if (NULL == m_pConnectClient)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Run]pConnectClient new is NULL(%d).\n", emState));
        return false;
    }

    m_pConnectClient->SetServerID(m_nServerID);
    m_pConnectClient->reactor(m_pReactor);

    if (blIsReady == true && SERVER_CONNECT_FIRST != m_emConnectState && SERVER_CONNECT_RECONNECT != m_emConnectState)
    {
        if (m_pReactorConnect->connect(m_pConnectClient, m_AddrServer, ACE_Synch_Options::defaults, m_AddrLocal) == -1)
        {
            m_emConnectState = SERVER_CONNECT_FAIL;
            OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Run](%s:%d) connection fails(ServerID=%d) error(%d).\n", m_AddrServer.get_host_addr(), m_AddrServer.get_port_number(), m_nServerID, ACE_OS::last_error()));
            //这里设置为True，为了让自动重试起作用
            return true;
        }

        m_emConnectState = SERVER_CONNECT_OK;
    }

    return true;
}

bool CReactorClientInfo::SendData(ACE_Message_Block* pmblk)
{
    if (NULL == m_pConnectClient)
    {
        //如果连接正在建立过程中，等待5毫秒，如果
        if(SERVER_CONNECT_FIRST == m_emConnectState || SERVER_CONNECT_RECONNECT == m_emConnectState)
        {
            return false;
        }

        if (NULL == m_pConnectClient)
        {
            if(SERVER_CONNECT_FIRST != m_emConnectState && SERVER_CONNECT_RECONNECT != m_emConnectState)
            {
                //如果连接不存在，则建立链接。
                if (false == Run(true, SERVER_CONNECT_RECONNECT))
                {
                    OUR_DEBUG((LM_INFO, "[CReactorClientInfo::SendData]Run error.\n"));
                }
            }

            if (NULL != pmblk)
            {
                App_MessageBlockManager::instance()->Close(pmblk);
            }

            //如果消息有处理接口，则返回失败接口
            if (NULL != m_pClientMessage)
            {
                //服务器已经断开，需要等待重新连接的结果
                _ClientIPInfo objServerIPInfo;
                sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrServer.get_host_addr());
                objServerIPInfo.m_nPort = m_AddrServer.get_port_number();
                m_pClientMessage->ConnectError(101, objServerIPInfo);
            }

            return false;
        }
    }

    if(true == m_pClientMessage->Need_Send_Format())
    {
        //调用数据发送组装
        ACE_Message_Block* pSend = NULL;
        bool blRet = m_pClientMessage->Send_Format_data(pmblk->rd_ptr(), (uint32)pmblk->length(), App_MessageBlockManager::instance(), pSend);

        if(false == blRet)
        {
            App_MessageBlockManager::instance()->Close(pmblk);
            App_MessageBlockManager::instance()->Close(pSend);
            return false;
        }
        else
        {
            App_MessageBlockManager::instance()->Close(pmblk);
        }

        //发送数据
        return m_pConnectClient->SendData(pSend);
    }
    else
    {
        //发送数据
        return m_pConnectClient->SendData(pmblk);
    }
}

int CReactorClientInfo::GetServerID()
{
    return m_nServerID;
}

bool CReactorClientInfo::Close()
{
    //OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Close]Begin.\n"));
    if (NULL != m_pConnectClient)
    {
        //OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::Close]End 2.\n"));
        SetConnectClient(NULL);
    }

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
    //这里增加是否是连接重练的判定
    if ((m_emConnectState == SERVER_CONNECT_FAIL) && NULL != m_pClientMessage)
    {
        m_emConnectState = SERVER_CONNECT_OK;
        //通知上层某一个连接已经恢复
        m_pClientMessage->ReConnect(m_nServerID);
    }

    return m_pClientMessage;
}

ACE_INET_Addr CReactorClientInfo::GetServerAddr()
{
    return m_AddrServer;
}

EM_Server_Connect_State CReactorClientInfo::GetServerConnectState()
{
    return m_emConnectState;
}

void CReactorClientInfo::SetServerConnectState(EM_Server_Connect_State objState)
{
    m_emConnectState = objState;
}

void CReactorClientInfo::SetLocalAddr( const char* pIP, int nPort, uint8 u1IPType )
{
    int nRet = 0;

    if (u1IPType == TYPE_IPV4)
    {
        nRet = m_AddrLocal.set(nPort, pIP);
    }
    else
    {
        nRet = m_AddrLocal.set(nPort, pIP, 1, PF_INET6);
    }

    if (-1 == nRet)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorClientInfo::SetLocalAddr]AddrLocal(%s:%d) error.\n", pIP, nPort));
        return;
    }
}

CClientReConnectManager::CClientReConnectManager(void)
{
    m_nTaskID                = -1;
    m_pReactor               = NULL;
    m_blReactorFinish        = false;
    m_u4ConnectServerTimeout = 0;
    m_u4MaxPoolCount         = 0;
}

CClientReConnectManager::~CClientReConnectManager(void)
{
    OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::~CClientReConnectManager].\n"));
    //Close();
}

bool CClientReConnectManager::Init(ACE_Reactor* pReactor)
{
    if (-1 == m_ReactorConnect.open(pReactor))
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Init]open error(%d).", ACE_OS::last_error()));
        return false;
    }

    //记录缓冲池的最大上限
    m_u4MaxPoolCount = App_MainConfig::instance()->GetServerConnectCount();

    //初始化Hash数组(TCP)
    m_objClientTCPList.Init((int)m_u4MaxPoolCount);

    //初始化Hash数组(UDP)
    m_objClientUDPList.Init((int)m_u4MaxPoolCount);

    m_u4ConnectServerTimeout = App_MainConfig::instance()->GetConnectServerTimeout() * 1000; //转换为微妙

    if (m_u4ConnectServerTimeout == 0)
    {
        m_u4ConnectServerTimeout = RE_CONNECT_SERVER_TIMEOUT;
    }

    m_pReactor        = pReactor;
    m_blReactorFinish = true;
    return true;
}

bool CClientReConnectManager::Connect(int nServerID, const char* pIP, int nPort, uint8 u1IPType, IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CReactorClientInfo* pClientInfo = NULL;

    //连接初始化动作
    if (false == ConnectTcpInit(nServerID, pIP, nPort, u1IPType, NULL, 0, u1IPType, pClientMessage, pClientInfo))
    {
        return false;
    }

    //开始链接
    if (false == pClientInfo->Run(m_blReactorFinish, SERVER_CONNECT_FIRST))
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]Run Error.\n"));
        delete pClientInfo;
        pClientInfo = NULL;

        if (false == Close(nServerID))
        {
            OUR_DEBUG((LM_INFO, "[CClientReConnectManager::Connect]Close Error"));
        }

        return false;
    }

    OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]nServerID =(%d) connect is OK.\n", nServerID));
    return true;
}

bool CClientReConnectManager::Connect(int nServerID, const char* pIP, int nPort, uint8 u1IPType, const char* pLocalIP, int nLocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CReactorClientInfo* pClientInfo = NULL;

    //连接初始化动作
    if (false == ConnectTcpInit(nServerID, pIP, nPort, u1IPType, pLocalIP, nLocalPort, u1LocalIPType, pClientMessage, pClientInfo))
    {
        return false;
    }

    //开始链接
    if (false == pClientInfo->Run(m_blReactorFinish, SERVER_CONNECT_FIRST))
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]Run Error.\n"));
        delete pClientInfo;
        pClientInfo = NULL;

        if (false == Close(nServerID))
        {
            OUR_DEBUG((LM_INFO, "[CClientReConnectManager::Connect]Close Error.\n"));
        }

        return false;
    }

    OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]nServerID =(%d) connect is OK.\n", nServerID));
    return true;
}

bool CClientReConnectManager::ConnectUDP(int nServerID, const char* pIP, int nPort, uint8 u1IPType, EM_UDP_TYPE emType, IClientUDPMessage* pClientUDPMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CReactorUDPClient* pReactorUDPClient = NULL;

    //初始化连接动作
    if (false == ConnectUdpInit(nServerID, pReactorUDPClient))
    {
        return false;
    }

    //初始化连接地址
    ACE_INET_Addr AddrLocal;
    int nErr = 0;

    if (emType != UDP_BROADCAST)
    {
        if (u1IPType == TYPE_IPV4)
        {
            nErr = AddrLocal.set(nPort, pIP);
        }
        else
        {
            nErr = AddrLocal.set(nPort, pIP, 1, PF_INET6);
        }
    }
    else
    {
        //如果是UDP广播
        AddrLocal.set(nPort, (uint32)INADDR_ANY);
    }

    if (nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[CClientReConnectManager::ConnectUDP](%d)UDP set_address error[%d].\n", nServerID, errno));
        SAFE_DELETE(pReactorUDPClient);
        return false;
    }

    //开始连接
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
    if (NULL == pConnectClient)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::SetHandler]pProConnectClient is NULL.\n"));
        return false;
    }

    //查找已有连接
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL == pClientInfo)
    {
        //如果这个链接已经存在，则不再添加到已经存在的客户端Hash数组管理中
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::SetHandler]nServerID =(%d) is not exist.\n", nServerID));
        return false;
    }

    pClientInfo->SetConnectClient(pConnectClient);
    return true;
}

bool CClientReConnectManager::Close(int nServerID)
{
    //如果是因为服务器断开，则只删除ProConnectClient的指针
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //查找已有连接
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
        return false;
    }

    //关闭链接对象
    if (NULL != pClientInfo->GetConnectClient())
    {
        pClientInfo->GetConnectClient()->ClientClose();
    }

    //从Hash里面删除当前存在的对象
    m_objClientTCPList.Del_Hash_Data(szServerID);

    return true;
}

bool CClientReConnectManager::CloseUDP(int nServerID)
{
    //如果是因为服务器断开，则只删除ProConnectClient的指针
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //查找已有连接
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorUDPClient* pClientInfo = m_objClientUDPList.Get_Hash_Box_Data(szServerID);

    if (NULL == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::CloseUDP]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
        return false;
    }

    pClientInfo->Close();
    SAFE_DELETE(pClientInfo);
    //从hash里面删除当前存在的对象
    m_objClientUDPList.Del_Hash_Data(szServerID);
    return true;
}

bool CClientReConnectManager::ConnectErrorClose(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //查找已有连接
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::ConnectErrorClose]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
        return false;
    }

    //从Hash里面删除当前存在的对象

    SAFE_DELETE(pClientInfo);
    m_objClientTCPList.Del_Hash_Data(szServerID);
    return true;
}

IClientMessage* CClientReConnectManager::GetClientMessage(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //查找已有连接
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL != pClientInfo)
    {
        return pClientInfo->GetClientMessage();
    }

    return NULL;
}

bool CClientReConnectManager::SendData(int nServerID, char*& pData, int nSize, bool blIsDelete)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //查找已有连接
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL == pClientInfo)
    {
        //如果这个链接已经存在，则不创建新的链接
        OUR_DEBUG((LM_ERROR, "[CProConnectManager::SendData]nServerID =(%d) is not exist.\n", nServerID));

        if (true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

    ACE_Message_Block* pmblk = App_MessageBlockManager::instance()->Create(nSize);

    if (NULL == pmblk)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::SendData]nServerID =(%d) pmblk is NULL.\n", nServerID));

        if (true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

    memcpy_safe((char* )pData, (uint32)nSize, pmblk->wr_ptr(), (uint32)nSize);
    pmblk->wr_ptr(nSize);

    if (true == blIsDelete)
    {
        SAFE_DELETE_ARRAY(pData);
    }

    //发送数据
    return pClientInfo->SendData(pmblk);
}

bool CClientReConnectManager::SendDataUDP(int nServerID, const char* pIP, int nPort, const char*& pMessage, uint32 u4Len, bool blIsDelete)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);
    CReactorUDPClient* pClientInfo = m_objClientUDPList.Get_Hash_Box_Data(szServerID);

    if (NULL == pClientInfo)
    {
        //如果这个链接已经存在，则不创建新的链接
        OUR_DEBUG((LM_ERROR, "[CProConnectManager::Close]nServerID =(%d) is not exist.\n", nServerID));

        if (true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pMessage);
        }

        return false;
    }

    //发送数据
    bool blSendRet = pClientInfo->SendMessage(pMessage, u4Len, pIP, nPort);

    if (true == blIsDelete)
    {
        SAFE_DELETE_ARRAY(pMessage);
    }

    return blSendRet;
}

bool CClientReConnectManager::StartConnectTask(int nIntervalTime)
{
    CancelConnectTask();
    m_nTaskID = m_ActiveTimer.schedule(this, (void*)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(nIntervalTime), ACE_Time_Value(nIntervalTime));

    if (m_nTaskID == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectManager::StartConnectTask].StartConnectTask is fail, time is (%d).\n", nIntervalTime));
        return false;
    }

    m_ActiveTimer.activate();
    return true;
}

void CClientReConnectManager::CancelConnectTask()
{
    if (m_nTaskID != -1)
    {
        //杀死之前的定时器，重新开启新的定时器
        m_ActiveTimer.cancel(m_nTaskID);
        m_nTaskID = -1;
    }
}

void CClientReConnectManager::Close()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]Begin.\n"));
    //如果有定时器，则删除定时器
    CancelConnectTask();

    //关闭所有已存在的链接
    vector<CReactorClientInfo*> vecReactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecReactorClientInfo);

    for(int i = 0; i < (int)vecReactorClientInfo.size(); i++)
    {
        CReactorClientInfo* pClientInfo = vecReactorClientInfo[i];

        if(NULL != pClientInfo)
        {
            if (false == pClientInfo->Close())
            {
                OUR_DEBUG((LM_INFO, "[CClientReConnectManager::Close]Close error.\n"));
            }

            SAFE_DELETE(pClientInfo);
        }
    }

    m_objClientTCPList.Close();

    vector<CReactorUDPClient*> vecReactorUDPClient;
    m_objClientUDPList.Get_All_Used(vecReactorUDPClient);

    for(int i = 0; i < (int)vecReactorUDPClient.size(); i++)
    {
        CReactorUDPClient* pClientInfo = vecReactorUDPClient[i];

        if(NULL != pClientInfo)
        {
            pClientInfo->Close();
            SAFE_DELETE(pClientInfo);
        }
    }

    m_objClientUDPList.Close();
    m_u4MaxPoolCount = 0;
    m_ActiveTimer.deactivate();
    OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]End.\n"));
}

bool CClientReConnectManager::ConnectTcpInit(int nServerID, const char* pIP, int nPort, uint8 u1IPType, const char* pLocalIP, int nLocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage, CReactorClientInfo*& pClientInfo)
{
    //查找已有连接
    char szServerID[10] = { '\0' };
    sprintf_safe(szServerID, 10, "%d", nServerID);
    pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL != pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect](%d)pClientInfo is exist.\n", nServerID));
        return false;
    }

    //如果池已经满了，则不能在申请新连接
    if (m_objClientTCPList.Get_Used_Count() == m_objClientTCPList.Get_Count())
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) m_objClientTCPList is full.\n", nServerID));
        return false;
    }

    //初始化链接信息
    pClientInfo = new CReactorClientInfo();

    if (NULL == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]pClientInfo is NULL.\n"));
        return false;
    }

    //添加有效的pClientMessage
    App_ServerMessageTask::instance()->AddClientMessage(pClientMessage);

    if (false == pClientInfo->Init(nServerID, pIP, nPort, u1IPType, &m_ReactorConnect, pClientMessage, m_pReactor))
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect]pClientInfo Init Error.\n"));
        SAFE_DELETE(pClientInfo);

        if (false == Close(nServerID))
        {
            OUR_DEBUG((LM_INFO, "[CClientReConnectManager::ConnectTcpInit]Close error.\n"));
        }

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

bool CClientReConnectManager::ConnectUdpInit(int nServerID, CReactorUDPClient*& pReactorUDPClient)
{
    //查找已有连接
    char szServerID[10] = { '\0' };
    sprintf_safe(szServerID, 10, "%d", nServerID);
    pReactorUDPClient = m_objClientUDPList.Get_Hash_Box_Data(szServerID);

    if (NULL != pReactorUDPClient)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Connect](%d)pClientInfo is exist.\n", nServerID));
        return false;
    }

    //如果池已经满了，则不能在申请新连接
    if (m_objClientUDPList.Get_Used_Count() == m_objClientUDPList.Get_Count())
    {
        OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) m_objClientTCPList is full.\n", nServerID));
        return false;
    }

    pReactorUDPClient = new CReactorUDPClient();

    if (NULL == pReactorUDPClient)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::ConnectUDP]nServerID =(%d) pProactorUDPClient is NULL.\n", nServerID));
        return false;
    }

    //链接已经建立，添加进hash
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
    //ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    if (arg != NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::handle_timeout] arg is not NULL, tv = %d.\n", tv.sec()));
    }

    m_ThreadWritrLock.acquire();
    vector<CReactorClientInfo*> vecCReactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecCReactorClientInfo);
    m_ThreadWritrLock.release();

    for (int i = 0; i < (int)vecCReactorClientInfo.size(); i++)
    {
        //int nServerID = (int)b->first;
        CReactorClientInfo* pClientInfo = vecCReactorClientInfo[i];

        if(NULL != pClientInfo)
        {
            if (NULL == pClientInfo->GetConnectClient())
            {
                //如果连接不存在，则重新建立连接
                if (false == pClientInfo->Run(m_blReactorFinish, SERVER_CONNECT_RECONNECT))
                {
                    OUR_DEBUG((LM_INFO, "[CClientReConnectManager::handle_timeout]Run error.\n"));
                }
            }
            else
            {
                //检查当前连接，是否已挂起或死锁
                ACE_Time_Value tvNow = ACE_OS::gettimeofday();

                //如果是异步模式，则需要检查处理线程是否被挂起
                if(App_MainConfig::instance()->GetConnectServerRunType() == 1)
                {
                    App_ServerMessageTask::instance()->CheckServerMessageThread(tvNow);
                }
                else
                {
                    pClientInfo->GetConnectClient()->GetTimeout(tvNow);
                }
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

    for (int i = 0; i < (int)vecReactorClientInfo.size(); i++)
    {
        CReactorClientInfo* pClientInfo =  vecReactorClientInfo[i];

        if (NULL != pClientInfo)
        {
            if (NULL != pClientInfo->GetConnectClient())
            {
                _ClientConnectInfo ClientConnectInfo = pClientInfo->GetConnectClient()->GetClientConnectInfo();
                ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
                VecClientConnectInfo.push_back(ClientConnectInfo);
            }
            else
            {
                _ClientConnectInfo ClientConnectInfo;
                ClientConnectInfo.m_blValid    = false;
                ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
                VecClientConnectInfo.push_back(ClientConnectInfo);
            }
        }
    }
}

void CClientReConnectManager::GetUDPConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
    VecClientConnectInfo.clear();
    vector<CReactorUDPClient*> vecReactorUDPClient;
    m_objClientUDPList.Get_All_Used(vecReactorUDPClient);

    for (int i = 0; i < (int)vecReactorUDPClient.size(); i++)
    {
        CReactorUDPClient* pClientInfo = vecReactorUDPClient[i];

        if (NULL != pClientInfo)
        {
            _ClientConnectInfo ClientConnectInfo = pClientInfo->GetClientConnectInfo();
            VecClientConnectInfo.push_back(ClientConnectInfo);
        }
    }
}

bool CClientReConnectManager::CloseByClient(int nServerID)
{
    //如果是因为远程连接断开，则只删除ProConnectClient的指针
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
        return false;
    }

    pClientInfo->SetConnectClient(NULL);
    pClientInfo->SetServerConnectState(SERVER_CONNECT_FAIL);

    return true;
}

EM_Server_Connect_State CClientReConnectManager::GetConnectState( int nServerID )
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL == pClientInfo)
    {
        //如果这个链接已经存在，则不创建新的链接
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::GetConnectState]nServerID =(%d) is not exist.\n", nServerID));
        return SERVER_CONNECT_FAIL;
    }

    return pClientInfo->GetServerConnectState();
}

bool CClientReConnectManager::ReConnect(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    char szServerID[10] = {'\0'};
    sprintf_safe(szServerID, 10, "%d", nServerID);

    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL == pClientInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
        return false;
    }

    if (NULL == pClientInfo->GetConnectClient())
    {
        //如果连接不存在，则重新建立连接
        if (false == pClientInfo->Run(m_blReactorFinish, SERVER_CONNECT_RECONNECT))
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

    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL == pClientInfo)
    {
        //如果这个链接不存在，则不创建新的链接
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

    if (NULL == pClientInfo)
    {
        //如果这个链接不存在，则不创建新的链接
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

    CReactorClientInfo* pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (NULL == pClientInfo)
    {
        //如果这个链接不存在，则不创建新的链接
        OUR_DEBUG((LM_ERROR, "[CClientReConnectManager::Close]nServerID =(%d) is not exist.\n", nServerID));
        return false;
    }
    else
    {
        ACE_INET_Addr remote_addr = pClientInfo->GetServerAddr();
        sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_50, remote_addr.get_host_addr());
        objServerIPInfo.m_nPort = remote_addr.get_port_number();
        return true;
    }
}

bool CClientReConnectManager::DeleteIClientMessage(IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //将异步回调有效队列中此pClientMessage设置为无效
    App_ServerMessageTask::instance()->DelClientMessage(pClientMessage);

    //一一寻找与之对应的连接以及相关信息并删除之
    vector<CReactorClientInfo*> vecReactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecReactorClientInfo);

    for(int i = 0; i < (int)vecReactorClientInfo.size(); i++)
    {
        CReactorClientInfo* pClientInfo = vecReactorClientInfo[i];

        if(NULL != pClientInfo && pClientInfo->GetClientMessage() == pClientMessage)
        {
            //关闭连接，并删除对象。
            //关闭链接对象
            if (NULL != pClientInfo->GetConnectClient())
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
