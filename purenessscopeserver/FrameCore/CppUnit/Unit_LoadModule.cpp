#include "Unit_LoadModule.h"

#ifdef _CPPUNIT_TEST

void CUnit_LoadModule::setUp(void)
{
    sprintf_safe(m_szModuleName, MAX_BUFF_20, "TCPTest");
}

void CUnit_LoadModule::tearDown(void)
{
    sprintf_safe(m_szModuleName, MAX_BUFF_20, "TCPTest");
}

void CUnit_LoadModule::Test_LoadModule(void)
{
    bool blRet = false;
    char szModuleName[MAX_BUFF_20] = { '\0' };
    uint16 u2ModuleNameLen = MAX_BUFF_20;


    const char* pModuleFileName = App_ModuleLoader::instance()->GetModuleFileName(m_szModuleName);

    if (nullptr == pModuleFileName)
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_LoadModule]pModuleFileName is nullptr.", true == blRet);
        return;
    }

    const char* pModuleFilePath = App_ModuleLoader::instance()->GetModuleFilePath(m_szModuleName);

    if (nullptr == pModuleFilePath)
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_LoadModule]pModuleFilePath is nullptr.", true == blRet);
        return;
    }

    const char* pModuleFileDesc = App_ModuleLoader::instance()->GetModuleFileDesc(m_szModuleName);

    if (nullptr == pModuleFileDesc)
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_LoadModule]pModuleFileDesc is nullptr.", true == blRet);
        return;
    }

    if (1 != App_ModuleLoader::instance()->GetModuleCount())
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_LoadModule]ModuleCount is error.", true == blRet);
        return;
    }

    vector<shared_ptr<_ModuleInfo>> vecModeInfo;
    App_ModuleLoader::instance()->GetAllModuleInfo(vecModeInfo);

    if (false == App_ModuleLoader::instance()->GetAllModuleName(0, szModuleName, u2ModuleNameLen))
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_LoadModule]GetAllModuleName is error.", true == blRet);
    }
    m_nTestCount++;
}

#endif
