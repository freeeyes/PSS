#ifndef _UNIT_MAKEPACKET_H
#define _UNIT_MAKEPACKET_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "MakePacket.h"

class CUnit_MakePacket : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_MakePacket);
    CPPUNIT_TEST(Test_MakePacket);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_MakePacket();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_MakePacket(void);

private:
    CMakePacket* m_pMakePacket = NULL;
    int m_nTestCount = 0;
};

#endif

#endif
