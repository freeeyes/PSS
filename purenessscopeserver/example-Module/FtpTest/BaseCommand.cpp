#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
    m_pServerObject = NULL;
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
        LeaveFtpUser(pMessage->GetMessageBase()->m_u4ConnectID);
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));
        return 0;
    }

    //用户登陆
    if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_LOGIN)
    {
        Do_Ftp_Login(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_LOGOUT)
    {
        Do_Ftp_Logout(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_FILELIST)
    {
        Do_Ftp_FileList(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_FILE_DOWNLOAD)
    {
        Do_Ftp_FileDownLoad(pMessage);
    }
    else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_FILE_UPLOAD)
    {
        Do_Ftp_FileUpLoad(pMessage);
    }

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}

void CBaseCommand::Do_Ftp_Login( IMessage* pMessage )
{
    VCHARS_STR strUserName;
    VCHARS_STR strUserPass;

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

    //这里可以添加用户自己的逻辑判定方法
    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_RETURN_LOGIN;

    //用户密码验证
    uint32 u4Ret = CheckFtpUser(strUserName.text, strUserPass.text, pMessage->GetMessageBase()->m_u4ConnectID);

    //返回验证结果
    (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
    (*pResponsesPacket) << (uint32)u4Ret;

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
}

void CBaseCommand::Do_Ftp_Logout( IMessage* pMessage )
{
    VCHARS_STR strUserName;

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

    //这里可以添加用户自己的逻辑判定方法
    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_RETURN_LOGOUT;

    //设置用户离开
    LeaveFtpUser(strUserName.text);

    //返回验证结果
    (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
    (*pResponsesPacket) << (uint32)LOGIN_SUCCESS;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    if(NULL != m_pServerObject->GetConnectManager())
    {
        //发送全部数据
        m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, true);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
        m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
    }
}

void CBaseCommand::Do_Ftp_FileList( IMessage* pMessage )
{
    VCHARS_STR strUserName;
    VCHARM_STR strFilePath;

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
    (*pBodyPacket) >> strFilePath;

    //这里可以添加用户自己的逻辑判定方法
    IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
    uint16 u2PostCommandID = COMMAND_RETURN_FILELIST;

    //查看用户有无操作权限
    bool blState = CheckOnlineUser(strUserName.text, pMessage->GetMessageBase()->m_u4ConnectID);

    if(blState == false)
    {
        //返回验证结果
        (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
        (*pResponsesPacket) << (uint32)OP_FAIL;
    }
    else
    {
        (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
        (*pResponsesPacket) << (uint32)OP_OK;
        CDirView objDirView;

        //组织文件信息
        vevFileInfo objvevFileInfo;
        objDirView.GetDir(strFilePath.text, objvevFileInfo);
        (*pResponsesPacket) << (uint32)objvevFileInfo.size();
        VCHARS_STR strFileName;

        for(uint32 i = 0; i < (uint32)objvevFileInfo.size(); i++)
        {
            strFileName.text = (char* )objvevFileInfo[i].m_szFileName;
            strFileName.u1Len = (uint8)ACE_OS::strlen(objvevFileInfo[i].m_szFileName);
            (*pResponsesPacket) << strFileName;
            (*pResponsesPacket) << objvevFileInfo[i].m_u4FileType;
            (*pResponsesPacket) << objvevFileInfo[i].m_u4FileSize;
        }
    }


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
}

void CBaseCommand::Do_Ftp_FileDownLoad( IMessage* pMessage )
{
    VCHARS_STR strUserName;
    VCHARM_STR strFilePath;
    uint32     u4BlockSize;            //块大小
    uint32     u4CurrBlockIndex;       //当前块ID

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
    (*pBodyPacket) >> strFilePath;
    (*pBodyPacket) >> u4BlockSize;
    (*pBodyPacket) >> u4CurrBlockIndex;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    bool blState = CheckOnlineUser(strUserName.text, pMessage->GetMessageBase()->m_u4ConnectID);

    if(blState == true)
    {
        //获得文件块
        CDirView objDirView;
        uint32 u4BufferCount = 0;
        objDirView.GetFileBufferCount(strFilePath.text, u4BlockSize, u4BufferCount);

        if(u4CurrBlockIndex > u4BufferCount)
        {
            return;
        }

        uint32 u4FileBlockSize = 0;
        char* pBuffer = new char[u4BlockSize];
        bool blRet = objDirView.GetFileBuffer(strFilePath.text, pBuffer, u4FileBlockSize, u4BlockSize, u4CurrBlockIndex);

        if(blRet == true)
        {
            //组成发送包
            IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
            uint16 u2PostCommandID = COMMAND_RETURN_FILE_DOWNLOAD;
            VCHARB_STR strFileBlock;

            strFileBlock.text  = pBuffer;
            strFileBlock.u4Len = (uint32)u4FileBlockSize;

            (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
            (*pResponsesPacket) << (uint32)OP_OK;
            (*pResponsesPacket) << u4BufferCount;             //文件块个数
            (*pResponsesPacket) << u4CurrBlockIndex;          //当前文件块index
            (*pResponsesPacket) << strFileBlock;              //文件块内容

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
        }

        SAFE_DELETE_ARRAY(pBuffer);
    }
    else
    {
        IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
        uint16 u2PostCommandID = COMMAND_RETURN_FILE_DOWNLOAD;

        (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
        (*pResponsesPacket) << (uint32)OP_FAIL;

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
    }
}


void CBaseCommand::Do_Ftp_FileUpLoad(IMessage* pMessage)
{
    VCHARS_STR strUserName;
    VCHARM_STR strFilePath;
    VCHARB_STR strFileBuffer(false, VCHARS_TYPE_BINARY);          //文件块信息(二进制模式)
    uint32     u4BlockSize;            //块大小
    uint32     u4CurrBlockIndex;       //当前块ID

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
    (*pBodyPacket) >> strFilePath;
    (*pBodyPacket) >> u4BlockSize;
    (*pBodyPacket) >> u4CurrBlockIndex;
    (*pBodyPacket) >> strFileBuffer;

    m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

    bool blState = CheckOnlineUser(strUserName.text, pMessage->GetMessageBase()->m_u4ConnectID);

    if(blState == true)
    {
        //获得文件块
        CDirView objDirView;
        uint32 u4BufferCount = 0;
        objDirView.GetFileBufferCount(strFilePath.text, u4BlockSize, u4BufferCount);

        if(u4CurrBlockIndex > u4BufferCount)
        {
            return;
        }

        char* pBuffer = new char[u4BlockSize];

        //接收文件块信息
        ACE_OS::memcpy(pBuffer, strFileBuffer.text, strFileBuffer.u4Len);
        bool blRet = objDirView.SetFileBuffer(strFilePath.text, pBuffer, u4BlockSize, u4CurrBlockIndex);

        if(blRet == true)
        {
            //组成发送包
            IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
            uint16 u2PostCommandID = COMMAND_RETURN_FILE_UPLOAD;

            (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
            (*pResponsesPacket) << (uint32)OP_OK;

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
        }

        SAFE_DELETE_ARRAY(pBuffer);
    }
    else
    {
        IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
        uint16 u2PostCommandID = COMMAND_RETURN_FILE_UPLOAD;

        (*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
        (*pResponsesPacket) << (uint32)OP_FAIL;

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
    }
}

void CBaseCommand::InitUserList()
{
    //初始化允许的用户名和密码
    m_mapUserList.clear();

    string    strUserName;
    _UserInfo* pUserInfo = new _UserInfo();

    //添加一个可以允许的账号
    strUserName = "freeeyes";
    sprintf_safe(pUserInfo->m_szUserPass, MAX_BUFF_100, "1234");

    m_mapUserList.insert(mapUserList::value_type(strUserName, pUserInfo));
}

void CBaseCommand::ClearUserList()
{
    for(mapUserList::iterator itorUsedB = m_mapUserList.begin(); itorUsedB != m_mapUserList.end(); itorUsedB++)
    {
        _UserInfo* pUserInfo = (_UserInfo* )itorUsedB->second;
        SAFE_DELETE(pUserInfo);
    }

    m_mapUserList.clear();
}

int CBaseCommand::CheckFtpUser( const char* pUserName, const char* pUserPass, uint32 u4ConnectID )
{
    string strUserName = (string)pUserName;

    mapUserList::iterator f = m_mapUserList.find(pUserName);

    if(f == m_mapUserList.end())
    {
        return LOGIN_FAIL_NOEXIST;   //账号不存在
    }

    _UserInfo* pUserInfo = (_UserInfo* )f->second;

    if(ACE_OS::strcmp(pUserInfo->m_szUserPass, pUserPass) == 0)
    {
        if(pUserInfo->m_blOnline == true)
        {
            return LOGIN_FAIL_ONLINE;
        }
        else
        {
            pUserInfo->m_blOnline    = true;
            pUserInfo->m_u4ConnectID = u4ConnectID;
            return LOGIN_SUCCESS;
        }
    }
    else
    {
        return LOGIN_FAIL_PASSWORD;
    }
}

void CBaseCommand::LeaveFtpUser( uint32 u4ConnectID )
{
    for(mapUserList::iterator itorUsedB = m_mapUserList.begin(); itorUsedB != m_mapUserList.end(); itorUsedB++)
    {
        _UserInfo* pUserInfo = (_UserInfo* )itorUsedB->second;

        if(pUserInfo->m_u4ConnectID == u4ConnectID)
        {
            pUserInfo->m_blOnline    = false;
            pUserInfo->m_u4ConnectID = 0;
            break;
        }
    }
}

void CBaseCommand::LeaveFtpUser( const char* pUserName )
{
    string strUserName = (string)pUserName;

    mapUserList::iterator f = m_mapUserList.find(pUserName);

    if(f != m_mapUserList.end())
    {
        _UserInfo* pUserInfo = (_UserInfo* )f->second;
        pUserInfo->m_blOnline    = false;
        pUserInfo->m_u4ConnectID = 0;
    }
}

bool CBaseCommand::CheckOnlineUser( const char* pUserName, uint32 u4ConnectID )
{
    string strUserName = (string)pUserName;

    mapUserList::iterator f = m_mapUserList.find(pUserName);

    if(f != m_mapUserList.end())
    {
        _UserInfo* pUserInfo = (_UserInfo* )f->second;

        if(pUserInfo->m_u4ConnectID == u4ConnectID)
        {
            return true;
        }
    }

    return false;
}
