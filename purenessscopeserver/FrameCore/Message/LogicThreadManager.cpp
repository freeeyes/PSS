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
    Close();
}

void CLogicThread::Init(CLogicThreadInfo objThreadInfo)
{
    m_objThreadInfo = objThreadInfo;

    //设置消息池
    m_MessagePool.Init(MAX_MESSAGE_POOL, CLogicThreadMessagePool::Init_Callback);
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

bool CLogicThread::PutMessage(int nMessageID, void* pParam)
{
    CLogicThreadMessage* pMessage = m_MessagePool.Create();

    if (NULL != pMessage)
    {
        ACE_Message_Block* mb = pMessage->GetQueueMessage();

        ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, 10000);

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

        ThreadReturn emRet;

        m_u4ThreadState = THREAD_RUNBEGIN;

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

}

CLogicThreadManager::~CLogicThreadManager()
{

}

int CLogicThreadManager::CreateLogicThread(int nLogicThreadID, int nTimeout, ThreadInit thread_init, ThreadCallbackLogic thread_callback_logic, ThreadErrorLogic thread_callback_error, ThreadExit thread_exit)
{
    return 0;
}

int CLogicThreadManager::KillLogicThread(int nLogicThreadID)
{
    return 0;
}

int CLogicThreadManager::MessageMappingLogicThread(int nLogicThreadID, int nMessageID)
{
    return 0;
}

int CLogicThreadManager::SendLogicThreadMessage(int nMessageID, void* arg)
{
    return 0;
}
