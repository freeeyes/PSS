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
    CUnit_EChartlog();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_EChartlog(void);

private:
    IEchartlog* m_pEchartlog;
};

#endif

#endif
