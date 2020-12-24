#include "BaseServerManager.h"

bool Server_Manager_Common_LogSystem()
{
    //初始化日志系统线程
    auto pFileLogger = std::make_shared<CFileLogger>();

    if (nullptr == pFileLogger)
    {
        PSS_LOGGER_DEBUG("[CServerManager::Init]pFileLogger new is nullptr.");
        return false;
    }

    pFileLogger->Init();
    AppLogManager::instance()->Init(1, MAX_MSG_THREADQUEUE, GetXmlConfigAttribute(xmlAlertConnect)->MailID);

    if (0 != AppLogManager::instance()->RegisterLog(pFileLogger))
    {
        PSS_LOGGER_DEBUG("[CServerManager::Init]AppLogManager::instance()->RegisterLog error.");
        return false;
    }
    else
    {
        PSS_LOGGER_DEBUG("[CServerManager::Init]AppLogManager is OK.");
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

    //初始化消息处理线程
    App_MessageServiceGroup::instance()->Init(GetXmlConfigAttribute(xmlMessage)->Msg_Thread,
            GetXmlConfigAttribute(xmlMessage)->Msg_MaxQueue,
            GetXmlConfigAttribute(xmlMessage)->Msg_Low_mark);

    return true;
}

bool Server_Manager_Common_IObject(IConnectManager* pConnectManager,
                                   IClientManager* pClientManager,
                                   IFrameCommand* pFrameCommand,
                                   IServerManager* pIServerManager,
                                   ITTyClientManager* pTTyClientManager,
                                   IControlListen* pControlListen)
{
    App_ServerObject::instance()->SetMessageManager(dynamic_cast<IMessageManager*>(App_MessageManager::instance()));
    App_ServerObject::instance()->SetLogManager(dynamic_cast<ILogManager*>(AppLogManager::instance()));
    App_ServerObject::instance()->SetConnectManager(pConnectManager);
    App_ServerObject::instance()->SetPacketManager(dynamic_cast<IPacketManager*>(App_BuffPacketManager::instance()));
    App_ServerObject::instance()->SetClientManager(pClientManager);
    App_ServerObject::instance()->SetModuleMessageManager(dynamic_cast<IModuleMessageManager*>(App_ModuleMessageManager::instance()));
    App_ServerObject::instance()->SetControlListen(pControlListen);
    App_ServerObject::instance()->SetModuleInfo(dynamic_cast<IModuleInfo*>(App_ModuleLoader::instance()));
    App_ServerObject::instance()->SetMessageBlockManager(dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()));
    App_ServerObject::instance()->SetFrameCommand(pFrameCommand);
    App_ServerObject::instance()->SetServerManager(pIServerManager);
    App_ServerObject::instance()->SetTTyClientManager(pTTyClientManager);
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
            PSS_LOGGER_DEBUG("[Server_Manager_Common_Module]LoadModule ({0})is error.",
                       GetXmlConfigAttribute(xmlModuleInfos)->vec[i].szModuleName);
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
        PSS_LOGGER_DEBUG("[Server_Manager_Common_Addr]({2})set_address error[{0}:{1}].", pIP, u2Port, errno);
        return false;
    }

    return true;
}
