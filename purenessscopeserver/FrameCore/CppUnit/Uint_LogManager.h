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
    CUnit_LogManager();

    virtual ~CUnit_LogManager();

    CUnit_LogManager(const CUnit_LogManager& ar);

    CUnit_LogManager& operator = (const CUnit_LogManager& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_LogBlockPool(void);

    void Test_Write_Log(void);

private:
    CLogBlockPool* m_pLogBlockPool;
};

#endif

#endif
