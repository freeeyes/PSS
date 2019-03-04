#ifndef _UNIT_TTYCLIENTMANAGER_H
#define _UNIT_TTYCLIENTMANAGER_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "ReTTyClientManager.h"

class CTTyMessage : public ITTyMessage
{
    virtual bool RecvData(uint16 u2ConnectID, const char* pData, uint32 u4len)
    {
        ACE_UNUSED_ARG(u4len);
        OUR_DEBUG((LM_INFO, "[CTTyMessage::RecvData]u2ConnectID=%d, Data=%s.\n", u2ConnectID, pData));
        return true;
    };

    virtual bool ReportMessage(uint16 u2ConnectID, uint32 u4ErrorNo, EM_TTY_EVENT_TYPE em_type)
    {
        ACE_UNUSED_ARG(em_type);
        OUR_DEBUG((LM_INFO, "[CTTyMessage::ReportMessage]u2ConnectID=%d, u4ErrorNo=%d.\n", u2ConnectID, u4ErrorNo));
        return true;
    };
};

class CUnit_TTyClientManager : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_TTyClientManager);
    CPPUNIT_TEST(Test_TTyClientManager);
    CPPUNIT_TEST_SUITE_END();

public:
    virtual ~CUnit_TTyClientManager();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_TTyClientManager(void);
};

#endif

#endif
