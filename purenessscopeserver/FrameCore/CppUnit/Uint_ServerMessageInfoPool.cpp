#include "Uint_ServerMessageInfoPool.h"

#ifdef _CPPUNIT_TEST

void CUnit_ServerMessageInfoPool::setUp(void)
{
    nCount++;
}

void CUnit_ServerMessageInfoPool::tearDown(void)
{
    nCount--;
    OUR_DEBUG((LM_INFO, "[CUnit_ServerMessageInfoPool::tearDown]Finish.\n"));
}

void CUnit_ServerMessageInfoPool::Test_ServerMessageInfoPool(void)
{
    bool blRet = true;

    //取消功能，不做测试。

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ServerMessageInfoPool]pServerMessageInfo is nullptr.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ServerMessageInfoPool]pServerMessageInfo is nullptr.", true == blRet);
        return;
    }
}

#endif

