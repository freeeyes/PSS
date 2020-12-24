#include "Unit_MakePacket.h"

#ifdef _CPPUNIT_TEST

void CUnit_MakePacket::setUp(void)
{
    m_pMakePacket = std::make_shared<CMakePacket>();
}

void CUnit_MakePacket::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_MakePacket::tearDown]Finish.");
}

void CUnit_MakePacket::Test_MakePacket(void)
{
    bool blRet = false;

    uint32 u4ConnectID = 1;
    auto tvNow = CTimeStamp::Get_Time_Stamp();
    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(10);

    if (false == m_pMakePacket->PutSendErrorMessage(u4ConnectID, pmb, tvNow))
    {
        PSS_LOGGER_DEBUG("[Test_ControlListen]m_pMakePacket->PutSendErrorMessage() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_ControlListen]m_pMakePacket->PutSendErrorMessage() Error.", true == blRet);
    }

    m_nTestCount++;
}

#endif
