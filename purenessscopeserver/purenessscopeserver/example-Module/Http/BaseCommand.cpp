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
    //__ENTER_FUNCTION是自动给你的函数追加try的宏，用于捕捉错误，你可以使用__THROW_FUNCTION()抛出你的错误
    //比如__THROW_FUNCTION("hello"); 它会自动在Log文件夹下的assert.log记录文件名，函数名，行数，描述。
    //推荐你使用这个宏，记住一定要在函数的结束使用__LEAVE_FUNCTION或者__LEAVE_FUNCTION_WITHRETURN与之配对。
    //__LEAVE_FUNCTION_WITHRETURN和__LEAVE_FUNCTION不同是前者可以支持函数退出的时候返回一个变量
    //比如这个函数是返回一个int
    //那么末尾就是__LEAVE_FUNCTION_WITHRETURN(0); 0就是返回的int，当然，也支持别的类型变量，具体看你自己的函数。
    //如果函数不返回任何变量，你可以使用__LEAVE_FUNCTION即可。
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

    //处理链接建立信息
    if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CONNECT)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CONNECT OK.\n"));
        return 0;
    }

    //处理链接断开信息
    if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CDISCONNET)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));
        return 0;
    }

    //处理正常信息
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

    //处理接收到的握手数据，返回握手命令
    char szReturnData[MAX_BUFF_500] = {'\0'};

    int nLength = ACE_OS::strlen(RETURN_HTTP_HTML);

    sprintf_safe(szReturnData, MAX_BUFF_500, "HTTP /1.1 200 OK\r\nServer: PSS\r\nContent-Length: %d\r\nContent-Type: text/html\r\n\r\n%s", nLength, RETURN_HTTP_HTML);

    uint16 u2PostCommandID = COMMAND_RETURN_HTTP_DATA;

    //返回验证码
    if(NULL != m_pServerObject->GetConnectManager())
    {
        //发送全部数据
        //m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, szReturnData,
        //        (uint32)ACE_OS::strlen(szReturnData), SENDMESSAGE_JAMPNOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_SELF_RECYC);

        const char* ptrReturnData = reinterpret_cast<const char*>(szReturnData);
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

