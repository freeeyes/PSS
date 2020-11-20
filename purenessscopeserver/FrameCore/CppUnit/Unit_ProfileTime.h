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
    CUnit_ProfileTime() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_ProfileTime(void);

private:
    shared_ptr<CProfileTime> m_pProfileTime = nullptr;
};

#endif

#endif
