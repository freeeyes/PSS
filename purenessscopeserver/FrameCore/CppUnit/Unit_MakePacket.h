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
    CUnit_MakePacket() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_MakePacket(void);

private:
    shared_ptr<CMakePacket> m_pMakePacket = nullptr;
    int m_nTestCount = 0;
};

#endif

#endif
