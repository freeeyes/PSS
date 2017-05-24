#include "ProAsynchConnect.h"
#include "ClientProConnectManager.h"

CProAsynchConnect::CProAsynchConnect(void)
{
    m_blConnectState    = false;
    m_nServerID         = 0;
}

CProAsynchConnect::~CProAsynchConnect(void)
{
}

CProConnectClient* CProAsynchConnect::make_handler()
{
    CProConnectClient* pProConnectClient = new CProConnectClient();

    if(NULL != pProConnectClient)
    {
        pProConnectClient->SetServerID(m_nServerID);
        SetConnectState(false);
    }
    else
    {
        SetConnectState(false);
    }

    return pProConnectClient;
}

int CProAsynchConnect::validate_connection(const ACE_Asynch_Connect::Result& result, const ACE_INET_Addr& remote, const ACE_INET_Addr& local)
{
    //异步检验链接是否有效，如果有效
    int nRet     = result.success ();
    ACE_HANDLE h = result.connect_handle();

    if (!nRet || h == ACE_INVALID_HANDLE)
    {
        SetConnectState(false);

        _ProConnectState_Info* pProConnectStateInfo = static_cast<_ProConnectState_Info* >(const_cast<void*>(result.act()));

        if(NULL != pProConnectStateInfo)
        {
            m_nServerID = pProConnectStateInfo->m_nServerID;
            SAFE_DELETE(pProConnectStateInfo);
        }

        ACE_INET_Addr remoteaddr = App_ClientProConnectManager::instance()->GetServerAddr(m_nServerID);
        App_ClientProConnectManager::instance()->SetServerConnectState(m_nServerID, SERVER_CONNECT_FAIL);
        OUR_DEBUG((LM_ERROR, "[CProAsynchConnect::validate_connection](%s:%d) connection fails(ServerID=%d).\n", remoteaddr.get_host_addr(), remoteaddr.get_port_number(), m_nServerID));
        m_nServerID = 0;

        return 1;
    }

    _ProConnectState_Info* pProConnectStateInfo = static_cast<_ProConnectState_Info* >(const_cast<void*>(result.act()));

    if(NULL != pProConnectStateInfo)
    {
        m_nServerID = pProConnectStateInfo->m_nServerID;
        SAFE_DELETE(pProConnectStateInfo);
    }


    //OUR_DEBUG((LM_ERROR, "[CProAsynchConnect::validate_connection]Connect IP=%s,Port=%d OK.\n", remote.get_host_addr(), remote.get_port_number()));
    return 0;
}

void CProAsynchConnect::SetConnectState(bool blConnectState)
{
    m_blConnectState = blConnectState;
}

bool CProAsynchConnect::GetConnectState()
{
    return m_blConnectState;
}
