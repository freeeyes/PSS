#include "Unit_BaseHandler.h"

#ifdef _CPPUNIT_TEST

void CUnit_Basehandler::setUp(void)
{
}

void CUnit_Basehandler::tearDown(void)
{
}


void CUnit_Basehandler::Test_Udp_Common_Recv_Stream(void)
{
    bool blRet                 = false;
    ACE_Message_Block*     pMB = new ACE_Message_Block(4);
    uint32 u4PacketParseInfoID = 1;
    CPacketParse objPacketParse;

    blRet = Udp_Common_Recv_Stream(pMB, &objPacketParse, u4PacketParseInfoID);

    CPPUNIT_ASSERT_MESSAGE("[Test_Udp_Common_Recv_Stream]Udp_Common_Recv_Stream is false.", true == blRet);

    SAFE_DELETE(pMB);
}

void CUnit_Basehandler::Test_Common_Recv_Stream(void)
{
    bool blRet = false;
    ACE_Message_Block*     pMB = new ACE_Message_Block(4);
    uint32 u4PacketParseInfoID = 1;
    CPacketParse objPacketParse;

    uint8 u1Ret = Tcp_Common_Recv_Stream(1, pMB, &objPacketParse, u4PacketParseInfoID);

    if (PACKET_GET_ENOUGTH == u1Ret)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_Common_Recv_Stream]Tcp_Common_Recv_Stream is false.", true == blRet);

    SAFE_DELETE(pMB);
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

    //´æ·Å²âÊÔÊý¾Ý
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

#endif