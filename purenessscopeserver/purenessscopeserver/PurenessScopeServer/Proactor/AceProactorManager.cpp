#include "AceProactorManager.h"

CAceProactor::CAceProactor()
{
	m_pProactor     = NULL;
	m_nProactorType = 0;
	m_nThreadCount  = 0;
	m_szError[0]    = '\0';
	m_blRun         = false;
	m_u4ProactorID  = 0;
}

CAceProactor::~CAceProactor()
{
	Close();
}

void CAceProactor::SetProactorID(uint32 u4ProactorID)
{
	m_u4ProactorID = u4ProactorID;
}

uint32 CAceProactor::GetProactorID()
{
	return m_u4ProactorID;
}

void CAceProactor::Close()
{
	SAFE_DELETE(m_pProactor);

	m_nProactorType = 0;
	m_nThreadCount  = 0;
	m_blRun         = false;
}

bool CAceProactor::Init(int nProactorType, int nThreadCount)
{
	try
	{
		switch (nProactorType)
		{
#ifdef WIN32		
		case Proactor_WIN32:    //这个功能限制于Windows操作的默认反应器，如果是COM服务器可以使用Proactor_WFMO_msg
			{
				int nEventCount = 0;
				if(nThreadCount == 0)
				{
					//得到CPU的个数，监听线程根据CPU数量来创建多个事件回调。
					nEventCount = ACE_OS::num_processors_online() * 2;
				}

				ACE_WIN32_Proactor* pWin32Proactor = new ACE_WIN32_Proactor(nEventCount);
				if(NULL == pWin32Proactor)
				{
					throw "[CAceProactor::Init]New ACE_WIN32_Proactor Error.";
				}

				m_pProactor = new ACE_Proactor(pWin32Proactor, 1);
				if(NULL == m_pProactor)
				{
					throw "[CAceProactor::Init]New m_pProactor Error[ACE_WIN32_Proactor].";
				}

				m_nProactorType = Proactor_WIN32;
				break;
			}
#else			
		case Proactor_POSIX:    //这个类的功能是服务于非Windows的默认反应器
			{
				ACE_POSIX_Proactor* pPosixProactor = new ACE_POSIX_Proactor();
				if(NULL == pPosixProactor)
				{
					throw "[CAceProactor::Init]New ACE_POSIX_Proactor Error.";
				}

				m_pProactor = new ACE_Proactor(pPosixProactor, 1);
				if(NULL == m_pProactor)
				{
					throw "[CAceProactor::Init]New m_pProactor Error[ACE_POSIX_Proactor].";
				}

				m_nProactorType = Proactor_POSIX;
				break;
			}
#endif
		}

		m_nThreadCount = nThreadCount;
		return true;
	}
	catch (const char* szError)
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
		return false;
	}
}

int CAceProactor::open(void* args)
{
	OUR_DEBUG((LM_ERROR, "CAceProactor::Open Begin nProactorID= [%d].\n", m_u4ProactorID));
	if(activate(THR_NEW_LWP | THR_BOUND | THR_DETACHED, m_nThreadCount)  == -1)
	{
		m_blRun = false;
		OUR_DEBUG((LM_ERROR, "[CAceProactor::Open]activate error ProactorType = [%d] nThreadCount = [%d] Start!\n", m_nProactorType, m_nThreadCount));
		return -1;
	}
	else
	{
		m_blRun = true;
		OUR_DEBUG((LM_ERROR, "CAceProactor::Open Begin nProactorID= [%d] OK\n", m_u4ProactorID));
		return 0;
	}
}

int CAceProactor::svc()
{

	if(NULL == m_pProactor)
	{
		OUR_DEBUG((LM_ERROR, "[CAceProactor::Svc]m_pProactor is NULL.\n", m_nProactorType, m_nThreadCount));
		return -1;
	}
	else
	{
		m_blRun = true;
		m_pProactor->proactor_run_event_loop();
		OUR_DEBUG((LM_ERROR, "CAceProactor::Svc] (%P|%t) Begin nProactorID= [%d] end .... \n", m_u4ProactorID));
		return 0;
	}
}

bool CAceProactor::Start()
{
	OUR_DEBUG((LM_INFO, "[CAceProactor::Start] ProactorID = [%d] ProactorType = [%d] nThreadCount = [%d] Start!\n", GetProactorID(), m_nProactorType, m_nThreadCount));
	if(0 == open())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CAceProactor::Stop()
{
	if(NULL == m_pProactor)
	{
		OUR_DEBUG((LM_ERROR, "[CAceProactor::Stop]m_pProactor is NULL.\n", m_nProactorType, m_nThreadCount));
		return false;
	}

	if(m_blRun == true)
	{
		m_pProactor->proactor_end_event_loop();
	}
	m_blRun = false;

	return true;
}

const char* CAceProactor::GetError()
{
	return m_szError;
}

int CAceProactor::GetThreadCount()
{
	return m_nThreadCount;
}

int CAceProactor::GetProactorType()
{
	return m_nProactorType;
}

ACE_Proactor* CAceProactor::GetProactor()
{
	return m_pProactor;
}

CAceProactorManager::CAceProactorManager(void)
{
}

CAceProactorManager::~CAceProactorManager(void)
{
	Close();
}

void CAceProactorManager::Close()
{
	mapAceProactor::iterator b = m_mapAceProactor.begin();
	mapAceProactor::iterator e = m_mapAceProactor.end();

	for(b; b!= e; b++)
	{
		CAceProactor* pAceProactor = (CAceProactor* )b->second;
		SAFE_DELETE(pAceProactor);
	}

	m_mapAceProactor.clear();
}

const char* CAceProactorManager::GetError()
{
	return m_szError;
}

bool CAceProactorManager::AddNewProactor(int nProactorID, int nProactorType, int nThreadCount)
{
	CAceProactor* pAceProactor = new CAceProactor();
	if(NULL == pAceProactor)
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CAceProactorManager::AddNewProactor]New CAceProactor is NULL.");
		return false;
	}

	pAceProactor->SetProactorID((uint32)nProactorID);
	bool blState = pAceProactor->Init(nProactorType, nThreadCount);
	if(!blState)
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "%s", pAceProactor->GetError());
		SAFE_DELETE(pAceProactor);
		return false;
	}

	mapAceProactor::iterator f = m_mapAceProactor.find(nProactorID);
	if(f != m_mapAceProactor.end())
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CAceProactorManager::AddNewProactor]CAceProactor is exist[%d].", nProactorID);
		SAFE_DELETE(pAceProactor);
		return false;
	}

	m_mapAceProactor.insert(mapAceProactor::value_type(nProactorID, pAceProactor));
	OUR_DEBUG((LM_INFO, "[CAceProactorManager::AddNewProactor]New [%d] ProactorType = [%d] nThreadCount = [%d]. pAceProactor=[%@]\n", nProactorID, nProactorType, nThreadCount,pAceProactor));
	return true;
}

bool CAceProactorManager::StartProactor()
{
	mapAceProactor::iterator b = m_mapAceProactor.begin();
	mapAceProactor::iterator e = m_mapAceProactor.end();

	//先启动非总的Rector
	for(b; b!= e; b++)
	{
		CAceProactor* pAceProactor = (CAceProactor* )b->second;
		if(NULL != pAceProactor)
		{
			pAceProactor->Start();
		}
	}

	return true;
}

bool CAceProactorManager::StartProactorDefault()
{
	OUR_DEBUG((LM_INFO, "[CAceProactorManager::StartProactorDefault]..1. .\n"));
	//启动默认的 Proactor
	mapAceProactor::iterator f = m_mapAceProactor.find(REACTOR_CLIENTDEFINE);

	if(f != m_mapAceProactor.end())
	{
		CAceProactor* pAceProactor = (CAceProactor* )f->second;
		if(NULL != pAceProactor)
		{
			pAceProactor->Start();
		}
		OUR_DEBUG((LM_INFO, "[CAceProactorManager::StartProactorDefault]... 2.\n"));
	}

	return true;
}

bool CAceProactorManager::StopProactor()
{
	mapAceProactor::iterator b = m_mapAceProactor.begin();
	mapAceProactor::iterator e = m_mapAceProactor.end();

	for(b; b!= e; b++)
	{
		int nProactorID           = (int)b->first;
		CAceProactor* pAceProactor = (CAceProactor* )b->second;
		if(NULL != pAceProactor)
		{
			pAceProactor->Stop();
			//pAceProactor->wait();
		}
	}

	return true;
}

CAceProactor* CAceProactorManager::GetAceProactor(int nProactorID)
{
	mapAceProactor::iterator f = m_mapAceProactor.find(nProactorID);
	if(f != m_mapAceProactor.end())
	{
		return (CAceProactor* )f->second;
	}
	else
	{
		return NULL;
	}
}

ACE_Proactor* CAceProactorManager::GetAce_Proactor(int nProactorID)
{
	mapAceProactor::iterator f = m_mapAceProactor.find(nProactorID);
	if(f != m_mapAceProactor.end())
	{
		CAceProactor* pAceProactor = (CAceProactor* )f->second;
		if(NULL != pAceProactor)
		{
			//OUR_DEBUG((LM_INFO, "CAceProactorManager::GetAce_Proactor id=[%d] pAceProactor=[0x%@]\n",nProactorID, pAceProactor));
			return pAceProactor->GetProactor();
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}
