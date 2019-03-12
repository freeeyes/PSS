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
    CUnit_ForbiddenIP();

    virtual ~CUnit_ForbiddenIP();

    CUnit_ForbiddenIP(const CUnit_ForbiddenIP& ar);

    CUnit_ForbiddenIP& operator = (const CUnit_ForbiddenIP& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_ForbiddenIP(void);

private:
    CForbiddenIP* m_pForbiddenIP;
};

#endif

#endif
