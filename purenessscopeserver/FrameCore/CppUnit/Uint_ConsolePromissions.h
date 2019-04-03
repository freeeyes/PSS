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
    CUnit_ConsolePromissions();

    virtual ~CUnit_ConsolePromissions();

    CUnit_ConsolePromissions(const CUnit_ConsolePromissions& ar);

    CUnit_ConsolePromissions& operator = (const CUnit_ConsolePromissions& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }


    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_Promission_User(void);
private:
    CConsolePromissions* m_pConsolePromissions;
};

#endif

#endif
