#ifndef UNIT_SERVERMESSAGEMANAGER_H
#define UNIT_SERVERMESSAGEMANAGER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "PostServerMessage.h"

class CUnit_ServerMessageManager : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ServerMessageManager);
    CPPUNIT_TEST(Test_ServerMessageManager);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ServerMessageManager() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_ServerMessageManager(void);

private:
    int nCount = 0;
};

#endif

#endif
