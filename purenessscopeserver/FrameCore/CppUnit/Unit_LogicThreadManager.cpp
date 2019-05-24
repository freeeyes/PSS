#include "Unit_LogicThreadManager.h"

#ifdef _CPPUNIT_TEST

CUnit_LogicThreadManager::CUnit_LogicThreadManager()
{
    m_nMessage        = 1000;
    m_pLogicTestQueue = NULL;
}

CUnit_LogicThreadManager::CUnit_LogicThreadManager(const CUnit_LogicThreadManager& ar)
{
    (*this) = ar;
}

CUnit_LogicThreadManager::~CUnit_LogicThreadManager()
{
}

void CUnit_LogicThreadManager::setUp(void)
{
    m_pLogicTestQueue = new CLogicTestQueue(1, 120, "test");
}

void CUnit_LogicThreadManager::tearDown(void)
{
}

void CUnit_LogicThreadManager::Test_LogicThreadManager(void)
{
    bool blRet = false;
    int nRet = App_MessageQueueManager::instance()->CreateLogicThread(1,
               120,
               m_pLogicTestQueue);

    if (0 != nRet)
    {
        blRet = true;
        CPPUNIT_ASSERT_MESSAGE("[Test_TimerManager]CreateLogicThread is fail.", true == blRet);
    }

    nRet = App_MessageQueueManager::instance()->MessageMappingLogicThread(1, m_nMessage);

    if (0 != nRet)
    {
        blRet = true;
        CPPUNIT_ASSERT_MESSAGE("[Test_TimerManager]MessageMappingLogicThread is fail.", true == blRet);
    }

    nRet = App_MessageQueueManager::instance()->SendLogicThreadMessage(m_nMessage, NULL);

    if (0 != nRet)
    {
        blRet = true;
        CPPUNIT_ASSERT_MESSAGE("[Test_TimerManager]SendLogicThreadMessage is fail.", true == blRet);
    }

    ACE_Time_Value tvSleep(0, 1000);
    ACE_OS::sleep(tvSleep);

    OUR_DEBUG((LM_INFO, "[CUnit_LogicThreadManager]objActiveTimer is close.\n"));
}

#endif
