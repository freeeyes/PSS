#ifndef UNIT_BUFFPACKET_H
#define UNIT_BUFFPACKET_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "BuffPacket.h"

const uint32 TEST_BUFFPACKET_MAX_SIZE = 20480;

class CUnit_BuffPacket : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_BuffPacket);
    CPPUNIT_TEST(Read_Write_BuffPacket);
    CPPUNIT_TEST(Read_Write_String_BuffPacket);
    CPPUNIT_TEST(Read_Write_Binary_BuffPacket);
    CPPUNIT_TEST(Check_Size_BuffPacket);
    CPPUNIT_TEST(Check_AddBuff);
    CPPUNIT_TEST(Check_RollBack);
    CPPUNIT_TEST(Check_String);
    CPPUNIT_TEST(Check_AddBuff_More_Uint8);
    CPPUNIT_TEST(Check_AddBuff_More_Uint16);
    CPPUNIT_TEST(Check_AddBuff_More_Uint32);
    CPPUNIT_TEST(Check_AddBuff_More_Uint64);
    CPPUNIT_TEST(Check_AddBuff_More_Int8);
    CPPUNIT_TEST(Check_AddBuff_More_Int16);
    CPPUNIT_TEST(Check_AddBuff_More_Int32);
    CPPUNIT_TEST(Check_AddBuff_More_Float32);
    CPPUNIT_TEST(Check_AddBuff_More_Float64);
    CPPUNIT_TEST(Check_AddBuff_More_VCHARS_STR);
    CPPUNIT_TEST(Check_AddBuff_More_VCHARM_STR);
    CPPUNIT_TEST(Check_AddBuff_More_VCHARB_STR);
    CPPUNIT_TEST(Test_PacketCount);
    CPPUNIT_TEST_SUITE_END();
public:
    CUnit_BuffPacket() = default;

    virtual void setUp(void) final;

    virtual void tearDown(void) final;

    void Read_Write_BuffPacket(void);          //∂¡–¥≤‚ ‘
    void Read_Write_String_BuffPacket(void);   //∂¡–¥◊÷∑˚¥Æ≤‚ ‘
    void Read_Write_Binary_BuffPacket(void);   //∂¡–¥∂˛Ω¯÷∆≤‚ ‘
    void Check_Size_BuffPacket(void);          //œ‡πÿ∂‘œÛ≥ﬂ¥Áµƒ≤‚ ‘
    void Check_AddBuff(void);
    void Check_RollBack(void);
    void Check_String(void);

    void Check_AddBuff_More_Uint8(void);
    void Check_AddBuff_More_Uint16(void);
    void Check_AddBuff_More_Uint32(void);
    void Check_AddBuff_More_Uint64(void);
    void Check_AddBuff_More_Int8(void);
    void Check_AddBuff_More_Int16(void);
    void Check_AddBuff_More_Int32(void);
    void Check_AddBuff_More_Float32(void);
    void Check_AddBuff_More_Float64(void);
    void Check_AddBuff_More_VCHARS_STR(void);
    void Check_AddBuff_More_VCHARM_STR(void);
    void Check_AddBuff_More_VCHARB_STR(void);

    void Test_PacketCount();

private:
    shared_ptr<CBuffPacket> m_pBuffPacket = nullptr;
};

#endif

#endif
