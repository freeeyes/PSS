#include     "CommandAccount.h"

void CCommandAccount::InitName(const char* pName, uint32 u4CommandCount)
{
    m_strName = pName;

    //初始化HashTable
    ACE_UNUSED_ARG(u4CommandCount);
}

void CCommandAccount::Init(uint8 u1CommandAccount, uint8 u1Flow, uint16 u2PacketTimeout)
{
    m_u1Flow           = u1Flow;
    m_u1CommandAccount = u1CommandAccount;
    m_u8PacketTimeout  = (uint64)u2PacketTimeout;   //单位是毫秒

    m_vecCommandAlertData.clear();
}

void CCommandAccount::AddCommandAlert(uint16 u2CommandID, uint32 u4Count, uint16 u2MailID)
{
    if(u4Count > 0)
    {
        _CommandAlertData objCommandAlertData;

        objCommandAlertData.m_u2CommandID    = u2CommandID;
        objCommandAlertData.m_u4CommandCount = u4Count;
        objCommandAlertData.m_u2MailID       = u2MailID;

        m_vecCommandAlertData.push_back(objCommandAlertData);
    }
}

void CCommandAccount::Save_Command_To_File(shared_ptr<_CommandData> pCommandData) const
{
    if (pCommandData != nullptr)
    {
        ACE_TString   strCommandType;
        ACE_TString   strPacketType;

        if (pCommandData->m_u1CommandType == COMMAND_TYPE_IN)
        {
            strCommandType = "Server In Data";
        }
        else
        {
            strCommandType = "Server Out Data";
        }

        if (pCommandData->m_u1PacketType == EM_CONNECT_IO_TYPE::CONNECT_IO_TCP)
        {
            strPacketType = "TCP";
        }
        else
        {
            strPacketType = "UDP";
        }
    }
}

void CCommandAccount::Close()
{
    PSS_LOGGER_DEBUG("CCommandAccount::Close]Begin.");

    m_objCommandDataList.clear();
    m_objectPortAccount.clear();

    m_u1CommandAccount = 0;
    PSS_LOGGER_DEBUG("CCommandAccount::Close]End.");
}

bool CCommandAccount::Save_Flow(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, PSS_Time_Point const& tvTime)
{
    ACE_UNUSED_ARG(u2CommandID);
    shared_ptr<_Port_Data_Account> p_Port_Data_Account = nullptr;

    if (m_u1Flow == 1)
    {
        //在hashMap中查找是否已存在端口信息
        hashmapPortAccount::iterator f = m_objectPortAccount.find(u2Port);

        if (m_objectPortAccount.end() != f)
        {
            //找到了
            f->second->SetFlow(u1CommandType, u4PacketSize, tvTime);
        }
        else
        {
            //创建新的端口信息
            p_Port_Data_Account = std::make_shared<_Port_Data_Account>();
            p_Port_Data_Account->Init(u1PacketType, u2Port);
            p_Port_Data_Account->SetFlow(u1CommandType, u4PacketSize, tvTime);
            m_objectPortAccount[u2Port] = p_Port_Data_Account;
        }
    }

    return true;
}

bool CCommandAccount::Save_Command(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, PSS_Time_Point const& tvTime)
{
    ACE_UNUSED_ARG(u2Port);

    if (m_u1CommandAccount == 0)
    {
        return true;
    }

    //查找并添加
    shared_ptr<_CommandData> pCommandData = nullptr;
    hashmapCommandData::iterator f = m_objCommandDataList.find(u2CommandID);

    if (m_objCommandDataList.end() != f)
    {
        //如果已经存在，则直接添加
        pCommandData = f->second;

        pCommandData->m_u4CommandCount++;
        pCommandData->m_u1PacketType = u1PacketType;
        pCommandData->m_u4PacketSize += u4PacketSize;
        pCommandData->m_tvCommandTime = tvTime;
    }
    else
    {
        //添加新的命令统计信息
        pCommandData = std::make_shared<_CommandData>();
		pCommandData->m_u2CommandID = u2CommandID;
		pCommandData->m_u4CommandCount = 1;
		pCommandData->m_u1CommandType = u1CommandType;
		pCommandData->m_u1PacketType = u1PacketType;
		pCommandData->m_u4PacketSize += u4PacketSize;
		pCommandData->m_tvCommandTime = tvTime;

        m_objCommandDataList[u2CommandID] = pCommandData;
    }

    return true;
}

bool CCommandAccount::Save_Alert(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, PSS_Time_Point const& tvTime) const
{
    ACE_UNUSED_ARG(u2Port);
    ACE_UNUSED_ARG(u1PacketType);
    ACE_UNUSED_ARG(u4PacketSize);
    ACE_UNUSED_ARG(u1CommandType);

    auto u1Minute = CTimeStamp::Get_Time_of_Minute(tvTime);

    for (_CommandAlertData commandalewrtdata : m_vecCommandAlertData)
    {
        if (commandalewrtdata.m_u2CommandID == u2CommandID)
        {
            if (commandalewrtdata.m_u1Minute != u1Minute)
            {
                commandalewrtdata.m_u4CurrCount = 1;
                commandalewrtdata.m_u1Minute = u1Minute;

                continue;
            }

            commandalewrtdata.m_u4CurrCount++;

            if (commandalewrtdata.m_u4CurrCount >= commandalewrtdata.m_u4CommandCount)
            {
                //如果大于阀值，则记录日志，并且归零当前计数器
                commandalewrtdata.m_u4CurrCount = 0;

                string strLog = fmt::format("u2CommandID=%d, m_u4CommandCount more than [%d].",
                    u2CommandID,
                    commandalewrtdata.m_u4CommandCount);

                AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_PACKETTIME,
                    commandalewrtdata.m_u2MailID,
                    "Alert",
                    strLog);
            }

            break;
        }
    }

    return true;
}

bool CCommandAccount::SaveCommandData(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, PSS_Time_Point const& tvTime)
{
    //如果流量开关打开，则记录流量(单位是分钟)
    bool blRet = true;
    blRet = Save_Flow(u2CommandID, u2Port, u1PacketType, u4PacketSize, u1CommandType, tvTime);

    if (false == blRet)
    {
        PSS_LOGGER_DEBUG("[CCommandAccount::SaveCommandData]Save_Flow is error.");
    }

    //如果统计开关打开，才开始记录统计信息
    blRet = Save_Command(u2CommandID, u2Port, u1PacketType, u4PacketSize, u1CommandType, tvTime);

    if (false == blRet)
    {
        PSS_LOGGER_DEBUG("[CCommandAccount::SaveCommandData]Save_Command is error.");
    }

    //判定是否存在告警阀值计算
    blRet = Save_Alert(u2CommandID, u2Port, u1PacketType, u4PacketSize, u1CommandType, tvTime);

    if (false == blRet)
    {
        PSS_LOGGER_DEBUG("[CCommandAccount::SaveCommandData]Save_Command is error.");
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

    string strLog = fmt::format("<Command Data Account[{0}]>.", m_strName);

    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_COMMANDDATA, strLog);

    //使用lambda表达式遍历map
    for_each(m_objCommandDataList.begin(), m_objCommandDataList.end(), [this](const std::pair<uint16, shared_ptr<_CommandData>>& iter) {
        //写入文件
        this->Save_Command_To_File(iter.second);
        });

    strLog = fmt::format("<({0}Command Data Account End>.", m_strName);
    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_COMMANDDATA, strLog);

    return true;
}

uint32 CCommandAccount::GetFlowIn()
{
    uint32 u4FlowIn = 0;

    //遍历HashMap
    for_each(m_objectPortAccount.begin(), m_objectPortAccount.end(), [&u4FlowIn](const std::pair<uint16, shared_ptr<_Port_Data_Account>>& iter) {
        auto p_Port_Data_Account = iter.second;

        if (nullptr != p_Port_Data_Account)
        {
            u4FlowIn += p_Port_Data_Account->GetFlowIn();
        }
        });

    return u4FlowIn;
}

uint32 CCommandAccount::GetFlowOut()
{
    uint32 u4FlowOut = 0;

    //遍历HashMap
    for_each(m_objectPortAccount.begin(), m_objectPortAccount.end(), [&u4FlowOut](const std::pair<uint16, shared_ptr<_Port_Data_Account>>& iter) {
        auto p_Port_Data_Account = iter.second;

        if (nullptr != p_Port_Data_Account)
        {
            u4FlowOut += p_Port_Data_Account->GetFlowOut();
        }
        });

    return u4FlowOut;
}

uint8 CCommandAccount::GetFLow() const
{
    return m_u1Flow;
}

void CCommandAccount::GetCommandAlertData(vecCommandAlertData& CommandAlertDataList)
{
    CommandAlertDataList.insert(CommandAlertDataList.end(), m_vecCommandAlertData.begin(), m_vecCommandAlertData.end());
}

void CCommandAccount::GetFlowPortList(vector<_Port_Data_Account>& vec_Port_Data_Account)
{
    vec_Port_Data_Account.clear();
    

    for_each(m_objectPortAccount.begin(), m_objectPortAccount.end(), [&vec_Port_Data_Account](const std::pair<uint16, shared_ptr<_Port_Data_Account>>& iter) {
        auto p_Port_Data_Account = iter.second;

        _Port_Data_Account obj_Port_Data_Account;
        if (nullptr != p_Port_Data_Account)
        {
            obj_Port_Data_Account = *p_Port_Data_Account;
            vec_Port_Data_Account.emplace_back(obj_Port_Data_Account);
        }
        });
}
