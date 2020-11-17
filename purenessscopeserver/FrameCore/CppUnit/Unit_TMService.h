#ifndef _UNIT_TMSERVICE_H
#define _UNIT_TMSERVICE_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "IObject.h"

class CTestUser : public IMessagePrecess
{
public:
    void DoMessage(int _message_id, void* _arg)
    {
        OUR_DEBUG((LM_INFO, "[CTestUser::DoMessage]_message_id=%d.\n", _message_id));
        int* nData = (int*)_arg;
        (*nData) = 1;
    }
};

class CUnit_TMService : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_TMService);
    CPPUNIT_TEST(Test_TMService);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_TMService();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_TMService(void);

private:
    ITMService* m_pTMService = nullptr;
};

#endif

#endif
