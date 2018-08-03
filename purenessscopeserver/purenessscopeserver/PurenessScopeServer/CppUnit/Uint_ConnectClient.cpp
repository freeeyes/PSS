#include "Uint_ConnectClient.h"

#ifdef _CPPUNIT_TEST

CUnit_ConnectClient::~CUnit_ConnectClient()
{
    m_pConnectClient = NULL;
}

void CUnit_ConnectClient::setUp(void)
{
    m_pConnectClient = new CConnectClient();
}

void CUnit_ConnectClient::tearDown(void)
{
    delete m_pConnectClient;
    m_pConnectClient = NULL;
}

void CUnit_ConnectClient::Test_handle_input(void)
{
    //测试创建指定的反应器
    bool blRet = false;

    int nRet = m_pConnectClient->handle_input();

    if (-1 != nRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_handle_input]handle_input is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_handle_input]handle_input is fail.", true == blRet);
        return;
    }

    nRet = m_pConnectClient->handle_input(1);

    if (-1 != nRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_handle_input]handle_input m_pCurrMessage is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_handle_input]handle_input m_pCurrMessage is fail.", true == blRet);
    }
}

void CUnit_ConnectClient::Test_GetTimeout(void)
{
    bool blRet = false;
    ACE_Time_Value tvNow = ACE_OS::gettimeofday();
    m_pConnectClient->GetTimeout(tvNow);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetTimeout]GetTimeout is fail.", false == blRet);
}

#endif

