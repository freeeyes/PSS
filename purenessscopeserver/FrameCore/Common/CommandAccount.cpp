#include     "CommandAccount.h"

CCommandAccount::CCommandAccount()
{
}

void CCommandAccount::InitName(const char* pName, uint32 u4CommandCount)
{
    m_strName = pName;

    //��ʼ��HashTable
    m_objCommandDataList.Init((int32)u4CommandCount);
}

void CCommandAccount::Init(uint8 u1CommandAccount, uint8 u1Flow, uint16 u2PacketTimeout)
{
    m_u1Flow           = u1Flow;
    m_u1CommandAccount = u1CommandAccount;
    m_u8PacketTimeout  = (uint64)u2PacketTimeout;   //��λ�Ǻ���

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

void CCommandAccount::Close()
{
    OUR_DEBUG((LM_ERROR, "CCommandAccount::Close]Begin.\n"));
    vector<_CommandData*> vecCommandData;
    m_objCommandDataList.Get_All_Used(vecCommandData);

    for(const _CommandData* pCommandData : vecCommandData)
    {
        SAFE_DELETE(pCommandData);
    }

    //���ն˿�ͳ���ڴ�
    for (hashmapPortAccount::iterator iter = m_objectPortAccount.begin();
         iter != m_objectPortAccount.end(); iter++)
    {
        const _Port_Data_Account* p_Port_Data_Account = (*iter).int_id_;
        SAFE_DELETE(p_Port_Data_Account);
    }

    m_objectPortAccount.close();

    m_objCommandDataList.Close();
    m_u1CommandAccount = 0;
    OUR_DEBUG((LM_ERROR, "CCommandAccount::Close]End.\n"));
}

bool CCommandAccount::Save_Flow(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, ACE_Time_Value const& tvTime)
{
    ACE_UNUSED_ARG(u2CommandID);

    if (m_u1Flow == 1)
    {
        //��hashMap�в����Ƿ��Ѵ��ڶ˿���Ϣ
        _Port_Data_Account* p_Port_Data_Account = NULL;
        m_objectPortAccount.find(u2Port, p_Port_Data_Account);

        if (NULL != p_Port_Data_Account)
        {
            //�ҵ���
            p_Port_Data_Account->SetFlow(u1CommandType, u4PacketSize, tvTime);
        }
        else
        {
            //�����µĶ˿���Ϣ
            p_Port_Data_Account = new _Port_Data_Account();
            p_Port_Data_Account->Init(u1PacketType, u2Port);
            p_Port_Data_Account->SetFlow(u1CommandType, u4PacketSize, tvTime);
            m_objectPortAccount.bind(u2Port, p_Port_Data_Account);
        }
    }

    return true;
}

bool CCommandAccount::Save_Command(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, ACE_Time_Value const& tvTime)
{
    ACE_UNUSED_ARG(u2Port);

    if (m_u1CommandAccount == 0)
    {
        return true;
    }

    std::stringstream ss_format;
    ss_format << u2CommandID;
    string strCommandID = ss_format.str();

    //���Ҳ����
    _CommandData* pCommandData = m_objCommandDataList.Get_Hash_Box_Data(strCommandID.c_str());

    if (NULL != pCommandData)
    {
        //����Ѿ����ڣ���ֱ�����
        pCommandData->m_u4CommandCount++;
        pCommandData->m_u1PacketType = u1PacketType;
        pCommandData->m_u4PacketSize += u4PacketSize;
        pCommandData->m_tvCommandTime = tvTime;
    }
    else
    {
        //����µ�����ͳ����Ϣ
        pCommandData = new _CommandData();
		pCommandData->m_u2CommandID = u2CommandID;
		pCommandData->m_u4CommandCount = 1;
		pCommandData->m_u1CommandType = u1CommandType;
		pCommandData->m_u1PacketType = u1PacketType;
		pCommandData->m_u4PacketSize += u4PacketSize;
		pCommandData->m_tvCommandTime = tvTime;

		if (-1 == m_objCommandDataList.Add_Hash_Data(strCommandID.c_str(), pCommandData))
		{
			OUR_DEBUG((LM_INFO, "[CCommandAccount::SaveCommandData]szHashID=%s Add Hash Data Error.\n", strCommandID.c_str()));
		}
    }

    return true;
}

bool CCommandAccount::Save_Alert(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, ACE_Time_Value const& tvTime) const
{
    ACE_UNUSED_ARG(u2Port);
    ACE_UNUSED_ARG(u1PacketType);
    ACE_UNUSED_ARG(u4PacketSize);
    ACE_UNUSED_ARG(u1CommandType);

    ACE_Date_Time dtNowTime(tvTime);
    auto u1Minute = (uint8)dtNowTime.minute();

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
                //������ڷ�ֵ�����¼��־�����ҹ��㵱ǰ������
                commandalewrtdata.m_u4CurrCount = 0;

                AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_PACKETTIME,
                        commandalewrtdata.m_u2MailID,
                        "Alert",
                        "u2CommandID=%d, m_u4CommandCount more than [%d].",
                        u2CommandID,
                        commandalewrtdata.m_u4CommandCount);
            }

            break;
        }
    }

    return true;
}

bool CCommandAccount::SaveCommandData(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType, uint32 u4PacketSize, uint8 u1CommandType, ACE_Time_Value const& tvTime)
{
    //����������ش򿪣����¼����(��λ�Ƿ���)
    bool blRet = true;
    blRet = Save_Flow(u2CommandID, u2Port, u1PacketType, u4PacketSize, u1CommandType, tvTime);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[CCommandAccount::SaveCommandData]Save_Flow is error.\n"));
    }

    //���ͳ�ƿ��ش򿪣��ſ�ʼ��¼ͳ����Ϣ
    blRet = Save_Command(u2CommandID, u2Port, u1PacketType, u4PacketSize, u1CommandType, tvTime);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[CCommandAccount::SaveCommandData]Save_Command is error.\n"));
    }

    //�ж��Ƿ���ڸ澯��ֵ����
    blRet = Save_Alert(u2CommandID, u2Port, u1PacketType, u4PacketSize, u1CommandType, tvTime);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[CCommandAccount::SaveCommandData]Save_Command is error.\n"));
    }

    return true;
}


bool CCommandAccount::SaveCommandDataLog()
{
    //���ͳ�ƿ��ش򿪣��ſ�ʼ��¼ͳ����Ϣ
    if(m_u1CommandAccount == 0)
    {
        return true;
    }

    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_COMMANDDATA, "<Command Data Account[%s]>", m_strName.c_str());
    vector<_CommandData*> vecCommandData;
    m_objCommandDataList.Get_All_Used(vecCommandData);

    for(const _CommandData* pCommandData : vecCommandData)
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

            if(pCommandData->m_u1PacketType == EM_CONNECT_IO_TYPE::CONNECT_IO_TCP)
            {
                strPacketType = "TCP";
            }
            else
            {
                strPacketType = "UDP";
            }

            AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_COMMANDDATA, "CommandID=0x%04x, CommandType=%s, CommandCount=%d, CommandCost=%lldns, PacketType=%s, PacketSize=%d, CommandLastTime=%04d-%02d-%02d %02d:%02d:%02d%",
                                                  (int32)pCommandData->m_u2CommandID,
                                                  strCommandType.c_str(),
                                                  (int32)pCommandData->m_u4CommandCount,
                                                  pCommandData->m_u8CommandCost,
                                                  strPacketType.c_str(),
                                                  pCommandData->m_u4PacketSize,
                                                  dtLastTime.year(), dtLastTime.month(), dtLastTime.day(), dtLastTime.hour(), dtLastTime.minute(), dtLastTime.second());

        }
    }

    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_COMMANDDATA, "<(%s)Command Data Account End>", "CCommandAccount");

    return true;
}

uint32 CCommandAccount::GetFlowIn()
{
    uint32 u4FlowIn = 0;

    //����HashMap
    for (hashmapPortAccount::iterator iter = m_objectPortAccount.begin();
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

    //����HashMap
    for (hashmapPortAccount::iterator iter = m_objectPortAccount.begin();
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

uint8 CCommandAccount::GetFLow() const
{
    return m_u1Flow;
}

_CommandData* CCommandAccount::GetCommandData(uint16 u2CommandID)
{
    std::stringstream ss_format;
    ss_format << u2CommandID;
    string strCommandID = ss_format.str();
    return m_objCommandDataList.Get_Hash_Box_Data(strCommandID.c_str());
}

void CCommandAccount::GetCommandAlertData(vecCommandAlertData& CommandAlertDataList)
{
    CommandAlertDataList.insert(CommandAlertDataList.end(), m_vecCommandAlertData.begin(), m_vecCommandAlertData.end());
}

void CCommandAccount::GetFlowPortList(vector<_Port_Data_Account>& vec_Port_Data_Account)
{
    vec_Port_Data_Account.clear();
    _Port_Data_Account obj_Port_Data_Account;

    for (hashmapPortAccount::iterator iter = m_objectPortAccount.begin();
         iter != m_objectPortAccount.end(); iter++)
    {
        const _Port_Data_Account* p_Port_Data_Account = (*iter).int_id_;

        if (NULL != p_Port_Data_Account)
        {
            obj_Port_Data_Account = *p_Port_Data_Account;
            vec_Port_Data_Account.push_back(obj_Port_Data_Account);
        }
    }
}
