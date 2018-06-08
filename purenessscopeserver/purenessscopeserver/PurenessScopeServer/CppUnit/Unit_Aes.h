#ifndef _UNIT_AES_H
#define _UNIT_AES_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "define.h"
#include "Aes.h"

class CUnit_ConnectTcp : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_ConnectTcp);
    CPPUNIT_TEST(Test_Aes);
    CPPUNIT_TEST_SUITE_END();

public:
    virtual ~CUnit_ConnectTcp();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_Aes(void);

    void Byte2Hex(const unsigned char* src, int len, char* dest);

    void Hex2Byte(const char* src, int len, unsigned char* dest);

    int Char2Int(char c);

private:
    AES* m_pAES;
};

#endif

#endif
