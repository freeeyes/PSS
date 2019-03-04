#include "MonitorCommand.h"

CPSSMonitorCommand::CPSSMonitorCommand()
{

}

CPSSMonitorCommand::~CPSSMonitorCommand()
{

}

int CPSSMonitorCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{
    __ENTER_FUNCTION();

    DO_TRACE("./", pMessage->GetMessageBase()->m_szTraceID);

    if (m_pServerObject == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] m_pServerObject is NULL(%d).\n", bDeleteFlag));
        return -1;
    }

    if (pMessage == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pMessage is NULL.\n"));
        return -1;
    }

    //处理链接建立信息
    MESSAGE_FUNCTION_BEGIN(pMessage->GetMessageBase()->m_u2Cmd);
    MESSAGE_FUNCTION(COMMAND_MONITOR_LOGIN, Do_Pss_Login, pMessage);
    MESSAGE_FUNCTION(COMMAND_MONITOR_DATA, Do_Pss_Monitor_Data, pMessage);
    MESSAGE_FUNCTION_END;

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}

void CPSSMonitorCommand::SetServerObject(CServerObject* pServerObject)
{
    m_pServerObject = pServerObject;
}

CServerObject* CPSSMonitorCommand::GetServerObject()
{
    return m_pServerObject;
}

int CPSSMonitorCommand::Do_Pss_Login(IMessage* pMessage)
{
    OUR_DEBUG((LM_INFO, "[CPSSMonitorCommand::Do_Pss_Login]=====.\n"));

    if (NULL == pMessage)
    {
        OUR_DEBUG((LM_ERROR, "[CPSSMonitorCommand::Do_Pss_Login] pMessage is NULL.\n"));
        return -1;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    //消息染色测试
    DO_TRACE("./", pMessage->GetMessageBase()->m_szTraceID);

    IBuffPacket* pBodyBuffPacket = m_pServerObject->GetPacketManager()->Create();

    if (NULL == pBodyBuffPacket)
    {
        OUR_DEBUG((LM_INFO, "[CPSSMonitorCommand::Do_Pss_Login]pBodyBuffPacket is NULL.\n"));
        return -1;
    }

    pBodyBuffPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    VCHARM_STR objClientIP;
    (*pBodyBuffPacket) >> objClientIP;

    char szClientIP[MAX_BUFF_50] = { '\0' };
    memcpy_safe(objClientIP.text, MAX_BUFF_50, szClientIP, MAX_BUFF_50);

    OUR_DEBUG((LM_INFO, "[CPSSMonitorCommand::Do_Pss_Login]szClientIP=%s.\n", szClientIP));

    m_pServerObject->GetPacketManager()->Delete(pBodyBuffPacket);

    //返回相应消息
    IBuffPacket* pSendBuffPacket = m_pServerObject->GetPacketManager()->Create();

    if (NULL == pSendBuffPacket)
    {
        OUR_DEBUG((LM_INFO, "[CPSSMonitorCommand::Do_Pss_Login]pBodyBuffPacket is NULL.\n"));
        return -1;
    }

    uint16 u2PostCommandID = COMMAND_MONITOR_LOGIN_ACK;

    (*pSendBuffPacket) << (uint32)6;
    (*pSendBuffPacket) << u2PostCommandID;
    (*pSendBuffPacket) << (uint32)0;

    m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID,
            pSendBuffPacket,
            SENDMESSAGE_JAMPNOMAL,
            u2PostCommandID,
            PACKET_SEND_IMMEDIATLY,
            PACKET_IS_FRAMEWORK_RECYC,
            0);

    return 0;
}

int CPSSMonitorCommand::Do_Pss_Monitor_Data(IMessage* pMessage)
{
    OUR_DEBUG((LM_INFO, "[CPSSMonitorCommand::Do_Pss_Monitor_Data]=====.\n"));

    if (NULL == pMessage)
    {
        OUR_DEBUG((LM_ERROR, "CPSSMonitorCommand::Do_Pss_Monitor_Data] pMessage is NULL.\n"));
        return -1;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    //消息染色测试
    DO_TRACE("./", pMessage->GetMessageBase()->m_szTraceID);

    IBuffPacket* pBodyBuffPacket = m_pServerObject->GetPacketManager()->Create();

    if (NULL == pBodyBuffPacket)
    {
        OUR_DEBUG((LM_INFO, "[CPSSMonitorCommand::Do_Pss_Monitor_Data]pBodyBuffPacket is NULL.\n"));
        return -1;
    }

    pBodyBuffPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    uint32 u4Cpu          = 0;
    uint32 u4MemorySize   = 0;
    uint32 u4ConnectCount = 0;
    uint32 u4DataIn       = 0;
    uint32 u4DataOut      = 0;
    (*pBodyBuffPacket) >> u4Cpu;
    (*pBodyBuffPacket) >> u4MemorySize;
    (*pBodyBuffPacket) >> u4ConnectCount;
    (*pBodyBuffPacket) >> u4DataIn;
    (*pBodyBuffPacket) >> u4DataOut;

    OUR_DEBUG((LM_INFO, "[CPSSMonitorCommand::Do_Pss_Monitor_Data]u4Cpu=%d, u4MemorySize=%d, u4ConnectCount=%d, u4DataIn=%d, u4DataOut=%d.\n",
               u4Cpu,
               u4MemorySize,
               u4ConnectCount,
               u4DataIn,
               u4DataOut));

    //写入当前信息
    CDataManager::GetInstance()->AddNodeDate(pMessage->GetMessageBase()->m_szIP,
            u4Cpu,
            u4MemorySize,
            u4ConnectCount,
            u4DataIn,
            u4DataOut);

    m_pServerObject->GetPacketManager()->Delete(pBodyBuffPacket);

    //返回相应消息
    IBuffPacket* pSendBuffPacket = m_pServerObject->GetPacketManager()->Create();

    if (NULL == pSendBuffPacket)
    {
        OUR_DEBUG((LM_INFO, "[CPSSMonitorCommand::Do_Pss_Login]pBodyBuffPacket is NULL.\n"));
        return -1;
    }

    uint16 u2PostCommandID = COMMAND_MONITOR_DATA_ACK;

    (*pSendBuffPacket) << (uint32)6;
    (*pSendBuffPacket) << u2PostCommandID;
    (*pSendBuffPacket) << (uint32)0;

    m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID,
            pSendBuffPacket,
            SENDMESSAGE_JAMPNOMAL,
            u2PostCommandID,
            PACKET_SEND_IMMEDIATLY,
            PACKET_IS_FRAMEWORK_RECYC,
            0);

    return 0;
}
