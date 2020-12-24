#include "Unit_ForbiddenIP.h"

#ifdef _CPPUNIT_TEST

void CUnit_ForbiddenIP::setUp(void)
{
    m_pForbiddenIP = std::make_shared <CForbiddenIP>();
}

void CUnit_ForbiddenIP::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_ForbiddenIP::tearDown]Finish.");
}

void CUnit_ForbiddenIP::Test_ForbiddenIP(void)
{
    bool blRet = false;

    //¼ì²âÁÙÊ±½ûÖ¹IP
    if (false == m_pForbiddenIP->AddTempIP("127.0.1.0", 20, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP))
    {
        PSS_LOGGER_DEBUG("[Test_ForbiddenIP] m_pForbiddenIP->AddTempIP() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_ForbiddenIP] m_pForbiddenIP->AddTempIP() Error.", true == blRet);
        return;
    }

    if (true == m_pForbiddenIP->CheckIP("127.0.1.0", EM_CONNECT_IO_TYPE::CONNECT_IO_TCP))
    {
        PSS_LOGGER_DEBUG("[Test_ForbiddenIP] m_pForbiddenIP->CheckIP() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_ForbiddenIP] m_pForbiddenIP->CheckIP() Error.", true == blRet);
    }
}

#endif
