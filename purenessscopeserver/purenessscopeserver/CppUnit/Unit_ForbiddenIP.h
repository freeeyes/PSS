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
    virtual ~CUnit_ForbiddenIP();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_ForbiddenIP(void);

private:
    CForbiddenIP* m_pForbiddenIP;
};

#endif

#endif
