#include "ConnectAccept.h"

ConnectAcceptor::ConnectAcceptor()
{
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

uint32 ConnectAcceptor::GetPacketParseInfoID() const
{
    return m_u4PacketParseInfoID;
}

int ConnectAcceptor::make_svc_handler(CConnectHandler*& sh)
{
    //������ڴ�������ӳ����˷������趨����ֵ�����������Ӽ������ӷ�����
    if (App_ConnectHandlerPool::instance()->GetUsedCount() > (int)GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount)
    {
        OUR_DEBUG((LM_ERROR, "[ConnectAcceptor::make_svc_handler]Connect is more MaxHandlerCount(%d > %d).\n",
                   App_ConnectHandlerPool::instance()->GetUsedCount(),
                   GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount));
        //����������
        return -1;
    }
    else
    {
        //��������
        CConnectHandler* pConnectHandler = App_ConnectHandlerPool::instance()->Create();

        if (nullptr != pConnectHandler)
        {
            pConnectHandler->SetLocalIPInfo(m_szListenIP, m_u2Port);

            //�������ݷ�Ӧ���߳����ã��Զ�ƥ��һ�����еķ�Ӧ��
            int nIndex = (int)(m_u4AcceptCount % m_u4ClientReactorCount);
            ACE_Reactor* pReactor = App_ReactorManager::instance()->GetAce_Client_Reactor(nIndex);

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

int ConnectAcceptor::open2(const ACE_INET_Addr& local_addr, ACE_Reactor* reactor, int flags, int backlog)
{
    ACE_TRACE("ACE_Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::open");
    this->flags_ = flags;
    this->use_select_ = 1;
    this->reuse_addr_ = 1;
    this->peer_acceptor_addr_ = local_addr;

    //��Ӽ�¼������������IP�Ͷ˿ڵ�ַ
    sprintf_safe(m_szListenIP, MAX_BUFF_20, "%s", local_addr.get_host_addr());
    m_u2Port = local_addr.get_port_number();

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
    //��Ӽ�¼������������IP�Ͷ˿ڵ�ַ
    sprintf_safe(m_szListenIP, MAX_BUFF_20, "%s", local_addr.get_host_addr());
    m_u2Port = local_addr.get_port_number();

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

const char* ConnectAcceptor::GetListenIP() const
{
    return m_szListenIP;
}

uint16 ConnectAcceptor::GetListenPort() const
{
    return m_u2Port;
}

//==============================================================================

CConnectAcceptorManager::CConnectAcceptorManager(void)
{
}

bool CConnectAcceptorManager::InitConnectAcceptor(int nCount, uint32 u4ClientReactorCount)
{
    try
    {
        Close();

        for (int i = 0; i < nCount; i++)
        {
            ConnectAcceptor* pConnectAcceptor = new ConnectAcceptor();

            if (nullptr == pConnectAcceptor)
            {
                throw std::domain_error("[CConnectAcceptorManager::InitConnectAcceptor]pConnectAcceptor new is fail.");
            }

            pConnectAcceptor->InitClientReactor(u4ClientReactorCount);
            m_vecConnectAcceptor.push_back(pConnectAcceptor);
        }

        return true;
    }
    catch (const std::domain_error& ex)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", ex.what());
        return false;
    }
}

void CConnectAcceptorManager::Close()
{
    OUR_DEBUG((LM_INFO, "[CConnectAcceptorManager::Close]Begin.\n"));

    for (ConnectAcceptor* pConnectAcceptor : m_vecConnectAcceptor)
    {
        pConnectAcceptor->close();
        delete pConnectAcceptor;
        pConnectAcceptor = nullptr;
    }

    m_vecConnectAcceptor.clear();
    m_nAcceptorCount = 0;
    OUR_DEBUG((LM_INFO, "[CConnectAcceptorManager::Close]End.\n"));
}

bool CConnectAcceptorManager::Close(const char* pIP, uint16 u2Port)
{
    //�ҵ���������ָ���Ķ˿�ֹͣ����
    vecConnectAcceptor::iterator b = m_vecConnectAcceptor.begin();
    while(b != m_vecConnectAcceptor.end())
    {
        ConnectAcceptor* pConnectAcceptor = *b;

		if (ACE_OS::strcmp(pConnectAcceptor->GetListenIP(), pIP) == 0
			&& pConnectAcceptor->GetListenPort() == u2Port)
		{
			pConnectAcceptor->close();
			SAFE_DELETE(pConnectAcceptor);
			m_vecConnectAcceptor.erase(b);
			break;
		}
        ++b;
    }

    return true;
}

int CConnectAcceptorManager::GetCount() const
{
    return (int)m_vecConnectAcceptor.size();
}

ConnectAcceptor* CConnectAcceptorManager::GetConnectAcceptor(int nIndex)
{
    if (nIndex < 0 || nIndex >= (int)m_vecConnectAcceptor.size())
    {
        return nullptr;
    }

    return m_vecConnectAcceptor[nIndex];
}

const char* CConnectAcceptorManager::GetError() const
{
    return m_szError;
}

bool CConnectAcceptorManager::CheckIPInfo(const char* pIP, uint16 u2Port)
{
    //�ҵ���������ָ���Ķ˿�ֹͣ����
	vecConnectAcceptor::iterator b = m_vecConnectAcceptor.begin();
	while (b != m_vecConnectAcceptor.end())
    {
        const ConnectAcceptor* pConnectAcceptor = *b;

        if (ACE_OS::strcmp(pConnectAcceptor->GetListenIP(), pIP) == 0
            && pConnectAcceptor->GetListenPort() == u2Port)
        {
            return true;
        }

        ++b;
    }

    return false;
}

ConnectAcceptor* CConnectAcceptorManager::GetNewConnectAcceptor()
{
    ConnectAcceptor* pConnectAcceptor = new ConnectAcceptor();

    if(nullptr == pConnectAcceptor)
    {
        return nullptr;
    }

    m_vecConnectAcceptor.push_back(pConnectAcceptor);
    return pConnectAcceptor;
}

