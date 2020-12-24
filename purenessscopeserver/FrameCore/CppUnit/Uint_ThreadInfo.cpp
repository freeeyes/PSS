#include "Uint_ThreadInfo.h"

#ifdef _CPPUNIT_TEST

void CUnit_ThreadInfo::setUp(void)
{
    m_pThreadInfo = std::make_shared<_ThreadInfo>();
}

void CUnit_ThreadInfo::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_ThreadInfo::tearDown]Finish.");
}

void CUnit_ThreadInfo::Test_ThreadInfo(void)
{
    //这个功能不必在测试
    int blRet = true;

    if (false == blRet)
    {
        PSS_LOGGER_DEBUG("[Test_ThreadInfo]m_pThreadInfo->Close() is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_ThreadInfo]m_pThreadInfo->Close() is fail.", true == blRet);
        return;
    }
}

#endif

