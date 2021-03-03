#include "ServerManager.h"

#ifdef _CPPUNIT_TEST
#include "CppUnitMain.h"
#endif

bool CServerManager::Init()
{
    auto nServerPortCount = (int)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();
    int nReactorCount = 3 + GetXmlConfigAttribute(xmlMessage)->Msg_Thread;

    //初始化Console相关参数
    App_ConsoleManager::instance()->Init();

    //初始化模块数组相关参数
    App_MessageManager::instance()->Init(GetXmlConfigAttribute(xmlModuleMangager)->MaxCount,
                                         GetXmlConfigAttribute(xmlCommandAccount)->MaxCommandCount);

    //初始化加载模块的信息
    App_ModuleLoader::instance()->Init(GetXmlConfigAttribute(xmlModuleMangager)->MaxCount);

    //初始化禁止IP列表
    App_ForbiddenIP::instance()->Init(FORBIDDENIP_FILE);

    PSS_LOGGER_DEBUG("[CServerManager::Init]nReactorCount={0}.", nReactorCount);

    //为多进程做准备，针对epoll和epollet初始化不能在这里去做,因为在多进程里epoll_create必须在子进程里去声明
    if (NETWORKMODE::NETWORKMODE_RE_EPOLL != GetXmlConfigAttribute(xmlNetWorkMode)->Mode
        && NETWORKMODE::NETWORKMODE_RE_EPOLL_ET != GetXmlConfigAttribute(xmlNetWorkMode)->Mode)
    {
        //初始化反应器集合
        App_ReactorManager::instance()->Init((uint16)nReactorCount);

        Init_Reactor((uint8)nReactorCount, GetXmlConfigAttribute(xmlNetWorkMode)->Mode);
    }

    //初始化日志系统线程
    if (false == Server_Manager_Common_LogSystem())
    {
        return false;
    }

    //初始化各种对象池
    Server_Manager_Common_Pool();

    //初始化ConnectHandler对象池
    if (GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount <= 0)
    {
        //初始化PacketParse对象池
        App_ConnectHandlerPool::instance()->Init(MAX_HANDLE_POOL);
    }
    else
    {
        //初始化PacketParse对象池
        App_ConnectHandlerPool::instance()->Init(GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount);
    }

    //初始化给插件的对象接口
    auto pConnectManager   = dynamic_cast<IConnectManager*>(App_HandlerManager::instance());
    auto pClientManager    = dynamic_cast<IClientManager*>(App_ClientReConnectManager::instance());
    auto pFrameCommand     = dynamic_cast<IFrameCommand*>(&m_objFrameCommand);
    auto pServerManager    = dynamic_cast<IServerManager*>(this);
    auto pTTyClientManager = dynamic_cast<ITTyClientManager*>(App_ReTTyClientManager::instance());
    auto pControlListen    = dynamic_cast<IControlListen*>(App_ControlListen::instance());

    Server_Manager_Common_IObject(pConnectManager,
                                  pClientManager,
                                  pFrameCommand,
                                  pServerManager,
                                  pTTyClientManager,
                                  pControlListen);

    //初始化模块加载，因为这里可能包含了中间服务器连接加载
    if (false == Server_Manager_Common_Module())
    {
        return false;
    }

    //初始化转发列表
    App_ForwardManager::instance()->Init();


    //让所有的线程拷同步副本
    App_MessageServiceGroup::instance()->CopyMessageManagerList();

    //初始化连接器
    uint32 u4ClientReactorCount = (uint32)nReactorCount - 3;

    if (!App_ConnectAcceptorManager::instance()->InitConnectAcceptor(nServerPortCount, u4ClientReactorCount))
    {
        PSS_LOGGER_DEBUG("[CServerManager::Init]{0}.", App_ConnectAcceptorManager::instance()->GetError());
        return false;
    }

    return true;
}

bool CServerManager::Start()
{
    //启动TCP监听初始化
    bool blRet = false;
    blRet = Start_Tcp_Listen();

    if (false == blRet)
    {
        return blRet;
    }

    //启动后台管理端口监听
    blRet = Start_Console_Tcp_Listen();

    if (false == blRet)
    {
        return blRet;
    }

    if (GetXmlConfigAttribute(xmlMessage)->Msg_Process > 1)
    {
        Multiple_Process_Start();
    }
    else
    {
        if (false == Run())
        {
            PSS_LOGGER_DEBUG("child Run failure.");
            return false;
        }
    }

    return true;
}

bool CServerManager::Init_Reactor(uint8 u1ReactorCount, NETWORKMODE u1NetMode) const
{
    bool blState = true;

    //初始化反应器
    for (uint8 i = 0; i < u1ReactorCount; i++)
    {
        PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]... i=[{0}].", i);

        if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_SELECT)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_Select, 1);
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_Select.");
        }
        else if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_TPSELECT)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_TP, 1);
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_TP.");
        }
        else if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_EPOLL)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_DEV_POLL, 1, GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount);
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL.");
        }
        else if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_EPOLL_ET)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_DEV_POLL_ET, 1, GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount);
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL_ET.");
        }
        else
        {
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewProactor NETWORKMODE Error.");
            return false;
        }

        if (!blState)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewReactor [{0}] Error.", i);
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]Error={0}.", App_ReactorManager::instance()->GetError());
            return false;
        }
    }

    return blState;
}

bool CServerManager::Run()
{
    //启动定时器
    App_TimerManager::instance()->Start();
    PSS_LOGGER_DEBUG("[CServerManager::Run]run timer start.");

    //对应多进程，epoll必须在子进程里进行初始化
    if (NETWORKMODE::NETWORKMODE_RE_EPOLL == GetXmlConfigAttribute(xmlNetWorkMode)->Mode
        || NETWORKMODE::NETWORKMODE_RE_EPOLL_ET == GetXmlConfigAttribute(xmlNetWorkMode)->Mode)
    {
        //初始化反应器集合
        App_ReactorManager::instance()->Init((uint16)(3 + GetXmlConfigAttribute(xmlMessage)->Msg_Thread));

        if (false == Init_Reactor((uint8)(3 + GetXmlConfigAttribute(xmlMessage)->Msg_Thread), GetXmlConfigAttribute(xmlNetWorkMode)->Mode))
        {
            PSS_LOGGER_DEBUG("[CServerManager::Run]Init_Reactor Error.");
            return false;
        }
    }

	//启动UDP监听
	bool blRet = Start_Udp_Listen();

	if (false == blRet)
	{
		return blRet;
	}

    //初始化TTy连接管理器
    App_ReTTyClientManager::instance()->Init(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_POSTDEFINE),
            GetXmlConfigAttribute(xmlTTyClientManagerInfo)->MaxTTyDevCount,
            GetXmlConfigAttribute(xmlTTyClientManagerInfo)->TimeCheck);

    auto u2ServerPortCount = (uint16)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();

    //创建和启动TCP反应器
    for (uint16 i = 0; i < u2ServerPortCount; i++)
    {
        //得到接收器
        auto pConnectAcceptor = App_ConnectAcceptorManager::instance()->GetConnectAcceptor(i);

        //打开监听对应事件
        pConnectAcceptor->Run_Open(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));
    }

    m_ConnectConsoleAcceptor.Run_Open(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));

    //启动日志服务线程
    if (0 != AppLogManager::instance()->Start())
    {
        AppLogManager::instance()->WriteLog_r(LOG_SYSTEM, "[CServerManager::Init]AppLogManager is [error].");
    }
    else
    {
        AppLogManager::instance()->WriteLog_r(LOG_SYSTEM, "[CServerManager::Init]AppLogManager is [ok].");
    }

    //启动服务器间检查线程
    if (false == App_ClientReConnectManager::instance()->StartConnectTask(GetXmlConfigAttribute(xmlConnectServer)->TimeCheck))
    {
        PSS_LOGGER_DEBUG("[CServerManager::Run]StartConnectTask error.");
        return false;
    }

    //启动所有反应器(不是客户端接收的反应器，在这里不能启动)
    if (!App_ReactorManager::instance()->StartOtherReactor())
    {
        PSS_LOGGER_DEBUG("[CServerManager::Run]App_ReactorManager::instance()->StartOtherReactor is error.");
        return false;
    }

	//启动中间服务器链接管理器
	App_ClientReConnectManager::instance()->Init(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_POSTDEFINE));

    //加载所有的插件初始化动作
    if (false == App_ModuleLoader::instance()->InitModule())
    {
        PSS_LOGGER_DEBUG("[CServerManager::Run]App_ModuleLoader::instance()->InitModule() is error.");
        return false;
    }

    //开始消息处理线程
    App_MessageServiceGroup::instance()->Start();

    //开始启动tty相关监听
    auto nReTTyCount = (int)GetXmlConfigAttribute(xmlTTyDrives)->vec.size();

    for (int i = 0; i < nReTTyCount; i++)
    {
        _TTyDevParam objReCom;

        objReCom.m_nBaudRate = (int)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4Baud;
        objReCom.m_uDatabits = (unsigned char)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4DataBits;
        objReCom.m_uStopbits = (unsigned char)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4StopBits;
        objReCom.m_pParitymode = GetXmlConfigAttribute(xmlTTyDrives)->vec[i].strParity.c_str();
        strcpy_safe(GetXmlConfigAttribute(xmlTTyDrives)->vec[i].strPortName.c_str(), objReCom.m_szDevName, 32);

        //添加监听
        App_ReTTyClientManager::instance()->ConnectFrame((uint16)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4TTyID,
                GetXmlConfigAttribute(xmlTTyDrives)->vec[i].strPortName.c_str(),
                objReCom,
                GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4PacketParseID);

    }

    //开启服务器间通讯相关配置信息
    auto nS2SCount = (int)GetXmlConfigAttribute(xmlServer2Server)->vec.size();

    for (int i = 0; i < nS2SCount; i++)
    {
        uint8 u1IPType = TYPE_IPV4;
        Check_IPType(GetXmlConfigAttribute(xmlServer2Server)->vec[i].strServerIP.c_str(), u1IPType);

        //开启服务器2服务器监听
        App_ClientReConnectManager::instance()->ConnectFrame((uint16)GetXmlConfigAttribute(xmlServer2Server)->vec[i].u4ServerID,
                GetXmlConfigAttribute(xmlServer2Server)->vec[i].strServerIP.c_str(),
                GetXmlConfigAttribute(xmlServer2Server)->vec[i].u2ServerPort,
                u1IPType,
                GetXmlConfigAttribute(xmlServer2Server)->vec[i].u4PacketParseID);
    }

    //最后开闸，启动客户端反应器，让客户端数据进来
    if (!App_ReactorManager::instance()->StartClientReactor())
    {
        PSS_LOGGER_DEBUG("[CServerManager::Run]App_ReactorManager::instance()->StartClientReactor is error.");
        return false;
    }

#ifdef _CPPUNIT_TEST
    //运行CppUnit自动化测试
    PSS_LOGGER_DEBUG("[CppUnit]********************************");
    CCppUnitMain objCppUnitMain;
    objCppUnitMain.Run();
    PSS_LOGGER_DEBUG("[CppUnit]********************************");
#endif

    ACE_Thread_Manager::instance()->wait();

    return true;
}

bool CServerManager::Start_Tcp_Listen() const
{
    auto nServerPortCount = (int)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();

    for (int i = 0; i < nServerPortCount; i++)
    {
        ACE_INET_Addr listenAddr;

        if (false == Server_Manager_Common_Addr(GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].ipType,
                                                GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].ip.c_str(),
                                                GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].port,
                                                listenAddr))
        {
            return false;
        }

        //得到接收器
        auto pConnectAcceptor = App_ConnectAcceptorManager::instance()->GetConnectAcceptor(i);

        if (nullptr == pConnectAcceptor)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Start]pConnectAcceptor[{0}] is nullptr.", i);
            return false;
        }

        pConnectAcceptor->SetPacketParseInfoID(GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].packetparseid);
        int nRet = pConnectAcceptor->Init_Open(listenAddr, 0, 1, 1, (int)GetXmlConfigAttribute(xmlNetWorkMode)->BackLog);

        if (-1 == nRet)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Start] pConnectAcceptor->open[{0}] is error.", i);
            PSS_LOGGER_DEBUG("[CServerManager::Start] Listen from [{0}:{1}] error({1}).", listenAddr.get_host_addr(), listenAddr.get_port_number(), errno);
            return false;
        }

        PSS_LOGGER_DEBUG("[CServerManager::Start] Listen from [{0}:{1}] OK.", listenAddr.get_host_addr(), listenAddr.get_port_number());
    }

    return true;
}

bool CServerManager::Start_Udp_Listen()
{
    auto nUDPServerPortCount = (int)GetXmlConfigAttribute(xmlUDPServerIPs)->vec.size();

    for (int i = 0; i < nUDPServerPortCount; i++)
    {
        ACE_INET_Addr listenAddr;

        if (false == Server_Manager_Common_Addr(GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uipType,
                                                GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uip.c_str(),
                                                GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uport,
                                                listenAddr))
        {
            return false;
        }

        //得到接收器
        auto pReactorUDPHandler = std::make_shared<CReactorUDPHander>();

        pReactorUDPHandler->SetPacketParseInfoID(GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uPacketParseID);
        int nRet = pReactorUDPHandler->OpenAddress(listenAddr, App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));

        if (-1 == nRet)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Start] UDP Listen from [{0}:{1}] error({2}).", listenAddr.get_host_addr(), listenAddr.get_port_number(), errno);
            return false;
        }

        m_vecUDPList.emplace_back(pReactorUDPHandler);

        PSS_LOGGER_DEBUG("[CServerManager::Start] UDP Listen from [{0}:{1}] OK.", listenAddr.get_host_addr(), listenAddr.get_port_number());
    }

    return true;
}

bool CServerManager::Start_Console_Tcp_Listen()
{
    if (GetXmlConfigAttribute(xmlConsole)->support == CONSOLE_ENABLE)
    {
        ACE_INET_Addr listenConsoleAddr;
        int nErr = 0;

        if (GetXmlConfigAttribute(xmlConsole)->ipType == TYPE_IPV4)
        {
            if (ACE_OS::strcmp(GetXmlConfigAttribute(xmlConsole)->sip.c_str(), "INADDR_ANY") == 0)
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport, PSS_INADDR_ANY);
            }
            else
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport,
                                             GetXmlConfigAttribute(xmlConsole)->sip.c_str());
            }
        }
        else
        {
            if (ACE_OS::strcmp(GetXmlConfigAttribute(xmlConsole)->sip.c_str(), "INADDR_ANY") == 0)
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport, PSS_INADDR_ANY);
            }
            else
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport,
                                             GetXmlConfigAttribute(xmlConsole)->sip.c_str(), 1, PF_INET6);
            }
        }

        if (nErr != 0)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Start]listenConsoleAddr set_address error[{0}].", errno);
            return false;
        }

        int nRet = m_ConnectConsoleAcceptor.Init_Open(listenConsoleAddr);

        if (-1 == nRet)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Start] pConnectAcceptor->open is error.");
            PSS_LOGGER_DEBUG("[CServerManager::Start] Listen from [{0}:{1}] error({2}).", listenConsoleAddr.get_host_addr(), listenConsoleAddr.get_port_number(), errno);
            return false;
        }
    }

    return true;
}

void CServerManager::Multiple_Process_Start()
{
#ifndef WIN32
    //当前监控子线程个数
    int nNumChlid = GetXmlConfigAttribute(xmlMessage)->Msg_Process;

    //检测时间间隔参数
    //主进程检测时间间隔（设置每隔5秒一次）
    ACE_Time_Value tvMonitorSleep(5, 0);

    //文件锁
    int fd_lock = 0;

    int nRet = 0;

    //获得当前路径
    char szWorkDir[MAX_BUFF_500] = { 0 };

    if (!ACE_OS::getcwd(szWorkDir, sizeof(szWorkDir)))
    {
        exit(1);
    }

    //在Linux下采用多进程的方式启动
    // 打开（创建）锁文件
    string strFileName = fmt::format("{0}/lockwatch.lk", szWorkDir);
    //文件打开权限为  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
    fd_lock = open(strFileName.c_str(), O_RDWR | O_CREAT, S_IRWXU | S_IRWXG);

    if (fd_lock < 0)
    {
        PSS_LOGGER_DEBUG("open the flock and exit, errno = {0}.", errno);
        exit(1);
    }

    //查看当前文件锁是否已锁
    nRet = SeeLock(fd_lock, 0, sizeof(int));

    if (nRet == -1 || nRet == 2)
    {
        PSS_LOGGER_DEBUG("file is already exist!");
        exit(1);
    }

    //如果文件锁没锁，则锁住当前文件锁
    if (AcquireWriteLock(fd_lock, 0, sizeof(int)) != 0)
    {
        PSS_LOGGER_DEBUG("lock the file failure and exit, idx = 0.");
        exit(1);
    }

    //写入子进程锁信息
    lseek(fd_lock, 0, SEEK_SET);

    for (int nIndex = 0; nIndex <= nNumChlid; nIndex++)
    {
        ssize_t stSize = write(fd_lock, &nIndex, sizeof(nIndex));

        if (0 == stSize)
        {
            PSS_LOGGER_DEBUG("write idx = {0}, error.", nIndex);
            exit(1);
        }
    }

    //启动并监控子进程
    while (true)
    {
        Run_Child_Process_Start(nNumChlid, fd_lock);

        //检查间隔
        ACE_OS::sleep(tvMonitorSleep);
    }

#endif
}

void CServerManager::Run_Child_Process_Start(int nNumChlid, const int& fd_lock)
{
    int nRet = 0;
	for (int nChlidIndex = 1; nChlidIndex <= nNumChlid; nChlidIndex++)
	{
		//休眠100ms
		ACE_Time_Value tvSleep(0, 100000);
		ACE_OS::sleep(tvSleep);

		//测试每个子进程的锁是否还存在
		nRet = SeeLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));

		if (nRet == -1 || nRet == 2)
		{
			continue;
		}

		//如果文件锁没有被锁，则设置文件锁，并启动子进程
		int npid = ACE_OS::fork();

		if (npid == 0)
		{
			//上文件锁
			AcquireWriteLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));

			//启动子进程
			if (false == Run())
			{
                PSS_LOGGER_DEBUG("child {0} Run failure.", nChlidIndex);
				exit(1);
			}

			//子进程在执行完任务后必须退出循环和释放锁
			ReleaseLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));
		}
	}
}

bool CServerManager::Close()
{
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close begin....");
    App_ConnectAcceptorManager::instance()->Close();
    m_ConnectConsoleAcceptor.close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close Acceptor OK");
    App_ClientReConnectManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ClientReConnectManager OK.");
    App_ReTTyClientManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ReTTyClientManager OK.");
    App_MessageManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_MessageManager OK.");
    App_MessageServiceGroup::instance()->Close();
    PSS_LOGGER_DEBUG("[App_MessageServiceGroup::Close]Close App_MessageServiceGroup OK.");
    App_ModuleLoader::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_MessageManager OK.");
    App_ConnectHandlerPool::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ConnectHandlerPool OK.");
    AppLogManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]AppLogManager OK.");
    App_ForwardManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ForwardManager OK.");
    App_IPAccount::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_IPAccount OK.");
    App_MessageBlockManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_MessageBlockManager OK.");
    App_FileTestManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_FileTestManager OK.");
    for (auto UdpServer : m_vecUDPList)
    {
        UdpServer->CloseFinaly();
    }
    m_vecUDPList.clear();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close UDP Listen OK.");
    App_ReactorManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ReactorManager OK.");
    App_ConsoleManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ConsoleManager OK.");
    App_TimerManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_TimerManager OK.");
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close end....");

    return true;
}


