#include "ProServerManager.h"

CProServerManager::CProServerManager(void)
{
}

CProServerManager::~CProServerManager(void)
{
}

bool CProServerManager::Init()
{
	int nServerPortCount    = App_MainConfig::instance()->GetServerPortCount();
	int nUDPServerPortCount = App_MainConfig::instance()->GetUDPServerPortCount();
	int nReactorCount       = App_MainConfig::instance()->GetReactorCount();

	bool blState = false;

	//初始化禁止IP列表
	App_ForbiddenIP::instance()->Init(FORBIDDENIP_FILE);

	//初始化连接器
	if(!m_ConnectAcceptorManager.InitConnectAcceptor(nServerPortCount))
	{
		OUR_DEBUG((LM_INFO, "[CProServerManager::Init]%s.\n", m_ConnectAcceptorManager.GetError()));
		return false;
	}

	//初始化反应器
	for(int i = 0 ; i < nReactorCount; i++)
	{
		OUR_DEBUG((LM_INFO, "[CProServerManager::Init()]... i=[%d].\n",i));
		if(i == 0)
		{
			//这里区分操作系统版本，使用不同的反应器
			if(App_MainConfig::instance()->GetNetworkMode() == NETWORKMODE_PRO_IOCP)
			{
				blState = App_ProactorManager::instance()->AddNewProactor(REACTOR_CLIENTDEFINE, Proactor_WIN32, 0);
				OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AddNewProactor NETWORKMODE = Proactor_WIN32.\n"));
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AddNewProactor NETWORKMODE Error.\n"));
				return false;
			}

			//linux下的posix，暂时注释，因为linux对posix支持不完整，所以这种模式暂时不考虑。
			//blState = App_ProactorManager::instance()->AddNewProactor(REACTOR_CLIENTDEFINE, Proactor_POSIX, 0);
			//OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AddNewProactor REACTOR_CLIENTDEFINE = Proactor_POSIX.\n"));			
			if(!blState)
			{
				OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AddNewProactor NETWORKMODE Error.\n"));
				return false;
			}
		}
		else
		{
			if(App_MainConfig::instance()->GetNetworkMode() == NETWORKMODE_PRO_IOCP)
			{
				blState = App_ProactorManager::instance()->AddNewProactor(i, Proactor_WIN32, 1);
				OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AddNewProactor NETWORKMODE = Proactor_WIN32.\n"));
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AddNewProactor NETWORKMODE Error.\n"));
				return false;
			}

			if(!blState)
			{
				OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AddNewProactor [%d] Error.\n", i));
				return false;
			}
		}
	}

	//初始化日志系统线程
	CFileLogger* pFileLogger = new CFileLogger();
	if(NULL == pFileLogger)
	{
		OUR_DEBUG((LM_INFO, "[CProServerManager::Init]pFileLogger new is NULL.\n"));
		return false;
	}

	pFileLogger->Init();
	AppLogManager::instance()->Init();
	if(0 != AppLogManager::instance()->RegisterLog(pFileLogger))
	{
		OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AppLogManager::instance()->RegisterLog error.\n"));
		return false;
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AppLogManager is OK.\n"));
	}

	//初始化防攻击系统
	App_IPAccount::instance()->Init(App_MainConfig::instance()->GetValid(), App_MainConfig::instance()->GetValidConnectCount(), App_MainConfig::instance()->GetTrackIPCount());

	//初始化BuffPacket缓冲池
	App_BuffPacketManager::instance()->Init(BUFFPACKET_MAX_COUNT);

	//初始化PacketParse对象池
	App_PacketParsePool::instance()->Init(MAX_PACKET_PARSE);

	//初始化MakePacket对象池
	App_MakePacket::instance()->Init();

	//初始化Message对象池
	App_MessagePool::instance()->Init(MAX_MESSAGE_POOL);

	//初始化ProConnectHandler对象池
	if(App_MainConfig::instance()->GetHandleCount() <= 0)
	{
		App_ProConnectHandlerPool::instance()->Init(MAX_HANDLE_POOL);
	}
	else
	{
		App_ProConnectHandlerPool::instance()->Init(App_MainConfig::instance()->GetHandleCount());
	}

	//初始化统计模块功能
	App_CommandAccount::instance()->Init(App_MainConfig::instance()->GetCommandAccount(), App_MainConfig::instance()->GetCommandFlow(), App_MainConfig::instance()->GetPacketTimeOut());
	
	//初始化链接管理器
	App_ProConnectManager::instance()->Init(App_MainConfig::instance()->GetSendQueueCount());

	//初始化消息处理线程
	App_MessageServiceGroup::instance()->Init(App_MainConfig::instance()->GetThreadCount(), App_MainConfig::instance()->GetMsgMaxQueue(), App_MainConfig::instance()->GetMgsHighMark(), App_MainConfig::instance()->GetMsgLowMark());

	//初始化给DLL的对象接口
	App_ServerObject::instance()->SetMessageManager((IMessageManager* )App_MessageManager::instance());
	App_ServerObject::instance()->SetLogManager((ILogManager*  )AppLogManager::instance());
	App_ServerObject::instance()->SetConnectManager((IConnectManager* )App_ProConnectManager::instance());
	App_ServerObject::instance()->SetPacketManager((IPacketManager* )App_BuffPacketManager::instance());
	App_ServerObject::instance()->SetClientManager((IClientManager* )App_ClientProConnectManager::instance());
	App_ServerObject::instance()->SetUDPConnectManager((IUDPConnectManager* )App_ProUDPManager::instance());
	App_ServerObject::instance()->SetTimerManager((ActiveTimer* )App_TimerManager::instance());
	App_ServerObject::instance()->SetModuleMessageManager((IModuleMessageManager* )App_ModuleMessageManager::instance());

	return true;
}

bool CProServerManager::Start()
{
	//注册信号量
	//if(0 != App_SigHandler::instance()->RegisterSignal(NULL))
	//{
	//	return false;
	//}

	//启动TCP监听
	int nServerPortCount = App_MainConfig::instance()->GetServerPortCount();
	bool blState = false;

	//初始化监听远程连接
	for(int i = 0 ; i < nServerPortCount; i++)
	{
		ACE_INET_Addr listenAddr;

		_ServerInfo* pServerInfo = App_MainConfig::instance()->GetServerPort(i);
		if(NULL == pServerInfo)
		{
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start]pServerInfo [%d] is NULL.\n", i));
			return false;
		}

		//判断IPv4还是IPv6
		int nErr = 0;
		if(pServerInfo->m_u1IPType == TYPE_IPV4)
		{
			nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP);
		}
		else
		{
			nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP, 1, PF_INET6);
		}
		if(nErr != 0)
		{
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start](%d)set_address error[%d].\n", i, errno));
			return false;
		}

		//得到接收器
		ProConnectAcceptor* pConnectAcceptor = m_ConnectAcceptorManager.GetConnectAcceptor(i);

		if(NULL == pConnectAcceptor)
		{
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start]pConnectAcceptor[%d] is NULL.\n", i));
			return false;
		}

		ACE_Proactor* pProactor = App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE);
		if(NULL == pProactor)
		{
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE) is NULL.\n"));
			return false;
		}

		int nRet = pConnectAcceptor->open(listenAddr, 0, 1, App_MainConfig::instance()->GetBacklog(), 1, pProactor);
		if(-1 == nRet)
		{
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start] pConnectAcceptor->open[%d] is error.\n", i));
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start] Listen from [%s:%d] error(%d).\n",listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
			return false;
		}
		OUR_DEBUG((LM_INFO, "[CProServerManager::Start] Listen from [%s:%d] OK.\n", listenAddr.get_host_addr(), listenAddr.get_port_number()));
	}

	//启动UDP监听
	int nUDPServerPortCount = App_MainConfig::instance()->GetUDPServerPortCount();
	for(int i = 0 ; i < nUDPServerPortCount; i++)
	{
		ACE_INET_Addr listenAddr;

		_ServerInfo* pServerInfo = App_MainConfig::instance()->GetUDPServerPort(i);
		if(NULL == pServerInfo)
		{
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start]UDP pServerInfo [%d] is NULL.\n", i));
			return false;
		}


		CProactorUDPHandler* pProactorUDPHandler = App_ProUDPManager::instance()->Create();
		if(NULL == pProactorUDPHandler)
		{
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start] pProactorUDPHandler is NULL[%d] is error.\n", i));
			return false;
		}
		else
		{
			int nErr = 0;
			if(pServerInfo->m_u1IPType == TYPE_IPV4)
			{
				nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP);
			}
			else
			{
				nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP, 1, PF_INET6);
			}
			if(nErr != 0)
			{
				OUR_DEBUG((LM_INFO, "[CProServerManager::Start](%d)UDP set_address error[%d].\n", i, errno));
				return false;
			}

			ACE_Proactor* pProactor = App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE);
			if(NULL == pProactor)
			{
				OUR_DEBUG((LM_INFO, "[CProServerManager::Start]UDP App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE) is NULL.\n"));
				return false;
			}

			if(0 != pProactorUDPHandler->OpenAddress(listenAddr, pProactor))
			{
				OUR_DEBUG((LM_INFO, "[CProServerManager::Start] UDP Listen from [%s:%d] error(%d).\n",listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
				return false;
			}

			OUR_DEBUG((LM_INFO, "[CProServerManager::Start] UDP Listen from [%s:%d] OK.\n", listenAddr.get_host_addr(), listenAddr.get_port_number()));
		}
	}

	//启动后台管理端口监听
	if(App_MainConfig::instance()->GetConsoleSupport() == CONSOLE_ENABLE)
	{
		ACE_INET_Addr listenConsoleAddr;

		int nErr = 0;
		if(App_MainConfig::instance()->GetConsoleIPType() == TYPE_IPV4)
		{
			nErr = listenConsoleAddr.set(App_MainConfig::instance()->GetConsolePort(), App_MainConfig::instance()->GetConsoleIP());
		}
		else
		{
			nErr = listenConsoleAddr.set(App_MainConfig::instance()->GetConsolePort(), App_MainConfig::instance()->GetConsoleIP(), 1, PF_INET6);
		}
		if(nErr != 0)
		{
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start]listenConsoleAddr set_address error[%d].\n", errno));
			return false;
		}

		ACE_Proactor* pProactor = App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE);
		if(NULL == pProactor)
		{
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE) is NULL.\n"));
			return false;
		}

		int nRet = m_ProConsoleConnectAcceptor.open(listenConsoleAddr, 0, 1, MAX_ASYNCH_BACKLOG, 1, pProactor, true);
		if(-1 == nRet)
		{
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start] m_ProConsoleConnectAcceptor.open is error.\n"));
			OUR_DEBUG((LM_INFO, "[CProServerManager::Start] Listen from [%s:%d] error(%d).\n",listenConsoleAddr.get_host_addr(), listenConsoleAddr.get_port_number(), errno));
			return false;
		}
	}

	//启动日志服务线程
	if(0 != AppLogManager::instance()->Start())
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM, "[CProServerManager::Init]AppLogManager is ERROR.");
	}
	else
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM, "[CProServerManager::Init]AppLogManager is OK.");
	}
	
	/*
	//设置定时器策略(使用高精度定时器)
	(void) ACE_High_Res_Timer::global_scale_factor ();
	auto_ptr<ACE_Timer_Heap_Variable_Time_Source> tq(new ACE_Timer_Heap_Variable_Time_Source);
	tq->set_time_policy(&ACE_High_Res_Timer::gettimeofday_hr);
	App_TimerManager::instance()->timer_queue(tq);
	tq.release();
	*/

	//启动定时器
	if(0 != App_TimerManager::instance()->activate())
	{
		OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_TimerManager::instance()->Start() is error.\n"));
		return false;
	}

	//先启动其他的Proactor，最后启动原始的Proactor，因为原始的会挂起线程，所以最后启动一下。
	if(!App_ProactorManager::instance()->StartProactor())
	{
		OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_ProactorManager::instance()->StartProactor is error.\n"));
		return false;
	}

	//启动中间服务器链接管理器
	App_ClientProConnectManager::instance()->Init(App_ProactorManager::instance()->GetAce_Proactor(REACTOR_POSTDEFINE));
	App_ClientProConnectManager::instance()->StartConnectTask(App_MainConfig::instance()->GetConnectServerCheck());

	//初始化模块加载，因为这里可能包含了中间服务器连接加载
	blState = App_ModuleLoader::instance()->LoadModule(App_MainConfig::instance()->GetModulePath(), App_MainConfig::instance()->GetModuleString());
	if(false == blState)
	{
		OUR_DEBUG((LM_INFO, "[CProServerManager::Start]LoadModule is error.\n"));
		return false;
	}

	//开始消息处理线程
	App_MessageServiceGroup::instance()->Start();

	//开始启动链接发送定时器
	App_ProConnectManager::instance()->StartTimer();

	//最后启动反应器
	OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_ProactorManager::instance()->StartProactorDefault begin....\n"));
	if(!App_ProactorManager::instance()->StartProactorDefault())
	{
		OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_ProactorManager::instance()->StartProactorDefault is error.\n"));
		return false;
	}

	return true;
}

bool CProServerManager::Close()
{
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close begin....\n"));
	m_ConnectAcceptorManager.Close();
	m_ProConsoleConnectAcceptor.cancel();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_TimerManager OK.\n"));
	App_TimerManager::instance()->deactivate();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ReUDPManager OK.\n"));
	App_ClientProConnectManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ClientReConnectManager OK.\n"));
	App_ProUDPManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ModuleLoader OK.\n"));
	App_ModuleLoader::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_MessageManager OK.\n"));
	App_MessageManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ConnectManager OK.\n"));
	App_MessageServiceGroup::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_MessageServiceGroup OK.\n"));
	App_ProConnectManager::instance()->CloseAll();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ConnectManager OK.\n"));
	App_CommandAccount::instance()->SaveCommandDataLog();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Save App_CommandAccount OK.\n"));
	AppLogManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]AppLogManager OK\n"));
	App_BuffPacketManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]BuffPacketManager OK\n"));
	App_ProactorManager::instance()->StopProactor();
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ReactorManager OK.\n"));	
	OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close end....\n"));

	return true;
}
