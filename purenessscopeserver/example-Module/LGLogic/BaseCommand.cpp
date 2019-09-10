#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
    m_pServerObject = NULL;
    m_nCount        = 0;

    m_szLSIP[0]     = '\0';
    m_u4LSPort      = 0;
    m_u4LSServerID  = 0;
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
    MESSAGE_FUNCTION(CLIENT_LINK_CONNECT,         Do_Connect,            pMessage);
    MESSAGE_FUNCTION(CLIENT_LINK_CDISCONNET,      Do_DisConnect,         pMessage);
    MESSAGE_FUNCTION(CLINET_LINK_SENDTIMEOUT,     Do_ClientSendTimeout,  pMessage);
    MESSAGE_FUNCTION(COMMAND_LOGIC_CLIENAT_LOGIN, Do_Logic_Client_Login, pMessage);
    MESSAGE_FUNCTION_END;

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}

int CBaseCommand::Do_Connect(IMessage* pMessage)
{
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Connect] (%d)TCP CLIENT_LINK_CONNECT OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));

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

int CBaseCommand::Do_Logic_Client_Login(IMessage* pMessage)
{
    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return -1;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    _VCHARS_STR strUserName;
    _VCHARS_STR strUserPass;
    _VCHARS_STR strLSKey;

    (*pBodyPacket) >> strUserName;
    (*pBodyPacket) >> strUserPass;
    (*pBodyPacket) >> strLSKey;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    uint8  u1Type = 0;  //1需要去LS更新列表，0不需要更新

    if(ACE_OS::strcmp(strLSKey.text, m_objLSServer.Get_LS_Key()) != 0)
    {
        u1Type = 1;
    }

    //发送客户端数据
    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_LOGIC_CLIENAT_LOGIN_R;

    //设置数据报文采用网络字序
    pResponsesPacket->SetNetSort(true);

    uint32 u4UserID = 0;
    _VCHARS_STR strServerCode;

    //拼接服务器MD5
    uint8 u1Len = (uint8)ACE_OS::strlen( m_objLSServer.Get_LS_Key());
    strServerCode.SetData( m_objLSServer.Get_LS_Key(), u1Len);

    uint32 u4ListCount       = 0;
    IBuffPacket* pListPacket = NULL;

    //拼接列表包体
    if(u1Type == 1)
    {
        pListPacket = m_pServerObject->GetPacketManager()->Create();
        pListPacket->SetNetSort(true);
        m_objLSServer.Get_All_LG_List(pListPacket, u4ListCount);
    }

    //消息列表返回
    uint32 u4SendPacketLen = 0;

    if(u1Type == 1)
    {
        u4SendPacketLen = sizeof(uint32) + sizeof(uint8) +
                          sizeof(uint8) + ACE_OS::strlen( m_objLSServer.Get_LS_Key()) + 1
                          + sizeof(uint32) + pListPacket->GetPacketLen();
    }
    else
    {
        u4SendPacketLen = sizeof(uint32) + sizeof(uint8) +
                          sizeof(uint8) + ACE_OS::strlen( m_objLSServer.Get_LS_Key()) + 1;
    }

    //拆解出完整的数据包头
    _PacketInfo HeadPacket;
    pMessage->GetPacketHead(HeadPacket);

    IBuffPacket* pHeadPacket = m_pServerObject->GetPacketManager()->Create();
    pHeadPacket->WriteStream(HeadPacket.m_pData, HeadPacket.m_nDataLen);

    uint16 u2CommandID   = 0;           //命令字
    uint32 u2Version     = 0;           //协议版本号
    uint32 u4BodyLen     = 0;           //包体长度
    char   szSession[33] = {'\0'};      //Session字符串

    //解析包头中的数据包长
    (*pHeadPacket) >> u2Version;
    (*pHeadPacket) >> u2CommandID;
    (*pHeadPacket) >> u4BodyLen;
    pHeadPacket->WriteStream(szSession, 32);

    m_pServerObject->GetPacketManager()->Delete(pHeadPacket);

    (*pResponsesPacket) << u2Version;
    (*pResponsesPacket) << u2PostCommandID;
    (*pResponsesPacket) << u4SendPacketLen; //数据包体长度
    pResponsesPacket->WriteStream(szSession, 32);

    (*pResponsesPacket) << u4UserID;
    (*pResponsesPacket) << u1Type;
    (*pResponsesPacket) << strServerCode;

    //如果需要发送最新List列表，则在这里拼包
    if(u1Type == 1)
    {
        (*pResponsesPacket) << u4ListCount;
        pResponsesPacket->WriteStream(pListPacket->GetData(), pListPacket->GetPacketLen());
        m_pServerObject->GetPacketManager()->Delete(pListPacket);
    }

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

    //m_pServerObject->GetConnectManager()->CloseConnect(pMessage->GetMessageBase()->m_u4ConnectID);

    return 0;
}

void CBaseCommand::Init(const char* pFileName)
{
    ReadIniFile(pFileName);

    //m_objLSServer.Init(1, "127.0.0.1", 10080, m_pServerObject);
    m_objLSServer.Init(1, m_szLSIP, m_u4LSPort, m_pServerObject);

    vecControlInfo objControlList;

    uint32 u4ListenCount = m_pServerObject->GetControlListen()->GetListenCount();

    for (uint32 i = 0; i < u4ListenCount; i++)
    {
        _ControlInfo objControlInfo;
        m_pServerObject->GetControlListen()->ShowListen(i, objControlInfo);
        objControlList.push_back(objControlInfo);
    }

    //自动得到当前开放的IP和端口，然后记录下来，发送给远端LS
    m_objLSServer.Set_LG_Info(objControlList[0].m_szListenIP,
                              objControlList[0].m_u4Port,
                              (uint32)m_pServerObject->GetControlListen()->GetServerID());
    m_objLSServer.Connect();

    ActiveTimer* pTimer = m_pServerObject->GetAceTimerManager();
    m_u4TimeID = (uint32)pTimer->schedule(this, NULL, ACE_OS::gettimeofday() + ACE_Time_Value(MAX_TIMER_INTERVAL), ACE_Time_Value(MAX_TIMER_INTERVAL));
}

int CBaseCommand::handle_timeout( const ACE_Time_Value& tv, const void* arg )
{
    if(arg != NULL)
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::handle_timeout]tv.sec=%d!\n", tv.sec()));
    }

    //定时器到时执行
    OUR_DEBUG((LM_INFO, "[CBaseCommand::handle_timeout]Run!\n"));
    m_objLSServer.Send_LG_Alive();
    return 0;
}

void CBaseCommand::ReadIniFile(const char* pIniFileName)
{
    dictionary* pDictionary = NULL;
    pDictionary = iniparser_load(pIniFileName);

    if(NULL == pDictionary)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::ReadIniFile](%s)Read Ini fail.\n", pIniFileName));
        return;
    }
    else
    {
        //读取Ini文件内容
        m_u4LSServerID = (uint32)iniparser_getint(pDictionary, "LSServer:ServerID", 0);

        if(0 != m_u4LSServerID)
        {
            OUR_DEBUG((LM_INFO, "[CBaseCommand::ReadIniFile]ServerID=%d.\n", m_u4LSServerID));
        }

        char* pData = iniparser_getstring(pDictionary, "LSServer:IP", NULL);

        if(NULL != pData)
        {
            OUR_DEBUG((LM_INFO, "[CBaseCommand::ReadIniFile]IP=%s.\n", pData));
            sprintf_safe(m_szLSIP, 50, "%s", pData);
        }

        m_u4LSPort = (uint32)iniparser_getint(pDictionary, "LSServer:Port", 0);

        if(0 != m_u4LSPort)
        {
            OUR_DEBUG((LM_INFO, "[CBaseCommand::ReadIniFile]Port=%d.\n", m_u4LSPort));
        }

        iniparser_freedict(pDictionary);
    }
}
