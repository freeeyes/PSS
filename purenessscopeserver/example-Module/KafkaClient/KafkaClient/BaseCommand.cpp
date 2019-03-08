#include "BaseCommand.h"

void thread_consume(void* arg)
{
    CKafkaClient* pKafkaClient = (CKafkaClient* )arg;

    pKafkaClient->Recv_Topic_Message();

}

CBaseCommand::CBaseCommand(void)
{
    m_pServerObject = NULL;
    m_nCount        = 0;
    m_nComsumeIndex = 0;
}

CBaseCommand::~CBaseCommand(void)
{
}

void CBaseCommand::Init(const char* pPath)
{
    SetLogPath(pPath);

    //��ȡ�����ļ�����õ�ǰ���е�IDλ��
    m_nComsumeIndex = Get_Log_File();

    m_KafkaClient.Set_Start_Offset(m_nComsumeIndex);

    m_KafkaClient.Set_Log_Path(m_szLogPath);

    //����kafka��Ⱥ������ֻ��ʾ������ģ�ͣ�������ģ��ͬ��
    m_KafkaClient.Connect_Kafka_Server("127.0.0.1:10086", KA_CONSUMER);

    m_KafkaClient.Create_topic("freeeyes");

    Run();
}

int CBaseCommand::Get_Log_File()
{
    FILE* pFile = fopen(m_szLogPath, "r");

    if(NULL == pFile)
    {
        return 0;
    }

    //����ļ�����
    fseek(pFile, 0l, SEEK_END);
    int nSize = ftell(pFile);
    fseek(pFile, 0l, SEEK_SET);

    char* szBuffer = new char[nSize + 1];
    memset(szBuffer, 0, nSize + 1);
    fread(szBuffer, nSize, sizeof(char), pFile);

    int nRet = atoi(szBuffer);
    delete [] szBuffer;

    fclose(pFile);
    return nRet;
}

void CBaseCommand::Fini()
{
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Fini] Begin.\n"));
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Fini] End.\n"));
}

void CBaseCommand::SetServerObject(CServerObject* pServerObject)
{
    m_pServerObject = pServerObject;
}

int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{
    //__ENTER_FUNCTION���Զ�����ĺ���׷��try�ĺ꣬���ڲ�׽���������ʹ��__THROW_FUNCTION()�׳���Ĵ���
    //����__THROW_FUNCTION("hello"); �����Զ���Log�ļ����µ�assert.log��¼�ļ�������������������������
    //�Ƽ���ʹ������꣬��סһ��Ҫ�ں����Ľ���ʹ��__LEAVE_FUNCTION����__LEAVE_FUNCTION_WITHRETURN��֮��ԡ�
    //__LEAVE_FUNCTION_WITHRETURN��__LEAVE_FUNCTION��ͬ��ǰ�߿���֧�ֺ����˳���ʱ�򷵻�һ������
    //������������Ƿ���һ��int
    //��ôĩβ����__LEAVE_FUNCTION_WITHRETURN(0); 0���Ƿ��ص�int����Ȼ��Ҳ֧�ֱ�����ͱ��������忴���Լ��ĺ�����
    //��������������κα����������ʹ��__LEAVE_FUNCTION���ɡ�
    __ENTER_FUNCTION();

    if(NULL == pMessage)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pMessage is NULL(%d).\n", bDeleteFlag));
    }

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}

void CBaseCommand::SetLogPath(const char* pPath)
{
    if(strlen(pPath) == 0)
    {
        sprintf_safe(m_szLogPath, 256, "./comsumre.log");
    }
    else
    {
        sprintf_safe(m_szLogPath, 256, "%s", pPath);
    }
}

void CBaseCommand::Run()
{
    //����kafka�������߳�
    ACE_Thread_Manager::instance()->spawn(ACE_THR_FUNC(thread_consume), (void*)&m_KafkaClient);
}

