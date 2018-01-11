#include "MonitorCommand.h"


CMonitorCommand::CMonitorCommand()
{
    m_emMonitorState  = MONITOR_STATE_DISCONNECT;
    m_pPostServerData = NULL;
}

CMonitorCommand::~CMonitorCommand()
{
    SAFE_DELETE(m_pPostServerData);
}

int CMonitorCommand::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    //查看当前状态，是否需要定时发送数据
    ACE_UNUSED_ARG(tv);
    ACE_UNUSED_ARG(arg);

    if(MONITOR_STATE_LOGINED == m_emMonitorState)
    {
        uint16 u2ReturnCommandID = 0;
        uint32 u4ConnectCount    = 0;

        //获得当前连接信息
        IBuffPacket* pBuffPacket = m_pServerObject->GetPacketManager()->Create();
        m_pServerObject->GetFrameCommand()->DoFrameCommand(FRAME_CONNECT_INFO, pBuffPacket);
        (*pBuffPacket) >> u2ReturnCommandID;
        (*pBuffPacket) >> u4ConnectCount;
        m_pServerObject->GetPacketManager()->Delete(pBuffPacket);

        //获得当前数据吞吐字节数
        uint32 u4CpuRote     = 0;
        uint32 u4MemorySize  = 0;
        uint8  u1Switch      = 0;
        uint32 u4DataInSize  = 0;
        uint32 u4DataOutSize = 0;
        pBuffPacket = m_pServerObject->GetPacketManager()->Create();
        m_pServerObject->GetFrameCommand()->DoFrameCommand(FRAME_CONNECT_TRAFFIC, pBuffPacket);
        (*pBuffPacket) >> u2ReturnCommandID;
        (*pBuffPacket) >> u4CpuRote;
        (*pBuffPacket) >> u4MemorySize;
        (*pBuffPacket) >> u1Switch;
        (*pBuffPacket) >> u4DataInSize;
        (*pBuffPacket) >> u4DataOutSize;
        m_pServerObject->GetPacketManager()->Delete(pBuffPacket);

        //组装发送数据包
        char szSendBuff[MAX_BUFF_100] = { '\0' };
        int nPos = 0;
        uint16 u2CommandID = COMMAND_MONITOR_DATA;
        uint16 u2IPLength = (uint16)ACE_OS::strlen(m_objMonitorPara.m_szLocalIP);
        uint32 u4PacketSize = sizeof(uint32) + sizeof(uint32) + sizeof(uint32) + sizeof(uint32) + sizeof(uint32);

        uint16 u2Version = 1;
        char szSession[32] = { '\0' };
        memcpy_safe((char*)&u2Version, sizeof(uint16), &szSendBuff[nPos], sizeof(uint16));
        nPos += sizeof(uint16);
        memcpy_safe((char*)&u2CommandID, sizeof(uint16), &szSendBuff[nPos], sizeof(uint16));
        nPos += sizeof(uint16);
        memcpy_safe((char*)&u4PacketSize, sizeof(uint32), &szSendBuff[nPos], sizeof(uint32));
        nPos += sizeof(uint32);
        memcpy_safe((char*)szSession, sizeof(char)*32, &szSendBuff[nPos], sizeof(char) * 32);
        nPos += sizeof(char) * 32;

        memcpy_safe((char*)&u4CpuRote, sizeof(uint32), &szSendBuff[nPos], sizeof(uint32));
        nPos += sizeof(uint32);
        memcpy_safe((char*)&u4MemorySize, sizeof(uint32), &szSendBuff[nPos], sizeof(uint32));
        nPos += sizeof(uint32);
        memcpy_safe((char*)&u4ConnectCount, sizeof(uint32), &szSendBuff[nPos], sizeof(uint32));
        nPos += sizeof(uint32);
        memcpy_safe((char*)&u4DataInSize, sizeof(uint32), &szSendBuff[nPos], sizeof(uint32));
        nPos += sizeof(uint32);
        memcpy_safe((char*)&u4DataOutSize, sizeof(uint32), &szSendBuff[nPos], sizeof(uint32));
        nPos += sizeof(uint32);

        char* ptrSendData = const_cast<char*>(szSendBuff);

        if (false == m_pServerObject->GetClientManager()->SendData(MONITER_SERVER_ID, ptrSendData, nPos, false))
        {
            OUR_DEBUG((LM_INFO, "[CMonitorCommand::handle_timeout] GetClientManager Send Login error.\n"));
            return -1;
        }

        OUR_DEBUG((LM_INFO, "[CMonitorCommand::handle_timeout] Send COMMAND_MONITOR_DATA OK.\n"));
    }

    return 0;
}

int CMonitorCommand::Init(const char* pJson, CServerObject* pServerObject)
{
    //测试Json
    //{"MonitorIP":"127.0.0.1","MonitorPort":"10050","MonitorIP":"127.0.0.1","TimeInterval":"60"}

    //接收配置的Json参数
    cJSON* pJsonParse = cJSON_Parse(pJson);

    if (NULL == pJsonParse)
    {
        OUR_DEBUG((LM_INFO, "[CMonitorCommand::Init](%s) is not json.\n", pJson));
        return -1;
    }

    cJSON* pJsonMonitorIP = cJSON_GetObjectItem(pJsonParse, "MonitorIP");

    if (NULL == pJsonMonitorIP)
    {
        OUR_DEBUG((LM_INFO, "[CLotDispose_Http::Get_Lot_Download_Json](%s)MonitorIP is no find.\n", pJson));
        cJSON_Delete(pJsonParse);
        return -1;
    }

    sprintf_safe(m_objMonitorPara.m_szMonitorIP, MAX_BUFF_50, "%s", pJsonMonitorIP->valuestring);

    cJSON* pJsonMonitorPort = cJSON_GetObjectItem(pJsonParse, "MonitorPort");

    if (NULL == pJsonMonitorPort)
    {
        OUR_DEBUG((LM_INFO, "[CLotDispose_Http::Get_Lot_Download_Json](%s)MonitorPort is no find.\n", pJson));
        cJSON_Delete(pJsonParse);
        return -1;
    }

    m_objMonitorPara.m_u2MonitorPort = (uint16)ACE_OS::atoi(pJsonMonitorPort->valuestring);

    cJSON* pJsonLocalIP = cJSON_GetObjectItem(pJsonParse, "LocalIP");

    if (NULL == pJsonLocalIP)
    {
        OUR_DEBUG((LM_INFO, "[CLotDispose_Http::Get_Lot_Download_Json](%s)LocalIP is no find.\n", pJson));
        cJSON_Delete(pJsonParse);
        return -1;
    }

    sprintf_safe(m_objMonitorPara.m_szLocalIP, MAX_BUFF_50, "%s", pJsonLocalIP->valuestring);

    cJSON* pJsonTimeInterval = cJSON_GetObjectItem(pJsonParse, "TimeInterval");

    if (NULL == pJsonTimeInterval)
    {
        OUR_DEBUG((LM_INFO, "[CLotDispose_Http::Get_Lot_Download_Json](%s)TimeInterval is no find.\n", pJson));
        cJSON_Delete(pJsonParse);
        return -1;
    }

    m_objMonitorPara.m_u2TimeInterval = (uint16)ACE_OS::atoi(pJsonTimeInterval->valuestring);

    m_pServerObject = pServerObject;
    cJSON_Delete(pJsonParse);
    return 0;
}



int CMonitorCommand::Connect_Monitor()
{
    m_pPostServerData = new CPostServerData();
    m_pPostServerData->Init((IMonitorCommand*)this);
    bool blRest = m_pServerObject->GetClientManager()->Connect(MONITER_SERVER_ID, m_objMonitorPara.m_szMonitorIP, m_objMonitorPara.m_u2MonitorPort, TYPE_IPV4, (IClientMessage*)m_pPostServerData);

    if (false == blRest)
    {
        OUR_DEBUG((LM_INFO, "[CMonitorCommand::Connect_Monitor] GetClientManager Connect error.\n"));
        m_emMonitorState = MONITOR_STATE_DISCONNECT;
        return -1;
    }

    return 0;
}

int CMonitorCommand::Monitor_Server_Login()
{
    m_emMonitorState = MONITOR_STATE_CONNECT;

    //发送注册数据
    char szSendBuff[MAX_BUFF_100] = { '\0' };
    int nPos                      = 0;
    uint16 u2CommandID            = COMMAND_MONITOR_LOGIN;
    uint16 u2IPLength             = (uint16)ACE_OS::strlen(m_objMonitorPara.m_szLocalIP);
    uint32 u4PacketSize           = sizeof(uint16) + u2IPLength;

    uint16 u2Version = 1;
    char szSession[32] = { '\0' };
    memcpy_safe((char*)&u2Version, sizeof(uint16), &szSendBuff[nPos], sizeof(uint16));
    nPos += sizeof(uint16);
    memcpy_safe((char*)&u2CommandID, sizeof(uint16), &szSendBuff[nPos], sizeof(uint16));
    nPos += sizeof(uint16);
    memcpy_safe((char*)&u4PacketSize, sizeof(uint32), &szSendBuff[nPos], sizeof(uint32));
    nPos += sizeof(uint32);
    memcpy_safe((char*)szSession, sizeof(char) * 32, &szSendBuff[nPos], sizeof(char) * 32);
    nPos += sizeof(char) * 32;

    memcpy_safe((char*)&u2IPLength, sizeof(uint16), &szSendBuff[nPos], sizeof(uint16));
    nPos += sizeof(uint16);
    memcpy_safe((char*)m_objMonitorPara.m_szLocalIP, u2IPLength, &szSendBuff[nPos], u2IPLength);
    nPos += u2IPLength;

    char* ptrSendData = const_cast<char*>(szSendBuff);

    if (false == m_pServerObject->GetClientManager()->SendData(MONITER_SERVER_ID, ptrSendData, nPos, false))
    {
        OUR_DEBUG((LM_INFO, "[CMonitorCommand::Monitor_Server_Login] GetClientManager Send Login error.\n"));
        return -1;
    }

    OUR_DEBUG((LM_INFO, "[CMonitorCommand::Monitor_Server_Login] Send COMMAND_MONITOR_LOGIN OK.\n"));
    return 0;
}

int CMonitorCommand::Monitor_Server_Disconnect()
{
    //这里关闭定时器
    if (-1 != m_n4TimerID)
    {
        m_pServerObject->GetTimerManager()->cancel(m_n4TimerID);
        m_n4TimerID = -1;
    }


    return 0;
}

int CMonitorCommand::Monitor_Server_Timer()
{
    //开启定时器
    if (-1 != m_n4TimerID)
    {
        m_pServerObject->GetTimerManager()->cancel(m_n4TimerID);
        m_n4TimerID = -1;
    }

    m_n4TimerID = (int32)m_pServerObject->GetTimerManager()->schedule(this, (void*)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(m_objMonitorPara.m_u2TimeInterval), ACE_Time_Value(m_objMonitorPara.m_u2TimeInterval));

    return 0;
}

void CMonitorCommand::SetState(EM_MONITOR_STATE em_monitor_state)
{
    if (MONITOR_STATE_CONNECT == em_monitor_state)
    {
        //发送注册包
        Monitor_Server_Login();
    }
    else if(MONITOR_STATE_DISCONNECT == em_monitor_state)
    {
        //与远程服务器断开连接
        Monitor_Server_Disconnect();
    }
    else
    {
        //收到注册回应包成功
        Monitor_Server_Timer();
    }

    m_emMonitorState = em_monitor_state;
}
