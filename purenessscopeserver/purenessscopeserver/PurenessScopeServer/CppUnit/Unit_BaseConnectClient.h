#ifndef _UNIT_BASECONNECTCLIENT_H
#define _UNIT_BASECONNECTCLIENT_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "BaseConnectClient.h"


class CUnit_BaseConnectClient : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_BaseConnectClient);
    CPPUNIT_TEST(Test_Make_Common_Dispose_Client_WorkTread_Message);
    CPPUNIT_TEST_SUITE_END();

public:
    virtual ~CUnit_BaseConnectClient();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_Make_Common_Dispose_Client_WorkTread_Message(void);

private:
    uint16 m_u2CommandID;
};

#endif

#endif
