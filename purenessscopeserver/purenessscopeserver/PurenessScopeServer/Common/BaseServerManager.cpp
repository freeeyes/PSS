#include "BaseServerManager.h"

bool Server_Manager_Common_FrameLogging(Frame_Logging_Strategy*& pFrameLoggingStrategy)
{
    if (App_MainConfig::instance()->GetDebugTrunOn() == 1)
    {
        pFrameLoggingStrategy = new Frame_Logging_Strategy();

        //是否打开ACE_DEBUG文件存储
        Logging_Config_Param objParam;

        sprintf_safe(objParam.m_strLogFile, 256, "%s", App_MainConfig::instance()->GetDebugFileName());
        objParam.m_iChkInterval = App_MainConfig::instance()->GetChkInterval();
        objParam.m_iLogFileMaxCnt = App_MainConfig::instance()->GetLogFileMaxCnt();
        objParam.m_iLogFileMaxSize = App_MainConfig::instance()->GetLogFileMaxSize();
        sprintf_safe(objParam.m_strLogLevel, 128, "%s", App_MainConfig::instance()->GetDebugLevel());

        pFrameLoggingStrategy->InitLogStrategy(objParam);
    }

    return true;
}

bool Server_Manager_Common_LogSystem()
{
    //初始化日志系统线程
    CFileLogger* pFileLogger = new CFileLogger();

    if (NULL == pFileLogger)
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Init]pFileLogger new is NULL.\n"));
        return false;
    }

    pFileLogger->Init();
    AppLogManager::instance()->Init(1, MAX_MSG_THREADQUEUE, App_MainConfig::instance()->GetConnectAlert()->m_u4MailID);

    if (0 != AppLogManager::instance()->RegisterLog(pFileLogger))
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Init]AppLogManager::instance()->RegisterLog error.\n"));
        return false;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Init]AppLogManager is OK.\n"));
    }

    return true;
}

bool Server_Manager_Common_Pool()
{
    //初始化防攻击系统
    App_IPAccount::instance()->Init(App_MainConfig::instance()->GetIPAlert()->m_u4IPMaxCount);

    App_ConnectAccount::instance()->Init(App_MainConfig::instance()->GetConnectAlert()->m_u4ConnectMin,
                                         App_MainConfig::instance()->GetConnectAlert()->m_u4ConnectMax,
                                         App_MainConfig::instance()->GetConnectAlert()->m_u4DisConnectMin,
                                         App_MainConfig::instance()->GetConnectAlert()->m_u4DisConnectMax);

    //初始化BuffPacket缓冲池.默认都是当前最大连接数的2倍
    App_BuffPacketManager::instance()->Init(App_MainConfig::instance()->GetBuffPacketPoolCount(), CBuffPacketManager::Init_Callback);

    //初始化服务器间异步接收队列
    App_ServerMessageInfoPool::instance()->Init(App_MainConfig::instance()->GetServerConnectCount());

    if (App_MainConfig::instance()->GetMaxHandlerCount() <= 0)
    {
        //初始化PacketParse对象池
        App_PacketParsePool::instance()->Init(MAX_HANDLE_POOL, CPacketParsePool::Init_Callback);
    }
    else
    {
        //初始化PacketParse对象池
        App_PacketParsePool::instance()->Init(App_MainConfig::instance()->GetMaxHandlerCount(), CPacketParsePool::Init_Callback);
    }

    //初始化消息处理线程
    App_MessageServiceGroup::instance()->Init(App_MainConfig::instance()->GetThreadCount(),
            App_MainConfig::instance()->GetMsgMaxQueue(),
            App_MainConfig::instance()->GetMsgLowMark(),
            App_MainConfig::instance()->GetMgsHighMark());

    return true;
}

bool Server_Manager_Common_IObject(IConnectManager* pConnectManager, IClientManager* pClientManager, IUDPConnectManager* pUDPConnectManager, IFrameCommand* pFrameCommand, IServerManager* pIServerManager)
{
    App_ServerObject::instance()->SetMessageManager(dynamic_cast<IMessageManager*>(App_MessageManager::instance()));
    App_ServerObject::instance()->SetLogManager(dynamic_cast<ILogManager*>(AppLogManager::instance()));
    App_ServerObject::instance()->SetConnectManager(pConnectManager);
    App_ServerObject::instance()->SetPacketManager(dynamic_cast<IPacketManager*>(App_BuffPacketManager::instance()));
    App_ServerObject::instance()->SetClientManager(pClientManager);
    App_ServerObject::instance()->SetUDPConnectManager(pUDPConnectManager);
    App_ServerObject::instance()->SetTimerManager(reinterpret_cast<ActiveTimer*>(App_TimerManager::instance()));
    App_ServerObject::instance()->SetModuleMessageManager(dynamic_cast<IModuleMessageManager*>(App_ModuleMessageManager::instance()));
    App_ServerObject::instance()->SetControlListen(dynamic_cast<IControlListen*>(App_ControlListen::instance()));
    App_ServerObject::instance()->SetModuleInfo(dynamic_cast<IModuleInfo*>(App_ModuleLoader::instance()));
    App_ServerObject::instance()->SetMessageBlockManager(dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()));
    App_ServerObject::instance()->SetFrameCommand(pFrameCommand);
    App_ServerObject::instance()->SetServerManager(pIServerManager);

    return true;
}

bool Server_Manager_Common_Module()
{
    uint16 u2ModuleVCount = App_MainConfig::instance()->GetModuleInfoCount();

    for (uint16 i = 0; i < u2ModuleVCount; i++)
    {
        _ModuleConfig* pModuleConfig = App_MainConfig::instance()->GetModuleInfo(i);

        if (NULL != pModuleConfig)
        {
            bool blState = App_ModuleLoader::instance()->LoadModule(pModuleConfig->m_szModulePath,
                           pModuleConfig->m_szModuleName,
                           pModuleConfig->m_szModuleParam);

            if (false == blState)
            {
                OUR_DEBUG((LM_INFO, "[Server_Manager_Common_Module]LoadModule (%s)is error.\n", pModuleConfig->m_szModuleName));
                return false;
            }
        }
    }

    return true;
}

bool Server_Manager_Common_Addr(_ServerInfo* pServerInfo, ACE_INET_Addr& listenAddr)
{
    //判断IPv4还是IPv6
    int nErr = 0;

    if (pServerInfo->m_u1IPType == TYPE_IPV4)
    {
        if (ACE_OS::strcmp(pServerInfo->m_szServerIP, "INADDR_ANY") == 0)
        {
            nErr = listenAddr.set(pServerInfo->m_nPort, (uint32)INADDR_ANY);
        }
        else
        {
            nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP);
        }
    }
    else
    {
        if (ACE_OS::strcmp(pServerInfo->m_szServerIP, "INADDR_ANY") == 0)
        {
            nErr = listenAddr.set(pServerInfo->m_nPort, (uint32)INADDR_ANY);
        }
        else
        {
            nErr = listenAddr.set(pServerInfo->m_nPort, pServerInfo->m_szServerIP, 1, PF_INET6);
        }

    }

    if (nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[Server_Manager_Common_Addr](%d)set_address error[%s:%d].\n", pServerInfo->m_szServerIP, pServerInfo->m_nPort, errno));
        return false;
    }

    return true;
}
