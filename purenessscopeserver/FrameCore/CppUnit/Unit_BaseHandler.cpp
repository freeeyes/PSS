#include "Unit_BaseHandler.h"

#ifdef _CPPUNIT_TEST

void CUnit_Basehandler::setUp(void)
{
    m_nBaseHandlerID = 1;
}

void CUnit_Basehandler::tearDown(void)
{
    m_nBaseHandlerID = 0;
}

void CUnit_Basehandler::Test_Tcp_Common_File_Message(void)
{
    bool blRet = false;
    _File_Message_Param obj_File_Message_Param;
    char szConnectName[MAX_BUFF_100] = { '\0' };
    uint32 u4PacketDebugSize         = 4;
    ACE_INET_Addr addrRemote;
    addrRemote.set(10002, "127.0.0.1");

    sprintf_safe(szConnectName, MAX_BUFF_100, "freeeyes");

    //存放测试数据
    uint32 u4Data = 1;

    auto pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    (*pBuffPacket) << u4Data;

    obj_File_Message_Param.m_addrRemote        = addrRemote;
    obj_File_Message_Param.m_blDelete          = false;
    obj_File_Message_Param.m_pFileTest         = nullptr;
    obj_File_Message_Param.m_u4ConnectID       = 1;
    obj_File_Message_Param.m_u4PacketDebugSize = u4PacketDebugSize;

    blRet = Tcp_Common_File_Message(obj_File_Message_Param, pBuffPacket, szConnectName);

    CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_File_Message]Tcp_Common_File_Message is false.", true == blRet);

    m_nTestCount++;
}

void CUnit_Basehandler::Test_Tcp_Common_ClientInfo(void)
{
    bool blRet = true;
    _ClientConnectInfo_Param obj_ClientConnectInfo_Param;

    Tcp_Common_ClientInfo(obj_ClientConnectInfo_Param);

    CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_ClientInfo]obj_ClientConnectInfo_Param is false.", true == blRet);
    m_nTestCount++;
}

void CUnit_Basehandler::Test_Tcp_Common_Send_Input_To_Cache(void)
{
    bool blRet = false;

    _Input_To_Cache_Param obj_Input_To_Cache_Param;
    obj_Input_To_Cache_Param.m_emSendType          = NAMESPACE::EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    obj_Input_To_Cache_Param.m_blDelete            = false;
    obj_Input_To_Cache_Param.m_u2CommandID         = 0x1001;
    obj_Input_To_Cache_Param.m_u4ConnectID         = 1;
    obj_Input_To_Cache_Param.m_u4PacketParseInfoID = 1;
    obj_Input_To_Cache_Param.m_u4SendMaxBuffSize   = MAX_BUFF_1024;

    ACE_Message_Block* pMB = new ACE_Message_Block(MAX_BUFF_1024);
   

    uint32 u4Data = 1;

    auto pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    (*pBuffPacket) << u4Data;

    uint32 u4PacketSize = pBuffPacket->GetPacketLen();

    pMB->copy(pBuffPacket->GetData(), pBuffPacket->GetWriteLen());

    CMakePacket MakePacket;
    blRet = Tcp_Common_Send_Input_To_Cache(MakePacket,
        obj_Input_To_Cache_Param,
        pMB,
        u4PacketSize,
        pBuffPacket);

    CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_Send_Input_To_Cache]Tcp_Common_Send_Input_To_Cache is false.", true == blRet);

    //测试缓冲区小于发送数据的分支
    obj_Input_To_Cache_Param.m_u4SendMaxBuffSize = 2;

    PSS_LOGGER_DEBUG("[Tcp_Common_Send_Input_To_Cache]Begin,");
    Tcp_Common_Send_Input_To_Cache(MakePacket,
        obj_Input_To_Cache_Param,
        pMB,
        u4PacketSize,
        pBuffPacket);
    PSS_LOGGER_DEBUG("[Tcp_Common_Send_Input_To_Cache]End,");
}

void CUnit_Basehandler::Test_Tcp_Common_Manager_Timeout_CheckInfo(void)
{
    bool blRet = true;

    Tcp_Common_Manager_Timeout_CheckInfo(0);

    CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_Manager_Timeout_CheckInfo]Tcp_Common_Manager_Timeout_CheckInfo is false.", true == blRet);
    m_nTestCount++;
}

void CUnit_Basehandler::Test_Tcp_Common_ClientNameInfo(void)
{
    bool blRet = false;

    _ClientNameInfo objClientNameInfo = Tcp_Common_ClientNameInfo(1, "freeeyes", "127.0.0.1", 20022, true);

    if (objClientNameInfo.m_nConnectID != 1 ||
        objClientNameInfo.m_strName != "freeeyes" ||
        objClientNameInfo.m_strClientIP != "127.0.0.1" ||
        objClientNameInfo.m_u2Port != 20022 ||
        objClientNameInfo.m_nLog != 1)
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_ClientNameInfo]Tcp_Common_ClientNameInfo is false.", true == blRet);
    }
    m_nTestCount++;
}

void CUnit_Basehandler::Test_Udp_Common_Recv_Stream(void)
{
    bool blRet = false;

    //拼装测试发送数据
    char szSendUDP[MAX_BUFF_200] = { '\0' };
    char szBodyBuff[MAX_BUFF_20] = { '\0' };
    char szSession[32]           = { '\0' };
    sprintf_safe(szBodyBuff, 20, "testudp");
    sprintf_safe(szSession, 32, "FREEEYES");

    //测试以流模式解数据包
    short sVersion = 1;
    short sCommand = (short)0x1000;
    int nPacketLen = (int)ACE_OS::strlen(szBodyBuff);

    memcpy(szSendUDP, (char*)&sVersion, sizeof(short));
    memcpy(&szSendUDP[2], (char*)&sCommand, sizeof(short));
    memcpy(&szSendUDP[4], (char*)&nPacketLen, sizeof(int));
    memcpy(&szSendUDP[8], (char*)&szSession, sizeof(char) * 32);
    memcpy(&szSendUDP[40], (char*)szBodyBuff, sizeof(char) * nPacketLen);
    uint32 u4SendLen = nPacketLen + 40;

    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(u4SendLen);
    memcpy_safe(szSendUDP, u4SendLen, pmb->wr_ptr(), u4SendLen);
    pmb->wr_ptr(u4SendLen);

    auto pPacketParseInfo = App_PacketParseLoader::instance()->GetPacketParseInfo(1);

    auto pPacketParse = std::make_shared<CPacketParse>();

    if (false == Udp_Common_Recv_Stream(1, pmb, pPacketParse, pPacketParseInfo))
    {
        PSS_LOGGER_DEBUG("[Test_Udp_Common_Recv_Stream]Udp_Common_Recv_Stream is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Udp_Common_Recv_Stream]Udp_Common_Recv_Stream is fail.", true == blRet);
    }

    App_MessageBlockManager::instance()->Close(pmb);
    m_nTestCount++;
}

void CUnit_Basehandler::Test_Udp_Common_Send_Message(void)
{
    bool blRet = false;
    ACE_Message_Block* pMbData = App_MessageBlockManager::instance()->Create(200);
    ACE_SOCK_Dgram skRemote;

    char szTestIP[MAX_BUFF_50] = { '\0' };
    sprintf_safe(szTestIP, MAX_BUFF_50, "300.0.0.1");

    _Send_Message_Param obj_Send_Message_Param;
    obj_Send_Message_Param.m_u4PacketParseInfoID = 1;
    obj_Send_Message_Param.m_blDlete             = false;
    obj_Send_Message_Param.m_emSendType          = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    obj_Send_Message_Param.m_u2Port              = 20002;
    obj_Send_Message_Param.m_strClientIP         = szTestIP;
    obj_Send_Message_Param.m_u2CommandID         = 0x1002;
    obj_Send_Message_Param.m_u4SendLength        = 4;

    auto pPacketParseInfo = App_PacketParseLoader::instance()->GetPacketParseInfo(1);

    auto ptestBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    (*ptestBuffPacket) << (uint32)1;

    //测试错误的IP地址
    bool blState = Udp_Common_Send_Message(obj_Send_Message_Param,
        ptestBuffPacket,
        skRemote,
        pPacketParseInfo,
        pMbData);

    if (blState == true)
    {
        PSS_LOGGER_DEBUG("[Test_Udp_Common_Send_Message]Udp_Common_Send_Message(300.0.0.1) is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Udp_Common_Send_Message]Udp_Common_Send_Message(300.0.0.1) is fail.", true == blRet);
        return;
    }

    //测试正常的数据解析
    sprintf_safe(szTestIP, MAX_BUFF_50, "127.0.0.1");
    obj_Send_Message_Param.m_strClientIP = szTestIP;
	blState = Udp_Common_Send_Message(obj_Send_Message_Param,
        ptestBuffPacket,
		skRemote,
		pPacketParseInfo,
		pMbData);

    if (blState == true)
    {
        PSS_LOGGER_DEBUG("[Test_Udp_Common_Send_Message]Udp_Common_Send_Message(127.0.0.1) is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Udp_Common_Send_Message]Udp_Common_Send_Message(127.0.0.1) is fail.", true == blRet);
        return;
    }

    App_MessageBlockManager::instance()->Close(pMbData);

}

void CUnit_Basehandler::Test_Tcp_Common_Make_Send_Packet(void)
{
    bool blRet = false;
    ACE_Message_Block* pMbData       = nullptr;
    ACE_Message_Block* pBlockMessage = App_MessageBlockManager::instance()->Create(MAX_BUFF_200);
    auto pBuffPacket         = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    (*pBuffPacket) << (uint32)10;

    pBlockMessage->copy(pBuffPacket->GetData(), pBuffPacket->GetWriteLen());

    _Send_Packet_Param obj_Send_Packet_Param;
    obj_Send_Packet_Param.m_blDelete = false;
    obj_Send_Packet_Param.m_emSendType  = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    obj_Send_Packet_Param.m_u2CommandID = 0x2001;
    obj_Send_Packet_Param.m_u4ConnectID = 1;
    obj_Send_Packet_Param.m_u4PacketParseInfoID = 1;
    obj_Send_Packet_Param.m_u4SendMaxBuffSize = 1;

    uint32 u4PacketSize = 4;

    //测试数据长度超长
    CMakePacket MakePacket;
    bool blState = Tcp_Common_Make_Send_Packet(MakePacket,
        obj_Send_Packet_Param,
        pBuffPacket,
        pBlockMessage,
        u4PacketSize);

    pMbData = App_MessageBlockManager::instance()->Create(pBlockMessage->length());
    pMbData->copy(pBlockMessage->rd_ptr(), pBlockMessage->length());

    if (blState == true)
    {
        PSS_LOGGER_DEBUG("[Test_Tcp_Common_Make_Send_Packet]Tcp_Common_Make_Send_Packet(m_u4SendMaxBuffSize=1) is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_Make_Send_Packet]Tcp_Common_Make_Send_Packet(m_u4SendMaxBuffSize=1) is fail.", true == blRet);
        return;
    }

    obj_Send_Packet_Param.m_u4SendMaxBuffSize = MAX_BUFF_200;

    blState = Tcp_Common_Make_Send_Packet(MakePacket,
        obj_Send_Packet_Param,
        pBuffPacket,
		pBlockMessage,
        u4PacketSize);

    if (blState == false)
    {
        PSS_LOGGER_DEBUG("[Test_Tcp_Common_Make_Send_Packet]Tcp_Common_Make_Send_Packet(m_u4SendMaxBuffSize=1) is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_Make_Send_Packet]Tcp_Common_Make_Send_Packet(m_u4SendMaxBuffSize=1) is fail.", true == blRet);
        return;
    }

    App_MessageBlockManager::instance()->Close(pMbData);
    App_MessageBlockManager::instance()->Close(pBlockMessage);

    m_nTestCount++;
}

#endif