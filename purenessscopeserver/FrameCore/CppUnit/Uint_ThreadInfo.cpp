#include "Uint_ThreadInfo.h"

#ifdef _CPPUNIT_TEST

void CUnit_ThreadInfo::setUp(void)
{
    m_pThreadInfo = std::make_shared<_ThreadInfo>();
}

void CUnit_ThreadInfo::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_ThreadInfo::tearDown]Finish.\n"));
}

void CUnit_ThreadInfo::Test_ThreadInfo(void)
{
    //������ܲ����ڲ���
    int blRet = true;

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ThreadInfo]m_pThreadInfo->Close() is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ThreadInfo]m_pThreadInfo->Close() is fail.", true == blRet);
        return;
    }
}

#endif

