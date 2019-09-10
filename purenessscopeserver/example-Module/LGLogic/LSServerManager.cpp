#include "LSServerManager.h"

CLSServerManager::CLSServerManager()
{
    m_u4ServerID       = 0;
    m_szServerIP[0]    = '\0';
    m_u4Port           = 0;
    m_pServerObject    = NULL;

    m_szLGIP[0]        = '\0';
    m_u4LGPort         = 0;
    m_u4LGID           = 0;
    m_u4RecvBuffLength = 0;

    sprintf_safe(m_szSessionKey, 32, "%s", SESSION_KEY);
}

CLSServerManager::~CLSServerManager()
{
}

void CLSServerManager::Init(uint32 u4ServerID, const char* pIP, uint32 u4Port, CServerObject* pServerObject)
{
    sprintf_safe(m_szServerIP, 50, "%s", pIP);
    m_u4Port        = u4Port;
    m_pServerObject = pServerObject;
    m_u4ServerID    = u4ServerID;

    //读取当前配置文件
    m_objlistManager.ReadList();
}

bool CLSServerManager::Connect()
{
    if(NULL == m_pServerObject)
    {
        return false;
    }
    else
    {
        return m_pServerObject->GetClientManager()->Connect(
                   m_u4ServerID,
                   m_szServerIP,
                   m_u4Port,
                   TYPE_IPV4,
                   (IClientMessage* )this);
    }
}

bool CLSServerManager::RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv, _ClientIPInfo const& objServerIPInfo)
{
    //处理收到来自LS的数据
    if(mbRecv->length() <= 40)
    {
        //接收到了不完整的包头，丢弃
        OUR_DEBUG((LM_INFO, "[CLSServerManager::RecvData](%s:%d) recv not enougth.\n", objServerIPInfo.m_szClientIP, objServerIPInfo.m_nPort));
        return true;
    }

    char* pRecvBuffer = mbRecv->rd_ptr();

    //从包头中解析出命令字ID
    memcpy_safe(&pRecvBuffer[2], sizeof(uint16), (char* )&u2CommandID, sizeof(uint16));

    if(u2CommandID == COMMAND_LOGIC_LG_LOGIN_R)
    {
        Recv_LS_Login(pRecvBuffer, (uint32)mbRecv->length());
    }
    else if(u2CommandID == COMMAND_LOGIC_LG_KEY_R)
    {
        Recv_LS_Key_Update(pRecvBuffer, (uint32)mbRecv->length());
    }
    else if(u2CommandID == COMMAND_LOGIC_LG_LIST_R)
    {
        Recv_LS_List_Update(pRecvBuffer, (uint32)mbRecv->length());
    }

    return true;
}

bool CLSServerManager::ConnectError(int nError, _ClientIPInfo const& objServerIPInfo)
{
    OUR_DEBUG((LM_INFO, "[CLSServerManager::ConnectError]nError=%d,(%s:%d).\n", nError, objServerIPInfo.m_szClientIP, objServerIPInfo.m_nPort));
    return true;
}

void CLSServerManager::ReConnect(int nServerID)
{
    OUR_DEBUG((LM_INFO, "[CLSServerManager::ReConnect]nServerID=%d.\n", nServerID));
    //重新发送注册
    Send_LG_Login();
    return;
}

void CLSServerManager::Send_LG_Login()
{
    if(NULL == m_pServerObject)
    {
        return;
    }

    //拼装向LS注册的数据包
    uint32 u4SendPacketLen = 2 * sizeof(uint32) + 2 * sizeof(uint8)
                             + ACE_OS::strlen(m_szLGIP) + ACE_OS::strlen(m_objlistManager.Get_MD5_Data()) + 2;

    IBuffPacket* pSendPacket = m_pServerObject->GetPacketManager()->Create();
    (*pSendPacket) << (uint16)SERVER_PROTOCAL_VERSION;
    (*pSendPacket) << (uint16)COMMAND_LOGIC_LG_LOGIN;
    (*pSendPacket) << (uint32)u4SendPacketLen;
    pSendPacket->WriteStream(m_szSessionKey, 32);

    _VCHARS_STR strLDIP;
    uint8 u1Len = (uint8)ACE_OS::strlen(m_szLGIP);
    strLDIP.SetData(m_szLGIP, u1Len);
    _VCHARS_STR strServerCode;
    u1Len = (uint8)ACE_OS::strlen(m_objlistManager.Get_MD5_Data());
    strServerCode.SetData(m_objlistManager.Get_MD5_Data(), u1Len);
    (*pSendPacket) << m_u4LGID;
    (*pSendPacket) << strLDIP;
    (*pSendPacket) << m_u4LGPort;
    (*pSendPacket) << strServerCode;

    char* pData = (char* )pSendPacket->GetData();

    m_pServerObject->GetClientManager()->SendData(m_u4ServerID, pData, pSendPacket->GetPacketLen(), false);

    m_pServerObject->GetPacketManager()->Delete(pSendPacket);
}

void CLSServerManager::Send_LG_List()
{
    if(NULL == m_pServerObject)
    {
        return;
    }

    //拼装向LS注册的数据包
    uint32 u4SendPacketLen = sizeof(uint32);

    IBuffPacket* pSendPacket = m_pServerObject->GetPacketManager()->Create();
    (*pSendPacket) << (uint16)SERVER_PROTOCAL_VERSION;
    (*pSendPacket) << (uint16)COMMAND_LOGIC_LG_LIST;
    (*pSendPacket) << (uint32)u4SendPacketLen;
    pSendPacket->WriteStream(m_szSessionKey, 32);

    _VCHARS_STR strLDIP;
    uint8 u1Len = (uint8)ACE_OS::strlen(m_szLGIP);
    strLDIP.SetData(m_szLGIP, u1Len);
    _VCHARS_STR strServerCode;
    u1Len = (uint8)ACE_OS::strlen(m_objlistManager.Get_MD5_Data());
    strServerCode.SetData(m_objlistManager.Get_MD5_Data(), u1Len);
    (*pSendPacket) << m_u4LGID;

    char* pData = (char*)pSendPacket->GetData();

    m_pServerObject->GetClientManager()->SendData(m_u4ServerID, pData, pSendPacket->GetPacketLen(), false);

    m_pServerObject->GetPacketManager()->Delete(pSendPacket);
}

void CLSServerManager::Set_LG_Info(const char* pLGIP, uint32 u4LGPort, uint32 u4LGID)
{
    sprintf_safe(m_szLGIP, 50, "%s", pLGIP);
    m_u4LGPort    = u4LGPort;
    m_u4LGID      = u4LGID;
}

void CLSServerManager::Send_LG_Alive()
{
    if(NULL == m_pServerObject)
    {
        return;
    }

    //拼装向LS注册的数据包
    uint32 u4SendPacketLen = sizeof(uint32);

    IBuffPacket* pSendPacket = m_pServerObject->GetPacketManager()->Create();
    (*pSendPacket) << (uint16)SERVER_PROTOCAL_VERSION;
    (*pSendPacket) << (uint16)COMMAND_LOGIC_ALIVE;
    (*pSendPacket) << (uint32)u4SendPacketLen;
    pSendPacket->WriteStream(m_szSessionKey, 32);

    _VCHARS_STR strLDIP;
    uint8 u1Len = (uint8)ACE_OS::strlen(m_szLGIP);
    strLDIP.SetData(m_szLGIP, u1Len);
    _VCHARS_STR strServerCode;
    u1Len = (uint8)ACE_OS::strlen(m_objlistManager.Get_MD5_Data());
    strServerCode.SetData(m_objlistManager.Get_MD5_Data(), u1Len);
    (*pSendPacket) << m_u4LGID;

    char* pData = (char*)pSendPacket->GetData();

    m_pServerObject->GetClientManager()->SendData(m_u4ServerID, pData, pSendPacket->GetPacketLen(), false);

    m_pServerObject->GetPacketManager()->Delete(pSendPacket);
}

void CLSServerManager::Recv_LS_Login(const char* pRecvBuff, uint32 u4Len)
{
    uint32 u4LGID        = 0;
    char   szListKey[33] = {'\0'};

    if(u4Len <= 40)
    {
        OUR_DEBUG((LM_INFO, "[CLSServerManager::Recv_LS_Login]not enought.\n"));
        return;
    }

    memcpy_safe((char* )&pRecvBuff[40], sizeof(uint32), (char* )&u4LGID, sizeof(uint32));

    if(m_u4LGID == u4LGID)
    {
        //更新LSKEY
        uint8 u1Len = 0;
        memcpy_safe((char* )&pRecvBuff[44], 1, (char* )&u1Len, 1);

        memcpy_safe((char* )&pRecvBuff[45], u1Len, szListKey, u1Len);

        OUR_DEBUG((LM_INFO, "[CLSServerManager::Recv_LS_Login]m_szLSKey=%s.\n", szListKey));
    }
}

void CLSServerManager::Recv_LS_Key_Update(const char* pRecvBuff, uint32 u4Len)
{
    uint32 u4LGID        = 0;
    char   szListKey[33] = {'\0'};

    if(u4Len <= 40)
    {
        OUR_DEBUG((LM_INFO, "[CLSServerManager::Recv_LS_Key_Update]not enought.\n"));
        return;
    }

    memcpy_safe((char* )&pRecvBuff[40], sizeof(uint32), (char* )&u4LGID, sizeof(uint32));

    if(m_u4LGID == u4LGID)
    {
        //更新LSKEY
        uint8 u1Len = 0;
        memcpy_safe((char* )&pRecvBuff[44], 1, (char* )&u1Len, 1);

        memcpy_safe((char* )&pRecvBuff[45], u1Len, szListKey, u1Len);

        OUR_DEBUG((LM_INFO, "[CLSServerManager::Recv_LS_Key_Update]m_szLSKey=%s.\n", szListKey));
    }
}

char* CLSServerManager::Get_LS_Key()
{
    return m_objlistManager.Get_MD5_Data();
}

void CLSServerManager::Recv_LS_List_Update(const char* pRecvBuff, uint32 u4Len)
{
    if(u4Len <= 40)
    {
        OUR_DEBUG((LM_INFO, "[CLSServerManager::Recv_LS_List_Update]not enought.\n"));
        return;
    }

    m_objlistManager.Clear();

    //获得当前的MD5Key值
    uint32 u4Pos = 40;
    uint8 u1KeyLen = pRecvBuff[u4Pos];
    u4Pos++;
    char szMD5[33] = {'\0'};
    memcpy_safe((char* )&pRecvBuff[u4Pos], u1KeyLen, szMD5, u1KeyLen);
    u4Pos += (uint32)u1KeyLen;
    m_objlistManager.Set_MD5_Data(szMD5);

    //得到列表的长度
    uint32 u4Count = 0;
    memcpy_safe((char* )&pRecvBuff[u4Pos], (uint32)sizeof(uint32), (char* )&u4Count, (uint32)sizeof(uint32));
    u4Pos += (uint32)sizeof(uint32);

    uint32 u4LGID     = 0;
    char   szLGIP[50] = {'\0'};
    uint32 u4LGPort   = 0;
    _VCHARS_STR strIP;

    for(uint32 i = 0; i < u4Count; i++)
    {
        memcpy_safe((char* )&pRecvBuff[u4Pos], (uint32)sizeof(uint32), (char* )&u4LGID, (uint32)sizeof(uint32));
        u4Pos += (uint32)sizeof(uint32);
        uint8 u1IPLen = pRecvBuff[u4Pos];
        u4Pos++;
        memcpy_safe((char* )&pRecvBuff[u4Pos], u1IPLen, szLGIP, u1IPLen);
        u4Pos += (uint32)u1IPLen;
        memcpy_safe((char* )&pRecvBuff[u4Pos], (uint32)sizeof(uint32), (char* )&u4LGPort, (uint32)sizeof(uint32));
        u4Pos += (uint32)sizeof(uint32);

        m_objlistManager.Add_LG_Info(0, u4LGID, szLGIP, u4LGPort);
    }

    //存储为本地文件
    m_objlistManager.SaveList();

}

void CLSServerManager::Get_All_LG_List(IBuffPacket* pbpList, uint32& u4Count)
{
    m_objlistManager.Get_All_LG_List(pbpList, u4Count);
}

bool CLSServerManager::Need_Send_Format()
{
    return false;
}

bool CLSServerManager::Send_Format_data(char* pData, uint32 u4Len, IMessageBlockManager* pMessageBlockManager, ACE_Message_Block*& mbSend)
{
    if(NULL != pData && NULL != pMessageBlockManager && NULL == mbSend)
    {
        OUR_DEBUG((LM_INFO, "[CLSServerManager::Send_Format_data]u4Len=%d.\n", u4Len));
    }

    return false;
}

bool CLSServerManager::Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID, ACE_Message_Block*& mbFinishRecv, EM_PACKET_ROUTE& emPacketRoute)
{
    if(m_u4RecvBuffLength + mbRecv->length() < 40 || mbRecv->length() >= RECV_BUFF_SIZE)
    {
        if(mbRecv->length() > 0)
        {
            ACE_OS::memcpy(&m_szRecvBuffData[m_u4RecvBuffLength], mbRecv->rd_ptr(), mbRecv->length());
            m_u4RecvBuffLength += mbRecv->length();
            mbRecv->rd_ptr(mbRecv->length());
        }

        return false;
    }
    else
    {
        ACE_OS::memcpy(&m_szRecvBuffData[m_u4RecvBuffLength], mbRecv->rd_ptr(), mbRecv->length());
        m_u4RecvBuffLength += mbRecv->length();
        mbRecv->rd_ptr(mbRecv->length());

        //拆解出完整的数据包头
        uint32 u2Version     = 0;           //协议版本号
        uint32 u4BodyLen     = 0;           //包体长度
        char   szSession[33] = {'\0'};      //Session字符串

        //解析包头中的数据包长
        uint32 u4Pos = 0;
        memcpy_safe((char* )&m_szRecvBuffData[u4Pos], (uint32)sizeof(uint16), (char* )&u2Version, (uint32)sizeof(uint16));
        u4Pos += sizeof(uint16);
        memcpy_safe((char* )&m_szRecvBuffData[u4Pos], (uint32)sizeof(uint16), (char* )&u2CommandID, (uint32)sizeof(uint16));
        u4Pos += sizeof(uint16);
        memcpy_safe((char* )&m_szRecvBuffData[u4Pos], (uint32)sizeof(uint32), (char* )&u4BodyLen, (uint32)sizeof(uint32));
        u4Pos += sizeof(uint32);
        memcpy_safe((char* )&m_szRecvBuffData[u4Pos], (uint32)(sizeof(char)*32), (char* )&szSession, (uint32)(sizeof(char)*32));
        u4Pos += sizeof(char)*32;

        //整个数据包的长度
        uint32 u4PacketSize = u4BodyLen + 40;

        if(m_u4RecvBuffLength >= u4PacketSize)
        {
            //完整的数据包
            mbFinishRecv = pMessageBlockManager->Create(u4PacketSize);

            //创建完整数据包
            memcpy_safe(m_szRecvBuffData, u4PacketSize, mbFinishRecv->wr_ptr(),  u4PacketSize);
            mbFinishRecv->wr_ptr( u4BodyLen + 40);

            //数据缓冲向前移位
            if(m_u4RecvBuffLength - u4PacketSize > 0)
            {
                memcpy_safe(&m_szRecvBuffData[u4PacketSize], m_u4RecvBuffLength - u4PacketSize, m_szRecvBuffData, m_u4RecvBuffLength - u4PacketSize);
                m_u4RecvBuffLength -= u4PacketSize;
            }
            else
            {
                m_u4RecvBuffLength = 0;
            }

            return true;
        }
        else
        {
            //不完整的，继续等
            return false;
        }
    }


}




