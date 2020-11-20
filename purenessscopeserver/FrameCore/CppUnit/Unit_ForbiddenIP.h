#ifndef _UNIT_FORBIDDENIP_H
#define _UNIT_FORBIDDENIP_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ForbiddenIP.h"

class CUnit_ForbiddenIP : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ForbiddenIP);
    CPPUNIT_TEST(Test_ForbiddenIP);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ForbiddenIP() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_ForbiddenIP(void);

private:
    shared_ptr<CForbiddenIP> m_pForbiddenIP = nullptr;
};

#endif

#endif
