#ifndef _UNIT_LOADMODULE_H
#define _UNIT_LOADMODULE_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "LoadModule.h"

class CUnit_LoadModule : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_LoadModule);
    CPPUNIT_TEST(Test_LoadModule);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_LoadModule() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_LoadModule(void);

private:
    char m_szModuleName[MAX_BUFF_20] = {'\0'};
    int m_nTestCount = 0;
};

#endif

#endif
