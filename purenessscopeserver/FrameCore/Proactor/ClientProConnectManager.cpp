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
    PSS_LOGGER_DEBUG("[CProactorClientInfo::Init]SetAddrServer({0}:{1}) Begin.", pIP, u2Port);

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
        PSS_LOGGER_DEBUG("[CProactorClientInfo::Init]nServerID = {0}, adrClient({1}:{2}) error.", nServerID, pIP, u2Port);
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
        PSS_LOGGER_DEBUG("[CProactorClientInfo::Run]Connect is exist.");
        return false;
    }

    if(nullptr == m_pProAsynchConnect)
    {
        PSS_LOGGER_DEBUG("[CProactorClientInfo::Run]m_pAsynchConnect is nullptr.");
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
                PSS_LOGGER_DEBUG("[CProactorClientInfo::Run]m_pAsynchConnect open error({0}).", ACE_OS::last_error());
                return false;
            }
        }
        else
        {
            if (m_pProAsynchConnect->connect(m_AddrServer, (ACE_INET_Addr&)ACE_Addr::sap_any, 1, (const void*)pProConnectInfo) == -1)
            {
                PSS_LOGGER_DEBUG("[CProactorClientInfo::Run]m_pAsynchConnect open error({0}).", ACE_OS::last_error());
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
        //如果连接正在建立过程中，等待5毫秒，如果
        if(EM_Server_Connect_State::SERVER_CONNECT_FIRST == m_emConnectState 
            || EM_Server_Connect_State::SERVER_CONNECT_RECONNECT == m_emConnectState)
        {
            return false;
        }

        if(nullptr == m_pProConnectClient)
        {
            //发送连接建立无信息
            Common_Send_ConnectError(pmblk, m_AddrServer, m_pClientMessage);

            if (EM_Server_Connect_State::SERVER_CONNECT_FIRST != m_emConnectState
                && EM_Server_Connect_State::SERVER_CONNECT_RECONNECT != m_emConnectState
                && false == Run(true, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
            {
                //如果连接不存在，则建立链接。
                PSS_LOGGER_DEBUG("[CProactorClientInfo::SendData]Run error.");
            }

            return false;
        }
    }

    if(true == m_pClientMessage->Need_Send_Format())
    {
        //调用数据发送组装
        ACE_Message_Block* pSend = nullptr;

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
    //这里增加是否是连接重练的判定以及是否是第一次连接的回调
    if((m_emConnectState == EM_Server_Connect_State::SERVER_CONNECT_RECONNECT || m_emConnectState == EM_Server_Connect_State::SERVER_CONNECT_FIRST) && nullptr != m_pClientMessage)
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
    m_blProactorFinish       = false;
    m_u4MaxPoolCount         = 0;
    m_u4ConnectServerTimeout = 0;
    m_emS2SRunState          = EM_S2S_Run_State::S2S_Run_State_Init;
}

CClientProConnectManager::~CClientProConnectManager(void)
{
    Close();
}

bool CClientProConnectManager::Init(ACE_Proactor* pProactor)
{
    if(pProactor == nullptr)
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::Init]pProactor is nullptr.");
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
        PSS_LOGGER_DEBUG("[CClientProConnectManager::Init]m_ProAsynchConnect open error({0}).", ACE_OS::last_error());
        return false;
    }

    //标记Proactor已经连接成功
    m_blProactorFinish = true;

    m_emS2SRunState = EM_S2S_Run_State::S2S_Run_State_Run;

    //将之前有缓冲的连接信息建立连接
    for (CS2SConnectGetRandyInfo f : m_GetReadyInfoList)
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

bool CClientProConnectManager::Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, IClientMessage* pClientMessage, uint32 u4PacketParseID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CProactorClientInfo* pClientInfo = nullptr;

    if (EM_S2S_Run_State::S2S_Run_State_Init == m_emS2SRunState)
    {
        //如果反应器还没初始化，添加到一个列表中，等待反应器初始化后调用
        CS2SConnectGetRandyInfo objS2SConnectGetRandyInfo;

        objS2SConnectGetRandyInfo.m_nServerID       = nServerID;
        objS2SConnectGetRandyInfo.m_strServerIP     = pIP;
        objS2SConnectGetRandyInfo.m_u2ServerPort    = u2Port;
        objS2SConnectGetRandyInfo.m_u1Type          = u1IPType;
        objS2SConnectGetRandyInfo.m_pClientMessage  = pClientMessage;
        objS2SConnectGetRandyInfo.m_u4PacketParseID = u4PacketParseID;
        m_GetReadyInfoList.emplace_back(objS2SConnectGetRandyInfo);

        return true;
    }

    //连接初始化动作
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, nullptr, 0, u1IPType, pClientMessage, pClientInfo, u4PacketParseID))
    {
        return false;
    }

    //第一次开始链接
    if(false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    PSS_LOGGER_DEBUG("[CClientProConnectManager::Connect]nServerID =({0}) connect is OK.", nServerID);

    return true;
}

bool CClientProConnectManager::Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage, uint32 u4PacketParseID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CProactorClientInfo* pClientInfo = nullptr;

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
        m_GetReadyInfoList.emplace_back(objS2SConnectGetRandyInfo);

        return true;
    }

    //连接初始化动作
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, pLocalIP, u2LocalPort, u1LocalIPType, pClientMessage, pClientInfo, u4PacketParseID))
    {
        return false;
    }

    //第一次开始链接
    if(false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    PSS_LOGGER_DEBUG("[CClientProConnectManager::Connect]nServerID =({0}) connect is OK.", nServerID);

    return true;
}

bool CClientProConnectManager::ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, uint32 u4PacketParse)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CProactorClientInfo* pClientInfo = nullptr;

    //连接初始化动作
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, nullptr, 0, u1IPType, nullptr, pClientInfo, u4PacketParse))
    {
        return false;
    }

    //第一次开始链接
    if (false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    PSS_LOGGER_DEBUG("[CClientProConnectManager::Connect]nServerID =({0}) connect is OK.", nServerID);

    return true;
}

bool CClientProConnectManager::ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, uint32 u4PacketParse)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CProactorClientInfo* pClientInfo = nullptr;

    //连接初始化动作
    if (false == ConnectTcpInit(nServerID, pIP, u2Port, u1IPType, pLocalIP, u2LocalPort, u1LocalIPType, nullptr, pClientInfo, u4PacketParse))
    {
        return false;
    }

    //第一次开始链接
    if (false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_FIRST))
    {
        SAFE_DELETE(pClientInfo);
        return false;
    }

    PSS_LOGGER_DEBUG("[CClientProConnectManager::Connect]nServerID =({0}) connect is OK.", nServerID);

    return true;
}

bool CClientProConnectManager::ConnectUDP(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, EM_UDP_TYPE emType, IClientUDPMessage* pClientUDPMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
    CProactorUDPClient* pProactorUDPClient = nullptr;

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
        AddrLocal.set(u2Port, PSS_INADDR_ANY);
    }

    if (nErr != 0)
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::ConnectUDP]({0})UDP set_address error[{1}].", nServerID, errno);
        SAFE_DELETE(pProactorUDPClient);
        return false;
    }

    //连接UDP
    if(0 != pProactorUDPClient->OpenAddress(AddrLocal, emType, App_ProactorManager::instance()->GetAce_Proactor(), pClientUDPMessage))
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::ConnectUDP]({0})UDP OpenAddress error.", nServerID);
        SAFE_DELETE(pProactorUDPClient);
        return false;
    }

    return true;
}

bool CClientProConnectManager::SetHandler(int nServerID, CProConnectClient* pProConnectClient)
{
    if(nullptr == pProConnectClient)
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::SetHandler]pProConnectClient is nullptr.");
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
        //如果这个链接不存在
        PSS_LOGGER_DEBUG("[CClientProConnectManager::SetHandler]nServerID =({0}) is not exist.", nServerID);
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

    if(nullptr == pClientInfo)
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::Close]nServerID =({0}) pClientInfo is nullptr.", nServerID);
        return false;
    }

    EM_s2s ems2s = EM_s2s::S2S_INNEED_CALLBACK;

    //关闭链接对象
    if(nullptr != pClientInfo->GetProConnectClient())
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

    if(nullptr == pClientInfo)
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::CloseUDP]nServerID =({0}) pClientInfo is nullptr.", nServerID);
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

    if(nullptr == pClientInfo)
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::ConnectErrorClose]nServerID =({0}) pClientInfo is nullptr.", nServerID);
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
        //如果这个链接不存在，则不创建新的链接
        PSS_LOGGER_DEBUG("[CClientProConnectManager::SendData]nServerID =({0}) is not exist.", nServerID);

        if(true == blIsDelete)
        {
            SAFE_DELETE_ARRAY(pData);
        }

        return false;
    }

    ACE_Message_Block* pmblk = App_MessageBlockManager::instance()->Create(nSize);

    if(nullptr == pmblk)
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::SendData]nServerID =({0}) pmblk is nullptr.", nServerID);

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

    //发送数据
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
        //如果这个链接不存在，则不创建新的链接
        PSS_LOGGER_DEBUG("[CClientProConnectManager::SendDataUDP]nServerID =({0}) is not exist.", nServerID);

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

bool CClientProConnectManager::StartConnectTask(uint16 u2IntervalTime)
{
    CancelConnectTask();

    m_u2ThreadTimeCheck = u2IntervalTime;

    m_blTimerState = true;

    timer_task(App_TimerManager::instance()->GetTimerPtr());

    return true;
}

void CClientProConnectManager::CancelConnectTask()
{
    m_blTimerState = false;
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

int CClientProConnectManager::timer_task(brynet::TimerMgr::Ptr timerMgr)
{
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
                //如果连接不存在，则重新建立连接
                if (false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
                {
                    PSS_LOGGER_DEBUG("[CClientProConnectManager::handle_timeout]Run is fail.");
                }
            }
        }
    }

    if (true == m_blTimerState)
    {
        start_new_task(timerMgr);
    }

    return 0;
}

void CClientProConnectManager::start_new_task(brynet::TimerMgr::Ptr timerMgr)
{
    PSS_LOGGER_DEBUG("[CClientProConnectManager::start_new_task]new timer is set({0}).", m_u2ThreadTimeCheck);
    auto timer = timerMgr->addTimer(std::chrono::seconds(m_u2ThreadTimeCheck), [this, timerMgr]() {
        timer_task(timerMgr);
        });
}

bool CClientProConnectManager::ConnectTcpInit(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, int nLocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage, CProactorClientInfo*& pClientInfo, uint32 u4PacketParseID)
{
    char szServerID[10] = { '\0' };
    sprintf_safe(szServerID, 10, "%d", nServerID);
    pClientInfo = m_objClientTCPList.Get_Hash_Box_Data(szServerID);

    if (nullptr != pClientInfo)
    {
        //如果这个链接已经存在，则不创建新的链接
        PSS_LOGGER_DEBUG("[CClientProConnectManager::Connect]nServerID =({0}) is exist.", nServerID);
        return false;
    }

    //如果池已经满了，则不能在申请新连接
    if (m_objClientTCPList.Get_Used_Count() == m_objClientTCPList.Get_Count())
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::Connect]nServerID =({0}) m_objClientTCPList is full.", nServerID);
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
    if (nullptr != pLocalIP && nLocalPort > 0)
    {
        pClientInfo->SetLocalAddr(pLocalIP, nLocalPort, u1LocalIPType);
    }

    //添加进hash
    if (-1 == m_objClientTCPList.Add_Hash_Data(szServerID, pClientInfo))
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::Connect]nServerID =({0}) add m_objClientTCPList is fail.", nServerID);
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
        //如果这个链接已经存在，则不创建新的链接
        PSS_LOGGER_DEBUG("[CClientProConnectManager::ConnectUDP]nServerID =({0}) is exist.", nServerID);
        return false;
    }

    //如果池已经满了，则不能在申请新连接
    if (m_objClientUDPList.Get_Used_Count() == m_objClientUDPList.Get_Count())
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::ConnectUDP]nServerID =({0}) m_objClientTCPList is full.", nServerID);
        return false;
    }

    pProactorUDPClient = new CProactorUDPClient();

    if (nullptr == pProactorUDPClient)
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::ConnectUDP]nServerID =({0}) pProactorUDPClient is nullptr.\n", nServerID);
        return false;
    }

    //链接已经建立，添加进hash
    if (-1 == m_objClientUDPList.Add_Hash_Data(szServerID, pProactorUDPClient))
    {
        PSS_LOGGER_DEBUG("[CClientProConnectManager::Connect]nServerID =({0}) add m_objClientTCPList is fail.", nServerID);
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
        if(nullptr != pClientInfo)
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

    if(nullptr == pClientInfo)
    {
        return false;
    }

    if(nullptr == pClientInfo->GetProConnectClient())
    {
        //如果连接不存在，则重新建立连接
        if (false == pClientInfo->Run(m_blProactorFinish, EM_Server_Connect_State::SERVER_CONNECT_RECONNECT))
        {
            PSS_LOGGER_DEBUG("[CClientProConnectManager::ReConnect]Run is fail.");
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
    //检查当前连接是否是活跃的
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
    //检查当前连接是否是活跃的
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

    //一一寻找与之对应的连接以及相关信息并删除之
    vector<CProactorClientInfo*> vecProactorClientInfo;
    m_objClientTCPList.Get_All_Used(vecProactorClientInfo);

    for(auto* pClientInfo : vecProactorClientInfo)
    {
        if(nullptr != pClientInfo && pClientInfo->GetClientMessage() == pClientMessage)
        {
            //关闭连接，并删除对象。
            //关闭链接对象
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
