#include "Unit_ModuleMessageManager.h"

#ifdef _CPPUNIT_TEST

void CUnit_ModuleMessageManager::setUp(void)
{
    m_pModuleMessageManager = std::make_shared<CModuleMessageManager>();
}

void CUnit_ModuleMessageManager::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_ModuleMessageManager::tearDown]Finish.\n"));
}

void CUnit_ModuleMessageManager::Test_SendModuleMessage(void)
{
    bool blRet = false;
    uint16 u2Command   = 0x1001;
    char szSession[32] = { '\0' };
    char szData[30]    = { '\0' };

    sprintf_safe(szSession, 32, "freeeyes");
    sprintf_safe(szData, 30, "freeeyes");

    IBuffPacket* pBuffPacket       = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);
    IBuffPacket* pReturnBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    (*pBuffPacket) << (uint16)1;
    (*pBuffPacket) << u2Command;
    (*pBuffPacket) << (uint16)ACE_OS::strlen(szData);
    pBuffPacket->WriteStream(szSession, 32);
    pBuffPacket->WriteStream(szData, (int)ACE_OS::strlen(szData));

    if(0 == m_pModuleMessageManager->SendModuleMessage("UDPTest", u2Command, pBuffPacket, pReturnBuffPacket))
    {
        blRet = true;
    }

    App_BuffPacketManager::instance()->Delete(pBuffPacket);

    CPPUNIT_ASSERT_MESSAGE("[Test_SendModuleMessage]SendModuleMessage is fail.", true == blRet);
}

void CUnit_ModuleMessageManager::Test_SendFrameMessage(void)
{
    bool blRet = false;
    uint16 u2Command = 0x1001;
    char szSession[32] = { '\0' };
    char szData[30] = { '\0' };

    sprintf_safe(szSession, 32, "freeeyes");
    sprintf_safe(szData, 30, "freeeyes");

    IBuffPacket* pHeadPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);
    IBuffPacket* pBodyPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

    (*pHeadPacket) << (uint16)1;
    (*pHeadPacket) << u2Command;
    (*pHeadPacket) << (uint16)ACE_OS::strlen(szData);
    pHeadPacket->WriteStream(szSession, 32);
    pBodyPacket->WriteStream(szData, (int)ACE_OS::strlen(szData));

    if (0 == m_pModuleMessageManager->SendFrameMessage(u2Command, 0, pHeadPacket, pBodyPacket))
    {
        blRet = true;
    }

    App_BuffPacketManager::instance()->Delete(pHeadPacket);
    App_BuffPacketManager::instance()->Delete(pBodyPacket);

    CPPUNIT_ASSERT_MESSAGE("[Test_SendFrameMessage]SendModuleMessage is fail.", true == blRet);
}

#endif


