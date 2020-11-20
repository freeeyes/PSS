#include "Unit_BaseConnectClient.h"

#ifdef _CPPUNIT_TEST

void CUnit_BaseConnectClient::setUp(void)
{
    m_u2CommandID = 0x1003;
}

void CUnit_BaseConnectClient::tearDown(void)
{
    m_u2CommandID = 0;
}

void CUnit_BaseConnectClient::Test_Common_Send_ConnectError(void)
{
    ACE_INET_Addr objAddrServer;
    CPostServerData objPostServerData;

    ACE_Message_Block* pMb = App_MessageBlockManager::instance()->Create(10);

    Common_Send_ConnectError(pMb, objAddrServer, dynamic_cast<IClientMessage*>(&objPostServerData));

    m_nTestCount++;
}

#endif
