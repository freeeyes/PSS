#include     "CommandAccount.h"

CCommandAccount::CCommandAccount()
{
    m_u1CommandAccount = 0;
    m_u1Flow           = 0;

    ACE_Time_Value tvNow = ACE_OS::gettimeofday();
    ACE_Date_Time dtNowTime(tvNow);
    m_u1Minute = (uint8)dtNowTime.minute();

    m_u4FlowIn         = 0;
    m_u4FlowOut        = 0;

    m_u4PrvFlowIn      = 0;
    m_u4PrvFlowOut     = 0;

    m_szName[0]        = '\0';

    m_u8PacketTimeout  =  MAX_QUEUE_TIMEOUT * 1000;
}

CCommandAccount::~CCommandAccount()
{
    OUR_DEBUG((LM_ERROR, "CCommandAccount::~CCommandAccount].\n"));
    Close();
}

void CCommandAccount::InitName(const char* pName, uint32 u4CommandCount)
{
    sprintf_safe(m_szName, MAX_BUFF_50, "%s", pName);

    //初始化HashTable
    m_objCommandDataList.Init((int32)u4CommandCount);
}

void CCommandAccount::Init(uint8 u1CommandAccount, uint8 u1Flow, uint16 u2PacketTimeout)
{
    m_u1Flow           = u1Flow;
    m_u1CommandAccount = u1CommandAccount;
    m_u8PacketTimeout  = (uint64)u2PacketTimeout;   //单位是毫秒

    m_vecCommandAlertData.clear();
}

void CCommandAccount::AddCommandAlert(uint16 u2CommandID, uint32 u4Count, uint32 u4MailID)
{
    if(u4Count > 0)
    {
        _CommandAlertData objCommandAlertData;

        objCommandAlertData.m_u2CommandID    = u2CommandID;
        objCommandAlertData.m_u4CommandCount = u4Count;
        objCommandAlertData.m_u4MailID       = u4MailID;

        m_vecCommandAlertData.push_back(objCommandAlertData);
    }
}

void CCommandAccount::Close()
{
    vector<_CommandData*> vecCommandData;
    m_objCommandDataList.Get_All_Used(vecCommandData);

    for(int32 i = 0; i < (int32)vecCommandData.size(); i++)
    {
        _CommandData* pCommandData = vecCommandData[i];

        if(NULL != pCommandData)
        {
            SAFE_DELETE(pCommandData);
        }
    }

    m_objCommandDataList.Close();
    m_u1CommandAccount = 0;
}

bool CCommandAccount::SaveCommandData(uint16 u2CommandID, uint64 u8CommandCost, uint8 u1PacketType, uint32 u4PacketSize, uint32 u4CommandSize, uint8 u1CommandType, ACE_Time_Value tvTime)
{
    //统计数据到达时间，是否已经超越了限定的阀值，如果超越了，写入日志。
    if(m_u8PacketTimeout < u8CommandCost)
    {
        //记录超时的命令
        _CommandTimeOut objCommandTimeOut;
        objCommandTimeOut.m_u2CommandID   = u2CommandID;
        objCommandTimeOut.m_u4TimeOutTime = (uint32)u8CommandCost;  //转换为毫秒
        m_vecCommandTimeOut.push_back(objCommandTimeOut);
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_PACKETTIME, "u2CommandID=%d, Timeout=[%d].",
                                            u2CommandID,
                                            (uint32)u8CommandCost);
    }

    ACE_Date_Time dtNowTime(tvTime);
    uint8 u1Minute = (uint8)dtNowTime.minute();

    //如果流量开关打开，则记录流量(单位是分钟)
    if(m_u1Flow == 1)
    {
        if(m_u1Minute != u1Minute)
        {
            m_u4PrvFlowIn  = m_u4FlowIn;
            m_u4PrvFlowOut = m_u4FlowOut;

            if(u1CommandType == COMMAND_TYPE_IN)
            {
                m_u4FlowIn  = u4PacketSize;
                m_u4FlowOut = 0;
            }
            else
            {
                m_u4FlowIn  = 0;
                m_u4FlowOut = u4PacketSize;
            }

            m_u1Minute         = u1Minute;
        }
        else
        {
            if(u1CommandType == COMMAND_TYPE_IN)
            {
                m_u4FlowIn += u4PacketSize;
            }
            else
            {
                m_u4FlowOut += u4PacketSize;
            }
        }
    }

    //如果统计开关打开，才开始记录统计信息
    if(m_u1CommandAccount == 0)
    {
        return true;
    }
    else
    {
        char szHashID[10] = {'\0'};
        sprintf_safe(szHashID, 10, "%d", u2CommandID);

        //查找并添加
        _CommandData* pCommandData = m_objCommandDataList.Get_Hash_Box_Data(szHashID);

        if(NULL != pCommandData)
        {
            //如果已经存在，则直接添加
            pCommandData->m_u4CommandCount++;
            pCommandData->m_u8CommandCost += u8CommandCost;
            pCommandData->m_u1PacketType  = u1PacketType;
            pCommandData->m_u4PacketSize  += u4PacketSize;
            pCommandData->m_u4CommandSize += u4CommandSize;
            pCommandData->m_tvCommandTime = tvTime;
        }
        else
        {
            //添加新的命令统计信息
            _CommandData* pCommandData = new _CommandData();

            if(pCommandData != NULL)
            {
                pCommandData->m_u2CommandID    = u2CommandID;
                pCommandData->m_u4CommandCount = 1;
                pCommandData->m_u1CommandType  = u1CommandType;
                pCommandData->m_u8CommandCost  = u8CommandCost;
                pCommandData->m_u1PacketType   = u1PacketType;
                pCommandData->m_u4PacketSize   += u4PacketSize;
                pCommandData->m_u4CommandSize  += u4CommandSize;
                pCommandData->m_tvCommandTime  = tvTime;

                if (-1 == m_objCommandDataList.Add_Hash_Data(szHashID, pCommandData))
                {
                    OUR_DEBUG((LM_INFO, "[CCommandAccount::SaveCommandData]szHashID=%s Add Hash Data Error.\n", szHashID));
                }
            }
            else
            {
                return false;
            }
        }
    }

    //判定是否存在告警阀值计算
    if(m_vecCommandAlertData.size() > 0)
    {
        for(int32 i = 0 ; i < (int32)m_vecCommandAlertData.size(); i++)
        {
            if(m_vecCommandAlertData[i].m_u2CommandID == u2CommandID)
            {
                if(m_vecCommandAlertData[i].m_u1Minute != u1Minute)
                {
                    m_vecCommandAlertData[i].m_u4CurrCount = 1;
                    m_vecCommandAlertData[i].m_u1Minute    = u1Minute;
                }
                else
                {
                    m_vecCommandAlertData[i].m_u4CurrCount++;

                    if(m_vecCommandAlertData[i].m_u4CurrCount >= m_vecCommandAlertData[i].m_u4CommandCount)
                    {
                        //如果大于阀值，则记录日志，并且归零当前计数器
                        m_vecCommandAlertData[i].m_u4CurrCount = 0;

                        AppLogManager::instance()->WriteToMail(LOG_SYSTEM_PACKETTIME,
                                                               m_vecCommandAlertData[i].m_u4MailID,
                                                               (char* )"Alert",
                                                               "u2CommandID=%d, m_u4CommandCount more than [%d].",
                                                               u2CommandID,
                                                               (uint32) m_vecCommandAlertData[i].m_u4CommandCount);
                    }
                }

                break;
            }
        }
    }

    return true;
}


bool CCommandAccount::SaveCommandDataLog()
{
    //如果统计开关打开，才开始记录统计信息
    if(m_u1CommandAccount == 0)
    {
        return true;
    }

    AppLogManager::instance()->WriteLog(LOG_SYSTEM_COMMANDDATA, "<Command Data Account[%s]>", m_szName);
    vector<_CommandData*> vecCommandData;
    m_objCommandDataList.Get_All_Used(vecCommandData);

    for(int32 i = 0; i < (int32_t)vecCommandData.size(); i++)
    {
        _CommandData* pCommandData = vecCommandData[i];

        if(pCommandData != NULL)
        {
            ACE_Date_Time dtLastTime(pCommandData->m_tvCommandTime);
            ACE_TString   strCommandType;
            ACE_TString   strPacketType;

            if(pCommandData->m_u1CommandType == COMMAND_TYPE_IN)
            {
                strCommandType = "Server In Data";
            }
            else
            {
                strCommandType = "Server Out Data";
            }

            if(pCommandData->m_u1PacketType == PACKET_TCP)
            {
                strPacketType = "TCP";
            }
            else
            {
                strPacketType = "UDP";
            }

            AppLogManager::instance()->WriteLog(LOG_SYSTEM_COMMANDDATA, "CommandID=0x%04x, CommandType=%s, CommandCount=%d, CommandCost=%lldns, PacketType=%s, PacketSize=%d, CommandSize=%d, CommandLastTime=%04d-%02d-%02d %02d:%02d:%02d%",
                                                (int32)pCommandData->m_u2CommandID,
                                                strCommandType.c_str(),
                                                (int32)pCommandData->m_u4CommandCount,
                                                (uint64)pCommandData->m_u8CommandCost,
                                                strPacketType.c_str(),
                                                (uint32)pCommandData->m_u4PacketSize,
                                                (uint32)pCommandData->m_u4CommandSize,
                                                dtLastTime.year(), dtLastTime.month(), dtLastTime.day(), dtLastTime.hour(), dtLastTime.minute(), dtLastTime.second());

        }
    }

    AppLogManager::instance()->WriteLog(LOG_SYSTEM_COMMANDDATA, "<Command Data Account End>");

    return true;
}

void CCommandAccount::ClearTimeOut()
{
    m_vecCommandTimeOut.clear();
}

uint32 CCommandAccount::GetTimeoutCount()
{
    return (uint32)m_vecCommandTimeOut.size();
}

_CommandTimeOut* CCommandAccount::GetTimeoutInfo(uint32 u4Index)
{
    if(u4Index >= m_vecCommandTimeOut.size())
    {
        return NULL;
    }

    return (_CommandTimeOut* )&m_vecCommandTimeOut[u4Index];
}

uint32 CCommandAccount::GetFlowIn()
{
    ACE_Time_Value tvTime = ACE_OS::gettimeofday();
    ACE_Date_Time dtNowTime(tvTime);
    uint8 u1Minute = (uint8)dtNowTime.minute();

    if(m_u1Minute != u1Minute)
    {
        m_u4PrvFlowIn  = m_u4FlowIn;
        m_u4PrvFlowOut = m_u4FlowOut;

        m_u4FlowIn  = 0;
        m_u4FlowOut = 0;

        m_u1Minute         = u1Minute;
    }

    return m_u4FlowIn;
}

uint32 CCommandAccount::GetFlowOut()
{
    ACE_Time_Value tvTime = ACE_OS::gettimeofday();
    ACE_Date_Time dtNowTime(tvTime);
    uint8 u1Minute = (uint8)dtNowTime.minute();

    if(m_u1Minute != u1Minute)
    {
        m_u4PrvFlowIn  = m_u4FlowIn;
        m_u4PrvFlowOut = m_u4FlowOut;

        m_u4FlowIn  = 0;
        m_u4FlowOut = 0;

        m_u1Minute         = u1Minute;
    }


    return m_u4FlowOut;
}

uint8 CCommandAccount::GetFLow()
{
    return m_u1Flow;
}

_CommandData* CCommandAccount::GetCommandData(uint16 u2CommandID)
{
    char szHashID[10] = {'\0'};
    sprintf_safe(szHashID, 10, "%d", u2CommandID);
    _CommandData* pCommandData = m_objCommandDataList.Get_Hash_Box_Data(szHashID);
    return pCommandData;
}

_CommandFlowAccount CCommandAccount::GetCommandFlowAccount()
{
    _CommandFlowAccount objCommandFlowAccount;

    objCommandFlowAccount.m_u1FLow    = GetFLow();
    objCommandFlowAccount.m_u4FlowIn  = GetFlowIn();
    objCommandFlowAccount.m_u4FlowOut = GetFlowOut();

    return objCommandFlowAccount;

}

void CCommandAccount::GetCommandTimeOut(vecCommandTimeOut& CommandTimeOutList)
{
    CommandTimeOutList.insert(CommandTimeOutList.end(), m_vecCommandTimeOut.begin(), m_vecCommandTimeOut.end());
}

void CCommandAccount::GetCommandAlertData(vecCommandAlertData& CommandAlertDataList)
{
    CommandAlertDataList.insert(CommandAlertDataList.end(), m_vecCommandAlertData.begin(), m_vecCommandAlertData.end());
}