#ifndef _UNIT_LOGICTHREADMANAGER_H
#define _UNIT_LOGICTHREADMANAGER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "define.h"
#include "ILogicQueue.h"
#include "LogicThreadManager.h"

class CLogicTestQueue : public ILogicQueue
{
public:
    CLogicTestQueue(uint32 u4LogicThreadID, uint32 u4Timeout, std::string strDesc):
        ILogicQueue(u4LogicThreadID, u4Timeout, strDesc)
    {
    };

    virtual ~CLogicTestQueue() {};

    virtual bool Init(int nLogicThread)
    {
        OUR_DEBUG((LM_INFO, "[Test_LogicThreadInit]Init LogicThread=%d.\n", nLogicThread));
        return true;
    };

    //执行逻辑
    virtual ThreadReturn Run(int nLogicThread, int nMessage, void* arg)
    {
        ACE_UNUSED_ARG(arg);

        OUR_DEBUG((LM_INFO, "[Test_LogicThreadCallbackLogic]LogicThread=%d, nMessage=%d.\n", nLogicThread, nMessage));

        return THREAD_Task_Finish;
    };

    //错误处理
    virtual uint32 Error(int nLogicThread, int nErrorID)
    {
        OUR_DEBUG((LM_INFO, "[Test_LogicThreadErrorLogic]LogicThread=%d, nErrorID=%d.\n", nLogicThread, nErrorID));
        return 0;
    };

    //退出善后
    virtual void Exit(int nLogicThread)
    {
        OUR_DEBUG((LM_INFO, "[Test_LogicThreadExit]LogicThread=%d.\n", nLogicThread));
    };
};

class CUnit_LogicThreadManager : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_LogicThreadManager);
    CPPUNIT_TEST(Test_LogicThreadManager);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_LogicThreadManager();

    virtual ~CUnit_LogicThreadManager();

    CUnit_LogicThreadManager(const CUnit_LogicThreadManager& ar);

    CUnit_LogicThreadManager& operator = (const CUnit_LogicThreadManager& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_LogicThreadManager(void);

private:
    int  m_nMessage;
    CLogicTestQueue* m_pLogicTestQueue;
};

#endif

#endif
