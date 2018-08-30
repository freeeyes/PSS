#include "BaseHander.h"

void Recovery_Message(bool blDelete, char*& pMessage)
{
    if (true == blDelete)
    {
        SAFE_DELETE_ARRAY(pMessage);
    }
}

bool Udp_Common_Send_Message(_Send_Message_Param obj_Send_Message_Param, ACE_INET_Addr& AddrRemote, char*& pMessage,
                             ACE_Message_Block*& pMbData, ACE_SOCK_Dgram& skRemote)
{
    int nErr = AddrRemote.set(obj_Send_Message_Param.m_nPort, obj_Send_Message_Param.m_pIP);

    if (nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[Udp_Common_Send_Message]set_address error[%d].\n", errno));
        Recovery_Message(obj_Send_Message_Param.m_blDlete, pMessage);
        return false;
    }

    if (obj_Send_Message_Param.m_blHead == true)
    {
        //如果需要拼接包头，则拼接包头
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
        //不需要拼接包头，直接发送
        pMbData = App_MessageBlockManager::instance()->Create(obj_Send_Message_Param.m_u4Len);

        //将数据拷贝到ACE_Message_Block对象中
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
    bool blStateHead = App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Parse_Packet_Head_Info(0, pMBHead, App_MessageBlockManager::instance(), &obj_Head_Info);

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
    bool blStateBody = App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Parse_Packet_Body_Info(0, pMBBody, App_MessageBlockManager::instance(), &obj_Body_Info);

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
    //以数据流处理
    _Packet_Info obj_Packet_Info;

    if (PACKET_GET_ENOUGTH == App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Parse_Packet_Stream(0, pMbData, App_MessageBlockManager::instance(), &obj_Packet_Info))
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

bool Udp_Common_Send_WorkThread(CPacketParse*& pPacketParse, ACE_INET_Addr addrRemote, ACE_INET_Addr addrLocal, ACE_Time_Value& tvCheck)
{
    //组织数据包
    _MakePacket objMakePacket;
    objMakePacket.m_u4ConnectID = UDP_HANDER_ID;
    objMakePacket.m_pPacketParse = pPacketParse;
    objMakePacket.m_PacketType = PACKET_UDP;
    objMakePacket.m_AddrRemote = addrRemote;
    objMakePacket.m_AddrListen = addrLocal;
    objMakePacket.m_u1Option = PACKET_PARSE;

    //UDP因为不是面向链接的
    if (false == App_MakePacket::instance()->PutMessageBlock(&objMakePacket, tvCheck))
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

void Tcp_Common_Send_Message_Error(bool blDelete, IBuffPacket* pBuffPacket)
{
    ACE_Message_Block* pSendMessage = App_MessageBlockManager::instance()->Create(pBuffPacket->GetPacketLen());
    memcpy_safe((char*)pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), (char*)pSendMessage->wr_ptr(), pBuffPacket->GetPacketLen());
    pSendMessage->wr_ptr(pBuffPacket->GetPacketLen());
    ACE_Time_Value tvNow = ACE_OS::gettimeofday();

    if (false == App_MakePacket::instance()->PutSendErrorMessage(0, pSendMessage, tvNow))
    {
        OUR_DEBUG((LM_INFO, "Tcp_Common_Send_Message_Error]PutSendErrorMessage error.\n"));
    }

    Recovery_Common_BuffPacket(blDelete, pBuffPacket);
}

uint8 Tcp_Common_Recv_Stream(uint32 u4ConnectID, ACE_Message_Block* pMbData, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID)
{
    _Packet_Info obj_Packet_Info;
    uint8 n1Ret = App_PacketParseLoader::instance()->GetPacketParseInfo(u4PacketParseInfoID)->Parse_Packet_Stream(u4ConnectID, pMbData, dynamic_cast<IMessageBlockManager*>(App_MessageBlockManager::instance()), &obj_Packet_Info);

    if (PACKET_GET_ENOUGTH == n1Ret)
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
        return false;
    }

    return n1Ret;
}

void Send_MakePacket_Queue(uint32 u4ConnectID, uint32 u4PacketParseID, CPacketParse* m_pPacketParse, uint8 u1Option, ACE_INET_Addr& addrRemote, const char* pLocalIP, uint32 u4LocalPort)
{
    //需要回调发送成功回执
    _MakePacket objMakePacket;

    objMakePacket.m_u4ConnectID = u4ConnectID;
    objMakePacket.m_pPacketParse = m_pPacketParse;
    objMakePacket.m_u1Option = u1Option;
    objMakePacket.m_AddrRemote = addrRemote;
    objMakePacket.m_u4PacketParseID = u4PacketParseID;

    if (ACE_OS::strcmp("INADDR_ANY", pLocalIP) == 0)
    {
        objMakePacket.m_AddrListen.set(u4LocalPort);
    }
    else
    {
        objMakePacket.m_AddrListen.set(u4LocalPort, pLocalIP);
    }

    //如果这个端口是转发协议端口且为本地不处理，则不处理
    if (false == App_TcpRedirection::instance()->GetMode(u4LocalPort))
    {
        return;
    }

    //将数据送入工作线程处理队列
    ACE_Time_Value tvNow = ACE_OS::gettimeofday();

    if (false == App_MakePacket::instance()->PutMessageBlock(&objMakePacket, tvNow))
    {
        OUR_DEBUG((LM_ERROR, "[Send_MakePacket_Queue] ConnectID = %d, PACKET_CONNECT is error.\n", u4ConnectID));
    }
}

bool Tcp_Common_File_Message(_File_Message_Param obj_File_Message_Param, IBuffPacket*& pBuffPacket, const char* pConnectName)
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

    char* pData = (char*)pBuffPacket->GetData();

    for (uint32 i = 0; i < u4DebugSize; i++)
    {
        sprintf_safe(szLog, 10, "0x%02X ", (unsigned char)pData[i]);
        sprintf_safe(obj_File_Message_Param.m_pPacketDebugData + 5 * i, MAX_BUFF_1024 - 5 * i, "0x%02X ", (unsigned char)pData[i]);
    }

    obj_File_Message_Param.m_pPacketDebugData[5 * u4DebugSize] = '\0';

    if (blblMore == true)
    {
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_DEBUG_CLIENTSEND, "[(%s)%s:%d]%s.(数据包过长)", pConnectName, obj_File_Message_Param.m_addrRemote.get_host_addr(), obj_File_Message_Param.m_addrRemote.get_port_number(), obj_File_Message_Param.m_pPacketDebugData);
    }
    else
    {
        AppLogManager::instance()->WriteLog(LOG_SYSTEM_DEBUG_CLIENTSEND, "[(%s)%s:%d]%s.", pConnectName, obj_File_Message_Param.m_addrRemote.get_host_addr(), obj_File_Message_Param.m_addrRemote.get_port_number(), obj_File_Message_Param.m_pPacketDebugData);
    }

    //回调测试文件管理接口
    if (NULL != obj_File_Message_Param.m_pFileTest)
    {
        obj_File_Message_Param.m_pFileTest->HandlerServerResponse(obj_File_Message_Param.m_u4ConnectID);
    }

    //回收用完的对象
    Recovery_Common_BuffPacket(obj_File_Message_Param.m_blDelete, pBuffPacket);

    return true;
}

_ClientConnectInfo Tcp_Common_ClientInfo(_ClientConnectInfo_Param obj_ClientConnectInfo_Param)
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
    //先判断要发送的数据长度，看看是否可以放入缓冲，缓冲是否已经放满。
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
        //如果连接不存在了，在这里返回失败，回调给业务逻辑去处理
        ACE_Message_Block* pSendMessage = App_MessageBlockManager::instance()->Create(pBuffPacket->GetPacketLen());
        memcpy_safe((char*)pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), (char*)pSendMessage->wr_ptr(), pBuffPacket->GetPacketLen());
        pSendMessage->wr_ptr(pBuffPacket->GetPacketLen());
        ACE_Time_Value tvNow = ACE_OS::gettimeofday();
        App_MakePacket::instance()->PutSendErrorMessage(0, pSendMessage, tvNow);

        Recovery_Common_BuffPacket(obj_Input_To_Cache_Param.m_blDelete, pBuffPacket);

        return false;
    }
    else
    {
        //添加进缓冲区
        //SENDMESSAGE_NOMAL是需要包头的时候，否则，不组包直接发送
        if (obj_Input_To_Cache_Param.m_u1SendType == SENDMESSAGE_NOMAL)
        {
            //这里组成返回数据包
            App_PacketParseLoader::instance()->GetPacketParseInfo(obj_Input_To_Cache_Param.m_u4PacketParseInfoID)->Make_Send_Packet(obj_Input_To_Cache_Param.m_u4ConnectID, pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pBlockMessage, obj_Input_To_Cache_Param.m_u2CommandID);
        }
        else
        {
            //如果不是SENDMESSAGE_NOMAL，则直接组包
            memcpy_safe((char*)pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), (char*)pBlockMessage->wr_ptr(), pBuffPacket->GetPacketLen());
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
    if (obj_Send_Packet_Param.m_u1SendType == SENDMESSAGE_NOMAL)
    {
        u4PacketSize = App_PacketParseLoader::instance()->GetPacketParseInfo(obj_Send_Packet_Param.m_u4PacketParseInfoID)->Make_Send_Packet_Length(obj_Send_Packet_Param.m_u4ConnectID, pBuffPacket->GetPacketLen(), obj_Send_Packet_Param.m_u2CommandID);

        if (u4PacketSize >= obj_Send_Packet_Param.m_u4SendMaxBuffSize)
        {
            OUR_DEBUG((LM_DEBUG, "[Tcp_Common_Make_Send_Packet](%d) u4SendPacketSize is more than(%d)(%d).\n", obj_Send_Packet_Param.m_u4ConnectID, u4PacketSize, obj_Send_Packet_Param.m_u4SendMaxBuffSize));

            Recovery_Common_BuffPacket(obj_Send_Packet_Param.m_blDelete, pBuffPacket);

            return false;
        }

        App_PacketParseLoader::instance()->GetPacketParseInfo(obj_Send_Packet_Param.m_u4PacketParseInfoID)->Make_Send_Packet(obj_Send_Packet_Param.m_u4ConnectID, pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pBlockMessage, obj_Send_Packet_Param.m_u2CommandID);
        //这里MakePacket已经加了数据长度，所以在这里不再追加
    }
    else
    {
        u4PacketSize = (uint32)pBuffPacket->GetPacketLen();

        if (u4PacketSize >= obj_Send_Packet_Param.m_u4SendMaxBuffSize)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectHandle::SendMessage](%d) u4SendPacketSize is more than(%d)(%d).\n", obj_Send_Packet_Param.m_u4ConnectID, u4PacketSize, obj_Send_Packet_Param.m_u4SendMaxBuffSize));
            //如果连接不存在了，在这里返回失败，回调给业务逻辑去处理
            ACE_Message_Block* pSendMessage = App_MessageBlockManager::instance()->Create(pBuffPacket->GetPacketLen());
            memcpy_safe((char*)pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), (char*)pSendMessage->wr_ptr(), pBuffPacket->GetPacketLen());
            pSendMessage->wr_ptr(pBuffPacket->GetPacketLen());
            ACE_Time_Value tvNow = ACE_OS::gettimeofday();
            App_MakePacket::instance()->PutSendErrorMessage(0, pSendMessage, tvNow);

            Recovery_Common_BuffPacket(obj_Send_Packet_Param.m_blDelete, pBuffPacket);

            return false;
        }

        memcpy_safe((char*)pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), (char*)pBlockMessage->wr_ptr(), pBuffPacket->GetPacketLen());
        pBlockMessage->wr_ptr(pBuffPacket->GetPacketLen());
    }

    //因为是异步发送，发送的数据指针不可以立刻释放，所以需要在这里创建一个新的发送数据块，将数据考入
    pMbData = App_MessageBlockManager::instance()->Create((uint32)pBlockMessage->length());

    OUR_DEBUG((LM_INFO, "[Tcp_Common_Make_Send_Packet]pBlockMessage->length()=%d.\n", pBlockMessage->length()));
    memcpy_safe(pBlockMessage->rd_ptr(), (uint32)pBlockMessage->length(), pMbData->wr_ptr(), (uint32)pBlockMessage->length());
    pMbData->wr_ptr(pBlockMessage->length());
    //放入完成，则清空缓存数据，使命完成
    pBlockMessage->reset();

    return true;
}

bool Tcp_Common_CloseConnect_By_Queue(uint32 u4ConnectID, CSendMessagePool& objSendMessagePool, uint32 u4SendQueuePutTime, ACE_Task<ACE_MT_SYNCH>* pTask)
{
    //放入发送队列
    _SendMessage* pSendMessage = objSendMessagePool.Create();

    if (NULL == pSendMessage)
    {
        OUR_DEBUG((LM_ERROR, "[Tcp_Common_CloseConnect_By_Queue] new _SendMessage is error.\n"));
        return false;
    }

    ACE_Message_Block* mb = pSendMessage->GetQueueMessage();

    if (NULL != mb)
    {
        //组装关闭连接指令
        pSendMessage->m_u4ConnectID = u4ConnectID;
        pSendMessage->m_pBuffPacket = NULL;
        pSendMessage->m_nEvents = 0;
        pSendMessage->m_u2CommandID = 0;
        pSendMessage->m_u1SendState = 0;
        pSendMessage->m_blDelete = false;
        pSendMessage->m_nMessageID = 0;
        pSendMessage->m_u1Type = 1;
        pSendMessage->m_tvSend = ACE_OS::gettimeofday();

        //判断队列是否是已经最大
        int nQueueCount = (int)pTask->msg_queue()->message_count();

        if (nQueueCount >= (int)MAX_MSG_THREADQUEUE)
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

    //放入发送队列
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

        //判断队列是否是已经最大
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
    //检测连接总数是否超越监控阀值
    if (GetXmlConfigAttribute(xmlAlertConnect)->ConnectAlert > 0
        && nActiveConnectCount > (int)GetXmlConfigAttribute(xmlAlertConnect)->ConnectAlert)
    {
        AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT,
                                               GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                                               (char*)"Alert",
                                               "[Tcp_Common_Manager_Timeout_CheckInfo]active ConnectCount is more than limit(%d > %d).",
                                               nActiveConnectCount,
                                               GetXmlConfigAttribute(xmlAlertConnect)->ConnectAlert);
    }

    //检测单位时间连接数是否超越阀值
    int nCheckRet = App_ConnectAccount::instance()->CheckConnectCount();

    if (nCheckRet == 1)
    {
        AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT,
                                               GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                                               "Alert",
                                               "[Tcp_Common_Manager_Timeout_CheckInfo]CheckConnectCount is more than limit(%d > %d).",
                                               App_ConnectAccount::instance()->GetCurrConnect(),
                                               App_ConnectAccount::instance()->GetConnectMax());
    }
    else if (nCheckRet == 2)
    {
        AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT,
                                               GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                                               "Alert",
                                               "[Tcp_Common_Manager_Timeout_CheckInfo]CheckConnectCount is little than limit(%d < %d).",
                                               App_ConnectAccount::instance()->GetCurrConnect(),
                                               App_ConnectAccount::instance()->Get4ConnectMin());
    }

    //检测单位时间连接断开数是否超越阀值
    nCheckRet = App_ConnectAccount::instance()->CheckDisConnectCount();

    if (nCheckRet == 1)
    {
        AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT,
                                               GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                                               "Alert",
                                               "[Tcp_Common_Manager_Timeout_CheckInfo]CheckDisConnectCount is more than limit(%d > %d).",
                                               App_ConnectAccount::instance()->GetCurrConnect(),
                                               App_ConnectAccount::instance()->GetDisConnectMax());
    }
    else if (nCheckRet == 2)
    {
        AppLogManager::instance()->WriteToMail(LOG_SYSTEM_CONNECT,
                                               GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                                               "Alert",
                                               "[Tcp_Common_Manager_Timeout_CheckInfo]CheckDisConnectCount is little than limit(%d < %d).",
                                               App_ConnectAccount::instance()->GetCurrConnect(),
                                               App_ConnectAccount::instance()->GetDisConnectMin());
    }

}

_ClientNameInfo Tcp_Common_ClientNameInfo(uint32 u4ConnectID, const char* pConnectName, const char* pClientIP, int nClientPort, bool IsLog)
{
    _ClientNameInfo ClientNameInfo;
    ClientNameInfo.m_nConnectID = (int)u4ConnectID;
    sprintf_safe(ClientNameInfo.m_szName, MAX_BUFF_100, "%s", pConnectName);
    sprintf_safe(ClientNameInfo.m_szClientIP, MAX_BUFF_50, "%s", pClientIP);
    ClientNameInfo.m_nPort = nClientPort;

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
    //按照线程初始化统计模块的名字
    char szName[MAX_BUFF_50] = { '\0' };
    sprintf_safe(szName, MAX_BUFF_50, "发送线程(%d)", u2Index);
    objCommandAccount.InitName(szName, GetXmlConfigAttribute(xmlCommandAccount)->MaxCommandCount);

    //初始化统计模块功能
    objCommandAccount.Init(GetXmlConfigAttribute(xmlCommandAccount)->Account,
                           GetXmlConfigAttribute(xmlCommandAccount)->FlowAccount,
                           GetXmlConfigAttribute(xmlThreadInfo)->DisposeTimeout);

    //初始化队列最大发送缓冲数量
    u2SendQueueMax = GetXmlConfigAttribute(xmlSendInfo)->SendQueueMax;

    //初始化发送缓冲池
    objSendCacheManager.Init(GetXmlConfigAttribute(xmlSendInfo)->BlockCount, GetXmlConfigAttribute(xmlSendInfo)->MaxBlockSize);
}
