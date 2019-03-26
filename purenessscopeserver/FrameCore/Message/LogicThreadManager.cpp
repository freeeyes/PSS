#include "LogicThreadManager.h"

CLogicThreadMessagePool::CLogicThreadMessagePool()
{
}

CLogicThreadMessagePool::CLogicThreadMessagePool(const CLogicThreadMessagePool& ar)
{
    (*this) = ar;
}

void CLogicThreadMessagePool::Init_Callback(int nIndex, CLogicThreadMessage* pMessage)
{
    pMessage->SetHashID(nIndex);
}

void CLogicThreadMessagePool::Close_Callback(int nIndex, CLogicThreadMessage* pMessage)
{
    ACE_UNUSED_ARG(nIndex);
    ACE_UNUSED_ARG(pMessage);
}

CLogicThreadMessage* CLogicThreadMessagePool::Create()
{
    return dynamic_cast<CLogicThreadMessage*>(CObjectPoolManager<CLogicThreadMessage, ACE_Recursive_Thread_Mutex>::Create(__FILE__, __LINE__));
}

bool CLogicThreadMessagePool::Delete(CLogicThreadMessage* pMessage)
{
    if (NULL == pMessage)
    {
        return false;
    }

    pMessage->Clear();

    bool blState = CObjectPoolManager<CLogicThreadMessage, ACE_Recursive_Thread_Mutex>::Delete(pMessage->GetHashID(), pMessage);

    if (false == blState)
    {
        OUR_DEBUG((LM_INFO, "[CLogicThreadMessagePool::Delete]HashID=%d(0x%08x).\n", pMessage->GetHashID(), pMessage));
    }

    return true;
}

CLogicThreadMessagePool::~CLogicThreadMessagePool()
{
    OUR_DEBUG((LM_INFO, "[CLogicThreadMessagePool::~CLogicThreadMessagePool].\n"));
    OUR_DEBUG((LM_INFO, "[CLogicThreadMessagePool::~CLogicThreadMessagePool]End.\n"));
}

//========================================================
CLogicThread::CLogicThread() : m_blRun(false),
    m_logicthreadmutex(),
    m_logicthreadcond(m_logicthreadmutex),
    m_u4ThreadState(0)
{
}

CLogicThread::CLogicThread(const CLogicThread& ar) : CLogicThread()
{
    (*this) = ar;
}

CLogicThread::~CLogicThread()
{
}

void CLogicThread::Init(CLogicThreadInfo objThreadInfo)
{
    m_objThreadInfo = objThreadInfo;

    //设置消息池
    m_MessagePool.Init(MAX_MESSAGE_POOL, CLogicThreadMessagePool::Init_Callback);
}

bool CLogicThread::Start()
{
    if (0 != open())
    {
        return false;
    }
    else
    {
        return true;
    }
}

int CLogicThread::handle_signal(int signum, siginfo_t* siginfo, ucontext_t* ucontext)
{
    if (signum == SIGUSR1 + grp_id())
    {
        OUR_DEBUG((LM_INFO, "[CLogicThread::handle_signal(](%d) will be kill.\n", grp_id()));

        if (NULL != siginfo && NULL != ucontext)
        {
            OUR_DEBUG((LM_INFO, "[CLogicThread::handle_signal(]siginfo is not null.\n"));
        }

        ACE_Thread::exit();
    }

    return 0;
}

int CLogicThread::open(void* args)
{
    if (args != NULL)
    {
        OUR_DEBUG((LM_INFO, "[CLogicThread::open]args is not NULL.\n"));
    }

    m_blRun = true;

    if (activate(THREAD_PARAM, MAX_MSG_THREADCOUNT) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CMessageService::open] activate error ThreadCount = [%d].\n", MAX_MSG_THREADCOUNT));
        m_blRun = false;
        return -1;
    }

    resume();
    return 0;
}

int CLogicThread::svc(void)
{
    //调用初始化线程操作
    m_u4ThreadState = THREAD_INIT;

    if (NULL != m_objThreadInfo.fn_thread_init)
    {
        m_objThreadInfo.fn_thread_init(m_objThreadInfo.m_nLogicThreadID);
    }

    while (m_blRun)
    {
        if (false == Dispose_Queue())
        {
            break;
        }

        //使用内存池，这块内存不必再释放
    }

    //调用线程结束函数
    if (NULL != m_objThreadInfo.fn_thread_exit)
    {
        m_objThreadInfo.fn_thread_exit(m_objThreadInfo.m_nLogicThreadID);
    }

    return 0;
}

int CLogicThread::Close()
{
    if (m_blRun)
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

    m_MessagePool.Close_Object(CLogicThreadMessagePool::Close_Callback);

    return 0;
}

bool CLogicThread::CheckTimeout(ACE_Time_Value tvNow)
{
    if (THREAD_RUNBEGIN == m_u4ThreadState)
    {
        ACE_Time_Value tvInterval = tvNow - m_tvUpdateTime;

        if (tvInterval.sec() > m_objThreadInfo.m_nTimeout)
        {
            //回调线程超时错误接口
            m_objThreadInfo.fn_thread_callback_error(m_objThreadInfo.m_nLogicThreadID, 1);
            return false;
        }
    }

    return true;
}

bool CLogicThread::PutMessage(int nMessageID, void* pParam)
{
    CLogicThreadMessage* pMessage = m_MessagePool.Create();

    if (NULL != pMessage)
    {
        pMessage->m_nMessageID = nMessageID;
        pMessage->m_pParam     = pParam;

        ACE_Message_Block* mb  = pMessage->GetQueueMessage();
        ACE_Time_Value xtime   = ACE_OS::gettimeofday() + ACE_Time_Value(0, 10000);

        if (this->putq(mb, &xtime) == -1)
        {
            OUR_DEBUG((LM_ERROR, "[CLogicThread::PutMessage] Queue putq  error errno = [%d].\n", errno));
            return false;
        }
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CLogicThread::PutMessage]Create fail.\n"));
        return false;
    }

    return true;
}

CLogicThreadInfo* CLogicThread::GetThreadInfo()
{
    return &m_objThreadInfo;
}

int CLogicThread::CloseMsgQueue()
{
    return Task_Common_CloseMsgQueue((ACE_Task<ACE_MT_SYNCH>*)this, m_logicthreadcond, m_logicthreadmutex);
}

bool CLogicThread::Dispose_Queue()
{
    ACE_Message_Block* mb = NULL;
    ACE_OS::last_error(0);

    if (getq(mb, 0) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CLogicThread::Dispose_Queue] PutMessage error errno = [%d].\n", ACE_OS::last_error()));
        m_blRun = false;
        return false;
    }
    else if (mb->msg_type() == ACE_Message_Block::MB_STOP)
    {
        m_logicthreadmutex.acquire();
        mb->release();
        this->msg_queue()->deactivate();
        m_logicthreadcond.signal();
        m_logicthreadmutex.release();
        m_blRun = false;
        return false;
    }
    else
    {
        //处理消息
        CLogicThreadMessage* msg = *((CLogicThreadMessage**)mb->base());

        ThreadReturn emRet = THREAD_Task_Finish;

        m_u4ThreadState = THREAD_RUNBEGIN;
        m_tvUpdateTime  = ACE_OS::gettimeofday();

        if (NULL != m_objThreadInfo.fn_thread_callback_logic)
        {
            emRet = m_objThreadInfo.fn_thread_callback_logic(m_objThreadInfo.m_nLogicThreadID,
                    msg->m_nMessageID,
                    msg->m_pParam);

        }

        //回收消息对象
        m_MessagePool.Delete(msg);

        m_u4ThreadState = THREAD_RUNEND;

        if (THREAD_Task_Finish == emRet)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

CLogicThreadManager::CLogicThreadManager()
{
    m_u4TimerID = 0;
}

CLogicThreadManager::~CLogicThreadManager()
{
}

int CLogicThreadManager::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWriteLock);
    ACE_UNUSED_ARG(arg);

    vector<CLogicThread*> vecLogicThreadList;

    m_objThreadInfoList.Get_All_Used(vecLogicThreadList);

    for (int i = 0; i < (int)vecLogicThreadList.size(); i++)
    {
        vecLogicThreadList[i]->CheckTimeout(tv);
    }

    return 0;
}

void CLogicThreadManager::Init()
{
    //初始化Hash表
    m_objThreadInfoList.Init(LOGICTHREAD_MAX_COUNT);
    m_objMessageIDList.Init(LOGICTHREAD_MESSAGE_MAX_COUNT);

    //启动定时器(默认一分钟一次)
    m_u4TimerID = App_TimerManager::instance()->schedule(this, NULL, ACE_OS::gettimeofday() + ACE_Time_Value(MAX_MSG_STARTTIME), ACE_Time_Value(MAX_MSG_TIMEDELAYTIME));
}

void CLogicThreadManager::Close()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWriteLock);

    //停止定时器
    if (m_u4TimerID > 0)
    {
        App_TimerManager::instance()->cancel(m_u4TimerID);
        m_u4TimerID = 0;
    }

    ACE_Time_Value tvSleep(0, 1000);
    vector<CLogicThread*> vecLogicThreadList;

    m_objThreadInfoList.Get_All_Used(vecLogicThreadList);

    for (int i = 0; i < (int)vecLogicThreadList.size(); i++)
    {
        vecLogicThreadList[i]->Close();

        ACE_OS::sleep(tvSleep);
        SAFE_DELETE(vecLogicThreadList[i]);
    }

    vecLogicThreadList.clear();
}

int CLogicThreadManager::CreateLogicThread(int nLogicThreadID, int nTimeout, ThreadInit thread_init, ThreadCallbackLogic thread_callback_logic, ThreadErrorLogic thread_callback_error, ThreadExit thread_exit)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWriteLock);

    CLogicThread* pLogicThread = m_objThreadInfoList.Get_Hash_Box_Data_By_Uint32((uint32)nLogicThreadID);

    if (NULL != pLogicThread)
    {
        OUR_DEBUG((LM_INFO, "[CLogicThreadManager::CreateLogicThread]Create logic thread id(%d) is exist.\n", nLogicThreadID));
        return -1;
    }

    CLogicThreadInfo* pLogicThreadInfo = new CLogicThreadInfo();

    pLogicThreadInfo->m_nLogicThreadID         = nLogicThreadID;
    pLogicThreadInfo->m_nTimeout               = nTimeout;
    pLogicThreadInfo->fn_thread_init           = thread_init;
    pLogicThreadInfo->fn_thread_callback_logic = thread_callback_logic;
    pLogicThreadInfo->fn_thread_callback_error = thread_callback_error;
    pLogicThreadInfo->fn_thread_exit           = thread_exit;

    pLogicThread = new CLogicThread();
    pLogicThread->Init(*pLogicThreadInfo);

    if (false == pLogicThread->Start())
    {
        OUR_DEBUG((LM_INFO, "[CLogicThreadManager::CreateLogicThread]nLogicThreadID=%d Start error.\n", nLogicThreadID));
        SAFE_DELETE(pLogicThreadInfo);
        SAFE_DELETE(pLogicThread);
        return -1;
    }

    if (0 > m_objThreadInfoList.Add_Hash_Data_By_Key_Unit32(nLogicThreadID, pLogicThread))
    {
        OUR_DEBUG((LM_INFO, "[CLogicThreadManager::CreateLogicThread]nLogicThreadID=%d add error.\n", nLogicThreadID));
        SAFE_DELETE(pLogicThreadInfo);
        SAFE_DELETE(pLogicThread);
        return -1;
    }

    return 0;
}

int CLogicThreadManager::KillLogicThread(int nLogicThreadID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWriteLock);

    CLogicThread* pLogicThread = m_objThreadInfoList.Get_Hash_Box_Data_By_Uint32((uint32)nLogicThreadID);

    if (NULL == pLogicThread)
    {
        OUR_DEBUG((LM_INFO, "[CLogicThreadManager::CreateLogicThread]Create logic thread id(%d) is no exist.\n", nLogicThreadID));
        return -1;
    }
    else
    {
        pLogicThread->Close();
        SAFE_DELETE(pLogicThread);
        m_objThreadInfoList.Del_Hash_Data_By_Unit32((uint32)nLogicThreadID);
        return 0;
    }
}

int CLogicThreadManager::MessageMappingLogicThread(int nLogicThreadID, int nMessageID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWriteLock);

    CLogicThreadInfo* pLogicThreadInfo = NULL;
    CLogicThread* pLogicThread = m_objThreadInfoList.Get_Hash_Box_Data_By_Uint32((uint32)nLogicThreadID);

    if (NULL == pLogicThread)
    {
        OUR_DEBUG((LM_INFO, "[CLogicThreadManager::CreateLogicThread]Create logic thread id(%d) is no exist.\n", nLogicThreadID));
        return -1;
    }

    pLogicThreadInfo = m_objMessageIDList.Get_Hash_Box_Data_By_Uint32((uint32)nMessageID);

    if (NULL != pLogicThreadInfo)
    {
        OUR_DEBUG((LM_INFO, "[CLogicThreadManager::CreateLogicThread]Message id(%d) at logic thread(%d) is exist.\n",
                   nMessageID,
                   pLogicThreadInfo->m_nLogicThreadID));
        return -1;
    }

    pLogicThreadInfo = pLogicThread->GetThreadInfo();

    if (0 > m_objMessageIDList.Add_Hash_Data_By_Key_Unit32((uint32)nMessageID, pLogicThreadInfo))
    {
        OUR_DEBUG((LM_INFO, "[CLogicThreadManager::CreateLogicThread]Message id(%d) at logic thread(%d) add error.\n",
                   nMessageID,
                   pLogicThreadInfo->m_nLogicThreadID));
        return -1;
    }

    return 0;
}

int CLogicThreadManager::SendLogicThreadMessage(int nMessageID, void* arg)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWriteLock);

    CLogicThreadInfo* pLogicThreadInfo = m_objMessageIDList.Get_Hash_Box_Data_By_Uint32((uint32)nMessageID);

    if (NULL == pLogicThreadInfo)
    {
        OUR_DEBUG((LM_INFO, "[CLogicThreadManager::CreateLogicThread]Message id(%d) at logic is no exist.\n",
                   nMessageID,
                   pLogicThreadInfo->m_nLogicThreadID));
        return -1;
    }

    CLogicThread* pLogicThread = m_objThreadInfoList.Get_Hash_Box_Data_By_Uint32((uint32)pLogicThreadInfo->m_nLogicThreadID);

    if (NULL == pLogicThread)
    {
        OUR_DEBUG((LM_INFO, "[CLogicThreadManager::CreateLogicThread]Create logic thread id(%d) is no exist.\n", pLogicThreadInfo->m_nLogicThreadID));
        return -1;
    }

    if (false == pLogicThread->PutMessage(nMessageID, arg))
    {
        OUR_DEBUG((LM_INFO, "[CLogicThreadManager::CreateLogicThread]Create logic thread id(%d) putmessage error.\n", pLogicThreadInfo->m_nLogicThreadID));
        return -1;
    }

    return 0;
}
