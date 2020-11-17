#include "ProactorUDPClient.h"

CProactorUDPClient::CProactorUDPClient(void)
{
    m_pClientUDPMessage = nullptr;
    m_u4RecvPacketCount = 0;
    m_u4SendPacketCount = 0;
    m_u4RecvSize        = 0;
    m_u4SendSize        = 0;
}

CProactorUDPClient::~CProactorUDPClient(void)
{
}

int CProactorUDPClient::OpenAddress(const ACE_INET_Addr& AddrLocal, EM_UDP_TYPE emType, ACE_Proactor* pProactor, IClientUDPMessage* pClientUDPMessage)
{
    if(m_skRemote.open(AddrLocal) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorUDPClient::OpenAddress]Open error(%d).\n", errno));
        return -1;
    }

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

#if PSS_PLATFORM == PLATFORM_WIN
    //����wsaIoctl
    bool blBehavior = false;
    unsigned long lRet = 0;
    int nStatus = ACE_OS::ioctl(m_skRemote.get_handle(), SIO_UDP_CONNRESET, &blBehavior, sizeof(blBehavior), nullptr, 0, &lRet, nullptr, nullptr);

    if(0 != nStatus)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]ioctl SIO_UDP_CONNRESET error.\n"));
    }

#endif

    char* pCompletionKey = nullptr;

    if(m_Read.open(*this, m_skRemote.get_handle(), pCompletionKey, pProactor) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorUDPClient::OpenAddress]m_Read error.\n"));
        return -1;
    }

    if(m_Write.open(*this, m_skRemote.get_handle(), pCompletionKey, pProactor) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorUDPClient::OpenAddress]m_Write error.\n"));
        return -1;
    }

    char* pAct = nullptr;
    ACE_Message_Block* pMBBuff = App_MessageBlockManager::instance()->Create(MAX_UDP_PACKET_LEN);

    if(nullptr == pMBBuff)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorUDPClient::OpenAddress]pMBBuff is nullptr.\n"));
        return -1;
    }

    m_pClientUDPMessage = pClientUDPMessage;
    size_t stRecvLen = MAX_UDP_PACKET_LEN;
    int nRecvSize = (int)m_Read.recv(pMBBuff, stRecvLen, 0, PF_INET, pAct);
    return nRecvSize;
}

void CProactorUDPClient::Close()
{
    m_Read.cancel();
    m_Write.cancel();
    m_skRemote.close();
}

void CProactorUDPClient::handle_read_dgram(const ACE_Asynch_Read_Dgram::Result& result)
{
    //��ΪUDP��һ���Է������������ݰ������Բ�����ճ������
    ACE_Message_Block* pMBBuff = nullptr;

    ACE_Message_Block* pMb = result.message_block();
    int nPacketLen = (int)result.bytes_transferred();
    int nTran = (int)result.bytes_transferred();

    ACE_INET_Addr ServerAddr;
    result.remote_address(ServerAddr);
    m_addrRemote.set(ServerAddr);

    if(nPacketLen != 0 && nTran != 0)
    {
        //��������
        if (false == CheckMessage(pMb, (uint32)nTran))
        {
            OUR_DEBUG((LM_INFO, "[CProactorUDPClient::handle_read_dgram]CheckMessage error.\n"));
        }

        App_MessageBlockManager::instance()->Close(pMb);

        pMBBuff = App_MessageBlockManager::instance()->Create(MAX_UDP_PACKET_LEN);
    }
    else
    {
        pMBBuff = pMb;
    }

    if(nullptr == pMBBuff)
    {
        OUR_DEBUG((LM_INFO, "[CProactorUDPClient::handle_read_dgram]pMBBuff is nullptr.\n"));
    }
    else
    {
        size_t stRecvLen = MAX_UDP_PACKET_LEN;
        m_Read.recv(pMBBuff, stRecvLen, 0, PF_INET, nullptr);
    }
}

bool CProactorUDPClient::SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, int nPort)
{
    ACE_INET_Addr AddrRemote;
    int nErr = AddrRemote.set(nPort, szIP);

    if(nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[CProactorUDPClient::SendMessage]set_address error[%d].\n", errno));
        SAFE_DELETE_ARRAY(pMessage);
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
        OUR_DEBUG((LM_ERROR, "[CProactorUDPClient::SendMessage]send error(%d).\n", errno));
        return false;
    }
}

_ClientConnectInfo CProactorUDPClient::GetClientConnectInfo()
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

bool CProactorUDPClient::CheckMessage(ACE_Message_Block* pMbData, uint32 u4Len)
{
    if(nullptr == m_pClientUDPMessage || nullptr == pMbData)
    {
        return false;
    }

    _ClientIPInfo objServerIPInfo;
    objServerIPInfo.m_strClientIP = m_addrRemote.get_host_addr();
    objServerIPInfo.m_u2Port = m_addrRemote.get_port_number();
    m_pClientUDPMessage->RecvData(pMbData->rd_ptr(), u4Len, objServerIPInfo);

    m_atvInput = ACE_OS::gettimeofday();
    m_u4RecvSize += u4Len;
    m_u4RecvPacketCount++;

    return true;
}

