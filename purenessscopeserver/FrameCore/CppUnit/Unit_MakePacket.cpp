#include "Unit_MakePacket.h"

#ifdef _CPPUNIT_TEST

void CUnit_MakePacket::setUp(void)
{
    m_pMakePacket = std::make_shared<CMakePacket>();
}

void CUnit_MakePacket::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_MakePacket::tearDown]Finish.\n"));
}

void CUnit_MakePacket::Test_MakePacket(void)
{
    bool blRet = false;

    uint32 u4ConnectID = 1;
    ACE_Time_Value tvNow = ACE_OS::gettimeofday();
    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(10);

    if (false == m_pMakePacket->PutSendErrorMessage(u4ConnectID, pmb, tvNow))
    {
        OUR_DEBUG((LM_INFO, "[Test_ControlListen]m_pMakePacket->PutSendErrorMessage() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ControlListen]m_pMakePacket->PutSendErrorMessage() Error.", true == blRet);
    }

    m_nTestCount++;
}

#endif
