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

    //读取配置文件，获得当前运行的ID位置
    m_nComsumeIndex = Get_Log_File();

    m_KafkaClient.Set_Start_Offset(m_nComsumeIndex);

    m_KafkaClient.Set_Log_Path(m_szLogPath);

    //连接kafka集群，这里只演示消费者模型，生产者模型同理
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

    //获得文件长度
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
    //__ENTER_FUNCTION是自动给你的函数追加try的宏，用于捕捉错误，你可以使用__THROW_FUNCTION()抛出你的错误
    //比如__THROW_FUNCTION("hello"); 它会自动在Log文件夹下的assert.log记录文件名，函数名，行数，描述。
    //推荐你使用这个宏，记住一定要在函数的结束使用__LEAVE_FUNCTION或者__LEAVE_FUNCTION_WITHRETURN与之配对。
    //__LEAVE_FUNCTION_WITHRETURN和__LEAVE_FUNCTION不同是前者可以支持函数退出的时候返回一个变量
    //比如这个函数是返回一个int
    //那么末尾就是__LEAVE_FUNCTION_WITHRETURN(0); 0就是返回的int，当然，也支持别的类型变量，具体看你自己的函数。
    //如果函数不返回任何变量，你可以使用__LEAVE_FUNCTION即可。
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
    //运行kafka消费者线程
    ACE_Thread_Manager::instance()->spawn(ACE_THR_FUNC(thread_consume), (void*)&m_KafkaClient);
}

