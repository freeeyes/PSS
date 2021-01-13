// Base.cpp : 一个TCP的测试类
//用于PurenessScopeServer的测试和使用
//考虑到析构顺序，static CBaseCommand改为指针，这样保证ACE的全局对象析构顺序一致。
//add by freeeyes
//2011-09-20

#include "BaseCommand.h"
#include "IObject.h"

#include "ace/svc_export.h"

static const char* g_szDesc      = "样例TCP模块";       //模块的描述文字
static const char* g_szName      = "TCPTest";           //模块的名字
static const char* g_szModuleKey = "BaseTCP";           //模块的Key

#ifdef WIN32
#ifdef TEST_TCP_BUILD_DLL
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
	DECLDIR int InitModule(CServerObject* pServerObject);
    DECLDIR int UnLoadModuleData();
    DECLDIR int InitModule(CServerObject* pServerObject);
    DECLDIR const char* GetDesc();
    DECLDIR const char* GetName();
    DECLDIR const char* GetModuleKey();
    DECLDIR int DoModuleMessage(uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket);
    DECLDIR bool GetModuleState(uint32& u4ErrorID);
    DECLDIR void Set_output(shared_ptr<spdlog::logger> logger);
}

shared_ptr<CBaseCommand>   g_BaseCommand   = NULL;
CServerObject*             g_pServerObject = NULL;

int LoadModuleData(CServerObject* pServerObject)
{
    g_pServerObject = pServerObject;

    g_BaseCommand = std::make_shared<CBaseCommand>();

    PSS_LOGGER_INFO("[Base LoadModuleData] Begin.");

    if(g_pServerObject != NULL)
    {
        g_BaseCommand->SetServerObject(pServerObject);
    }
    else
    {
        PSS_LOGGER_INFO("[Base LoadModuleData] pServerObject is NULL.");
    }

    IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();

    if(NULL != pMessageManager)
    {
        pMessageManager->AddClientCommand(COMMAND_BASE, g_BaseCommand, g_szName);
        pMessageManager->AddClientCommand(COMMAND_TESTREPLY, g_BaseCommand, g_szName);
        pMessageManager->AddClientCommand(COMMAND_TESTREPLY_SYNC, g_BaseCommand, g_szName);

        //测试绑定指定的监听端口
        //_ClientIPInfo objClientIPInfo;
        //sprintf_safe(objClientIPInfo.m_szClientIP, MAX_BUFF_50, "0.0.0.0", objClientIPInfo);
        //objClientIPInfo.m_nPort = 10002;
        //pMessageManager->AddClientCommand(COMMAND_BASE, &g_BaseCommand, g_szName);

        pMessageManager->AddClientCommand(CLIENT_LINK_CONNECT, g_BaseCommand, g_szName);
        pMessageManager->AddClientCommand(CLIENT_LINK_CDISCONNET, g_BaseCommand, g_szName);
        pMessageManager->AddClientCommand(CLINET_LINK_SENDTIMEOUT, g_BaseCommand, g_szName);
        pMessageManager->AddClientCommand(CLIENT_LINK_SENDOK, g_BaseCommand, g_szName);
    }
    else
    {
        PSS_LOGGER_INFO("[Base LoadModuleData] pMessageManager = NULL.");
    }

    //显示监听端口信息
    uint32 u4ListenCount = pServerObject->GetControlListen()->GetListenCount();

    for (uint32 i = 0; i < u4ListenCount; i++)
    {
        _ControlInfo objControlInfo;
        pServerObject->GetControlListen()->ShowListen(i, objControlInfo);
        PSS_LOGGER_INFO("[Base LoadModuleData] Listen IP={0}, Pord={1}.", objControlInfo.m_strListenIP, objControlInfo.m_u4Port);
    }

    //显示框架工作线程ID
    PSS_LOGGER_INFO("[Base LoadModuleData] *********************************.");

    uint32 u4WorkThread = pMessageManager->GetWorkThreadCount();
    PSS_LOGGER_INFO("[Base LoadModuleData] WorkThreadCount={0}.", u4WorkThread);

    for(uint32 u4Index = 0; u4Index < u4WorkThread; u4Index++)
    {
        PSS_LOGGER_INFO("[Base LoadModuleData] WorkThreadID={0}.", pMessageManager->GetWorkThreadByIndex(u4Index));
    }

    PSS_LOGGER_INFO("[Base LoadModuleData] *********************************.");

    g_BaseCommand->ReadIniFile(pServerObject->GetModuleInfo()->GetModuleParam(g_szName));

    PSS_LOGGER_INFO("[Base LoadModuleData] End.");

    return 0;
}

int UnLoadModuleData()
{
    //卸载插件，会自动调用插件回收，不需要在手动pMessageManager->DelClientCommand
    PSS_LOGGER_INFO("[Base UnLoadModuleData] Begin.");

    PSS_LOGGER_INFO("[Base UnLoadModuleData] End.");
    return 0;
}

int InitModule(CServerObject* pServerObject)
{
    PSS_LOGGER_INFO("[InitModule] Begin.");
    PSS_UNUSED_ARG(pServerObject);
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
int DoModuleMessage(uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket)
{
    PSS_LOGGER_INFO("[DoModuleMessage] u2CommandID={0}, size={1}, return={2}.",
               u2CommandID,
               pBuffPacket->GetPacketLen(),
               pReturnBuffPacket->GetPacketLen());
    return 0;
}


//交给框架使用，用于框架定时巡检插件状态
//默认这里返回true，如果你需要对框架内部细节做监控。
//请在这里实现之，根据错误的不同，返回false的同时，设置ErrorID
//框架会根据这个设置，发送邮件给指定的邮箱
bool GetModuleState(uint32& u4ErrorID)
{
    PSS_LOGGER_INFO("[GetModuleState] u4ErrorID={0}.",
               u4ErrorID);
    return true;
}

void Set_output(shared_ptr<spdlog::logger> logger)
{
    //设置输出对象
    spdlog::set_default_logger(logger);
}
