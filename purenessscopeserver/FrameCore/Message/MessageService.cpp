// MessageService.h
// 处理消息的线程类，Connect会将要处理的CMessage对象放入这些线程中处理。
// 以为有些故事，以为过久了就会忘掉。却发现，沉淀的却是自我，慢慢的，故事变成了信仰。
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

    //添加线程信息
    m_u4ThreadID = u4ThreadID;
    m_ThreadInfo.m_u4ThreadID   = u4ThreadID;

    //设置当前线程是否CPU绑定
    m_blIsCpuAffinity = blIsCpuAffinity;

    m_u4WorkQueuePutTime = GetXmlConfigAttribute(xmlThreadInfo)->PutQueueTimeout * 1000;

    //初始化线程AI
    m_WorkThreadAI.Init(GetXmlConfigAttribute(xmlThreadInfoAI)->AI,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->TimeoutCount,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->CheckTime,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->TimeoutCount,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->StopTime,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->ReturnDataType,
                        GetXmlConfigAttribute(xmlThreadInfoAI)->ReturnData.c_str());

    //按照线程初始化统计模块的名字
    char szName[MAX_BUFF_50] = {'\0'};
    sprintf_safe(szName, MAX_BUFF_50, "工作线程(%d)", u4ThreadID);
    m_CommandAccount.InitName(szName, GetXmlConfigAttribute(xmlCommandAccount)->MaxCommandCount);

    //初始化统计模块功能
    m_CommandAccount.Init(GetXmlConfigAttribute(xmlCommandAccount)->Account,
                          GetXmlConfigAttribute(xmlCommandAccount)->FlowAccount,
                          GetXmlConfigAttribute(xmlThreadInfo)->ThreadTimeout);

    //初始化本地信令列表副本
    m_objClientCommandList.Init(App_MessageManager::instance()->GetMaxCommandCount());

    //初始化当前线程Handler列表
    uint16 u2PoolSize = GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount;
    m_objHandlerList.Init(u2PoolSize);

    //初始化CommandID告警阀值相关
    for(const auto& objCommandInfo : GetXmlConfigAttribute(xmlCommandInfos)->vec)
    {
        m_CommandAccount.AddCommandAlert(objCommandInfo.CommandID,
                                         objCommandInfo.CommandCount,
                                         objCommandInfo.MailID);
    }

    //设置消息池
    m_DeviceHandlerPool.Init(u2PoolSize, CDeviceHandlerPool::Init_Callback);

    //初始化发送缓冲
    m_objBuffSendPacket.Init(DEFINE_PACKET_SIZE, GetXmlConfigAttribute(xmlSendInfo)->MaxBlockSize);
    m_objBuffSendPacket.SetNetSort(GetXmlConfigAttribute(xmlNetWorkMode)->NetByteOrder);

    //设置最大的等待时间(单位是毫秒)
    m_u4MaxRecvWait = GetXmlConfigAttribute(xmlClientInfo)->CheckAliveTime * 1000;

    m_PerformanceCounter.init("WorkThread", 10000);
}

bool CMessageService::Start()
{
    if(0 != open())
    {
        m_emThreadState = MESSAGE_SERVICE_THREAD_STATE::THREAD_STOP;
        return false;
    }
    else
    {
        m_emThreadState = MESSAGE_SERVICE_THREAD_STATE::THREAD_RUN;
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
	//判断是否要绑定CPU
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

        //使用内存池，这块内存不必再释放
    }

	//关闭所有的链接
	vector<CWorkThread_Handler_info*> handlerList;
	m_objHandlerList.Get_All_Used(handlerList);

    for (CWorkThread_Handler_info* handler_info : handlerList)
    {
        handler_info->m_pHandler->Close(handler_info->m_u4ConnectID);
    }
    m_objHandlerList.Clear();

    OUR_DEBUG((LM_INFO,"[CMessageService::svc] svc finish!\n"));
    return 0;
}

bool CMessageService::PutMessage(CWorkThreadMessage* pMessage)
{
    ACE_Message_Block* mb = pMessage->GetQueueMessage();

    if (true == m_blOverload)
    {
        size_t nQueueCount = msg_queue()->message_count();
        if (nQueueCount < m_u4MaxQueue)
        {
            //超载已经结束了，记录恢复时间
            OUR_DEBUG((LM_ERROR, "[CMessageService::PutMessage] Queue is Full end nQueueCount = [%d].\n", nQueueCount));
        }
        else
        {
            //超载尚未结束，返回错误
            return false;
        }

		m_blOverload = false;
    }

    if(this->putq(mb, nullptr) == -1)
    {
        int nError = errno;
        if (false == m_blOverload)
        {
            OUR_DEBUG((LM_ERROR, "[CMessageService::PutMessage] Queue is Full errno = [%d].\n", nError));
            //线程处理过载，写入日志
            AppLogManager::instance()->WriteLog(LOG_SYSTEM_ERROR,
                "[CMessageService::PutMessage](%d)Queue is Full begin errno = [%d]",
                m_u4ThreadID,
                nError);

            m_blOverload = true;
        }
           
        return false;
    }

    return true;
}

bool CMessageService::PutUpdateCommandMessage(uint32 u4UpdateIndex)
{
    ACE_Message_Block* mblk = App_MessageBlockManager::instance()->Create(sizeof(int));

    if (nullptr == mblk)
    {
        return false;
    }

    memcpy_safe((char* )&u4UpdateIndex, sizeof(int), mblk->wr_ptr(), sizeof(int));
    mblk->wr_ptr(sizeof(int));

    mblk->msg_type(ACE_Message_Block::MB_USER);

    //判断队列是否是已经最大
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

bool CMessageService::ProcessRecvMessage(CWorkThreadMessage* pMessage, uint32 u4ThreadID)
{
    //将数据转换成Message对象
    CMessage objRecvMessage;
    CWorkThread_Handler_info* pWorkThread_Handler_info = m_objHandlerList.Get_Hash_Box_Data_By_Uint32(pMessage->m_u4ConnectID);

    //判断消息类型
    if (CLIENT_LINK_CONNECT == pMessage->m_u2Cmd)
    {
        //如果是链接建立消息
        if (nullptr == pWorkThread_Handler_info)
        {
            //添加链接对象
            pWorkThread_Handler_info = m_DeviceHandlerPool.Create();

            pWorkThread_Handler_info->m_strLocalIP   = pMessage->m_AddrListen.get_host_addr();
            pWorkThread_Handler_info->m_u2LocalPort  = pMessage->m_AddrListen.get_port_number();
			pWorkThread_Handler_info->m_strRemoteIP  = pMessage->m_AddrRemote.get_host_addr();
			pWorkThread_Handler_info->m_u2RemotePort = pMessage->m_AddrRemote.get_port_number();

            pWorkThread_Handler_info->m_u4ConnectID  = pMessage->m_u4ConnectID;
            pWorkThread_Handler_info->m_pHandler     = pMessage->m_pHandler;
            pWorkThread_Handler_info->m_emPacketType = pMessage->m_emPacketType;

            pWorkThread_Handler_info->m_tvInput      = pMessage->m_tvMessage;

            m_objHandlerList.Add_Hash_Data_By_Key_Unit32(pMessage->m_u4ConnectID, pWorkThread_Handler_info);

			//写入连接日志
			AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Connection from [%s:%d] ConnectID=%d.",
                pWorkThread_Handler_info->m_strRemoteIP.c_str(),
                pWorkThread_Handler_info->m_u2RemotePort,
                pMessage->m_u4ConnectID);
        }
    }

	if (nullptr == pWorkThread_Handler_info)
	{
		OUR_DEBUG((LM_ERROR, "[CMessageService::ProcessMessage] [%d](%d)pWorkThread_Handler_info is nullptr.\n", 
            u4ThreadID,
            pMessage->m_u4ConnectID));
		DeleteMessage(pMessage);
		return false;
	}

    uint32 u4PacletHeadLength = 0;
    uint32 u4PacletBodyLength = 0;
    if (nullptr != pMessage->m_pmbRecvHead)
    {
        u4PacletHeadLength = (uint32)pMessage->m_pmbRecvHead->length();
    }

    if (nullptr != pMessage->m_pmbRecvBody)
    {
        u4PacletBodyLength = (uint32)pMessage->m_pmbRecvBody->length();
    }

	if (pMessage->m_u2Cmd >= CLIENT_LINK_USER)
	{
		//添加Handler统计消息
		pWorkThread_Handler_info->m_InPacketCount++;
		pWorkThread_Handler_info->m_RecvSize += u4PacletHeadLength + u4PacletBodyLength;
		pWorkThread_Handler_info->m_tvInput = pMessage->m_tvMessage;
	}

    //拼接消息
    objRecvMessage.GetMessageBase()->m_u2Cmd         = pMessage->m_u2Cmd;
    objRecvMessage.GetMessageBase()->m_strClientIP   = pWorkThread_Handler_info->m_strRemoteIP;
    objRecvMessage.GetMessageBase()->m_u2ClientPort  = pWorkThread_Handler_info->m_u2RemotePort;
    objRecvMessage.GetMessageBase()->m_strListenIP   = pWorkThread_Handler_info->m_strLocalIP;
    objRecvMessage.GetMessageBase()->m_u2ListenPort  = pWorkThread_Handler_info->m_u2LocalPort;
    objRecvMessage.GetMessageBase()->m_tvRecvTime    = pMessage->m_tvMessage;
    objRecvMessage.GetMessageBase()->m_u4ConnectID   = pMessage->m_u4ConnectID;
    objRecvMessage.GetMessageBase()->m_u4HeadSrcSize = u4PacletHeadLength;
    objRecvMessage.GetMessageBase()->m_u4BodySrcSize = u4PacletBodyLength;
    objRecvMessage.GetMessageBase()->m_emPacketType  = pMessage->m_emPacketType;
    objRecvMessage.GetMessageBase()->m_emResouceType = pMessage->m_emResouceType;

    objRecvMessage.SetPacketHead(pMessage->m_pmbRecvHead);
    objRecvMessage.SetPacketBody(pMessage->m_pmbRecvBody);

    //在这里进行线程自检代码
    m_ThreadInfo.m_tvUpdateTime = pMessage->m_tvMessage;
    m_ThreadInfo.m_u4State = THREADSTATE::THREAD_RUNBEGIN;

    //将要处理的数据放到逻辑处理的地方去
    uint16 u2CommandID = objRecvMessage.GetMessageBase()->m_u2Cmd;          //数据包的CommandID

    uint16 u2CommandCount = 0;      //命令被调用次数
    bool   blDeleteFlag   = true;   //用完是否删除，默认是删除

    DoMessage(&objRecvMessage, u2CommandID, u2CommandCount, blDeleteFlag);

    if(objRecvMessage.GetMessageBase()->m_u2Cmd >= CLIENT_LINK_USER)
    {
        //同步发送数据逻辑
        Synchronize_SendPostMessage(pWorkThread_Handler_info, pMessage->m_tvMessage);
    }

    //如果是断开消息，在这里发还给逻辑线程去处理
    if (CLIENT_LINK_CDISCONNET == pMessage->m_u2Cmd ||
        CLIENT_LINK_SDISCONNET == pMessage->m_u2Cmd)
    {
		//写日志
		AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendCount = %d, SendSize = %d.",
            pWorkThread_Handler_info->m_strRemoteIP.c_str(),
            pWorkThread_Handler_info->m_u2RemotePort,
            pWorkThread_Handler_info->m_InPacketCount,
            pWorkThread_Handler_info->m_RecvSize,
            pWorkThread_Handler_info->m_OutPacketCount,
            pWorkThread_Handler_info->m_SendSize);

        pWorkThread_Handler_info->m_pHandler->Close(pMessage->m_u4ConnectID);
        m_objHandlerList.Del_Hash_Data_By_Unit32(pMessage->m_u4ConnectID);
        m_DeviceHandlerPool.Delete(pWorkThread_Handler_info);
    }

	if (true == blDeleteFlag)
	{
		DeleteMessage(pMessage);
	}

    m_ThreadInfo.m_u4State = THREADSTATE::THREAD_RUNEND;

    return true;
}

bool CMessageService::ProcessSendMessage(CWorkThreadMessage* pMessage, uint32 u4ThreadID)
{
    int nRet = true;
	CWorkThread_Handler_info* pWorkThread_Handler_info = m_objHandlerList.Get_Hash_Box_Data_By_Uint32(pMessage->m_SendMessageInfo.u4ConnectID);

    if (nullptr != pWorkThread_Handler_info)
    {
        uint32 u4PacketSize = 0;
        nRet = pWorkThread_Handler_info->m_pHandler->SendMessage(pMessage->m_SendMessageInfo, u4PacketSize);
    }
    else
    {
        nRet = false;
        OUR_DEBUG((LM_INFO, "[CMessageService::ProcessSendMessage](ThreadID=%d)(CommandID=%d)Handler is nullptr.\n", 
            u4ThreadID,
            pMessage->m_u4ConnectID));
    }

    DeleteMessage(pMessage);
    return nRet;
}

bool CMessageService::ProcessSendClose(CWorkThreadMessage* pMessage, uint32 u4ThreadID)
{
	int nRet = true;
	CWorkThread_Handler_info* pWorkThread_Handler_info = m_objHandlerList.Get_Hash_Box_Data_By_Uint32(pMessage->m_u4ConnectID);

	if (nullptr != pWorkThread_Handler_info)
	{
        pWorkThread_Handler_info->m_pHandler->Close(pWorkThread_Handler_info->m_u4ConnectID);
        m_objClientCommandList.Del_Hash_Data_By_Unit32(pMessage->m_u4ConnectID);
	}
	else
	{
		nRet = false;
		OUR_DEBUG((LM_INFO, "[CMessageService::ProcessSendClose](ThreadID=%d)(CommandID=%d)Handler is nullptr.\n",
			u4ThreadID,
			pMessage->m_u4ConnectID));
	}

    DeleteMessage(pMessage);
    return nRet;
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

    m_WorkThreadAI.Close();

    OUR_DEBUG((LM_INFO, "[CMessageService::close] Close().\n"));
    return 0;
}

bool CMessageService::SaveThreadInfoData(const ACE_Time_Value& tvNow)
{
    //这里进行线程自检
    ACE_Date_Time dt(m_ThreadInfo.m_tvUpdateTime);

    //添加到线程信息历史数据表
    _ThreadInfo objCurrThreadInfo    = m_ThreadInfo;
    objCurrThreadInfo.m_tvUpdateTime = ACE_OS::gettimeofday();

    //开始查看线程是否超时
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

        //发现阻塞线程，需要重启相应的线程
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
    //清理当前信令列表
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

bool CMessageService::DoMessage(IMessage* pMessage, uint16& u2CommandID, uint16& u2Count, bool& bDeleteFlag)
{
    if (nullptr == pMessage)
    {
        OUR_DEBUG((LM_ERROR, "[CMessageService::DoMessage] pMessage is nullptr.\n"));
        return false;
    }

    //放给需要继承的ClientCommand类去处理
    CClientCommandList* pClientCommandList = GetClientCommandList(u2CommandID);

    if (nullptr == pClientCommandList)
    {
		//没有找到对应的注册指令，如果不是define.h定义的异常，则记录异常命令日志
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
        if (nullptr != pClientCommandInfo)
        {
            //判断当前消息是否有指定的监听端口
            if (pClientCommandInfo->m_objListenIPInfo.m_u2Port > 0 &&
                (pClientCommandInfo->m_objListenIPInfo.m_strClientIP != pMessage->GetMessageBase()->m_strListenIP ||
                    pClientCommandInfo->m_objListenIPInfo.m_u2Port != pMessage->GetMessageBase()->m_u2ListenPort))
            {
                continue;
            }

            //标记当前命令运行状态
            pClientCommandInfo->m_pClientCommand->DoMessage(pMessage, bDeleteFlag, &m_objBuffSendPacket);

        }

        //记录命令被调用次数
        u2Count = (uint16)nCount;
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

uint32 CMessageService::GetHandlerCount()
{
    return m_objHandlerList.Get_Used_Count();
}

void CMessageService::CopyMessageManagerList()
{
    CloseCommandList();

    CHashTable<CClientCommandList>* pHashtCommandList = App_MessageManager::instance()->GetHashCommandList();

    if (nullptr == pHashtCommandList)
    {
        return;
    }

	vector<CClientCommandList*> vecClientCommandList;
	pHashtCommandList->Get_All_Used(vecClientCommandList);

	for (CClientCommandList* pClientCommandList : vecClientCommandList)
	{
        if (nullptr == pClientCommandList)
        {
            continue;
        }

		CClientCommandList* pCurrClientCommandList = new CClientCommandList();

		if (nullptr == pCurrClientCommandList)
		{
			continue;
		}

		pCurrClientCommandList->SetCommandID(pClientCommandList->GetCommandID());
		pCurrClientCommandList->SetCommandTimeout(pClientCommandList->GetCommandTimeout());

		for (int j = 0; j < pClientCommandList->GetCount(); j++)
		{
			const _ClientCommandInfo* pClientCommandInfo = pClientCommandList->GetClientCommandIndex(j);

			if (nullptr != pClientCommandInfo)
			{
				pCurrClientCommandList->AddClientCommand(pClientCommandInfo->m_pClientCommand, pClientCommandInfo->m_szModuleName, nullptr);
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
    return (uint32)msg_queue()->message_count();
}

CWorkThreadMessage* CMessageService::CreateMessage()
{
    return new CWorkThreadMessage();
}

void CMessageService::DeleteMessage(CWorkThreadMessage* pMessage)
{
    //清理数据
    pMessage->Close();
    delete pMessage;
}

void CMessageService::GetFlowPortList(vector<_Port_Data_Account>& vec_Port_Data_Account)
{
    m_CommandAccount.GetFlowPortList(vec_Port_Data_Account);
}

bool CMessageService::Synchronize_SendPostMessage(CWorkThread_Handler_info* pHandlerInfo, const ACE_Time_Value tvMessage)
{
	//同步发送数据
    uint32 u4SendLength = m_objBuffSendPacket.GetPacketLen();
    if (u4SendLength > 0)
    {
        ACE_Message_Block* pBlockSend = new ACE_Message_Block(u4SendLength);

        memcpy_safe(m_objBuffSendPacket.GetData(),
            u4SendLength,
            pBlockSend->rd_ptr(),
            u4SendLength);

        pBlockSend->wr_ptr(u4SendLength);

        pHandlerInfo->m_pHandler->PutSendPacket(pHandlerInfo->m_u4ConnectID, pBlockSend, u4SendLength, tvMessage);
        pBlockSend->release();
        m_objBuffSendPacket.Clear();

        //添加Handler统计信息
        pHandlerInfo->m_OutPacketCount++;
        pHandlerInfo->m_SendSize += u4SendLength;
        pHandlerInfo->m_tvOutput = tvMessage;

        return true;
    }
    else
    {
        return false;
    }
}

bool CMessageService::SendPostMessage(CSendMessageInfo objSendMessageInfo)
{
    //将数据放入队列
    CWorkThreadMessage* pWorkThreadMessage = CreateMessage();

    pWorkThreadMessage->m_emDirect = EM_WORKTHREAD_DIRECT::EM_WORKTHREAD_DIRECT_OUTPUT;
    pWorkThreadMessage->m_SendMessageInfo = objSendMessageInfo;

    return PutMessage(pWorkThreadMessage);
}

bool CMessageService::SendCloseMessage(uint32 u4ConnectID)
{
	//将数据放入队列
	CWorkThreadMessage* pWorkThreadMessage = CreateMessage();

	pWorkThreadMessage->m_emDirect    = EM_WORKTHREAD_DIRECT::EM_WORKTHREAD_DIRECT_OUTPUT;
	pWorkThreadMessage->m_u2Cmd       = CLINET_LINK_HANDLER_CLOSE;
    pWorkThreadMessage->m_u4ConnectID = u4ConnectID;

	return PutMessage(pWorkThreadMessage);
}

_ClientIPInfo CMessageService::GetClientIPInfo(uint32 u4ConnectID)
{
    _ClientIPInfo objClientIPInfo;
	CWorkThread_Handler_info* pWorkThread_Handler_info = m_objHandlerList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if (nullptr != pWorkThread_Handler_info)
    {
        objClientIPInfo.m_strClientIP = pWorkThread_Handler_info->m_strRemoteIP;
        objClientIPInfo.m_u2Port      = pWorkThread_Handler_info->m_u2RemotePort;
    }

    return objClientIPInfo;
}

NAMESPACE::_ClientIPInfo CMessageService::GetLocalIPInfo(uint32 u4ConnectID)
{
	_ClientIPInfo objClientIPInfo;
	CWorkThread_Handler_info* pWorkThread_Handler_info = m_objHandlerList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

	if (nullptr != pWorkThread_Handler_info)
	{
		objClientIPInfo.m_strClientIP = pWorkThread_Handler_info->m_strLocalIP;
		objClientIPInfo.m_u2Port = pWorkThread_Handler_info->m_u2LocalPort;
	}

    return objClientIPInfo;
}

void CMessageService::Check_Handler_Recv_Timeout()
{
    if (0 == m_u4MaxRecvWait)
    {
        //不检查直接退出
        return;
    }

    vector<CWorkThread_Handler_info*> vecList;
    m_objHandlerList.Get_All_Used(vecList);

    ACE_Time_Value tvNow = ACE_OS::gettimeofday();

    for (CWorkThread_Handler_info* pHandlerInfo : vecList)
    {
        if (EM_CONNECT_IO_TYPE::CONNECT_IO_TCP != pHandlerInfo->m_emPacketType 
            && EM_CONNECT_IO_TYPE::CONNECT_IO_UDP != pHandlerInfo->m_emPacketType)
        {
            //如果不是TCP连接，则不做链接检查
            continue;
        }

        ACE_Time_Value tvTimeWait = tvNow - pHandlerInfo->m_tvInput;
        if (m_u4MaxRecvWait < tvTimeWait.msec())
        {
            OUR_DEBUG((LM_INFO, "[CMessageService::Check_Handler_Recv_Timeout]u2CommandID=%d is recv timeout.\n", pHandlerInfo->m_u4ConnectID));

            //超时了，发送链接断开消息
			CSendMessageInfo objSendMessageInfo;

			objSendMessageInfo.u2CommandID = CLIENT_LINK_SDISCONNET;
			objSendMessageInfo.u4ConnectID = pHandlerInfo->m_u4ConnectID;

			SendPostMessage(objSendMessageInfo);
        }
    }
}

int CMessageService::handle_signal(int signum, siginfo_t* siginfo, ucontext_t* ucontext)
{
    if (signum == SIGUSR1 + grp_id())
    {
        OUR_DEBUG((LM_INFO,"[CMessageService::handle_signal](%d) will be kill.\n", grp_id()));

        if(nullptr != siginfo && nullptr != ucontext)
        {
            OUR_DEBUG((LM_INFO,"[CMessageService::handle_signal]siginfo is not nullptr.\n"));
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
            //需要通知大家再更新一下副本(让新的加载生效)
            App_MessageServiceGroup::instance()->PutUpdateCommandMessage(App_MessageManager::instance()->GetUpdateIndex());
        }

        //同步信令列表
        CopyMessageManagerList();
    }
}

bool CMessageService::Dispose_Queue()
{
    ACE_Message_Block* mb = nullptr;

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
        CWorkThreadMessage* msg = *((CWorkThreadMessage**)mb->base());

        if (EM_WORKTHREAD_DIRECT::EM_WORKTHREAD_DIRECT_INPUT == msg->m_emDirect)
        {
            //是接收数据
            if (false == ProcessRecvMessage(msg, m_u4ThreadID))
            {
                OUR_DEBUG((LM_ERROR, "[CMessageService::svc](%d)ProcessMessage is false!\n", m_u4ThreadID));
            }
        }
        else
        {
            //是发送数据和助攻关闭链接
            if (CLINET_LINK_HANDLER_CLOSE == msg->m_u2Cmd)
            {
                //关闭链接
                ProcessSendClose(msg, m_u4ThreadID);
            }
            else
            {
                //是发送数据
                ProcessSendMessage(msg, m_u4ThreadID);
            }
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

    //检查超时的链接
    CheckRecvTimeout();

    //检查所有工作线程
    if (false == CheckWorkThread(tv))
    {
        OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::handle_timeout]CheckWorkThread is fail.\n"));
    }

    //记录PacketParse的统计过程
    if (false == CheckPacketParsePool())
    {
        OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::handle_timeout]CheckPacketParsePool is fail.\n"));
    }

    //记录统计CPU和内存的使用
    if (false == CheckCPUAndMemory())
    {
        OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::handle_timeout]CheckCPUAndMemory is fail.\n"));
    }

    //检查所有插件状态
    if (false == CheckPlugInState())
    {
        OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::handle_timeout]CheckPlugInState is fail.\n"));
    }

    //检查日志文件
    ofstream* pLogoStream = (ofstream*)ACE_LOG_MSG->msg_ostream();
	Set_Output_To_File(GetXmlConfigAttribute(xmlAceDebug)->TrunOn,
		pLogoStream,
        GetXmlConfigAttribute(xmlAceDebug)->DebugPath.c_str(),
		GetXmlConfigAttribute(xmlAceDebug)->DebugName.c_str(),
		GetXmlConfigAttribute(xmlAceDebug)->LogFileMaxSize);

    return 0;
}

bool CMessageServiceGroup::Init(uint32 u4ThreadCount, uint32 u4MaxQueue, uint32 u4LowMask)
{
    //删除以前的所有CMessageService对象

    //记录当前设置
    m_u4MaxQueue     = u4MaxQueue;
    m_u4HighMask     = u4MaxQueue * 8; //这里的高水位标不在以设置为准，而是以最大队列数*指针大小计算
    m_u4LowMask      = u4LowMask;
    m_u2CurrThreadID = 0;

    m_objAllThreadInfo.Init((int)u4ThreadCount);

    //时序模式开启
    OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::Init]Timing sequence Start.\n"));

    //获得当前CPU的数量
#if PSS_PLATFORM == PLATFORM_WIN
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    m_u2CpuNumber = (uint16)si.dwNumberOfProcessors;
#else
    m_u2CpuNumber = (uint16)sysconf(_SC_NPROCESSORS_CONF);
#endif

    OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::Init]CPU NUmber is %d.\n", m_u2CpuNumber));

    //初始化所有的Message对象
    for (uint32 i = 0; i < u4ThreadCount; i++)
    {
        CMessageService* pMessageService = new CMessageService();

        if (nullptr == pMessageService)
        {
            OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::Init](%d)pMessageService is nullptr.\n", i));
            return false;
        }

        //如果CPU多于工作线程数量，则自动绑定线程到CPU
        if (m_u2CpuNumber >= u4ThreadCount)
        {
            pMessageService->Init(i, u4MaxQueue, u4LowMask, m_u4HighMask, true);
        }
        else
        {
            pMessageService->Init(i, u4MaxQueue, u4LowMask, m_u4HighMask, false);
        }

        //将线程信息放入线程组
        _ThreadInfo* pThreadInfo = pMessageService->GetThreadInfo();

        m_objAllThreadInfo.AddThreadInfo(i, pThreadInfo);

        m_vecMessageService.push_back(pMessageService);
    }

    return true;
}

bool CMessageServiceGroup::PutMessage(CWorkThreadMessage* pMessage)
{
    //判断是否为TCP包，如果是则按照ConnectID区分。UDP则随机分配一个

    //得到工作线程ID
    CMessageService* pMessageService = m_vecMessageService[pMessage->m_u4WorkThreadID];

    if (false == pMessageService->PutMessage(pMessage))
    {
        OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::PutMessage](%d)pMessageService fail.\n", pMessageService->GetThreadID()));
    }

    return true;
}

bool CMessageServiceGroup::PutUpdateCommandMessage(uint32 u4UpdateIndex)
{
    //向所有工作线程群发副本更新消息
    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (nullptr != pMessageService && false == pMessageService->PutUpdateCommandMessage(u4UpdateIndex))
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

        if (nullptr != pMessageService && 0 != pMessageService->Close())
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

        if (nullptr != pMessageService && false == pMessageService->Start())
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

    m_u4TimerID = (uint32)App_TimerManager::instance()->schedule(this, nullptr, ACE_OS::gettimeofday() + ACE_Time_Value(MAX_MSG_STARTTIME), ACE_Time_Value(m_u2ThreadTimeCheck));

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

bool CMessageServiceGroup::CheckRecvTimeout()
{
    for (CMessageService* pWorkThread : m_vecMessageService)
    {
        pWorkThread->Check_Handler_Recv_Timeout();
    }

    return true;
}

bool CMessageServiceGroup::CheckWorkThread(const ACE_Time_Value& tvNow)
{
    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (nullptr != pMessageService && false == pMessageService->SaveThreadInfoData(tvNow))
        {
            OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::CheckWorkThread]SaveThreadInfo error.\n"));
        }

        //目前在工作线程发生阻塞的时候不在杀死工程线程，杀了工作线程会导致一些内存问题。
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

        //获得相关Messageblock,BuffPacket,MessageCount,内存大小
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

bool CMessageServiceGroup::Send_Post_Message(CSendMessageInfo objSendMessageInfo)
{
	//得到这个线程ID
	uint32 u4ThreadID = GetWorkThreadID(objSendMessageInfo.u4ConnectID, 
        EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

    CMessageService* pMessageService = m_vecMessageService[u4ThreadID];

    return pMessageService->SendPostMessage(objSendMessageInfo);
}

bool CMessageServiceGroup::Send_Close_Message(uint32 u4ConnectID)
{
	//得到这个线程ID
	uint32 u4ThreadID = GetWorkThreadID(u4ConnectID,
		EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

	CMessageService* pMessageService = m_vecMessageService[u4ThreadID];

    return pMessageService->SendCloseMessage(u4ConnectID);
}

NAMESPACE::_ClientIPInfo CMessageServiceGroup::GetClientIPInfo(uint32 u4ConnectID)
{
	//得到这个线程ID
	uint32 u4ThreadID = GetWorkThreadID(u4ConnectID,
		EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

	CMessageService* pMessageService = m_vecMessageService[u4ThreadID];

    return pMessageService->GetClientIPInfo(u4ConnectID);
}

_ClientIPInfo CMessageServiceGroup::GetLocalIPInfo(uint32 u4ConnectID)
{
	//得到这个线程ID
	uint32 u4ThreadID = GetWorkThreadID(u4ConnectID,
		EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);

	CMessageService* pMessageService = m_vecMessageService[u4ThreadID];

	return pMessageService->GetLocalIPInfo(u4ConnectID);
}

uint32 CMessageServiceGroup::GetHandlerCount()
{
    uint32 u4HandlerCount = 0;
	for (CMessageService* pMessageService : m_vecMessageService)
	{
		if (nullptr != pMessageService)
		{
            u4HandlerCount += pMessageService->GetHandlerCount();
		}
	}

    return u4HandlerCount;
}

bool CMessageServiceGroup::CheckPlugInState() const
{
    vector<_ModuleInfo*> vecModeInfo;
    App_ModuleLoader::instance()->GetAllModuleInfo(vecModeInfo);

    for (const _ModuleInfo* pModuleInfo : vecModeInfo)
    {
        if (nullptr != pModuleInfo)
        {
            uint32 u4ErrorID = 0;
            bool blModuleState = pModuleInfo->GetModuleState(u4ErrorID);

            if (false == blModuleState)
            {
                char szTitle[MAX_BUFF_50] = { '\0' };
                sprintf_safe(szTitle, MAX_BUFF_50, "ModuleStateError");

                //发送邮件
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

        if (nullptr != pMessageService)
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

        if (nullptr != pMessageService)
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

        if (nullptr != pMessageService)
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

        if (nullptr != pMessageService)
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

        if (nullptr != pMessageService)
        {
            pMessageService->SetAI(u1AI, u4DisposeTime, u4WTCheckTime, u4WTStopTime);
        }
    }
}

void CMessageServiceGroup::GetCommandData(uint16 u2CommandID, _CommandData& objCommandData) const
{
    for (CMessageService* pMessageService : m_vecMessageService)
    {
        if (nullptr != pMessageService)
        {
            const _CommandData* pCommandData = pMessageService->GetCommandData(u2CommandID);

            if (nullptr != pCommandData)
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

        if (nullptr != pMessageService)
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

        if (nullptr != pMessageService)
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

        if (nullptr != pMessageService)
        {
            pMessageService->SaveCommandDataLog();
        }
    }
}

CWorkThreadMessage* CMessageServiceGroup::CreateMessage(uint32 u4ConnectID, EM_CONNECT_IO_TYPE u1PacketType)
{
    uint32 u4ThreadID = 0;
    u4ThreadID = GetWorkThreadID(u4ConnectID, u1PacketType);

    CMessageService* pMessageService = m_vecMessageService[u4ThreadID];

    if (nullptr != pMessageService)
    {
        CWorkThreadMessage* pWorkThreadMessage = pMessageService->CreateMessage();
        //设置线程的ID
        pWorkThreadMessage->m_u4WorkThreadID = u4ThreadID;
        return pWorkThreadMessage;
    }
    else
    {
        return nullptr;
    }
}

void CMessageServiceGroup::DeleteMessage(CWorkThreadMessage* pMessage)
{
    CMessageService* pMessageService = m_vecMessageService[(uint32)pMessage->m_u4WorkThreadID];

    if (nullptr != pMessageService)
    {
        pMessageService->DeleteMessage(pMessage);
    }
}

void CMessageServiceGroup::CopyMessageManagerList()
{
    //初始化所有的Message对象
    uint32 u4Size = (uint32)m_vecMessageService.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (nullptr == pMessageService)
        {
            OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::CopyMessageManagerList](%d)pMessageService is nullptr.\n", i));
        }
        else
        {
            pMessageService->CopyMessageManagerList();
        }
    }
}

uint32 CMessageServiceGroup::GetWorkThreadID(uint32 u4ConnectID, EM_CONNECT_IO_TYPE emPackeType)
{
    ACE_UNUSED_ARG(emPackeType);
    uint32 u4ThreadID = 0;

    if (m_vecMessageService.size() == 0)
    {
        return u4ThreadID;
    }

    return u4ConnectID % (uint32)m_vecMessageService.size();
}

