// MessageService.h
// 处理消息的线程类，Connect会将要处理的CMessage对象放入这些线程中处理。
// 以为有些故事，以为过久了就会忘掉。却发现，沉淀的却是自我，慢慢的，故事变成了信仰。
// add by freeeyes
// 2009-01-26

#include "MessageService.h"

CMessageService::CMessageService():m_mutex(), m_cond(m_mutex)
{
    m_u4ThreadID      = 0;
    m_u4MaxQueue      = MAX_MSG_THREADQUEUE;
    m_blRun           = false;
    m_u4HighMask      = 0;
    m_u4LowMask       = 0;
    m_u8TimeCost      = 0;
    m_u4Count         = 0;
    m_emThreadState   = THREAD_STOP;

    uint16 u2ThreadTimeOut = App_MainConfig::instance()->GetThreadTimuOut();

    if(u2ThreadTimeOut <= 0)
    {
        m_u2ThreadTimeOut = MAX_MSG_THREADTIMEOUT;
    }
    else
    {
        m_u2ThreadTimeOut = u2ThreadTimeOut;
    }
}

CMessageService::~CMessageService()
{
    OUR_DEBUG((LM_INFO, "[CMessageService::~CMessageService].\n"));
}

void CMessageService::Init(uint32 u4ThreadID, uint32 u4MaxQueue, uint32 u4LowMask, uint32 u4HighMask)
{
    m_u4MaxQueue    = u4MaxQueue;
    m_u4HighMask    = u4HighMask;
    m_u4LowMask     = u4LowMask;

    //OUR_DEBUG((LM_INFO, "[CMessageService::Init]ID=%d,m_u4State=%d.\n", m_u4ThreadID = u4ThreadID, m_ThreadInfo.m_u4State));

    //添加线程信息
    m_u4ThreadID = u4ThreadID;
    m_ThreadInfo.m_u4ThreadID   = u4ThreadID;

    m_u4WorkQueuePutTime = App_MainConfig::instance()->GetWorkQueuePutTime() * 1000;

    //初始化线程AI
    m_WorkThreadAI.Init(App_MainConfig::instance()->GetWTAI(),
                        App_MainConfig::instance()->GetPacketTimeOut(),
                        App_MainConfig::instance()->GetWTCheckTime(),
                        App_MainConfig::instance()->GetWTTimeoutCount(),
                        App_MainConfig::instance()->GetWTStopTime(),
                        App_MainConfig::instance()->GetWTReturnDataType(),
                        App_MainConfig::instance()->GetWTReturnData());

    //按照线程初始化统计模块的名字
    char szName[MAX_BUFF_50] = {'\0'};
    sprintf_safe(szName, MAX_BUFF_50, "工作线程(%d)", u4ThreadID);
    m_CommandAccount.InitName(szName, App_MainConfig::instance()->GetMaxCommandCount());

    //初始化统计模块功能
    m_CommandAccount.Init(App_MainConfig::instance()->GetCommandAccount(),
                          App_MainConfig::instance()->GetCommandFlow(),
                          App_MainConfig::instance()->GetPacketTimeOut());

    //初始化CommandID告警阀值相关
    for(int i = 0; i < (int)App_MainConfig::instance()->GetCommandAlertCount(); i++)
    {
        _CommandAlert* pCommandAlert = App_MainConfig::instance()->GetCommandAlert(i);

        if(NULL != pCommandAlert)
        {
            m_CommandAccount.AddCommandAlert(pCommandAlert->m_u2CommandID,
                                             pCommandAlert->m_u4CommandCount,
                                             pCommandAlert->m_u4MailID);
        }
    }

    //设置消息池
    m_MessagePool.Init(MAX_MESSAGE_POOL);
}

bool CMessageService::Start()
{
    m_emThreadState = THREAD_RUN;

    if(0 != open())
    {
        m_emThreadState = THREAD_STOP;
        return false;
    }

    return true;
}

int CMessageService::open(void* args)
{
    if(args != NULL)
    {
        OUR_DEBUG((LM_INFO,"[CMessageService::open]args is not NULL.\n"));
    }

    m_blRun = true;
    msg_queue()->high_water_mark(m_u4HighMask);
    msg_queue()->low_water_mark(m_u4LowMask);

    OUR_DEBUG((LM_INFO,"[CMessageService::open] m_u4HighMask = [%d] m_u4LowMask = [%d]\n", m_u4HighMask, m_u4LowMask));

    if(activate(THREAD_PARAM, MAX_MSG_THREADCOUNT) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CMessageService::open] activate error ThreadCount = [%d].", MAX_MSG_THREADCOUNT));
        m_blRun = false;
        return -1;
    }

    resume();
    return 0;
}

int CMessageService::svc(void)
{
    // Cache our ACE_Thread_Manager pointer.
    ACE_Thread_Manager* mgr = this->thr_mgr ();

    while(true)
    {
        if (mgr->testcancel(mgr->thr_self ()))
        {
            return 0;
        }

        ACE_Message_Block* mb = NULL;
        ACE_OS::last_error(0);

        //xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);
        if(getq(mb, 0) == -1)
        {
            OUR_DEBUG((LM_ERROR,"[CMessageService::svc] PutMessage error errno = [%d].\n", ACE_OS::last_error()));
            m_blRun = false;
            break;
        }
        else
        {
            if(mb == NULL)
            {
                continue;
            }

            if ((0 == mb->size ()) && (mb->msg_type () == ACE_Message_Block::MB_STOP))
            {
                m_mutex.acquire();
                mb->release ();
                this->msg_queue ()->deactivate ();
                m_cond.signal();
                m_mutex.release();
                break;
            }

            while(m_emThreadState != THREAD_RUN)
            {
                //如果模块正在卸载或者重载，线程在这里等加载完毕（等1ms）。
                ACE_Time_Value tvsleep(0, 1000);
                ACE_OS::sleep(tvsleep);
            }

            CMessage* msg = *((CMessage**)mb->base());

            if(!msg)
            {
                OUR_DEBUG((LM_ERROR,"[CMessageService::svc] mb msg == NULL CurrthreadNo=[%d]!\n", m_u4ThreadID));
                continue;
            }

            this->ProcessMessage(msg, m_u4ThreadID);
        }

        //使用内存池，这块内存不必再释放
    }

    OUR_DEBUG((LM_INFO,"[CMessageService::svc] svc finish!\n"));
    return 0;
}

bool CMessageService::PutMessage(CMessage* pMessage)
{
    ACE_Message_Block* mb = pMessage->GetQueueMessage();

    if(NULL != mb)
    {
        //判断队列是否是已经最大
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

    return true;
}

bool CMessageService::ProcessMessage(CMessage* pMessage, uint32 u4ThreadID)
{
    //CProfileTime DisposeTime;
    //uint32 u4Cost = (uint32)(pMessage->GetMessageBase()->m_ProfileTime.Stop());

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

    //在这里进行线程自检代码
    m_ThreadInfo.m_tvUpdateTime = ACE_OS::gettimeofday();
    m_ThreadInfo.m_u4State = THREAD_RUNBEGIN;

    //OUR_DEBUG((LM_ERROR,"[CMessageService::ProcessMessage]1 [%d],m_u4State=%d, commandID=%d.\n", u4ThreadID, m_ThreadInfo.m_u4State,  pMessage->GetMessageBase()->m_u2Cmd));

    //将要处理的数据放到逻辑处理的地方去
    uint16 u2CommandID = 0;          //数据包的CommandID

    u2CommandID = pMessage->GetMessageBase()->m_u2Cmd;

    //抛出掉链接建立和断开，只计算逻辑数据包
    if(pMessage->GetMessageBase()->m_u2Cmd != CLIENT_LINK_CONNECT
       && pMessage->GetMessageBase()->m_u2Cmd != CLIENT_LINK_CDISCONNET
       && pMessage->GetMessageBase()->m_u2Cmd != CLIENT_LINK_SDISCONNET
       && pMessage->GetMessageBase()->m_u2Cmd != CLINET_LINK_SENDTIMEOUT
       && pMessage->GetMessageBase()->m_u2Cmd != CLINET_LINK_SENDERROR
       && pMessage->GetMessageBase()->m_u2Cmd != CLINET_LINK_CHECKTIMEOUT)
    {
        m_ThreadInfo.m_u4RecvPacketCount++;
        m_ThreadInfo.m_u4CurrPacketCount++;
        m_ThreadInfo.m_u2CommandID   = u2CommandID;

        bool blIsDead = m_WorkThreadAI.CheckCurrTimeout(pMessage->GetMessageBase()->m_u2Cmd, (uint32)m_ThreadInfo.m_tvUpdateTime.sec());

        if(blIsDead == true)
        {
            OUR_DEBUG((LM_ERROR,"[CMessageService::ProcessMessage]Command(%d) is Delele.\n", pMessage->GetMessageBase()->m_u2Cmd));
            //直接返回应急数据给客户端，不在到逻辑里去处理

            const char* ptrReturnData = reinterpret_cast<const char*>(m_WorkThreadAI.GetReturnData());
#ifdef WIN32
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
                    (uint16)COMMAND_RETURN_BUSY,
                    PACKET_SEND_IMMEDIATLY,
                    PACKET_IS_SELF_RECYC);
#endif
            DeleteMessage(pMessage);
            m_ThreadInfo.m_u4State = THREAD_RUNEND;

            return true;
        }
    }

    uint32 u4TimeCost     = 0;      //命令执行时间
    uint16 u2CommandCount = 0;      //命令被调用次数
    bool   blDeleteFlag   = true;   //用完是否删除，默认是删除

    App_MessageManager::instance()->DoMessage(m_ThreadInfo.m_tvUpdateTime, pMessage, u2CommandID, u4TimeCost, u2CommandCount, blDeleteFlag);

    if(true == blDeleteFlag)
    {
        DeleteMessage(pMessage);
    }

    if(pMessage->GetMessageBase()->m_u2Cmd != CLIENT_LINK_CONNECT
       && pMessage->GetMessageBase()->m_u2Cmd != CLIENT_LINK_CDISCONNET
       && pMessage->GetMessageBase()->m_u2Cmd != CLIENT_LINK_SDISCONNET
       && pMessage->GetMessageBase()->m_u2Cmd != CLINET_LINK_SENDTIMEOUT
       && pMessage->GetMessageBase()->m_u2Cmd != CLINET_LINK_SENDERROR
       && pMessage->GetMessageBase()->m_u2Cmd != CLINET_LINK_CHECKTIMEOUT)
    {
        //如果AI启动了，则在这里进行AI判定
        m_WorkThreadAI.SaveTimeout(pMessage->GetMessageBase()->m_u2Cmd, u4TimeCost);

        if(u2CommandCount > 0)
        {
            //获得单个命令的执行时间
            u4TimeCost = u4TimeCost/u2CommandCount;
        }

        //OUR_DEBUG((LM_ERROR,"[CMessageService::ProcessMessage]Command(%d)=[%d].\n", pMessage->GetMessageBase()->m_u2Cmd, u2CommandCount));

        //添加统计信息
        m_CommandAccount.SaveCommandData(u2CommandID,
                                         (uint64)u4TimeCost,
                                         pMessage->GetMessageBase()->m_u1PacketType,
                                         pMessage->GetMessageBase()->m_u4HeadSrcSize + pMessage->GetMessageBase()->m_u4BodySrcSize,
                                         (uint32)(pMessage->GetMessageBase()->m_u4HeadSrcSize + pMessage->GetMessageBase()->m_u4BodySrcSize),
                                         COMMAND_TYPE_IN);
    }

    m_ThreadInfo.m_u4State = THREAD_RUNEND;

    //开始测算数据包处理的时间
    if(m_ThreadInfo.m_u2PacketTime == 0)
    {
        m_ThreadInfo.m_u2PacketTime = (uint16)u4TimeCost;
    }
    else
    {
        //计算数据包的平均处理时间
        m_ThreadInfo.m_u2PacketTime = (uint16)((m_ThreadInfo.m_u2PacketTime + (uint16)u4TimeCost)/2);
    }

    return true;
}

int CMessageService::Close()
{
    if(m_blRun)
    {
        m_blRun = false;
        this->CloseMsgQueue();
    }
    else
    {
        m_blRun = false;
        msg_queue()->deactivate();
    }

    m_MessagePool.Close();
    OUR_DEBUG((LM_INFO, "[CMessageService::close] Close().\n"));
    return 0;
}

bool CMessageService::SaveThreadInfo()
{
    return SaveThreadInfoData();
}

bool CMessageService::SaveThreadInfoData()
{
    //这里进行线程自检
    ACE_Time_Value tvNow(ACE_OS::gettimeofday());
    ACE_Date_Time dt(m_ThreadInfo.m_tvUpdateTime);

    //开始查看线程是否超时
    //OUR_DEBUG((LM_INFO, "[CMessageService::SaveThreadInfoData]ID=%d,m_u4State=%d,m_u2ThreadTimeOut=%d,cost=%d.\n", m_ThreadInfo.m_u4ThreadID, m_ThreadInfo.m_u4State, m_u2ThreadTimeOut, tvNow.sec() - m_ThreadInfo.m_tvUpdateTime.sec()));
    if(m_ThreadInfo.m_u4State == THREAD_RUNBEGIN && tvNow.sec() - m_ThreadInfo.m_tvUpdateTime.sec() > m_u2ThreadTimeOut)
    {
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_WORKTHREAD, "[CMessageService::handle_timeout] pThreadInfo = [%d] State = [%d] Time = [%04d-%02d-%02d %02d:%02d:%02d] PacketCount = [%d] LastCommand = [0x%x] PacketTime = [%d] TimeOut > %d[%d] CurrPacketCount = [%d] QueueCount = [%d] BuffPacketUsed = [%d] BuffPacketFree = [%d].",
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
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_WORKTHREAD, "[CMessageService::handle_timeout] ThreadID = [%d] Thread is reset.", m_u4ThreadID);
        return false;
    }
    else
    {
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_WORKTHREAD, "[CMessageService::handle_timeout] pThreadInfo = [%d] State = [%d] Time = [%04d-%02d-%02d %02d:%02d:%02d] PacketCount = [%d] LastCommand = [0x%x] PacketTime = [%d] CurrPacketCount = [%d] QueueCount = [%d] BuffPacketUsed = [%d] BuffPacketFree = [%d].",
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

    return true;
}

_ThreadInfo* CMessageService::GetThreadInfo()
{
    return &m_ThreadInfo;
}

void CMessageService::GetAIInfo(_WorkThreadAIInfo& objAIInfo)
{
    m_WorkThreadAI.GetAIInfo(objAIInfo);
}

uint32 CMessageService::GetThreadID()
{
    return m_u4ThreadID;
}

void CMessageService::GetAITO(vecCommandTimeout& objTimeout)
{
    m_WorkThreadAI.GetAllTimeout(m_u4ThreadID, objTimeout);
}

void CMessageService::GetAITF(vecCommandTimeout& objTimeout)
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

_CommandFlowAccount CMessageService::GetCommandFlowAccount()
{
    return m_CommandAccount.GetCommandFlowAccount();
}

void CMessageService::GetCommandTimeOut(vecCommandTimeOut& CommandTimeOutList)
{
    m_CommandAccount.GetCommandTimeOut(CommandTimeOutList);
}

void CMessageService::GetCommandAlertData(vecCommandAlertData& CommandAlertDataList)
{
    m_CommandAccount.GetCommandAlertData(CommandAlertDataList);
}

void CMessageService::ClearCommandTimeOut()
{
    m_CommandAccount.ClearTimeOut();
}

void CMessageService::SaveCommandDataLog()
{
    m_CommandAccount.SaveCommandDataLog();
}

void CMessageService::SetThreadState(MESSAGE_SERVICE_THREAD_STATE emState)
{
    m_emThreadState = emState;
}

MESSAGE_SERVICE_THREAD_STATE CMessageService::GetThreadState()
{
    return m_emThreadState;
}

uint32 CMessageService::GetStepState()
{
    return m_ThreadInfo.m_u4State;
}

uint32 CMessageService::GetUsedMessageCount()
{
    return (uint32)m_MessagePool.GetUsedCount();
}

CMessage* CMessageService::CreateMessage()
{
    //OUR_DEBUG((LM_INFO, "[CMessageService::CreateMessage]GetThreadID=%d, m_MessagePool=0x%08x.\n", GetThreadID(), m_MessagePool));
    CMessage* pMessage = m_MessagePool.Create();

    if(NULL != pMessage)
    {
        pMessage->GetMessageBase()->m_u4WorkThreadID = GetThreadID();
    }

    return pMessage;
}

void CMessageService::DeleteMessage(CMessage* pMessage)
{
    //OUR_DEBUG((LM_INFO, "[CMessageService::DeleteMessage]GetThreadID=%d, m_MessagePool=0x%08x.\n", GetThreadID(), m_MessagePool));
    m_MessagePool.Delete(pMessage);
}

int CMessageService::handle_signal (int signum,siginfo_t* siginfo,ucontext_t* ucontext)
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
    // We can choose to process the message or to differ it into the message
    // queue, and process them into the svc() method. Chose the last option.
    int retval;

    ACE_Message_Block* mblk = 0;
    ACE_NEW_RETURN(mblk,ACE_Message_Block (0, ACE_Message_Block::MB_STOP),-1);

    // If queue is full, flush it before block in while
    if (msg_queue ()->is_full())
    {
        if ((retval=msg_queue ()->flush()) == -1)
        {
            OUR_DEBUG((LM_ERROR, "[CMessageService::CloseMsgQueue]put error flushing queue\n"));
            return -1;
        }
    }

    m_mutex.acquire();

    while ((retval = putq (mblk)) == -1)
    {
        if (msg_queue ()->state () != ACE_Message_Queue_Base::PULSED)
        {
            OUR_DEBUG((LM_ERROR,ACE_TEXT("[CMessageService::CloseMsgQueue]put Queue not activated.\n")));
            break;
        }
    }

    m_cond.wait();
    m_mutex.release();
    return retval;
}

CMessageServiceGroup::CMessageServiceGroup()
{
    m_u4TimerID = 0;

    uint16 u2ThreadTimeCheck = App_MainConfig::instance()->GetThreadTimeCheck();

    if(u2ThreadTimeCheck <= 0)
    {
        m_u2ThreadTimeCheck = MAX_MSG_TIMEDELAYTIME;
    }
    else
    {
        m_u2ThreadTimeCheck = u2ThreadTimeCheck;
    }
}

CMessageServiceGroup::~CMessageServiceGroup()
{
    //Close();
}

int CMessageServiceGroup::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    if(arg != NULL)
    {
        OUR_DEBUG((LM_ERROR,"[CMessageServiceGroup::handle_timeout] arg is not NULL, time is (%d).\n", tv.sec()));
    }

    //检查所有工作线程
    CheckWorkThread();

    //记录PacketParse的统计过程
    CheckPacketParsePool();

    //记录统计CPU和内存的使用
    CheckCPUAndMemory();

    //检查所有插件状态
    CheckPlugInState();

    return 0;
}

bool CMessageServiceGroup::Init(uint32 u4ThreadCount, uint32 u4MaxQueue, uint32 u4LowMask, uint32 u4HighMask, uint8 u1ServiceType)
{
    //删除以前的所有CMessageService对象
    Close();

    //记录当前设置
    m_u4MaxQueue    = u4MaxQueue;
    m_u4HighMask    = u4HighMask;
    m_u4LowMask     = u4LowMask;
    m_u1ServiceType = u1ServiceType;

    m_objAllThreadInfo.Init((int)u4ThreadCount);

    if (0 == m_u1ServiceType)
    {
        //时序模式开启
        OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::Init]Timing sequence Start.\n"));

        //初始化所有的Message对象
        for (uint32 i = 0; i < u4ThreadCount; i++)
        {
            CMessageService* pMessageService = new CMessageService();

            if (NULL == pMessageService)
            {
                OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::Init](%d)pMessageService is NULL.\n", i));
                return false;
            }

            pMessageService->Init(i, u4MaxQueue, u4LowMask, u4HighMask);
            m_vecMessageService.push_back(pMessageService);
        }
    }
    else
    {
        //随机模式开启
        OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::Init]random model Start.\n"));
    }

    //初始化随机范围，仅UDP协议包需要
    m_objRandomNumber.SetRange(0, u4ThreadCount - 1);

    return true;
}

bool CMessageServiceGroup::PutMessage(CMessage* pMessage)
{
    //判断是否为TCP包，如果是则按照ConnectID区分。UDP则随机分配一个
    int32 n4ThreadID = 0;

    //得到工作线程ID
    n4ThreadID = pMessage->GetMessageBase()->m_u4WorkThreadID;

    if(-1 == n4ThreadID)
    {
        pMessage->Clear();
        SAFE_DELETE(pMessage);
        return false;
    }

    //m_ThreadWriteLock.acquire();
    CMessageService* pMessageService = m_vecMessageService[(uint32)n4ThreadID];

    if(NULL != pMessageService)
    {
        pMessageService->PutMessage(pMessage);
    }

    //m_ThreadWriteLock.release();

    return true;
}

void CMessageServiceGroup::Close()
{
    KillTimer();

    ACE_Time_Value tvSleep(0, 1000);

    for(uint32 i = 0; i < (uint32)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            pMessageService->Close();
            ACE_OS::sleep(tvSleep);
            SAFE_DELETE(pMessageService);
        }
    }

    m_vecMessageService.clear();
}

bool CMessageServiceGroup::Start()
{
    if(StartTimer() == false)
    {
        return false;
    }

    for(uint32 i = 0; i < (uint32)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            pMessageService->Start();
        }

        OUR_DEBUG((LM_INFO,"[CMessageServiceGroup::Start](%d)WorkThread is OK.\n", i));
    }

    return true;
}

bool CMessageServiceGroup::StartTimer()
{
    OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::StartTimer] begin....\n"));

    m_u4TimerID = App_TimerManager::instance()->schedule(this, NULL, ACE_OS::gettimeofday() + ACE_Time_Value(MAX_MSG_STARTTIME), ACE_Time_Value(m_u2ThreadTimeCheck));

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

bool CMessageServiceGroup::CheckWorkThread()
{
    for (uint32 i = 0; i < (uint32)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if (NULL != pMessageService)
        {
            pMessageService->SaveThreadInfo();
            //目前在工作线程发生阻塞的时候不在杀死工程线程，杀了工作线程会导致一些内存问题。
            /*
            if (false == blFlag)
            {
                //首先让对象失效
                m_vecMessageService[i] = NULL;

                //获得当前线程ID
                uint32 u4ThreadID = pMessageService->GetThreadInfo()->m_u4ThreadID;

                //杀死当前工作线程
                int ret = ACE_Thread_Manager::instance()->cancel_task(pMessageService);

                if (0 != ret)
                {
                    OUR_DEBUG((LM_DEBUG, "[CMessageServiceGroup::CheckServerMessageThread]kill return %d fail(%d).\n", ret, errno));
                }
                else
                {
                    OUR_DEBUG((LM_DEBUG, "[CMessageServiceGroup::CheckServerMessageThread]kill Thread:%d return OK.\n",u4ThreadID));
                }

                //需要重启工作线程，先关闭当前的工作线程
                pMessageService->Close();
                SAFE_DELETE(pMessageService);

                //创建一个新的工作线程，并赋值进去
                pMessageService = new CMessageService();

                if (NULL != pMessageService)
                {
                    pMessageService->Init(u4ThreadID, m_u4MaxQueue, m_u4LowMask, m_u4HighMask);
                    pMessageService->Start();

                    //m_ThreadWriteLock.acquire();
                    m_vecMessageService[i] = pMessageService;
                    //m_ThreadWriteLock.release();
                }
                else
                {
                    OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::handle_timeout] reset workthread is NULL (%d).\n", i));
                }
            }
            */
        }
    }

    return true;
}

bool CMessageServiceGroup::CheckPacketParsePool()
{
    AppLogManager::instance()->WriteLog(LOG_SYSTEM_PACKETTHREAD, "[CMessageService::handle_timeout] UsedCount = %d, FreeCount= %d.", App_PacketParsePool::instance()->GetUsedCount(), App_PacketParsePool::instance()->GetFreeCount());
    return true;
}

bool CMessageServiceGroup::CheckCPUAndMemory()
{
    if (App_MainConfig::instance()->GetMonitor() == 1)
    {
#ifdef WIN32
        uint32 u4CurrCpu = (uint32)GetProcessCPU_Idel();
        //uint32 u4CurrMemory = (uint32)GetProcessMemorySize();
#else
        uint32 u4CurrCpu = (uint32)GetProcessCPU_Idel_Linux();
        //uint32 u4CurrMemory = (uint32)GetProcessMemorySize_Linux();
#endif

        //获得相关Messageblock,BuffPacket,MessageCount,内存大小
        uint32 u4MessageBlockUsedSize = App_MessageBlockManager::instance()->GetUsedSize();
        uint32 u4BuffPacketCount = App_BuffPacketManager::instance()->GetBuffPacketUsedCount();
        uint32 u4MessageCount = GetUsedMessageCount();

        if (u4CurrCpu > App_MainConfig::instance()->GetCpuMax() || u4MessageBlockUsedSize > App_MainConfig::instance()->GetMemoryMax())
        {
            OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::handle_timeout]CPU Rote=%d,MessageBlock=%d,u4BuffPacketCount=%d,u4MessageCount=%d ALERT.\n", u4CurrCpu, u4MessageBlockUsedSize, u4BuffPacketCount, u4MessageCount));
            AppLogManager::instance()->WriteLog(LOG_SYSTEM_MONITOR, "[Monitor] CPU Rote=%d,MessageBlock=%d,u4BuffPacketCount=%d,u4MessageCount=%d.", u4CurrCpu, u4MessageBlockUsedSize, u4BuffPacketCount, u4MessageCount);
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::handle_timeout]CPU Rote=%d,MessageBlock=%d,u4BuffPacketCount=%d,u4MessageCount=%d OK.\n", u4CurrCpu, u4MessageBlockUsedSize, u4BuffPacketCount, u4MessageCount));
        }
    }

    return true;
}

bool CMessageServiceGroup::CheckPlugInState()
{
    vector<_ModuleInfo*> vecModeInfo;
    App_ModuleLoader::instance()->GetAllModuleInfo(vecModeInfo);

    for (int i = 0; i < (int)vecModeInfo.size(); i++)
    {
        _ModuleInfo* pModuleInfo = vecModeInfo[i];

        if (NULL != pModuleInfo)
        {
            uint32 u4ErrorID = 0;
            bool blModuleState = pModuleInfo->GetModuleState(u4ErrorID);

            if (false == blModuleState)
            {
                char szTitle[MAX_BUFF_50] = { '\0' };
                sprintf_safe(szTitle, MAX_BUFF_50, "ModuleStateError");

                //发送邮件
                AppLogManager::instance()->WriteToMail(LOG_SYSTEM_MONITOR, 1,
                                                       szTitle,
                                                       "Module ErrorID=%d.\n",
                                                       u4ErrorID);
            }
        }
    }

    return true;
}

CThreadInfo* CMessageServiceGroup::GetThreadInfo()
{
    for(uint32 i = 0; i < (uint32)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            _ThreadInfo* pThreadInfo = pMessageService->GetThreadInfo();

            if(NULL != pThreadInfo)
            {
                m_objAllThreadInfo.AddThreadInfo(i, pThreadInfo);
            }
        }
    }

    return &m_objAllThreadInfo;
}

uint32 CMessageServiceGroup::GetUsedMessageCount()
{
    uint32 u4Count = 0;

    for (int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        u4Count += m_vecMessageService[i]->GetUsedMessageCount();
    }

    return u4Count;
}

uint32 CMessageServiceGroup::GetWorkThreadCount()
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
        return m_vecMessageService[u4Index]->GetThreadInfo()->m_u4ThreadID;
    }
}

void CMessageServiceGroup::GetWorkThreadAIInfo(vecWorkThreadAIInfo& objvecWorkThreadAIInfo)
{
    objvecWorkThreadAIInfo.clear();

    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        _WorkThreadAIInfo objWorkThreadAIInfo;
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
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

    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        _WorkThreadAIInfo objWorkThreadAIInfo;
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            pMessageService->GetAITO(objTimeout);
        }
    }
}

void CMessageServiceGroup::GetAITF(vecCommandTimeout& objTimeout)
{
    objTimeout.clear();

    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            pMessageService->GetAITF(objTimeout);
        }
    }
}

void CMessageServiceGroup::SetAI(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime)
{
    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            pMessageService->SetAI(u1AI, u4DisposeTime, u4WTCheckTime, u4WTStopTime);
        }
    }
}

void CMessageServiceGroup::GetCommandData(uint16 u2CommandID, _CommandData& objCommandData)
{
    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            _CommandData* pCommandData = pMessageService->GetCommandData(u2CommandID);

            if(NULL != pCommandData)
            {
                objCommandData += (*pCommandData);
            }
        }
    }
}

void CMessageServiceGroup::GetFlowInfo(_CommandFlowAccount& objCommandFlowAccount)
{
    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            _CommandFlowAccount CommandFlowAccount = pMessageService->GetCommandFlowAccount();
            objCommandFlowAccount += CommandFlowAccount;
        }
    }
}

void CMessageServiceGroup::GetCommandTimeOut(vecCommandTimeOut& CommandTimeOutList)
{
    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            pMessageService->GetCommandTimeOut(CommandTimeOutList);
        }
    }
}

void CMessageServiceGroup::GetCommandAlertData(vecCommandAlertData& CommandAlertDataList)
{
    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            pMessageService->GetCommandAlertData(CommandAlertDataList);
        }
    }
}

void CMessageServiceGroup::ClearCommandTimeOut()
{
    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            pMessageService->ClearCommandTimeOut();
        }
    }
}

void CMessageServiceGroup::SaveCommandDataLog()
{
    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            pMessageService->SaveCommandDataLog();
        }
    }
}

bool CMessageServiceGroup::UnloadModule(const char* pModuleName, uint8 u1State)
{
    OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::UnloadModule] Begin.\n"));

    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            pMessageService->SetThreadState(THREAD_MODULE_UNLOAD);
        }
    }

    OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::UnloadModule] SET THREAD_MODULE_UNLOAD.\n"));

    //等待所有进程执行结果
    bool blWait = true;

    while(blWait)
    {
        blWait = false;

        for(int i = 0; i < (int)m_vecMessageService.size(); i++)
        {
            CMessageService* pMessageService = m_vecMessageService[i];

            if(NULL != pMessageService)
            {
                uint32 u4State = pMessageService->GetStepState();

                if(THREAD_RUNBEGIN == u4State || THREAD_BLOCK == u4State)
                {
                    //不是所有线程都执行完了，继续等
                    blWait = true;
                    break;
                }
            }
        }

        if(blWait == true)
        {
            ACE_Time_Value tvSleep(0, 1000);
            ACE_OS::sleep(tvSleep);
        }
    }

    OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::UnloadModule] THREAD_MODULE_UNLOAD OVER.\n"));

    //等待结束，开始重载
    App_MessageManager::instance()->UnloadModuleCommand(pModuleName, u1State);

    OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::UnloadModule] UnloadModuleCommand OK.\n"));

    //重载结束，全部恢复线程工作
    for(int i = 0; i < (int)m_vecMessageService.size(); i++)
    {
        CMessageService* pMessageService = m_vecMessageService[i];

        if(NULL != pMessageService)
        {
            pMessageService->SetThreadState(THREAD_RUN);
        }
    }

    OUR_DEBUG((LM_ERROR, "[CMessageServiceGroup::UnloadModule] End.\n"));

    return true;
}

CMessage* CMessageServiceGroup::CreateMessage(uint32 u4ConnectID, uint8 u1PacketType)
{
    int32 n4ThreadID = 0;
    n4ThreadID = GetWorkThreadID(u4ConnectID, u1PacketType);

    if(-1 == n4ThreadID)
    {
        return NULL;
    }

    //OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::CreateMessage]n4ThreadID=%d.\n", n4ThreadID));

    CMessageService* pMessageService = m_vecMessageService[(uint32)n4ThreadID];

    if(NULL != pMessageService)
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
    if(u4ConnectID == 0)
    {
        OUR_DEBUG((LM_INFO, "[CMessageServiceGroup::DeleteMessage]u4ConnectID=%d.\n", u4ConnectID));
    }

    int32 n4ThreadID  = 0;
    n4ThreadID = pMessage->GetMessageBase()->m_u4WorkThreadID;

    if(-1 == n4ThreadID)
    {
        pMessage->Clear();
        SAFE_DELETE(pMessage);
        return;
    }

    CMessageService* pMessageService = m_vecMessageService[(uint32)n4ThreadID];

    if(NULL != pMessageService)
    {
        pMessageService->DeleteMessage(pMessage);
    }
}

int32 CMessageServiceGroup::GetWorkThreadID(uint32 u4ConnectID, uint8 u1PackeType)
{
    int32 n4ThreadID = -1;

    if(m_vecMessageService.size() == 0)
    {
        return n4ThreadID;
    }

    if(u1PackeType == PACKET_TCP)
    {
        n4ThreadID = u4ConnectID % (uint32)m_vecMessageService.size();
    }
    else if(u1PackeType == PACKET_UDP)
    {
        //如果是UDP协议，则获取当前随机数值
        n4ThreadID = m_objRandomNumber.GetRandom();
    }

    return n4ThreadID;
}
