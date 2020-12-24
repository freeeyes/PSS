#include "Uint_Proformancecount.h"

#ifdef _CPPUNIT_TEST

void CUnit_ProformanceCounter::setUp(void)
{
    m_pPerformanceCounter = std::make_shared<CPerformanceCounter>();
    m_pPerformanceCounter->init("Test");
}

void CUnit_ProformanceCounter::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_ProformanceCounter::tearDown]Finish.");
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
        PSS_LOGGER_DEBUG("[Test_ProformanceCounter]AddNewReactor Reactor_Select is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_ProformanceCounter]AddNewReactor Reactor_Select is fail.", true == blRet);
        return;
    }
}

#endif

