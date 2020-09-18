// MessageService.h
// ������Ϣ���߳��࣬Connect�ὫҪ�����CMessage���������Щ�߳��д���
// ��Ϊ��Щ���£���Ϊ�����˾ͻ�������ȴ���֣������ȴ�����ң������ģ����±����������
// add by freeeyes
// 2009-01-26

#include "MessageService.h"

CMessageService::CMessageService(): m_cond(m_mutex)
{
    uint16 u2ThreadTimeOut = GetXmlConfigAttribute(xmlThreadInfo)->ThreadTimeout;

    if(u2ThreadTimeOut == 0)
    {
        m_u2ThreadTimeOut = MAX_MSG_THREADTIMEOUT;
    }
    else
    {
        m_u2ThreadTimeOut = u2ThreadTimeOut;
    }
}

void CMessageService::Init(uint32 u4ThreadID, uint32 u4MaxQueue, uint32 u4LowMask, uint32 u4HighMask, bool blIsCpuAffinity)
{
    m_u4MaxQueue    = u4MaxQueue;
    m_u4HighMask    = u4HighMask;
    m_u4LowMask     = u4LowMask;

    //����߳���Ϣ
    m_u4ThreadID = u4ThreadID;
    m_ThreadInfo.m_u4ThreadID   = u4ThreadID;

    //���õ�ǰ�߳��Ƿ�CPU��
    m_blIsCpuAffinity = blIsCpuAffinity;

    m_u4WorkQueuePutTime = GetXmlConfigAttribute(xmlThreadInfo)->PutQueueTimeout * 1000;

    //��ʼ���߳�AI
    m_WorkThreadAI.Init(GetXmlConfigAttribute(xmlThreadInfoAI)->AI,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->TimeoutCount,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->CheckTime,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->TimeoutCount,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->StopTime,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->ReturnDataType,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->ReturnData.c_str());

    //�����̳߳�ʼ��ͳ��ģ�������
    char szName[MAX_BUFF_50] = {'\0'};
    sprintf_safe(szName, MAX_BUFF_50, "�����߳�(%d)", u4ThreadID);
    m_CommandAccount.InitName(szName, GetXmlConfigAttribute(xmlCommandAccount)->MaxCommandCount);

    //��ʼ��ͳ��ģ�鹦��
    m_CommandAccount.Init(GetXmlConfigAttribute(xmlCommandAccount)->Account,
                          GetXmlConfigAttribute(xmlCommandAccount)->FlowAccount,
                          GetXmlConfigAttribute(xmlThreadInfo)->ThreadTimeout);

    //��ʼ�����������б���
    m_objClientCommandList.Init(App_MessageManager::instance()->GetMaxCommandCount());

    //��ʼ��CommandID�澯��ֵ���
    for(const auto& objCommandInfo : GetXmlConfigAttribute(xmlCommandInfos)->vec)
    {
        m_CommandAccount.AddCommandAlert(objCommandInfo.CommandID,
                                         objCommandInfo.CommandCount,
                                         objCommandInfo.MailID);
    }

    //������Ϣ��
    m_MessagePool.Init(GetXmlConfigAttribute(xmlMessage)->Msg_Pool, CMessagePool::Init_Callback);
}

bool CMessageService::Start()
{
    if(0 != open())
    {
        m_emThreadState = THREAD_STOP;
        return false;
    }
    else
    {
        m_emThreadState = THREAD_RUN;
    }

    return true;
}

int CMessageService::open()
{
    m_blRun = true;
    msg_queue()->high_water_mark(m_u4HighMask);
    msg_queue()->low_water_mark(m_u4LowMask);

    OUR_DEBUG((LM_INFO,"[CMessageService::open] m_u4HighMask = [%d] m_u4LowMask = [%d]\n", m_u4HighMask, m_u4LowMask));

    if(activate(THREAD_PARAM, MAX_MSG_THREADCOUNT) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CMessageService::open] activate error ThreadCount = [%d].\n", MAX_MSG_THREADCOUNT));
        m_blRun = false;
        return -1;
    }

    resume();
    return 0;
}

int CMessageService::svc(void)
{
    //�ж��Ƿ�Ҫ��CPU
	//�ж��Ƿ�Ҫ��CPU
	if (true == m_blIsCpuAffinity)
	{
#if PSS_PLATFORM == PLATFORM_WIN
		SetThreadAffinityMask(GetCurrentThread(), m_u4ThreadID);
#else
		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(m_u4ThreadID, &mask);

		if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) != 0)
		{
			OUR_DEBUG((LM_ERROR, "[CMessageService::svc](%d)cound not get thread affinity.\n", m_u4ThreadID));
		}

#endif
	}

    while(m_blRun)
    {
        if (false == Dispose_Queue())
        {
            break;
        }

        //ʹ���ڴ�أ�����ڴ治�����ͷ�
    }

    OUR_DEBUG((LM_INFO,"[CMessageService::svc] svc finish!\n"));
    return 0;
}

bool CMessageService::PutMessage(CMessage* pMessage)
{
    ACE_Message_Block* mb = pMessage->GetQueueMessage();

    if(NULL != mb)
    {
        //�ж϶����Ƿ����Ѿ����
        int nQueueCount = (int)msg_queue()->message_count();

        if(nQueueCount >= (int)m_u4MaxQueue)
        {
            OUR_DEBUG((LM_ERROR,"[CMessageService::PutMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));
            return false;
        }

        ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, m_u4WorkQueuePutTime);

        if(this->putq(mb, &xtime) == -1)
        {
            OUR_DEBUG((LM_ERROR,"[CMessageService::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));
            return false;
        }
    }
    else
    {
        OUR_DEBUG((LM_ERROR,"[CMessageService::PutMessage] mb new error.\n"));
        return false;
    }

	//���Դ���
	if (0 == m_u2PutMessageCount && pMessage->GetMessageBase()->m_u2Cmd >= CLIENT_LINK_USER)
	{
		m_avPutBegin = ACE_OS::gettimeofday();
        OUR_DEBUG((LM_INFO, "[CMessageService::PutMessage]m_avPutBegin=%d.\n", m_avPutBegin.sec()));
		m_u2PutMessageCount++;
	}
	else if(pMessage->GetMessageBase()->m_u2Cmd >= CLIENT_LINK_USER)
	{
        m_u2PutMessageCount++;
        if (m_u2PutMessageCount >= 1000)
        {
            m_avPutEnd = ACE_OS::gettimeofday();
            ACE_Time_Value tvInterval = m_avPutEnd - m_avPutBegin;
            OUR_DEBUG((LM_INFO, "[CMessageService::PutMessage]m_avPutEnd=%d.\n", m_avPutEnd.sec()));
            OUR_DEBUG((LM_INFO, "[CMessageService::PutMessage]tvInterval=%d.\n", tvInterval.msec()));
            m_u2PutMessageCount = 0;
        }
	}

    return true;
}

bool CMessageService::PutUpdateCommandMessage(uint32 u4UpdateIndex)
{
    ACE_Message_Block* mblk = App_MessageBlockManager::instance()->Create(sizeof(int));

    if (NULL == mblk)
    {
        return false;
    }

    memcpy_safe((char* )&u4UpdateIndex, sizeof(int), mblk->wr_ptr(), sizeof(int));
    mblk->wr_ptr(sizeof(int));

    mblk->msg_type(ACE_Message_Block::MB_USER);

    //�ж϶����Ƿ����Ѿ����
    int nQueueCount = (int)msg_queue()->message_count();

    if (nQueueCount >= (int)m_u4MaxQueue)
    {
        OUR_DEBUG((LM_ERROR, "[CMessageService::PutUpdateCommandMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));
        return false;
    }

    ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, m_u4WorkQueuePutTime);

    if (this->putq(mblk, &xtime) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CMessageService::PutUpdateCommandMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));
        return false;
    }

    return true;
}

bool CMessageService::ProcessMessage(CMessage* pMessage, uint32 u4ThreadID)
{
    if(NULL == pMessage)
    {
        OUR_DEBUG((LM_ERROR,"[CMessageService::ProcessMessage] [%d]pMessage is NULL.\n", u4ThreadID));
        return false;
    }

    if(NULL == pMessage->GetMessageBase())
    {
        OUR_DEBUG((LM_ERROR,"[CMessageService::ProcessMessage] [%d]pMessage->GetMessageBase() is NULL.\n", u4ThreadID));
        DeleteMessage(pMessage);
        return false;
    }

    //����������߳��Լ����
    m_ThreadInfo.m_tvUpdateTime = ACE_OS::gettimeofday();
    m_ThreadInfo.m_u4State = THREADSTATE::THREAD_RUNBEGIN;

    //�ж϶��д���ʱ���Ƿ񳬹�����������е�ʱ��
    ACE_Time_Value tvQueueDispose(m_ThreadInfo.m_tvUpdateTime - pMessage->GetMessageBase()->m_tvRecvTime);

    if (tvQueueDispose.msec() > (uint32)GetXmlConfigAttribute(xmlThreadInfo)->DisposeTimeout)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_COMMANDDATA, "[CMessageService::ProcessMessage]CommandID=0x%04x, Queue put dispose time interval(%d).\n",
                                              (int)pMessage->GetMessageBase()->m_u2Cmd,
                                              tvQueueDispose.msec());
    }

    //��Ҫ��������ݷŵ��߼�����ĵط�ȥ
    uint16 u2CommandID = 0;          //���ݰ���CommandID

    u2CommandID = pMessage->GetMessageBase()->m_u2Cmd;

    //�׳������ӽ����ͶϿ���ֻ�����߼����ݰ�
    if(pMessage->GetMessageBase()->m_u2Cmd >= CLIENT_LINK_USER)
    {
        m_ThreadInfo.m_u4RecvPacketCount++;
        m_ThreadInfo.m_u4CurrPacketCount++;
        m_ThreadInfo.m_u2CommandID   = u2CommandID;

        bool blIsDead = m_WorkThreadAI.CheckCurrTimeout(pMessage->GetMessageBase()->m_u2Cmd, (uint64)m_ThreadInfo.m_tvUpdateTime.sec());

        if(blIsDead == true)
        {
            OUR_DEBUG((LM_ERROR,"[CMessageService::ProcessMessage]Command(%d) is Delele.\n", pMessage->GetMessageBase()->m_u2Cmd));
            //ֱ�ӷ���Ӧ�����ݸ��ͻ��ˣ����ڵ��߼���ȥ����

            char* ptrReturnData = m_WorkThreadAI.GetReturnData();
#if PSS_PLATFORM == PLATFORM_WIN
            App_ProConnectManager::instance()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID,
                    ptrReturnData,
                    m_WorkThreadAI.GetReturnDataLength(),
                    SENDMESSAGE_NOMAL,
                    (uint16)COMMAND_RETURN_BUSY,
                    PACKET_SEND_IMMEDIATLY,
                    PACKET_IS_SELF_RECYC);
#else
            App_ConnectManager::instance()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID,
                    ptrReturnData,
                    m_WorkThreadAI.GetReturnDataLength(),
                    SENDMESSAGE_NOMAL,
                    COMMAND_RETURN_BUSY,
                    PACKET_SEND_IMMEDIATLY,
                    PACKET_IS_SELF_RECYC);
#endif
            DeleteMessage(pMessage);
            m_ThreadInfo.m_u4State = THREADSTATE::THREAD_RUNEND;

            return true;
        }
    }

    uint32 u4TimeCost     = 0;      //����ִ��ʱ��
    uint16 u2CommandCount = 0;      //������ô���
    bool   blDeleteFlag   = true;   //�����Ƿ�ɾ����Ĭ����ɾ��

		//���Դ���
	if (0 == m_u2MessageCount && pMessage->GetMessageBase()->m_u2Cmd >= CLIENT_LINK_USER)
	{
		m_avBegin = ACE_OS::gettimeofday();
		m_u2MessageCount++;
	}
	else if (pMessage->GetMessageBase()->m_u2Cmd >= CLIENT_LINK_USER)
	{
		m_u2MessageCount++;
		if (m_u2MessageCount >= 1000)
		{
			m_avEnd = ACE_OS::gettimeofday();
			ACE_Time_Value tvInterval = m_avEnd - m_avBegin;
			OUR_DEBUG((LM_INFO, "[CMessageService::DoMessage]tvInterval=%d.\n", tvInterval.msec()));
			m_u2MessageCount = 0;
		}
	}

    DoMessage(m_ThreadInfo.m_tvUpdateTime, pMessage, u2CommandID, u4TimeCost, u2CommandCount, blDeleteFlag);

    if(pMessage->GetMessageBase()->m_u2Cmd >= CLIENT_LINK_USER)
    {
        //���AI�����ˣ������������AI�ж�
        m_WorkThreadAI.SaveTimeout(pMessage->GetMessageBase()->m_u2Cmd, u4TimeCost);

        if(u2CommandCount > 0)
        {
            //��õ��������ִ��ʱ��
            u4TimeCost = u4TimeCost/u2CommandCount;
        }

        //���ͳ����Ϣ
        m_CommandAccount.SaveCommandData(u2CommandID,
                                         (uint16)pMessage->GetMessageBase()->m_u4ListenPort,
                                         pMessage->GetMessageBase()->m_u1PacketType,
                                         pMessage->GetMessageBase()->m_u4HeadSrcSize + pMessage->GetMessageBase()->m_u4BodySrcSize,
                                         COMMAND_TYPE_IN,
                                         m_ThreadInfo.m_tvUpdateTime);
    }

    if (true == blDeleteFlag)
    {
        DeleteMessage(pMessage);
    }

    m_ThreadInfo.m_u4State = THREADSTATE::THREAD_RUNEND;

    //��ʼ�������ݰ������ʱ��
    if(m_ThreadInfo.m_u2PacketTime == 0)
    {
        m_ThreadInfo.m_u2PacketTime = (uint16)u4TimeCost;
    }
    else
    {
        //�������ݰ���ƽ������ʱ��
        m_ThreadInfo.m_u2PacketTime = (uint16)((m_ThreadInfo.m_u2PacketTime + (uint16)u4TimeCost)/2);
    }

    return true;
}

int CMessageService::Close()
{
    if(m_blRun)
    {
        if (false == this->CloseMsgQueue())
        {
            OUR_DEBUG((LM_INFO, "[CMessageService::Close]CloseMsgQueue is fail.\n"));
        }
    }
    else
    {
        msg_queue()->deactivate();
    }

    CloseCommandList();

    m_objClientCommandList.Close();

    m_CommandAccount.Close();

    m_MessagePool.Close_Object(CMessagePool::Close_Callback);

    m_WorkThreadAI.Close();

    OUR_DEBUG((LM_INFO, "[CMessageService::close] Close().\n"));
    return 0;
}

bool CMessageService::SaveThreadInfoData(const ACE_Time_Value& tvNow)
{
    //��������߳��Լ�
    ACE_Date_Time dt(m_ThreadInfo.m_tvUpdateTime);

    //��ӵ��߳���Ϣ��ʷ���ݱ�
    _ThreadInfo objCurrThreadInfo    = m_ThreadInfo;
    objCurrThreadInfo.m_tvUpdateTime = ACE_OS::gettimeofday();

    //��ʼ�鿴�߳��Ƿ�ʱ
    if(m_ThreadInfo.m_u4State == THREADSTATE::THREAD_RUNBEGIN && tvNow.sec() - m_ThreadInfo.m_tvUpdateTime.sec() > m_u2ThreadTimeOut)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_WORKTHREAD, "[CMessageService::handle_timeout] pThreadInfo = [%d] State = [%d] Time = [%04d-%02d-%02d %02d:%02d:%02d] PacketCount = [%d] LastCommand = [0x%x] PacketTime = [%d] TimeOut > %d[%d] CurrPacketCount = [%d] QueueCount = [%d] BuffPacketUsed = [%d] BuffPacketFree = [%d].",
                                              m_ThreadInfo.m_u4ThreadID,
                                              m_ThreadInfo.m_u4State,
                                              dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second(),
                                              m_ThreadInfo.m_u4RecvPacketCount,
                                              m_ThreadInfo.m_u2CommandID,
                                              m_ThreadInfo.m_u2PacketTime,
                                              m_u2ThreadTimeOut,
                                              tvNow.sec() - m_ThreadInfo.m_tvUpdateTime.sec(),
                                              m_ThreadInfo.m_u4CurrPacketCount,
                                              (int)msg_queue()->message_count(),
                                              App_BuffPacketManager::instance()->GetBuffPacketUsedCount(),
                                              App_BuffPacketManager::instance()->GetBuffPacketFreeCount());

        //���������̣߳���Ҫ������Ӧ���߳�
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_WORKTHREAD, "[CMessageService::handle_timeout] ThreadID = [%d] Thread is reset.", m_u4ThreadID);
        return false;
    }
    else
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_WORKTHREAD, "[CMessageService::handle_timeout] pThreadInfo = [%d] State = [%d] Time = [%04d-%02d-%02d %02d:%02d:%02d] PacketCount = [%d] LastCommand = [0x%x] PacketTime = [%d] CurrPacketCount = [%d] QueueCount = [%d] BuffPacketUsed = [%d] BuffPacketFree = [%d].",
                                              m_ThreadInfo.m_u4ThreadID,
                                              m_ThreadInfo.m_u4State,
                                              dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second(),
                                              m_ThreadInfo.m_u4RecvPacketCount,
                                              m_ThreadInfo.m_u2CommandID,
                                              m_ThreadInfo.m_u2PacketTime,
                                              m_ThreadInfo.m_u4CurrPacketCount,
                                              (int)msg_queue()->message_count(),
                                              App_BuffPacketManager::instance()->GetBuffPacketUsedCount(),
                                              App_BuffPacketManager::instance()->GetBuffPacketFreeCount());

        m_ThreadInfo.m_u4CurrPacketCount = 0;
        return true;
    }
}

void CMessageService::CloseCommandList()
{
    //����ǰ�����б�
    vector<CClientCommandList*> vecClientCommandList;
    m_objClientCommandList.Get_All_Used(vecClientCommandList);

    uint32 u4Size = (uint32)vecClientCommandList.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        vecClientCommandList[i]->Close();
        SAFE_DELETE(vecClientCommandList[i]);
    }

    m_objClientCommandList.Clear_Unit32();
}

CClientCommandList* CMessageService::GetClientCommandList(uint16 u2CommandID)
{
    return m_objClientCommandList.Get_Hash_Box_Data_By_Uint32((uint32)u2CommandID);
}

bool CMessageService::DoMessage(const ACE_Time_Value& tvBegin, IMessage* pMessage, uint16& u2CommandID, uint32& u4TimeCost, uint16& u2Count, bool& bDeleteFlag)
{
    if (NULL == pMessage)
    {
        OUR_DEBUG((LM_ERROR, "[CMessageService::DoMessage] pMessage is NULL.\n"));
        return false;
    }

    //���Դ���
    if (0 == m_u2MessageCount && pMessage->GetMessageBase()->m_u2Cmd >= CLIENT_LINK_USER)
    {
        m_avBegin = ACE_OS::gettimeofday();
        m_u2MessageCount++;
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd >= CLIENT_LINK_USER)
    {
        m_u2MessageCount++;
        if (m_u2MessageCount >= 1000)
        {
			m_avEnd = ACE_OS::gettimeofday();
			ACE_Time_Value tvInterval = m_avEnd - m_avBegin;
			OUR_DEBUG((LM_INFO, "[CMessageService::DoMessage]tvInterval=%d.\n", tvInterval.msec()));
			m_u2MessageCount = 0;
        }
    }

    //�Ÿ���Ҫ�̳е�ClientCommand��ȥ����
    CClientCommandList* pClientCommandList = GetClientCommandList(u2CommandID);

    if (NULL == pClientCommandList)
    {
		//û���ҵ���Ӧ��ע��ָ��������define.h������쳣�����¼�쳣������־
		if (u2CommandID >= CLIENT_LINK_USER)
		{
			AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_ERROR, "[CommandID=%d][HeadLen=%d][BodyLen=%d] is not plugin dispose.",
				u2CommandID,
				pMessage->GetMessageBase()->m_u4HeadSrcSize,
				pMessage->GetMessageBase()->m_u4BodySrcSize);
		}

        OUR_DEBUG((LM_ERROR, "[CMessageService::DoMessage] pClientCommandList no find.\n"));
        return false;
    }

    int nCount = pClientCommandList->GetCount();

    for (int i = 0; i < nCount; i++)
    {
        const _ClientCommandInfo* pClientCommandInfo = pClientCommandList->GetClientCommandIndex(i);

        if (NULL != pClientCommandInfo)
        {
            //�жϵ�ǰ��Ϣ�Ƿ���ָ���ļ����˿�
            if (pClientCommandInfo->m_objListenIPInfo.m_u2Port > 0 &&
                (ACE_OS::strcmp(pClientCommandInfo->m_objListenIPInfo.m_szClientIP, pMessage->GetMessageBase()->m_szListenIP) != 0 ||
                    (uint32)pClientCommandInfo->m_objListenIPInfo.m_u2Port != pMessage->GetMessageBase()->m_u4ListenPort))
            {
                continue;
            }

            //��ǵ�ǰ��������״̬
            pClientCommandInfo->m_pClientCommand->DoMessage(pMessage, bDeleteFlag);

            //����ָ��¼���������
            ACE_Time_Value tvCost = ACE_OS::gettimeofday() - tvBegin;
            u4TimeCost = (uint32)tvCost.msec();

            //��¼������ô���
            u2Count++;
        }
    }

    //�ж��Ƿ���Ҫ��¼��ʱ��־
    if (pClientCommandList->GetCommandTimeout() > 0 && u4TimeCost >= pClientCommandList->GetCommandTimeout())
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_WORKTHREAD, "ThreadID=%d, CommandID=%d, Timeout=%d ms, Cost time=%d.",
                                                m_u4ThreadID,
                                                u2CommandID,
                                                pClientCommandList->GetCommandTimeout(),
                                                u4TimeCost);
    }

    return true;
}

_ThreadInfo* CMessageService::GetThreadInfo()
{
    return &m_ThreadInfo;
}

void CMessageService::GetAIInfo(_WorkThreadAIInfo& objAIInfo) const
{
    m_WorkThreadAI.GetAIInfo(objAIInfo);
}

uint32 CMessageService::GetThreadID() const
{
    return m_u4ThreadID;
}

void CMessageService::CopyMessageManagerList()
{
    CloseCommandList();

    CHashTable<CClientCommandList>* pHashtCommandList = App_MessageManager::instance()->GetHashCommandList();

    if (NULL == pHashtCommandList)
    {
        return;
    }

	vector<CClientCommandList*> vecClientCommandList;
	pHashtCommandList->Get_All_Used(vecClientCommandList);

	for (CClientCommandList* pClientCommandList : vecClientCommandList)
	{
        if (NULL == pClientCommandList)
        {
            continue;
        }

		CClientCommandList* pCurrClientCommandList = new CClientCommandList();

		if (NULL == pCurrClientCommandList)
		{
			continue;
		}

		pCurrClientCommandList->SetCommandID(pClientCommandList->GetCommandID());
		pCurrClientCommandList->SetCommandTimeout(pClientCommandList->GetCommandTimeout());

		for (int j = 0; j < pClientCommandList->GetCount(); j++)
		{
			const _ClientCommandInfo* pClientCommandInfo = pClientCommandList->GetClientCommandIndex(j);

			if (NULL != pClientCommandInfo)
			{
				pCurrClientCommandList->AddClientCommand(pClientCommandInfo->m_pClientCommand, pClientCommandInfo->m_szModuleName, NULL);
			}
		}

		if (0 > m_objClientCommandList.Add_Hash_Data_By_Key_Unit32((uint32)pClientCommandList->GetCommandID(), pCurrClientCommandList))
		{
			OUR_DEBUG((LM_INFO, "[CMessageService::CopyMessageManagerList]CommandID=%d add error.\n", pClientCommandList->GetCommandID()));
			SAFE_DELETE(pCurrClientCommandList);
		}
	}
}

void CMessageService::GetAITO(vecCommandTimeout& objTimeout) const
{
    m_WorkThreadAI.GetAllTimeout(m_u4ThreadID, objTimeout);
}

void CMessageService::GetAITF(vecCommandTimeout& objTimeout) const
{
    m_WorkThreadAI.GetAllForbiden(m_u4ThreadID, objTimeout);
}

void CMessageService::SetAI(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime)
{
    m_WorkThreadAI.ReSet(u1AI, u4DisposeTime, u4WTCheckTime, u4WTStopTime);
}

_CommandData* CMessageService::GetCommandData(uint16 u2CommandID)
{
    return m_CommandAccount.GetCommandData(u2CommandID);
}

void CMessageService::GetFlowInfo(uint32& u4FlowIn, uint32& u4FlowOut)
{
    u4FlowIn  = m_CommandAccount.GetFlowIn();
    u4FlowOut = m_CommandAccount.GetFlowOut();
}

void CMessageService::GetCommandAlertData(vecCommandAlertData& CommandAlertDataList)
{
    m_CommandAccount.GetCommandAlertData(CommandAlertDataList);
}

void CMessageService::SaveCommandDataLog()
{
    m_CommandAccount.SaveCommandDataLog();
}

void CMessageService::SetThreadState(MESSAGE_SERVICE_THREAD_STATE emState)
{
    m_emThreadState = emState;
}

MESSAGE_SERVICE_THREAD_STATE CMessageService::GetThreadState() const
{
    return m_emThreadState;
}

THREADSTATE CMessageService::GetStepState() const
{
    return m_ThreadInfo.m_u4State;
}

uint32 CMessageService::GetUsedMessageCount()
{
    return (uint32)m_MessagePool.GetUsedCount();
}

CMessage* CMessageService::CreateMessage()
{
    CMessage* pMessage = m_MessagePool.Create();

    if(NULL != pMessage)
    {
        pMessage->GetMessageBase()->m_u4WorkThreadID = GetThreadID();
    }

    return pMessage;
}

void CMessageService::DeleteMessage(CMessage* pMessage)
{
    if (false == m_MessagePool.Delete(pMessage))
    {
        OUR_DEBUG((LM_INFO, "[CMessageService::DeleteMessage]pMessage == NULL.\n"));
    }
}

void CMessageService::GetFlowPortList(vector<_Port_Data_Account>& vec_Port_Data_Account)
{
    m_CommandAccount.GetFlowPortList(vec_Port_Data_Account);
}

int CMessageService::handle_signal(int signum, siginfo_t* siginfo, ucontext_t* ucontext)
{
    if (signum == SIGUSR1 + grp_id())
    {
        OUR_DEBUG((LM_INFO,"[CMessageService::handle_signal](%d) will be kill.\n", grp_id()));

        if(NULL != siginfo && NULL != ucontext)
        {
            OUR_DEBUG((LM_INFO,"[CMessageService::handle_signal]siginfo is not null.\n"));
        }

        ACE_Thread::exit();
    }

    return 0;
}

int CMessageService::CloseMsgQueue()
{
    return Task_Common_CloseMsgQueue((ACE_Task<ACE_MT_SYNCH>*)this, m_cond, m_mutex);
}

void CMessageService::UpdateCommandList(const ACE_Message_Block* pmb)
{
    uint32 u4UpdateIndex = 0;
    memcpy_safe(pmb->rd_ptr(), sizeof(int), (char*)&u4UpdateIndex, sizeof(int));
    OUR_DEBUG((LM_ERROR, "[CMessageService::svc](%d)<UpDateIndex=%d>CopyMessageManagerList.\n", m_ThreadInfo.m_u4ThreadID, u4UpdateIndex));

    if (u4UpdateIndex > 0)
    {
        int nReload = App_ModuleLoader::instance()->UnloadListUpdate(u4UpdateIndex);

        if (1 == nReload)
        {
            //��Ҫ֪ͨ����ٸ���һ�¸���(���µļ�����Ч)
            App_MessageServiceGroup::instance()->PutUpdateCommandMessage(App_MessageManager::instance()->GetUpdateIndex());
        }

        //ͬ�������б�
        CopyMessageManagerList();
    }
}

bool CMessageService::Dispose_Queue()
{
    ACE_Message_Block* mb = NULL;
    ACE_OS::last_error(0);

    if (getq(mb, 0) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CMessageService::Dispose_Queue] PutMessage error errno = [%d].\n", ACE_OS::last_error()));
        m_blRun = false;
        return false;
    }
    else if (mb->msg_type() == ACE_Message_Block::MB_USER)
    {
        UpdateCommandList(mb);

        App_MessageBlockManager::instance()->Close(mb);
        return true;
    }
    else if (mb->msg_type() == ACE_Message_Block::MB_STOP)
    {
        m_mutex.acquire();
        mb->release();
        this->msg_queue()->deactivate();
        m_cond.signal();
        m_mutex.release();
        m_blRun = false;
        return false;
    }
    else
    {
        CMessage* msg = *((CMessage**)mb->base());

        if (false == this->ProcessMessage(msg, m_u4ThreadID))
        {
            OUR_DEBUG((LM_ERROR, "[CMessageService::svc](%d)ProcessMessage is false!\n", m_u4ThreadID));
        }

        return true;
    }
}

//==========================================================
CMessageServiceGroup::CMessageServiceGroup()
{
    uint16 u2ThreadTimeCheck = GetXmlConfigAttribute(xmlThreadInfo)->ThreadTimeCheck;

    if(u2ThreadTimeCheck == 0)
    {
        m_u2ThreadTimeCheck = MAX_MSG_TIMEDELAYTIME;
    }
    else
    {
        m_u2ThreadTimeCheck = u2ThreadTimeCheck;
    }
}

int CMessageServiceGroup::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    ACE_UNUSED_ARG(arg);

    //������й����߳�
    if (false == CheckWorkThread(tv))
    {
        OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::handle_timeout]CheckWorkThread is fail.\n"));
    }

    //��¼PacketParse��ͳ�ƹ���
    if (false == CheckPacketParsePool())
    {
        OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::handle_timeout]CheckPacketParsePool is fail.\n"));
    }

    //��¼ͳ��CPU���ڴ��ʹ��
    if (false == CheckCPUAndMemory())
    {
        OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::handle_timeout]CheckCPUAndMemory is fail.\n"));
    }

    //������в��״̬
    if (false == CheckPlugInState())
    {
        OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::handle_timeout]CheckPlugInState is fail.\n"));
    }

    //�����־�ļ�
    ofstream* pLogoStream = (ofstream*)ACE_LOG_MSG->msg_ostream();
	Set_Output_To_File(GetXmlConfigAttribute(xmlAceDebug)->TrunOn,
		pLogoStream,
        GetXmlConfigAttribute(xmlAceDebug)->DebugPath.c_str(),
		GetXmlConfigAttribute(xmlAceDebug)->DebugName.c_str(),
		GetXmlConfigAttribute(xmlAceDebug)->LogFileMaxSize);

    return 0;
}

bool CMessageServiceGroup::Init(uint32 u4ThreadCount, uint32 u4MaxQueue, uint32 u4LowMask, uint32 u4HighMask)
{
    //ɾ����ǰ������CMessageService����

    //��¼��ǰ����
    m_u4MaxQueue     = u4MaxQueue;
    m_u4HighMask     = u4HighMask;
    m_u4LowMask      = u4LowMask;
    m_u2CurrThreadID = 0;

    m_objAllThreadInfo.Init((int)u4ThreadCount);

    //ʱ��ģʽ����
    OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::Init]Timing sequence Start.\n"));

    //��õ�ǰCPU������
#if PSS_PLATFORM == PLATFORM_WIN
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    m_u2CpuNumber = (uint16)si.dwNumberOfProcessors;
#else
    m_u2CpuNumber = (uint16)sysconf(_SC_NPROCESSORS_CONF);
#endif

    OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::Init]CPU NUmber is %d.\n", m_u2CpuNumber));

    //��ʼ�����е�Message����
    for (uint32 i = 0; i < u4ThreadCount; i++)
    {
        CMessageService* pMessageService = new CMessageService();

        if (NULL == pMessageService)
        {
            OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::Init](%d)pMessageService is NULL.\n", i));
            return false;
        }

        //���CPU���ڹ����߳����������Զ����̵߳�CPU
        if (m_u2CpuNumber >= u4ThreadCount)
        {
            pMessageService->Init(i, u4MaxQueue, u4LowMask, u4HighMask, true);
        }
        else
        {
            pMessageService->Init(i, u4MaxQueue, u4LowMask, u4HighMask, false);
        }

        //���߳���Ϣ�����߳���
        _ThreadInfo* pThreadInfo = pMessageService->GetThreadInfo();

        m_objAllThreadInfo.AddThreadInfo(i, pThreadInfo);

        m_vecMessageService.push_back(pMessageService);
    }

    return true;
}

bool CMessageServiceGroup::PutMessage(CMessage* pMessage)
{
    //�ж��Ƿ���Ҫ����Ⱦɫ
    string strTraceID = m_objMessageDyeingManager.GetTraceID(pMessage->GetMessageBase()->m_szIP,
                        (short)pMessage->GetMessageBase()->m_u4Port,
                        pMessage->GetMessageBase()->m_u2Cmd);

    if (strTraceID.length() > 0)
    {
        //��ҪȾɫ������TraceID
        sprintf_safe(pMessage->GetMessageBase()->m_szTraceID, MAX_BUFF_50, "%s", strTraceID.c_str());
    }

    //�ж��Ƿ�ΪTCP�������������ConnectID���֡�UDP���������һ��
    int32 n4ThreadID = 0;

    //�õ������߳�ID
    n4ThreadID = pMessage->GetMessageBase()->m_u4WorkThreadID;

    if (-1 == n4ThreadID)
    {
        pMessage->Clear();
        SAFE_DELETE(pMessage);
        return false;
    }

    CMessageService* pMessageService = m_vecMessageService[(uint32)n4ThreadID];

    if (NULL != pMessageService && false == pMessageService->PutMessage(pMessage))
    {
        OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::PutMessage](%d)pMessageService fail.\n", pMessageService->GetThreadID()));
    }

    return true;
}

bool CMessageServiceGroup::PutUpdateCommandMessage(uint32 u4UpdateIndex)
{
    //�����й����߳�Ⱥ������������Ϣ
    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService && false == pMessageService->PutUpdateCommandMessage(u4UpdateIndex))
        {
            OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::PutMessage](%d)pMessageService fail.\n", pMessageService->GetThreadID()));
            return false;
        }
    }

    return true;
}

void CMessageServiceGroup::Close()
{
    if (false == KillTimer())
    {
        OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::Close]KillTimer error.\n"));
    }

    ACE_Time_Value tvSleep(0, 1000);

    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService && 0 != pMessageService->Close())
        {
            OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::Close](%d)pMessageService fail.\n", pMessageService->GetThreadID()));
        }

        ACE_OS::sleep(tvSleep);
        SAFE_DELETE(pMessageService);
    }

    m_vecMessageService.clear();

    m_objAllThreadInfo.Close();

}

bool CMessageServiceGroup::Start()
{
    if(StartTimer() == false)
    {
        return false;
    }

    OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::Start]Work thread count=%d.\n", m_vecMessageService.size()));

    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService && false == pMessageService->Start())
        {
            OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::Start](%d)WorkThread is fail.\n", i));
            return false;
        }

        OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::Start](%d)WorkThread is OK.\n", i));
    }

    return true;
}

bool CMessageServiceGroup::StartTimer()
{
    OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::StartTimer] begin....\n"));

    m_u4TimerID = (uint32)App_TimerManager::instance()->schedule(this, NULL, ACE_OS::gettimeofday() + ACE_Time_Value(MAX_MSG_STARTTIME), ACE_Time_Value(m_u2ThreadTimeCheck));

    if(0 == m_u4TimerID)
    {
        OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::StartTimer] Start thread time error.\n"));
        return false;
    }

    return true;
}

bool CMessageServiceGroup::KillTimer()
{
    OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::KillTimer] begin....\n"));

    if(m_u4TimerID > 0)
    {
        App_TimerManager::instance()->cancel(m_u4TimerID);
        m_u4TimerID = 0;
    }

    OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::KillTimer] end....\n"));
    return true;
}

bool CMessageServiceGroup::CheckWorkThread(const ACE_Time_Value& tvNow)
{
    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService && false == pMessageService->SaveThreadInfoData(tvNow))
        {
            OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::CheckWorkThread]SaveThreadInfo error.\n"));
        }

        //Ŀǰ�ڹ����̷߳���������ʱ����ɱ�������̣߳�ɱ�˹����̻߳ᵼ��һЩ�ڴ����⡣
    }

    return true;
}

bool CMessageServiceGroup::CheckPacketParsePool() const
{
    AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_PACKETTHREAD, "[CMessageService::handle_timeout] UsedCount = %d, FreeCount= %d.", App_PacketParsePool::instance()->GetUsedCount(), App_PacketParsePool::instance()->GetFreeCount());
    return true;
}

bool CMessageServiceGroup::CheckCPUAndMemory(bool blTest)
{
    if (GetXmlConfigAttribute(xmlMonitor)->CpuAndMemory == 1 || true == blTest)
    {
#if PSS_PLATFORM == PLATFORM_WIN
        uint32 u4CurrCpu = (uint32)GetProcessCPU_Idel();
#else
        uint32 u4CurrCpu = (uint32)GetProcessCPU_Idel_Linux();
#endif

        //������Messageblock,BuffPacket,MessageCount,�ڴ��С
        uint32 u4MessageBlockUsedSize = App_MessageBlockManager::instance()->GetUsedSize();
        uint32 u4BuffPacketCount = App_BuffPacketManager::instance()->GetBuffPacketUsedCount();
        uint32 u4MessageCount = GetUsedMessageCount();

        if (u4CurrCpu > GetXmlConfigAttribute(xmlMonitor)->CpuMax || u4MessageBlockUsedSize > GetXmlConfigAttribute(xmlMonitor)->MemoryMax)
        {
            OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::handle_timeout]CPU Rote=%d,MessageBlock=%d,u4BuffPacketCount=%d,u4MessageCount=%d ALERT.\n", u4CurrCpu, u4MessageBlockUsedSize, u4BuffPacketCount, u4MessageCount));
            AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_MONITOR, "[Monitor] CPU Rote=%d,MessageBlock=%d,u4BuffPacketCount=%d,u4MessageCount=%d.", u4CurrCpu, u4MessageBlockUsedSize, u4BuffPacketCount, u4MessageCount);
        }
    }

    return true;
}

bool CMessageServiceGroup::CheckPlugInState() const
{
    vector<_ModuleInfo*> vecModeInfo;
    App_ModuleLoader::instance()->GetAllModuleInfo(vecModeInfo);

    for (const _ModuleInfo* pModuleInfo : vecModeInfo)
    {
        if (NULL != pModuleInfo)
        {
            uint32 u4ErrorID = 0;
            bool blModuleState = pModuleInfo->GetModuleState(u4ErrorID);

            if (false == blModuleState)
            {
                char szTitle[MAX_BUFF_50] = { '\0' };
                sprintf_safe(szTitle, MAX_BUFF_50, "ModuleStateError");

                //�����ʼ�
                AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_MONITOR, 1,
                        szTitle,
                        "Module ErrorID=%d.\n",
                        u4ErrorID);
            }
        }
    }

    return true;
}

void CMessageServiceGroup::AddDyringIP(const char* pClientIP, uint16 u2MaxCount)
{
    return m_objMessageDyeingManager.AddDyringIP(pClientIP, u2MaxCount);
}

bool CMessageServiceGroup::AddDyeingCommand(uint16 u2CommandID, uint16 u2MaxCount)
{
    return m_objMessageDyeingManager.AddDyeingCommand(u2CommandID, u2MaxCount);
}

void CMessageServiceGroup::GetDyeingCommand(vec_Dyeing_Command_list& objList) const
{
    m_objMessageDyeingManager.GetDyeingCommand(objList);
}

void CMessageServiceGroup::GetFlowPortList(vector<_Port_Data_Account>& vec_Port_Data_Account)
{
    vec_Port_Data_Account.clear();
    vector<_Port_Data_Account> vec_Service_Port_Data_Account;

    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService)
        {
            pMessageService->GetFlowPortList(vec_Service_Port_Data_Account);

            Combo_Port_List(vec_Service_Port_Data_Account, vec_Port_Data_Account);
        }
    }
}

CThreadInfoList* CMessageServiceGroup::GetThreadInfo()
{
    return &m_objAllThreadInfo;
}

uint32 CMessageServiceGroup::GetUsedMessageCount()
{
    uint32 u4Count = 0;

    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        u4Count += m_vecMessageService[i]->GetUsedMessageCount();
    }

    return u4Count;
}

uint32 CMessageServiceGroup::GetWorkThreadCount() const
{
    return (uint32)m_vecMessageService.size();
}

uint32 CMessageServiceGroup::GetWorkThreadIDByIndex(uint32 u4Index)
{
    if(u4Index >= m_vecMessageService.size())
    {
        return (uint32)0;
    }
    else
    {
        return (uint32)m_vecMessageService[u4Index]->GetThreadInfo()->m_u4ThreadID;
    }
}

void CMessageServiceGroup::GetWorkThreadAIInfo(vecWorkThreadAIInfo& objvecWorkThreadAIInfo)
{
    objvecWorkThreadAIInfo.clear();

    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        _WorkThreadAIInfo objWorkThreadAIInfo;
        const CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService)
        {
            pMessageService->GetAIInfo(objWorkThreadAIInfo);
            objWorkThreadAIInfo.m_u4ThreadID = pMessageService->GetThreadID();
            objvecWorkThreadAIInfo.push_back(objWorkThreadAIInfo);
        }
    }
}

void CMessageServiceGroup::GetAITO(vecCommandTimeout& objTimeout)
{
    objTimeout.clear();

    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        const CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService)
        {
            pMessageService->GetAITO(objTimeout);
        }
    }
}

void CMessageServiceGroup::GetAITF(vecCommandTimeout& objTimeout)
{
    objTimeout.clear();

    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        const CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService)
        {
            pMessageService->GetAITF(objTimeout);
        }
    }
}

void CMessageServiceGroup::SetAI(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime)
{
    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService)
        {
            pMessageService->SetAI(u1AI, u4DisposeTime, u4WTCheckTime, u4WTStopTime);
        }
    }
}

void CMessageServiceGroup::GetCommandData(uint16 u2CommandID, _CommandData& objCommandData) const
{
    for (CMessageService* pMessageService : m_vecMessageService)
    {
        if (NULL != pMessageService)
        {
            const _CommandData* pCommandData = pMessageService->GetCommandData(u2CommandID);

            if (NULL != pCommandData)
            {
                objCommandData += (*pCommandData);
            }
        }
    }
}

void CMessageServiceGroup::GetFlowInfo(uint32& u4FlowIn, uint32& u4FlowOut)
{
    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        uint32 u4CurrFlowIn  = 0;
        uint32 u4CurrFlowOut = 0;
        CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService)
        {
            pMessageService->GetFlowInfo(u4CurrFlowIn, u4CurrFlowOut);
            u4FlowIn  += u4CurrFlowIn;
            u4FlowOut += u4CurrFlowOut;
        }
    }
}

void CMessageServiceGroup::GetCommandAlertData(vecCommandAlertData& CommandAlertDataList)
{
    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService)
        {
            pMessageService->GetCommandAlertData(CommandAlertDataList);
        }
    }
}

void CMessageServiceGroup::SaveCommandDataLog()
{
    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService)
        {
            pMessageService->SaveCommandDataLog();
        }
    }
}

CMessage* CMessageServiceGroup::CreateMessage(uint32 u4ConnectID, EM_CONNECT_IO_TYPE u1PacketType)
{
    int32 n4ThreadID = 0;
    n4ThreadID = GetWorkThreadID(u4ConnectID, u1PacketType);

    if (-1 == n4ThreadID)
    {
        return NULL;
    }

    CMessageService* pMessageService = m_vecMessageService[(uint32)n4ThreadID];

    if (NULL != pMessageService)
    {
        return pMessageService->CreateMessage();
    }
    else
    {
        return NULL;
    }
}

void CMessageServiceGroup::DeleteMessage(uint32 u4ConnectID, CMessage* pMessage)
{
    ACE_UNUSED_ARG(u4ConnectID);

    int32 n4ThreadID = 0;
    n4ThreadID = pMessage->GetMessageBase()->m_u4WorkThreadID;

    if (-1 == n4ThreadID)
    {
        pMessage->Clear();
        SAFE_DELETE(pMessage);
        return;
    }

    CMessageService* pMessageService = m_vecMessageService[(uint32)n4ThreadID];

    if (NULL != pMessageService)
    {
        pMessageService->DeleteMessage(pMessage);
    }
}

void CMessageServiceGroup::CopyMessageManagerList()
{
    //��ʼ�����е�Message����
    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL == pMessageService)
        {
            OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::CopyMessageManagerList](%d)pMessageService is NULL.\n", i));
        }
        else
        {
            pMessageService->CopyMessageManagerList();
        }
    }
}

int32 CMessageServiceGroup::GetWorkThreadID(uint32 u4ConnectID, EM_CONNECT_IO_TYPE u1PackeType)
{
    int32 n4ThreadID = -1;

    if(m_vecMessageService.size() == 0)
    {
        return n4ThreadID;
    }

    if (EM_CONNECT_IO_TYPE::CONNECT_IO_TCP == u1PackeType
        || EM_CONNECT_IO_TYPE::CONNECT_IO_TTY == u1PackeType
        || EM_CONNECT_IO_TYPE::CONNECT_IO_SERVER_TCP == u1PackeType
        || EM_CONNECT_IO_TYPE::CONNECT_IO_SERVER_UDP == u1PackeType)
    {
        n4ThreadID = u4ConnectID % (uint32)m_vecMessageService.size();
    }
    else if(u1PackeType == EM_CONNECT_IO_TYPE::CONNECT_IO_UDP)
    {
        //�����UDPЭ�飬���¼��ǰ�̵߳�λ�ã�ֱ��+1������������ٶȱȽ�������ΪҪ���ļ���
        m_ThreadLock.acquire();
        n4ThreadID = m_u2CurrThreadID;

        //��ǰm_u2CurrThreadIDָ����һ���߳�ID
        if (m_u2CurrThreadID >= m_objAllThreadInfo.GetThreadCount() - 1)
        {
            m_u2CurrThreadID = 0;
        }
        else
        {
            m_u2CurrThreadID++;
        }

        m_ThreadLock.release();
    }

    return n4ThreadID;
}

