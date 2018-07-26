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
    virtual ~CUnit_ThreadInfo();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_ThreadInfo(void);

private:
    CThreadInfo* m_pThreadInfo;
};

#endif

#endif
