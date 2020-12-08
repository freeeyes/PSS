#include "Uint_PacketParse.h"

#ifdef _CPPUNIT_TEST

void CUnit_PacketParse::setUp(void)
{
    m_pPacketParsePool = std::make_shared<CPacketParsePool>();
    m_pPacketParsePool->Init(1, CPacketParsePool::Init_Callback);
}

void CUnit_PacketParse::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_PacketParse::tearDown]Finish.\n"));
}

void CUnit_PacketParse::Test_PacketParsePool(void)
{
    //���������ԣ���Ϊ����Ҫ��
    bool blRet = true;

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_PacketParsePool]m_pPacketParsePool->Create().\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_PacketParsePool]m_pPacketParsePool->Create() is fail.", true == blRet);
        return;
    }
}

#endif

