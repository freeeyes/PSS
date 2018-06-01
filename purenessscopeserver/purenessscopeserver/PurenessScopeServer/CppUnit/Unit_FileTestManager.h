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

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_FileTestStart(void);
    void Test_FileTestStop(void);
    void Test_handle_timeout(void);

protected:
    CFileTestManager* m_pFileTestManager;
};

#endif

#endif
