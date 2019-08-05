#include     "CommandAccount.h"

CCommandAccount::CCommandAccount()
{
    m_u1CommandAccount = 0;
    m_u1Flow           = 0;
    m_u1Minute         = 0;

    m_szName[0]        = '\0';

    m_u8PacketTimeout  =  MAX_QUEUE_TIMEOUT * 1000;
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
    OUR_DEBUG((LM_ERROR, "CCommandAccount::Close]Begin.\n"));
    vector<_CommandData*> vecCommandData;
    m_objCommandDataList.Get_All_Used(vecCommandData);

    for(_CommandData* pCommandData : vecCommandData)
    {
        SAFE_DELETE(pCommandData);
    }

    //回收端口统计内存
    for (ACE_Hash_Map<uint32, _Port_Data_Account*>::iterator iter = m_objectPortAccount.begin();
         iter != m_objectPortAccount.end(); iter++)
    {
        _Port_Data_Account* p_Port_Data_Account = (*iter).int_id_;
        SAFE_DELETE(p_Port_Data_Account);
    }

    m_objectPortAccount.close();

    m_objCommandDataList.Close();
    m_u1CommandAccount = 0;
    OUR_DEBUG((LM_ERROR, "CCommandAccount::Close]End.\n"));
}

bool CCommandAccount::Save_Flow(uint16 u2CommandID, uint32 u4Port, uint8 u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, ACE_Time_Value const& tvTime)
{
    ACE_UNUSED_ARG(u2CommandID);

    if (m_u1Flow == 1)
    {
        //在hashMap中查找是否已存在端口信息
        _Port_Data_Account* p_Port_Data_Account = NULL;
        m_objectPortAccount.find(u4Port, p_Port_Data_Account);

        if (NULL != p_Port_Data_Account)
        {
            //找到了
            p_Port_Data_Account->SetFlow(u1CommandType, u4PacketSize, tvTime);
        }
        else
        {
            //创建新的端口信息
            p_Port_Data_Account = new _Port_Data_Account();
            p_Port_Data_Account->Init(u1PacketType, u4Port);
            p_Port_Data_Account->SetFlow(u1CommandType, u4PacketSize, tvTime);
            m_objectPortAccount.bind(u4Port, p_Port_Data_Account);
        }
    }

    return true;
}

bool CCommandAccount::Save_Command(uint16 u2CommandID, uint32 u4Port, uint8 u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, ACE_Time_Value const& tvTime)
{
    ACE_UNUSED_ARG(u4Port);

    if (m_u1CommandAccount == 0)
    {
        return true;
    }
    else
    {
        char szHashID[10] = { '\0' };
        sprintf_safe(szHashID, 10, "%d", u2CommandID);

        //查找并添加
        _CommandData* pCommandData = m_objCommandDataList.Get_Hash_Box_Data(szHashID);

        if (NULL != pCommandData)
        {
            //如果已经存在，则直接添加
            pCommandData->m_u4CommandCount++;
            pCommandData->m_u1PacketType = u1PacketType;
            pCommandData->m_u4PacketSize += u4PacketSize;
            pCommandData->m_tvCommandTime = tvTime;
        }
        else
        {
            //添加新的命令统计信息
            pCommandData = new _CommandData();

            if (pCommandData != NULL)
            {
                pCommandData->m_u2CommandID = u2CommandID;
                pCommandData->m_u4CommandCount = 1;
                pCommandData->m_u1CommandType = u1CommandType;
                pCommandData->m_u1PacketType = u1PacketType;
                pCommandData->m_u4PacketSize += u4PacketSize;
                pCommandData->m_tvCommandTime = tvTime;

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

    return true;
}

bool CCommandAccount::Save_Alert(uint16 u2CommandID, uint32 u4Port, uint8 u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, ACE_Time_Value const& tvTime)
{
    ACE_UNUSED_ARG(u4Port);
    ACE_UNUSED_ARG(u1PacketType);
    ACE_UNUSED_ARG(u4PacketSize);
    ACE_UNUSED_ARG(u1CommandType);

    ACE_Date_Time dtNowTime(tvTime);
    uint8 u1Minute = (uint8)dtNowTime.minute();

    int32 size = (int32)m_vecCommandAlertData.size();

    for (int32 i = 0; i < size; i++)
    {
        if (m_vecCommandAlertData[i].m_u2CommandID == u2CommandID)
        {
            if (m_vecCommandAlertData[i].m_u1Minute != u1Minute)
            {
                m_vecCommandAlertData[i].m_u4CurrCount = 1;
                m_vecCommandAlertData[i].m_u1Minute = u1Minute;
            }
            else
            {
                m_vecCommandAlertData[i].m_u4CurrCount++;

                if (m_vecCommandAlertData[i].m_u4CurrCount >= m_vecCommandAlertData[i].m_u4CommandCount)
                {
                    //如果大于阀值，则记录日志，并且归零当前计数器
                    m_vecCommandAlertData[i].m_u4CurrCount = 0;

                    AppLogManager::instance()->WriteToMail(LOG_SYSTEM_PACKETTIME,
                                                           m_vecCommandAlertData[i].m_u4MailID,
                                                           (char*)"Alert",
                                                           "u2CommandID=%d, m_u4CommandCount more than [%d].",
                                                           u2CommandID,
                                                           (uint32)m_vecCommandAlertData[i].m_u4CommandCount);
                }
            }

            break;
        }
    }

    return true;
}

bool CCommandAccount::SaveCommandData(uint16 u2CommandID, uint32 u4Port, uint8 u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, ACE_Time_Value const& tvTime)
{
    //如果流量开关打开，则记录流量(单位是分钟)
    bool blRet = true;
    blRet = Save_Flow(u2CommandID, u4Port, u1PacketType, u4PacketSize, u1CommandType, tvTime);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[CCommandAccount::SaveCommandData]Save_Flow is error.\n"));
    }

    //如果统计开关打开，才开始记录统计信息
    blRet = Save_Command(u2CommandID, u4Port, u1PacketType, u4PacketSize, u1CommandType, tvTime);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[CCommandAccount::SaveCommandData]Save_Command is error.\n"));
    }

    //判定是否存在告警阀值计算
    blRet = Save_Alert(u2CommandID, u4Port, u1PacketType, u4PacketSize, u1CommandType, tvTime);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[CCommandAccount::SaveCommandData]Save_Command is error.\n"));
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

    for(_CommandData* pCommandData : vecCommandData)
    {
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

            if(pCommandData->m_u1PacketType == CONNECT_IO_TCP)
            {
                strPacketType = "TCP";
            }
            else
            {
                strPacketType = "UDP";
            }

            AppLogManager::instance()->WriteLog(LOG_SYSTEM_COMMANDDATA, "CommandID=0x%04x, CommandType=%s, CommandCount=%d, CommandCost=%lldns, PacketType=%s, PacketSize=%d, CommandLastTime=%04d-%02d-%02d %02d:%02d:%02d%",
                                                (int32)pCommandData->m_u2CommandID,
                                                strCommandType.c_str(),
                                                (int32)pCommandData->m_u4CommandCount,
                                                (uint64)pCommandData->m_u8CommandCost,
                                                strPacketType.c_str(),
                                                (uint32)pCommandData->m_u4PacketSize,
                                                dtLastTime.year(), dtLastTime.month(), dtLastTime.day(), dtLastTime.hour(), dtLastTime.minute(), dtLastTime.second());

        }
    }

    AppLogManager::instance()->WriteLog(LOG_SYSTEM_COMMANDDATA, "<Command Data Account End>");

    return true;
}

uint32 CCommandAccount::GetFlowIn()
{
    uint32 u4FlowIn = 0;

    //遍历HashMap
    for (ACE_Hash_Map<uint32, _Port_Data_Account*>::iterator iter = m_objectPortAccount.begin();
         iter != m_objectPortAccount.end(); iter++)
    {
        _Port_Data_Account* p_Port_Data_Account = (*iter).int_id_;

        if (NULL != p_Port_Data_Account)
        {
            u4FlowIn += p_Port_Data_Account->GetFlowIn();
        }
    }

    return u4FlowIn;
}

uint32 CCommandAccount::GetFlowOut()
{
    uint32 u4FlowOut = 0;

    //遍历HashMap
    for (ACE_Hash_Map<uint32, _Port_Data_Account*>::iterator iter = m_objectPortAccount.begin();
         iter != m_objectPortAccount.end(); iter++)
    {
        _Port_Data_Account* p_Port_Data_Account = (*iter).int_id_;

        if (NULL != p_Port_Data_Account)
        {
            u4FlowOut += p_Port_Data_Account->GetFlowOut();
        }
    }

    return u4FlowOut;
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

void CCommandAccount::GetCommandAlertData(vecCommandAlertData& CommandAlertDataList)
{
    CommandAlertDataList.insert(CommandAlertDataList.end(), m_vecCommandAlertData.begin(), m_vecCommandAlertData.end());
}

void CCommandAccount::GetFlowPortList(vector<_Port_Data_Account>& vec_Port_Data_Account)
{
    vec_Port_Data_Account.clear();
    _Port_Data_Account obj_Port_Data_Account;

    for (ACE_Hash_Map<uint32, _Port_Data_Account*>::iterator iter = m_objectPortAccount.begin();
         iter != m_objectPortAccount.end(); iter++)
    {
        _Port_Data_Account* p_Port_Data_Account = (*iter).int_id_;

        if (NULL != p_Port_Data_Account)
        {
            obj_Port_Data_Account = *p_Port_Data_Account;
            vec_Port_Data_Account.push_back(obj_Port_Data_Account);
        }
    }
}

void Combo_Port_List(vecPortDataAccount& vec_Port_Data_Account, vecPortDataAccount& vec_Port_Data_All_Account)
{
    int n4PartSize = (int)vec_Port_Data_Account.size();

    for (int iLoop = 0; iLoop < n4PartSize; iLoop++)
    {
        int n4AllSize = (int)vec_Port_Data_All_Account.size();
        bool bFound = false;

        for (int jLoop = 0; jLoop < n4AllSize; jLoop++)
        {
            if (vec_Port_Data_Account[iLoop].m_u4Port == vec_Port_Data_All_Account[jLoop].m_u4Port)
            {
                vec_Port_Data_All_Account[jLoop] += vec_Port_Data_Account[iLoop];
                bFound = true;
                break;
            }
        }

        if (false == bFound)
        {
            vec_Port_Data_All_Account.push_back(vec_Port_Data_Account[iLoop]);
        }
    }
}
