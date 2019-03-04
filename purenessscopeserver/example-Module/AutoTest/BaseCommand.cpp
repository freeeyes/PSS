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
        //发送全部数据
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

    //数据原样奉还
    (*pResponsesPacket) << (uint32)BodyPacket.m_nDataLen;
    (*pResponsesPacket) << u2PostCommandID;
    pResponsesPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //发送全部数据
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
        //发送全部数据
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

    //数据原样奉还
    (*pResponsesPacket) << (uint32)10;
    (*pResponsesPacket) << (uint16)u2PostCommandID;
    (*pResponsesPacket) << (uint32)32;

    IBuffPacket* pResponsesPacket1 = m_pServerObject->GetPacketManager()->Create();
    (*pResponsesPacket1) << (uint32)64;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //发送全部数据
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

    //写入日志
    m_pServerObject->GetLogManager()->WriteLog(LOG_SYSTEM, "[Do_LogData]LogData nIdex=%d.", u4Index);

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_AUTOTEST_RETUEN_LOGDATA;

    (*pResponsesPacket) << u2PostCommandID;
    (*pResponsesPacket) << (uint32)0;

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //发送全部数据
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

    //如果是第一个包，沉睡10秒
    if(u4Index == 0)
    {
        //沉睡10秒
        ACE_Time_Value tvSleep(10, 0);
        ACE_OS::sleep(tvSleep);
    }


    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_AUTOTEST_RETURN_WORKTIMEOUT;

    (*pResponsesPacket) << u2PostCommandID;
    (*pResponsesPacket) << (uint32)0;

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //发送全部数据
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

    //沉睡1秒
    ACE_Time_Value tvSleep(0, 100000);
    ACE_OS::sleep(tvSleep);

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_AUTOTEST_RETURN_WORKAI;

    (*pResponsesPacket) << u2PostCommandID;
    (*pResponsesPacket) << (uint32)0;

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //发送全部数据
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }

    return true;
}
