#ifndef UNIT_LOGMANAGER_H
#define UNIT_LOGMANAGER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "LogManager.h"

class CUnit_LogManager : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_LogManager);
    CPPUNIT_TEST(Test_LogBlockPool);
    CPPUNIT_TEST(Test_Write_Log);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_LogManager() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_LogBlockPool(void);

    void Test_Write_Log(void);

private:
    shared_ptr<CLogBlockPool> m_pLogBlockPool = nullptr;
    int m_nTestCount = 0;
};

#endif

#endif
