#include "Uint_LogManager.h"

#ifdef _CPPUNIT_TEST

CUnit_LogManager::~CUnit_LogManager()
{
    m_pLogBlockPool = NULL;
}

void CUnit_LogManager::setUp(void)
{
    m_pLogBlockPool = new CLogBlockPool();
    m_pLogBlockPool->Init(20, 2);
}

void CUnit_LogManager::tearDown(void)
{
    delete m_pLogBlockPool;
    m_pLogBlockPool = NULL;
}

void CUnit_LogManager::Test_LogBlockPool(void)
{
    //测试创建指定的反应器
    bool blRet = false;

    _LogBlockInfo* pLogBlockInfo = m_pLogBlockPool->GetLogBlockInfo();

    if (NULL == pLogBlockInfo)
    {
        OUR_DEBUG((LM_INFO, "[Test_LogBlockPool]GetLogBlockInfo is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_LogBlockPool]GetLogBlockInfo is fail.", true == blRet);
        return;
    }

    m_pLogBlockPool->ReturnBlockInfo(pLogBlockInfo);

}

void CUnit_LogManager::Test_Write_Log(void)
{
    bool blRet = false;
    char szData[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szData, MAX_BUFF_20, "freeeyes");

    //测试二进制写入
    if (-1 == AppLogManager::instance()->WriteLogBinary(LOG_SYSTEM, szData, (int)ACE_OS::strlen(szData)))
    {
        OUR_DEBUG((LM_INFO, "[Test_Write_Log]WriteLogBinary is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_Write_Log]WriteLogBinary is fail.", true == blRet);
        return;
    }

    //测试邮件写入
    if (-1 == AppLogManager::instance()->WriteToMail(LOG_SYSTEM, 1, szData, "freeeyes test(%d)", 1))
    {
        OUR_DEBUG((LM_INFO, "[Test_Write_Log]WriteToMail is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_Write_Log]WriteToMail is fail.", true == blRet);
    }
}

#endif

