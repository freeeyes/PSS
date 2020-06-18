#ifndef _UNIT_AES_H
#define _UNIT_AES_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "define.h"
#include "Aes.h"

class CUnit_Aes : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_Aes);
    CPPUNIT_TEST(Test_Aes);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_Aes();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_Aes(void);

    void Byte2Hex(const unsigned char* src, int len, char* dest) const;

    void Hex2Byte(const char* src, int len, unsigned char* dest);

    int Char2Int(char c) const;

private:
    AES* m_pAES = NULL;
};

#endif

#endif
