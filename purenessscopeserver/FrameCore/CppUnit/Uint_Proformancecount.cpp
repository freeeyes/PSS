#include "Uint_Proformancecount.h"

#ifdef _CPPUNIT_TEST

CUnit_ProformanceCounter::CUnit_ProformanceCounter()
{
}

void CUnit_ProformanceCounter::setUp(void)
{
    m_pPerformanceCounter = new CPerformanceCounter();
    m_pPerformanceCounter->init("Test");
}

void CUnit_ProformanceCounter::tearDown(void)
{
    delete m_pPerformanceCounter;
    m_pPerformanceCounter = nullptr;
}

void CUnit_ProformanceCounter::Test_ProformanceCounter(void)
{
    //测试创建指定的反应器
    int blRet = true;
    
    for (int i = 0; i < 1000; i++)
    {
        m_pPerformanceCounter->counter();
    }

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ProformanceCounter]AddNewReactor Reactor_Select is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ProformanceCounter]AddNewReactor Reactor_Select is fail.", true == blRet);
        return;
    }
}

#endif

