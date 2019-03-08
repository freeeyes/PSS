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
    MESSAGE_FUNCTION(CLIENT_LINK_CONNECT,       Do_Connect,           pMessage);
    MESSAGE_FUNCTION(CLIENT_LINK_CDISCONNET,    Do_DisConnect,        pMessage);
    MESSAGE_FUNCTION(CLINET_LINK_SENDTIMEOUT,   Do_ClientSendTimeout, pMessage);
    MESSAGE_FUNCTION(COMMAND_LOGIC_LG_LOGIN,    Do_Logic_LG_Login,    pMessage);
    MESSAGE_FUNCTION(COMMAND_LOGIC_ALIVE,       Do_Logic_LG_Alive,    pMessage);
    MESSAGE_FUNCTION_END;

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}

int CBaseCommand::Do_Connect(IMessage* pMessage)
{
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Connect] (%d)TCP CLIENT_LINK_CONNECT OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));
    return 0;
}

int CBaseCommand::Do_DisConnect(IMessage* pMessage)
{
    //�������ӶϿ��¼�
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_DisConnect](%d)CLIENT_LINK_CDISCONNET OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));

    m_listManager.Del_LG_Info_By_ConnectID(pMessage->GetMessageBase()->m_u4ConnectID);

    Do_Logic_All_LG_Key(pMessage, COMMAND_LOGIC_LG_KEY_R);

    return 0;
}

int CBaseCommand::Do_ClientSendTimeout(IMessage* pMessage)
{
    //������������Ϳͻ����������ӳ�����ֵ���¼�
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_DisConnect](%d)CLINET_LINK_SNEDTIMEOUT OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));
    return 0;
}

void CBaseCommand::ReadIniFile(const char* pIniFileName)
{
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::ReadIniFile]pIniFileName=%s.\n", pIniFileName));
    //��ʼ����ط������б�
    m_listManager.ReadList();
}

int CBaseCommand::Do_Logic_LG_Login(IMessage* pMessage)
{
    //�����������ݰ�
    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return -1;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    _PacketInfo HeadPacket;
    pMessage->GetPacketHead(HeadPacket);

    IBuffPacket* pHeadPacket = m_pServerObject->GetPacketManager()->Create();
    pHeadPacket->WriteStream(HeadPacket.m_pData, HeadPacket.m_nDataLen);

    uint16 u2CommandID   = 0;           //������
    uint32 u2Version     = 0;           //Э��汾��
    uint32 u4BodyLen     = 0;           //���峤��
    char   szSession[33] = {'\0'};      //Session�ַ���

    //������ͷ�е����ݰ���
    (*pHeadPacket) >> u2Version;
    (*pHeadPacket) >> u2CommandID;
    (*pHeadPacket) >> u4BodyLen;
    pHeadPacket->WriteStream(szSession, 32);

    m_pServerObject->GetPacketManager()->Delete(pHeadPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    _VCHARS_STR strIP;
    _VCHARS_STR strServerVserion;
    uint32 u4LGID = 0;
    uint32 u4Port = 0;

    (*pBodyPacket) >> u4LGID;
    (*pBodyPacket) >> strIP;
    (*pBodyPacket) >> u4Port;
    (*pBodyPacket) >> strServerVserion;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    m_listManager.Add_LG_Info(pMessage->GetMessageBase()->m_u4ConnectID,
                              u4LGID,
                              strIP.text,
                              u4Port,
                              strServerVserion.text,
                              szSession,
                              u2Version);

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    Do_Logic_All_LG_Key(pMessage, COMMAND_LOGIC_LG_LOGIN_R);

    //�ж��Ƿ����е���Ҫ��صķ��������ѵ�½������ǣ����·�ͬ�������б�
    if(true == m_listManager.Get_All_Target_list_Finish())
    {
        for(uint32 i = 0; i < m_listManager.Get_LG_Count(); i++)
        {
            _LG_Info* pLGInfo = m_listManager.Get_LG_Info_By_Index(i);

            if(NULL != pLGInfo && ACE_OS::strcmp(pLGInfo->m_szMD5, m_listManager.Get_MD5_Data()) != 0)
            {
                //��ֵ���µ�MD5
                sprintf_safe(pLGInfo->m_szMD5, 33, "%s", m_listManager.Get_MD5_Data());

                //MD5��ƥ�䣬��Ҫ֪ͨLGˢ���б�
                Send_Logic_LG_List(pLGInfo->m_u4ConnectID,
                                   pLGInfo->m_szSession,
                                   pLGInfo->m_u2Version);
            }
        }
    }

    return 0;
}

int CBaseCommand::Do_Logic_LG_Alive(IMessage* pMessage)
{
    //����ʲô������������PSS��ܱ���ĳ�ʱ�����ж�
    if(NULL == pMessage)
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::Do_Logic_LG_Alive]pMessage os NULL.\n"));
    }

    return 0;
}

int CBaseCommand::Do_Logic_All_LG_Key(IMessage* pMessage, uint16 u2CommandID)
{
    _PacketInfo HeadPacket;
    pMessage->GetPacketHead(HeadPacket);

    IBuffPacket* pHeadPacket = m_pServerObject->GetPacketManager()->Create();
    pHeadPacket->WriteStream(HeadPacket.m_pData, HeadPacket.m_nDataLen);

    uint32 u2Version     = 0;           //Э��汾��
    uint32 u4BodyLen     = 0;           //���峤��
    char   szSession[33] = {'\0'};      //Session�ַ���

    //������ͷ�е����ݰ���
    (*pHeadPacket) >> u2Version;
    (*pHeadPacket) >> u2CommandID;
    (*pHeadPacket) >> u4BodyLen;
    pHeadPacket->WriteStream(szSession, 32);

    m_pServerObject->GetPacketManager()->Delete(pHeadPacket);

    //Ⱥ�����е�LG KEY������Ϣ
    uint32 u4Count = m_listManager.Get_LG_Count();

    for(uint32 i = 0; i < u4Count; i++)
    {
        _LG_Info* pInfo = m_listManager.Get_LG_Info_By_Index(i);

        if(NULL != pInfo)
        {
            //���������б�
            IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
            uint16 u2PostCommandID = u2CommandID;

            _VCHARS_STR strMD5;
            uint8 u1Len = (uint8)32;
            strMD5.SetData(m_listManager.Get_MD5_Data(), u1Len);

            //��Ϣ�б���
            uint32 u4SendPacketLen = 32 + sizeof(uint8) + sizeof(uint32) + 1;
            (*pResponsesPacket) << u2Version;
            (*pResponsesPacket) << u2PostCommandID;
            (*pResponsesPacket) << u4SendPacketLen; //���ݰ��峤��
            pResponsesPacket->WriteStream(szSession, 32);

            (*pResponsesPacket) << pInfo->m_u4LGID;
            (*pResponsesPacket) << strMD5;

            if(NULL != m_pServerObject->GetConnectManager())
            {
                //����ȫ������
                m_pServerObject->GetConnectManager()->PostMessage(pInfo->m_u4ConnectID,
                        pResponsesPacket,
                        SENDMESSAGE_JAMPNOMAL,
                        u2PostCommandID,
                        PACKET_SEND_IMMEDIATLY,
                        PACKET_IS_FRAMEWORK_RECYC);
            }
            else
            {
                OUR_DEBUG((LM_INFO, "[CBaseCommand::Do_Logic_All_LG_Key] m_pConnectManager = NULL.\n"));
                m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
            }
        }
    }

    return 0;
}

int CBaseCommand::Send_Logic_LG_List(uint32 u4ConnectID, const char* pSession, uint16 u2Version)
{
    //���������б�
    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_LOGIC_LG_LIST_R;
    //�õ������б������Ϣ
    IBuffPacket* pListPacket = m_pServerObject->GetPacketManager()->Create();

    uint32 u4ListCount = 0;
    m_listManager.Get_All_LG_List(pListPacket, u4ListCount);

    //�õ�List MD5
    _VCHARS_STR strMD5;
    uint8 u1MD5Len = (uint8)ACE_OS::strlen(m_listManager.Get_MD5_Data());
    strMD5.SetData(m_listManager.Get_MD5_Data(), u1MD5Len);;

    //��Ϣ�б���
    uint32 u4SendPacketLen = pListPacket->GetPacketLen() + sizeof(u4ListCount)
                             + sizeof(uint8) + u1MD5Len + 1;
    (*pResponsesPacket) << u2Version;
    (*pResponsesPacket) << u2PostCommandID;
    (*pResponsesPacket) << u4SendPacketLen; //���ݰ��峤��
    pResponsesPacket->WriteStream(pSession, 32);

    (*pResponsesPacket) << strMD5;
    (*pResponsesPacket) << u4ListCount;
    pResponsesPacket->WriteStream(pListPacket->GetData(), pListPacket->GetPacketLen());

    m_pServerObject->GetPacketManager()->Delete(pListPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //����ȫ������
        m_pServerObject->GetConnectManager()->PostMessage(u4ConnectID,
                pResponsesPacket,
                SENDMESSAGE_JAMPNOMAL,
                u2PostCommandID,
                PACKET_SEND_IMMEDIATLY,
                PACKET_IS_FRAMEWORK_RECYC);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL.\n"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }

    //m_pServerObject->GetConnectManager()->CloseConnect(pMessage->GetMessageBase()->m_u4ConnectID);

    return 0;
}

int CBaseCommand::Do_Logic_LG_List(IMessage* pMessage)
{
    _PacketInfo HeadPacket;
    pMessage->GetPacketHead(HeadPacket);

    IBuffPacket* pHeadPacket = m_pServerObject->GetPacketManager()->Create();
    pHeadPacket->WriteStream(HeadPacket.m_pData, HeadPacket.m_nDataLen);

    uint16 u2CommandID   = 0;           //������
    uint32 u2Version     = 0;           //Э��汾��
    uint32 u4BodyLen     = 0;           //���峤��
    char   szSession[33] = {'\0'};      //Session�ַ���

    //������ͷ�е����ݰ���
    (*pHeadPacket) >> u2Version;
    (*pHeadPacket) >> u2CommandID;
    (*pHeadPacket) >> u4BodyLen;
    pHeadPacket->WriteStream(szSession, 32);

    m_pServerObject->GetPacketManager()->Delete(pHeadPacket);

    //�����������ݰ�
    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return -1;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    _VCHARS_STR strMD5;

    (*pBodyPacket) >> strMD5;

    //��¼�ͻ���������Դ
    m_pServerObject->GetLogManager()->WriteLog(LOG_SYSTEM, "[Code](%s:%d) is get list(%s).",
            m_pServerObject->GetConnectManager()->GetClientIPInfo(pMessage->GetMessageBase()->m_u4ConnectID).m_szClientIP,
            m_pServerObject->GetConnectManager()->GetClientIPInfo(pMessage->GetMessageBase()->m_u4ConnectID).m_nPort,
            strMD5.text);

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    Send_Logic_LG_List(pMessage->GetMessageBase()->m_u4ConnectID,
                       szSession,
                       u2Version);

    return 0;
}
