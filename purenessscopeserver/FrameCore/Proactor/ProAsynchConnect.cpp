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

    if(nullptr != pProConnectClient)
    {
        //在这里设置连接的相关参数
        pProConnectClient->SetServerID(m_nServerID);
        uint32 u4PacketParseID = App_ClientProConnectManager::instance()->GetPacketParseID(m_nServerID);
        IClientMessage* pClientMessage = App_ClientProConnectManager::instance()->GetClientMessage(m_nServerID);

        if (u4PacketParseID > 0)
        {
            pProConnectClient->SetPacketParseInfoID(u4PacketParseID);
        }
        else
        {
            pProConnectClient->SetClientMessage(pClientMessage);
        }

        App_ClientProConnectManager::instance()->SetHandler(m_nServerID, pProConnectClient);

        pProConnectClient->addresses(m_AddrClientRemote, m_AddrClientLocal);

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
    int nError   = result.error();
    int nRet     = result.success ();
    ACE_HANDLE h = result.connect_handle();

    if (!nRet || h == ACE_INVALID_HANDLE)
    {
        SetConnectState(false);

        _ProConnectState_Info* pProConnectStateInfo = static_cast<_ProConnectState_Info* >(const_cast<void*>(result.act()));

        if(nullptr != pProConnectStateInfo)
        {
            m_nServerID = pProConnectStateInfo->m_nServerID;
            SAFE_DELETE(pProConnectStateInfo);
        }

        ACE_INET_Addr remoteaddr = App_ClientProConnectManager::instance()->GetServerAddr(m_nServerID);
        App_ClientProConnectManager::instance()->SetServerConnectState(m_nServerID, EM_Server_Connect_State::SERVER_CONNECT_FAIL);
        PSS_LOGGER_DEBUG("[CProAsynchConnect::validate_connection]({0}:{1}) connection fails,error={2}(ServerID={3}).", remoteaddr.get_host_addr(), remoteaddr.get_port_number(), nError, m_nServerID);
        m_nServerID = 0;

        return 1;
    }

    _ProConnectState_Info* pProConnectStateInfo = static_cast<_ProConnectState_Info* >(const_cast<void*>(result.act()));

    if(nullptr != pProConnectStateInfo)
    {
        m_nServerID = pProConnectStateInfo->m_nServerID;
        SAFE_DELETE(pProConnectStateInfo);
    }

    m_AddrClientRemote = remote;
    m_AddrClientLocal  = local;

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
