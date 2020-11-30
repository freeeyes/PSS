#include "ProServerManager.h"

CProServerManager::CProServerManager(void)
{
}

bool CProServerManager::Init()
{
    int nServerPortCount    = (int)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();
    int nReactorCount       = 1;

    bool blState = false;

    //初始化Console相关参数
    App_ConsoleManager::instance()->Init();

    //初始化模块数组相关参数
    App_MessageManager::instance()->Init(GetXmlConfigAttribute(xmlModuleMangager)->MaxCount,
                                         GetXmlConfigAttribute(xmlCommandAccount)->MaxCommandCount);

    //初始化加载模块的信息
    App_ModuleLoader::instance()->Init(GetXmlConfigAttribute(xmlModuleMangager)->MaxCount);

    //初始化禁止IP列表
    App_ForbiddenIP::instance()->Init(FORBIDDENIP_FILE);

    //初始化日志系统线程
    if (false == Server_Manager_Common_LogSystem())
    {
        return false;
    }

    //初始化各种对象池
    Server_Manager_Common_Pool();

    //初始化ProConnectHandler对象池
    if(GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount <= 0)
    {
        //初始化PacketParse对象池
        App_ProConnectHandlerPool::instance()->Init(MAX_HANDLE_POOL);
    }
    else
    {
        //初始化PacketParse对象池
        App_ProConnectHandlerPool::instance()->Init(GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount);
    }

    //初始化TMS系统
    m_TMService.Init();

    //初始化给插件的对象接口
    IConnectManager* pConnectManager           = dynamic_cast<IConnectManager*>(App_HandlerManager::instance());
    IClientManager*  pClientManager            = dynamic_cast<IClientManager*>(App_ClientProConnectManager::instance());
    IFrameCommand* pFrameCommand               = dynamic_cast<IFrameCommand*>(&m_objFrameCommand);
    ITMService* pTMService                     = dynamic_cast<ITMService*>(&m_TMService);
    IServerManager* pServerManager             = dynamic_cast<IServerManager*>(this);
    ITTyClientManager* pTTyClientManager       = dynamic_cast<ITTyClientManager*>(App_ProTTyClientManager::instance());
    IControlListen* pControlListen             = dynamic_cast<IControlListen*>(App_ProControlListen::instance());

    Server_Manager_Common_IObject(pConnectManager,
                                  pClientManager,
                                  pFrameCommand,
                                  pServerManager,
                                  pTMService,
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
    uint32 u4ClientProactorCount = 1;

    if (!App_ProConnectAcceptManager::instance()->InitConnectAcceptor(nServerPortCount, u4ClientProactorCount))
    {
        OUR_DEBUG((LM_INFO, "[CProServerManager::Init]%s.\n", App_ProConnectAcceptManager::instance()->GetError()));
        return false;
    }

    //初始化反应器集合
    App_ProactorManager::instance()->Init((uint16)nReactorCount);

    //初始化反应器
    for (int i = 0; i < nReactorCount; i++)
    {
        OUR_DEBUG((LM_INFO, "[CProServerManager::Init()]... i=[%d].\n", i));

        if (GetXmlConfigAttribute(xmlNetWorkMode)->Mode == NETWORKMODE::NETWORKMODE_PRO_IOCP)
        {
            blState = App_ProactorManager::instance()->AddNewProactor(i, 
                Proactor_WIN32, 
                GetXmlConfigAttribute(xmlNetWorkMode)->ThreadCount);
            OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AddNewProactor NETWORKMODE = Proactor_WIN32.\n"));
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AddNewProactor NETWORKMODE Error.\n"));
            return false;
        }

        if (!blState)
        {
            OUR_DEBUG((LM_INFO, "[CProServerManager::Init]AddNewProactor [%d] Error.\n", i));
            return false;
        }
    }

    return true;
}

bool CProServerManager::Start()
{
    //启动TCP监听
    int nServerPortCount = (int)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();

    //初始化监听远程连接
    for(int i = 0 ; i < nServerPortCount; i++)
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
        ProConnectAcceptor* pConnectAcceptor = App_ProConnectAcceptManager::instance()->GetConnectAcceptor(i);

        if(nullptr == pConnectAcceptor)
        {
            OUR_DEBUG((LM_INFO, "[CProServerManager::Start]pConnectAcceptor[%d] is nullptr.\n", i));
            return false;
        }

        //设置监听IP信息
        pConnectAcceptor->SetPacketParseInfoID(GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].packetparseid);
        pConnectAcceptor->SetListenInfo(GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].ip.c_str(),
                                        (uint32)GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].port);

        ACE_Proactor* pProactor = App_ProactorManager::instance()->GetAce_Proactor();

        int nBackLog = GetXmlConfigAttribute(xmlNetWorkMode)->BackLog;
        int nRet = pConnectAcceptor->open(listenAddr, 
            0, 
            1, 
            GetXmlConfigAttribute(xmlNetWorkMode)->BackLog, 
            1, 
            pProactor);

        if(-1 == nRet)
        {
            OUR_DEBUG((LM_INFO, "[CProServerManager::Start] pConnectAcceptor->open[%d] is error.\n", i));
            OUR_DEBUG((LM_INFO, "[CProServerManager::Start] Listen from [%s:%d] error(%d).\n",listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
            return false;
        }

        OUR_DEBUG((LM_INFO, "[CProServerManager::Start] Listen from [%s:%d] OK.\n", listenAddr.get_host_addr(), listenAddr.get_port_number()));
    }

    //启动UDP监听
    int nUDPServerPortCount = (int)GetXmlConfigAttribute(xmlUDPServerIPs)->vec.size();

    for(int i = 0 ; i < nUDPServerPortCount; i++)
    {
        ACE_INET_Addr listenAddr;

        CProactorUDPHandler* pProactorUDPHandler = new CProactorUDPHandler();

        if(nullptr == pProactorUDPHandler)
        {
            OUR_DEBUG((LM_INFO, "[CProServerManager::Start] pProactorUDPHandler is nullptr[%d] is error.\n", i));
            return false;
        }
        else
        {
            pProactorUDPHandler->SetPacketParseInfoID(GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uPacketParseID);
            int nErr = 0;

            if (false == Server_Manager_Common_Addr(GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uipType,
                                                    GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uip.c_str(),
                                                    GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uport,
                                                    listenAddr))
            {
                return false;
            }

            ACE_Proactor* pProactor = App_ProactorManager::instance()->GetAce_Proactor();

            if(nullptr == pProactor)
            {
                OUR_DEBUG((LM_INFO, "[CProServerManager::Start]UDP App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE) is nullptr.\n"));
                return false;
            }

            pProactorUDPHandler->SetRecvSize(GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uMaxRecvSize);

            if(0 != pProactorUDPHandler->OpenAddress(listenAddr, pProactor))
            {
                OUR_DEBUG((LM_INFO, "[CProServerManager::Start] UDP Listen from [%s:%d] error(%d).\n",listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
                return false;
            }

            OUR_DEBUG((LM_INFO, "[CProServerManager::Start] UDP Listen from [%s:%d] OK.\n", listenAddr.get_host_addr(), listenAddr.get_port_number()));
        }
    }

    //启动后台管理端口监听
    if(GetXmlConfigAttribute(xmlConsole)->support == CONSOLE_ENABLE)
    {
        ACE_INET_Addr listenConsoleAddr;

        int nErr = 0;

        if(GetXmlConfigAttribute(xmlConsole)->ipType == TYPE_IPV4)
        {
            if(ACE_OS::strcmp(GetXmlConfigAttribute(xmlConsole)->sip.c_str(), "INADDR_ANY") == 0)
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport,
                                             (uint32)INADDR_ANY);
            }
            else
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport,
                                             GetXmlConfigAttribute(xmlConsole)->sip.c_str());
            }
        }
        else
        {
            if(ACE_OS::strcmp(GetXmlConfigAttribute(xmlConsole)->sip.c_str(), "INADDR_ANY") == 0)
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport,
                                             (uint32)INADDR_ANY);
            }
            else
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport,
                                             GetXmlConfigAttribute(xmlConsole)->sip.c_str(), 1, PF_INET6);
            }
        }

        if(nErr != 0)
        {
            OUR_DEBUG((LM_INFO, "[CProServerManager::Start]listenConsoleAddr set_address error[%d].\n", errno));
            return false;
        }

        ACE_Proactor* pProactor = App_ProactorManager::instance()->GetAce_Proactor();

        if(nullptr == pProactor)
        {
            OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE) is nullptr.\n"));
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
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM, "[CProServerManager::Init]AppLogManager is ERROR.");
    }
    else
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM, "[CProServerManager::Init]AppLogManager is OK.");
    }

    //启动定时器
    if(0 != App_TimerManager::instance()->activate())
    {
        OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_TimerManager::instance()->Start() is error.\n"));
        return false;
    }

    //启动反应器(其他的反应器，因为插件第三方需要)
    if(!App_ProactorManager::instance()->StartOtherProactor())
    {
        OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_ProactorManager::instance()->StartOtherProactor is error.\n"));
        return false;
    }

    //初始化服务器间通讯类
    App_ClientProConnectManager::instance()->Init(App_ProactorManager::instance()->GetAce_Proactor());

    //初始化TTy连接管理器
    App_ProTTyClientManager::instance()->Init(App_ProactorManager::instance()->GetAce_Proactor(),
            GetXmlConfigAttribute(xmlTTyClientManagerInfo)->MaxTTyDevCount,
            GetXmlConfigAttribute(xmlTTyClientManagerInfo)->TimeCheck);

    //启动中间服务器链接管理器定时器
    App_ClientProConnectManager::instance()->StartConnectTask(GetXmlConfigAttribute(xmlConnectServer)->TimeCheck);

    //加载所有的插件初始化动作
    if (false == App_ModuleLoader::instance()->InitModule())
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Run]App_ModuleLoader::instance()->InitModule() is error.\n"));
        return false;
    }

    //开始消息处理线程
    App_MessageServiceGroup::instance()->Start();

    if(GetXmlConfigAttribute(xmlConnectServer)->RunType == 1)
    {
        //启动异步处理服务器间消息包的过程
        App_ServerMessageTask::instance()->Start();
    }

    //开始启动tty相关监听
    int nTTyCount = (int)GetXmlConfigAttribute(xmlTTyDrives)->vec.size();

    for (int i = 0; i < nTTyCount; i++)
    {
        _TTyDevParam objCom;

        objCom.m_nBaudRate   = (int)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4Baud;
        objCom.m_uDatabits   = (unsigned char)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4DataBits;
        objCom.m_uStopbits   = (unsigned char)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4StopBits;
        objCom.m_pParitymode = (const char*)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].strParity.c_str();
        strcpy_safe(GetXmlConfigAttribute(xmlTTyDrives)->vec[i].strPortName.c_str(), objCom.m_szDevName, 32);

        //添加监听
        App_ProTTyClientManager::instance()->ConnectFrame(GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4TTyID,
                GetXmlConfigAttribute(xmlTTyDrives)->vec[i].strPortName.c_str(),
                objCom,
                GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4PacketParseID);

    }

    //开启服务器间通讯相关配置信息
    int nS2SCount = (int)GetXmlConfigAttribute(xmlServer2Server)->vec.size();

    for (int i = 0; i < nS2SCount; i++)
    {
        uint8 u1IPType = TYPE_IPV4;
        Check_IPType(GetXmlConfigAttribute(xmlServer2Server)->vec[i].strServerIP.c_str(), u1IPType);

        //开启服务器2服务器监听
        App_ClientProConnectManager::instance()->ConnectFrame(GetXmlConfigAttribute(xmlServer2Server)->vec[i].u4ServerID,
                GetXmlConfigAttribute(xmlServer2Server)->vec[i].strServerIP.c_str(),
                GetXmlConfigAttribute(xmlServer2Server)->vec[i].u2ServerPort,
                u1IPType,
                GetXmlConfigAttribute(xmlServer2Server)->vec[i].u4PacketParseID);
    }

    //开闸，让客户端数据进来
    if (!App_ProactorManager::instance()->StartClientProactor())
    {
        OUR_DEBUG((LM_INFO, "[CProServerManager::Start]App_ProactorManager::instance()->StartClientProactor is error.\n"));
        return false;
    }

    return true;
}

bool CProServerManager::Close()
{
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close begin....\n"));

    m_TMService.Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close m_TSThread OK.\n"));

    App_ProConnectAcceptManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ProConnectAcceptManager OK.\n"));

    m_ProConsoleConnectAcceptor.cancel();
    App_TimerManager::instance()->deactivate();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_TimerManager OK.\n"));

    App_ClientProConnectManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ClientProConnectManager OK.\n"));

    App_ProTTyClientManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ProTTyClientManager OK.\n"));

    App_MessageManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_MessageManager OK.\n"));

    App_MessageServiceGroup::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_MessageServiceGroup OK.\n"));

    App_ModuleLoader::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ModuleLoader OK.\n"));

    App_ServerMessageTask::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ServerMessageTask OK.\n"));

    AppLogManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close AppLogManager OK\n"));

    App_BuffPacketManager::instance()->Close_Object(CBuffPacketManager::Close_Callback);
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_BuffPacketManager OK\n"));


    App_ProactorManager::instance()->StopProactor();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ReactorManager OK.\n"));

    App_ForwardManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ForwardManager OK.\n"));
    App_IPAccount::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_IPAccount OK.\n"));
    App_MessageBlockManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_MessageBlockManager OK.\n"));
    App_PacketParsePool::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_PacketParsePool OK.\n"));
    App_FileTestManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_FileTestManager OK.\n"));
    App_ProConnectHandlerPool::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ConnectHandlerPool OK.\n"));
    App_ConsoleManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close App_ConsoleManager OK.\n"));

    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]Close end....\n"));

    //等待所有资源释放完毕
    ACE_Time_Value tvSleep(0, 100);
    ACE_OS::sleep(tvSleep);

    OUR_DEBUG((LM_INFO, "[CProServerManager::Close]EndLogStrategy end....\n"));

    return true;
}
