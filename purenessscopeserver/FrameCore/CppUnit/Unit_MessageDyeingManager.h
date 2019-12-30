#ifndef _UNIT_MESSAGEDYEINGMANAGER_H
#define _UNIT_MESSAGEDYEINGMANAGER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "MessageDyeingManager.h"

class CUnit_MessageDyeingManager : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_MessageDyeingManager);
    CPPUNIT_TEST(Test_MessageDyeingManager);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_MessageDyeingManager();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_MessageDyeingManager(void);

private:
    CMessageDyeingManager* m_pMessageDyeingManager = NULL;
};

#endif

#endif
