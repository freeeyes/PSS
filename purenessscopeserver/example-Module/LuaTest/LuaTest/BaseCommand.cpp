#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
    m_pServerObject = NULL;
    m_nCount        = 0;
}

CBaseCommand::~CBaseCommand(void)
{
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

    if(m_pServerObject == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] m_pServerObject is NULL(%d).\n", bDeleteFlag));
        return -1;
    }

    if(pMessage == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pMessage is NULL.\n"));
        return -1;
    }

    //�������ӽ�����Ϣ
    MESSAGE_FUNCTION_BEGIN(pMessage->GetMessageBase()->m_u2Cmd);
    MESSAGE_FUNCTION(CLIENT_LINK_CONNECT,     Do_Connect,           pMessage);
    MESSAGE_FUNCTION(CLIENT_LINK_CDISCONNET,  Do_DisConnect,        pMessage);
    MESSAGE_FUNCTION(CLINET_LINK_SENDTIMEOUT, Do_ClientSendTimeout, pMessage);
    MESSAGE_FUNCTION(COMMAND_BASE,            Do_Base,              pMessage);
    MESSAGE_FUNCTION_END;

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}

int CBaseCommand::Do_Connect(IMessage* pMessage)
{
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Connect] CLIENT_LINK_CONNECT OK.\n"));

    //�жϵ�ǰ���������Ƿ�Խ��2000��
    int nConnectCount = m_pServerObject->GetConnectManager()->GetCount();

    if(nConnectCount > 2000)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Connect]connect count is more(%d).\n", nConnectCount));
    }

    return 0;
}

int CBaseCommand::Do_DisConnect(IMessage* pMessage)
{
    //�������ӶϿ��¼�
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_DisConnect](%d)CLIENT_LINK_CDISCONNET OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));

    return 0;
}

int CBaseCommand::Do_ClientSendTimeout(IMessage* pMessage)
{
    //������������Ϳͻ����������ӳ�����ֵ���¼�
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_DisConnect](%d)CLINET_LINK_SNEDTIMEOUT OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));

    return 0;
}

int CBaseCommand::Do_Base(IMessage* pMessage)
{
    //OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));
    //m_pServerObject->GetLogManager()->WriteToMail(LOG_SYSTEM, 1, "�����ʼ�", "����");

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return -1;
    }

    //�����������
    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_BASE;

    //����Lua�Ľӿڲ���������
    Do_Execute_Lua_Function(pMessage, pResponsesPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID,
                pResponsesPacket,
                SENDMESSAGE_JAMPNOMAL,
                u2PostCommandID,
                PACKET_SEND_IMMEDIATLY,
                PACKET_IS_FRAMEWORK_RECYC);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }

    return 0;
}

int CBaseCommand::Do_Execute_Lua_Function(IMessage* pMessage, IBuffPacket* pSendBuffPacket)
{
    _ParamData* pSendParam1   = NULL;
    _ParamData* pSendParam2   = NULL;
    _ParamData* pSendParam3   = NULL;
    _ParamData* pSendParam4   = NULL;
    _ParamData* pSendParamOut = NULL;

    int nSendLength = 0;
    char szSendBuffData[MAX_BUFF_50] = {'\0'};

    pSendParam1   = new _ParamData();
    pSendParam2   = new _ParamData();
    pSendParam3   = new _ParamData();
    pSendParam4   = new _ParamData();
    pSendParamOut = new _ParamData();

    CParamGroup objIn;
    CParamGroup objOut;

    objIn.NeedRetrieve(false);
    objOut.NeedRetrieve(false);

    int nSendIndex  = 1;
    int nLuaSendLen = MAX_BUFF_50;
    pSendParam1->SetParam((char* )szSendBuffData, "void", sizeof(int));
    pSendParam2->SetParam((char* )&nLuaSendLen, "int", sizeof(int));
    pSendParam3->SetParam((char* )&pMessage->GetMessageBase()->m_u4WorkThreadID, "int", sizeof(int));
    pSendParam4->SetParam((char* )&nSendIndex, "int", sizeof(int));

    pSendParamOut->SetParam((char* )&nSendLength, "int", sizeof(int));

    objIn.Push(pSendParam1);
    objIn.Push(pSendParam2);
    objIn.Push(pSendParam3);
    objIn.Push(pSendParam4);
    objOut.Push(pSendParamOut);

    m_objLuaFn.CallFileFn("LuaTest_SendData", objIn, objOut);

    ACE_OS::memcpy(&nSendLength, pSendParamOut->GetParam(), sizeof(int));

    delete pSendParam1;
    delete pSendParam2;
    delete pSendParam3;
    delete pSendParam4;
    delete pSendParamOut;

    if(nSendLength > 0)
    {
        pSendBuffPacket->WriteStream(szSendBuffData, nSendLength);
    }

    return nSendLength;
}

void CBaseCommand::Init()
{
    //��ʼ�����õ�Lua�ļ�
    m_objLuaFn.InitClass();
    bool blState = m_objLuaFn.LoadLuaFile(LUA_FILE);

    if(blState == true)
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::Init] LoadLuaFile ok.\n"));
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::Init] LoadLuaFile fail.\n"));
    }
}
