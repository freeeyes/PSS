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
    CUnit_LoadModule();

    virtual ~CUnit_LoadModule();

    CUnit_LoadModule(const CUnit_LoadModule& ar);

    CUnit_LoadModule& operator = (const CUnit_LoadModule& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_LoadModule(void);

private:
    char m_szModuleName[MAX_BUFF_20];
};

#endif

#endif
