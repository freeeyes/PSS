#include "Unit_AppConfig.h"

#ifdef _CPPUNIT_TEST

CUnit_AppConfig::~CUnit_AppConfig()
{
}

void CUnit_AppConfig::setUp(void)
{
    m_pAppConfig = new CAppConfig();
}


void CUnit_AppConfig::tearDown(void)
{
    delete m_pAppConfig;
    m_pAppConfig = NULL;
}

void CUnit_AppConfig::Test_XML_Read(void)
{
    bool blRet = false;

    if (false == m_pAppConfig->ReadConfig("CppUnitTest.ini"))
    {
        OUR_DEBUG((LM_INFO, "[Test_XML_Read]Read CppUnitTest.xml is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_XML_Read]Read CppUnitTest.xml is fail.", true == blRet);
        return;
    }

    ACE_TString strValue;

    if (false == m_pAppConfig->GetValue("CppUnitTest", strValue, "Config"))
    {
        OUR_DEBUG((LM_INFO, "[Test_XML_Read]Read CppUnitTest.xml CppUnitTest is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_XML_Read]Read CppUnitTest.xml CppUnitTest is fail.", true == blRet);
        return;
    }

    if (ACE_OS::strcmp(strValue.c_str(), "aaa") == 0)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_XML_Read]Read CppUnitTest.xml strValue is error.", true == blRet);
}

void CUnit_AppConfig::Test_XML_Write(void)
{
    bool blRet = false;

    if (false == m_pAppConfig->ReadConfig("CppUnitTest.ini"))
    {
        OUR_DEBUG((LM_INFO, "[Test_XML_Write]Read CppUnitTest.xml is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_XML_Write]Read CppUnitTest.xml is fail.", true == blRet);
        return;
    }

    ACE_TString strValue = "bbb";

    if (false == m_pAppConfig->SetValue("CppUnitTestW", strValue, "Config"))
    {
        OUR_DEBUG((LM_INFO, "[Test_XML_Write]Write CppUnitTest.xml CppUnitTest is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_XML_Write]Write CppUnitTest.xml CppUnitTest is fail.", true == blRet);
        return;
    }

    if (true == m_pAppConfig->WriteConfig() && m_pAppConfig->WriteConfig("ccc.xml"))
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_XML_Read]Write CppUnitTest.xml is error.", true == blRet);
}

#endif
