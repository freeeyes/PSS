#ifndef UNIT_BUFFPACKET_H
#define UNIT_BUFFPACKET_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "BuffPacket.h"

class CUnit_BuffPacket : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_BuffPacket);
    CPPUNIT_TEST(Read_Write_BuffPacket);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp(void);
    
    void tearDown(void);
    
protected:
    void Read_Write_BuffPacket(void);      //∂¡–¥≤‚ ‘

private:
	CBuffPacket* m_pBuffPacket;
};

#endif

#endif
