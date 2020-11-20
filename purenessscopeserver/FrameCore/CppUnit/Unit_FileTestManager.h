#ifndef UNIT_FILETESTMANAGER_H
#define UNIT_FILETESTMANAGER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "FileTestManager.h"

class CUnit_FileTestManager : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_FileTestManager);
    CPPUNIT_TEST(Test_FileTestStart);
    CPPUNIT_TEST(Test_handle_timeout);
    CPPUNIT_TEST(Test_FileTestStop);
    CPPUNIT_TEST_SUITE_END();
public:
    CUnit_FileTestManager() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_FileTestStart(void);
    void Test_FileTestStop(void);
    void Test_handle_timeout(void);

private:
    shared_ptr<CFileTestManager> m_pFileTestManager = nullptr;
};

#endif

#endif
