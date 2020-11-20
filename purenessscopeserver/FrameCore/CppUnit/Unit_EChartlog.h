#ifndef _UNIT_ECHARTLOG_H
#define _UNIT_ECHARTLOG_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "Echartlog.h"

class CUnit_EChartlog : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_EChartlog);
    CPPUNIT_TEST(Test_EChartlog);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_EChartlog() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_EChartlog(void);

private:
    IEchartlog* m_pEchartlog = nullptr;
};

#endif

#endif
