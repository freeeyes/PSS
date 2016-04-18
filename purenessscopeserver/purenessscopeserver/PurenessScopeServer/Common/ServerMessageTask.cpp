#include "ServerMessageTask.h"
#include "ace/Sig_Handler.h"

ACE_Sig_Handler g_ServerMessageTask_Handler;

Mutex_Allocator _msg_server_message_mb_allocator; 

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

	if(activate(THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED | THR_SUSPENDED, MAX_MSG_THREADCOUNT) == -1)
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
	ACE_Message_Block* mb = NULL;

	//稍微休息一下，等一下其他线程再如主循环
	ACE_Time_Value tvSleep(0, MAX_MSG_SENDCHECKTIME*MAX_BUFF_1000);
	ACE_OS::sleep(tvSleep);

	while(IsRun())
	{
		mb = NULL;
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
			mb->release();
			continue;
		}

		this->ProcessMessage(msg, m_u4ThreadID);

		mb->release();
	}

	OUR_DEBUG((LM_INFO,"[CMessageService::svc] svc finish!\n"));
	return 0;
}

uint32 CServerMessageTask::GetThreadID()
{
	return m_u4ThreadID;
}

bool CServerMessageTask::PutMessage(_Server_Message_Info* pMessage)
{
	ACE_Message_Block* mb = NULL;

	ACE_NEW_MALLOC_NORETURN(mb, 
		static_cast<ACE_Message_Block*>( _msg_server_message_mb_allocator.malloc(sizeof(ACE_Message_Block))),
		ACE_Message_Block(sizeof(_Server_Message_Info*), // size
		ACE_Message_Block::MB_DATA, // type
		0,
		0,
		&_msg_server_message_mb_allocator, // allocator_strategy
		0, // locking strategy
		ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, // priority
		ACE_Time_Value::zero,
		ACE_Time_Value::max_time,
		&_msg_server_message_mb_allocator,
		&_msg_server_message_mb_allocator
		));

	if(NULL != mb)
	{
		_Server_Message_Info** ppMessage = (_Server_Message_Info **)mb->base();
		*ppMessage = pMessage;

		//判断队列是否是已经最大
		int nQueueCount = (int)msg_queue()->message_count();
		if(nQueueCount >= (int)m_u4MaxQueue)
		{
			OUR_DEBUG((LM_ERROR,"[CServerMessageTask::PutMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));
			mb->release();
			return false;
		}

		ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, 100000);
		if(this->putq(mb, &xtime) == -1)
		{
			OUR_DEBUG((LM_ERROR,"[CServerMessageTask::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));
			mb->release();
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
	SAFE_DELETE(pMessage);
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
	for(vecValidIClientMessage::iterator b = m_vecValidIClientMessage.begin(); b != m_vecValidIClientMessage.end(); b++)
	{
		if((IClientMessage* )*b == pClientMessage)
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
	SAFE_DELETE(m_pServerMessageTask);
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

		//设计线程事件关联
#ifndef WIN32
		g_ServerMessageTask_Handler.register_handler (SIGUSR1 + m_pServerMessageTask->grp_id(), m_pServerMessageTask);	
#endif

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
		return m_pServerMessageTask->Close();
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
			//int grp_id = m_pServerMessageTask->grp_id(); 
			//int ret = ACE_Thread_Manager::instance()->kill_grp(grp_id, SIGUSR1);
			//int ret = pthread_cancel(m_pServerMessageTask->Get_Thread_ID());  
			int ret = ACE_Thread_Manager::instance()->kill_grp(m_pServerMessageTask->grp_id(), SIGUSR1 + m_pServerMessageTask->grp_id());
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
