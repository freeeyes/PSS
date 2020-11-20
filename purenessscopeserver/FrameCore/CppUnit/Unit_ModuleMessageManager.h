#ifndef _UNIT_MODULEMESSAGEMANAGER_H
#define _UNIT_MODULEMESSAGEMANAGER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ModuleMessageManager.h"

class CUnit_ModuleMessageManager : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ModuleMessageManager);
    CPPUNIT_TEST(Test_SendModuleMessage);
    CPPUNIT_TEST(Test_SendFrameMessage);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_ModuleMessageManager() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_SendModuleMessage(void);  //测试模块间数据发送

    void Test_SendFrameMessage(void);   //测试消息队列

private:
    shared_ptr<CModuleMessageManager> m_pModuleMessageManager = nullptr;
};

#endif

#endif
