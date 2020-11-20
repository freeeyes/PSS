#ifndef UNIT_CONSOLEPROMISSIONS_H
#define UNIT_CONSOLEPROMISSIONS_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ConsolePromiss.h"

class CUnit_ConsolePromissions : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ConsolePromissions);
    CPPUNIT_TEST(Test_Promission_User);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ConsolePromissions() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_Promission_User(void);
private:
    shared_ptr<CConsolePromissions> m_pConsolePromissions = nullptr;
};

#endif

#endif
