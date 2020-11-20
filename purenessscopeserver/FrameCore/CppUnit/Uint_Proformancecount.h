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
    CUnit_ProformanceCounter() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_ProformanceCounter(void);

private:
    shared_ptr<CPerformanceCounter> m_pPerformanceCounter = nullptr;
};

#endif

#endif
