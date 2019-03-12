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

    virtual ~CUnit_ControlListen();

    CUnit_ControlListen(const CUnit_ControlListen& ar);

    CUnit_ControlListen& operator = (const CUnit_ControlListen& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_ControlListen(void);

private:
    CControlListen* m_pControlListen;
};

#endif

#endif
