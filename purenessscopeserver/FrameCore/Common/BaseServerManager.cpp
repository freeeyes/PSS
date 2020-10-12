#include "BaseServerManager.h"

bool Server_Manager_Common_LogSystem()
{
    //初始化日志系统线程
    auto pFileLogger = new CFileLogger();

    if (NULL == pFileLogger)
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Init]pFileLogger new is NULL.\n"));
        return false;
    }

    pFileLogger->Init();
    AppLogManager::instance()->Init(1, MAX_MSG_THREADQUEUE, GetXmlConfigAttribute(xmlAlertConnect)->MailID);

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
    App_IPAccount::instance()->Init(GetXmlConfigAttribute(xmlIP)->IPMax);

    App_ConnectAccount::instance()->Init(GetXmlConfigAttribute(xmlAlertConnect)->ConnectMin,
                                         GetXmlConfigAttribute(xmlAlertConnect)->DisConnectMax,
                                         GetXmlConfigAttribute(xmlAlertConnect)->DisConnectMin,
                                         GetXmlConfigAttribute(xmlAlertConnect)->DisConnectMax);

    //初始化BuffPacket缓冲池.默认都是当前最大连接数的2倍
    App_BuffPacketManager::instance()->Init(GetXmlConfigAttribute(xmlBuffPacket)->Count, CBuffPacketManager::Init_Callback);
    App_BuffPacketManager::instance()->SetCreateFlag(true);

    //初始化服务器间异步接收队列
    App_ServerMessageInfoPool::instance()->Init(GetXmlConfigAttribute(xmlConnectServer)->Count);

    if (GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount <= 0)
    {
        //初始化PacketParse对象池
        App_PacketParsePool::instance()->Init(MAX_HANDLE_POOL, CPacketParsePool::Init_Callback);
    }
    else
    {
        //初始化PacketParse对象池
        App_PacketParsePool::instance()->Init(GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount, CPacketParsePool::Init_Callback);
    }

    //初始化消息处理线程
    App_MessageServiceGroup::instance()->Init(GetXmlConfigAttribute(xmlMessage)->Msg_Thread,
            GetXmlConfigAttribute(xmlMessage)->Msg_MaxQueue,
            GetXmlConfigAttribute(xmlMessage)->Msg_Low_mark);

    return true;
}

bool Server_Manager_Common_IObject(IConnectManager* pConnectManager,
                                   IClientManager* pClientManager,
                                   IUDPConnectManager* pUDPConnectManager,
                                   IFrameCommand* pFrameCommand,
                                   IServerManager* pIServerManager,
                                   ITMService* pTMService,
                                   ITTyClientManager* pTTyClientManager,
                                   IMessageQueueManager* pMessageQueueManager,
                                   IControlListen* pControlListen)
{
    App_ServerObject::instance()->SetMessageManager(dynamic_cast<IMessageManager*>(App_MessageManager::instance()));
    App_ServerObject::instance()->SetLogManager(dynamic_cast<ILogManager*>(AppLogManager::instance()));
    App_ServerObject::instance()->SetConnectManager(pConnectManager);
    App_ServerObject::instance()->SetPacketManager(dynamic_cast<IPacketManager*>(App_BuffPacketManager::instance()));
    App_ServerObject::instance()->SetClientManager(pClientManager);
    App_ServerObject::instance()->SetUDPConnectManager(pUDPConnectManager);
    App_ServerObject::instance()->SetTimerManager(App_TimerManager::instance());
    App_ServerObject::instance()->SetModuleMessageManager(dynamic_cast<IModuleMessageManager*>(App_ModuleMessageManager::instance()));
    App_ServerObject::instance()->SetControlListen(pControlListen);
    App_ServerObject::instance()->SetModuleInfo(dynamic_cast<IModuleInfo*>(App_ModuleLoader::instance()));
    App_ServerObject::instance()->SetMessageBlockManager(dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()));
    App_ServerObject::instance()->SetFrameCommand(pFrameCommand);
    App_ServerObject::instance()->SetServerManager(pIServerManager);
    App_ServerObject::instance()->SetTMSServce(pTMService);
    App_ServerObject::instance()->SetTTyClientManager(pTTyClientManager);
    App_ServerObject::instance()->SetMessageQueueManager(pMessageQueueManager);
    App_ServerObject::instance()->SetEchartlog(dynamic_cast<IEchartlog*>(App_Echartlog::instance()));

    return true;
}

bool Server_Manager_Common_Module()
{
    auto u2ModuleVCount = (uint16)GetXmlConfigAttribute(xmlModuleInfos)->vec.size();

    for (uint16 i = 0; i < u2ModuleVCount; i++)
    {
        bool blState = App_ModuleLoader::instance()->LoadModule(GetXmlConfigAttribute(xmlModuleInfos)->vec[i].szModulePath.c_str(),
                       GetXmlConfigAttribute(xmlModuleInfos)->vec[i].szModuleName.c_str(),
                       GetXmlConfigAttribute(xmlModuleInfos)->vec[i].szModuleParam.c_str());

        if (false == blState)
        {
            OUR_DEBUG((LM_INFO, "[Server_Manager_Common_Module]LoadModule (%s)is error.\n",
                       GetXmlConfigAttribute(xmlModuleInfos)->vec[i].szModuleName.c_str()));
            return false;
        }
    }

    return true;
}

bool Server_Manager_Common_Addr(uint8 u4IpType, const char* pIP, uint16 u2Port, ACE_INET_Addr& listenAddr)
{
    //判断IPv4还是IPv6
    int nErr = 0;

    if (u4IpType == TYPE_IPV4)
    {
        if (ACE_OS::strcmp(pIP, "INADDR_ANY") == 0)
        {
            uint32 u4IPAddr = INADDR_ANY;
            nErr = listenAddr.set(u2Port, u4IPAddr);
        }
        else
        {
            nErr = listenAddr.set(u2Port, pIP);
        }
    }
    else
    {
        if (ACE_OS::strcmp(pIP, "INADDR_ANY") == 0)
        {
            uint32 u4IPAddr = INADDR_ANY;
            nErr = listenAddr.set(u2Port, u4IPAddr);
        }
        else
        {
            nErr = listenAddr.set(u2Port, pIP, 1, PF_INET6);
        }

    }

    if (nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[Server_Manager_Common_Addr](%d)set_address error[%s:%d].\n", pIP, u2Port, errno));
        return false;
    }

    return true;
}
