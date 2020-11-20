#ifndef UNIT_ACEREACTORMANAGER_H
#define UNIT_ACEREACTORMANAGER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "AceReactorManager.h"

class CUnit_AceReactorManager : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_AceReactorManager);
    CPPUNIT_TEST(Test_DoMessage_AceReactorManager);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_AceReactorManager() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_DoMessage_AceReactorManager(void);

private:
    shared_ptr<CAceReactorManager> m_pReactorManager = nullptr;
};

#endif

#endif
