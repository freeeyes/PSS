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

    //初始化给插件的对象接口
    IConnectManager* pConnectManager           = dynamic_cast<IConnectManager*>(App_HandlerManager::instance());
    IClientManager*  pClientManager            = dynamic_cast<IClientManager*>(App_ClientProConnectManager::instance());
    IFrameCommand* pFrameCommand               = dynamic_cast<IFrameCommand*>(&m_objFrameCommand);
    IServerManager* pServerManager             = dynamic_cast<IServerManager*>(this);
    ITTyClientManager* pTTyClientManager       = dynamic_cast<ITTyClientManager*>(App_ProTTyClientManager::instance());
    IControlListen* pControlListen             = dynamic_cast<IControlListen*>(App_ProControlListen::instance());

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
    uint32 u4ClientProactorCount = 1;

    if (!App_ProConnectAcceptManager::instance()->InitConnectAcceptor(nServerPortCount, u4ClientProactorCount))
    {
        PSS_LOGGER_DEBUG("[CProServerManager::Init]{0}.", App_ProConnectAcceptManager::instance()->GetError());
        return false;
    }

    //初始化反应器集合
    App_ProactorManager::instance()->Init((uint16)nReactorCount);

    //初始化反应器
    for (int i = 0; i < nReactorCount; i++)
    {
        PSS_LOGGER_DEBUG("[CProServerManager::Init()]... i=[{0}].", i);

        if (GetXmlConfigAttribute(xmlNetWorkMode)->Mode == NETWORKMODE::NETWORKMODE_PRO_IOCP)
        {
            blState = App_ProactorManager::instance()->AddNewProactor(i, 
                Proactor_WIN32, 
                GetXmlConfigAttribute(xmlNetWorkMode)->ThreadCount);
            PSS_LOGGER_DEBUG("[CProServerManager::Init]AddNewProactor NETWORKMODE = Proactor_WIN32.");
        }
        else
        {
            PSS_LOGGER_DEBUG("[CProServerManager::Init]AddNewProactor NETWORKMODE Error.");
            return false;
        }

        if (!blState)
        {
            PSS_LOGGER_DEBUG("[CProServerManager::Init]AddNewProactor [{0}] Error.", i);
            return false;
        }
    }

    return true;
}

bool CProServerManager::Start()
{
    //启动定时器
    App_TimerManager::instance()->Start();

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
            PSS_LOGGER_DEBUG("[CProServerManager::Start]pConnectAcceptor[{0}] is nullptr.", i);
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
            PSS_LOGGER_DEBUG("[CProServerManager::Start] pConnectAcceptor->open[{0}] is error.", i);
            PSS_LOGGER_DEBUG("[CProServerManager::Start] Listen from [{0}:{1}] error({2}).",listenAddr.get_host_addr(), listenAddr.get_port_number(), errno);
            return false;
        }

        PSS_LOGGER_DEBUG("[CProServerManager::Start] Listen from [{0}:{1}] OK.", listenAddr.get_host_addr(), listenAddr.get_port_number());
    }

    //启动UDP监听
    int nUDPServerPortCount = (int)GetXmlConfigAttribute(xmlUDPServerIPs)->vec.size();

    for(int i = 0 ; i < nUDPServerPortCount; i++)
    {
        ACE_INET_Addr listenAddr;

        CProactorUDPHandler* pProactorUDPHandler = new CProactorUDPHandler();

        if(nullptr == pProactorUDPHandler)
        {
            PSS_LOGGER_DEBUG("[CProServerManager::Start] pProactorUDPHandler is nullptr[{0}] is error.", i);
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
                PSS_LOGGER_DEBUG("[CProServerManager::Start]UDP App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE) is nullptr.");
                return false;
            }

            pProactorUDPHandler->SetRecvSize(GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uMaxRecvSize);

            if(0 != pProactorUDPHandler->OpenAddress(listenAddr, pProactor))
            {
                PSS_LOGGER_DEBUG("[CProServerManager::Start] UDP Listen from [{0}:{1}] error({2}).",listenAddr.get_host_addr(), listenAddr.get_port_number(), errno);
                return false;
            }

            PSS_LOGGER_DEBUG("[CProServerManager::Start] UDP Listen from [{0}:{1}] OK.", listenAddr.get_host_addr(), listenAddr.get_port_number());
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
                    PSS_INADDR_ANY);
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
                    PSS_INADDR_ANY);
            }
            else
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport,
                                             GetXmlConfigAttribute(xmlConsole)->sip.c_str(), 1, PF_INET6);
            }
        }

        if(nErr != 0)
        {
            PSS_LOGGER_DEBUG("[CProServerManager::Start]listenConsoleAddr set_address error[{0}].", errno);
            return false;
        }

        ACE_Proactor* pProactor = App_ProactorManager::instance()->GetAce_Proactor();

        if(nullptr == pProactor)
        {
            PSS_LOGGER_DEBUG("[CProServerManager::Start]App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE) is nullptr.");
            return false;
        }

        int nRet = m_ProConsoleConnectAcceptor.open(listenConsoleAddr, 0, 1, MAX_ASYNCH_BACKLOG, 1, pProactor, true);

        if(-1 == nRet)
        {
            PSS_LOGGER_DEBUG("[CProServerManager::Start] m_ProConsoleConnectAcceptor.open is error.");
            PSS_LOGGER_DEBUG("[CProServerManager::Start] Listen from [{0}:{1}] error({2}).",listenConsoleAddr.get_host_addr(), listenConsoleAddr.get_port_number(), errno);
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

    //启动反应器(其他的反应器，因为插件第三方需要)
    if(!App_ProactorManager::instance()->StartOtherProactor())
    {
        PSS_LOGGER_DEBUG("[CProServerManager::Start]App_ProactorManager::instance()->StartOtherProactor is error.");
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
        PSS_LOGGER_DEBUG("[CServerManager::Run]App_ModuleLoader::instance()->InitModule() is error.");
        return false;
    }

    //开始消息处理线程
    App_MessageServiceGroup::instance()->Start();

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
        PSS_LOGGER_DEBUG("[CProServerManager::Start]App_ProactorManager::instance()->StartClientProactor is error.");
        return false;
    }

    return true;
}

bool CProServerManager::Close()
{
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close begin....");

    App_ProConnectAcceptManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_ProConnectAcceptManager OK.");

    m_ProConsoleConnectAcceptor.cancel();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close Acceptor OK.");

    App_ClientProConnectManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_ClientProConnectManager OK.");

    App_ProTTyClientManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_ProTTyClientManager OK.");

    App_MessageManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_MessageManager OK.");

    App_MessageServiceGroup::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_MessageServiceGroup OK.");

    App_ModuleLoader::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_ModuleLoader OK.");

    AppLogManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close AppLogManager OK");

    App_ProactorManager::instance()->StopProactor();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_ReactorManager OK.");

    App_ForwardManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_ForwardManager OK.");
    App_IPAccount::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_IPAccount OK.");
    App_MessageBlockManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_MessageBlockManager OK.");
    App_FileTestManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_FileTestManager OK.");
    App_ProConnectHandlerPool::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_ConnectHandlerPool OK.");
    App_ConsoleManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close App_ConsoleManager OK.");
    App_TimerManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_TimerManager OK.");
    PSS_LOGGER_DEBUG("[CProServerManager::Close]Close end....");

    //等待所有资源释放完毕
    ACE_Time_Value tvSleep(0, 100);
    ACE_OS::sleep(tvSleep);

    PSS_LOGGER_DEBUG("[CProServerManager::Close]EndLogStrategy end....");

    return true;
}
