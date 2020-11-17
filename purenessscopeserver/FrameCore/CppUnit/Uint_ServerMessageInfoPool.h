#ifndef UNIT_SERVERMESSAGEINFOPOOL_H
#define UNIT_SERVERMESSAGEINFOPOOL_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ServerMessageTask.h"

class CUnit_ServerMessageInfoPool : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ServerMessageInfoPool);
    CPPUNIT_TEST(Test_ServerMessageInfoPool);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ServerMessageInfoPool();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_ServerMessageInfoPool(void);

private:
    CServerMessageInfoPool* m_pServerMessageInfoPool = nullptr;
};

#endif

#endif
