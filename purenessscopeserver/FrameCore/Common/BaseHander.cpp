#include "BaseHander.h"

void Recovery_Message(bool blDelete, char*& pMessage)
{
    if (true == blDelete)
    {
        SAFE_DELETE_ARRAY(pMessage);
    }
}

bool Udp_Common_Send_Message(_Send_Message_Param const& obj_Send_Message_Param, char*& pMessage,
                             ACE_Message_Block*& pMbData, const ACE_SOCK_Dgram& skRemote)
{
    ACE_INET_Addr AddrRemote;
    int nErr = AddrRemote.set(obj_Send_Message_Param.m_u2Port, obj_Send_Message_Param.m_pIP);

    if (nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[Udp_Common_Send_Message]set_address error[%d].\n", errno));
        Recovery_Message(obj_Send_Message_Param.m_blDlete, pMessage);
        return false;
    }

    if (obj_Send_Message_Param.m_blHead == true)
    {
        //�����Ҫƴ�Ӱ�ͷ����ƴ�Ӱ�ͷ
        uint32 u4SendLength = App_PacketParseLoader::instance()->GetPacketParseInfo(obj_Send_Message_Param.m_u4PacketParseInfoID)->Make_Send_Packet_Length(0, obj_Send_Message_Param.m_u4Len, obj_Send_Message_Param.m_u2CommandID);
        pMbData = App_MessageBlockManager::instance()->Create(u4SendLength);

        if (false == App_PacketParseLoader::instance()->GetPacketParseInfo(obj_Send_Message_Param.m_u4PacketParseInfoID)->Make_Send_Packet(0, pMessage, obj_Send_Message_Param.m_u4Len, pMbData, obj_Send_Message_Param.m_u2CommandID))
        {
            OUR_DEBUG((LM_INFO, "[Udp_Common_Send_Message]Make_Send_Packet is false.\n"));
            Recovery_Message(obj_Send_Message_Param.m_blDlete, pMessage);
            return false;
        }

        Recovery_Message(obj_Send_Message_Param.m_blDlete, pMessage);
    }
    else
    {
        //����Ҫƴ�Ӱ�ͷ��ֱ�ӷ���
        pMbData = App_MessageBlockManager::instance()->Create(obj_Send_Message_Param.m_u4Len);

        //�����ݿ�����ACE_Message_Block������
        memcpy_safe(pMessage, obj_Send_Message_Param.m_u4Len, pMbData->wr_ptr(), obj_Send_Message_Param.m_u4Len);
        pMbData->wr_ptr(obj_Send_Message_Param.m_u4Len);

        Recovery_Message(obj_Send_Message_Param.m_blDlete, pMessage);
    }

    int nSize = (int)skRemote.send(pMbData->rd_ptr(), pMbData->length(), AddrRemote);

    if ((uint32)nSize != pMbData->length())
    {
        return false;
    }

    return true;
}

bool Udp_Common_Recv_Head(ACE_Message_Block* pMBHead, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID, uint32 u4Len)
{
    _Head_Info obj_Head_Info;
    bool blStateHead = App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Parse_Packet_Head_Info(0, pMBHead, App_MessageBlockManager::instance(), &obj_Head_Info, EM_CONNECT_IO_TYPE::CONNECT_IO_UDP);

    if (false == blStateHead)
    {
        return false;
    }
    else
    {
        pPacketParse->SetPacket_IsHandleHead(false);
        pPacketParse->SetPacket_Head_Curr_Length(obj_Head_Info.m_u4HeadCurrLen);
        pPacketParse->SetPacket_Body_Src_Length(obj_Head_Info.m_u4BodySrcLen);
        pPacketParse->SetPacket_CommandID(obj_Head_Info.m_u2PacketCommandID);
        pPacketParse->SetPacket_Head_Message(pMBHead);
    }

    if (u4Len != pPacketParse->GetPacketHeadSrcLen() + pPacketParse->GetPacketBodySrcLen())
    {
        return false;
    }

    return true;
}

bool Udp_Common_Recv_Body(ACE_Message_Block* pMBBody, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID)
{
    _Body_Info obj_Body_Info;
    bool blStateBody = App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Parse_Packet_Body_Info(0, pMBBody, App_MessageBlockManager::instance(), &obj_Body_Info, EM_CONNECT_IO_TYPE::CONNECT_IO_UDP);

    if (false == blStateBody)
    {
        return false;
    }
    else
    {
        pPacketParse->SetPacket_Body_Message(pMBBody);
        pPacketParse->SetPacket_Body_Curr_Length(obj_Body_Info.m_u4BodyCurrLen);
    }

    return true;
}

bool Udp_Common_Recv_Stream(ACE_Message_Block* pMbData, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID)
{
    //������������
    _Packet_Info obj_Packet_Info;

    if (PACKET_GET_ENOUGH == App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Parse_Packet_Stream(0, pMbData, App_MessageBlockManager::instance(), &obj_Packet_Info, EM_CONNECT_IO_TYPE::CONNECT_IO_UDP))
    {
        pPacketParse->SetPacket_Head_Message(obj_Packet_Info.m_pmbHead);
        pPacketParse->SetPacket_Body_Message(obj_Packet_Info.m_pmbBody);
        pPacketParse->SetPacket_CommandID(obj_Packet_Info.m_u2PacketCommandID);
        pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4HeadSrcLen);
        pPacketParse->SetPacket_Head_Curr_Length(obj_Packet_Info.m_u4HeadCurrLen);
        pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4BodySrcLen);
        pPacketParse->SetPacket_Body_Curr_Length(obj_Packet_Info.m_u4BodyCurrLen);
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[Udp_Common_Recv_Stream]m_pPacketParse GetPacketStream is error.\n"));
        App_PacketParsePool::instance()->Delete(pPacketParse);
        return false;
    }

    return true;
}

bool Udp_Common_Send_WorkThread(CPacketParse*& pPacketParse, const ACE_INET_Addr& addrRemote, const ACE_INET_Addr& addrLocal, const ACE_Time_Value& tvCheck)
{
    //��֯���ݰ�
    _MakePacket objMakePacket;
    objMakePacket.m_u4ConnectID = UDP_HANDER_ID;
    objMakePacket.m_pPacketParse = pPacketParse;
    objMakePacket.m_u1PacketType = EM_CONNECT_IO_TYPE::CONNECT_IO_UDP;
    objMakePacket.m_AddrRemote = addrRemote;
    objMakePacket.m_AddrListen = addrLocal;
    objMakePacket.m_u1Option = PACKET_PARSE;

    //UDP��Ϊ�����������ӵ�
    if (false == App_MakePacket::instance()->PutMessageBlock(objMakePacket, tvCheck))
    {
        OUR_DEBUG((LM_ERROR, "[Udp_Common_Send_WorkThread]PutMessageBlock is error.\n"));
        App_PacketParsePool::instance()->Delete(pPacketParse);
        return false;
    }

    return true;
}

void Recovery_Common_BuffPacket(bool blDelete, IBuffPacket* pBuffPacket)
{
    if (true == blDelete)
    {
        App_BuffPacketManager::instance()->Delete(pBuffPacket);
    }
}

void Tcp_Common_Send_Message_Error(uint32 u4ConnectID, uint16 u2CommandID, bool blDelete, IBuffPacket* pBuffPacket)
{
    ACE_Message_Block* pSendMessage = NULL;

    if (pBuffPacket != NULL || pBuffPacket->GetPacketLen() > 0)
    {
        pSendMessage = App_MessageBlockManager::instance()->Create(pBuffPacket->GetPacketLen());
        memcpy_safe(pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pSendMessage->wr_ptr(), pBuffPacket->GetPacketLen());
        pSendMessage->wr_ptr(pBuffPacket->GetPacketLen());

        if (false == App_MakePacket::instance()->PutSendErrorMessage(u4ConnectID, pSendMessage, ACE_OS::gettimeofday()))
        {
            OUR_DEBUG((LM_INFO, "[Tcp_Common_Send_Message_Error]Tcp_Common_Send_Message_Error]PutSendErrorMessage error.\n"));
        }
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[Tcp_Common_Send_Message_Error]u4ConnectID=%d,u2CommandID=%d pBuffPacket error.\n", u4ConnectID, u2CommandID));
    }

    Recovery_Common_BuffPacket(blDelete, pBuffPacket);
}

uint8 Tcp_Common_Recv_Stream(uint32 u4ConnectID, ACE_Message_Block* pMbData, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID)
{
    _Packet_Info obj_Packet_Info;
    uint8 n1Ret = App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Parse_Packet_Stream(u4ConnectID, pMbData, dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()), &obj_Packet_Info, EM_CONNECT_IO_TYPE::CONNECT_IO_UDP);

    if (PACKET_GET_ENOUGH == n1Ret)
    {
        pPacketParse->SetPacket_Head_Message(obj_Packet_Info.m_pmbHead);
        pPacketParse->SetPacket_Body_Message(obj_Packet_Info.m_pmbBody);
        pPacketParse->SetPacket_CommandID(obj_Packet_Info.m_u2PacketCommandID);
        pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4HeadSrcLen);
        pPacketParse->SetPacket_Head_Curr_Length(obj_Packet_Info.m_u4HeadCurrLen);
        pPacketParse->SetPacket_Body_Src_Length(obj_Packet_Info.m_u4BodySrcLen);
        pPacketParse->SetPacket_Body_Curr_Length(obj_Packet_Info.m_u4BodyCurrLen);
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[Udp_Common_Recv_Stream]m_pPacketParse GetPacketStream is error.\n"));
        App_PacketParsePool::instance()->Delete(pPacketParse);
    }

    return n1Ret;
}

void Send_MakePacket_Queue(_MakePacket objMakePacket, const char* pLocalIP, uint16 u2LocalPort)
{
    //��Ҫ�ص����ͳɹ���ִ
    if (EM_CONNECT_IO_TYPE::CONNECT_IO_TCP == objMakePacket.m_u1PacketType || EM_CONNECT_IO_TYPE::CONNECT_IO_UDP == objMakePacket.m_u1PacketType)
    {
        if (ACE_OS::strcmp("INADDR_ANY", pLocalIP) == 0)
        {
            objMakePacket.m_AddrListen.set(u2LocalPort);
        }
        else
        {
            objMakePacket.m_AddrListen.set(u2LocalPort, pLocalIP);
        }
    }

    //���������빤���̴߳������
    ACE_Time_Value tvNow = ACE_OS::gettimeofday();

    if (false == App_MakePacket::instance()->PutMessageBlock(objMakePacket, tvNow))
    {
        OUR_DEBUG((LM_ERROR, "[Send_MakePacket_Queue] ConnectID = %d, PACKET_CONNECT is error.\n", objMakePacket.m_u4ConnectID));
    }
}

bool Tcp_Common_File_Message(_File_Message_Param const& obj_File_Message_Param, IBuffPacket*& pBuffPacket, const char* pConnectName)
{
    char szLog[10] = { '\0' };
    uint32 u4DebugSize = 0;
    bool blblMore = false;

    if (pBuffPacket->GetPacketLen() >= obj_File_Message_Param.m_u4PacketDebugSize)
    {
        u4DebugSize = obj_File_Message_Param.m_u4PacketDebugSize - 1;
        blblMore = true;
    }
    else
    {
        u4DebugSize = (int)pBuffPacket->GetPacketLen();
    }

    const char* pData = pBuffPacket->GetData();

    for (uint32 i = 0; i < u4DebugSize; i++)
    {
        sprintf_safe(szLog, 10, "0x%02X ", (unsigned char)pData[i]);
        sprintf_safe(obj_File_Message_Param.m_pPacketDebugData + 5 * i, MAX_BUFF_1024 - 5 * i, "0x%02X ", (unsigned char)pData[i]);
    }

    obj_File_Message_Param.m_pPacketDebugData[5 * u4DebugSize] = '\0';

    if (blblMore == true)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_DEBUG_CLIENTSEND, "[(%s)%s:%d]%s.(���ݰ�����)", pConnectName, obj_File_Message_Param.m_addrRemote.get_host_addr(), obj_File_Message_Param.m_addrRemote.get_port_number(), obj_File_Message_Param.m_pPacketDebugData);
    }
    else
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_DEBUG_CLIENTSEND, "[(%s)%s:%d]%s.", pConnectName, obj_File_Message_Param.m_addrRemote.get_host_addr(), obj_File_Message_Param.m_addrRemote.get_port_number(), obj_File_Message_Param.m_pPacketDebugData);
    }

    //�ص������ļ�����ӿ�
    if (NULL != obj_File_Message_Param.m_pFileTest)
    {
        obj_File_Message_Param.m_pFileTest->HandlerServerResponse(obj_File_Message_Param.m_u4ConnectID);
    }

    //��������Ķ���
    Recovery_Common_BuffPacket(obj_File_Message_Param.m_blDelete, pBuffPacket);

    return true;
}

_ClientConnectInfo Tcp_Common_ClientInfo(_ClientConnectInfo_Param const& obj_ClientConnectInfo_Param)
{
    _ClientConnectInfo ClientConnectInfo;

    ClientConnectInfo.m_blValid = true;
    ClientConnectInfo.m_u4ConnectID = obj_ClientConnectInfo_Param.m_u4ConnectID;
    ClientConnectInfo.m_addrRemote = obj_ClientConnectInfo_Param.m_addrRemote;
    ClientConnectInfo.m_u4RecvCount = obj_ClientConnectInfo_Param.m_u4AllRecvCount;
    ClientConnectInfo.m_u4SendCount = obj_ClientConnectInfo_Param.m_u4AllSendCount;
    ClientConnectInfo.m_u4AllRecvSize = obj_ClientConnectInfo_Param.m_u4AllRecvSize;
    ClientConnectInfo.m_u4AllSendSize = obj_ClientConnectInfo_Param.m_u4AllSendSize;
    ClientConnectInfo.m_u4BeginTime = (uint32)obj_ClientConnectInfo_Param.m_atvConnect.sec();
    ClientConnectInfo.m_u4AliveTime = (uint32)(ACE_OS::gettimeofday().sec() - obj_ClientConnectInfo_Param.m_atvConnect.sec());
    ClientConnectInfo.m_u4RecvQueueCount = obj_ClientConnectInfo_Param.m_u4RecvQueueCount;
    ClientConnectInfo.m_u8RecvQueueTimeCost = obj_ClientConnectInfo_Param.m_u8RecvQueueTimeCost;
    ClientConnectInfo.m_u8SendQueueTimeCost = obj_ClientConnectInfo_Param.m_u8SendQueueTimeCost;

    return ClientConnectInfo;
}

bool Tcp_Common_Send_Input_To_Cache(_Input_To_Cache_Param obj_Input_To_Cache_Param,
                                    ACE_Message_Block* pBlockMessage, uint32& u4PacketSize,
                                    IBuffPacket*& pBuffPacket)
{
    //���ж�Ҫ���͵����ݳ��ȣ������Ƿ���Է��뻺�壬�����Ƿ��Ѿ�������
    uint32 u4SendPacketSize = 0;

    if (obj_Input_To_Cache_Param.m_u1SendType == SENDMESSAGE_NOMAL)
    {
        u4SendPacketSize = App_PacketParseLoader::instance()->GetPacketParseInfo(obj_Input_To_Cache_Param.m_u4PacketParseInfoID)->Make_Send_Packet_Length(obj_Input_To_Cache_Param.m_u4ConnectID, pBuffPacket->GetPacketLen(), obj_Input_To_Cache_Param.m_u2CommandID);
    }
    else
    {
        u4SendPacketSize = (uint32)pBlockMessage->length();
    }

    u4PacketSize = u4SendPacketSize;

    if (u4SendPacketSize + (uint32)pBlockMessage->length() >= obj_Input_To_Cache_Param.m_u4SendMaxBuffSize)
    {
        OUR_DEBUG((LM_DEBUG, "[Tcp_Common_Send_Input_To_Cache] Connectid=[%d] m_pBlockMessage is not enougth.\n", obj_Input_To_Cache_Param.m_u4ConnectID));
        //������Ӳ������ˣ������ﷵ��ʧ�ܣ��ص���ҵ���߼�ȥ����
        ACE_Message_Block* pSendMessage = App_MessageBlockManager::instance()->Create(pBuffPacket->GetPacketLen());
        memcpy_safe(pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pSendMessage->wr_ptr(), pBuffPacket->GetPacketLen());
        pSendMessage->wr_ptr(pBuffPacket->GetPacketLen());
        ACE_Time_Value tvNow = ACE_OS::gettimeofday();
        App_MakePacket::instance()->PutSendErrorMessage(0, pSendMessage, tvNow);

        Recovery_Common_BuffPacket(obj_Input_To_Cache_Param.m_blDelete, pBuffPacket);

        return false;
    }
    else
    {
        //��ӽ�������
        //SENDMESSAGE_NOMAL����Ҫ��ͷ��ʱ�򣬷��򣬲����ֱ�ӷ���
        if (obj_Input_To_Cache_Param.m_u1SendType == SENDMESSAGE_NOMAL)
        {
            //������ɷ������ݰ�
            App_PacketParseLoader::instance()->GetPacketParseInfo(obj_Input_To_Cache_Param.m_u4PacketParseInfoID)->Make_Send_Packet(obj_Input_To_Cache_Param.m_u4ConnectID, pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pBlockMessage, obj_Input_To_Cache_Param.m_u2CommandID);
        }
        else
        {
            //�������SENDMESSAGE_NOMAL����ֱ�����
            memcpy_safe(pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pBlockMessage->wr_ptr(), pBuffPacket->GetPacketLen());
            pBlockMessage->wr_ptr(pBuffPacket->GetPacketLen());
        }
    }

    Recovery_Common_BuffPacket(obj_Input_To_Cache_Param.m_blDelete, pBuffPacket);

    return true;
}

bool Tcp_Common_Make_Send_Packet(_Send_Packet_Param obj_Send_Packet_Param,
                                 IBuffPacket*& pBuffPacket,
                                 ACE_Message_Block* pBlockMessage,
                                 ACE_Message_Block*& pMbData,
                                 uint32& u4PacketSize)
{
    if (pBlockMessage == NULL)
    {
        OUR_DEBUG((LM_DEBUG, "[Tcp_Common_Make_Send_Packet](%d) pBlockMessage is NULL.\n", obj_Send_Packet_Param.m_u4ConnectID));
    }

    if (obj_Send_Packet_Param.m_u1SendType == SENDMESSAGE_NOMAL)
    {
        u4PacketSize = App_PacketParseLoader::instance()->GetPacketParseInfo(obj_Send_Packet_Param.m_u4PacketParseInfoID)->Make_Send_Packet_Length(obj_Send_Packet_Param.m_u4ConnectID, pBuffPacket->GetPacketLen(), obj_Send_Packet_Param.m_u2CommandID);

        if (u4PacketSize >= obj_Send_Packet_Param.m_u4SendMaxBuffSize)
        {
            OUR_DEBUG((LM_DEBUG, "[Tcp_Common_Make_Send_Packet](%d) u4SendPacketSize is more than(%d)(%d).\n", obj_Send_Packet_Param.m_u4ConnectID, u4PacketSize, obj_Send_Packet_Param.m_u4SendMaxBuffSize));

            Recovery_Common_BuffPacket(obj_Send_Packet_Param.m_blDelete, pBuffPacket);

            return false;
        }

        if (false == App_PacketParseLoader::instance()->GetPacketParseInfo(obj_Send_Packet_Param.m_u4PacketParseInfoID)->Make_Send_Packet(obj_Send_Packet_Param.m_u4ConnectID, pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pBlockMessage, obj_Send_Packet_Param.m_u2CommandID))
        {
            //����������ʧ�ܣ�����false
            return false;
        }
        //����MakePacket�Ѿ��������ݳ��ȣ����������ﲻ��׷��
    }
    else
    {
        u4PacketSize = pBuffPacket->GetPacketLen();

        if (obj_Send_Packet_Param.m_u4SendMaxBuffSize > 0 && u4PacketSize >= obj_Send_Packet_Param.m_u4SendMaxBuffSize)
        {
            OUR_DEBUG((LM_DEBUG, "[Tcp_Common_Make_Send_Packet](%d) u4SendPacketSize is more than(%d)(%d).\n", obj_Send_Packet_Param.m_u4ConnectID, u4PacketSize, obj_Send_Packet_Param.m_u4SendMaxBuffSize));
            //������Ӳ������ˣ������ﷵ��ʧ�ܣ��ص���ҵ���߼�ȥ����
            ACE_Message_Block* pSendMessage = App_MessageBlockManager::instance()->Create(pBuffPacket->GetPacketLen());
            memcpy_safe(pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pSendMessage->wr_ptr(), pBuffPacket->GetPacketLen());
            pSendMessage->wr_ptr(pBuffPacket->GetPacketLen());
            ACE_Time_Value tvNow = ACE_OS::gettimeofday();
            App_MakePacket::instance()->PutSendErrorMessage(0, pSendMessage, tvNow);

            Recovery_Common_BuffPacket(obj_Send_Packet_Param.m_blDelete, pBuffPacket);

            return false;
        }

        memcpy_safe(pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pBlockMessage->wr_ptr(), pBuffPacket->GetPacketLen());
        pBlockMessage->wr_ptr(pBuffPacket->GetPacketLen());
    }

    //����������ݰ��������ҳ��ȴ����㣬�Ž��������
    if (0 < pBlockMessage->length())
    {
        //��Ϊ���첽���ͣ����͵�����ָ�벻���������ͷţ�������Ҫ�����ﴴ��һ���µķ������ݿ飬�����ݿ���
        pMbData = App_MessageBlockManager::instance()->Create((uint32)pBlockMessage->length());

        memcpy_safe(pBlockMessage->rd_ptr(), (uint32)pBlockMessage->length(), pMbData->wr_ptr(), (uint32)pBlockMessage->length());
        pMbData->wr_ptr(pBlockMessage->length());
        //������ɣ�����ջ������ݣ�ʹ�����
        pBlockMessage->reset();
        return true;
    }
    else
    {
        OUR_DEBUG((LM_DEBUG, "[Tcp_Common_Make_Send_Packet](%d) commandID=%d,m_u1SendType=%d, pBlockMessage is 0.\n", 
            obj_Send_Packet_Param.m_u4ConnectID, 
            obj_Send_Packet_Param.m_u2CommandID,
            obj_Send_Packet_Param.m_u1SendType));

        return false;
    }
}

bool Tcp_Common_CloseConnect_By_Queue(uint32 u4ConnectID, CSendMessagePool& objSendMessagePool, uint32 u4SendQueuePutTime, ACE_Task<ACE_MT_SYNCH>* pTask)
{
    //���뷢�Ͷ���
    _SendMessage* pSendMessage = objSendMessagePool.Create();

    if (NULL == pSendMessage)
    {
        OUR_DEBUG((LM_ERROR, "[Tcp_Common_CloseConnect_By_Queue] new _SendMessage is error.\n"));
        return false;
    }

    ACE_Message_Block* mb = pSendMessage->GetQueueMessage();

    if (NULL != mb)
    {
        //��װ�ر�����ָ��
        pSendMessage->m_u4ConnectID = u4ConnectID;
        pSendMessage->m_pBuffPacket = NULL;
        pSendMessage->m_nEvents = 0;
        pSendMessage->m_u2CommandID = 0;
        pSendMessage->m_u1SendState = 0;
        pSendMessage->m_blDelete = false;
        pSendMessage->m_nMessageID = 0;
        pSendMessage->m_u1Type = 1;
        pSendMessage->m_tvSend = ACE_OS::gettimeofday();

        //�ж϶����Ƿ����Ѿ����
        int nQueueCount = (int)pTask->msg_queue()->message_count();

        if (nQueueCount >= MAX_MSG_THREADQUEUE)
        {
            OUR_DEBUG((LM_ERROR, "[Tcp_Common_CloseConnect_By_Queue] Queue is Full nQueueCount = [%d].\n", nQueueCount));
            objSendMessagePool.Delete(pSendMessage);
            return false;
        }

        ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, u4SendQueuePutTime);

        if (pTask->putq(mb, &xtime) == -1)
        {
            OUR_DEBUG((LM_ERROR, "[Tcp_Common_CloseConnect_By_Queue] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));
            objSendMessagePool.Delete(pSendMessage);
            return false;
        }
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[Tcp_Common_CloseConnect_By_Queue] mb new error.\n"));
        objSendMessagePool.Delete(pSendMessage);
        return false;
    }

    return true;
}

bool Tcp_Common_Manager_Post_Message(_Post_Message_Param obj_Post_Message_Param, IBuffPacket* pBuffPacket,
                                     CSendMessagePool& objSendMessagePool,
                                     ACE_Task<ACE_MT_SYNCH>* pTask)
{
    if (NULL == pBuffPacket)
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectManager::PutMessage] pBuffPacket is NULL.\n"));
        return false;
    }

    //���뷢�Ͷ���
    _SendMessage* pSendMessage = objSendMessagePool.Create();

    if (NULL == pSendMessage)
    {
        OUR_DEBUG((LM_ERROR, "[CProConnectManager::PutMessage] new _SendMessage is error.\n"));

        if (true == obj_Post_Message_Param.m_blDelete)
        {
            App_BuffPacketManager::instance()->Delete(pBuffPacket);
        }

        return false;
    }

    ACE_Message_Block* mb = pSendMessage->GetQueueMessage();

    if (NULL != mb)
    {
        pSendMessage->m_u4ConnectID = obj_Post_Message_Param.m_u4ConnectID;
        pSendMessage->m_pBuffPacket = pBuffPacket;
        pSendMessage->m_nEvents     = obj_Post_Message_Param.m_u1SendType;
        pSendMessage->m_u2CommandID = obj_Post_Message_Param.m_u2CommandID;
        pSendMessage->m_u1SendState = obj_Post_Message_Param.m_u1SendState;
        pSendMessage->m_blDelete    = obj_Post_Message_Param.m_blDelete;
        pSendMessage->m_nMessageID  = obj_Post_Message_Param.m_nMessageID;
        pSendMessage->m_u1Type      = 0;
        pSendMessage->m_tvSend      = ACE_OS::gettimeofday();

        //�ж϶����Ƿ����Ѿ����
        int nQueueCount = (int)pTask->msg_queue()->message_count();

        if (nQueueCount >= (int)obj_Post_Message_Param.m_u2SendQueueMax)
        {
            OUR_DEBUG((LM_ERROR, "[CProConnectManager::PutMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));

            if (true == obj_Post_Message_Param.m_blDelete)
            {
                App_BuffPacketManager::instance()->Delete(pBuffPacket);
            }

            objSendMessagePool.Delete(pSendMessage);
            return false;
        }

        ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, obj_Post_Message_Param.m_u4SendQueuePutTime);

        if (pTask->putq(mb, &xtime) == -1)
        {
            OUR_DEBUG((LM_ERROR, "[CProConnectManager::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));

            if (true == obj_Post_Message_Param.m_blDelete)
            {
                App_BuffPacketManager::instance()->Delete(pBuffPacket);
            }

            objSendMessagePool.Delete(pSendMessage);
            return false;
        }
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CMessageService::PutMessage] mb new error.\n"));

        if (true == obj_Post_Message_Param.m_blDelete)
        {
            App_BuffPacketManager::instance()->Delete(pBuffPacket);
        }

        objSendMessagePool.Delete(pSendMessage);
        return false;
    }

    return true;
}

void Tcp_Common_Manager_Timeout_CheckInfo(int nActiveConnectCount)
{
    //������������Ƿ�Խ��ط�ֵ
    if (GetXmlConfigAttribute(xmlAlertConnect)->ConnectAlert > 0
        && nActiveConnectCount > (int)GetXmlConfigAttribute(xmlAlertConnect)->ConnectAlert)
    {
        AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECT,
                GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                "Alert",
                "[Tcp_Common_Manager_Timeout_CheckInfo]active ConnectCount is more than limit(%d > %d).",
                nActiveConnectCount,
                GetXmlConfigAttribute(xmlAlertConnect)->ConnectAlert);
    }

    //��ⵥλʱ���������Ƿ�Խ��ֵ
    int nCheckRet = App_ConnectAccount::instance()->CheckConnectCount();

    if (nCheckRet == 1)
    {
        AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECT,
                GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                "Alert",
                "[Tcp_Common_Manager_Timeout_CheckInfo]CheckConnectCount is more than limit(%d > %d).",
                App_ConnectAccount::instance()->GetCurrConnect(),
                App_ConnectAccount::instance()->GetConnectMax());
    }
    else if (nCheckRet == 2)
    {
        AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECT,
                GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                "Alert",
                "[Tcp_Common_Manager_Timeout_CheckInfo]CheckConnectCount is little than limit(%d < %d).",
                App_ConnectAccount::instance()->GetCurrConnect(),
                App_ConnectAccount::instance()->Get4ConnectMin());
    }

    //��ⵥλʱ�����ӶϿ����Ƿ�Խ��ֵ
    nCheckRet = App_ConnectAccount::instance()->CheckDisConnectCount();

    if (nCheckRet == 1)
    {
        AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECT,
                GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                "Alert",
                "[Tcp_Common_Manager_Timeout_CheckInfo]CheckDisConnectCount is more than limit(%d > %d).",
                App_ConnectAccount::instance()->GetCurrConnect(),
                App_ConnectAccount::instance()->GetDisConnectMax());
    }
    else if (nCheckRet == 2)
    {
        AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECT,
                GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                "Alert",
                "[Tcp_Common_Manager_Timeout_CheckInfo]CheckDisConnectCount is little than limit(%d < %d).",
                App_ConnectAccount::instance()->GetCurrConnect(),
                App_ConnectAccount::instance()->GetDisConnectMin());
    }

}

_ClientNameInfo Tcp_Common_ClientNameInfo(uint32 u4ConnectID, const char* pConnectName, const char* pClientIP, uint16 u2ClientPort, bool IsLog)
{
    _ClientNameInfo ClientNameInfo;
    ClientNameInfo.m_nConnectID = (int)u4ConnectID;
    sprintf_safe(ClientNameInfo.m_szName, MAX_BUFF_100, "%s", pConnectName);
    sprintf_safe(ClientNameInfo.m_szClientIP, MAX_BUFF_50, "%s", pClientIP);
    ClientNameInfo.m_u2Port = u2ClientPort;

    if (IsLog == true)
    {
        ClientNameInfo.m_nLog = 1;
    }
    else
    {
        ClientNameInfo.m_nLog = 0;
    }

    return ClientNameInfo;
}

void Tcp_Common_Manager_Init(uint16 u2Index, CCommandAccount& objCommandAccount, uint16& u2SendQueueMax, CSendCacheManager& objSendCacheManager)
{
    //�����̳߳�ʼ��ͳ��ģ�������
    char szName[MAX_BUFF_50] = { '\0' };
    sprintf_safe(szName, MAX_BUFF_50, "�����߳�(%d)", u2Index);
    objCommandAccount.InitName(szName, GetXmlConfigAttribute(xmlCommandAccount)->MaxCommandCount);

    //��ʼ��ͳ��ģ�鹦��
    objCommandAccount.Init(GetXmlConfigAttribute(xmlCommandAccount)->Account,
                           GetXmlConfigAttribute(xmlCommandAccount)->FlowAccount,
                           GetXmlConfigAttribute(xmlThreadInfo)->DisposeTimeout);

    //��ʼ����������ͻ�������
    u2SendQueueMax = GetXmlConfigAttribute(xmlSendInfo)->SendQueueMax;

    //��ʼ�����ͻ����
    objSendCacheManager.Init(GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount, GetXmlConfigAttribute(xmlSendInfo)->MaxBlockSize);
}
