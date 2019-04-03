#ifndef UNIT_MESSAGESERVICE_H
#define UNIT_MESSAGESERVICE_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "MessageService.h"

class CUnit_MessageService : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_MessageService);
    CPPUNIT_TEST(Test_MessageService);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_MessageService();

    virtual ~CUnit_MessageService();

    CUnit_MessageService(const CUnit_MessageService& ar);

    CUnit_MessageService& operator = (const CUnit_MessageService& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_MessageService(void);

private:
    CMessageService* m_pMessageService;
};

#endif

#endif
