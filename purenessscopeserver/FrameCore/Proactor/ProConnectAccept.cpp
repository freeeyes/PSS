#include "ProConnectAccept.h"

ProConnectAcceptor::ProConnectAcceptor()
{
    m_szListenIP[0]         = '\0';
    m_u4Port                = 0;
    m_u4AcceptCount         = 0;
    m_u4ClientProactorCount = 1;
    m_u4PacketParseInfoID   = 0;
}

void ProConnectAcceptor::InitClientProactor(uint32 u4ClientProactorCount)
{
    m_u4ClientProactorCount = u4ClientProactorCount;
}

void ProConnectAcceptor::SetPacketParseInfoID(uint32 u4PaccketParseInfoID)
{
    m_u4PacketParseInfoID = u4PaccketParseInfoID;
}

uint32 ProConnectAcceptor::GetPacketParseInfoID()
{
    return m_u4PacketParseInfoID;
}

CProConnectHandler* ProConnectAcceptor::make_handler(void)
{
    CProConnectHandler* pProConnectHandle = App_ProConnectHandlerPool::instance()->Create();

    if(nullptr != pProConnectHandle)
    {
        pProConnectHandle->SetLocalIPInfo(m_szListenIP, m_u4Port);
        //这里会根据反应器线程配置，自动匹配一个空闲的反应器
        int nIndex = (int)(m_u4AcceptCount % m_u4ClientProactorCount);
        ACE_Proactor* pProactor = App_ProactorManager::instance()->GetAce_Client_Proactor(nIndex);
        pProConnectHandle->proactor(pProactor);
        pProConnectHandle->SetPacketParseInfoID(m_u4PacketParseInfoID);
        m_u4AcceptCount++;
    }

    return pProConnectHandle;
}

int ProConnectAcceptor::validate_connection (const ACE_Asynch_Accept::Result& result,
        const ACE_INET_Addr& remote,
        const ACE_INET_Addr& local)
{
    //如果正在处理的链接超过了服务器设定的数值，则不允许链接继续链接服务器
    if(App_ProConnectHandlerPool::instance()->GetUsedCount() > (int)GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount)
    {
        PSS_LOGGER_DEBUG("[ProConnectAcceptor::validate_connection]Connect is more MaxHandlerCount({0} > {1}).",
                   App_ProConnectHandlerPool::instance()->GetUsedCount(),
                   GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount);
        //不允许链接
        return -1;
    }
    else
    {
        //允许链接
        return 0;
    }
}

char* ProConnectAcceptor::GetListenIP()
{
    return m_szListenIP;
}

uint32 ProConnectAcceptor::GetListenPort()
{
    return m_u4Port;
}

void ProConnectAcceptor::SetListenInfo(const char* pIP, uint32 u4Port)
{
    sprintf_safe(m_szListenIP, MAX_BUFF_20, "%s", pIP);
    m_u4Port = u4Port;
}

CProConnectAcceptManager::CProConnectAcceptManager(void)
{
    m_nAcceptorCount = 0;
    m_szError[0]     = '\0';
}

CProConnectAcceptManager::~CProConnectAcceptManager(void)
{
    Close();
}

bool CProConnectAcceptManager::InitConnectAcceptor(int nCount, uint32 u4ClientProactorCount)
{
    try
    {
        Close();

        for(int i = 0; i < nCount; i++)
        {
            ProConnectAcceptor* pConnectAcceptor = new ProConnectAcceptor();

            if(nullptr == pConnectAcceptor)
            {
                throw "[CProConnectAcceptManager::InitConnectAcceptor]pConnectAcceptor new is fail.";
            }

            pConnectAcceptor->InitClientProactor(u4ClientProactorCount);
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

void CProConnectAcceptManager::Close()
{
    ACE_Time_Value tvSleep(0, 10000);

    for(auto* pConnectAcceptor : m_vecConnectAcceptor)
    {
        if(nullptr != pConnectAcceptor)
        {
            pConnectAcceptor->cancel();
            ACE_OS::sleep(tvSleep);
            SAFE_DELETE(pConnectAcceptor);
        }
    }

    m_vecConnectAcceptor.clear();
    m_nAcceptorCount = 0;
}

bool CProConnectAcceptManager::Close( const char* pIP, uint32 n4Port )
{
    //找到符合条件指定的端口停止监听
    for(vecProConnectAcceptor::iterator b = m_vecConnectAcceptor.begin(); b != m_vecConnectAcceptor.end(); ++b)
    {
        ProConnectAcceptor* pConnectAcceptor = (ProConnectAcceptor*)(*b);

        if (nullptr != pConnectAcceptor)
        {
            if(ACE_OS::strcmp(pConnectAcceptor->GetListenIP(), pIP) == 0
               && pConnectAcceptor->GetListenPort() == n4Port)
            {
                pConnectAcceptor->cancel();
                SAFE_DELETE(pConnectAcceptor);
                m_vecConnectAcceptor.erase(b);
                break;
            }
        }
    }

    return true;
}

int CProConnectAcceptManager::GetCount()
{
    return (int)m_vecConnectAcceptor.size();
}

ProConnectAcceptor* CProConnectAcceptManager::GetConnectAcceptor(int nIndex)
{
    if(nIndex < 0 || nIndex >= (int)m_vecConnectAcceptor.size())
    {
        return nullptr;
    }

    return m_vecConnectAcceptor[nIndex];
}

const char* CProConnectAcceptManager::GetError()
{
    return m_szError;
}

bool CProConnectAcceptManager::CheckIPInfo(const char* pIP, uint32 n4Port)
{
    //找到符合条件指定的端口停止监听
    for(auto* ProConnectAcceptor : m_vecConnectAcceptor)
    {
        if (nullptr != ProConnectAcceptor)
        {
            if(ACE_OS::strcmp(ProConnectAcceptor->GetListenIP(), pIP) == 0
               && ProConnectAcceptor->GetListenPort() == n4Port)
            {
                return true;
            }
        }
    }

    return false;
}

ProConnectAcceptor* CProConnectAcceptManager::GetNewConnectAcceptor()
{
    ProConnectAcceptor* pConnectAcceptor = new ProConnectAcceptor();

    if(nullptr == pConnectAcceptor)
    {
        return nullptr;
    }

    m_vecConnectAcceptor.push_back(pConnectAcceptor);
    return pConnectAcceptor;
}
