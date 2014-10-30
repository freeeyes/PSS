#include "ace/ace.h"
#include "ace/OS_main.h"
#include "MainConfig.h"
#ifdef WIN32
#include "ClientProConnectManager.h"
#else
#include "ClientReConnectManager.h"
#endif
#include "MonitorFSMManager.h"


CMonitorFSMManager g_MonitorFSMManager;

bool Init()
{
	//初始化代码
	App_MainConfig::instance()->Init(MAINCONFIG);

	App_MainConfig::instance()->Display();
#ifdef WIN32
	bool blState = App_ProactorManager::instance()->AddNewProactor(REACTOR_CLIENTDEFINE, Proactor_WIN32, 1);
	if(!blState)
	{
		OUR_DEBUG((LM_INFO, "[Init]AddNewProactor [%d] Error.\n", REACTOR_CLIENTDEFINE));
		return false;
	}

	//定时重练检测
	int nReConnectTime = 60;
	App_ClientProConnectManager::instance()->Init(App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE));
	App_ClientProConnectManager::instance()->StartConnectTask(nReConnectTime);
#else
	bool blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_DEV_POLL, 1, 1000);
	if(!blState)
	{
		OUR_DEBUG((LM_INFO, "[Init]AddNewReactor [%d] Error.\n", REACTOR_CLIENTDEFINE));
		return false;
	}

	//定时重练检测
	int nReConnectTime = 60;
	App_ClientReConnectManager::instance()->Init(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));
	App_ClientReConnectManager::instance()->StartConnectTask(nReConnectTime);
#endif

	OUR_DEBUG((LM_INFO, "[Init]Init OK.\n"));
	return true;
}

bool Start()
{
	for(uint32 i = 0; i < App_MainConfig::instance()->GerServerInfoCount(); i++)
	{
		_ServerInfo* pServerInfo = App_MainConfig::instance()->GetServerInfo(i);
		if(NULL != pServerInfo)
		{
			MonitorFSM* pMonitorFSM = g_MonitorFSMManager.Create(pServerInfo->m_szServerIP, 
				pServerInfo->m_nPort,
				pServerInfo->m_szKey);

			if(NULL == pMonitorFSM)
			{
				OUR_DEBUG((LM_INFO, "[Start]pMonitorFSM new fail.\n"));
				return false;
			}

#ifdef WIN32
			App_ClientProConnectManager::instance()->Connect(i, 
				pServerInfo->m_szServerIP,
				pServerInfo->m_nPort,
				pServerInfo->m_u1IPType,
				pMonitorFSM->GetClientMessage());
#else
			App_ClientReConnectManager::instance()->Connect(i, 
				pServerInfo->m_szServerIP,
				pServerInfo->m_nPort,
				pServerInfo->m_u1IPType,
				pMonitorFSM->GetClientMessage());
#endif

			ACE_Time_Value tvSleep(0, 10000);
			ACE_OS::sleep(tvSleep);
		}
	}

#ifdef WIN32
	//启动主反应器
	App_ProactorManager::instance()->StartProactor();
#else
	App_ReactorManager::instance()->StartReactor();
#endif

	ACE_Time_Value tvSleep(0, 10000);
	ACE_OS::sleep(tvSleep);

	//注册定时器
	ACE_Time_Value tvInterval(120, 0);

	App_TimerManager::instance()->schedule(&g_MonitorFSMManager, 
		NULL, 
		ACE_OS::gettimeofday(), ACE_OS::gettimeofday() + tvInterval);

	//启动定时器
	App_TimerManager::instance()->activate();

	OUR_DEBUG((LM_INFO, "[Init]Start OK.\n"));
	return true;
}

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
	Init();

	Start();

	ACE_Thread_Manager::instance()->wait();
	getchar();

	return 0;
}