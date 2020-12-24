#include "Uint_LogManager.h"

#ifdef _CPPUNIT_TEST

void CUnit_LogManager::setUp(void)
{
    m_nTestCount = 0;
}

void CUnit_LogManager::tearDown(void)
{
    m_nTestCount = 0;
}

void CUnit_LogManager::Test_Write_Log(void)
{
    bool blRet = false;
    string strName = "freeeyes";

    //测试二进制写入
    if (-1 == AppLogManager::instance()->WriteLogBinary(LOG_SYSTEM, strName))
    {
        PSS_LOGGER_DEBUG("[Test_Write_Log]WriteLogBinary is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Write_Log]WriteLogBinary is fail.", true == blRet);
    }

    m_nTestCount++;
}

#endif

