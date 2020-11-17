#include "Unit_ForbiddenIP.h"

#ifdef _CPPUNIT_TEST

CUnit_ForbiddenIP::CUnit_ForbiddenIP()
{
}

void CUnit_ForbiddenIP::setUp(void)
{
    m_pForbiddenIP = new CForbiddenIP();
}

void CUnit_ForbiddenIP::tearDown(void)
{
    delete m_pForbiddenIP;
    m_pForbiddenIP = nullptr;
}

void CUnit_ForbiddenIP::Test_ForbiddenIP(void)
{
    bool blRet = false;

    //�����ʱ��ֹIP
    if (false == m_pForbiddenIP->AddTempIP("127.0.1.0", 20, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP))
    {
        OUR_DEBUG((LM_INFO, "[Test_ForbiddenIP] m_pForbiddenIP->AddTempIP() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ForbiddenIP] m_pForbiddenIP->AddTempIP() Error.", true == blRet);
        return;
    }

    if (true == m_pForbiddenIP->CheckIP("127.0.1.0", EM_CONNECT_IO_TYPE::CONNECT_IO_TCP))
    {
        OUR_DEBUG((LM_INFO, "[Test_ForbiddenIP] m_pForbiddenIP->CheckIP() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ForbiddenIP] m_pForbiddenIP->CheckIP() Error.", true == blRet);
    }
}

#endif
