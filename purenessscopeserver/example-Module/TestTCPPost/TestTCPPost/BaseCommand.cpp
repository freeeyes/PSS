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

    /*
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] WorkThreadID=%d, ClientID=%d, Cmd=%04x.\n",
        pMessage->GetMessageBase()->m_u4WorkThreadID,
        pMessage->GetMessageBase()->m_u4ConnectID,
        pMessage->GetMessageBase()->m_u2Cmd));
    */

    MESSAGE_FUNCTION_BEGIN(pMessage->GetMessageBase()->m_u2Cmd);
    MESSAGE_FUNCTION(CLIENT_LINK_CONNECT,     Do_Connect,           pMessage);
    MESSAGE_FUNCTION(CLIENT_LINK_CDISCONNET,  Do_DisConnect,        pMessage);
    MESSAGE_FUNCTION(CLINET_LINK_SENDTIMEOUT, Do_ClientSendTimeout, pMessage);
    MESSAGE_FUNCTION(COMMAND_BASE,            Do_PostBase,          pMessage);
    MESSAGE_FUNCTION_END;

    /*
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] WorkThreadID=%d, ClientID=%d, Cmd=%04x End.\n",
        pMessage->GetMessageBase()->m_u4WorkThreadID,
        pMessage->GetMessageBase()->m_u4ConnectID,
        pMessage->GetMessageBase()->m_u2Cmd));
    */

    return 0;
}

void CBaseCommand::InitServer(const char* pModuleName)
{
    //��õ�ǰ�������ж��ٸ������߳�
    //���ݹ����̴߳�����Ӧ��map�����������Ϳ�ܱ���һ��
    //�������Ͳ����ж�������������ƣ���Ϊ�����߳��Ǹ���ConnectIDȡ���
    //ֻҪ�ϲ��㷨һ�£��Ͳ���Ҫ�߳����Ŀ���
    int nWorkThreadCount = m_pServerObject->GetMessageManager()->GetWorkThreadCount();

    //����չʾ�ӿ���л�����ò���Ĳ��������������Բ��������������
    const char* pParam = m_pServerObject->GetModuleInfo()->GetModuleParam(pModuleName);
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::InitServer] pParam=%s.\n", pParam));

    m_objProxyManager.Init(nWorkThreadCount);

}

void CBaseCommand::AddClient2Server(uint32 u4ClientID)
{
    m_objProxyManager.AddClient2Server(u4ClientID, m_pServerObject);
}

void CBaseCommand::DelClient2Server(uint32 u4ClientID)
{
    m_objProxyManager.DelClient2Server(u4ClientID);
}

CPostServerData* CBaseCommand::GetClient2Server_ServerID(uint32 u4ClientID)
{
    return m_objProxyManager.GetClient2Server_ServerID(u4ClientID);
}

int CBaseCommand::Do_Connect(IMessage* pMessage)
{
    AddClient2Server(pMessage->GetMessageBase()->m_u4ConnectID);
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] TCPPOST CLIENT_LINK_CONNECT OK.\n"));
    return 0;
}

int CBaseCommand::Do_DisConnect(IMessage* pMessage)
{
    DelClient2Server(pMessage->GetMessageBase()->m_u4ConnectID);
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));

    return 0;
}

int CBaseCommand::Do_ClientSendTimeout(IMessage* pMessage)
{
    if(NULL != pMessage)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_ClientSendTimeout] ClientSendTimeout.\n"));
    }

    return 0;
}

int CBaseCommand::Do_PostBase(IMessage* pMessage)
{
    _PacketInfo objPacketInfo;
    pMessage->GetPacketBody(objPacketInfo);

    //OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] TcpPostTest CommandID = %d.\n", COMMAND_BASE));

    //���õ�ǰ�������ݵ�ConnectID�������յ�Զ�̻�Ӧ��Ϣ����
    //m_pPostServerData1->SetConnectID(pMessage->GetMessageBase()->m_u4ConnectID);
    CPostServerData* pPostServerData = GetClient2Server_ServerID(pMessage->GetMessageBase()->m_u4ConnectID);

    if(NULL != pPostServerData)
    {
        pPostServerData->SendData(objPacketInfo.m_pData, objPacketInfo.m_nDataLen);
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pPostServerData is NULL.\n"));
        m_objProxyManager.Display(pMessage->GetMessageBase()->m_u4ConnectID);
    }

    return 0;
}