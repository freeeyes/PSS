#include "Uint_LogManager.h"

#ifdef _CPPUNIT_TEST

void CUnit_LogManager::setUp(void)
{
    m_pLogBlockPool = std::make_shared<CLogBlockPool>();
    m_pLogBlockPool->Init(20, 2);
}

void CUnit_LogManager::tearDown(void)
{
    m_pLogBlockPool->Close();
}

void CUnit_LogManager::Test_LogBlockPool(void)
{
    //���Դ���ָ���ķ�Ӧ��
    bool blRet = false;

    _LogBlockInfo* pLogBlockInfo = m_pLogBlockPool->GetLogBlockInfo();

    if (nullptr == pLogBlockInfo)
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

    //���Զ�����д��
    if (-1 == AppLogManager::instance()->WriteLogBinary(LOG_SYSTEM, szData, (int)ACE_OS::strlen(szData)))
    {
        OUR_DEBUG((LM_INFO, "[Test_Write_Log]WriteLogBinary is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_Write_Log]WriteLogBinary is fail.", true == blRet);
    }

    m_nTestCount++;
}

#endif

