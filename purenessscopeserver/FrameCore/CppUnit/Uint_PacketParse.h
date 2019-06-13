#ifndef UNIT_PACKETPARSE_H
#define UNIT_PACKETPARSE_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "PacketParsePool.h"

class CUnit_PacketParse : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_PacketParse);
    CPPUNIT_TEST(Test_PacketParsePool);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_PacketParse();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_PacketParsePool(void);

private:
    CPacketParsePool* m_pPacketParsePool;
};

#endif

#endif
