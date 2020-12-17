#ifndef UNIT_SERVERMESSAGEINFOPOOL_H
#define UNIT_SERVERMESSAGEINFOPOOL_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "define.h"

class CUnit_ServerMessageInfoPool : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ServerMessageInfoPool);
    CPPUNIT_TEST(Test_ServerMessageInfoPool);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ServerMessageInfoPool() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_ServerMessageInfoPool(void);

private:
    int nCount = 0;
};

#endif

#endif
