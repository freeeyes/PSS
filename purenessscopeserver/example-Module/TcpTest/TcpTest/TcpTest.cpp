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
    DECLDIR int DoModuleMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);
    DECLDIR bool GetModuleState(uint32& u4ErrorID);
}

CBaseCommand*        g_BaseCommand   = NULL;
CServerObject*       g_pServerObject = NULL;

int LoadModuleData(CServerObject* pServerObject)
{
    g_pServerObject = pServerObject;

    if(NULL != g_BaseCommand)
    {
        SAFE_DELETE(g_BaseCommand);
    }

    g_BaseCommand = new CBaseCommand();

    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] Begin.\n"));

    if(g_pServerObject != NULL)
    {
        g_BaseCommand->SetServerObject(pServerObject);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pServerObject is NULL.\n"));
    }

    IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();

    if(NULL != pMessageManager)
    {
        pMessageManager->AddClientCommand(COMMAND_BASE, g_BaseCommand, g_szName);

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
        OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pMessageManager = NULL.\n"));
    }

    //��ʾ�����˿���Ϣ
    uint32 u4ListenCount = pServerObject->GetControlListen()->GetListenCount();

    for (uint32 i = 0; i < u4ListenCount; i++)
    {
        _ControlInfo objControlInfo;
        pServerObject->GetControlListen()->ShowListen(i, objControlInfo);
        OUR_DEBUG((LM_INFO, "[Base LoadModuleData] Listen IP=%s, Pord=%d.\n", objControlInfo.m_szListenIP, objControlInfo.m_u4Port));
    }

    //��ʾ��ܹ����߳�ID
    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] *********************************.\n"));

    uint32 u4WorkThread = pMessageManager->GetWorkThreadCount();
    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] WorkThreadCount=%d.\n", u4WorkThread));

    for(uint32 u4Index = 0; u4Index < u4WorkThread; u4Index++)
    {
        OUR_DEBUG((LM_INFO, "[Base LoadModuleData] WorkThreadID=%d.\n", pMessageManager->GetWorkThreadByIndex(u4Index)));
    }

    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] *********************************.\n"));

    g_BaseCommand->ReadIniFile(pServerObject->GetModuleInfo()->GetModuleParam(g_szName));

    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] End.\n"));

    return 0;
}

int UnLoadModuleData()
{
    //ж�ز�������Զ����ò�����գ�����Ҫ���ֶ�pMessageManager->DelClientCommand
    OUR_DEBUG((LM_INFO, "[Base UnLoadModuleData] Begin.\n"));

    SAFE_DELETE(g_BaseCommand);

    OUR_DEBUG((LM_INFO, "[Base UnLoadModuleData] End.\n"));
    return 0;
}

int InitModule(CServerObject* pServerObject)
{
    OUR_DEBUG((LM_INFO, "[InitModule] Begin.\n"));
    ACE_UNUSED_ARG(pServerObject);
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
int DoModuleMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket)
{
    OUR_DEBUG((LM_INFO, "[DoModuleMessage] u2CommandID=%d, size=%d, return=%d.\n",
               u2CommandID,
               pBuffPacket->GetPacketLen(),
               pReturnBuffPacket->GetPacketLen()));
    return 0;
}


//�������ʹ�ã����ڿ�ܶ�ʱѲ����״̬
//Ĭ�����ﷵ��true���������Ҫ�Կ���ڲ�ϸ������ء�
//��������ʵ��֮�����ݴ���Ĳ�ͬ������false��ͬʱ������ErrorID
//��ܻ����������ã������ʼ���ָ��������
bool GetModuleState(uint32& u4ErrorID)
{
    OUR_DEBUG((LM_INFO, "[GetModuleState] u4ErrorID=%d.\n",
               u4ErrorID));
    return true;
}

