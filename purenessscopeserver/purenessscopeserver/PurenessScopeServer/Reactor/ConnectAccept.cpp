#include "ConnectAccept.h"

ConnectAcceptor::ConnectAcceptor()
{
    m_u4AcceptCount        = 0;
    m_u4ClientReactorCount = 1;
    m_szListenIP[0]        = '\0';
    m_u4Port               = 0;
    m_u4PacketParseInfoID  = 0;
}

void ConnectAcceptor::InitClientReactor(uint32 u4ClientReactorCount)
{
    if(u4ClientReactorCount > 0)
    {
        m_u4ClientReactorCount = u4ClientReactorCount;
    }
}

void ConnectAcceptor::SetPacketParseInfoID(uint32 u4PaccketParseInfoID)
{
    m_u4PacketParseInfoID = u4PaccketParseInfoID;
}

uint32 ConnectAcceptor::GetPacketParseInfoID()
{
    return m_u4PacketParseInfoID;
}

int ConnectAcceptor::make_svc_handler(CConnectHandler*& sh)
{
    OUR_DEBUG((LM_ERROR, "[ConnectAcceptor::make_svc_handler]Connect accept.\n"));

    //如果正在处理的链接超过了服务器设定的数值，则不允许链接继续链接服务器
    if (App_ConnectHandlerPool::instance()->GetUsedCount() > App_MainConfig::instance()->GetMaxHandlerCount())
    {
        OUR_DEBUG((LM_ERROR, "[ConnectAcceptor::make_svc_handler]Connect is more MaxHandlerCount(%d > %d).\n", App_ConnectHandlerPool::instance()->GetUsedCount(), App_MainConfig::instance()->GetMaxHandlerCount()));
        //不允许链接
        return -1;
    }
    else
    {
        //允许链接
        CConnectHandler* pConnectHandler = App_ConnectHandlerPool::instance()->Create();

        if (NULL != pConnectHandler)
        {
            pConnectHandler->SetLocalIPInfo(m_szListenIP, m_u4Port);

            //这里会根据反应器线程配置，自动匹配一个空闲的反应器
            int nIndex = (int)(m_u4AcceptCount % m_u4ClientReactorCount);
            ACE_Reactor* pReactor = App_ReactorManager::instance()->GetAce_Client_Reactor(nIndex);
            //OUR_DEBUG((LM_ERROR, "[ConnectAcceptor::make_svc_handler]m_u4AcceptCount=%d, pReactor=0x%08x.\n", m_u4AcceptCount, pReactor));
            pConnectHandler->reactor(pReactor);
            pConnectHandler->SetPacketParseInfoID(m_u4PacketParseInfoID);
            m_u4AcceptCount++;

            sh = pConnectHandler;
            return 0;
        }
        else
        {
            return -1;
        }
    }
}

int ConnectAcceptor::open2(ACE_INET_Addr& local_addr, ACE_Reactor* reactor, int flags, int backlog)
{
    ACE_TRACE("ACE_Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::open");
    this->flags_ = flags;
    this->use_select_ = 1;
    this->reuse_addr_ = 1;
    this->peer_acceptor_addr_ = local_addr;

    //添加记录监听服务器的IP和端口地址
    sprintf_safe(m_szListenIP, MAX_BUFF_20, "%s", local_addr.get_host_addr());
    m_u4Port = (uint32)local_addr.get_port_number();

    if (reactor == 0)
    {
        errno = EINVAL;
        return -1;
    }

    // Open the underlying PEER_ACCEPTOR.
    if (this->peer_acceptor_.open(local_addr, 1, 0, backlog) == -1)
    {
        return -1;
    }

    // Set the peer acceptor's handle into non-blocking mode.  This is a
    // safe-guard against the race condition that can otherwise occur
    // between the time when <select> indicates that a passive-mode
    // socket handle is "ready" and when we call <accept>.  During this
    // interval, the client can shutdown the connection, in which case,
    // the <accept> call can hang!
    (void) this->peer_acceptor_.enable(ACE_NONBLOCK);
    int const result = reactor->register_handler(this,
                       ACE_Event_Handler::ACCEPT_MASK);

    if (result != -1)
    {
        this->reactor(reactor);
    }
    else
    {
        this->peer_acceptor_.close();
    }

    return result;
}

int ConnectAcceptor::Init_Open(const ACE_INET_Addr& local_addr, int flags, int use_select, int reuse_addr, int backlog)
{
    //添加记录监听服务器的IP和端口地址
    sprintf_safe(m_szListenIP, MAX_BUFF_20, "%s", local_addr.get_host_addr());
    m_u4Port = (uint32)local_addr.get_port_number();

    this->flags_ = flags;
    this->use_select_ = use_select;
    this->reuse_addr_ = reuse_addr;
    this->peer_acceptor_addr_ = local_addr;

    // Open the underlying PEER_ACCEPTOR.
    if (this->peer_acceptor_.open(local_addr, 1, 0, backlog) == -1)
    {
        return -1;
    }

    (void) this->peer_acceptor_.enable(ACE_NONBLOCK);

    return 0;
}

int ConnectAcceptor::Run_Open(ACE_Reactor* reactor)
{
    this->reactor(reactor);
    int result = this->reactor()->register_handler(this,
                 ACE_Event_Handler::ACCEPT_MASK);

    if (result != -1)
    {
        return result;
    }
    else
    {
        this->peer_acceptor_.close();
    }

    return result;
}

char* ConnectAcceptor::GetListenIP()
{
    return m_szListenIP;
}

uint32 ConnectAcceptor::GetListenPort()
{
    return m_u4Port;
}

//==============================================================================

CConnectAcceptorManager::CConnectAcceptorManager(void)
{
    m_nAcceptorCount = 0;
    m_szError[0]     = '\0';
}

CConnectAcceptorManager::~CConnectAcceptorManager(void)
{
    OUR_DEBUG((LM_INFO, "[CConnectAcceptorManager::~CConnectAcceptorManager].\n"));
    Close();
    OUR_DEBUG((LM_INFO, "[CConnectAcceptorManager::~CConnectAcceptorManager]End.\n"));
}

bool CConnectAcceptorManager::InitConnectAcceptor(int nCount, uint32 u4ClientReactorCount)
{
    try
    {
        Close();

        for (int i = 0; i < nCount; i++)
        {
            ConnectAcceptor* pConnectAcceptor = new ConnectAcceptor();

            if (NULL == pConnectAcceptor)
            {
                throw "[CConnectAcceptorManager::InitConnectAcceptor]pConnectAcceptor new is fail.";
            }

            pConnectAcceptor->InitClientReactor(u4ClientReactorCount);
            m_vecConnectAcceptor.push_back(pConnectAcceptor);
        }

        return true;
    }
    catch (const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}

void CConnectAcceptorManager::Close()
{
    for (int i = 0; i < (int)m_vecConnectAcceptor.size(); i++)
    {
        ConnectAcceptor* pConnectAcceptor = m_vecConnectAcceptor[i];

        if (NULL != pConnectAcceptor)
        {
            pConnectAcceptor->close();
            delete pConnectAcceptor;
            pConnectAcceptor = NULL;
        }
    }

    m_vecConnectAcceptor.clear();
    m_nAcceptorCount = 0;
}

bool CConnectAcceptorManager::Close(const char* pIP, uint32 n4Port)
{
    //找到符合条件指定的端口停止监听
    for(vecConnectAcceptor::iterator b = m_vecConnectAcceptor.begin(); b != m_vecConnectAcceptor.end(); ++b)
    {
        ConnectAcceptor* pConnectAcceptor = (ConnectAcceptor*)(*b);

        if (NULL != pConnectAcceptor)
        {
            if(ACE_OS::strcmp(pConnectAcceptor->GetListenIP(), pIP) == 0
               && pConnectAcceptor->GetListenPort() == n4Port)
            {
                pConnectAcceptor->close();
                SAFE_DELETE(pConnectAcceptor);
                m_vecConnectAcceptor.erase(b);
                break;
            }
        }
    }

    return true;
}

int CConnectAcceptorManager::GetCount()
{
    return (int)m_vecConnectAcceptor.size();
}

ConnectAcceptor* CConnectAcceptorManager::GetConnectAcceptor(int nIndex)
{
    if (nIndex < 0 || nIndex >= (int)m_vecConnectAcceptor.size())
    {
        return NULL;
    }

    return m_vecConnectAcceptor[nIndex];
}

const char* CConnectAcceptorManager::GetError()
{
    return m_szError;
}

bool CConnectAcceptorManager::CheckIPInfo(const char* pIP, uint32 n4Port)
{
    //找到符合条件指定的端口停止监听
    for(vecConnectAcceptor::iterator b = m_vecConnectAcceptor.begin(); b != m_vecConnectAcceptor.end(); ++b)
    {
        ConnectAcceptor* pConnectAcceptor = (ConnectAcceptor*)(*b);

        if (NULL != pConnectAcceptor)
        {
            if(ACE_OS::strcmp(pConnectAcceptor->GetListenIP(), pIP) == 0
               && pConnectAcceptor->GetListenPort() == n4Port)
            {
                return true;
            }
        }
    }

    return false;
}

ConnectAcceptor* CConnectAcceptorManager::GetNewConnectAcceptor()
{
    ConnectAcceptor* pConnectAcceptor = new ConnectAcceptor();

    if(NULL == pConnectAcceptor)
    {
        return NULL;
    }

    m_vecConnectAcceptor.push_back(pConnectAcceptor);
    return pConnectAcceptor;
}

