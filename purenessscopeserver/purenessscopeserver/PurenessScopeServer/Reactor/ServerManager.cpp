#include "ServerManager.h"

CServerManager::CServerManager(void)
{
    m_pFrameLoggingStrategy = NULL;
}

CServerManager::~CServerManager(void)
{
    OUR_DEBUG((LM_INFO, "[CServerManager::~CServerManager].\n"));
}

bool CServerManager::Init()
{
    //是否打开ACE_DEBUG文件存储
    Server_Manager_Common_FrameLogging(m_pFrameLoggingStrategy);

    int nServerPortCount = App_MainConfig::instance()->GetServerPortCount();
    int nReactorCount = App_MainConfig::instance()->GetReactorCount();

    //初始化模块数组相关参数
    App_MessageManager::instance()->Init(App_MainConfig::instance()->GetMaxModuleCount(), App_MainConfig::instance()->GetMaxCommandCount());

    //初始化加载模块的信息
    App_ModuleLoader::instance()->Init(App_MainConfig::instance()->GetMaxModuleCount());

    //初始化禁止IP列表
    App_ForbiddenIP::instance()->Init(FORBIDDENIP_FILE);

    OUR_DEBUG((LM_INFO, "[CServerManager::Init]nReactorCount=%d.\n", nReactorCount));

    //为多进程做准备，针对epoll和epollet初始化不能在这里去做,因为在多进程里epoll_create必须在子进程里去声明
    if (NETWORKMODE_RE_EPOLL != App_MainConfig::instance()->GetNetworkMode() && NETWORKMODE_RE_EPOLL_ET != App_MainConfig::instance()->GetNetworkMode())
    {
        //初始化反应器集合
        App_ReactorManager::instance()->Init((uint16)nReactorCount);

        OUR_DEBUG((LM_INFO, "[CServerManager::Init]****1*******.\n"));
        Init_Reactor((uint8)nReactorCount, App_MainConfig::instance()->GetNetworkMode());
    }

    //初始化日志系统线程
    if (false == Server_Manager_Common_LogSystem())
    {
        return false;
    }

    //初始化各种对象池
    Server_Manager_Common_Pool();

    //初始化ConnectHandler对象池
    if (App_MainConfig::instance()->GetMaxHandlerCount() <= 0)
    {
        //初始化PacketParse对象池
        App_ConnectHandlerPool::instance()->Init(MAX_HANDLE_POOL);
    }
    else
    {
        //初始化PacketParse对象池
        App_ConnectHandlerPool::instance()->Init(App_MainConfig::instance()->GetMaxHandlerCount());
    }

    //初始化链接管理器
    App_ConnectManager::instance()->Init(App_MainConfig::instance()->GetSendQueueCount());

    //初始化给插件的对象接口
    IConnectManager* pConnectManager       = dynamic_cast<IConnectManager*>(App_ConnectManager::instance());
    IClientManager*  pClientManager        = dynamic_cast<IClientManager*>(App_ClientReConnectManager::instance());
    IUDPConnectManager* pUDPConnectManager = dynamic_cast<IUDPConnectManager*>(App_ReUDPManager::instance());
    IFrameCommand* pFrameCommand           = dynamic_cast<IFrameCommand*>(&m_objFrameCommand);
    IServerManager* pServerManager         = dynamic_cast<IServerManager*>(this);
    Server_Manager_Common_IObject(pConnectManager,
                                  pClientManager,
                                  pUDPConnectManager,
                                  pFrameCommand,
                                  pServerManager);

    //初始化消息处理线程
    App_MessageServiceGroup::instance()->Init(App_MainConfig::instance()->GetThreadCount(),
            App_MainConfig::instance()->GetMsgMaxQueue(),
            App_MainConfig::instance()->GetMsgLowMark(),
            App_MainConfig::instance()->GetMgsHighMark());

    //初始化模块加载，因为这里可能包含了中间服务器连接加载
    if (false == Server_Manager_Common_Module())
    {
        return false;
    }

    //让所有的线程拷同步副本
    App_MessageServiceGroup::instance()->CopyMessageManagerList();

    //初始化连接器
    uint32 u4ClientReactorCount = (uint32)nReactorCount - 3;

    if (!App_ConnectAcceptorManager::instance()->InitConnectAcceptor(nServerPortCount, u4ClientReactorCount))
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Init]%s.\n", App_ConnectAcceptorManager::instance()->GetError()));
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

    //启动UDP监听
    blRet = Start_Udp_Listen();

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

    if (App_MainConfig::instance()->GetProcessCount() > 1)
    {
        Multiple_Process_Start();
    }
    else
    {
        if (false == Run())
        {
            OUR_DEBUG((LM_INFO, "child Run failure.\n"));
            return false;
        }
    }

    return true;
}

bool CServerManager::Init_Reactor(uint8 u1ReactorCount, uint8 u1NetMode)
{
    bool blState = true;

    //初始化反应器
    for (uint8 i = 0; i < u1ReactorCount; i++)
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]... i=[%d].\n", i));

        if (u1NetMode == NETWORKMODE_RE_SELECT)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_Select, 1);
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_Select.\n"));
        }
        else if (u1NetMode == NETWORKMODE_RE_TPSELECT)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_TP, 1);
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_TP.\n"));
        }
        else if (u1NetMode == NETWORKMODE_RE_EPOLL)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_DEV_POLL, 1, App_MainConfig::instance()->GetMaxHandlerCount());
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL.\n"));
        }
        else if (u1NetMode == NETWORKMODE_RE_EPOLL_ET)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, Reactor_DEV_POLL_ET, 1, App_MainConfig::instance()->GetMaxHandlerCount());
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL_ET.\n"));
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewProactor NETWORKMODE Error.\n"));
            return false;
        }

        if (!blState)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewReactor [%d] Error.\n", i));
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]Error=%s.\n", App_ReactorManager::instance()->GetError()));
            return false;
        }
    }

    return blState;
}

bool CServerManager::Run()
{
    //对应多进程，epoll必须在子进程里进行初始化
    if (NETWORKMODE_RE_EPOLL == App_MainConfig::instance()->GetNetworkMode() || NETWORKMODE_RE_EPOLL_ET == App_MainConfig::instance()->GetNetworkMode())
    {
        //初始化反应器集合
        App_ReactorManager::instance()->Init((uint16)App_MainConfig::instance()->GetReactorCount());

        if (false == Init_Reactor((uint8)App_MainConfig::instance()->GetReactorCount(), App_MainConfig::instance()->GetNetworkMode()))
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Run]Init_Reactor Error.\n"));
            return false;
        }
    }

    //启动中间服务器链接管理器
    App_ClientReConnectManager::instance()->Init(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_POSTDEFINE));

    int nServerPortCount = App_MainConfig::instance()->GetServerPortCount();

    //创建和启动TCP反应器
    for (int i = 0; i < nServerPortCount; i++)
    {
        //得到接收器
        ConnectAcceptor* pConnectAcceptor = App_ConnectAcceptorManager::instance()->GetConnectAcceptor(i);

        //打开监听对应事件
        pConnectAcceptor->Run_Open(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));
    }

    m_ConnectConsoleAcceptor.Run_Open(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));

    //创建和启动UDP反应器
    uint16 u2UDPServerPortCount = App_MainConfig::instance()->GetUDPServerPortCount();

    for (uint16 i = 0; i < u2UDPServerPortCount; i++)
    {
        CReactorUDPHander* pReactorUDPHandler = App_ReUDPManager::instance()->GetUDPHandle((uint8)i);

        if (NULL != pReactorUDPHandler)
        {
            pReactorUDPHandler->Run_Open(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));
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

    //启动服务器间检查线程
    if (false == App_ClientReConnectManager::instance()->StartConnectTask(App_MainConfig::instance()->GetConnectServerCheck()))
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Run]StartConnectTask error.\n"));
        return false;
    }

    //启动定时器
    if (0 != App_TimerManager::instance()->activate())
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Run]App_TimerManager::instance()->Start() is error.\n"));
        return false;
    }

    //启动所有反应器(不是客户端接收的反应器，在这里不能启动)
    if (!App_ReactorManager::instance()->StartOtherReactor())
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Run]App_ReactorManager::instance()->StartOtherReactor is error.\n"));
        return false;
    }

    //加载所有的插件初始化动作
    if (false == App_ModuleLoader::instance()->InitModule())
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Run]App_ModuleLoader::instance()->InitModule() is error.\n"));
        return false;
    }

    //开始消息处理线程
    App_MessageServiceGroup::instance()->Start();

    if (App_MainConfig::instance()->GetConnectServerRunType() == 1)
    {
        //启动异步处理服务器间消息包的过程
        App_ServerMessageTask::instance()->Start();
    }

    //开始启动链接发送定时器
    App_ConnectManager::instance()->StartTimer();

    //最后开闸，启动客户端反应器，让客户端数据进来
    if (!App_ReactorManager::instance()->StartClientReactor())
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Run]App_ReactorManager::instance()->StartClientReactor is error.\n"));
        return false;
    }

    ACE_Thread_Manager::instance()->wait();

    return true;
}

bool CServerManager::Start_Tcp_Listen()
{
    int nServerPortCount = App_MainConfig::instance()->GetServerPortCount();

    for (int i = 0; i < nServerPortCount; i++)
    {
        ACE_INET_Addr listenAddr;

        _ServerInfo* pServerInfo = App_MainConfig::instance()->GetServerPort(i);

        if (false == Server_Manager_Common_Addr(pServerInfo, listenAddr))
        {
            return false;
        }

        //得到接收器
        ConnectAcceptor* pConnectAcceptor = App_ConnectAcceptorManager::instance()->GetConnectAcceptor(i);

        if (NULL == pConnectAcceptor)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Start]pConnectAcceptor[%d] is NULL.\n", i));
            return false;
        }

        pConnectAcceptor->SetPacketParseInfoID(pServerInfo->m_u4PacketParseInfoID);
        int nRet = pConnectAcceptor->Init_Open(listenAddr, 0, 1, 1, (int)App_MainConfig::instance()->GetBacklog());

        if (-1 == nRet)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Start] pConnectAcceptor->open[%d] is error.\n", i));
            OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] error(%d).\n", listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
            return false;
        }

        OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] OK.\n", listenAddr.get_host_addr(), listenAddr.get_port_number()));
    }

    return true;
}

bool CServerManager::Start_Udp_Listen()
{
    int nUDPServerPortCount = App_MainConfig::instance()->GetUDPServerPortCount();

    for (int i = 0; i < nUDPServerPortCount; i++)
    {
        ACE_INET_Addr listenAddr;

        _ServerInfo* pServerInfo = App_MainConfig::instance()->GetUDPServerPort(i);

        if (false == Server_Manager_Common_Addr(pServerInfo, listenAddr))
        {
            return false;
        }

        //得到接收器
        CReactorUDPHander* pReactorUDPHandler = App_ReUDPManager::instance()->Create();

        if (NULL == pReactorUDPHandler)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Start]UDP pReactorUDPHandler[%d] is NULL.\n", i));
            return false;
        }

        pReactorUDPHandler->SetPacketParseInfoID(pServerInfo->m_u4PacketParseInfoID);
        int nRet = pReactorUDPHandler->OpenAddress(listenAddr);

        if (-1 == nRet)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Start] UDP Listen from [%s:%d] error(%d).\n", listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
            return false;
        }

        OUR_DEBUG((LM_INFO, "[CServerManager::Start] UDP Listen from [%s:%d] OK.\n", listenAddr.get_host_addr(), listenAddr.get_port_number()));
    }

    return true;
}

bool CServerManager::Start_Console_Tcp_Listen()
{
    if (App_MainConfig::instance()->GetConsoleSupport() == CONSOLE_ENABLE)
    {
        ACE_INET_Addr listenConsoleAddr;
        int nErr = 0;

        if (App_MainConfig::instance()->GetConsoleIPType() == TYPE_IPV4)
        {
            if (ACE_OS::strcmp(App_MainConfig::instance()->GetConsoleIP(), "INADDR_ANY") == 0)
            {
                nErr = listenConsoleAddr.set(App_MainConfig::instance()->GetConsolePort(), (uint32)INADDR_ANY);
            }
            else
            {
                nErr = listenConsoleAddr.set(App_MainConfig::instance()->GetConsolePort(), App_MainConfig::instance()->GetConsoleIP());
            }
        }
        else
        {
            if (ACE_OS::strcmp(App_MainConfig::instance()->GetConsoleIP(), "INADDR_ANY") == 0)
            {
                nErr = listenConsoleAddr.set(App_MainConfig::instance()->GetConsolePort(), (uint32)INADDR_ANY);
            }
            else
            {
                nErr = listenConsoleAddr.set(App_MainConfig::instance()->GetConsolePort(), App_MainConfig::instance()->GetConsoleIP(), 1, PF_INET6);
            }
        }

        if (nErr != 0)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Start]listenConsoleAddr set_address error[%d].\n", errno));
            return false;
        }

        int nRet = m_ConnectConsoleAcceptor.Init_Open(listenConsoleAddr);

        if (-1 == nRet)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Start] pConnectAcceptor->open is error.\n"));
            OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] error(%d).\n", listenConsoleAddr.get_host_addr(), listenConsoleAddr.get_port_number(), errno));
            return false;
        }
    }

    return true;
}

void CServerManager::Multiple_Process_Start()
{
#ifndef WIN32
    //当前监控子线程个数
    int nNumChlid = App_MainConfig::instance()->GetProcessCount();

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
    char szFileName[200] = { '\0' };
    //memset(szFileName, 0, sizeof(szFileName));
    sprintf(szFileName, "%s/lockwatch.lk", szWorkDir);
    fd_lock = open(szFileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (fd_lock < 0)
    {
        printf("open the flock and exit, errno = %d.\n", errno);
        exit(1);
    }

    //查看当前文件锁是否已锁
    nRet = SeeLock(fd_lock, 0, sizeof(int));

    if (nRet == -1 || nRet == 2)
    {
        printf("file is already exist!\n");
        exit(1);
    }

    //如果文件锁没锁，则锁住当前文件锁
    if (AcquireWriteLock(fd_lock, 0, sizeof(int)) != 0)
    {
        printf("lock the file failure and exit, idx = 0.\n");
        exit(1);
    }

    //写入子进程锁信息
    lseek(fd_lock, 0, SEEK_SET);

    for (int nIndex = 0; nIndex <= nNumChlid; nIndex++)
    {
        write(fd_lock, &nIndex, sizeof(nIndex));
    }

    //启动并监控子进程
    while (1)
    {
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
                if (AcquireWriteLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int)) != 0)
                {
                    printf("child %d AcquireWriteLock failure.\n", nChlidIndex);
                    exit(1);
                }

                //启动子进程
                if (false == Run())
                {
                    printf("child %d Run failure.\n", nChlidIndex);
                    exit(1);
                }

                //子进程在执行完任务后必须退出循环和释放锁
                ReleaseLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));
            }
        }

        //printf("child count(%d) is ok.\n", nNumChlid);
        //检查间隔
        ACE_OS::sleep(tvMonitorSleep);
    }

#endif
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
    App_ServerMessageTask::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ServerMessageTask OK.\n"));
    App_MessageServiceGroup::instance()->Close();
    OUR_DEBUG((LM_INFO, "[App_MessageServiceGroup::Close]Close App_MessageServiceGroup OK.\n"));
    App_ConnectManager::instance()->CloseAll();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ConnectManager OK.\n"));
    AppLogManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]AppLogManager OK\n"));
    App_MessageManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_MessageManager OK.\n"));
    App_BuffPacketManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]BuffPacketManager OK\n"));
    App_ReactorManager::instance()->StopReactor();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ReactorManager OK.\n"));
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close end....\n"));

    if (NULL != m_pFrameLoggingStrategy)
    {
        m_pFrameLoggingStrategy->EndLogStrategy();
        SAFE_DELETE(m_pFrameLoggingStrategy);
    }

    return true;
}


