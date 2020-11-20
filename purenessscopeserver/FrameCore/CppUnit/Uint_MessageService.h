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
    CUnit_MessageService() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_MessageService(void);

private:
    shared_ptr<CMessageService> m_pMessageService = nullptr;
};

#endif

#endif
