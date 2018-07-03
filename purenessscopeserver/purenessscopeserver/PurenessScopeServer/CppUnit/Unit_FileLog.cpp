#include "Unit_FileLog.h"

#ifdef _CPPUNIT_TEST

CUnit_FileLogger::~CUnit_FileLogger()
{

}

void CUnit_FileLogger::setUp(void)
{
    m_pLogFile = new CLogFile("./", 8192, 1024);
}

void CUnit_FileLogger::tearDown(void)
{
    SAFE_DELETE(m_pLogFile);
}

void CUnit_FileLogger::Test_FileLogger(void)
{
    bool blRet = false;

    m_pLogFile->SetLoggerName("CppunitTest");
    m_pLogFile->SetLoggerID(2000);
    m_pLogFile->SetLoggerClass(1);
    m_pLogFile->SetLevel(1);
    m_pLogFile->SetServerName("127_");
    m_pLogFile->SetDisplay(0);
    m_pLogFile->Init();

    if (false == m_pLogFile->Run())
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->Run() fail.", true == blRet);
        return;
    }

    _LogBlockInfo objLogBlockInfo;
    char szData[20] = { '\0' };
    sprintf_safe(szData, 20, "freeeyes OK");
    objLogBlockInfo.m_pBlock   = reinterpret_cast<char*>(szData);
    objLogBlockInfo.m_u4Length = (uint32)ACE_OS::strlen(szData);

    if (0 != m_pLogFile->doLog(&objLogBlockInfo))
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->doLog() fail.", true == blRet);
        return;
    }

}

#endif
