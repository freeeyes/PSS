#include "BaseHander.h"

void Recovery_Message(bool blDelete, char*& pMessage)
{
    if (true == blDelete)
    {
        SAFE_DELETE_ARRAY(pMessage);
    }
}

bool Udp_Common_Send_Message(_Send_Message_Param const& obj_Send_Message_Param,
    IBuffPacket* pBuffPacket, 
    const ACE_SOCK_Dgram& skRemote,
    const _Packet_Parse_Info* pPacketParseInfo,
    ACE_Message_Block* pBlockMessage)
{
    if (nullptr == pBuffPacket)
    {
        OUR_DEBUG((LM_INFO, "[Udp_Common_Send_Message]pBuffPacket is nullptr.\n"));
        return false;
    }

    ACE_INET_Addr AddrRemote;
    int nErr = AddrRemote.set(obj_Send_Message_Param.m_u2Port, obj_Send_Message_Param.m_strClientIP.c_str());

    if (nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[Udp_Common_Send_Message]set_address error[%d].\n", errno));
        return false;
    }

    if (obj_Send_Message_Param.m_emSendType == EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_NOMAL)
    {
        ACE_Message_Block* pMbData = nullptr;
        //如果需要拼接包头，则拼接包头
        uint32 u4SendLength = pPacketParseInfo->Make_Send_Packet_Length(
            0, 
            pBuffPacket->GetPacketLen(), 
            obj_Send_Message_Param.m_u2CommandID);
        pMbData = App_MessageBlockManager::instance()->Create(u4SendLength);

        if (false == pPacketParseInfo->Make_Send_Packet(0, 
            pBuffPacket->GetData(), 
            pBuffPacket->GetPacketLen(), 
            pMbData, 
            obj_Send_Message_Param.m_u2CommandID))
        {
            OUR_DEBUG((LM_INFO, "[Udp_Common_Send_Message]Make_Send_Packet is false.\n"));
            return false;
        }

        //将发送数据放入缓冲
        memcpy_safe(pMbData->rd_ptr(), 
            (uint32)pMbData->length(), 
            pBlockMessage->wr_ptr(), 
            (uint32)pMbData->length());
        pBlockMessage->wr_ptr(pMbData->length());
        pMbData->release();
    }
    else
    {
        //将数据拷贝到ACE_Message_Block对象中
        memcpy_safe(pBuffPacket->GetData(), 
            pBuffPacket->GetPacketLen(),
            pBlockMessage->wr_ptr(),
            pBuffPacket->GetPacketLen());
        pBlockMessage->wr_ptr(pBuffPacket->GetPacketLen());
    }

    auto nSize = (int)skRemote.send(pBlockMessage->rd_ptr(), pBlockMessage->length(), AddrRemote);

    if ((uint32)nSize != pBlockMessage->length())
    {
        return false;
    }

    //发送完成释放
    pBlockMessage->reset();

    return true;
}

bool Udp_Common_Recv_Head(uint32 u4ConnectID, ACE_Message_Block* pMBHead, CPacketParse* pPacketParse, const _Packet_Parse_Info* pPacketParseInfo, uint32 u4Len)
{
    _Head_Info obj_Head_Info;
    bool blStateHead = pPacketParseInfo->Parse_Packet_Head_Info(u4ConnectID, pMBHead, App_MessageBlockManager::instance(), &obj_Head_Info, EM_CONNECT_IO_TYPE::CONNECT_IO_UDP);

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

bool Udp_Common_Recv_Body(uint32 u4ConnectID, ACE_Message_Block* pMBBody, CPacketParse* pPacketParse, const _Packet_Parse_Info* pPacketParseInfo)
{
    _Body_Info obj_Body_Info;
    bool blStateBody = pPacketParseInfo->Parse_Packet_Body_Info(u4ConnectID, pMBBody, App_MessageBlockManager::instance(), &obj_Body_Info, EM_CONNECT_IO_TYPE::CONNECT_IO_UDP);

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

bool Udp_Common_Recv_Stream(uint32 u4ConnectID, ACE_Message_Block* pMbData, CPacketParse* pPacketParse, const _Packet_Parse_Info* pPacketParseInfo)
{
    //以数据流处理
    _Packet_Info obj_Packet_Info;

    if (PACKET_GET_ENOUGH == pPacketParseInfo->Parse_Packet_Stream(u4ConnectID, pMbData, App_MessageBlockManager::instance(), &obj_Packet_Info, EM_CONNECT_IO_TYPE::CONNECT_IO_UDP))
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

bool Udp_Common_Send_WorkThread(uint32 u4ConnectID, CPacketParse* pPacketParse, const ACE_INET_Addr& addrRemote, const ACE_INET_Addr& addrLocal, const ACE_Time_Value& tvCheck)
{
    //组织数据包
    _MakePacket objMakePacket;
    objMakePacket.m_u4ConnectID  = u4ConnectID;
    objMakePacket.m_pPacketParse = pPacketParse;
    objMakePacket.m_emPacketType = EM_CONNECT_IO_TYPE::CONNECT_IO_UDP;
    objMakePacket.m_AddrRemote   = addrRemote;
    objMakePacket.m_AddrListen   = addrLocal;
    objMakePacket.m_u1Option     = PACKET_PARSE;

    //UDP因为不是面向链接的
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

    if (pBuffPacket != NULL && pBuffPacket->GetPacketLen() > 0)
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

void Send_MakePacket_Queue(_MakePacket const& objMakePacket)
{
    //放入消息队列
    if (false == App_MakePacket::instance()->PutMessageBlock(objMakePacket, objMakePacket.m_tvRecv))
    {
        OUR_DEBUG((LM_ERROR, "[Send_MakePacket_Queue] ConnectID = %d, PACKET_CONNECT is error.\n", objMakePacket.m_u4ConnectID));
    }
}

bool Tcp_Common_File_Message(_File_Message_Param const& obj_File_Message_Param, IBuffPacket*& pBuffPacket, const char* pConnectName)
{
    string strHexChar;     //单个十六进制的字符
    string strHexData;     //十六进制的字符串

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
        std::stringstream ss_format;

        ss_format << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (int)pData[i] << " ";
        strHexData += ss_format.str();
    }

    if (blblMore == true)
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_DEBUG_CLIENTSEND, "[(%s)%s:%d]%s.(数据包过长)", pConnectName, obj_File_Message_Param.m_addrRemote.get_host_addr(), obj_File_Message_Param.m_addrRemote.get_port_number(), strHexData.c_str());
    }
    else
    {
        AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_DEBUG_CLIENTSEND, "[(%s)%s:%d]%s.", pConnectName, obj_File_Message_Param.m_addrRemote.get_host_addr(), obj_File_Message_Param.m_addrRemote.get_port_number(), strHexData.c_str());
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

_ClientConnectInfo Tcp_Common_ClientInfo(_ClientConnectInfo_Param const& obj_ClientConnectInfo_Param)
{
    _ClientConnectInfo ClientConnectInfo;

    ClientConnectInfo.m_blValid       = true;
    ClientConnectInfo.m_u4ConnectID   = obj_ClientConnectInfo_Param.m_u4ConnectID;
    ClientConnectInfo.m_addrRemote    = obj_ClientConnectInfo_Param.m_addrRemote;
    ClientConnectInfo.m_u4RecvCount   = obj_ClientConnectInfo_Param.m_u4AllRecvCount;
    ClientConnectInfo.m_u4SendCount   = obj_ClientConnectInfo_Param.m_u4AllSendCount;
    ClientConnectInfo.m_u4AllRecvSize = obj_ClientConnectInfo_Param.m_u4AllRecvSize;
    ClientConnectInfo.m_u4AllSendSize = obj_ClientConnectInfo_Param.m_u4AllSendSize;
    ClientConnectInfo.m_u4BeginTime = (uint32)obj_ClientConnectInfo_Param.m_atvConnect.sec();
    ClientConnectInfo.m_u4AliveTime = (uint32)(ACE_OS::gettimeofday().sec() - obj_ClientConnectInfo_Param.m_atvConnect.sec());
    ClientConnectInfo.m_u4RecvQueueCount = obj_ClientConnectInfo_Param.m_u4RecvQueueCount;

    return ClientConnectInfo;
}

bool Tcp_Common_Send_Input_To_Cache(_Input_To_Cache_Param obj_Input_To_Cache_Param,
                                    ACE_Message_Block* pBlockMessage, uint32& u4PacketSize,
                                    IBuffPacket*& pBuffPacket)
{
    //先判断要发送的数据长度，看看是否可以放入缓冲，缓冲是否已经放满。
    uint32 u4SendPacketSize = 0;

    if (EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_NOMAL == obj_Input_To_Cache_Param.m_emSendType)
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
        memcpy_safe(pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pSendMessage->wr_ptr(), pBuffPacket->GetPacketLen());
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
        if (EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_NOMAL == obj_Input_To_Cache_Param.m_emSendType)
        {
            //这里组成返回数据包
            App_PacketParseLoader::instance()->GetPacketParseInfo(obj_Input_To_Cache_Param.m_u4PacketParseInfoID)->Make_Send_Packet(obj_Input_To_Cache_Param.m_u4ConnectID, pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pBlockMessage, obj_Input_To_Cache_Param.m_u2CommandID);
        }
        else
        {
            //如果不是SENDMESSAGE_NOMAL，则直接组包
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
                                 uint32& u4PacketSize)
{
    if (pBlockMessage == NULL)
    {
        OUR_DEBUG((LM_DEBUG, "[Tcp_Common_Make_Send_Packet](%d) pBlockMessage is NULL.\n", obj_Send_Packet_Param.m_u4ConnectID));
        return false;
    }

    if (EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_NOMAL == obj_Send_Packet_Param.m_emSendType)
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
            //如果发送组包失败，返回false
            return false;
        }
        //这里MakePacket已经加了数据长度，所以在这里不再追加
    }
    else
    {
        u4PacketSize = pBuffPacket->GetPacketLen();

        if (obj_Send_Packet_Param.m_u4SendMaxBuffSize > 0 && u4PacketSize >= obj_Send_Packet_Param.m_u4SendMaxBuffSize)
        {
            OUR_DEBUG((LM_DEBUG, "[Tcp_Common_Make_Send_Packet](%d) u4SendPacketSize is more than(%d)(%d).\n", obj_Send_Packet_Param.m_u4ConnectID, u4PacketSize, obj_Send_Packet_Param.m_u4SendMaxBuffSize));
            //如果连接不存在了，在这里返回失败，回调给业务逻辑去处理
            ACE_Message_Block* pSendMessage = App_MessageBlockManager::instance()->Create(u4PacketSize);
            memcpy_safe(pBuffPacket->GetData(), u4PacketSize, pSendMessage->wr_ptr(), u4PacketSize);
            pSendMessage->wr_ptr(u4PacketSize);
            ACE_Time_Value tvNow = ACE_OS::gettimeofday();
            App_MakePacket::instance()->PutSendErrorMessage(0, pSendMessage, tvNow);

            Recovery_Common_BuffPacket(obj_Send_Packet_Param.m_blDelete, pBuffPacket);

            return false;
        }

        memcpy_safe(pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pBlockMessage->wr_ptr(), pBuffPacket->GetPacketLen());
        pBlockMessage->wr_ptr(pBuffPacket->GetPacketLen());
    }

    //如果发送数据包有数据且长度大于零，才进行组包。
    if (0 < pBlockMessage->length())
    {
        //放入完成，则清空缓存数据，使命完成
        return true;
    }
    else
    {
        OUR_DEBUG((LM_DEBUG, "[Tcp_Common_Make_Send_Packet](%d) commandID=%d, pBlockMessage is 0.\n", 
            obj_Send_Packet_Param.m_u4ConnectID, 
            obj_Send_Packet_Param.m_u2CommandID));

        return false;
    }
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
        auto nQueueCount = (int)pTask->msg_queue()->message_count();

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

        if (pTask->putq(mb, NULL) == -1)
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
        AppLogManager::instance()->WriteToMail_i(LOG_SYSTEM_CONNECT,
                GetXmlConfigAttribute(xmlAlertConnect)->MailID,
                "Alert",
                "[Tcp_Common_Manager_Timeout_CheckInfo]active ConnectCount is more than limit(%d > %d).",
                nActiveConnectCount,
                GetXmlConfigAttribute(xmlAlertConnect)->ConnectAlert);
    }

    //检测单位时间连接数是否超越阀值
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

    //检测单位时间连接断开数是否超越阀值
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

