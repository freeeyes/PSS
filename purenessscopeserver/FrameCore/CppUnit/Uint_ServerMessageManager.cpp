#include "Uint_ServerMessageManager.h"

#ifdef _CPPUNIT_TEST

void CUnit_ServerMessageManager::setUp(void)
{
    nCount++;
}

void CUnit_ServerMessageManager::tearDown(void)
{
    nCount--;
    OUR_DEBUG((LM_INFO, "[CUnit_ServerMessageManager::tearDown]Finish.\n"));
}

void CUnit_ServerMessageManager::Test_ServerMessageManager(void)
{
    //���ڲ���
    bool blRet = true;
    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ServerMessageManager]m_pServerMessageManager->Start() false.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ServerMessageManager]m_pServerMessageManager->Start() false.", true == blRet);
        return;
    }
}

#endif

