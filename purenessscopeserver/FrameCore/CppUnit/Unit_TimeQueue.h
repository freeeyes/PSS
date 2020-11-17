#ifndef _UNIT_TIMEQUEUE_H
#define _UNIT_TIMEQUEUE_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "TimerManager.h"

//��ʱ��������
class CTimeTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CTimeTask(void);

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);
};

class CUnit_TimerManager : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_TimerManager);
    CPPUNIT_TEST(Test_TimerManager);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_TimerManager();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_TimerManager(void);

private:
    CTimeTask* m_pTimeTask = nullptr;
};

#endif

#endif
