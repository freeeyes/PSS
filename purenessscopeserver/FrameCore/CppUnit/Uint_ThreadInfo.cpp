#include "Uint_ThreadInfo.h"

#ifdef _CPPUNIT_TEST

CUnit_ThreadInfo::CUnit_ThreadInfo()
{
    m_pThreadInfo = NULL;
}

void CUnit_ThreadInfo::setUp(void)
{
    m_pThreadInfo = new CThreadInfoList();
    m_pThreadInfo->Init(3);
}

void CUnit_ThreadInfo::tearDown(void)
{
    m_pThreadInfo->Close();
    delete m_pThreadInfo;
    m_pThreadInfo = NULL;
}

void CUnit_ThreadInfo::Test_ThreadInfo(void)
{
    //测试创建指定的反应器
    int blRet = false;

    blRet = m_pThreadInfo->AddThreadInfo(0);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ThreadInfo]m_pThreadInfo->AddThreadInfo() is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ThreadInfo]m_pThreadInfo->AddThreadInfo() is fail.", true == blRet);
        return;
    }

    _ThreadInfo* pThreadInfo = m_pThreadInfo->GetThreadInfo(0);

    if (NULL == pThreadInfo)
    {
        OUR_DEBUG((LM_INFO, "[Test_ThreadInfo]m_pThreadInfo->GetThreadInfo(0) is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ThreadInfo]m_pThreadInfo->GetThreadInfo(0) is fail.", true == blRet);
        return;
    }

    blRet = m_pThreadInfo->CloseThread(0);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ThreadInfo]m_pThreadInfo->CloseThread() is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ThreadInfo]m_pThreadInfo->CloseThread() is fail.", true == blRet);
        return;
    }

    //测试关闭清理对象
    blRet = m_pThreadInfo->AddThreadInfo(0);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ThreadInfo]m_pThreadInfo->Close() is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ThreadInfo]m_pThreadInfo->Close() is fail.", true == blRet);
        return;
    }

    m_pThreadInfo->Close();

}

#endif

