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

    //判断当前连接总数是否超越了2000个
    int nConnectCount = m_pServerObject->GetConnectManager()->GetCount();

    if(nConnectCount > 2000)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Connect]connect count is more(%d).\n", nConnectCount));
    }

    return 0;
}

int CBaseCommand::Do_DisConnect(IMessage* pMessage)
{
    //处理连接断开事件
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_DisConnect](%d)CLIENT_LINK_CDISCONNET OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));

    return 0;
}

int CBaseCommand::Do_ClientSendTimeout(IMessage* pMessage)
{
    //处理服务器发送客户端数据连接超过阀值的事件
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_DisConnect](%d)CLINET_LINK_SNEDTIMEOUT OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));

    return 0;
}

int CBaseCommand::Do_Base(IMessage* pMessage)
{
    //OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] CommandID = %d", COMMAND_BASE));
    //m_pServerObject->GetLogManager()->WriteToMail(LOG_SYSTEM, 1, "测试邮件", "测试");

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return -1;
    }

    //处理接收数据
    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_BASE;

    //调用Lua的接口并返回数据
    Do_Execute_Lua_Function(pMessage, pResponsesPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //发送全部数据
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
    //初始化调用的Lua文件
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
