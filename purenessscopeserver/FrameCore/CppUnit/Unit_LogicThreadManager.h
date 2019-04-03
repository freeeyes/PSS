#ifndef _UNIT_LOGICTHREADMANAGER_H
#define _UNIT_LOGICTHREADMANAGER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "define.h"
#include "LogicThreadManager.h"


//初始化逻辑线程
void Test_LogicThreadInit(int nLogicThread);

//线程消息处理函数(参数为线程ID, 消息ID, 内容指针)
ThreadReturn Test_LogicThreadCallbackLogic(int nLogicThread, int nMessage, void* arg);

//线程错误回调函数(线程ID, 错误ID)
void Test_LogicThreadErrorLogic(int nLogicThread, int nErrorID);

//线程退出函数
void Test_LogicThreadExit(int nLogicThread);

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
};

#endif

#endif
