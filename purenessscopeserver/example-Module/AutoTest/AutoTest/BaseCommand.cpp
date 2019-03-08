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
    if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_AUTOTEST_HEAD)
    {
        Do_Head(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_AUTOTEST_NOHEAD)
    {
        Do_NoHead(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_AUTOTEST_HEADBUFF)
    {
        Do_HeadBuff(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_AUTOTEST_NOHEADBUFF)
    {
        Do_NoHeadBuff(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_AUTOTEST_LOGDATA)
    {
        Do_LogData(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_AUTOTEST_WORKTIMEOUT)
    {
        Do_SleepWorkThread(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_AUTOTEST_WORKAI)
    {
        Do_TimeoutWordThread(pMessage);
    }

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}

bool CBaseCommand::Do_Head(IMessage* pMessage)
{
    uint64     u8ClientTime = 0;

    //OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return false;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    (*pBodyPacket) >> u8ClientTime;

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_AUTOTEST_RETUEN_HEAD;

    (*pResponsesPacket) << u2PostCommandID;
    (*pResponsesPacket) << u8ClientTime;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }

    return true;
}

bool CBaseCommand::Do_NoHead(IMessage* pMessage)
{
    //OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return false;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_AUTOTEST_RETUEN_NOHEAD;

    //����ԭ���
    (*pResponsesPacket) << (uint32)BodyPacket.m_nDataLen;
    (*pResponsesPacket) << u2PostCommandID;
    pResponsesPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_JAMPNOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }

    return true;
}

bool CBaseCommand::Do_HeadBuff(IMessage* pMessage)
{
    uint64     u8ClientTime = 0;

    //OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return false;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    (*pBodyPacket) >> u8ClientTime;

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_AUTOTEST_RETUEN_HEADBUFF;

    (*pResponsesPacket) << u2PostCommandID;
    (*pResponsesPacket) << u8ClientTime;

    IBuffPacket* pResponsesPacket1 = m_pServerObject->GetPacketManager()->Create();

    (*pResponsesPacket1) << u2PostCommandID;
    (*pResponsesPacket1) << u8ClientTime;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, PACKET_SEND_CACHE, PACKET_IS_FRAMEWORK_RECYC);

        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket1, SENDMESSAGE_NOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket1);
    }

    return true;
}

bool CBaseCommand::Do_NoHeadBuff(IMessage* pMessage)
{
    //OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return false;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_AUTOTEST_RETUEN_NOHEADBUFF;

    //����ԭ���
    (*pResponsesPacket) << (uint32)10;
    (*pResponsesPacket) << (uint16)u2PostCommandID;
    (*pResponsesPacket) << (uint32)32;

    IBuffPacket* pResponsesPacket1 = m_pServerObject->GetPacketManager()->Create();
    (*pResponsesPacket1) << (uint32)64;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_JAMPNOMAL, u2PostCommandID, PACKET_SEND_CACHE, PACKET_IS_FRAMEWORK_RECYC);

        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket1, SENDMESSAGE_JAMPNOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket1);
    }

    return true;
}

bool CBaseCommand::Do_LogData(IMessage* pMessage)
{
    uint32     u4Index      = 0;

    //OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return false;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    (*pBodyPacket) >> u4Index;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    //д����־
    m_pServerObject->GetLogManager()->WriteLog(LOG_SYSTEM, "[Do_LogData]LogData nIdex=%d.", u4Index);

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_AUTOTEST_RETUEN_LOGDATA;

    (*pResponsesPacket) << u2PostCommandID;
    (*pResponsesPacket) << (uint32)0;

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }

    return true;
}

bool CBaseCommand::Do_SleepWorkThread(IMessage* pMessage)
{
    uint32     u4Index      = 0;

    //OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return false;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    (*pBodyPacket) >> u4Index;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    m_pServerObject->GetLogManager()->WriteLog(LOG_SYSTEM, "[Do_SleepWorkThread]LogData nIdex=%d.", u4Index);

    //����ǵ�һ��������˯10��
    if(u4Index == 0)
    {
        //��˯10��
        ACE_Time_Value tvSleep(10, 0);
        ACE_OS::sleep(tvSleep);
    }


    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_AUTOTEST_RETURN_WORKTIMEOUT;

    (*pResponsesPacket) << u2PostCommandID;
    (*pResponsesPacket) << (uint32)0;

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }

    return true;
}

bool CBaseCommand::Do_TimeoutWordThread( IMessage* pMessage )
{
    uint16     u2CommandID  = 0;
    uint32     u4Index      = 0;

    //OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return false;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    (*pBodyPacket) >> u2CommandID;
    (*pBodyPacket) >> u4Index;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    m_pServerObject->GetLogManager()->WriteLog(LOG_SYSTEM, "[Do_SleepWorkThread]LogData nIdex=%d.", u4Index);

    //��˯1��
    ACE_Time_Value tvSleep(0, 100000);
    ACE_OS::sleep(tvSleep);

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_AUTOTEST_RETURN_WORKAI;

    (*pResponsesPacket) << u2PostCommandID;
    (*pResponsesPacket) << (uint32)0;

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }

    return true;
}
