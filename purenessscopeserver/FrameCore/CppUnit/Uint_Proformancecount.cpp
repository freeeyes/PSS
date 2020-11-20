#include "Uint_Proformancecount.h"

#ifdef _CPPUNIT_TEST

void CUnit_ProformanceCounter::setUp(void)
{
    m_pPerformanceCounter = std::make_shared<CPerformanceCounter>();
    m_pPerformanceCounter->init("Test");
}

void CUnit_ProformanceCounter::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_ProformanceCounter::tearDown]Finish.\n"));
}

void CUnit_ProformanceCounter::Test_ProformanceCounter(void)
{
    //���Դ���ָ���ķ�Ӧ��
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

