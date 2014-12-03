#include "ServerManager.h"

#include "Frame_Logging_Strategy.h"

CServerManager::CServerManager(void)
{
	m_pFileLogger           = NULL;
	m_pFrameLoggingStrategy = NULL;
}

CServerManager::~CServerManager(void)
{
	OUR_DEBUG((LM_INFO, "[CServerManager::~CServerManager].\n"));
}

bool CServerManager::Init()
{

	if(App_MainConfig::instance()->GetDebugTrunOn() == 1)
	{
		m_pFrameLoggingStrategy = new Frame_Logging_Strategy();

		//是否打开ACE_DEBUG文件存储
		Logging_Config_Param objParam;

		sprintf_safe(objParam.m_strLogFile, 256, "%s", App_MainConfig::instance()->GetDebugFileName());
		objParam.m_iChkInterval    = App_MainConfig::instance()->GetChkInterval();
		objParam.m_iLogFileMaxCnt  = App_MainConfig::instance()->GetLogFileMaxCnt();
		objParam.m_iLogFileMaxSize = App_MainConfig::instance()->GetLogFileMaxSize();
		sprintf_safe(objParam.m_strLogLevel, 128, "%s", App_MainConfig::instance()->GetDebugLevel());

		m_pFrameLoggingStrategy->InitLogStrategy(objParam);
	}

	int nServerPortCount    = App_MainConfig::instance()->GetServerPortCount();
	int nReactorCount       = App_MainConfig::instance()->GetReactorCount();
	bool blState = false;
	//初始化禁止IP列表
	App_ForbiddenIP::instance()->Init(FORBIDDENIP_FILE);

	//初始化连接器
	if (!App_ConnectAcceptorManager::instance()->InitConnectAcceptor(nServerPortCount))
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Init]%s.\n", App_ConnectAcceptorManager::instance()->GetError()));
		return false;
	}

	OUR_DEBUG((LM_INFO, "[CServerManager::Init]nReactorCount=%d.\n", nReactorCount));

	//初始化反应器
	for (int i = 0 ; i < nReactorCount; i++)
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Init()]... i=[%d].\n", i));

		if (App_MainConfig::instance()->GetNetworkMode() == NETWORKMODE_RE_SELECT)
		{
			blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_Select, 1);
			OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_Select.\n"));
		}
		else if (App_MainConfig::instance()->GetNetworkMode() == NETWORKMODE_RE_TPSELECT)
		{
			blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_TP, 1);
			OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_TP.\n"));
		}
		else if (App_MainConfig::instance()->GetNetworkMode() == NETWORKMODE_RE_EPOLL)
		{
			blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_DEV_POLL, 1, App_MainConfig::instance()->GetMaxHandlerCount());
			OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL.\n"));
		}
		else if (App_MainConfig::instance()->GetNetworkMode() == NETWORKMODE_RE_EPOLL_ET)
		{
			blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_DEV_POLL_ET, 1, App_MainConfig::instance()->GetMaxHandlerCount());
			OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL_ET.\n"));
		}
		else
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewProactor NETWORKMODE Error.\n"));
			return false;
		}

		if (!blState)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Init]AddNewReactor [%d] Error.\n", i));
			OUR_DEBUG((LM_INFO, "[CServerManager::Init]Error=%s.\n", App_ReactorManager::instance()->GetError()));
			return false;
		}
	}

	//初始化日志系统线程
	m_pFileLogger = new CFileLogger();

	if (NULL == m_pFileLogger)
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Init]pFileLogger new is NULL.\n"));
		return false;
	}

	m_pFileLogger->Init();
	AppLogManager::instance()->Init();

	if (0 != AppLogManager::instance()->RegisterLog(m_pFileLogger))
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Init]AppLogManager::instance()->RegisterLog error.\n"));
		return false;
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Init]AppLogManager is OK.\n"));
	}

	//初始化防攻击系统
	App_IPAccount::instance()->Init(App_MainConfig::instance()->GetIPAlert()->m_u4IPMaxCount, App_MainConfig::instance()->GetTrackIPCount());

	App_ConnectAccount::instance()->Init(App_MainConfig::instance()->GetConnectAlert()->m_u4ConnectMin,
		App_MainConfig::instance()->GetConnectAlert()->m_u4ConnectMax,
		App_MainConfig::instance()->GetConnectAlert()->m_u4DisConnectMin,
		App_MainConfig::instance()->GetConnectAlert()->m_u4DisConnectMax);	

	//初始化BuffPacket缓冲池.默认都是当前最大连接数的2倍
	App_BuffPacketManager::instance()->Init(BUFFPACKET_MAX_COUNT, App_MainConfig::instance()->GetByteOrder());

	//初始化PacketParse对象池
	App_PacketParsePool::instance()->Init(MAX_PACKET_PARSE);

	//初始化MakePacket对象池
	App_MakePacket::instance()->Init();

	//初始化Message对象池
	App_MessagePool::instance()->Init(MAX_MESSAGE_POOL);

	//初始化ConnectHandler对象池
	if (App_MainConfig::instance()->GetHandleCount() <= 0)
	{
		App_ConnectHandlerPool::instance()->Init(MAX_HANDLE_POOL);
	}
	else
	{
		App_ConnectHandlerPool::instance()->Init(App_MainConfig::instance()->GetHandleCount());
	}

	//初始化链接管理器
	App_ConnectManager::instance()->Init(App_MainConfig::instance()->GetSendQueueCount());
	//初始化消息处理线程
	App_MessageServiceGroup::instance()->Init(App_MainConfig::instance()->GetThreadCount(), App_MainConfig::instance()->GetMsgMaxQueue(), App_MainConfig::instance()->GetMgsHighMark(), App_MainConfig::instance()->GetMsgLowMark());
	//初始化给DLL的对象接口
	App_ServerObject::instance()->SetMessageManager((IMessageManager*)App_MessageManager::instance());
	App_ServerObject::instance()->SetLogManager((ILogManager*)AppLogManager::instance());
	App_ServerObject::instance()->SetConnectManager((IConnectManager*)App_ConnectManager::instance());
	App_ServerObject::instance()->SetPacketManager((IPacketManager*)App_BuffPacketManager::instance());
	App_ServerObject::instance()->SetClientManager((IClientManager*)App_ClientReConnectManager::instance());
	App_ServerObject::instance()->SetUDPConnectManager((IUDPConnectManager*)App_ReUDPManager::instance());
	App_ServerObject::instance()->SetTimerManager((ActiveTimer*)App_TimerManager::instance());
	App_ServerObject::instance()->SetModuleMessageManager((IModuleMessageManager*)App_ModuleMessageManager::instance());
	App_ServerObject::instance()->SetControlListen((IControlListen*)App_ControlListen::instance());
	App_ServerObject::instance()->SetModuleInfo((IModuleInfo* )App_ModuleLoader::instance());

	return true;
}

bool CServerManager::Start()
{
	//启动TCP监听
	int nServerPortCount = App_MainConfig::instance()->GetServerPortCount();

	for (int i = 0 ; i < nServerPortCount; i++)
	{
		ACE_INET_Addr listenAddr;
		_ServerInfo* pServerInfo = App_MainConfig::instance()->GetServerPort(i);

		if (NULL == pServerInfo)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]pServerInfo [%d] is NULL.\n", i));
			return false;
		}

		//判断IPv4还是IPv6
		int nErr = 0;

		if (pServerInfo->m_u1IPType == TYPE_IPV4)
		{
			nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP);
		}
		else
		{
			nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP, 1, PF_INET6);
		}

		if (nErr != 0)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start](%d)set_address error[%d].\n", i, errno));
			return false;
		}

		//得到接收器
		ConnectAcceptor* pConnectAcceptor = App_ConnectAcceptorManager::instance()->GetConnectAcceptor(i);

		if (NULL == pConnectAcceptor)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]pConnectAcceptor[%d] is NULL.\n", i));
			return false;
		}

		int nRet = pConnectAcceptor->open2(listenAddr, App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE), ACE_NONBLOCK, (int)App_MainConfig::instance()->GetBacklog());

		if (-1 == nRet)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start] pConnectAcceptor->open[%d] is error.\n", i));
			OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] error(%d).\n", listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
			return false;
		}

		OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] OK.\n", listenAddr.get_host_addr(), listenAddr.get_port_number()));
	}

	//启动UDP监听
	int nUDPServerPortCount = App_MainConfig::instance()->GetUDPServerPortCount();

	for (int i = 0 ; i < nUDPServerPortCount; i++)
	{
		ACE_INET_Addr listenAddr;
		_ServerInfo* pServerInfo = App_MainConfig::instance()->GetUDPServerPort(i);

		if (NULL == pServerInfo)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]UDP pServerInfo [%d] is NULL.\n", i));
			return false;
		}

		int nErr = 0;

		if (pServerInfo->m_u1IPType == TYPE_IPV4)
		{
			nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP);
		}
		else
		{
			nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP, 1, PF_INET6);
		}

		if (nErr != 0)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]UDP (%d)set_address error[%d].\n", i, errno));
			return false;
		}

		//得到接收器
		CReactorUDPHander* pReactorUDPHandler = App_ReUDPManager::instance()->Create();

		if (NULL == pReactorUDPHandler)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]UDP pReactorUDPHandler[%d] is NULL.\n", i));
			return false;
		}

		int nRet = pReactorUDPHandler->OpenAddress(listenAddr, App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));

		if (-1 == nRet)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start] UDP Listen from [%s:%d] error(%d).\n", listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
			return false;
		}

		OUR_DEBUG((LM_INFO, "[CServerManager::Start] UDP Listen from [%s:%d] OK.\n", listenAddr.get_host_addr(), listenAddr.get_port_number()));
	}

	//启动后台管理端口监听
	if (App_MainConfig::instance()->GetConsoleSupport() == CONSOLE_ENABLE)
	{
		ACE_INET_Addr listenConsoleAddr;
		int nErr = 0;

		if (App_MainConfig::instance()->GetConsoleIPType() == TYPE_IPV4)
		{
			nErr = listenConsoleAddr.set(App_MainConfig::instance()->GetConsolePort(), App_MainConfig::instance()->GetConsoleIP());
		}
		else
		{
			nErr = listenConsoleAddr.set(App_MainConfig::instance()->GetConsolePort(), App_MainConfig::instance()->GetConsoleIP(), 1, PF_INET6);
		}

		if (nErr != 0)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]listenConsoleAddr set_address error[%d].\n", errno));
			return false;
		}

		int nRet = m_ConnectConsoleAcceptor.open(listenConsoleAddr, App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE), ACE_NONBLOCK);

		if (-1 == nRet)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start] pConnectAcceptor->open is error.\n"));
			OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] error(%d).\n", listenConsoleAddr.get_host_addr(), listenConsoleAddr.get_port_number(), errno));
			return false;
		}
	}

	//启动日志服务线程
	if (0 != AppLogManager::instance()->Start())
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM, "[CServerManager::Init]AppLogManager is ERROR.");
	}
	else
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM, "[CServerManager::Init]AppLogManager is OK.");
	}

	//启动定时器
	if (0 != App_TimerManager::instance()->activate())
	{
		OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_TimerManager::instance()->Start() is error.\n"));
		return false;
	}

	//启动中间服务器链接管理器
	App_ClientReConnectManager::instance()->Init(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_POSTDEFINE));
	App_ClientReConnectManager::instance()->StartConnectTask(App_MainConfig::instance()->GetConnectServerCheck());

	//启动所有反应器
	if (!App_ReactorManager::instance()->StartReactor())
	{
		OUR_DEBUG((LM_INFO, "[CServerManager::Start]App_ReactorManager::instance()->StartReactor is error.\n"));
		return false;
	}    

	//初始化模块加载，因为这里可能包含了中间服务器连接加载
	bool blState = false;
	if(ACE_OS::strlen(App_MainConfig::instance()->GetModuleString()) > 0)
	{
		blState = App_ModuleLoader::instance()->LoadModule(App_MainConfig::instance()->GetModulePath(), App_MainConfig::instance()->GetModuleString());
		if (false == blState)
		{
			OUR_DEBUG((LM_INFO, "[CServerManager::Start]LoadModule is error.\n"));
			return false;
		}
	}

	uint16 u2ModuleVCount = App_MainConfig::instance()->GetModuleInfoCount();
	for(uint16 i = 0; i < u2ModuleVCount; i++)
	{
		_ModuleConfig* pModuleConfig = App_MainConfig::instance()->GetModuleInfo(i);
		if(NULL != pModuleConfig)
		{
			blState = App_ModuleLoader::instance()->LoadModule(pModuleConfig->m_szModulePath, 
				pModuleConfig->m_szModuleName, 
				pModuleConfig->m_szModuleParam);

			if(false == blState)
			{
				OUR_DEBUG((LM_INFO, "[CProServerManager::Start]LoadModule (%s)is error.\n", pModuleConfig->m_szModuleName));
				return false;
			}
		}
	}

	//开始消息处理线程
	App_MessageServiceGroup::instance()->Start();
	//开始启动链接发送定时器
	App_ConnectManager::instance()->StartTimer();

	OUR_DEBUG((LM_INFO, "[CServerManager::Start]App_ReactorManager::instance()->StartReactorDefault begin....\n"));

	ACE_Thread_Manager::instance()->wait();

	return true;
}

bool CServerManager::Close()
{
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close begin....\n"));
	App_ConnectAcceptorManager::instance()->Close();
	m_ConnectConsoleAcceptor.close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_TimerManager OK.\n"));
	App_TimerManager::instance()->deactivate();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ReUDPManager OK.\n"));
	App_ReUDPManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ModuleLoader OK.\n"));
	App_ClientReConnectManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ClientReConnectManager OK.\n"));
	App_ModuleLoader::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_MessageManager OK.\n"));
	App_MessageManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ConnectManager OK.\n"));
	App_MessageServiceGroup::instance()->Close();
	OUR_DEBUG((LM_INFO, "[App_MessageServiceGroup::Close]Close App_MessageServiceGroup OK.\n"));
	App_ConnectManager::instance()->CloseAll();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ConnectManager OK.\n"));
	AppLogManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]AppLogManager OK\n"));
	App_BuffPacketManager::instance()->Close();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]BuffPacketManager OK\n"));
	App_ReactorManager::instance()->StopReactor();
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ReactorManager OK.\n"));	
	OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close end....\n"));


	m_pFrameLoggingStrategy->EndLogStrategy();
	SAFE_DELETE(m_pFrameLoggingStrategy);

	return true;
}
