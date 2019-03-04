#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
    m_pServerObject   = NULL;
    m_pPostServerData = NULL;
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

    //用户登陆
    if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_LOGIN)
    {
        Do_User_Login(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_LOGOUT)
    {
        Do_User_Logout(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_USERINFO)
    {
        Do_User_Info(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_SET_USERINFO)
    {
        Do_Set_User_Info(pMessage);
    }

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}

void CBaseCommand::Do_User_Login( IMessage* pMessage )
{
    bool       blIsNeedSend = true;
    VCHARS_STR strUserName;
    VCHARS_STR strUserPass;
    char szUserName[MAX_BUFF_100] = {'\0'};
    char szUserPass[MAX_BUFF_100] = {'\0'};

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    (*pBodyPacket) >> strUserName;
    (*pBodyPacket) >> strUserPass;

    //将接收数据转换为字符串
    ACE_OS::memcpy(szUserName, strUserName.text, strUserName.u1Len);
    ACE_OS::memcpy(szUserPass, strUserPass.text, strUserPass.u1Len);
    szUserName[strUserName.u1Len] = '\0';
    szUserPass[strUserPass.u1Len] = '\0';

    //这里可以添加用户自己的逻辑判定方法
    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_RETURN_LOGIN;

    uint32 u4Ret = LOGIN_SUCCESS;
    _UserValid* pUserValid = App_UserValidManager::instance()->GetUserValid(szUserName);

    if(NULL != pUserValid)
    {
        //比较用户密码是否正确
        if(ACE_OS::strcmp(pUserValid->m_szUserPass, szUserPass) == 0)
        {
            pUserValid->m_u4LoginCount++;
            u4Ret = LOGIN_SUCCESS;
        }
        else
        {
            u4Ret = LOGIN_FAIL_PASSWORD;
        }
    }
    else
    {
        //重新加载一下缓冲
        App_UserValidManager::instance()->GetFreeValid();

        //在重新找一下
        _UserValid* pUserValid = App_UserValidManager::instance()->GetUserValid(szUserName);

        if(NULL != pUserValid)
        {
            //比较用户密码是否正确
            if(ACE_OS::strcmp(pUserValid->m_szUserPass, szUserPass) == 0)
            {
                pUserValid->m_u4LoginCount++;
                u4Ret = LOGIN_SUCCESS;
            }
            else
            {
                u4Ret = LOGIN_FAIL_PASSWORD;
            }
        }
        else
        {
            //如果在本级别缓冲找不到，则到上层数据源去寻找。
            //告诉watch，我找不到数据，你来找，找到给我。
            if(NULL == m_pPostServerData)
            {
                u4Ret = LOGIN_FAIL_NOEXIST;
            }
            else
            {
                //组成发送数据包
                char szPostData[MAX_BUFF_200] = {'\0'};
                uint16 u2WatchCommandID = (uint16)SERVER_COMMAND_USERVALID;
                uint16 u2UserNameSize   = (uint16)ACE_OS::strlen(szUserName);
                uint16 u2UserPassSize   = (uint16)ACE_OS::strlen(szUserPass);
                uint32 u4SendSize = 4 + 2 + 2 + u2UserNameSize + 2 + u2UserPassSize + 4;
                uint32 u4PacketSize = 2 + 2 + u2UserNameSize + 2 + u2UserPassSize + 4;
                uint32 u4Pos = 0;
                ACE_OS::memcpy(&szPostData[u4Pos], (char* )&u4PacketSize, sizeof(uint32));
                u4Pos += sizeof(uint32);
                ACE_OS::memcpy(&szPostData[u4Pos], (char* )&u2WatchCommandID, sizeof(uint16));
                u4Pos += sizeof(uint16);
                ACE_OS::memcpy(&szPostData[u4Pos], (char* )&u2UserNameSize, sizeof(uint16));
                u4Pos += sizeof(uint16);
                ACE_OS::memcpy(&szPostData[u4Pos], (char* )&szUserName, u2UserNameSize);
                u4Pos += u2UserNameSize;
                ACE_OS::memcpy(&szPostData[u4Pos], (char* )&u2UserPassSize, sizeof(uint16));
                u4Pos += sizeof(uint16);
                ACE_OS::memcpy(&szPostData[u4Pos], (char* )&szUserPass, u2UserPassSize);
                u4Pos += u2UserPassSize;
                ACE_OS::memcpy(&szPostData[u4Pos], (char* )&pMessage->GetMessageBase()->m_u4ConnectID, sizeof(uint32));
                u4Pos += sizeof(uint32);

                if(false == m_pServerObject->GetClientManager()->SendData(1, szPostData, (int)u4SendSize, false))
                {
                    u4Ret = LOGIN_FAIL_NOEXIST;
                }
                else
                {
                    blIsNeedSend = false;
                }
            }
        }
    }

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    if(blIsNeedSend == true)
    {
        //返回验证结果
        (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
        (*pResponsesPacket) << (uint32)u4Ret;

        if(NULL != m_pServerObject->GetConnectManager())
        {
            //发送全部数据
            m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true, true);
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
            m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
        }
    }
    else
    {
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }
}

void CBaseCommand::Do_User_Logout( IMessage* pMessage )
{
    VCHARS_STR strUserName;
    char szUserName[MAX_BUFF_100] = {'\0'};

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    (*pBodyPacket) >> strUserName;

    //将接收数据转换为字符串
    ACE_OS::memcpy(szUserName, strUserName.text, strUserName.u1Len);

    //这里可以添加用户自己的逻辑判定方法
    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_RETURN_LOGOUT;

    uint32 u4Ret = LOGIN_SUCCESS;
    _UserValid* pUserValid = App_UserValidManager::instance()->GetUserValid(szUserName);

    if(NULL != pUserValid)
    {
        //比较用户密码是否正确
        u4Ret = LOGIN_SUCCESS;
    }
    else
    {
        u4Ret = LOGIN_FAIL_NOEXIST;
    }

    //返回验证结果
    (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
    (*pResponsesPacket) << (uint32)u4Ret;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //发送全部数据
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true, true);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }
}

void CBaseCommand::Do_User_Info( IMessage* pMessage )
{
    uint32     u4UserID     = 0;
    bool       blIsNeedSend = false;

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    (*pBodyPacket) >> u4UserID;

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);
    uint16 u2PostCommandID = COMMAND_RETURN_USERINFO;
    uint32 u4Ret = LOGIN_SUCCESS;

    _UserInfo* pUserInfo = App_UserInfoManager::instance()->GetUserInfo(u4UserID);

    if(pUserInfo == NULL)
    {
        //没有找到，找Watch要
        if(NULL == m_pPostServerData)
        {
            u4Ret = LOGIN_FAIL_NOEXIST;
            (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
            (*pResponsesPacket) << (uint32)u4Ret;
            (*pResponsesPacket) << (uint32)0;
            (*pResponsesPacket) << (uint32)0;
            (*pResponsesPacket) << (uint32)0;
            blIsNeedSend = true;
        }
        else
        {
            //向watch要数据
            //组成发送数据包
            char szPostData[MAX_BUFF_200] = {'\0'};
            uint16 u2WatchCommandID = (uint16)SERVER_COMMAND_USERINFO;
            uint32 u4SendSize = 4 + 2 + 4 + 4;
            uint32 u4PacketSize = 2 + 4 + 4;
            uint32 u4Pos = 0;
            ACE_OS::memcpy(&szPostData[u4Pos], (char* )&u4PacketSize, sizeof(uint32));
            u4Pos += sizeof(uint32);
            ACE_OS::memcpy(&szPostData[u4Pos], (char* )&u2WatchCommandID, sizeof(uint16));
            u4Pos += sizeof(uint16);
            ACE_OS::memcpy(&szPostData[u4Pos], (char* )&u4UserID, sizeof(uint32));
            u4Pos += sizeof(uint32);
            ACE_OS::memcpy(&szPostData[u4Pos], (char* )&pMessage->GetMessageBase()->m_u4ConnectID, sizeof(uint32));
            u4Pos += sizeof(uint32);

            if(false == m_pServerObject->GetClientManager()->SendData(1, szPostData, (int)u4SendSize, false))
            {
                u4Ret = LOGIN_FAIL_NOEXIST;
                (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
                (*pResponsesPacket) << (uint32)u4Ret;
                (*pResponsesPacket) << (uint32)0;
                (*pResponsesPacket) << (uint32)0;
                (*pResponsesPacket) << (uint32)0;
                blIsNeedSend = true;
            }
            else
            {
                blIsNeedSend = false;
            }

        }
    }
    else
    {
        //找到了，返回之
        //返回验证结果
        (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
        (*pResponsesPacket) << (uint32)u4Ret;
        (*pResponsesPacket) << (uint32)pUserInfo->m_u4UserID;
        (*pResponsesPacket) << (uint32)pUserInfo->m_u4Life;
        (*pResponsesPacket) << (uint32)pUserInfo->m_u4Magic;
        blIsNeedSend = true;
    }

    if(blIsNeedSend == true)
    {
        if(NULL != m_pServerObject->GetConnectManager())
        {
            //发送全部数据
            m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true, true);
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
            m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
        }
    }
    else
    {
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }

    return;
}

void CBaseCommand::Do_Set_User_Info( IMessage* pMessage )
{
    uint32     u4UserID     = 0;
    uint32     u4Life       = 0;
    uint32     u4Magic      = 0;

    IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();

    if(NULL == pBodyPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
        return;
    }

    _PacketInfo BodyPacket;
    pMessage->GetPacketBody(BodyPacket);

    pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

    (*pBodyPacket) >> u4UserID;
    (*pBodyPacket) >> u4Life;
    (*pBodyPacket) >> u4Magic;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);
    uint16 u2PostCommandID = COMMAND_RETURN_SET_USERINFO;
    uint32 u4Ret = LOGIN_SUCCESS;

    _UserInfo* pUserInfo = App_UserInfoManager::instance()->GetUserInfo(u4UserID);

    if(pUserInfo == NULL)
    {
        u4Ret = LOGIN_FAIL_NOEXIST;
        (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
        (*pResponsesPacket) << (uint32)u4Ret;
        (*pResponsesPacket) << (uint32)u4UserID;
    }
    else
    {

        //设置数据
        pUserInfo->m_u4Life  = u4Life;
        pUserInfo->m_u4Magic = u4Magic;

        (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
        (*pResponsesPacket) << (uint32)u4Ret;
        (*pResponsesPacket) << (uint32)u4UserID;
    }

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //发送全部数据
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true, true);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }

    return;
}

void CBaseCommand::InitUserList()
{
    App_UserValidManager::instance()->Init((uint32)MAX_LOGIN_VALID_COUNT, SHM_USERVALID_KEY, (uint32)sizeof(_UserValid));
    App_UserInfoManager::instance()->Init((uint32)MAX_LOGIN_INFO_COUNT, SHM_USERINFO_KEY, (uint32)sizeof(_UserInfo));

    m_pPostServerData = new CPostServerData();

    //设置返回客户端需要的发送对象
    m_pPostServerData->SetServerObject(m_pServerObject);

    //初始化连接关系
    m_pServerObject->GetClientManager()->Connect(1, "127.0.0.1", 10005, TYPE_IPV4, (IClientMessage* )m_pPostServerData);

}

void CBaseCommand::ClearUserList()
{
    SAFE_DELETE(m_pPostServerData);
    App_UserValidManager::instance()->Close();
}


