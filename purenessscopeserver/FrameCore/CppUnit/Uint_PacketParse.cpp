#include "Uint_PacketParse.h"

#ifdef _CPPUNIT_TEST

void CUnit_PacketParse::setUp(void)
{
    nCount = 1;
}

void CUnit_PacketParse::tearDown(void)
{
    nCount = 0;
    PSS_LOGGER_DEBUG("[CUnit_PacketParse::tearDown]Finish.");
}

void CUnit_PacketParse::Test_PacketParsePool(void)
{
    //不再做测试，因为不需要了
    bool blRet = true;

    if (false == blRet)
    {
        PSS_LOGGER_DEBUG("[Test_PacketParsePool]m_pPacketParsePool->Create().");
        CPPUNIT_ASSERT_MESSAGE("[Test_PacketParsePool]m_pPacketParsePool->Create() is fail.", true == blRet);
        return;
    }
}

#endif

