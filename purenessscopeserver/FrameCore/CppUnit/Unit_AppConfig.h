#ifndef UNIT_APPCONFIG_H
#define UNIT_APPCONFIG_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "AppConfig.h"

class CUnit_AppConfig : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_AppConfig);
    CPPUNIT_TEST(Test_XML_Read);
    CPPUNIT_TEST(Test_XML_Write);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_AppConfig() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_XML_Read(void);
    void Test_XML_Write(void);
private:
    shared_ptr<CAppConfig> m_pAppConfig = nullptr;
};

#endif

#endif
