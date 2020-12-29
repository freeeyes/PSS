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

int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag, IBuffPacket* pSendBuffPacket)
{
    //__ENTER_FUNCTION���Զ�����ĺ���׷��try�ĺ꣬���ڲ�׽���������ʹ��__THROW_FUNCTION()�׳���Ĵ���
    //����__THROW_FUNCTION("hello"); �����Զ���Log�ļ����µ�assert.log��¼�ļ�������������������������
    //�Ƽ���ʹ������꣬��סһ��Ҫ�ں����Ľ���ʹ��__LEAVE_FUNCTION����__LEAVE_FUNCTION_WITHRETURN��֮��ԡ�
    //__LEAVE_FUNCTION_WITHRETURN��__LEAVE_FUNCTION��ͬ��ǰ�߿���֧�ֺ����˳���ʱ�򷵻�һ������
    //������������Ƿ���һ��int
    //��ôĩβ����__LEAVE_FUNCTION_WITHRETURN(0); 0���Ƿ��ص�int����Ȼ��Ҳ֧�ֱ�����ͱ��������忴���Լ��ĺ�����
    //��������������κα����������ʹ��__LEAVE_FUNCTION���ɡ�
    __ENTER_FUNCTION();

    //DO_TRACE("./", pMessage->GetMessageBase()->m_szTraceID);

    if(m_pServerObject == NULL)
    {
        PSS_LOGGER_INFO("[CBaseCommand::DoMessage] m_pServerObject is NULL({0}).", bDeleteFlag);
        return -1;
    }

    if(pMessage == NULL)
    {
        PSS_LOGGER_INFO("[CBaseCommand::DoMessage] pMessage is NULL.");
        return -1;
    }

    //�������ӽ�����Ϣ
    MESSAGE_FUNCTION_BEGIN(pMessage->GetMessageBase()->m_u2Cmd);
    MESSAGE_FUNCTION(CLIENT_LINK_CONNECT,     Do_Connect,           pMessage, pSendBuffPacket);
    MESSAGE_FUNCTION(CLIENT_LINK_CDISCONNET,  Do_DisConnect,        pMessage, pSendBuffPacket);
    MESSAGE_FUNCTION(CLINET_LINK_SENDTIMEOUT, Do_ClientSendTimeout, pMessage, pSendBuffPacket);
    MESSAGE_FUNCTION(CLIENT_LINK_SENDOK,      Do_ClientSendOk,      pMessage, pSendBuffPacket);
    MESSAGE_FUNCTION(COMMAND_BASE,            Do_Base,              pMessage, pSendBuffPacket);
    MESSAGE_FUNCTION(COMMAND_TESTREPLY,       Do_ReplyTest,         pMessage, pSendBuffPacket);
    MESSAGE_FUNCTION(COMMAND_TESTREPLY_SYNC,  Do_ReplyTest_Sync,    pMessage, pSendBuffPacket);
    MESSAGE_FUNCTION_END;

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}

int CBaseCommand::Do_Connect(IMessage* pMessage, IBuffPacket* pSendBuffPacket)
{
    PSS_UNUSED_ARG(pSendBuffPacket);
    PSS_LOGGER_INFO("[CBaseCommand::Do_Connect] ({0})TCP CLIENT_LINK_CONNECT OK.", pMessage->GetMessageBase()->m_u4ConnectID);

    return 0;
}

int CBaseCommand::Do_DisConnect(IMessage* pMessage, IBuffPacket* pSendBuffPacket)
{
    PSS_UNUSED_ARG(pSendBuffPacket);
    //�������ӶϿ��¼�
    PSS_LOGGER_INFO("[CBaseCommand::Do_DisConnect]({0})CLIENT_LINK_CDISCONNET OK.", pMessage->GetMessageBase()->m_u4ConnectID);

    return 0;
}

int CBaseCommand::Do_ClientSendTimeout(IMessage* pMessage, IBuffPacket* pSendBuffPacket)
{
    PSS_UNUSED_ARG(pSendBuffPacket);
    //������������Ϳͻ����������ӳ�����ֵ���¼�
    PSS_LOGGER_INFO("[CBaseCommand::Do_DisConnect]({0})CLINET_LINK_SNEDTIMEOUT OK.", pMessage->GetMessageBase()->m_u4ConnectID);

    return 0;
}

int CBaseCommand::Do_Base(IMessage* pMessage, IBuffPacket* pSendBuffPacket)
{
    //m_pServerObject->GetLogManager()->WriteToMail(LOG_SYSTEM, 1, "�����ʼ�", "����");
    PSS_UNUSED_ARG(pSendBuffPacket);

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    //���Լ�¼��������־
    //m_pServerObject->GetLogManager()->WriteLogBinary(LOG_SYSTEM, BodyPacket.m_pData, BodyPacket.m_nDataLen);

    shared_ptr<IBuffPacket> pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
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

    CSend_Param objSendParam;

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������(�첽����)
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID,
                u2PostCommandID,
                pResponsesPacket,
                objSendParam);
    }
    else
    {
        PSS_LOGGER_INFO("[CBaseCommand::DoMessage] m_pConnectManager = NULL");
    }

    return 0;
}

void CBaseCommand::ReadIniFile(const char* pIniFileName)
{
    if(NULL == pIniFileName)
    {
        return;
    }
}

int CBaseCommand::Do_ClientSendOk(IMessage* pMessage, IBuffPacket* pSendBuffPacket)
{
    ACE_UNUSED_ARG(pSendBuffPacket);

    //�������ݷ��ͳɹ��¼�
    _PacketInfo HeadPacket;
    pMessage->GetPacketHead(HeadPacket);
    int nMessageID = 0;
    memcpy_safe(HeadPacket.m_pData, sizeof(int),(char* )&nMessageID, sizeof(int));

    return 0;
}

int CBaseCommand::Do_ReplyTest(IMessage* pMessage, IBuffPacket* pSendBuffPacket)
{
	_PacketInfo BodyPacket;
	pMessage->GetPacketBody(BodyPacket);

	SendClient(BodyPacket, COMMAND_TESTREPLY, pMessage->GetMessageBase()->m_u4ConnectID, false, pSendBuffPacket);
	return 0;
}

int CBaseCommand::Do_ReplyTest_Sync(IMessage* pMessage, IBuffPacket* pSendBuffPacket)
{
    __ENTER_FUNCTION();
    ACE_UNUSED_ARG(pSendBuffPacket);

    //�첽����
    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    shared_ptr<IBuffPacket> pCurrSendPacket= m_pServerObject->GetPacketManager()->Create();

    //ƴװ���Ͱ���
    char szSession[32] = { '\0' };
    uint16 u2Version = (short)NET_VERSION;
    uint16 u2Command = pMessage->GetMessageBase()->m_u2Cmd;
    uint32 u4PacketLen = BodyPacket.m_nDataLen;
    char* pBuffer = BodyPacket.m_pData;

    //ƴ����Ϣͷ
    (*pCurrSendPacket) << u2Version;
    (*pCurrSendPacket) << u2Command;
    (*pCurrSendPacket) << u4PacketLen;
    pCurrSendPacket->WriteStream(szSession, 32);

    //ƴ����Ϣ��
    pCurrSendPacket->WriteStream(pBuffer, u4PacketLen);

    //pCurrSendPacket->Close();

    CSend_Param objSendParam;
    //�첽������Ϣ
    m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID,
        pMessage->GetMessageBase()->m_u2Cmd,
        pCurrSendPacket,
        objSendParam);

    return 0;
    __LEAVE_FUNCTION_WITHRETURN(0);
}

// �ظ����ͻ���
int CBaseCommand::SendClient(_PacketInfo BodyPacket, short nCommand, uint32 nConnectId, bool nEncrypt, IBuffPacket* pSendBuffPacket)
{
	__ENTER_FUNCTION();

    PSS_UNUSED_ARG(nConnectId);

	int nRet = 0;
	//ƴװ���Ͱ���
	//printf("Send: [%d][%#x], %s", nConnectId, nCommand, pData.c_str());
	char szSession[32] = { '\0' };
	uint16 u2Version = (short)NET_VERSION;
	uint16 u2Command = nCommand;
	uint32 u4PacketLen = BodyPacket.m_nDataLen;
	char* pBuffer = BodyPacket.m_pData;

	if (nEncrypt) {
		//pBuffer = m_CAESClass.aes_encrypt((char*)pData.c_str(), pKey, pIv, nPacketLen);
	}

    //������ʾ����ͬ������
    //����Ҫ��ʱ���ص����������ǣ����ʹ�ô˷���
    //Ч�����

    //ƴ����Ϣͷ
    (*pSendBuffPacket) << u2Version;
    (*pSendBuffPacket) << u2Command;
    (*pSendBuffPacket) << u4PacketLen;
    pSendBuffPacket->WriteStream(szSession, 32);

    //ƴ����Ϣ��
    pSendBuffPacket->WriteStream(pBuffer, u4PacketLen);

    //����ʲô�����ô������ݰ��ᱻͬ�����ͻ�ȥ

	return nRet;
	__LEAVE_FUNCTION_WITHRETURN(0);
}
