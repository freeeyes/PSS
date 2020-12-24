#include "Unit_FrameCommand.h"

#ifdef _CPPUNIT_TEST

void CUnit_FrameCommand::setUp(void)
{
    m_pFrameCommand = std::make_shared<CFrameCommand>();
}

void CUnit_FrameCommand::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_FrameCommand::tearDown]Finish.");
}

void CUnit_FrameCommand::Test_Frame_Command(void)
{
    bool blRet = false;
    auto pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    if ((int)EM_CONSOLE_MESSAGE::CONSOLE_MESSAGE_SUCCESS == m_pFrameCommand->DoFrameCommand("ShowModule -a", pBuffPacket))
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_Frame_Command]DoFrameCommand is fail.", true == blRet);
}

#endif


