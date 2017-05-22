#include "ServerMessageTask.h"

CServerMessageInfoPool::CServerMessageInfoPool()
{

}

CServerMessageInfoPool::~CServerMessageInfoPool()
{
    OUR_DEBUG((LM_INFO, "[CMessagePool::~CMessagePool].\n"));
    Close();
    OUR_DEBUG((LM_INFO, "[CMessagePool::~CMessagePool]End.\n"));
}

void CServerMessageInfoPool::Init(uint32 u4PacketCount /*= MAX_SERVER_MESSAGE_INFO_COUNT*/)
{
    Close();

    m_objServerMessageList.Init((int)u4PacketCount);

    for(int i = 0; i < (int)u4PacketCount; i++)
    {
        _Server_Message_Info* pPacket = new _Server_Message_Info();

        if(NULL != pPacket)
        {
            //添加到Hash数组里面
            char szMessageID[10] = {'\0'};
            sprintf_safe(szMessageID, 10, "%d", i);
            int nHashPos = m_objServerMessageList.Add_Hash_Data(szMessageID, pPacket);

            if(-1 != nHashPos)
            {
                pPacket->SetHashID(nHashPos);
            }
        }
    }
}

int CServerMessageInfoPool::GetUsedCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return m_objServerMessageList.Get_Count() - m_objServerMessageList.Get_Used_Count();
}

int CServerMessageInfoPool::GetFreeCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return m_objServerMessageList.Get_Used_Count();
}

_Server_Message_Info* CServerMessageInfoPool::Create()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    //在Hash表中弹出一个已使用的数据
	_Server_Message_Info* pMessage = m_objServerMessageList.Pop();

    //没找到空余的
    return pMessage;
}

bool CServerMessageInfoPool::Delete(_Server_Message_Info* pObject)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if(NULL == pObject)
    {
        return false;
    }

    char szHashID[10] = {'\0'};
    sprintf_safe(szHashID, 10, "%d", pObject->GetHashID());
    bool blState = m_objServerMessageList.Push(szHashID, pObject);

    if(false == blState)
    {
        OUR_DEBUG((LM_INFO, "[CServerMessageInfoPool::Delete]szHandlerID=%s(0x%08x).\n", szHashID, pObject));
    }
    else
    {
        //OUR_DEBUG((LM_INFO, "[CServerMessageInfoPool::Delete]szHandlerID=%s(0x%08x) nPos=%d.\n", szHashID, pObject, nPos));
    }


    return true;
}


void CServerMessageInfoPool::Close()
{
    //清理所有已存在的指针
    vector<_Server_Message_Info*> vecServerMessageInfo;
    m_objServerMessageList.Get_All_Used(vecServerMessageInfo);

    for(int i = 0; i < (int)vecServerMessageInfo.size(); i++)
    {
        _Server_Message_Info* pMessage = vecServerMessageInfo[i];
        SAFE_DELETE(pMessage);
    }

    m_objServerMessageList.Close();
}

CServerMessageTask::CServerMessageTask()
{
    m_u4ThreadID = 0;
    m_blRun      = false;
    m_u4MaxQueue = MAX_SERVER_MESSAGE_QUEUE;
    m_emState    = SERVER_RECV_INIT;
}

CServerMessageTask::~CServerMessageTask()
{
    OUR_DEBUG((LM_INFO, "[CServerMessageTask::~CServerMessageTask].\n"));
}

bool CServerMessageTask::IsRun()
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_RunMutex);

    return m_blRun;
}

bool CServerMessageTask::Start()
{
    if(0 != open())
    {
        return false;
    }

    return true;
}

int CServerMessageTask::handle_signal (int signum,
                                       siginfo_t* siginfo,
                                       ucontext_t* ucontext)
{
    if (signum == SIGUSR1 + grp_id())
    {
        OUR_DEBUG((LM_INFO,"[CServerMessageTask::handle_signal](%d) will be kill.\n", grp_id()));

        if(NULL != siginfo && NULL != ucontext)
        {
            OUR_DEBUG((LM_INFO,"[CServerMessageTask::handle_signal]siginfo is not null.\n"));
        }

        ACE_Thread::exit();
    }

    return 0;
}

int CServerMessageTask::open(void* args /*= 0*/)
{
    if(args != NULL)
    {
        OUR_DEBUG((LM_INFO,"[CServerMessageTask::open]args is not NULL.\n"));
    }

    m_blRun = true;

    if(activate(THREAD_PARAM, MAX_MSG_THREADCOUNT) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CServerMessageTask::open] activate error ThreadCount = [%d].", MAX_MSG_THREADCOUNT));
        m_blRun = false;
        return -1;
    }

    resume();

    return 0;
}

int CServerMessageTask::Close()
{
    m_blRun = false;
    msg_queue()->deactivate();
    msg_queue()->flush();
    OUR_DEBUG((LM_INFO, "[CServerMessageTask::Close] Close().\n"));
    return 0;
}

int CServerMessageTask::svc(void)
{
    //稍微休息一下，等一下其他线程再如主循环
    ACE_Time_Value tvSleep(0, MAX_MSG_SENDCHECKTIME*MAX_BUFF_1000);
    ACE_OS::sleep(tvSleep);

    while(IsRun())
    {
		ACE_Message_Block* mb = NULL;

        //xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);
        if(getq(mb, 0) == -1)
        {
            OUR_DEBUG((LM_ERROR,"[CMessageService::svc] PutMessage error errno = [%d].\n", errno));
            m_blRun = false;
            break;
        }

        if (mb == NULL)
        {
            continue;
        }

        _Server_Message_Info* msg = *((_Server_Message_Info**)mb->base());

        if (! msg)
        {
            OUR_DEBUG((LM_ERROR,"[CMessageService::svc] mb msg == NULL CurrthreadNo=[%d]!\n", m_u4ThreadID));
            continue;
        }

        this->ProcessMessage(msg, m_u4ThreadID);
        App_ServerMessageInfoPool::instance()->Delete(msg);
    }

    OUR_DEBUG((LM_INFO,"[CServerMessageTask::svc] svc finish!\n"));
    return 0;
}

uint32 CServerMessageTask::GetThreadID()
{
    return m_u4ThreadID;
}

bool CServerMessageTask::PutMessage(_Server_Message_Info* pMessage)
{
    ACE_Message_Block* mb = pMessage->GetQueueMessage();

    if(NULL != mb)
    {
        //判断队列是否是已经最大
        int nQueueCount = (int)msg_queue()->message_count();

        if(nQueueCount >= (int)m_u4MaxQueue)
        {
            OUR_DEBUG((LM_ERROR,"[CServerMessageTask::PutMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));
            return false;
        }

        ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, 100000);

        if(this->putq(mb, &xtime) == -1)
        {
            OUR_DEBUG((LM_ERROR,"[CServerMessageTask::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));
            return false;
        }
    }
    else
    {
        OUR_DEBUG((LM_ERROR,"[CServerMessageTask::PutMessage] mb new error.\n"));
        return false;
    }

    return true;
}

bool CServerMessageTask::ProcessMessage(_Server_Message_Info* pMessage, uint32 u4ThreadID)
{
    if(NULL == pMessage)
    {
        OUR_DEBUG((LM_DEBUG, "[CServerMessageTask::ProcessMessage]u4ThreadID=%d, pMessage is NULL\n", u4ThreadID));
        return false;
    }

    //如果此数据处理指针已经不是有效指针，直接返回。
    if(CheckValidClientMessage(pMessage->m_pClientMessage) == false)
    {
        OUR_DEBUG((LM_DEBUG, "[CServerMessageTask::ProcessMessage]u4ThreadID=%d, m_pClientMessage is NULL\n", u4ThreadID));
        return true;
    }

    m_tvDispose = ACE_OS::gettimeofday();

    m_emState   = SERVER_RECV_BEGIN;
    //调用数据包处理
    pMessage->m_pClientMessage->RecvData(pMessage->m_u2CommandID, pMessage->m_pRecvFinish, pMessage->m_objServerIPInfo);
    //回收处理包
    App_MessageBlockManager::instance()->Close(pMessage->m_pRecvFinish);
    m_emState = SERVER_RECV_END;
    return true;
}

bool CServerMessageTask::CheckServerMessageThread(ACE_Time_Value tvNow)
{
    ACE_Time_Value tvIntval(tvNow - m_tvDispose);

    if(m_emState == SERVER_RECV_BEGIN && tvIntval.sec() > MAX_DISPOSE_TIMEOUT)
    {
        return false;
    }
    else
    {
        return true;
    }

}

void CServerMessageTask::AddClientMessage(IClientMessage* pClientMessage)
{
    //先查找有效的列表中是否包含此指针
    for(int i = 0; i < (int)m_vecValidIClientMessage.size(); i++)
    {
        if(m_vecValidIClientMessage[i] == pClientMessage)
        {
            //找到了，什么都不做
            return;
        }
    }

    m_vecValidIClientMessage.push_back(pClientMessage);

}

void CServerMessageTask::DelClientMessage(IClientMessage* pClientMessage)
{
    //先查找有效的列表中是否包含此指针
	for (vecValidIClientMessage::iterator b = m_vecValidIClientMessage.begin(); b != m_vecValidIClientMessage.end(); b++)
    {
        if(*b == pClientMessage)
        {
            //找到了，什么都不做
            m_vecValidIClientMessage.erase(b);
            return;
        }
    }
}

bool CServerMessageTask::CheckValidClientMessage(IClientMessage* pClientMessage)
{
    for(int i = 0; i < (int)m_vecValidIClientMessage.size(); i++)
    {
        if(m_vecValidIClientMessage[i] == pClientMessage)
        {
            return true;
        }
    }

    return false;
}

//************************************************

CServerMessageManager::CServerMessageManager()
{
    m_pServerMessageTask = NULL;
    Init();
}

CServerMessageManager::~CServerMessageManager()
{
    OUR_DEBUG((LM_DEBUG, "[CServerMessageManager::~CServerMessageManager].\n"));
}

void CServerMessageManager::Init()
{
    if(NULL == m_pServerMessageTask)
    {
        m_pServerMessageTask = new CServerMessageTask();
    }
}

bool CServerMessageManager::Start()
{
    if(NULL != m_pServerMessageTask)
    {
        bool blState = m_pServerMessageTask->Start();

        return blState;
    }
    else
    {
        return false;
    }
}

int CServerMessageManager::Close()
{
    if(NULL != m_pServerMessageTask)
    {
        m_pServerMessageTask->Close();
        OUR_DEBUG((LM_DEBUG, "[CServerMessageManager::Close]SAFE_DELETE Begin.\n"));
        SAFE_DELETE(m_pServerMessageTask);
        OUR_DEBUG((LM_DEBUG, "[CServerMessageManager::Close]SAFE_DELETE End.\n"));
        return 0;
    }
    else
    {
        return -1;
    }
}

bool CServerMessageManager::PutMessage(_Server_Message_Info* pMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    if(NULL != m_pServerMessageTask)
    {
        return m_pServerMessageTask->PutMessage(pMessage);
    }
    else
    {
        return false;
    }
}

bool CServerMessageManager::CheckServerMessageThread(ACE_Time_Value tvNow)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    if(NULL != m_pServerMessageTask)
    {
        bool blRet = m_pServerMessageTask->CheckServerMessageThread(tvNow);

        if(false == blRet)
        {
            OUR_DEBUG((LM_DEBUG, "[CServerMessageManager::CheckServerMessageThread]***App_ServerMessageTask Thread is DEAD***.\n"));

            //如果发现已经可能死亡，尝试重启线程
#ifdef WIN32
            ACE_hthread_t hthread = 0;
            int grp_id = m_pServerMessageTask->grp_id();

            if (ACE_Thread_Manager::instance()->hthread_grp_list(grp_id, &hthread, 1) == 1)
            {
                int ret = ::TerminateThread (hthread, -1);
                ACE_Thread_Manager::instance()->wait_grp (grp_id);
                OUR_DEBUG((LM_DEBUG, "[CServerMessageManager::CheckServerMessageThread]kill return %d, %d\n", ret, GetLastError()));
            }

#else
            int ret = ACE_Thread_Manager::instance()->cancel_task(m_pServerMessageTask, 1);
            OUR_DEBUG((LM_DEBUG, "[CServerMessageManager::CheckServerMessageThread]kill return %d OK.\n", ret));
#endif
            m_pServerMessageTask->Close();
            SAFE_DELETE(m_pServerMessageTask);

            //重建并重启相应线程
            Init();
            Start();
        }

        return false;
    }
    else
    {
        return true;
    }
}

void CServerMessageManager::AddClientMessage(IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    if(NULL != m_pServerMessageTask)
    {
        return m_pServerMessageTask->AddClientMessage(pClientMessage);
    }
}

void CServerMessageManager::DelClientMessage(IClientMessage* pClientMessage)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    if(NULL != m_pServerMessageTask)
    {
        return m_pServerMessageTask->DelClientMessage(pClientMessage);
    }
}
