#ifndef _UNIT_WORKTHREADAI_H
#define _UNIT_WORKTHREADAI_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "WorkThreadAI.h"

class CUnit_WorkThreadAI : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_WorkThreadAI);
    CPPUNIT_TEST(Test_WorkThreadAI);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_WorkThreadAI();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_WorkThreadAI(void);

private:
    CWorkThreadAI* m_pWorkThreadAI = nullptr;
};

#endif

#endif
