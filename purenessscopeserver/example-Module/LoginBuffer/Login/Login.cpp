// 支持共享内存缓冲的登陆服务器插件
// 此插件演示了两块共享内存，用户验证和用户信息。
// 在不同模式下的缓冲接口，提供以共享内存为基础的先入先出环形结构
//add by freeeyes
//2013-10-25

#include "BaseCommand.h"
#include "IObject.h"

#include "ace/svc_export.h"

static const char* g_szDesc      = "Login缓冲插件";       //模块的描述文字
static const char* g_szName      = "Login缓冲插件";       //模块的名字
static const char* g_szModuleKey = "BaseLogin";           //模块的Key

#ifdef WIN32
#ifdef TEST_TCP_BUILD_LOGIN_DLL
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif
#else
#define DECLDIR ACE_Svc_Export
#endif

extern "C"
{
    DECLDIR int LoadModuleData(CServerObject* pServerObject);
    DECLDIR int UnLoadModuleData();
    DECLDIR const char* GetDesc();
    DECLDIR const char* GetName();
    DECLDIR const char* GetModuleKey();
    DECLDIR int DoModuleMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);
    DECLDIR bool GetModuleState(uint32& u4ErrorID);

}

static CBaseCommand g_BaseCommand;
CServerObject*      g_pServerObject = NULL;

int LoadModuleData(CServerObject* pServerObject)
{
    g_pServerObject = pServerObject;
    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] Begin.\n"));

    if(g_pServerObject != NULL)
    {
        g_BaseCommand.SetServerObject(pServerObject);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pServerObject is NULL.\n"));
    }

    IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();

    if(NULL != pMessageManager)
    {
        pMessageManager->AddClientCommand(COMMAND_LOGIN, &g_BaseCommand, g_szName);
        pMessageManager->AddClientCommand(COMMAND_LOGOUT, &g_BaseCommand, g_szName);
        pMessageManager->AddClientCommand(COMMAND_USERINFO, &g_BaseCommand, g_szName);
        pMessageManager->AddClientCommand(COMMAND_SET_USERINFO, &g_BaseCommand, g_szName);
        pMessageManager->AddClientCommand(CLIENT_LINK_CONNECT, &g_BaseCommand, g_szName);
        pMessageManager->AddClientCommand(CLIENT_LINK_CDISCONNET, &g_BaseCommand, g_szName);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pMessageManager = NULL.\n"));
    }

    g_BaseCommand.InitUserList();
    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] End.\n"));

    return 0;
}

int UnLoadModuleData()
{
    OUR_DEBUG((LM_INFO, "[Base UnLoadModuleData] Begin.\n"));

    if(g_pServerObject != NULL)
    {
        IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();

        if(NULL != pMessageManager)
        {
            pMessageManager->DelClientCommand(COMMAND_LOGIN, &g_BaseCommand);
            pMessageManager->DelClientCommand(COMMAND_LOGOUT, &g_BaseCommand);
            pMessageManager->DelClientCommand(COMMAND_USERINFO, &g_BaseCommand);
            pMessageManager->DelClientCommand(COMMAND_SET_USERINFO, &g_BaseCommand);
            pMessageManager->DelClientCommand(CLIENT_LINK_CONNECT, &g_BaseCommand);
            pMessageManager->DelClientCommand(CLIENT_LINK_CDISCONNET, &g_BaseCommand);
            pMessageManager = NULL;
        }
    }

    g_BaseCommand.ClearUserList();
    OUR_DEBUG((LM_INFO, "[Base UnLoadModuleData] End.\n"));
    return 0;
}

const char* GetDesc()
{
    return g_szDesc;
}

const char* GetName()
{
    return g_szName;
}

const char* GetModuleKey()
{
    return g_szModuleKey;
}

//用于模块间的调用接口
int DoModuleMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket)
{
    OUR_DEBUG((LM_INFO, "[DoModuleMessage] u2CommandID=%d, size=%d, return=%d.\n",
               u2CommandID,
               pBuffPacket->GetPacketLen(),
               pReturnBuffPacket->GetPacketLen()));
    return 0;
}

//交给框架使用，用于框架定时巡检插件状态
//默认这里返回true，如果你需要对框架内部细节做监控。
//请在这里实现之，根据错误的不同，返回false的同时，设置ErrorID
//框架会根据这个设置，发送邮件给指定的邮箱
bool GetModuleState(uint32& u4ErrorID)
{
    OUR_DEBUG((LM_INFO, "[GetModuleState] u4ErrorID=%d.\n",
               u4ErrorID));
    return true;
}


