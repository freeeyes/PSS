#include "ReactorUDPClient.h"

CReactorUDPClient::CReactorUDPClient(void)
{
}

CReactorUDPClient::~CReactorUDPClient(void)
{
}

int CReactorUDPClient::OpenAddress(const ACE_INET_Addr& AddrRemote, EM_UDP_TYPE emType, ACE_Reactor* pReactor, IClientUDPMessage* pClientUDPMessage)
{
    if(m_skRemote.open(AddrRemote) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorUDPClient::OpenAddress]Open error(%d).\n", errno));
        return -1;
    }

    reactor(pReactor);

    //���÷��ͳ�ʱʱ�䣨��ΪUDP����ͻ��˲����ڵĻ���sendto������һ��recv����
    //����������һ����ʱ���ø�recv�������޵���ȥ
    struct timeval timeout = {MAX_RECV_UDP_TIMEOUT, 0};
    ACE_OS::setsockopt(m_skRemote.get_handle(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    if(emType == EM_UDP_TYPE::UDP_BROADCAST)
    {
        //����ǹ㲥������setoptΪ�㲥����
        bool bOpt = true;
        ACE_OS::setsockopt(m_skRemote.get_handle(), SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));
    }

    m_pClientUDPMessage = pClientUDPMessage;

    if(-1 == this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK))
    {
        OUR_DEBUG((LM_ERROR, "[CReactorUDPClient::OpenAddress] Addr is register_handler error(%d).\n", errno));
        return -1;
    }

    return 0;
}

void CReactorUDPClient::Close()
{
    ACE_Reactor_Mask close_mask = ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;
    this->reactor()->remove_handler(this, close_mask);
    m_skRemote.close();
}

ACE_HANDLE CReactorUDPClient::get_handle(void) const
{
    return m_skRemote.get_handle();
}

int CReactorUDPClient::handle_input(ACE_HANDLE fd)
{
    if(fd == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorUDPClient::handle_input]fd is ACE_INVALID_HANDLE.\n"));
        return -1;
    }

    char szBuff[MAX_UDP_PACKET_LEN] = {'\0'};

    int nDataLen = (int)m_skRemote.recv(szBuff, MAX_UDP_PACKET_LEN, m_addrRemote);

    if(nDataLen > 0 && false == CheckMessage(szBuff, (uint32)nDataLen))
    {
        OUR_DEBUG((LM_INFO, "[CReactorUDPClient::handle_inpu]CheckMessage error.\n"));
    }

    return 0;
}

int CReactorUDPClient::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
    if(handle == ACE_INVALID_HANDLE)
    {
        OUR_DEBUG((LM_ERROR, "[CReactorUDPClient::handle_close]close_mask = %d.\n", (uint32)close_mask));
    }

    Close();
    return 0;
}

bool CReactorUDPClient::SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, uint16 u2Port)
{
    ACE_INET_Addr AddrRemote;
    int nErr = AddrRemote.set(u2Port, szIP);

    if(nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::SendMessage]set_address error[%d].\n", errno));
        return false;
    }

    int nSize = (int)m_skRemote.send(pMessage, u4Len, AddrRemote);

    if((uint32)nSize == u4Len)
    {
        m_atvOutput = ACE_OS::gettimeofday();
        m_u4SendSize += u4Len;
        m_u4SendPacketCount++;
        return true;
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]send error(%d).\n", errno));
        return false;
    }
}

_ClientConnectInfo CReactorUDPClient::GetClientConnectInfo() const
{
    _ClientConnectInfo ClientConnectInfo;
    ClientConnectInfo.m_blValid       = true;
    ClientConnectInfo.m_u4ConnectID   = 0;
    ClientConnectInfo.m_u4AliveTime   = 0;
    ClientConnectInfo.m_u4BeginTime   = (uint32)m_atvInput.sec();
    ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
    ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
    ClientConnectInfo.m_u4RecvCount   = m_u4RecvPacketCount;
    ClientConnectInfo.m_u4SendCount   = m_u4SendPacketCount;
    return ClientConnectInfo;
}

bool CReactorUDPClient::CheckMessage(const char* pData, uint32 u4Len)
{
    if(NULL == m_pClientUDPMessage || NULL == pData)
    {
        return false;
    }

    _ClientIPInfo objServerIPInfo;
    objServerIPInfo.m_strClientIP = m_addrRemote.get_host_addr();
    objServerIPInfo.m_u2Port = m_addrRemote.get_port_number();
    m_pClientUDPMessage->RecvData(pData, u4Len, objServerIPInfo);

    m_atvInput = ACE_OS::gettimeofday();
    m_u4RecvSize += u4Len;
    m_u4RecvPacketCount++;

    return true;
}
