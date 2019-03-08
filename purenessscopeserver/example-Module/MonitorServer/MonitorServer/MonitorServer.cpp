// Base.cpp : һ��TCP�Ĳ�����
//����PurenessScopeServer�Ĳ��Ժ�ʹ��
//���ǵ�����˳��static CBaseCommand��Ϊָ�룬������֤ACE��ȫ�ֶ�������˳��һ�¡�
//add by freeeyes
//2011-09-20

#include "TimeEvent.h"
#include "MonitorCommand.h"
#include "IObject.h"

#include "ace/svc_export.h"

static const char* g_szDesc      = "MonitorServer";           //ģ�����������
static const char* g_szName      = "MonitorServer";           //ģ�������
static const char* g_szModuleKey = "MonitorServer";           //ģ���Key

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
    DECLDIR int UnLoadModuleData();
    DECLDIR int InitModule(CServerObject* pServerObject);
    DECLDIR const char* GetDesc();
    DECLDIR const char* GetName();
    DECLDIR const char* GetModuleKey();
    DECLDIR int DoModuleMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);
    DECLDIR bool GetModuleState(uint32& u4ErrorID);
}

CPSSMonitorCommand*     g_MonitorCommand = NULL;
CServerObject*          g_pServerObject  = NULL;
ACE_Event_Handler*      g_pTimeEventTask = NULL;
long                    g_lTimeEventID   = 0;

int LoadModuleData(CServerObject* pServerObject)
{
    g_pServerObject = pServerObject;

    if(NULL != g_MonitorCommand)
    {
        SAFE_DELETE(g_MonitorCommand);
    }

    g_MonitorCommand = new CPSSMonitorCommand();

    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] Begin.\n"));

    if (g_pServerObject != NULL)
    {
        g_MonitorCommand->SetServerObject(pServerObject);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pServerObject is NULL.\n"));
    }

    //ע������
    IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();

    if (NULL != pMessageManager)
    {
        pMessageManager->AddClientCommand(COMMAND_MONITOR_LOGIN, g_MonitorCommand, g_szName);
        pMessageManager->AddClientCommand(COMMAND_MONITOR_DATA, g_MonitorCommand, g_szName);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pServerObject is NULL.\n"));
    }

    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] End.\n"));

    return 0;
}

int UnLoadModuleData()
{
    //ж�ز�������Զ����ò�����գ�����Ҫ���ֶ�pMessageManager->DelClientCommand
    OUR_DEBUG((LM_INFO, "[Base UnLoadModuleData] Begin.\n"));

    //ֹͣ��ʱ��
    g_MonitorCommand->GetServerObject()->GetTimerManager()->cancel(g_lTimeEventID, NULL);

    //ɾ������
    SAFE_DELETE(g_pTimeEventTask);

    //���Message����
    SAFE_DELETE(g_MonitorCommand);

    OUR_DEBUG((LM_INFO, "[Base UnLoadModuleData] End.\n"));
    return 0;
}

int InitModule(CServerObject* pServerObject)
{
    OUR_DEBUG((LM_INFO, "[InitModule] Begin.\n"));
    //ACE_UNUSED_ARG(pServerObject);

    //��ʼ����ȡ����
    char* pParamData = (char* )pServerObject->GetModuleInfo()->GetModuleParam(g_szName);

    if (ACE_OS::strlen(pParamData) > 0)
    {
        if (false == CDataManager::GetInstance()->ParseXmlFile(pParamData))
        {
            OUR_DEBUG((LM_ERROR, "[InitModule]DataManager::GetInstance()->ParseXmlFile(%s) error.\n", pParamData));
        }
        else
        {
            //�����л��ļ������ϴ����еĽ��ˢ���ڴ�
            CDataManager::GetInstance()->UnSerialization();

            //��ʼ����ʱ��
            ACE_NEW_RETURN(g_pTimeEventTask, CTimeEventTask, 0);

            uint32 u4IntervalSecond = CDataManager::GetInstance()->GetTimeInterval();

            g_lTimeEventID = pServerObject->GetTimerManager()->schedule(g_pTimeEventTask,
                             NULL,
                             ACE_OS::gettimeofday() + ACE_Time_Value(u4IntervalSecond),
                             ACE_Time_Value(u4IntervalSecond));

            /*
            //�������кͷ������
            ACE_Time_Value tvSleep(1);
            CDataManager::GetInstance()->AddNodeDate("127.0.0.1", 0, 1024, 10, 100, 100);
            ACE_OS::sleep(tvSleep);
            CDataManager::GetInstance()->AddNodeDate("127.0.0.1", 10, 1024, 20, 120, 120);
            CDataManager::GetInstance()->AddNodeDate("127.0.0.2", 0, 1024, 10, 100, 100);

            CDataManager::GetInstance()->make_index_html();
            CDataManager::GetInstance()->make_detail_html();

            //CDataManager::GetInstance()->Serialization();
            //CDataManager::GetInstance()->UnSerialization();
            */
        }
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[InitModule]pParamData is NULL.\n"));
    }

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

