#ifndef _UNIT_PROFILE_H
#define _UNIT_PROFILE_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ProfileTime.h"

class CUnit_ProfileTime : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ProfileTime);
    CPPUNIT_TEST(Test_ProfileTime);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ProfileTime();

    virtual ~CUnit_ProfileTime();

    CUnit_ProfileTime(const CUnit_ProfileTime& ar);

    CUnit_ProfileTime& operator = (const CUnit_ProfileTime& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_ProfileTime(void);

private:
    CProfileTime* m_pProfileTime;
};

#endif

#endif
