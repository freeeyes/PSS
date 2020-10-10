#ifndef UNIT_PROFORMANCECOUNT_H
#define UNIT_PROFORMANCECOUNT_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "PerformanceCounter.h"

class CUnit_ProformanceCounter : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ProformanceCounter);
    CPPUNIT_TEST(Test_ProformanceCounter);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ProformanceCounter();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_ProformanceCounter(void);

private:
    CPerformanceCounter* m_pPerformanceCounter = NULL;
};

#endif

#endif
