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
    CUnit_ModuleMessageManager();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_SendModuleMessage(void);  //����ģ������ݷ���

    void Test_SendFrameMessage(void);   //������Ϣ����

private:
    CModuleMessageManager* m_pModuleMessageManager = nullptr;
};

#endif

#endif
