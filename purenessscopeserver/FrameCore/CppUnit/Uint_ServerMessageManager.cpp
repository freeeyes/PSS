#include "Uint_ServerMessageManager.h"

#ifdef _CPPUNIT_TEST

CUnit_ServerMessageManager::CUnit_ServerMessageManager()
{
}

void CUnit_ServerMessageManager::setUp(void)
{
    m_pServerMessageManager = new CServerMessageManager();
    m_pServerMessageManager->Init();
}

void CUnit_ServerMessageManager::tearDown(void)
{
    delete m_pServerMessageManager;
    m_pServerMessageManager = nullptr;
}

void CUnit_ServerMessageManager::Test_ServerMessageManager(void)
{
    bool blRet = false;
    CPostServerData objPostServerData;

    if (false == m_pServerMessageManager->Start())
    {
        OUR_DEBUG((LM_INFO, "[Test_ServerMessageManager]m_pServerMessageManager->Start() false.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ServerMessageManager]m_pServerMessageManager->Start() false.", true == blRet);
        return;
    }

    ACE_Time_Value tvSleepStart(0, 1000);
    ACE_OS::sleep(tvSleepStart);

    m_pServerMessageManager->AddClientMessage((IClientMessage*)&objPostServerData);

    _Server_Message_Info* pMessage = App_ServerMessageInfoPool::instance()->Create();

    if (nullptr == pMessage)
    {
        OUR_DEBUG((LM_INFO, "[Test_ServerMessageManager]App_ServerMessageInfoPool::instance()->Create() is nullptr.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ServerMessageManager]App_ServerMessageInfoPool::instance()->Create() is nullptr.", true == blRet);
        return;
    }

    OUR_DEBUG((LM_INFO, "[Test_ServerMessageManager]Begin make Send Data.\n"));

    //拼装测试发送数据
    char szSendBuffer[MAX_BUFF_200] = { '\0' };
    char szBuff[20] = { '\0' };
    char szSession[32] = { '\0' };
    sprintf_safe(szBuff, 20, "freeeyes");
    sprintf_safe(szSession, 32, "FREEEYES");

    short sVersion = 1;
    short sCommand = (short)0x1000;
    int nPacketLen = (int)ACE_OS::strlen(szBuff);

    memcpy(szSendBuffer, (char*)&sVersion, sizeof(short));
    memcpy(&szSendBuffer[2], (char*)&sCommand, sizeof(short));
    memcpy(&szSendBuffer[4], (char*)&nPacketLen, sizeof(int));
    memcpy(&szSendBuffer[8], (char*)&szSession, sizeof(char) * 32);
    memcpy(&szSendBuffer[40], (char*)szBuff, sizeof(char) * nPacketLen);
    uint32 u4SendLen = nPacketLen + 40;

    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(u4SendLen);
    memcpy_safe(pmb->wr_ptr(), u4SendLen, szSendBuffer, u4SendLen);
    pmb->wr_ptr(u4SendLen);

    pMessage->m_u2CommandID    = 0x1000;
    pMessage->m_pRecvFinish    = pmb;
    pMessage->m_pClientMessage = dynamic_cast<IClientMessage*>(&objPostServerData);

    OUR_DEBUG((LM_INFO, "[Test_ServerMessageManager]Begin Send Data.\n"));

    //模拟发送数据
    m_pServerMessageManager->PutMessage(pMessage);

    ACE_Time_Value tvSleepDispose(0, 10000);
    ACE_OS::sleep(tvSleepDispose);

    if (false == m_pServerMessageManager->CheckServerMessageThread(ACE_OS::gettimeofday()))
    {
        OUR_DEBUG((LM_INFO, "[Test_ServerMessageManager]m_pServerMessageManager->CheckServerMessageThread() is nullptr.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ServerMessageManager]m_pServerMessageManager->CheckServerMessageThread() is nullptr.", true == blRet);
        return;
    }

    OUR_DEBUG((LM_INFO, "[Test_ServerMessageManager]Begin Send Finish.\n"));

    m_pServerMessageManager->DelClientMessage(dynamic_cast<IClientMessage*>(&objPostServerData));

    OUR_DEBUG((LM_INFO, "[Test_ServerMessageManager]DelClientMessage Finish.\n"));

    m_pServerMessageManager->Close();

    OUR_DEBUG((LM_INFO, "[Test_ServerMessageManager]Close Finish.\n"));

    ACE_Time_Value tvSleepClose(0, 1000);
    ACE_OS::sleep(tvSleepClose);
}

#endif

