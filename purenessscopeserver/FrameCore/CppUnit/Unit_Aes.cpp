#include "Unit_Aes.h"

#ifdef _CPPUNIT_TEST

void CUnit_Aes::setUp(void)
{
    m_pAES = std::make_shared<AES>();

    unsigned char szkey[] =
    {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13,
        0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b,
        0x1c, 0x1d, 0x1e, 0x1f
    };

    m_pAES->SetKey((unsigned char* )szkey);
}

void CUnit_Aes::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_Aes::tearDown]Finish.");
}

void CUnit_Aes::Test_Aes(void)
{
    bool blRet                 = false;
    char szTest[MAX_BUFF_100]  = { '\0' };
    char szEncry[MAX_BUFF_100] = { '\0' };
    char szDecry[MAX_BUFF_100] = { '\0' };

    sprintf_safe(szTest, MAX_BUFF_100, "freeeyes is freedom eyes.");
    sprintf_safe(szEncry, MAX_BUFF_100, "%s", szTest);

    //¼ÓÃÜAes
    int nTestLen = (int)ACE_OS::strlen(szTest);
    int nEncryLength = nTestLen + (16 - (nTestLen % 16));

    m_pAES->Cipher(szEncry, nEncryLength);

    //ÏÔÊ¾¼ÓÃÜ×Ö·û´®
    char* pOut = new char[2 * (nEncryLength) + 1];
    memset(pOut, '\0', 2 * (nEncryLength) + 1);
    Byte2Hex((unsigned char* )szEncry, nEncryLength, pOut);

    PSS_LOGGER_DEBUG("[Test_Aes]encry is({0}).", pOut);

    //ÏÔÊ¾½âÃÜ×Ö·û´®
    m_pAES->InvCipher(szEncry, nEncryLength);

    sprintf_safe(szDecry, MAX_BUFF_100, "%s", szEncry);

    PSS_LOGGER_DEBUG("[Test_Aes]szDecry is({0}).\n", szDecry);

    if (ACE_OS::strcmp(szDecry, szTest) == 0)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_Aes]Aes is fail.", true == blRet);
}

void CUnit_Aes::Byte2Hex(const unsigned char* src, int len, char* dest) const
{
    for (int i = 0; i < len; ++i)
    {
        sprintf_safe(dest + i * 2, 3, "%02X", src[i]);
    }
}

void CUnit_Aes::Hex2Byte(const char* src, int len, unsigned char* dest)
{
    int length = len / 2;

    for (int i = 0; i < length; ++i)
    {
        dest[i] = (unsigned char)(Char2Int(src[i * 2]) * 16 + Char2Int(src[i * 2 + 1]));
    }

    m_nTestCount++;
}

int CUnit_Aes::Char2Int(char c) const
{
    if ('0' <= c && c <= '9')
    {
        return (c - '0');
    }
    else if ('a' <= c && c <= 'f')
    {
        return (c - 'a' + 10);
    }
    else if ('A' <= c && c <= 'F')
    {
        return (c - 'A' + 10);
    }

    return -1;
}

#endif


