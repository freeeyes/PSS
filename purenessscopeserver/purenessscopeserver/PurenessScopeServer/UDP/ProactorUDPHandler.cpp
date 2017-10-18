#include "ProactorUDPHandler.h"

CProactorUDPHandler::CProactorUDPHandler(void)
{
    m_pPacketParse        = NULL;
    m_u4RecvPacketCount   = 0;
    m_u4SendPacketCount   = 0;
    m_u4RecvSize          = 0;
    m_u4SendSize          = 0;
    m_szCompletionkey[0]  = '\0';
    m_szAct[0]            = '\0';
    m_u4PacketParseInfoID = 0;
}

CProactorUDPHandler::~CProactorUDPHandler(void)
{
}

void CProactorUDPHandler::SetPacketParseInfoID(uint32 u4PacketParseInfoID)
{
    m_u4PacketParseInfoID = u4PacketParseInfoID;
}

int CProactorUDPHandler::OpenAddress(const ACE_INET_Addr& AddrLocal, ACE_Proactor* pProactor)
{
    if(m_skRemote.open(AddrLocal) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]Open error(%d).\n", errno));
        return -1;
    }

    m_addrLocal = AddrLocal;

    sprintf_safe(m_szCompletionkey, MAX_BUFF_20, "CompUDP");
    sprintf_safe(m_szAct, MAX_BUFF_20, "ActUDP");

    //按照线程初始化统计模块的名字
    char szName[MAX_BUFF_50] = {'\0'};
    sprintf_safe(szName, MAX_BUFF_50, "发送线程");
    m_CommandAccount.InitName(szName, App_MainConfig::instance()->GetMaxCommandCount());

    //初始化统计模块功能
    m_CommandAccount.Init(App_MainConfig::instance()->GetCommandAccount(),
                          App_MainConfig::instance()->GetCommandFlow(),
                          App_MainConfig::instance()->GetPacketTimeOut());


    //设置发送超时时间（因为UDP如果客户端不存在的话，sendto会引起一个recv错误）
    //在这里设置一个超时，让个recv不会无限等下去
    struct timeval timeout = {MAX_RECV_UDP_TIMEOUT, 0};
    ACE_OS::setsockopt(m_skRemote.get_handle(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    //Linux下无此对应宏，为了考虑通用，这部分只在windows下设置
#ifdef WIN32
    //设置wsaIoctl
    bool blBehavior = false;
    unsigned long lRet = 0;
    int nStatus = ACE_OS::ioctl(m_skRemote.get_handle(), SIO_UDP_CONNRESET, &blBehavior, sizeof(blBehavior), NULL, 0, &lRet, NULL, NULL);

    if(0 != nStatus)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]ioctl SIO_UDP_CONNRESET error.\n"));
    }

#endif

    if(m_Read.open(*this, m_skRemote.get_handle(), m_szCompletionkey, pProactor) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]m_Read error.\n"));
        return -1;
    }

    if(m_Write.open(*this, m_skRemote.get_handle(), m_szCompletionkey, pProactor) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]m_Write error.\n"));
        return -1;
    }

    ACE_Message_Block* pMBBuff = App_MessageBlockManager::instance()->Create(MAX_UDP_PACKET_LEN);

    if(NULL == pMBBuff)
    {
        OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::OpenAddress]pMBBuff is NULL.\n"));
        return -1;
    }

    //初始化检查器
    m_TimeConnectInfo.Init(App_MainConfig::instance()->GetClientDataAlert()->m_u4RecvPacketCount,
                           App_MainConfig::instance()->GetClientDataAlert()->m_u4RecvDataMax,
                           App_MainConfig::instance()->GetClientDataAlert()->m_u4SendPacketCount,
                           App_MainConfig::instance()->GetClientDataAlert()->m_u4SendDataMax);

    m_pPacketParse = App_PacketParsePool::instance()->Create();

    size_t stRecvLen = MAX_UDP_PACKET_LEN;
    //OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::OpenAddress]pMBBuff=0x%08x.\n", pMBBuff));
    int nRecvSize = (int)m_Read.recv(pMBBuff, stRecvLen, 0, PF_INET, m_szAct);
    return nRecvSize;
}

void CProactorUDPHandler::Close()
{
    App_PacketParsePool::instance()->Delete(m_pPacketParse);
    m_Read.cancel();
    m_Write.cancel();
    m_skRemote.close();
}

void CProactorUDPHandler::handle_read_dgram(const ACE_Asynch_Read_Dgram::Result& result)
{
    //因为UDP是一次性发送完整的数据包，所以不必做粘包处理
    ACE_Message_Block* pMBBuff = NULL;

    ACE_Message_Block* pMb = result.message_block();
    int nTran = (int)result.bytes_transferred();

    ACE_INET_Addr ClientAddr;
    result.remote_address(ClientAddr);
    m_addrRemote.set(ClientAddr);

    if(nTran != 0)
    {
        //处理数据
        if (false == CheckMessage(pMb, (uint32)nTran))
        {
            OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::handle_read_dgram]CheckMessage error.\n"));
        }

        m_pPacketParse = App_PacketParsePool::instance()->Create();
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::handle_read_dgram]result.bytes_transferred() is 0.\n"));
    }

    App_MessageBlockManager::instance()->Close(pMb);
    pMBBuff = App_MessageBlockManager::instance()->Create(MAX_UDP_PACKET_LEN);

    if(NULL == pMBBuff)
    {
        OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::handle_read_dgram]pMBBuff is NULL.\n"));
        return;
    }
    else
    {
        size_t stRecvLen = MAX_UDP_PACKET_LEN;
        //OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::handle_read_dgram]pMBBuff=0x%08x.\n", pMBBuff));
        m_Read.recv(pMBBuff, stRecvLen, 0, PF_INET, m_szAct);
    }
}

bool CProactorUDPHandler::SendMessage(const char*& pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead, uint16 u2CommandID, bool blDlete)
{
    ACE_Time_Value m_tvBegin = ACE_OS::gettimeofday();

    ACE_INET_Addr AddrRemote;
    int nErr = AddrRemote.set(nPort, szIP);

    if(nErr != 0)
    {
        OUR_DEBUG((LM_INFO, "[CProactorUDPHandler::SendMessage]set_address error[%d].\n", errno));

        if(true == blDlete)
        {
            SAFE_DELETE_ARRAY(pMessage);
        }

        return false;
    }

    //如果需要拼接包头，则拼接包头
    if(blHead == true)
    {
        CPacketParse PacketParse;

        uint32 u4SendLength = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Make_Send_Packet_Length(0, u4Len, u2CommandID);
        ACE_Message_Block* pMbData = App_MessageBlockManager::instance()->Create(u4SendLength);

        if(NULL == pMbData)
        {
            if(true == blDlete)
            {
                SAFE_DELETE_ARRAY(pMessage);
            }

            return false;
        }

        App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Make_Send_Packet(0, pMessage, u4Len, pMbData, u2CommandID);

        uint32 u4DataLen = (uint32)pMbData->length();
        int nSize = (int)m_skRemote.send(pMbData->rd_ptr(), u4DataLen, AddrRemote);

        if((uint32)nSize == u4DataLen)
        {
            m_atvOutput = ACE_OS::gettimeofday();
            m_u4SendSize += u4DataLen;
            m_u4SendPacketCount++;

            if(true == blDlete)
            {
                SAFE_DELETE_ARRAY(pMessage);
            }

            //统计发送信息
            ACE_Time_Value tvInterval = ACE_OS::gettimeofday() - m_tvBegin;
            uint32 u4SendCost = (uint32)(tvInterval.msec());
            m_CommandAccount.SaveCommandData(u2CommandID, u4SendCost, PACKET_UDP, u4Len, u4DataLen, COMMAND_TYPE_OUT);

            //释放发送体
            App_MessageBlockManager::instance()->Close(pMbData);

            return true;
        }
        else
        {
            OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]send error(%d).\n", errno));

            if(true == blDlete)
            {
                SAFE_DELETE_ARRAY(pMessage);
            }

            //释放发送体
            App_MessageBlockManager::instance()->Close(pMbData);

            return false;
        }
    }
    else
    {
        int nSize = (int)m_skRemote.send(pMessage, u4Len, AddrRemote);

        if((uint32)nSize == u4Len)
        {
            m_atvOutput = ACE_OS::gettimeofday();
            m_u4SendSize += u4Len;
            m_u4SendPacketCount++;
            SAFE_DELETE_ARRAY(pMessage);

            //统计发送信息
            ACE_Time_Value tvInterval = ACE_OS::gettimeofday() - m_tvBegin;
            uint32 u4SendCost = (uint32)(tvInterval.msec());
            m_CommandAccount.SaveCommandData(u2CommandID, u4SendCost, PACKET_UDP, u4Len, u4Len, COMMAND_TYPE_OUT);

            return true;
        }
        else
        {
            OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]send error(%d).\n", errno));
            SAFE_DELETE_ARRAY(pMessage);
            return false;
        }
    }
}

_ClientConnectInfo CProactorUDPHandler::GetClientConnectInfo()
{
    _ClientConnectInfo ClientConnectInfo;
    ClientConnectInfo.m_blValid       = true;
    ClientConnectInfo.m_u4ConnectID   = 0;
    ClientConnectInfo.m_u4AliveTime   = 0;
    ClientConnectInfo.m_u4BeginTime   = (uint32)m_atvInput.sec();
    ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
    ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
    ClientConnectInfo.m_u4RecvCount   = m_u4RecvPacketCount;
    ClientConnectInfo.m_u4SendCount   = m_u4SendPacketCount;
    return ClientConnectInfo;
}

bool CProactorUDPHandler::CheckMessage(ACE_Message_Block* pMbData, uint32 u4Len)
{
    ACE_Time_Value tvCheck = ACE_OS::gettimeofday();

    if(NULL == m_pPacketParse || NULL == pMbData)
    {
        return false;
    }

    if(App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->m_u1PacketParseType == PACKET_WITHHEAD)
    {
        if(u4Len < m_pPacketParse->GetPacketHeadLen())
        {
            return false;
        }

        //将完整的数据包转换为PacketParse对象
        ACE_Message_Block* pMBHead = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketHeadSrcLen());
        memcpy_safe((char* )pMbData->rd_ptr(), m_pPacketParse->GetPacketHeadSrcLen(), (char* )pMBHead->wr_ptr(), m_pPacketParse->GetPacketHeadSrcLen());
        pMBHead->wr_ptr(m_pPacketParse->GetPacketHeadSrcLen());

        _Head_Info obj_Head_Info;
        bool blStateHead = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Head_Info(0, pMBHead, App_MessageBlockManager::instance(), &obj_Head_Info);

        if(false == blStateHead)
        {
            App_MessageBlockManager::instance()->Close(pMbData);
            App_PacketParsePool::instance()->Delete(m_pPacketParse);
            return false;
        }
        else
        {
            m_pPacketParse->SetPacket_IsHandleHead(false);
            m_pPacketParse->SetPacket_Head_Src_Length(obj_Head_Info.m_u4HeadCurrLen);
            m_pPacketParse->SetPacket_Head_Curr_Length(obj_Head_Info.m_u4HeadCurrLen);
            m_pPacketParse->SetPacket_Body_Src_Length(obj_Head_Info.m_u4BodySrcLen);
            m_pPacketParse->SetPacket_CommandID(obj_Head_Info.m_u2PacketCommandID);
            m_pPacketParse->SetPacket_Head_Message(pMBHead);
        }

        if(u4Len != m_pPacketParse->GetPacketHeadSrcLen() + m_pPacketParse->GetPacketBodySrcLen())
        {
            App_MessageBlockManager::instance()->Close(pMBHead);
            App_PacketParsePool::instance()->Delete(m_pPacketParse);
            return false;
        }

        pMbData->rd_ptr(m_pPacketParse->GetPacketHeadSrcLen());

        //如果包含包体
        if(m_pPacketParse->GetPacketBodySrcLen() > 0)
        {
            ACE_Message_Block* pMBBody = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketBodySrcLen());
            memcpy_safe((char* )pMbData->rd_ptr(), m_pPacketParse->GetPacketBodySrcLen(), (char* )pMBBody->wr_ptr(), m_pPacketParse->GetPacketBodySrcLen());
            pMBBody->wr_ptr(m_pPacketParse->GetPacketBodySrcLen());

            _Body_Info obj_Body_Info;
            bool blStateBody = App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Body_Info(0, pMBBody, App_MessageBlockManager::instance(), &obj_Body_Info);

            if(false  == blStateBody)
            {
                App_MessageBlockManager::instance()->Close(pMBHead);
                App_MessageBlockManager::instance()->Close(pMBBody);
                App_PacketParsePool::instance()->Delete(m_pPacketParse);
                return false;
            }
            else
            {
                m_pPacketParse->SetPacket_Body_Message(pMBBody);
                m_pPacketParse->SetPacket_Body_Curr_Length(obj_Body_Info.m_u4BodyCurrLen);
            }
        }

        //组织数据包
        _MakePacket objMakePacket;
        objMakePacket.m_u4ConnectID       = UDP_HANDER_ID;
        objMakePacket.m_pPacketParse      = m_pPacketParse;
        objMakePacket.m_PacketType        = PACKET_UDP;
        objMakePacket.m_AddrRemote        = m_addrRemote;
        objMakePacket.m_AddrListen        = m_addrLocal;
        objMakePacket.m_u1Option          = PACKET_PARSE;

        //UDP因为不是面向链接的
        if(false == App_MakePacket::instance()->PutMessageBlock(&objMakePacket, tvCheck))
        {
            OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]PutMessageBlock is error.\n"));
            App_PacketParsePool::instance()->Delete(m_pPacketParse);
            return false;
        }
    }
    else
    {
        //以数据流处理
        _Packet_Info obj_Packet_Info;

        if(PACKET_GET_ENOUGTH == App_PacketParseLoader::instance()->GetPacketParseInfo(m_u4PacketParseInfoID)->Parse_Packet_Stream(0, pMbData, App_MessageBlockManager::instance(), &obj_Packet_Info))
        {
            m_pPacketParse->SetPacket_Head_Message(obj_Packet_Info.m_pmbHead);
            m_pPacketParse->SetPacket_Body_Message(obj_Packet_Info.m_pmbBody);
            m_pPacketParse->SetPacket_CommandID(obj_Packet_Info.m_u2PacketCommandID);
            m_pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4HeadSrcLen);
            m_pPacketParse->SetPacket_Head_Curr_Length(obj_Packet_Info.m_u4HeadCurrLen);
            m_pPacketParse->SetPacket_Head_Src_Length(obj_Packet_Info.m_u4BodySrcLen);
            m_pPacketParse->SetPacket_Body_Curr_Length(obj_Packet_Info.m_u4BodyCurrLen);

            //组织数据包
            _MakePacket objMakePacket;
            objMakePacket.m_u4ConnectID       = UDP_HANDER_ID;
            objMakePacket.m_pPacketParse      = m_pPacketParse;
            objMakePacket.m_PacketType        = PACKET_UDP;
            objMakePacket.m_AddrRemote        = m_addrRemote;
            objMakePacket.m_AddrListen        = m_addrLocal;
            objMakePacket.m_u1Option          = PACKET_PARSE;

            //UDP因为不是面向链接的
            if(false == App_MakePacket::instance()->PutMessageBlock(&objMakePacket, tvCheck))
            {
                App_PacketParsePool::instance()->Delete(m_pPacketParse);
                OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]PutMessageBlock is error.\n"));
                return false;
            }
        }
        else
        {
            OUR_DEBUG((LM_ERROR, "[CProactorUDPHandler::SendMessage]m_pPacketParse GetPacketStream is error.\n"));
            App_PacketParsePool::instance()->Delete(m_pPacketParse);
            return false;
        }
    }

    App_PacketParsePool::instance()->Delete(m_pPacketParse);

    m_atvInput = ACE_OS::gettimeofday();
    m_u4RecvSize += u4Len;
    m_u4RecvPacketCount++;

    return true;
}

void CProactorUDPHandler::GetCommandData( uint16 u2CommandID, _CommandData& objCommandData )
{
    _CommandData* pCommandData = m_CommandAccount.GetCommandData(u2CommandID);

    if(pCommandData != NULL)
    {
        objCommandData += (*pCommandData);
    }
}
