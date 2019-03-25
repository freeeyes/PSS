#include "Unit_LogicThreadManager.h"

#ifdef _CPPUNIT_TEST

void Test_LogicThreadInit(int nLogicThread)
{
    OUR_DEBUG((LM_INFO, "[Test_LogicThreadInit]Init LogicThread=%d.\n", nLogicThread));
}

ThreadReturn Test_LogicThreadCallbackLogic(int nLogicThread, int nMessage, void* arg)
{
    ACE_UNUSED_ARG(arg);

    OUR_DEBUG((LM_INFO, "[Test_LogicThreadCallbackLogic]LogicThread=%d, nMessage=%d.\n", nLogicThread, nMessage));

    return THREAD_Task_Finish;
}

void Test_LogicThreadErrorLogic(int nLogicThread, int nErrorID)
{
    OUR_DEBUG((LM_INFO, "[Test_LogicThreadErrorLogic]LogicThread=%d, nErrorID=%d.\n", nLogicThread, nErrorID));
}

void Test_LogicThreadExit(int nLogicThread)
{
    OUR_DEBUG((LM_INFO, "[Test_LogicThreadExit]LogicThread=%d.\n", nLogicThread));
}

CUnit_LogicThreadManager::CUnit_LogicThreadManager()
{
    m_nMessage = 1000;
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
}

void CUnit_LogicThreadManager::tearDown(void)
{
}

void CUnit_LogicThreadManager::Test_LogicThreadManager(void)
{
    bool blRet = false;
    int nRet = App_LogicThreadManager::instance()->CreateLogicThread(1,
               120,
               Test_LogicThreadInit,
               Test_LogicThreadCallbackLogic,
               Test_LogicThreadErrorLogic,
               Test_LogicThreadExit);

    if (0 != nRet)
    {
        blRet = true;
        CPPUNIT_ASSERT_MESSAGE("[Test_TimerManager]CreateLogicThread is fail.", true == blRet);
    }

    nRet = App_LogicThreadManager::instance()->MessageMappingLogicThread(1, m_nMessage);

    if (0 != nRet)
    {
        blRet = true;
        CPPUNIT_ASSERT_MESSAGE("[Test_TimerManager]MessageMappingLogicThread is fail.", true == blRet);
    }

    nRet = App_LogicThreadManager::instance()->SendLogicThreadMessage(m_nMessage, NULL);

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
