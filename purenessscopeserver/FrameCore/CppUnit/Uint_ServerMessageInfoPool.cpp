#include "Uint_ServerMessageInfoPool.h"

#ifdef _CPPUNIT_TEST

void CUnit_ServerMessageInfoPool::setUp(void)
{
    m_pServerMessageInfoPool = std::make_shared <CServerMessageInfoPool>();
    m_pServerMessageInfoPool->Init();
}

void CUnit_ServerMessageInfoPool::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_ServerMessageInfoPool::tearDown]Finish.\n"));
}

void CUnit_ServerMessageInfoPool::Test_ServerMessageInfoPool(void)
{
    bool blRet = false;

    _Server_Message_Info* pServerMessageInfo = m_pServerMessageInfoPool->Create();

    if (nullptr == pServerMessageInfo)
    {
        OUR_DEBUG((LM_INFO, "[Test_ServerMessageInfoPool]pServerMessageInfo is nullptr.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ServerMessageInfoPool]pServerMessageInfo is nullptr.", true == blRet);
        return;
    }

    if (1 != m_pServerMessageInfoPool->GetUsedCount())
    {
        OUR_DEBUG((LM_INFO, "[Test_ServerMessageInfoPool]GetUsedCount is not 1.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ServerMessageInfoPool]GetUsedCount is not 1.", true == blRet);
        return;
    }

    if (false == m_pServerMessageInfoPool->Delete(pServerMessageInfo))
    {
        OUR_DEBUG((LM_INFO, "[Test_ServerMessageInfoPool]Delete is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ServerMessageInfoPool]Delete is fail.", true == blRet);
        return;
    }

    if (MAX_SERVER_MESSAGE_INFO_COUNT != m_pServerMessageInfoPool->GetFreeCount())
    {
        OUR_DEBUG((LM_INFO, "[Test_ServerMessageInfoPool]GetFreeCount is not MAX_SERVER_MESSAGE_INFO_COUNT.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ServerMessageInfoPool]GetFreeCount is not MAX_SERVER_MESSAGE_INFO_COUNT.", true == blRet);
    }
}

#endif

