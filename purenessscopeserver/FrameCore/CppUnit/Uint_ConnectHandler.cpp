#include "Uint_ConnectHandler.h"

#ifdef _CPPUNIT_TEST

CUnit_ConnectHandler::CUnit_ConnectHandler()
{
}

void CUnit_ConnectHandler::setUp(void)
{
    m_pConnectHandler = new CConnectHandler();

    m_pConnectHandler->Init(1);
    m_pConnectHandler->SetConnectID(111);
    m_pConnectHandler->SetPacketParseInfoID(1);
}

void CUnit_ConnectHandler::tearDown(void)
{
    m_pConnectHandler->CloseFinally();
    delete m_pConnectHandler;
    m_pConnectHandler = nullptr;
}

void CUnit_ConnectHandler::Test_ConnectHandler_Stream(void)
{
    //测试创建指定的反应器
    bool blRet = true;

    //拼装测试发送数据
    CBuffPacket send_buffpecket;
    string strBuff = "testtcp";
    string szSession = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

    _ClientConnectInfo objClientConnectInfo = m_pConnectHandler->GetClientInfo();

    if (111 != objClientConnectInfo.m_u4ConnectID)
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_Stream]GetClientInfo is fail(%d).\n", objClientConnectInfo.m_u4ConnectID));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_Stream]GetClientInfo is fail.", true == blRet);
        return;
    }

    IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    (*pBuffPacket) << (uint32)1;

    m_pConnectHandler->Write_SendData_To_File(false, pBuffPacket);

    uint16 u2PostCommand = 0x1001;
    uint32 u4PacketSize = 4;

    CSendMessageInfo objSendMessageInfo;
    objSendMessageInfo.emSendType  = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    objSendMessageInfo.u2CommandID = u2PostCommand;
    objSendMessageInfo.pBuffPacket = pBuffPacket;

    m_pConnectHandler->Send_Input_To_Cache(objSendMessageInfo, u4PacketSize);

    m_pConnectHandler->SetConnectName("127.0.0.1");

    if (false != m_pConnectHandler->GetIsLog())
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_Stream]GetIsLog is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_Stream]GetIsLog is fail.", true == blRet);
        return;
    }

    if (ACE_OS::strcmp(m_pConnectHandler->GetConnectName(), "127.0.0.1") != 0)
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_Stream]GetConnectName is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_Stream]GetConnectName is fail.", true == blRet);
        return;
    }

    //测试得到HashID
    m_pConnectHandler->SetHashID(111);

    if (111 != m_pConnectHandler->GetHashID())
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_Stream]GetHashID is fail(%d).\n", m_pConnectHandler->GetHashID()));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_Stream]GetHashID is fail.", true == blRet);
        return;
    }

    //测试以流模式解数据包
    short sVersion = 1;
    auto sCommand = (short)0x1000;
    auto nPacketLen = (int)strBuff.length();

    send_buffpecket << (uint16)sVersion;
    send_buffpecket << (uint16)sCommand;
    send_buffpecket << (uint32)nPacketLen;
    send_buffpecket.WriteStream(szSession.c_str(), szSession.length());
    send_buffpecket.WriteStream(strBuff.c_str(), strBuff.length());

    uint32 u4SendLen = send_buffpecket.GetPacketLen();
    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(u4SendLen);
    memcpy_safe(send_buffpecket.GetData(), u4SendLen, pmb->wr_ptr(), u4SendLen);
    pmb->wr_ptr(u4SendLen);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_Stream]Test_Paceket_Parse_Stream is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_Stream]Test_Paceket_Parse_Stream is fail.", true == blRet);
    }
}

void CUnit_ConnectHandler::Test_ConnectHandler_Debug(void)
{
    string strText = "freeeyes";

    auto pmb = new ACE_Message_Block(20);

    memcpy_safe(strText.c_str(), (uint32)strText.length(), pmb->wr_ptr(), (uint32)strText.length());
    pmb->wr_ptr(strText.length());

    m_pConnectHandler->SetIsLog(true);

    m_pConnectHandler->Output_Debug_Data(pmb, LOG_SYSTEM_DEBUG_CLIENTRECV);

    pmb->release();
}

void CUnit_ConnectHandler::Test_ConnectHandler_Close_Queue(void)
{
    bool blRet = false;
    blRet = App_HandlerManager::instance()->CloseConnect(1);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_Close_Queue]CloseConnect is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_Close_Queue]CloseConnect is fail.", true == blRet);
    }
    m_nTestCount++;
}

void CUnit_ConnectHandler::Test_ConnectHandler_PostMessage(void)
{
    bool blRet = false;

    IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    (*pBuffPacket) << (uint32)1;

    //测试群发数据;
    CSend_Param objSendParam;

    App_HandlerManager::instance()->PostMessage(1, 1001, pBuffPacket, objSendParam);

    App_HandlerManager::instance()->SetIsLog(1, false);

    _ClientIPInfo objClientIPInfo = App_HandlerManager::instance()->GetLocalIPInfo(1);

    if (objClientIPInfo.m_strClientIP != "")
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_PostMessage]GetLocalIPInfo is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_PostMessage]GetLocalIPInfo is fail.", true == blRet);
        return;
    }

    if (EM_Client_Connect_status::CLIENT_CONNECT_EXIST != App_HandlerManager::instance()->GetConnectState(1))
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_PostMessage]GetConnectState is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_PostMessage]GetConnectState is fail.", true == blRet);
    }

    App_BuffPacketManager::instance()->Delete(pBuffPacket);
    m_nTestCount++;
}

#endif

