#include "ServerMessageTask.h"

CServerMessageInfoPool::CServerMessageInfoPool()
{
}

void CServerMessageInfoPool::Init(uint32 u4PacketCount /*= MAX_SERVER_MESSAGE_INFO_COUNT*/)
{
    Close();

    m_objServerMessageList.Init((int)u4PacketCount);
    m_objArrayList.Init(u4PacketCount);

    for(uint32 i = 0; i < u4PacketCount; i++)
    {
        _Server_Message_Info* pPacket = m_objArrayList.GetObject(i);

        if(nullptr != pPacket)
        {
            //��ӵ�Hash��������
            std::stringstream ss_format;
            ss_format << i;
            string strHashID = ss_format.str();

            int nHashPos = m_objServerMessageList.Add_Hash_Data(strHashID.c_str(), pPacket);

            if(-1 != nHashPos)
            {
                pPacket->SetHashID(i);
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

    //��Hash���е���һ����ʹ�õ�����
    _Server_Message_Info* pMessage = m_objServerMessageList.Pop();

    //û�ҵ������
    return pMessage;
}

bool CServerMessageInfoPool::Delete(_Server_Message_Info* pObject)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if(nullptr == pObject)
    {
        return false;
    }

    std::stringstream ss_format;
    ss_format << pObject->GetHashID();
    string strHashID = ss_format.str();

    bool blState = m_objServerMessageList.Push(strHashID.c_str(), pObject);

    if(false == blState)
    {
        OUR_DEBUG((LM_INFO, "[CServerMessageInfoPool::Delete]szHandlerID=%s(0x%08x).\n", strHashID.c_str(), pObject));
    }

    return true;
}


void CServerMessageInfoPool::Close()
{
    //���������Ѵ��ڵ�ָ��
    OUR_DEBUG((LM_INFO, "[CMessagePool::Close]Begin.\n"));
    m_objServerMessageList.Close();

    for (uint32 i = 0; i < m_objArrayList.GetCount(); i++)
    {
        m_objArrayList.GetObject(i)->Close();
    }

    OUR_DEBUG((LM_INFO, "[CMessagePool::Close]End.\n"));
}

CServerMessageTask::CServerMessageTask() : m_cond(m_mutex)
{
}

bool CServerMessageTask::Start()
{
    if(0 != open())
    {
        return false;
    }

    return true;
}

int CServerMessageTask::handle_signal (int signum,siginfo_t* siginfo,ucontext_t* ucontext)
{
    if (signum == SIGUSR1 + grp_id())
    {
        OUR_DEBUG((LM_INFO,"[CServerMessageTask::handle_signal](%d) will be kill.\n", grp_id()));

        if(nullptr != siginfo && nullptr != ucontext)
        {
            OUR_DEBUG((LM_INFO,"[CServerMessageTask::handle_signal]siginfo is not nullptr.\n"));
        }

        ACE_Thread::exit();
    }

    return 0;
}

int CServerMessageTask::open()
{
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
    if (m_blRun)
    {
        OUR_DEBUG((LM_INFO, "[CServerMessageTask::Close]Begin.\n"));

        if (-1 == this->CloseMsgQueue())
        {
            OUR_DEBUG((LM_INFO, "[CServerMessageTask::Close]CloseMsgQueue error.\n"));
        }

        OUR_DEBUG((LM_INFO, "[CServerMessageTask::Close]End.\n"));

        //�ȴ��߳��ͷ�
        ACE_Time_Value tvSleep(0, 1000);
        ACE_OS::sleep(tvSleep);
    }
    else
    {
        msg_queue()->deactivate();
        msg_queue()->flush();

        App_ServerMessageInfoPool::instance()->Close();
    }

    return 0;
}

int CServerMessageTask::svc(void)
{
    while(m_blRun)
    {
        ACE_Message_Block* mb = nullptr;
        ACE_OS::last_error(0);

        if(getq(mb, 0) == -1)
        {
            OUR_DEBUG((LM_ERROR,"[CMessageService::svc] getq error errno = [%d].\n", ACE_OS::last_error()));
            m_blRun = false;
        }
        else
        {
            //���������Ϣ�����߳�
            if (mb->msg_type() == ACE_Message_Block::MB_STOP)
            {
                m_mutex.acquire();
                mb->release();
                this->msg_queue()->deactivate();
                m_cond.signal();
                m_mutex.release();
                m_blRun = false;
                break;
            }

            //����ClientMessage�������
            if (mb->msg_type() == ADD_SERVER_CLIENT)
            {
                IClientMessage* pClientMessage = nullptr;
                memcpy_safe(mb->rd_ptr(), sizeof(IClientMessage*), (char* )&pClientMessage, sizeof(IClientMessage*));

                Add_ValidIClientMessage(pClientMessage);

                App_MessageBlockManager::instance()->Close(mb);
                continue;
            }

            if (mb->msg_type() == DEL_SERVER_CLIENT)
            {
                IClientMessage* pClientMessage = nullptr;
                memcpy_safe(mb->rd_ptr(), sizeof(IClientMessage*), (char*)&pClientMessage, sizeof(IClientMessage*));

                Update_ValidIClientMessage(pClientMessage);

                App_MessageBlockManager::instance()->Close(mb);
                continue;
            }

            _Server_Message_Info* msg = *((_Server_Message_Info**)mb->base());

            if (! msg)
            {
                OUR_DEBUG((LM_ERROR,"[CMessageService::svc] mb msg == nullptr CurrthreadNo=[%d]!\n", m_u4ThreadID));
                continue;
            }

            if (false == this->ProcessMessage(msg, m_u4ThreadID))
            {
                OUR_DEBUG((LM_INFO, "[CServerMessageTask::svc]ProcessMessage error.\n"));
            }

            if (false == App_ServerMessageInfoPool::instance()->Delete(msg))
            {
                OUR_DEBUG((LM_INFO, "[CServerMessageTask::svc]Delete error.\n"));
            }
        }
    }

    App_ServerMessageInfoPool::instance()->Close();
    OUR_DEBUG((LM_INFO,"[CServerMessageTask::svc] svc finish!\n"));
    return 0;
}

uint32 CServerMessageTask::GetThreadID() const
{
    return m_u4ThreadID;
}

bool CServerMessageTask::PutMessage(_Server_Message_Info* pMessage)
{
    ACE_Message_Block* mb = pMessage->GetQueueMessage();

    if(nullptr != mb)
    {
        //�ж϶����Ƿ����Ѿ����
        auto u4QueueCount = (uint32)msg_queue()->message_count();

        if(u4QueueCount >= m_u4MaxQueue)
        {
            OUR_DEBUG((LM_ERROR,"[CServerMessageTask::PutMessage] Queue is Full nQueueCount = [%d].\n", u4QueueCount));
            return false;
        }

        ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, 100000);

        if(this->putq(mb, &xtime) == -1)
        {
            OUR_DEBUG((LM_ERROR,"[CServerMessageTask::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", u4QueueCount, errno));
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

bool CServerMessageTask::PutMessage_Add_Client(IClientMessage* pClientMessage)
{
    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(sizeof(IClientMessage*));

    if (nullptr == pmb)
    {
        OUR_DEBUG((LM_ERROR, "[CServerMessageTask::PutMessage_Add_Client] ACE_Message_Block is nullptr.\n"));
        return false;
    }

    sprintf_safe((char* )&pClientMessage, sizeof(IClientMessage*), pmb->wr_ptr(), sizeof(IClientMessage*));
    pmb->wr_ptr(sizeof(IClientMessage*));
    pmb->msg_type(ADD_SERVER_CLIENT);

    //�ж϶����Ƿ����Ѿ����
    auto u4QueueCount = (uint32)msg_queue()->message_count();

    if (u4QueueCount >= m_u4MaxQueue)
    {
        OUR_DEBUG((LM_ERROR, "[CServerMessageTask::PutMessage_Add_Client] Queue is Full nQueueCount = [%d].\n", u4QueueCount));
        return false;
    }

    ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, 100000);

    if (this->putq(pmb, &xtime) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CServerMessageTask::PutMessage_Add_Client] Queue putq  error nQueueCount = [%d] errno = [%d].\n", u4QueueCount, errno));
        return false;
    }

    return true;
}

bool CServerMessageTask::PutMessage_Del_Client(IClientMessage* pClientMessage)
{
    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(sizeof(IClientMessage*));

    if (nullptr == pmb)
    {
        OUR_DEBUG((LM_ERROR, "[CServerMessageTask::PutMessage_Del_Client] ACE_Message_Block is nullptr.\n"));
        return false;
    }

    sprintf_safe((char*)&pClientMessage, sizeof(IClientMessage*), pmb->wr_ptr(), sizeof(IClientMessage*));
    pmb->wr_ptr(sizeof(IClientMessage*));
    pmb->msg_type(DEL_SERVER_CLIENT);

    //�ж϶����Ƿ����Ѿ����
    auto u4QueueCount = (uint32)msg_queue()->message_count();

    if (u4QueueCount >= m_u4MaxQueue)
    {
        OUR_DEBUG((LM_ERROR, "[CServerMessageTask::PutMessage_Del_Client] Queue is Full nQueueCount = [%d].\n", u4QueueCount));
        return false;
    }

    ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, 100000);

    if (this->putq(pmb, &xtime) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CServerMessageTask::PutMessage_Del_Client] Queue putq  error nQueueCount = [%d] errno = [%d].\n", u4QueueCount, errno));
        return false;
    }

    return true;
}

bool CServerMessageTask::ProcessMessage(const _Server_Message_Info* pMessage, uint32 u4ThreadID)
{
    if(nullptr == pMessage)
    {
        OUR_DEBUG((LM_DEBUG, "[CServerMessageTask::ProcessMessage]u4ThreadID=%d, pMessage is nullptr\n", u4ThreadID));
        return false;
    }

    //��������ݴ���ָ���Ѿ�������Чָ�룬ֱ�ӷ��ء�
    if(CheckValidClientMessage(pMessage->m_pClientMessage) == false)
    {
        OUR_DEBUG((LM_DEBUG, "[CServerMessageTask::ProcessMessage]u4ThreadID=%d, m_pClientMessage is nullptr\n", u4ThreadID));
        return true;
    }

    m_tvDispose = ACE_OS::gettimeofday();

    m_emState   = EM_Server_Recv_State::SERVER_RECV_BEGIN;
    //�������ݰ�����
    pMessage->m_pClientMessage->RecvData(pMessage->m_u2CommandID, pMessage->m_pRecvFinish, pMessage->m_objServerIPInfo);
    //���մ����
    App_MessageBlockManager::instance()->Close(pMessage->m_pRecvFinish);
    m_emState = EM_Server_Recv_State::SERVER_RECV_END;
    return true;
}

bool CServerMessageTask::CheckServerMessageThread(ACE_Time_Value const& tvNow) const
{
    ACE_Time_Value tvIntval(tvNow - m_tvDispose);

    if(m_emState == EM_Server_Recv_State::SERVER_RECV_BEGIN && tvIntval.sec() > MAX_DISPOSE_TIMEOUT)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool CServerMessageTask::CheckValidClientMessage(const IClientMessage* pClientMessage) const
{
    for(const IClientMessage* pVecClientMessage : m_vecValidIClientMessage)
    {
        if(pVecClientMessage == pClientMessage)
        {
            return true;
        }
    }

    return false;
}

int CServerMessageTask::CloseMsgQueue()
{
    return Task_Common_CloseMsgQueue((ACE_Task<ACE_MT_SYNCH>*)this, m_cond, m_mutex);
}

void CServerMessageTask::Add_ValidIClientMessage(IClientMessage* pClientMessage)
{
    bool blIsFind = false;
    if (nullptr == pClientMessage)
    {
        return;
    }

    for (const IClientMessage* pvecClientMessage : m_vecValidIClientMessage)
    {
        if (pvecClientMessage == pClientMessage)
        {
            //�ҵ���
            blIsFind = true;
            break;
        }
    }

    if (false == blIsFind)
    {
        m_vecValidIClientMessage.push_back(pClientMessage);
    }
}

void CServerMessageTask::Update_ValidIClientMessage(const IClientMessage* pClientMessage)
{
    if (nullptr != pClientMessage)
    {
        //�Ȳ�����Ч���б����Ƿ������ָ��
        for (vecValidIClientMessage::iterator b = m_vecValidIClientMessage.begin(); b != m_vecValidIClientMessage.end(); ++b)
        {
            if (*b == pClientMessage)
            {
                //�ҵ��ˣ����
                m_vecValidIClientMessage.erase(b);
                break;
            }
        }
    }

}

//************************************************

CServerMessageManager::CServerMessageManager()
{
    Init();
}

void CServerMessageManager::Init()
{
    if(nullptr == m_pServerMessageTask)
    {
        m_pServerMessageTask = new CServerMessageTask();
    }
}

bool CServerMessageManager::Start()
{
    if(nullptr != m_pServerMessageTask)
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
    if(nullptr != m_pServerMessageTask)
    {
        if (0 != m_pServerMessageTask->Close())
        {
            OUR_DEBUG((LM_DEBUG, "[CServerMessageManager::Close]Close error.\n"));
        }

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
    if(nullptr != m_pServerMessageTask)
    {
        return m_pServerMessageTask->PutMessage(pMessage);
    }
    else
    {
        return false;
    }
}

bool CServerMessageManager::CheckServerMessageThread(ACE_Time_Value const& tvNow) const
{
    if(nullptr != m_pServerMessageTask)
    {
        bool blRet = m_pServerMessageTask->CheckServerMessageThread(tvNow);

        if(false == blRet)
        {
            //�߳�ֹͣ��������ʾ���⡣
            OUR_DEBUG((LM_DEBUG, "[CServerMessageManager::CheckServerMessageThread]***App_ServerMessageTask Thread is DEAD***.\n"));
            return false;
        }

        return true;
    }
    else
    {
        return true;
    }
}

bool CServerMessageManager::AddClientMessage(IClientMessage* pClientMessage)
{
    if(nullptr != m_pServerMessageTask)
    {
        return m_pServerMessageTask->PutMessage_Add_Client(pClientMessage);
    }

    return false;
}

bool CServerMessageManager::DelClientMessage(IClientMessage* pClientMessage)
{
    if(nullptr != m_pServerMessageTask)
    {
        return m_pServerMessageTask->PutMessage_Del_Client(pClientMessage);
    }

    return false;
}
