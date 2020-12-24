#include "ReTTyHandler.h"

CReTTyHandler::CReTTyHandler() 
{
    m_pmbReadBuff = App_MessageBlockManager::instance()->Create(MAX_BUFF_1024);
}

CReTTyHandler::~CReTTyHandler()
{
    PSS_LOGGER_DEBUG("[CReTTyHandler::~CReTTyHandler].");
    Close(GetConnectID());

    //回收资源
    SAFE_DELETE(m_pTTyMessage);
    SAFE_DELETE(m_pmbReadBuff);
}

bool CReTTyHandler::ConnectTTy()
{
    if (true == m_blState)
    {
        //如果设备已经在连接状态则什么也不做
        return true;
    }

    //连接设备描述符
    if (m_ReConnector.connect(m_ReTtyio, ACE_DEV_Addr(m_szName), 0, ACE_Addr::sap_any, 0, O_RDWR | FILE_FLAG_OVERLAPPED) == -1)
    {
        PSS_LOGGER_DEBUG("[CReTTyHandler::ConnectTTy]m_Connector.connect({0}) fail.", m_szName);
        return false;
    }

    //关联设备本身
    if (m_ReTtyio.control(ACE_TTY_IO::SETPARAMS, &m_ObjParams) == -1)
    {
        PSS_LOGGER_DEBUG("[CReTTyHandler::ConnectTTy]m_Ttyio SETPARAMS({0]) fail.", m_szName);
        return false;
    }

    if (this->reactor()->register_handler(m_ReTtyio.get_handle(), this, ACE_Event_Handler::READ_MASK) == -1)
    {
        PSS_LOGGER_DEBUG("[CReTTyHandler::ConnectTTy]({0}) Could not register with reactor.", m_szName);
        return false;
    }

    m_blState = true;
    return true;
}

void CReTTyHandler::Close(uint32 u4ConnectID)
{
    if (true == m_blState)
    {
        if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose && u4ConnectID == m_u4ConnectID)
        {
            //发送packetParse断开消息
            App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->DisConnect(m_u4ConnectID);

            //发送框架消息
            Send_Hander_Event(PACKET_TTY_DISCONNECT);
        }

        //关闭转发接口
        App_ForwardManager::instance()->DisConnectRegedit(m_szName, ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_TTY);
        m_strDeviceName = "";

        m_ReTtyio.close();
        m_blState     = false;
        m_u4ConnectID = 0;
    }
}

bool CReTTyHandler::SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize)
{
    if (nullptr != objSendMessageInfo.pBuffPacket &&
        objSendMessageInfo.pBuffPacket->GetHeadLen() > 0)
    {
        //发送数据
        u4PacketSize = objSendMessageInfo.pBuffPacket->GetHeadLen();
        return Send_Data(objSendMessageInfo.pBuffPacket->GetData(), objSendMessageInfo.pBuffPacket->GetHeadLen());
    }
    else
    {
        return false;
    }
}

bool CReTTyHandler::PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend)
{
    ACE_UNUSED_ARG(u4ConnectID);
    ACE_UNUSED_ARG(tvSend);

    if (nullptr != pMbData &&
        u4Size > 0)
    {
        //发送数据
        return Send_Data(pMbData->rd_ptr(), u4Size);
    }
    else
    {
        return false;
    }
}

void CReTTyHandler::SetIsLog(bool blIsLog)
{
    ACE_UNUSED_ARG(blIsLog);
    //暂不实现
}

bool CReTTyHandler::Init(uint32 u4ConnectID, const char* pName, ACE_TTY_IO::Serial_Params inParams, ITTyMessage* pTTyMessage, EM_CONNECT_IO_DISPOSE emDispose, uint32 u4PacketParseInfoID)
{
    m_u4ConnectID = u4ConnectID;
    sprintf_safe(m_szName, MAX_BUFF_100, "%s", pName);
    m_pTTyMessage         = pTTyMessage;
    m_ObjParams           = inParams;
    m_emDispose           = emDispose;
    m_u4PacketParseInfoID = u4PacketParseInfoID;

    m_pPacketParse = App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID);

    if (nullptr == m_pPacketParse)
    {
        //找不到解析器
        PSS_LOGGER_DEBUG("[CProTTyHandler::Init]u4PacketParseInfoID is not exist.");
        return false;
    }

    //初始化连接设备
    bool blRet = ConnectTTy();

    if (true == blRet && EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
    {
        _ClientIPInfo objClientIPInfo;
        _ClientIPInfo objLocalIPInfo;

        //发送packetParse断开消息
        m_pPacketParse->Connect(m_u4ConnectID,
                objClientIPInfo,
                objLocalIPInfo);
    }

    //查看是否存在转发接口
    m_strDeviceName = App_ForwardManager::instance()->ConnectRegedit(pName,
        ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_TTY,
        dynamic_cast<IHandler*>(this));

    return blRet;
}

bool CReTTyHandler::GetConnectState()
{
    return m_blState;
}

ACE_VERSIONED_NAMESPACE_NAME::ACE_TTY_IO::Serial_Params CReTTyHandler::GetParams()
{
    return m_ObjParams;
}

void CReTTyHandler::SetPause(bool blPause)
{
    m_blPause = blPause;
}

bool CReTTyHandler::GetPause()
{
    return m_blPause;
}

int CReTTyHandler::handle_input(ACE_HANDLE handle)
{
    ACE_UNUSED_ARG(handle);

    ssize_t bytes_read = m_ReTtyio.recv((void*)m_pmbReadBuff->wr_ptr(), MAX_BUFF_1024);

    if (bytes_read <= 0)
    {
        //接收设备数据异常
        PSS_LOGGER_DEBUG("[CReTTyHandler::handle_input]Error:{0}.", (int)errno);

        //通知上层应用
        m_pTTyMessage->ReportMessage(m_u4ConnectID, (uint32)errno, EM_TTY_EVENT_RW_ERROR);

        //发送框架消息
        Send_Hander_Event(PACKET_TTY_DISCONNECT);
        m_pmbReadBuff->reset();
        return 0;
    }

    if (true == m_blPause)
    {
        //在暂停阶段，不处理数据包直接丢弃
        m_pmbReadBuff->reset();
        return 0;
    }

    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->SendData(m_strDeviceName, m_pmbReadBuff);
    }

    if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN == m_emDispose)
    {
        //回调接收数据函数
        m_pTTyMessage->RecvData(m_u4ConnectID, m_pmbReadBuff->rd_ptr(), (uint32)bytes_read);
    }
    else
    {
        //调用框架的函数处理
        _Packet_Info obj_Packet_Info;
        uint8 n1Ret = m_pPacketParse->Parse_Packet_Stream(m_u4ConnectID,
            m_pmbReadBuff,
            dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()),
            &obj_Packet_Info,
            EM_CONNECT_IO_TYPE::CONNECT_IO_TTY);

        if (PACKET_GET_ENOUGH == n1Ret)
        {
            //发送消息给消息框架
            auto pPacketParse = std::make_shared<CPacketParse>();
            pPacketParse->SetPacket_Head_Message(obj_Packet_Info.m_pmbHead);
            pPacketParse->SetPacket_Body_Message(obj_Packet_Info.m_pmbBody);
            pPacketParse->SetPacket_CommandID(obj_Packet_Info.m_u2PacketCommandID);
            pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4HeadSrcLen);
            pPacketParse->SetPacket_Head_Curr_Length(obj_Packet_Info.m_u4HeadCurrLen);
            pPacketParse->SetPacket_Body_Src_Length(obj_Packet_Info.m_u4BodySrcLen);
            pPacketParse->SetPacket_Body_Curr_Length(obj_Packet_Info.m_u4BodyCurrLen);

            ACE_INET_Addr m_addrRemote;

            _MakePacket objMakePacket;

            objMakePacket.m_u4ConnectID = m_u4ConnectID;
            objMakePacket.m_pPacketParse = pPacketParse;
            objMakePacket.m_u1Option = PACKET_PARSE;
            objMakePacket.m_AddrRemote = m_addrRemote;
            objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
            objMakePacket.m_emPacketType = EM_CONNECT_IO_TYPE::CONNECT_IO_TTY;

            Send_MakePacket_Queue(m_MakePacket, objMakePacket);
        }
    }

    m_pmbReadBuff->reset();

    return 0;
}

int CReTTyHandler::handle_signal(int signum, siginfo_t* psinfo, ucontext_t* context)
{
    ACE_UNUSED_ARG(psinfo);
    ACE_UNUSED_ARG(context);

    PSS_LOGGER_DEBUG("[CReTTyHandler::handle_signal]Error:{0}.", signum);
    return 0;
}

int CReTTyHandler::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
    ACE_UNUSED_ARG(handle);
    ACE_UNUSED_ARG(close_mask);

    PSS_LOGGER_DEBUG("[CReTTyHandler::handle_close]Error:{0}.", errno);
    return 0;
}

int CReTTyHandler::handle_timeout(const ACE_Time_Value& tvNow, const void*  p)
{
    ACE_UNUSED_ARG(tvNow);
    ACE_UNUSED_ARG(p);

    PSS_LOGGER_DEBUG("[CReTTyHandler::handle_timeout]Run.");
    return 0;
}

bool CReTTyHandler::Send_Data(const char* pData, ssize_t nLen)
{
    //如果连接已断开，这里尝试重连
    ConnectTTy();

    if (true == m_blState && false == m_blPause)
    {
        if (nLen != m_ReTtyio.send_n(pData, nLen))
        {
            //发送数据失败
            m_pTTyMessage->ReportMessage(m_u4ConnectID, (uint32)errno, EM_TTY_EVENT_RW_ERROR);
        }

        return true;
    }
    else
    {
        //当前连接中断，无法发送数据
        return false;
    }
}

bool CReTTyHandler::Device_Send_Data(const char* pData, ssize_t nLen)
{
    return Send_Data(pData, nLen);
}

void CReTTyHandler::Send_Hander_Event(uint8 u1Option)
{
    _MakePacket objMakePacket;

    objMakePacket.m_u4ConnectID = GetConnectID();
    objMakePacket.m_pPacketParse = nullptr;
    objMakePacket.m_u1Option = u1Option;
    objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
    objMakePacket.m_pHandler = this;
    objMakePacket.m_emPacketType = EM_CONNECT_IO_TYPE::CONNECT_IO_TTY;

    Send_MakePacket_Queue(m_MakePacket, objMakePacket);
}

uint32 CReTTyHandler::GetConnectID()
{
    return m_u4ConnectID;
}

