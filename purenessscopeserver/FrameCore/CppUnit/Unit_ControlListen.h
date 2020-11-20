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
    CUnit_ControlListen() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_ControlListen(void);

private:
    shared_ptr<CControlListen> m_pControlListen = nullptr;
};

#endif

#endif
