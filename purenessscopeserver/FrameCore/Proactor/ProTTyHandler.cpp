#include "ProTTyHandler.h"

CProTTyHandler::CProTTyHandler()
{
    m_szName[0] = 0;
    m_pmbReadBuff = new ACE_Message_Block(MAX_BUFF_1024);
}

CProTTyHandler::~CProTTyHandler()
{
    Close(GetConnectID());

    //������Դ
    m_pmbReadBuff->release();
    SAFE_DELETE(m_pTTyMessage);
}

bool CProTTyHandler::ConnectTTy()
{
    if (true == m_blState)
    {
        //����豸�Ѿ�������״̬��ʲôҲ����
        return true;
    }

    //�����豸������
    if (m_Connector.connect(m_Ttyio, ACE_DEV_Addr(m_szName), 0, ACE_Addr::sap_any, 0, O_RDWR | FILE_FLAG_OVERLAPPED) == -1)
    {
        OUR_DEBUG((LM_INFO, "[CProTTyHandler::Init]m_Connector.connect(%s) fail.\n", m_szName));
        return false;
    }

    //�����豸����
    if (m_Ttyio.control(ACE_TTY_IO::SETPARAMS, &m_ObjParams) == -1)
    {
        OUR_DEBUG((LM_INFO, "[CProTTyHandler::Init]m_Ttyio SETPARAMS(%s) fail.\n", m_szName));
        return false;
    }

    //������󶨸���Ӧ��(������)
    if (-1 == m_ObjReadRequire.open(*this, m_Ttyio.get_handle(), 0, this->proactor()))
    {
        OUR_DEBUG((LM_INFO, "[CProTTyHandler::Init]m_Ttyio open(%s) read fail.\n", m_szName));
        return false;
    }

    //������󶨸���Ӧ����д����
    if (-1 == m_ObjWriteRequire.open(*this, m_Ttyio.get_handle(), 0, this->proactor()))
    {
        OUR_DEBUG((LM_INFO, "[CProTTyHandler::Init]m_Ttyio open(%s) write fail.\n", m_szName));
        return false;
    }

    m_blState = true;
    return true;
}

void CProTTyHandler::Close(uint32 u4ConnectID)
{
    if (true == m_blState && m_u4ConnectID == u4ConnectID)
    {
        m_ObjReadRequire.cancel();
        m_ObjWriteRequire.cancel();

        if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
        {
            //����packetParse�Ͽ���Ϣ
            App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->DisConnect(m_u4ConnectID);
        }

        //�ر�ת���ӿ�
        App_ForwardManager::instance()->DisConnectRegedit(m_szName, ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_TTY);
        m_strDeviceName = "";

        m_Ttyio.close();
        m_blState = false;
        m_u4ConnectID = 0;
    }
}

bool CProTTyHandler::SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize)
{
    if (nullptr != objSendMessageInfo.pBuffPacket &&
        objSendMessageInfo.pBuffPacket->GetHeadLen() > 0)
    {
        //��������
        u4PacketSize = objSendMessageInfo.pBuffPacket->GetHeadLen();
        return Send_Data(objSendMessageInfo.pBuffPacket->GetData(), objSendMessageInfo.pBuffPacket->GetHeadLen());
    }
    else
    {
        return false;
    }
}

bool CProTTyHandler::PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const ACE_Time_Value& tvSend)
{
    ACE_UNUSED_ARG(u4ConnectID);
    ACE_UNUSED_ARG(tvSend);

    if (nullptr != pMbData &&
        u4Size > 0)
    {
        //��������
        return Send_Data(pMbData->rd_ptr(), u4Size);
    }
    else
    {
        return false;
    }
}

void CProTTyHandler::SetIsLog(bool blIsLog)
{
    ACE_UNUSED_ARG(blIsLog);
    //�ݲ�ʵ��
}

bool CProTTyHandler::Init(uint32 u4ConnectID, const char* pName, ACE_TTY_IO::Serial_Params inParams, ITTyMessage* pTTyMessage, EM_CONNECT_IO_DISPOSE emDispose, uint32 u4PacketParseInfoID)
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
        //�Ҳ���������
        OUR_DEBUG((LM_INFO, "[CProTTyHandler::Init]u4PacketParseInfoID is not exist.\n"));
        return false;
    }

    //��ʼ�������豸
    if (true == ConnectTTy())
    {
        if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
        {
            //����packetParse�Ͽ���Ϣ
            _ClientIPInfo objClientIPInfo;
            _ClientIPInfo objLocalIPInfo;
            m_pPacketParse->Connect(m_u4ConnectID,
                    objClientIPInfo,
                    objLocalIPInfo);

            //���Ϳ����Ϣ
            Send_Hander_Event(PACKET_TTY_CONNECT);
        }

        //�鿴�Ƿ����ת���ӿ�
        m_strDeviceName = App_ForwardManager::instance()->ConnectRegedit(pName,
            ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_TTY,
            dynamic_cast<IDeviceHandler*>(this));

        //׼����������
        Ready_To_Read_Buff();
        return true;
    }
    else
    {
        return false;
    }
}

bool CProTTyHandler::GetConnectState()
{
    return m_blState;
}

ACE_TTY_IO::Serial_Params CProTTyHandler::GetParams()
{
    return m_ObjParams;
}

void CProTTyHandler::SetPause(bool blPause)
{
    m_blPause = blPause;
}

bool CProTTyHandler::GetPause()
{
    return m_blPause;
}

void CProTTyHandler::handle_read_file(const ACE_Asynch_Read_File::Result& result)
{
    if (!result.success())
    {
        //�����豸�����쳣
        OUR_DEBUG((LM_ERROR, "[CProTTyHandler::handle_read_file]Error:%d.\n", (int)result.error()));

        if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN == m_emDispose && nullptr != m_pTTyMessage)
        {
            //֪ͨ�ϲ�Ӧ��
            m_pTTyMessage->ReportMessage(m_u4ConnectID, (uint32)result.error(), EM_TTY_EVENT_RW_ERROR);
        }

        if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME == m_emDispose)
        {
            //֪ͨ�����Ϣ
            Send_Hander_Event(PACKET_TTY_DISCONNECT);

        }

        return;
    }

    if (0 == result.bytes_transferred())
    {
        Ready_To_Read_Buff();
        return;
    }

    ACE_Message_Block& mb = result.message_block();

    if ("" != m_strDeviceName)
    {
        App_ForwardManager::instance()->SendData(m_strDeviceName, &mb);
        return;
    }

    if (false == m_blPause)
    {
        if (EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN == m_emDispose && nullptr != m_pTTyMessage)
        {
            //�ص��������ݺ���
            m_pTTyMessage->RecvData(m_u4ConnectID, mb.rd_ptr(), (uint32)result.bytes_transferred());
        }
        else
        {
            //���ÿ�ܵĺ�������
            if (nullptr != m_pPacketParse)
            {
                _Packet_Info obj_Packet_Info;
                uint8 n1Ret = m_pPacketParse->Parse_Packet_Stream(m_u4ConnectID,
                              &mb,
                              dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()),
                              &obj_Packet_Info,
                              EM_CONNECT_IO_TYPE::CONNECT_IO_TTY);

                if (PACKET_GET_ENOUGH == n1Ret)
                {
                    //������Ϣ����Ϣ���
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
					objMakePacket.m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TTY;

					Send_MakePacket_Queue(objMakePacket);

                    //���������m_pPacketParse
                    App_PacketParsePool::instance()->Delete(pPacketParse);
                }
            }
        }
    }

    Ready_To_Read_Buff();
}

void CProTTyHandler::handle_write_file(const ACE_Asynch_Write_File::Result& result)
{
    if (!result.success())
    {
        OUR_DEBUG((LM_ERROR, "[CProTTyHandler::handle_write_file]Error:%s.\n", result.error()));
        return;
    }

    OUR_DEBUG((LM_ERROR, "[CProTTyHandler::handle_write_file]Send OK:(%d).\n", result.message_block().length()));
    result.message_block().release();
}

bool CProTTyHandler::Send_Data(const char* pData, ssize_t nLen)
{
    //��������ѶϿ������ﳢ������
    ConnectTTy();

    if (true == m_blState && false == m_blPause)
    {
        ACE_Message_Block* m = new ACE_Message_Block(nLen);
        m->copy(pData, nLen);

        int nCurrLen = (int)m->length();

        if (0 != m_ObjWriteRequire.write(*m, m->length()))
        {
            //��������ʧ��
            m_pTTyMessage->ReportMessage(m_u4ConnectID, (uint32)errno, EM_TTY_EVENT_RW_ERROR);
        }

        return true;
    }
    else
    {
        //��ǰ�����жϣ��޷���������
        return false;
    }

}

bool CProTTyHandler::Device_Send_Data(const char* pData, ssize_t nLen)
{
    return Send_Data(pData, nLen);
}

NAMESPACE::uint32 CProTTyHandler::GetConnectID()
{
    return m_u4ConnectID;
}

void CProTTyHandler::Ready_To_Read_Buff()
{
    m_pmbReadBuff->reset();

    m_ObjReadRequire.read(*m_pmbReadBuff, m_pmbReadBuff->space());
}

void CProTTyHandler::Send_Hander_Event(uint8 u1Option)
{
    _MakePacket objMakePacket;

    objMakePacket.m_u4ConnectID     = GetConnectID();
    objMakePacket.m_pPacketParse    = nullptr;
    objMakePacket.m_u1Option        = u1Option;
    objMakePacket.m_u4PacketParseID = m_u4PacketParseInfoID;
    objMakePacket.m_pHandler        = this;
    objMakePacket.m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TTY;

    Send_MakePacket_Queue(objMakePacket);
}
