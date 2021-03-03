#include "ClientReConnectManager.h"

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
        PSS_LOGGER_DEBUG("[CReactorClientInfo::Init]adrClient({0}:{1}) error.", pIP, u2Port);
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
        PSS_LOGGER_DEBUG("[CReactorClientInfo::Run]Connect is exist.");
        return false;
    }

    if (nullptr == m_pReactorConnect)
    {
        PSS_LOGGER_DEBUG("[CReactorClientInfo::Run]m_pAsynchConnect is nullptr.");
        return false;
    }

    m_pConnectClient = new CConnectClient();

    if (nullptr == m_pConnectClient)
    {
        PSS_LOGGER_DEBUG("[CReactorClientInfo::Run]pConnectClient new is nullptr({0}).", emState);
        return false;
    }

    //配置连接对象设置参数
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
            //需要指定接入端口
            if (m_pReactorConnect->connect(m_pConnectClient, m_AddrServer, ACE_Synch_Options::defaults, m_AddrLocal) == -1)
            {
                m_emConnectState = EM_Server_Connect_State::SERVER_CONNECT_FAIL;
                PSS_LOGGER_DEBUG("[CReactorClientInfo::Run]({0}:{1}) connection fails(ServerID={2}) error({3}).", m_AddrServer.get_host_addr(), m_AddrServer.get_port_number(), m_nServerID, ACE_OS::last_error());
                //这里设置为True，为了让自动重试起作用
                return true;
            }
        }
        else
        {
            //不需要指定接入端口
            if (m_pReactorConnect->connect(m_pConnectClient, m_AddrServer) == -1)
            {
                m_emConnectState = EM_Server_Connect_State::SERVER_CONNECT_FAIL;
                PSS_LOGGER_DEBUG("[CReactorClientInfo::Run]({0}:{1}) connection fails(ServerID={2}) error({3}).", m_AddrServer.get_host_addr(), m_AddrServer.get_port_number(), m_nServerID, ACE_OS::last_error());
                //这里设置为True，为了让自动重试起作用
                return true;
            }
            else
            {
                PSS_LOGGER_DEBUG("[CReactorClientInfo::Run]({0}:{1}) connection ok(ServerID={2}) OK.", m_AddrServer.get_host_addr(), m_AddrServer.get_port_number(), m_nServerID);
            }
        }

        m_emConnectState = EM_Server_Connect_State::SERVER_CONNECT_OK;
        //通知上层已经链接
        m_pClientMessage->ReConnect(m_nServerID);
    }

    return true;
}

bool CReactorClientInfo::SendData(ACE_Message_Block* pmblk)
{
    if (nullptr == m_pConnectClient)
    {
        //如果连接正在建立过程中，等待5毫秒，如果
        if(EM_Server_Connect_State::SERVER_CONNECT_FIRST == m_emConnectState 
            || EM_Server_Connect_State::SERVER_CONNECT_RECONNECT == m_emConnectState)
        {
            return false;
        }

        if (nullptr == m_pConnectClient)
        {
            //发送连接建立无信息
            Common_Send_ConnectError(pmblk, m_AddrServer, m_pClientMessage);

            if (EM_Server_Connect_State::SERVER_CONNECT_FIRST != m_emConnectState
                && EM_Server_Connect_State::SERVER_CONNECT_RECONNECT != m_emConnectState
                && false == Run(true, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
            {
                PSS_LOGGER_DEBUG("[CReactorClientInfo::SendData]Run error.");
            }

            return false;
        }
    }

    if(true == m_pClientMessage->Need_Send_Format())
    {
        //调用数据发送组装
        ACE_Message_Block* pSend = nullptr;

        if (false == Common_Send_Data(pmblk, m_pClientMessage, pSend))
        {
            return false;
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
        PSS_LOGGER_DEBUG("[CReactorClientInfo::SetLocalAddr]AddrLocal({0}:{1}) error.", pIP, u2Port);
        return;
    }

    m_blIsLocal = true;
}

bool CClientReConnectManager::Init(ACE_Reactor* pReactor)
{
    if (-1 == m_ReactorConnect.open(pReactor))
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Init]open error({0}).", ACE_OS::last_error());
        return false;
    }

    //记录缓冲池的最大上限
    m_u4MaxPoolCount = GetXmlConfigAttribute(xmlConnectServer)->Count;

    m_u4ConnectServerTimeout = GetXmlConfigAttribute(xmlConnectServer)->TimeInterval * 1000; //转换为微妙

    if (m_u4ConnectServerTimeout == 0)
    {
        m_u4ConnectServerTimeout = RE_CONNECT_SERVER_TIMEOUT;
    }

    m_pReactor        = pReactor;

    m_emS2SRunState = EM_S2S_Run_State::S2S_Run_State_Run;

    //将之前有缓冲的连接信息建立连接
    for(CS2SConnectGetRandyInfo f : m_GetReadyInfoList)
    {
        if (f.m_u2LocalPort == 0)
        {
            Connect(f.m_nServerID,
                    f.m_strServerIP.c_str(),
                    f.m_u2ServerPort,
                    f.m_u1Type,
                    f.m_pClientMessage,
                f.m_u4PacketParseID);
        }
        else
        {
            Connect(f.m_nServerID,
                    f.m_strServerIP.c_str(),
                    f.m_u2ServerPort,
                    f.m_u1Type,
                    f.m_strLocalIP.c_str(),
                    f.m_u2LocalPort,
                    f.m_u1LocalIPType,
                    f.m_pClientMessage, 
                f.m_u4PacketParseID);
        }
    }

    m_GetReadyInfoList.clear();

    return true;
}

bool CClientReConnectManager::Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, IClientMessage* pClientMessage, uint32 u4PacketParseID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    if (EM_S2S_Run_State::S2S_Run_State_Init == m_emS2SRunState)
    {
        //如果反应器还没初始化，添加到一个列表中，等待反应器初始化后调用
        CS2SConnectGetRandyInfo objS2SConnectGetRandyInfo;

        objS2SConnectGetRandyInfo.m_strServerIP     = pIP;
        objS2SConnectGetRandyInfo.m_nServerID       = nServerID;
        objS2SConnectGetRandyInfo.m_u2ServerPort    = u2Port;
        objS2SConnectGetRandyInfo.m_u1Type          = u1IPType;
        objS2SConnectGetRandyInfo.m_pClientMessage  = pClientMessage;
        objS2SConnectGetRandyInfo.m_u4PacketParseID = u4PacketParseID;
        m_GetReadyInfoList.emplace_back(objS2SConnectGetRandyInfo);

        return true;
    }

    //连接初始化动作
    auto pClientInfo = ConnectTcpInit(nServerID);

    if(nullptr == pClientInfo)
    {
        return false;
    }

    if (false == pClientInfo->Init(nServerID, pIP, u2Port, u1IPType, &m_ReactorConnect, pClientMessage, m_pReactor, u4PacketParseID))
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]pClientInfo Init Error.");

        if (false == Close(nServerID))
        {
            PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectTcpInit]Close error.");
        }

        return false;
    }

    //添加进hash
    m_objClientTCPList[nServerID] = pClientInfo;

    //开始链接
    if (false == pClientInfo->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]Run Error.");
        pClientInfo = nullptr;

        if (false == Close(nServerID))
        {
            PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]Close Error.");
        }

        return false;
    }

    PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]nServerID =({0}) connect is OK.", nServerID);
    return true;
}

bool CClientReConnectManager::Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage, uint32 u4PacketParseID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    if (EM_S2S_Run_State::S2S_Run_State_Init == m_emS2SRunState)
    {
        //如果反应器还没初始化，添加到一个列表中，等待反应器初始化后调用
        CS2SConnectGetRandyInfo objS2SConnectGetRandyInfo;

        objS2SConnectGetRandyInfo.m_nServerID       = nServerID;
        objS2SConnectGetRandyInfo.m_strServerIP     = pIP;
        objS2SConnectGetRandyInfo.m_u2ServerPort    = u2Port;
        objS2SConnectGetRandyInfo.m_u1Type          = u1IPType;
        objS2SConnectGetRandyInfo.m_strLocalIP      = pLocalIP;
        objS2SConnectGetRandyInfo.m_u2LocalPort     = u2LocalPort;
        objS2SConnectGetRandyInfo.m_u1LocalIPType   = u1LocalIPType;
        objS2SConnectGetRandyInfo.m_pClientMessage  = pClientMessage;
        objS2SConnectGetRandyInfo.m_u4PacketParseID = u4PacketParseID;
        m_GetReadyInfoList.push_back(objS2SConnectGetRandyInfo);

        return true;
    }

    //连接初始化动作
    auto pClientInfo = ConnectTcpInit(nServerID);
    
    if(nullptr == pClientInfo)
    {
        return false;
    }

    if (false == pClientInfo->Init(nServerID, pIP, u2Port, u1IPType, &m_ReactorConnect, pClientMessage, m_pReactor, u4PacketParseID))
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]pClientInfo Init Error.");

        if (false == Close(nServerID))
        {
            PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectTcpInit]Close error.");
        }

        return false;
    }

    //设置本地IP和端口
    pClientInfo->SetLocalAddr(pLocalIP, u2LocalPort, u1LocalIPType);

    //添加进hash
    m_objClientTCPList[nServerID] = pClientInfo;

    //开始链接
    if (false == pClientInfo->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]Run Error.");

        if (false == Close(nServerID))
        {
            PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]Close Error.");
        }

        return false;
    }

    PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]nServerID =({0}) connect is OK.", nServerID);
    return true;
}

bool CClientReConnectManager::ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, uint32 u4PacketParseID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //连接初始化动作
    auto pClientInfo = ConnectTcpInit(nServerID);

    if(nullptr == pClientInfo)
    {
        return false;
    }

    if (false == pClientInfo->Init(nServerID, pIP, u2Port, u1IPType, &m_ReactorConnect, nullptr, m_pReactor, u4PacketParseID))
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]pClientInfo Init Error.");

        if (false == Close(nServerID))
        {
            PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectTcpInit]Close error.");
        }

        return false;
    }

    //添加进hash
    m_objClientTCPList[nServerID] = pClientInfo;

    //开始链接
    if (false == pClientInfo->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectFrame]Run Error.");

        if (false == Close(nServerID))
        {
            PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectFrame]Close Error.");
        }

        return false;
    }

    PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectFrame]nServerID =({0}) connect is OK.", nServerID);
    return true;
}

bool CClientReConnectManager::ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, uint32 u4PacketParseID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //连接初始化动作
    auto pClientInfo = ConnectTcpInit(nServerID);
    
    if(nullptr == pClientInfo)
    {
        return false;
    }

    if (false == pClientInfo->Init(nServerID, pIP, u2Port, u1IPType, &m_ReactorConnect, nullptr, m_pReactor, u4PacketParseID))
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]pClientInfo Init Error.");

        if (false == Close(nServerID))
        {
            PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectTcpInit]Close error.");
        }

        return false;
    }

    //设置本地IP和端口
    if (nullptr != pLocalIP && u2LocalPort > 0)
    {
        pClientInfo->SetLocalAddr(pLocalIP, u2LocalPort, u1LocalIPType);
    }

    //添加进hash
    m_objClientTCPList[nServerID] = pClientInfo;

    //开始链接
    if (false == pClientInfo->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectFrame]Run Error.");

        if (false == Close(nServerID))
        {
            PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectFrame]Close Error.");
        }

        return false;
    }

    PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectFrame]nServerID =({0}) connect is OK.", nServerID);
    return true;
}

bool CClientReConnectManager::ConnectUDP(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, EM_UDP_TYPE emType, IClientUDPMessage* pClientUDPMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //初始化连接动作
    auto pReactorUDPClient = ConnectUdpInit(nServerID);

    if(nullptr == pReactorUDPClient)
    {
        return false;
    }

    //初始化连接地址
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
        AddrLocal.set(u2Port, PSS_INADDR_ANY);
    }

    if (nErr != 0)
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectUDP]({0})UDP set_address error[{1}].", nServerID, errno);
        return false;
    }

    //开始连接
    if (0 != pReactorUDPClient->OpenAddress(AddrLocal, emType, App_ReactorManager::instance()->GetAce_Reactor(REACTOR_UDPDEFINE), pClientUDPMessage))
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectUDP]({0})UDP OpenAddress error.", nServerID);
        return false;
    }

    //链接已经建立，添加进hash
    m_objClientUDPList[nServerID] = pReactorUDPClient;

    return true;
}

bool CClientReConnectManager::SetHandler(int nServerID, CConnectClient* pConnectClient)
{
    if (nullptr == pConnectClient)
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::SetHandler]pProConnectClient is nullptr.");
        return false;
    }

    //查找已有连接
    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() == f)
    {
        //如果这个链接已经存在，则不再添加到已经存在的客户端Hash数组管理中
        PSS_LOGGER_DEBUG("[CClientReConnectManager::SetHandler]nServerID =({0}) is not exist.", nServerID);
        return false;
    }

    auto pClientInfo = f->second;

    pClientInfo->SetConnectClient(pConnectClient);
    return true;
}

bool CClientReConnectManager::Close(int nServerID)
{
    //如果是因为服务器断开，则只删除ProConnectClient的指针
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //查找已有连接
    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() == f)
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Close]nServerID =({0}) pClientInfo is nullptr.", nServerID);
        return false;
    }

    auto pClientInfo = f->second;

    //关闭链接对象
    if (nullptr != pClientInfo->GetConnectClient())
    {
        pClientInfo->GetConnectClient()->ClientClose();
    }

    //从Hash里面删除当前存在的对象
    m_objClientTCPList.erase(f);

    return true;
}

bool CClientReConnectManager::CloseUDP(int nServerID)
{
    //如果是因为服务器断开，则只删除ProConnectClient的指针
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //查找已有连接
    auto f = m_objClientUDPList.find(nServerID);

    if (m_objClientUDPList.end() == f)
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::CloseUDP]nServerID =({0}) pClientInfo is nullptr.", nServerID);
        return false;
    }

    auto pClientInfo = f->second;

    pClientInfo->Close();
    //从hash里面删除当前存在的对象
    m_objClientUDPList.erase(f);
    return true;
}

bool CClientReConnectManager::ConnectErrorClose(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //查找已有连接
    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() == f)
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::ConnectErrorClose]nServerID =({0}) pClientInfo is nullptr.", nServerID);
        return false;
    }

    //从Hash里面删除当前存在的对象

    m_objClientTCPList.erase(f);
    return true;
}

IClientMessage* CClientReConnectManager::GetClientMessage(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //查找已有连接
    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() != f)
    {
        return f->second->GetClientMessage();
    }

    return nullptr;
}

bool CClientReConnectManager::SendData(int nServerID, char*& pData, int nSize, bool blIsDelete)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    //查找已有连接
    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() == f)
    {
        //如果这个链接不存在，则不创建新的链接
        PSS_LOGGER_DEBUG("[CProConnectManager::SendData]nServerID =({0}) is not exist.", nServerID);

        if (true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

    auto pClientInfo = f->second;

    ACE_Message_Block* pmblk = App_MessageBlockManager::instance()->Create(nSize);

    if (nullptr == pmblk)
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::SendData]nServerID =({0}) pmblk is nullptr.", nServerID);

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

    //发送数据
    return pClientInfo->SendData(pmblk);
}

bool CClientReConnectManager::SendDataUDP(int nServerID, const char* pIP, uint16 u2Port, char*& pMessage, uint32 u4Len, bool blIsDelete)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    auto f = m_objClientUDPList.find(nServerID);

    if (m_objClientUDPList.end() == f)
    {
        //如果这个链接已经存在，则不创建新的链接
        PSS_LOGGER_DEBUG("[CProConnectManager::Close]nServerID =({0}) is not exist.", nServerID);

        if (true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pMessage);
        }

        return false;
    }

    auto pClientInfo = f->second;

    //发送数据
    bool blSendRet = pClientInfo->SendMessage(pMessage, u4Len, pIP, u2Port);

    if (true == blIsDelete)
    {
        SAFE_DELETE_ARRAY(pMessage);
    }

    return blSendRet;
}

bool CClientReConnectManager::StartConnectTask(uint16 u2IntervalTime)
{
    CancelConnectTask();

    m_u2ThreadTimeCheck = u2IntervalTime;
    m_blReactorFinish   = true;
    m_blTimerState      = true;

    timer_task(App_TimerManager::instance()->GetTimerPtr());

    return true;
}

void CClientReConnectManager::CancelConnectTask()
{
    m_blTimerState = false;
}

void CClientReConnectManager::Close()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    PSS_LOGGER_DEBUG("[CClientReConnectManager::Close]Begin.");
    //如果有定时器，则删除定时器
    CancelConnectTask();

    PSS_LOGGER_DEBUG("[CClientReConnectManager::Close]CancelConnectTask.");

    //关闭所有已存在的链接
    for_each(m_objClientTCPList.begin(), m_objClientTCPList.end(), [](const std::pair<int, shared_ptr<CReactorClientInfo>>& iter) {
        iter.second->GetConnectClient()->ClientClose();
        });

    PSS_LOGGER_DEBUG("[CClientReConnectManager::Close]TCP Close.");

    m_objClientTCPList.clear();

    for_each(m_objClientUDPList.begin(), m_objClientUDPList.end(), [](const std::pair<int, shared_ptr<CReactorUDPClient>>& iter) {
        iter.second->Close();
        });

    PSS_LOGGER_DEBUG("[CClientReConnectManager::Close]UDP Close.");
    
    m_objClientUDPList.clear();

    std::this_thread::sleep_for(chrono::seconds(1));

    m_u4MaxPoolCount = 0;

    PSS_LOGGER_DEBUG("[CClientReConnectManager::Close]End.");
}

shared_ptr<CReactorClientInfo> CClientReConnectManager::ConnectTcpInit(int nServerID)
{
    //查找已有连接
    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() != f)
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]({0})pClientInfo is exist.", nServerID);
        return nullptr;
    }

    //如果池已经满了，则不能在申请新连接
    if ((uint32)m_objClientTCPList.size() == m_u4MaxPoolCount)
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::Connect]nServerID =({0}) m_objClientTCPList is full.", nServerID);
        return nullptr;
    }

    //初始化链接信息
    auto pClientInfo = std::make_shared<CReactorClientInfo>();

    return pClientInfo;
}

shared_ptr<CReactorUDPClient> CClientReConnectManager::ConnectUdpInit(int nServerID)
{
    //查找已有连接
    auto f = m_objClientUDPList.find(nServerID);

    if (m_objClientUDPList.end() != f)
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Connect]({0})pClientInfo is exist.", nServerID);
        return nullptr;
    }

    //如果池已经满了，则不能在申请新连接
    if ((uint32)m_objClientUDPList.size() == m_u4MaxPoolCount)
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::ConnectUdpInit]nServerID =({0}) <{1}>m_objClientUDPList is full.", nServerID, m_u4MaxPoolCount);
        return nullptr;
    }

    auto pReactorUDPClient = std::make_shared<CReactorUDPClient>();

    return pReactorUDPClient;
}

int CClientReConnectManager::timer_task(brynet::TimerMgr::Ptr timerMgr)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    
    for_each(m_objClientTCPList.begin(), m_objClientTCPList.end(), [this](const std::pair<int, shared_ptr<CReactorClientInfo>>& iter) {
        auto pClientInfo = iter.second;

        if (nullptr == pClientInfo->GetConnectClient())
        {
            //如果连接不存在，则重新建立连接
            pClientInfo->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT);

        }
        });

    if (true == m_blTimerState)
    {
        start_new_task(timerMgr);
    }

    return 0;
}

void CClientReConnectManager::start_new_task(brynet::TimerMgr::Ptr timerMgr)
{
    PSS_LOGGER_DEBUG("[CClientReConnectManager::start_new_task]new timer is set.");
    auto timer = timerMgr->addTimer(std::chrono::seconds(m_u2ThreadTimeCheck), [this, timerMgr]() {
        timer_task(timerMgr);
        });
}

void CClientReConnectManager::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    VecClientConnectInfo.clear();

    for_each(m_objClientTCPList.begin(), m_objClientTCPList.end(), [&VecClientConnectInfo](const std::pair<int, shared_ptr<CReactorClientInfo>>& iter) {
        auto pClientInfo = iter.second;
        if (nullptr != pClientInfo->GetConnectClient())
        {
            _ClientConnectInfo ClientConnectInfo = pClientInfo->GetConnectClient()->GetClientConnectInfo();
            ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
            VecClientConnectInfo.emplace_back(ClientConnectInfo);
        }
        });
}

void CClientReConnectManager::GetUDPConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    VecClientConnectInfo.clear();

    for_each(m_objClientUDPList.begin(), m_objClientUDPList.end(), [&VecClientConnectInfo](const std::pair<int, shared_ptr<CReactorUDPClient>>& iter) {
        auto pClientInfo = iter.second;
        _ClientConnectInfo ClientConnectInfo = pClientInfo->GetClientConnectInfo();
        VecClientConnectInfo.emplace_back(ClientConnectInfo);
        });
}

bool CClientReConnectManager::CloseByClient(int nServerID)
{
    //如果是因为远程连接断开，则只删除ProConnectClient的指针
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() != f)
    {
        f->second->SetConnectClient(nullptr);
        f->second->SetServerConnectState(EM_Server_Connect_State::SERVER_CONNECT_FAIL);
    }

    return true;
}

EM_Server_Connect_State CClientReConnectManager::GetConnectState( int nServerID )
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() == f)
    {
        //如果这个链接已经存在，则不创建新的链接
        PSS_LOGGER_DEBUG("[CClientReConnectManager::GetConnectState]nServerID =({0}) is not exist.", nServerID);
        return EM_Server_Connect_State::SERVER_CONNECT_FAIL;
    }

    return f->second->GetServerConnectState();
}

uint32 CClientReConnectManager::GetPacketParseID(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() != f)
    {
        return f->second->GetPacketParseID();
    }

    return 0;
}

bool CClientReConnectManager::ReConnect(int nServerID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    auto f = m_objClientTCPList.find(nServerID);
    if (m_objClientTCPList.end() == f)
    {
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Close]nServerID =({0}) pClientInfo is nullptr.", nServerID);
        return false;
    }

    if (nullptr == f->second->GetConnectClient())
    {
        //如果连接不存在，则重新建立连接
        if (false == f->second->Run(m_blReactorFinish, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
        {
            PSS_LOGGER_DEBUG("[CClientReConnectManager::Close]Run error.");
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

    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() == f)
    {
        //如果这个链接不存在，则不创建新的链接
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Close]nServerID =({0}) is not exist.", nServerID);
        return remote_addr;
    }
    else
    {
        remote_addr = f->second->GetServerAddr();
        return remote_addr;
    }
}

bool CClientReConnectManager::SetServerConnectState(int nServerID, EM_Server_Connect_State objState)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() == f)
    {
        //如果这个链接不存在，则不创建新的链接
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Close]nServerID =({0}) is not exist.", nServerID);
        return false;
    }
    else
    {
        f->second->SetServerConnectState(objState);
        return true;
    }
}

bool CClientReConnectManager::GetServerIPInfo( int nServerID, _ClientIPInfo& objServerIPInfo )
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    auto f = m_objClientTCPList.find(nServerID);

    if (m_objClientTCPList.end() == f)
    {
        //如果这个链接不存在，则不创建新的链接
        PSS_LOGGER_DEBUG("[CClientReConnectManager::Close]nServerID =({0}) is not exist.", nServerID);
        return false;
    }
    else
    {
        ACE_INET_Addr remote_addr     = f->second->GetServerAddr();
        objServerIPInfo.m_strClientIP = remote_addr.get_host_addr();
        objServerIPInfo.m_u2Port      = remote_addr.get_port_number();
        return true;
    }
}

bool CClientReConnectManager::DeleteIClientMessage(IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //一一寻找与之对应的连接以及相关信息并删除之
    for (auto it = m_objClientTCPList.begin(); it != m_objClientTCPList.end();)
    {
        auto pClientInfo = it->second;
        if (nullptr != pClientInfo && pClientInfo->GetClientMessage() == pClientMessage)
        {
            //关闭连接，并删除对象。
            //关闭链接对象
            if (nullptr != pClientInfo->GetConnectClient())
            {
                pClientInfo->GetConnectClient()->ClientClose();
            }

            m_objClientTCPList.erase(it);
            break;
        }
    }

    return true;
}
