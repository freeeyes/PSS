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
    char szPacketDebugData[10]       = { '\0' };
    uint32 u4PacketDebugSize         = 4;
    ACE_INET_Addr addrRemote;
    addrRemote.set(10002, "127.0.0.1");

    sprintf_safe(szConnectName, MAX_BUFF_100, "freeeyes");

    //存放测试数据
    uint32 u4Data = 1;

    IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create();

    (*pBuffPacket) << u4Data;

    obj_File_Message_Param.m_addrRemote        = addrRemote;
    obj_File_Message_Param.m_blDelete          = false;
    obj_File_Message_Param.m_pFileTest         = NULL;
    obj_File_Message_Param.m_pPacketDebugData  = (char* )szPacketDebugData;
    obj_File_Message_Param.m_u4ConnectID       = 1;
    obj_File_Message_Param.m_u4PacketDebugSize = u4PacketDebugSize;

    blRet = Tcp_Common_File_Message(obj_File_Message_Param, pBuffPacket, szConnectName);

    CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_File_Message]Tcp_Common_File_Message is false.", true == blRet);

    App_BuffPacketManager::instance()->Delete(pBuffPacket);
}

void CUnit_Basehandler::Test_Tcp_Common_ClientInfo(void)
{
    bool blRet = true;
    _ClientConnectInfo_Param obj_ClientConnectInfo_Param;

    Tcp_Common_ClientInfo(obj_ClientConnectInfo_Param);

    CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_ClientInfo]obj_ClientConnectInfo_Param is false.", true == blRet);
}

void CUnit_Basehandler::Test_Tcp_Common_Send_Input_To_Cache(void)
{
    bool blRet = false;

    _Input_To_Cache_Param obj_Input_To_Cache_Param;
    obj_Input_To_Cache_Param.m_u1SendType          = SENDMESSAGE_JAMPNOMAL;
    obj_Input_To_Cache_Param.m_blDelete            = false;
    obj_Input_To_Cache_Param.m_u2CommandID         = 0x1001;
    obj_Input_To_Cache_Param.m_u4ConnectID         = 1;
    obj_Input_To_Cache_Param.m_u4PacketParseInfoID = 1;
    obj_Input_To_Cache_Param.m_u4SendMaxBuffSize   = MAX_BUFF_1024;

    ACE_Message_Block* pMB = new ACE_Message_Block(MAX_BUFF_1024);

    uint32 u4Data = 1;

    IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create();

    (*pBuffPacket) << u4Data;

    uint32 u4PacketSize = pBuffPacket->GetPacketLen();

    blRet = Tcp_Common_Send_Input_To_Cache(obj_Input_To_Cache_Param,
                                           pMB,
                                           u4PacketSize,
                                           pBuffPacket);

    CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_Send_Input_To_Cache]Tcp_Common_Send_Input_To_Cache is false.", true == blRet);

    App_BuffPacketManager::instance()->Delete(pBuffPacket);
    SAFE_DELETE(pMB);
}

void CUnit_Basehandler::Test_Tcp_Common_Manager_Timeout_CheckInfo(void)
{
    bool blRet = true;

    Tcp_Common_Manager_Timeout_CheckInfo(0);

    CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_Manager_Timeout_CheckInfo]Tcp_Common_Manager_Timeout_CheckInfo is false.", true == blRet);
}

void CUnit_Basehandler::Test_Tcp_Common_ClientNameInfo(void)
{
    bool blRet = false;

    _ClientNameInfo objClientNameInfo = Tcp_Common_ClientNameInfo(1, "freeeyes", "127.0.0.1", 20022, true);

    if (objClientNameInfo.m_nConnectID != 1 ||
        ACE_OS::strcmp(objClientNameInfo.m_szName, "freeeyes") != 0 ||
        ACE_OS::strcmp(objClientNameInfo.m_szClientIP, "127.0.0.1") != 0 ||
        objClientNameInfo.m_nPort != 20022 ||
        objClientNameInfo.m_nLog != 1)
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_Tcp_Common_ClientNameInfo]Tcp_Common_ClientNameInfo is false.", true == blRet);
    }
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
    int nPacketLen = ACE_OS::strlen(szBodyBuff);

    memcpy(szSendUDP, (char*)&sVersion, sizeof(short));
    memcpy((char*)&szSendUDP[2], (char*)&sCommand, sizeof(short));
    memcpy((char*)&szSendUDP[4], (char*)&nPacketLen, sizeof(int));
    memcpy((char*)&szSendUDP[8], (char*)&szSession, sizeof(char) * 32);
    memcpy((char*)&szSendUDP[40], (char*)szBodyBuff, sizeof(char) * nPacketLen);
    uint32 u4SendLen = nPacketLen + 40;

    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(u4SendLen);
    memcpy_safe(szSendUDP, u4SendLen, pmb->wr_ptr(), u4SendLen);
    pmb->wr_ptr(u4SendLen);

    CPacketParse* pPacketParse = App_PacketParsePool::instance()->Create(__FILE__, __LINE__);

    if (false == Udp_Common_Recv_Stream(pmb, pPacketParse, 1))
    {
        OUR_DEBUG((LM_INFO, "[Test_Udp_Common_Recv_Stream]Udp_Common_Recv_Stream is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_Udp_Common_Recv_Stream]Udp_Common_Recv_Stream is fail.", true == blRet);
    }
    else
    {
        App_PacketParsePool::instance()->Delete(pPacketParse);
    }

    App_MessageBlockManager::instance()->Close(pmb);
}

void CUnit_Basehandler::Test_Udp_Common_Send_Message(void)
{
    bool blRet = false;
    ACE_Message_Block* pMbData = NULL;
    ACE_INET_Addr AddrRemote;
    ACE_SOCK_Dgram skRemote;

    uint32 u4Len = MAX_BUFF_20;
    char* pMessage = new char[u4Len];
    memset(pMessage, 0, u4Len);

    _Send_Message_Param obj_Send_Message_Param;
    obj_Send_Message_Param.m_u4PacketParseInfoID = 1;
    obj_Send_Message_Param.m_blDlete = false;
    obj_Send_Message_Param.m_blHead = true;
    obj_Send_Message_Param.m_nPort = 20002;
    obj_Send_Message_Param.m_pIP = (char*)"300.0.0.1";
    obj_Send_Message_Param.m_u2CommandID = 0x1002;
    obj_Send_Message_Param.m_u4Len = u4Len;

    //测试错误的IP地址
    bool blState = Udp_Common_Send_Message(obj_Send_Message_Param,
                                           AddrRemote,
                                           pMessage,
                                           pMbData,
                                           skRemote);

    if (blState == true)
    {
        OUR_DEBUG((LM_INFO, "[Test_Udp_Common_Send_Message]Udp_Common_Send_Message(300.0.0.1) is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_Udp_Common_Send_Message]Udp_Common_Send_Message(300.0.0.1) is fail.", true == blRet);
        return;
    }

    //测试正常的数据解析
    obj_Send_Message_Param.m_pIP = (char*)"127.0.0.1";
    blState = Udp_Common_Send_Message(obj_Send_Message_Param,
                                      AddrRemote,
                                      pMessage,
                                      pMbData,
                                      skRemote);

    if (blState == true)
    {
        OUR_DEBUG((LM_INFO, "[Test_Udp_Common_Send_Message]Udp_Common_Send_Message(127.0.0.1) is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_Udp_Common_Send_Message]Udp_Common_Send_Message(127.0.0.1) is fail.", true == blRet);
    }

}

#endif