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
    m_pConnectHandler = NULL;
}

void CUnit_ConnectHandler::Test_ConnectHandler_Stream(void)
{
    //测试创建指定的反应器
    bool blRet = false;

    //拼装测试发送数据
    char szSendData[MAX_BUFF_200] = { '\0' };
    char szBuff[20] = { '\0' };
    char szSession[32] = { '\0' };
    sprintf_safe(szBuff, 20, "testtcp");
    sprintf_safe(szSession, 32, "FREEEYES");

    //测试设置执行时间接口
    m_pConnectHandler->SetRecvQueueTimeCost(100000);

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

    CSendCacheManager objSendCacheManager;
    objSendCacheManager.Init(1, 1000);

    m_pConnectHandler->SetSendCacheManager(&objSendCacheManager);

    uint16 u2PostCommand = 0x1001;
    uint32 u4PacketSize = 4;
    m_pConnectHandler->Send_Input_To_Cache(SENDMESSAGE_JAMPNOMAL, u4PacketSize, u2PostCommand, true, pBuffPacket);

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

    ACE_Time_Value tvNow = ACE_OS::gettimeofday();
    App_ConnectManager::instance()->GetManagerFormList(0)->handle_timeout(tvNow, NULL);

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
    short sCommand = (short)0x1000;
    int nPacketLen = (int)ACE_OS::strlen(szBuff);

    memcpy(szSendData, (char*)&sVersion, sizeof(short));
    memcpy(&szSendData[2], (char*)&sCommand, sizeof(short));
    memcpy(&szSendData[4], (char*)&nPacketLen, sizeof(int));
    memcpy(&szSendData[8], (char*)&szSession, sizeof(char) * 32);
    memcpy(&szSendData[40], (char*)szBuff, sizeof(char) * nPacketLen);
    uint32 u4SendLen = nPacketLen + 40;

    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(u4SendLen);
    memcpy_safe(szSendData, u4SendLen, pmb->wr_ptr(), u4SendLen);
    pmb->wr_ptr(u4SendLen);

    blRet = m_pConnectHandler->Test_Paceket_Parse_Stream(pmb);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_Stream]Test_Paceket_Parse_Stream is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_Stream]Test_Paceket_Parse_Stream is fail.", true == blRet);
    }
}

void CUnit_ConnectHandler::Test_ConnectHandler_CloseMessages(void)
{
    bool blRet = false;

    blRet = m_pConnectHandler->SendCloseMessage();

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_CloseMessages]SendCloseMessage is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_CloseMessages]SendCloseMessage is fail.", true == blRet);
    }
}

void CUnit_ConnectHandler::Test_ConnectHandler_Debug(void)
{
    char szText[MAX_BUFF_20] = { '\0' };

    sprintf_safe(szText, MAX_BUFF_20, "freeeyes");

    ACE_Message_Block* pmb = new ACE_Message_Block(20);

    memcpy_safe(szText, (uint32)ACE_OS::strlen(szText), pmb->wr_ptr(), (uint32)ACE_OS::strlen(szText));
    pmb->wr_ptr(ACE_OS::strlen(szText));

    m_pConnectHandler->SetIsLog(true);

    m_pConnectHandler->Output_Debug_Data(pmb, LOG_SYSTEM_DEBUG_CLIENTRECV);

    pmb->release();
}

void CUnit_ConnectHandler::Test_ConnectHandler_Close_Queue(void)
{
    bool blRet = false;
    blRet = App_ConnectManager::instance()->CloseConnect(1);

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
    char szData[10] = { '\0' };
    sprintf_safe(szData, 10, "freeeyes");

    IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    (*pBuffPacket) << (uint32)1;

    //测试断开连接
    App_ConnectManager::instance()->Close(1);

    App_ConnectManager::instance()->CloseUnLock(1);

    App_ConnectManager::instance()->SetRecvQueueTimeCost(1, 1000);

    //测试群发数据
    char* ptrReturnData = szData;
    uint32 u4SendLen = (uint32)ACE_OS::strlen(szData);

    vector<uint32> vecConnectIDList;
    vecConnectIDList.push_back(1);

    App_ConnectManager::instance()->PostMessage(1, ptrReturnData, u4SendLen, SENDMESSAGE_NOMAL, 0, true, false, 0);

    App_ConnectManager::instance()->PostMessage(vecConnectIDList, ptrReturnData, u4SendLen, SENDMESSAGE_NOMAL, 0, true, false, 0);

    App_ConnectManager::instance()->PostMessage(vecConnectIDList, pBuffPacket, SENDMESSAGE_NOMAL, 0, true, false, 0);

    App_ConnectManager::instance()->PostMessageAll(ptrReturnData, u4SendLen, SENDMESSAGE_NOMAL, 0, true, false, 0);

    App_ConnectManager::instance()->PostMessageAll(ptrReturnData, u4SendLen, SENDMESSAGE_NOMAL, 0, false, false, 0);

    App_ConnectManager::instance()->PostMessageAll(pBuffPacket, SENDMESSAGE_NOMAL, 0, true, false, 0);

    App_ConnectManager::instance()->SetConnectName(1, "freeeyes");

    App_ConnectManager::instance()->SetIsLog(1, false);

    _ClientIPInfo objClientIPInfo = App_ConnectManager::instance()->GetLocalIPInfo(1);

    if (strcmp(objClientIPInfo.m_szClientIP, "") != 0)
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_PostMessage]GetLocalIPInfo is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_PostMessage]GetLocalIPInfo is fail.", true == blRet);
        return;
    }

    if (CLIENT_CONNECT_NO_EXIST != App_ConnectManager::instance()->GetConnectState(1))
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler_PostMessage]GetConnectState is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler_PostMessage]GetConnectState is fail.", true == blRet);
    }

    App_BuffPacketManager::instance()->Delete(pBuffPacket);
    m_nTestCount++;
}

void CUnit_ConnectHandler::Test_Connect_CheckTime(void)
{
    bool blRet = false;

    uint16 u2CheckTime = App_ConnectManager::instance()->GetConnectCheckTime();

    if(u2CheckTime != 60)
    {
        OUR_DEBUG((LM_INFO, "[Test_Connect_CheckTime]GetLocalIPInfo is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_CheckTime]GetLocalIPInfo is fail.", true == blRet);
        return;
    }

    m_nTestCount++;
}

#endif

