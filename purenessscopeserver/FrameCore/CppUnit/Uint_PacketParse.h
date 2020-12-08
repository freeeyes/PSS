#ifndef UNIT_PACKETPARSE_H
#define UNIT_PACKETPARSE_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "PacketParse.h"

class CUnit_PacketParse : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_PacketParse);
    CPPUNIT_TEST(Test_PacketParsePool);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_PacketParse() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_PacketParsePool(void);

private:
    int nCount = 0;
};

#endif

#endif
