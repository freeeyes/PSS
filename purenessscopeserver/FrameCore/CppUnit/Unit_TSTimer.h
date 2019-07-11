#ifndef _UNIT_TSTIMER_H
#define _UNIT_TSTIMER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "define.h"
#include "TimerThread.h"

class CTestTimeNode : public ts_timer::ITimeNode
{
public:
    CTestTimeNode() {};

    virtual ~CTestTimeNode() = default;

    virtual void Run(ts_timer::CTime_Value& tvNow, void* pArg, ts_timer::EM_Timer_State& emState)
    {
        ACE_UNUSED_ARG(tvNow);
        ACE_UNUSED_ARG(pArg);
        ACE_UNUSED_ARG(emState);

        OUR_DEBUG((LM_INFO, "[CTestTimeNode::Run]OK.\n"));
    }

    virtual void Error(int nLastRunTimerID, int nTimeoutTime, std::vector<ts_timer::CTime_Value>& vecTimoutList, void* pArg)
    {
        ACE_UNUSED_ARG(nLastRunTimerID);
        ACE_UNUSED_ARG(nTimeoutTime);
        ACE_UNUSED_ARG(vecTimoutList);
        ACE_UNUSED_ARG(pArg);

        OUR_DEBUG((LM_INFO, "[CTestTimeNode::Error]OK.\n"));
    }
};

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
    ts_timer::CTimerThread* m_pTimerThread;
    CTestTimeNode*          m_pThreadNode;
};

#endif

#endif
