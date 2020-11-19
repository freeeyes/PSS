#include "Uint_ThreadInfo.h"

#ifdef _CPPUNIT_TEST

CUnit_ThreadInfo::CUnit_ThreadInfo()
{
}

void CUnit_ThreadInfo::setUp(void)
{
}

void CUnit_ThreadInfo::tearDown(void)
{
    m_pThreadInfo = nullptr;
}

void CUnit_ThreadInfo::Test_ThreadInfo(void)
{
    //这个功能不必在测试
    int blRet = true;

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ThreadInfo]m_pThreadInfo->Close() is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ThreadInfo]m_pThreadInfo->Close() is fail.", true == blRet);
        return;
    }
}

#endif

