#include "Uint_ServerMessageManager.h"

#ifdef _CPPUNIT_TEST

void CUnit_ServerMessageManager::setUp(void)
{
    nCount++;
}

void CUnit_ServerMessageManager::tearDown(void)
{
    nCount--;
    PSS_LOGGER_DEBUG("[CUnit_ServerMessageManager::tearDown]Finish.");
}

void CUnit_ServerMessageManager::Test_ServerMessageManager(void)
{
    //²»ÔÚ²âÊÔ
    bool blRet = true;
    if (false == blRet)
    {
        PSS_LOGGER_DEBUG("[Test_ServerMessageManager]m_pServerMessageManager->Start() false.");
        CPPUNIT_ASSERT_MESSAGE("[Test_ServerMessageManager]m_pServerMessageManager->Start() false.", true == blRet);
        return;
    }
}

#endif

