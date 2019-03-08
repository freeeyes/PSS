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

    DO_TRACE("./", pMessage->GetMessageBase()->m_szTraceID);

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
    MESSAGE_FUNCTION(CLIENT_LINK_SENDOK,      Do_ClientSendOk,      pMessage);
    MESSAGE_FUNCTION(COMMAND_BASE,            Do_Base,              pMessage);
    MESSAGE_FUNCTION_END;

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}

int CBaseCommand::Do_Connect(IMessage* pMessage)
{
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Connect] (%d)TCP CLIENT_LINK_CONNECT OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));

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
    //OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] TcpTest CommandID = %d", COMMAND_BASE));
    //m_pServerObject->GetLogManager()->WriteToMail(LOG_SYSTEM, 1, "�����ʼ�", "����");

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return -1;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    //��ϢȾɫ����
    DO_TRACE("./", pMessage->GetMessageBase()->m_szTraceID);

    //���Լ�¼��������־
    //m_pServerObject->GetLogManager()->WriteLogBinary(LOG_SYSTEM, BodyPacket.m_pData, BodyPacket.m_nDataLen);

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_BASE;

    //������ݳ�����0����ʲô������
    if (BodyPacket.m_nDataLen == 0)
    {
        //��������ͷ
        (*pResponsesPacket) << (uint32)BodyPacket.m_nDataLen;
    }
    else
    {
        //����ԭ���
        (*pResponsesPacket) << (uint32)BodyPacket.m_nDataLen;
        pResponsesPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);
    }

    //(*pResponsesPacket) << u2PostCommandID;
    //(*pResponsesPacket) << u8ClientTime;
    int nMessageID = 1;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID,
                pResponsesPacket,
                SENDMESSAGE_JAMPNOMAL,
                u2PostCommandID,
                PACKET_SEND_IMMEDIATLY,
                PACKET_IS_FRAMEWORK_RECYC,
                nMessageID);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }

    //m_pServerObject->GetConnectManager()->CloseConnect(pMessage->GetMessageBase()->m_u4ConnectID);

    return 0;
}

void CBaseCommand::ReadIniFile(const char* pIniFileName)
{
    if(NULL == pIniFileName)
    {
        return;
    }
}

int CBaseCommand::Do_ClientSendOk(IMessage* pMessage)
{
    //�������ݷ��ͳɹ��¼�
    _PacketInfo HeadPacket;
    pMessage->GetPacketHead(HeadPacket);
    int nMessageID = 0;
    memcpy_safe(HeadPacket.m_pData, sizeof(int),(char* )&nMessageID, sizeof(int));

    OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage]nMessageID=%d, ConnectID=%d.\n", nMessageID, pMessage->GetMessageBase()->m_u4ConnectID));

    return 0;
}
