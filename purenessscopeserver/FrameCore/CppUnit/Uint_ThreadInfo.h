#ifndef UNIT_THREADINFO_H
#define UNIT_THREADINFO_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ThreadInfo.h"

class CUnit_ThreadInfo : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ThreadInfo);
    CPPUNIT_TEST(Test_ThreadInfo);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ThreadInfo() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_ThreadInfo(void);

private:
    shared_ptr<_ThreadInfo> m_pThreadInfo = nullptr;
};

#endif

#endif
