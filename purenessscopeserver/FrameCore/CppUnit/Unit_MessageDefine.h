#ifndef _UNIT_MESSAGEDEFINE_H
#define _UNIT_MESSAGEDEFINE_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "MessageDefine.h"

class CUnit_MessageDefine : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_MessageDefine);
    CPPUNIT_TEST(Test_GetDyeingCommand);
    CPPUNIT_TEST(Test_GetDyeingIP);
    CPPUNIT_TEST(Test_GetFileInfo);
    CPPUNIT_TEST(Test_GetTrackIP);
    CPPUNIT_TEST(Test_GetLogLevel);
    CPPUNIT_TEST(Test_GetAIInfo);
    CPPUNIT_TEST(Test_GetConnectID);
    CPPUNIT_TEST(Test_GetMaxConnectCount);
    CPPUNIT_TEST(Test_GetListenInfo);
    CPPUNIT_TEST(Test_GetPoolSet);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_MessageDefine();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_GetDyeingCommand(void) const;
    void Test_GetDyeingIP(void) const;
    void Test_GetFileInfo(void) const;
    void Test_GetTrackIP(void) const;
    void Test_GetLogLevel(void) const;
    void Test_GetAIInfo(void) const;
    void Test_GetConnectID(void) const;
    void Test_GetMaxConnectCount(void) const;
    void Test_GetListenInfo(void) const;
    void Test_GetPoolSet(void) const;
private:
    int m_nMessageDefine = 0;
};

#endif

#endif
