#ifndef _UNIT_CONTROLLISTEN_H
#define _UNIT_CONTROLLISTEN_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ControlListen.h"

class CUnit_ControlListen : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ControlListen);
    CPPUNIT_TEST(Test_ControlListen);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ControlListen();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_ControlListen(void);

private:
    CControlListen* m_pControlListen;
};

#endif

#endif
