#include "Uint_PacketParse.h"

#ifdef _CPPUNIT_TEST

CUnit_PacketParse::CUnit_PacketParse()
{
    m_pPacketParsePool = NULL;
}

void CUnit_PacketParse::setUp(void)
{
    m_pPacketParsePool = new CPacketParsePool();
    m_pPacketParsePool->Init(1, CPacketParsePool::Init_Callback);
}

void CUnit_PacketParse::tearDown(void)
{
    delete m_pPacketParsePool;
    m_pPacketParsePool = NULL;
}

void CUnit_PacketParse::Test_PacketParsePool(void)
{
    //测试从内存池中取得数据
    bool blRet = false;

    CPacketParse* pPacketParse = m_pPacketParsePool->Create(__FILE__, __LINE__);

    if (NULL == pPacketParse)
    {
        OUR_DEBUG((LM_INFO, "[Test_PacketParsePool]m_pPacketParsePool->Create().\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_PacketParsePool]m_pPacketParsePool->Create() is fail.", true == blRet);
        return;
    }

    ACE_Message_Block* pmbHead = App_MessageBlockManager::instance()->Create(4);
    ACE_Message_Block* pmbBody = App_MessageBlockManager::instance()->Create(4);

    pPacketParse->SetPacket_Head_Message(pmbHead);
    pPacketParse->SetPacket_Body_Message(pmbBody);

    //测试版本号
    if (0 != ACE_OS::strcmp(pPacketParse->GetPacketVersion(), "1.00"))
    {
        OUR_DEBUG((LM_INFO, "[Test_PacketParsePool]m_pPacketParsePool->GetPacketVersion().\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_PacketParsePool]m_pPacketParsePool->GetPacketVersion() is fail.", true == blRet);
        return;
    }

    //测试类型转换
    pPacketParse->SetSort(1);

    uint16 u2Data = ACE_HTONS(10);
    pPacketParse->Check_Recv_Unit16(u2Data);

    if (10 != u2Data)
    {
        OUR_DEBUG((LM_INFO, "[Test_PacketParsePool]m_pPacketParsePool->Check_Recv_Unit16().\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_PacketParsePool]m_pPacketParsePool->Check_Recv_Unit16() is fail.", true == blRet);
        return;
    }

    uint32 u4Data = ACE_HTONL(10);
    pPacketParse->Check_Recv_Unit32(u4Data);

    if (10 != u4Data)
    {
        OUR_DEBUG((LM_INFO, "[Test_PacketParsePool]m_pPacketParsePool->Check_Recv_Unit32().\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_PacketParsePool]m_pPacketParsePool->Check_Recv_Unit32() is fail.", true == blRet);
        return;
    }

    uint64 u8Data = hl64ton(10);
    pPacketParse->Check_Recv_Unit64(u8Data);

    if (10 != u8Data)
    {
        OUR_DEBUG((LM_INFO, "[Test_PacketParsePool]m_pPacketParsePool->Check_Recv_Unit64().\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_PacketParsePool]m_pPacketParsePool->Check_Recv_Unit64() is fail.", true == blRet);
        return;
    }

    //测试数据回收
    if (false == m_pPacketParsePool->Delete(pPacketParse, true))
    {
        OUR_DEBUG((LM_INFO, "[Test_PacketParsePool]m_pPacketParsePool->Delete().\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_PacketParsePool]m_pPacketParsePool->Delete() is fail.", true == blRet);
    }
}

#endif

