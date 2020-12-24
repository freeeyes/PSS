#include "Uint_ServerMessageInfoPool.h"

#ifdef _CPPUNIT_TEST

void CUnit_ServerMessageInfoPool::setUp(void)
{
    nCount++;
}

void CUnit_ServerMessageInfoPool::tearDown(void)
{
    nCount--;
    PSS_LOGGER_DEBUG("[CUnit_ServerMessageInfoPool::tearDown]Finish.");
}

void CUnit_ServerMessageInfoPool::Test_ServerMessageInfoPool(void)
{
    bool blRet = true;

    //取消功能，不做测试。

    if (false == blRet)
    {
        PSS_LOGGER_DEBUG("[Test_ServerMessageInfoPool]pServerMessageInfo is nullptr.");
        CPPUNIT_ASSERT_MESSAGE("[Test_ServerMessageInfoPool]pServerMessageInfo is nullptr.", true == blRet);
        return;
    }
}

#endif

