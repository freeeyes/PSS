// Base.cpp : һ��TCP�Ĳ�����
//����PurenessScopeServer�Ĳ��Ժ�ʹ��
//���ǵ�����˳��static CBaseCommand��Ϊָ�룬������֤ACE��ȫ�ֶ�������˳��һ�¡�
//add by freeeyes
//2011-09-20

#include "BaseCommand.h"
#include "IObject.h"

#include "ace/svc_export.h"

static const char* g_szDesc      = "����TCPģ��";       //ģ�����������
static const char* g_szName      = "TCPTest";           //ģ�������
static const char* g_szModuleKey = "BaseTCP";           //ģ���Key

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

        //���԰�ָ���ļ����˿�
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

    //��ʾ�����˿���Ϣ
    uint32 u4ListenCount = pServerObject->GetControlListen()->GetListenCount();

    for (uint32 i = 0; i < u4ListenCount; i++)
    {
        _ControlInfo objControlInfo;
        pServerObject->GetControlListen()->ShowListen(i, objControlInfo);
        PSS_LOGGER_INFO("[Base LoadModuleData] Listen IP={0}, Pord={1}.", objControlInfo.m_strListenIP, objControlInfo.m_u4Port);
    }

    //��ʾ��ܹ����߳�ID
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
    //ж�ز�������Զ����ò�����գ�����Ҫ���ֶ�pMessageManager->DelClientCommand
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

//����ģ���ĵ��ýӿ�
int DoModuleMessage(uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket)
{
    PSS_LOGGER_INFO("[DoModuleMessage] u2CommandID={0}, size={1}, return={2}.",
               u2CommandID,
               pBuffPacket->GetPacketLen(),
               pReturnBuffPacket->GetPacketLen());
    return 0;
}


//�������ʹ�ã����ڿ�ܶ�ʱѲ����״̬
//Ĭ�����ﷵ��true���������Ҫ�Կ���ڲ�ϸ������ء�
//��������ʵ��֮�����ݴ���Ĳ�ͬ������false��ͬʱ������ErrorID
//��ܻ����������ã������ʼ���ָ��������
bool GetModuleState(uint32& u4ErrorID)
{
    PSS_LOGGER_INFO("[GetModuleState] u4ErrorID={0}.",
               u4ErrorID);
    return true;
}

