#ifndef _UNIT_FILELOG_H
#define _UNIT_FILELOG_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "FileLogger.h"

class CUnit_FileLogger : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_FileLogger);
    CPPUNIT_TEST(Test_FileLogger);
    CPPUNIT_TEST(Test_SendMail);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_FileLogger() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_FileLogger(void);

    void Test_SendMail(void);

private:
    shared_ptr<CLogFile> m_pLogFile = nullptr;
    int m_nTestCount = 0;
};

#endif

#endif
