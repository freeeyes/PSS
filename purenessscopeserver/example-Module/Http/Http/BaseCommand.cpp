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
    if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CONNECT)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CONNECT OK.\n"));
        return 0;
    }

    //�������ӶϿ���Ϣ
    if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CDISCONNET)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));
        return 0;
    }

    //����������Ϣ
    if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_HTTP_DATA)
    {
        DoMessage_HttpData(pMessage, bDeleteFlag);
    }

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}

int CBaseCommand::DoMessage_HttpData(IMessage* pMessage, bool& bDeleteFlag)
{
    if(NULL == pMessage)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage_HttpData] pMessage is NULL(%d).\n", bDeleteFlag));
    }

    //������յ����������ݣ�������������
    char szReturnData[MAX_BUFF_500] = {'\0'};

    int nLength = ACE_OS::strlen(RETURN_HTTP_HTML);

    sprintf_safe(szReturnData, MAX_BUFF_500, "HTTP/1.1 200 OK\r\nServer: PSS\r\nContent-Length: %d\r\nContent-Type: text/html\r\n\r\n%s", nLength, RETURN_HTTP_HTML);

    uint16 u2PostCommandID = COMMAND_RETURN_HTTP_DATA;

    //������֤��
    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������
        //m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, szReturnData,
        //        (uint32)ACE_OS::strlen(szReturnData), SENDMESSAGE_JAMPNOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_SELF_RECYC);

        char* ptrReturnData = szReturnData;
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID,
                ptrReturnData,
                (uint32)ACE_OS::strlen(szReturnData),
                SENDMESSAGE_JAMPNOMAL,
                u2PostCommandID,
                PACKET_SEND_IMMEDIATLY,
                PACKET_IS_SELF_RECYC);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage_HandIn] m_pConnectManager = NULL.\n"));
    }

    return 0;
}

