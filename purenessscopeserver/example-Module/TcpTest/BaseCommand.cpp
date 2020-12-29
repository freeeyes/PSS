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
    //__ENTER_FUNCTION是自动给你的函数追加try的宏，用于捕捉错误，你可以使用__THROW_FUNCTION()抛出你的错误
    //比如__THROW_FUNCTION("hello"); 它会自动在Log文件夹下的assert.log记录文件名，函数名，行数，描述。
    //推荐你使用这个宏，记住一定要在函数的结束使用__LEAVE_FUNCTION或者__LEAVE_FUNCTION_WITHRETURN与之配对。
    //__LEAVE_FUNCTION_WITHRETURN和__LEAVE_FUNCTION不同是前者可以支持函数退出的时候返回一个变量
    //比如这个函数是返回一个int
    //那么末尾就是__LEAVE_FUNCTION_WITHRETURN(0); 0就是返回的int，当然，也支持别的类型变量，具体看你自己的函数。
    //如果函数不返回任何变量，你可以使用__LEAVE_FUNCTION即可。
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

    //处理链接建立信息
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
    //处理连接断开事件
    PSS_LOGGER_INFO("[CBaseCommand::Do_DisConnect]({0})CLIENT_LINK_CDISCONNET OK.", pMessage->GetMessageBase()->m_u4ConnectID);

    return 0;
}

int CBaseCommand::Do_ClientSendTimeout(IMessage* pMessage, IBuffPacket* pSendBuffPacket)
{
    PSS_UNUSED_ARG(pSendBuffPacket);
    //处理服务器发送客户端数据连接超过阀值的事件
    PSS_LOGGER_INFO("[CBaseCommand::Do_DisConnect]({0})CLINET_LINK_SNEDTIMEOUT OK.", pMessage->GetMessageBase()->m_u4ConnectID);

    return 0;
}

int CBaseCommand::Do_Base(IMessage* pMessage, IBuffPacket* pSendBuffPacket)
{
    //m_pServerObject->GetLogManager()->WriteToMail(LOG_SYSTEM, 1, "测试邮件", "测试");
    PSS_UNUSED_ARG(pSendBuffPacket);

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    //测试记录二进制日志
    //m_pServerObject->GetLogManager()->WriteLogBinary(LOG_SYSTEM, BodyPacket.m_pData, BodyPacket.m_nDataLen);

    shared_ptr<IBuffPacket> pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_BASE;

    //如果数据长度是0，则什么都不做
    if (BodyPacket.m_nDataLen == 0)
    {
        //返回数据头
        (*pResponsesPacket) << (uint32)BodyPacket.m_nDataLen;
    }
    else
    {
        //数据原样奉还
        (*pResponsesPacket) << (uint32)BodyPacket.m_nDataLen;
        pResponsesPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);
    }

    //(*pResponsesPacket) << u2PostCommandID;
    //(*pResponsesPacket) << u8ClientTime;

    CSend_Param objSendParam;

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //发送全部数据(异步发送)
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

    //接受数据发送成功事件
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

    //异步发送
    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    shared_ptr<IBuffPacket> pCurrSendPacket= m_pServerObject->GetPacketManager()->Create();

    //拼装发送包体
    char szSession[32] = { '\0' };
    uint16 u2Version = (short)NET_VERSION;
    uint16 u2Command = pMessage->GetMessageBase()->m_u2Cmd;
    uint32 u4PacketLen = BodyPacket.m_nDataLen;
    char* pBuffer = BodyPacket.m_pData;

    //拼接消息头
    (*pCurrSendPacket) << u2Version;
    (*pCurrSendPacket) << u2Command;
    (*pCurrSendPacket) << u4PacketLen;
    pCurrSendPacket->WriteStream(szSession, 32);

    //拼接消息体
    pCurrSendPacket->WriteStream(pBuffer, u4PacketLen);

    //pCurrSendPacket->Close();

    CSend_Param objSendParam;
    //异步发送消息
    m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID,
        pMessage->GetMessageBase()->m_u2Cmd,
        pCurrSendPacket,
        objSendParam);

    return 0;
    __LEAVE_FUNCTION_WITHRETURN(0);
}

// 回复给客户端
int CBaseCommand::SendClient(_PacketInfo BodyPacket, short nCommand, uint32 nConnectId, bool nEncrypt, IBuffPacket* pSendBuffPacket)
{
	__ENTER_FUNCTION();

    PSS_UNUSED_ARG(nConnectId);

	int nRet = 0;
	//拼装发送包体
	//printf("Send: [%d][%#x], %s", nConnectId, nCommand, pData.c_str());
	char szSession[32] = { '\0' };
	uint16 u2Version = (short)NET_VERSION;
	uint16 u2Command = nCommand;
	uint32 u4PacketLen = BodyPacket.m_nDataLen;
	char* pBuffer = BodyPacket.m_pData;

	if (nEncrypt) {
		//pBuffer = m_CAESClass.aes_encrypt((char*)pData.c_str(), pKey, pIv, nPacketLen);
	}

    //这里演示的是同步发送
    //在需要及时返回的数据需求是，最好使用此方法
    //效率最快

    //拼接消息头
    (*pSendBuffPacket) << u2Version;
    (*pSendBuffPacket) << u2Command;
    (*pSendBuffPacket) << u4PacketLen;
    pSendBuffPacket->WriteStream(szSession, 32);

    //拼接消息体
    pSendBuffPacket->WriteStream(pBuffer, u4PacketLen);

    //这里什么都不用处理，数据包会被同步发送回去

	return nRet;
	__LEAVE_FUNCTION_WITHRETURN(0);
}
