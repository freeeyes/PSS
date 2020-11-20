#ifndef _UNIT_TIMEQUEUE_H
#define _UNIT_TIMEQUEUE_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "TimerManager.h"

//定时器处理函数
class CTimeTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CTimeTask(void) = default;

    int handle_timeout(const ACE_Time_Value& tv, const void* arg) final;
};

class CUnit_TimerManager : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_TimerManager);
    CPPUNIT_TEST(Test_TimerManager);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_TimerManager() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_TimerManager(void);

private:
    CTimeTask* m_pTimeTask = nullptr;
};

#endif

#endif
