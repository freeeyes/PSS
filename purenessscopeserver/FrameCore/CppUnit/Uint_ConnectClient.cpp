#include "Uint_ConnectClient.h"

#ifdef _CPPUNIT_TEST

void CUnit_ConnectClient::setUp(void)
{
    m_pConnectClient = std::make_shared<CConnectClient>();
}

void CUnit_ConnectClient::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_ConnectClient::tearDown]Finish.");
}

void CUnit_ConnectClient::Test_handle_input(void)
{
    //测试创建指定的反应器
    bool blRet = false;

    int nRet = m_pConnectClient->handle_input();

    if (-1 != nRet)
    {
        PSS_LOGGER_DEBUG("[Test_handle_input]handle_input is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_handle_input]handle_input is fail.", true == blRet);
        return;
    }

    nRet = m_pConnectClient->handle_input(1);

    if (-1 != nRet)
    {
        PSS_LOGGER_DEBUG("[Test_handle_input]handle_input m_pCurrMessage is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_handle_input]handle_input m_pCurrMessage is fail.", true == blRet);
    }
}

void CUnit_ConnectClient::Test_GetTimeout(void)
{
    bool blRet = false;
    auto tvNow = CTimeStamp::Get_Time_Stamp();
    m_pConnectClient->GetTimeout(tvNow);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetTimeout]GetTimeout is fail.", false == blRet);
    m_nTestCount++;
}

void CUnit_ConnectClient::Test_Output_Debug_Data(void)
{
    uint32 u4Data = 10;
    ACE_Message_Block* pMbData = App_MessageBlockManager::instance()->Create(4);
    memcpy_safe((char* )&u4Data, 4, pMbData->wr_ptr(), 4);
    pMbData->wr_ptr(4);

    ACE_INET_Addr addrTest;
    addrTest.set(10002, "127.0.0.1");
    Output_Debug_Data(pMbData, LOG_SYSTEM_DEBUG_CLIENTRECV, addrTest);
    m_nTestCount++;
}

#endif

