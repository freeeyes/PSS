#ifndef _UNIT_TSTIMER_H
#define _UNIT_TSTIMER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "define.h"
#include "TimerEvent.hpp"

class CUnit_TimerThread : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_TimerThread);
    CPPUNIT_TEST(Test_TimerThread);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_TimerThread();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_TimerThread(void);

private:
    CTimerManager*          m_pTimerThread;
};

#endif

#endif
