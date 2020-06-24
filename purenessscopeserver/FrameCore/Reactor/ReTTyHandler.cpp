#include "ReTTyHandler.h"

CReTTyHandler::CReTTyHandler() 
{
    m_pmbReadBuff = new ACE_Message_Block(MAX_BUFF_1024);
}

CReTTyHandler::~CReTTyHandler()
{
    OUR_DEBUG((LM_INFO, "[CReTTyHandler::~CReTTyHandler].\n"));
    Close();

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
        OUR_DEBUG((LM_INFO, "[CReTTyHandler::ConnectTTy]m_Connector.connect(%s) fail.\n", m_szName));
        return false;
    }

    //关联设备本身
    if (m_ReTtyio.control(ACE_TTY_IO::SETPARAMS, &m_ObjParams) == -1)
    {
        OUR_DEBUG((LM_INFO, "[CReTTyHandler::ConnectTTy]m_Ttyio SETPARAMS(%s) fail.\n", m_szName));
        return false;
    }

    if (this->reactor()->register_handler(m_ReTtyio.get_handle(), this, ACE_Event_Handler::READ_MASK) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CReTTyHandler::ConnectTTy](%s) Could not register with reactor for %p\n", m_szName));
        return false;
    }

    m_blState = true;
    return true;
}

void CReTTyHandler::Close()
{
    if (true == m_blState)
    {
        if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
        {
            //发送packetParse断开消息
            App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->DisConnect(m_u4ConnectID);

            //发送框架消息
            ACE_INET_Addr m_addrRemote;

			_MakePacket objMakePacket;

			objMakePacket.m_u4ConnectID     = m_u4ConnectID;
			objMakePacket.m_pPacketParse    = NULL;
			objMakePacket.m_u1Option        = PACKET_TTY_DISCONNECT;
			objMakePacket.m_AddrRemote      = m_addrRemote;
			objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
			objMakePacket.m_u1PacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TTY;

            Send_MakePacket_Queue(objMakePacket, "TTy", 0);
        }

        //关闭转发接口
        App_ForwardManager::instance()->DisConnectRegedit(m_szName, ENUM_FORWARD_TCP_TTY);
        m_strDeviceName = "";

        m_ReTtyio.close();
        m_blState = false;
    }
}

bool CReTTyHandler::Init(uint32 u4ConnectID, const char* pName, ACE_TTY_IO::Serial_Params inParams, ITTyMessage* pTTyMessage, EM_CONNECT_IO_DISPOSE emDispose, uint32 u4PacketParseInfoID)
{
    m_u4ConnectID = u4ConnectID;
    sprintf_safe(m_szName, MAX_BUFF_100, "%s", pName);
    m_pTTyMessage         = pTTyMessage;
    m_ObjParams           = inParams;
    m_emDispose           = emDispose;
    m_u4PacketParseInfoID = u4PacketParseInfoID;

    //初始化连接设备
    bool blRet = ConnectTTy();

    if (true == blRet && EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
    {
        _ClientIPInfo objClientIPInfo;
        _ClientIPInfo objLocalIPInfo;

        //发送packetParse断开消息
        App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Connect(m_u4ConnectID,
                objClientIPInfo,
                objLocalIPInfo);

        //发送框架消息
        ACE_INET_Addr m_addrRemote;

		_MakePacket objMakePacket;

		objMakePacket.m_u4ConnectID     = m_u4ConnectID;
		objMakePacket.m_pPacketParse    = NULL;
		objMakePacket.m_u1Option        = PACKET_TTY_CONNECT;
		objMakePacket.m_AddrRemote      = m_addrRemote;
		objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
		objMakePacket.m_u1PacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TTY;

        Send_MakePacket_Queue(objMakePacket, "TTy", 0);
    }

    //查看是否存在转发接口
    m_strDeviceName = App_ForwardManager::instance()->ConnectRegedit(pName,
                      ENUM_FORWARD_TCP_TTY,
                      dynamic_cast<IDeviceHandler*>(this));

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

    m_pmbReadBuff->reset();

    ssize_t bytes_read = m_ReTtyio.recv((void*)m_pmbReadBuff->wr_ptr(), MAX_BUFF_1024);

    if (bytes_read <= 0)
    {
        //接收设备数据异常
        OUR_DEBUG((LM_ERROR, "[CReTTyHandler::handle_input]Error:%d.\n", (int)errno));

        //通知上层应用
        m_pTTyMessage->ReportMessage(m_u4ConnectID, (uint32)errno, EM_TTY_EVENT_RW_ERROR);

        //断开当前设备
        Close();
    }
    else
    {
        if ("" != m_strDeviceName)
        {
            App_ForwardManager::instance()->SendData(m_strDeviceName, m_pmbReadBuff);
            return 0;
        }

        if (false != m_blPause)
        {
            if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN == m_emDispose)
            {
                //回调接收数据函数
                m_pTTyMessage->RecvData(m_u4ConnectID, m_pmbReadBuff->rd_ptr(), (uint32)bytes_read);
            }
            else
            {
                //调用框架的函数处理
                _Packet_Parse_Info* pPacketParse = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID);

                if (NULL != pPacketParse)
                {
                    _Packet_Info obj_Packet_Info;
                    uint8 n1Ret = pPacketParse->Parse_Packet_Stream(m_u4ConnectID,
                                  m_pmbReadBuff,
                                  dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()),
                                  &obj_Packet_Info,
                                  EM_CONNECT_IO_TYPE::CONNECT_IO_TTY);

                    if (PACKET_GET_ENOUGH == n1Ret)
                    {
                        //发送消息给消息框架
                        CPacketParse* pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);
                        pPacketParse->SetPacket_Head_Message(obj_Packet_Info.m_pmbHead);
                        pPacketParse->SetPacket_Body_Message(obj_Packet_Info.m_pmbBody);
                        pPacketParse->SetPacket_CommandID(obj_Packet_Info.m_u2PacketCommandID);
                        pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4HeadSrcLen);
                        pPacketParse->SetPacket_Head_Curr_Length(obj_Packet_Info.m_u4HeadCurrLen);
                        pPacketParse->SetPacket_Body_Src_Length(obj_Packet_Info.m_u4BodySrcLen);
                        pPacketParse->SetPacket_Body_Curr_Length(obj_Packet_Info.m_u4BodyCurrLen);

                        ACE_INET_Addr m_addrRemote;

						_MakePacket objMakePacket;

						objMakePacket.m_u4ConnectID     = m_u4ConnectID;
						objMakePacket.m_pPacketParse    = pPacketParse;
						objMakePacket.m_u1Option        = PACKET_PARSE;
						objMakePacket.m_AddrRemote      = m_addrRemote;
						objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
						objMakePacket.m_u1PacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TTY;

                        Send_MakePacket_Queue(objMakePacket, "TTy", 0);

                        //清理用完的m_pPacketParse
                        App_PacketParsePool::instance()->Delete(pPacketParse);
                    }
                }
            }
        }
    }

    return 0;
}

int CReTTyHandler::handle_signal(int signum, siginfo_t* psinfo, ucontext_t* context)
{
    ACE_UNUSED_ARG(psinfo);
    ACE_UNUSED_ARG(context);

    OUR_DEBUG((LM_ERROR, "[CReTTyHandler::handle_signal]Error:%d.\n", (int)signum));
    return 0;
}

int CReTTyHandler::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
    ACE_UNUSED_ARG(handle);
    ACE_UNUSED_ARG(close_mask);

    OUR_DEBUG((LM_ERROR, "[CReTTyHandler::handle_close]Error:%d.\n", (int)errno));
    return 0;
}

int CReTTyHandler::handle_timeout(const ACE_Time_Value& tvNow, const void*  p)
{
    ACE_UNUSED_ARG(tvNow);
    ACE_UNUSED_ARG(p);

    OUR_DEBUG((LM_ERROR, "[CReTTyHandler::handle_timeout]Run.\n"));
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

            //中断设备
            Close();
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

