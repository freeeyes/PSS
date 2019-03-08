// Base.cpp : һ��TCP�Ĳ�����
//����PurenessScopeServer�Ĳ��Ժ�ʹ��
//add by freeeyes
//2011-09-20

#include "BaseCommand.h"
#include "IObject.h"

#include "ace/svc_export.h"

#ifdef WIN32
#ifdef TEST_TCP_BUILD_DLL
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif
#else
#define DECLDIR ACE_Svc_Export
#endif

static const char* g_szDesc      = "kafka Client Logic"; //ģ�����������
static const char* g_szName      = "kafkaClient";        //ģ�������
static const char* g_szModuleKey = "kafkaClient";        //ģ���Key

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

static CBaseCommand* g_pBaseCommand = NULL;
CServerObject*       g_pServerObject = NULL;

int LoadModuleData(CServerObject* pServerObject)
{
    g_pServerObject = pServerObject;
    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] Begin.\n"));

    if(NULL != g_pBaseCommand)
    {
        SAFE_DELETE(g_pBaseCommand);
    }

    g_pBaseCommand = new CBaseCommand();

    if(g_pServerObject != NULL)
    {
        g_pBaseCommand->SetServerObject(pServerObject);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pServerObject is NULL.\n"));
    }

    IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();

    if(NULL != pMessageManager)
    {
        //������ģ�飬����zookeeper�ͻ�����ز���������������ⲿָ��
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pMessageManager = NULL.\n"));
    }

    g_pBaseCommand->Init(pServerObject->GetModuleInfo()->GetModuleParam(g_szName));
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
            //������ģ��
            pMessageManager = NULL;
        }
    }

    g_pBaseCommand->Fini();
    SAFE_DELETE(g_pBaseCommand);
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

//����ģ���ĵ��ýӿ�
int DoModuleMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket)
{
    if(0 == u2CommandID || NULL == pBuffPacket || NULL == pReturnBuffPacket)
    {
        return 1;
    }

    return 0;
}

//�������ʹ�ã����ڿ�ܶ�ʱѲ����״̬
//Ĭ�����ﷵ��true���������Ҫ�Կ���ڲ�ϸ������ء�
//��������ʵ��֮�����ݴ���Ĳ�ͬ������false��ͬʱ������ErrorID
//��ܻ����������ã������ʼ���ָ��������
bool GetModuleState(uint32& u4ErrorID)
{
    ACE_Time_Value tvNow = ACE_OS::gettimeofday();

    u4ErrorID = 0;
    return true;
}


