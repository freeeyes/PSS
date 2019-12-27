#include "Unit_BaseConnectClient.h"

#ifdef _CPPUNIT_TEST

CUnit_BaseConnectClient::CUnit_BaseConnectClient()
{
}

void CUnit_BaseConnectClient::setUp(void)
{
    m_u2CommandID = 0x1003;
}

void CUnit_BaseConnectClient::tearDown(void)
{
    m_u2CommandID = 0;
}

void CUnit_BaseConnectClient::Test_Make_Common_Dispose_Client_WorkTread_Message(void)
{
    bool blRet = false;

    uint32 u4ServerID  = 1;

    char szData[10] = { '\0' };
    sprintf_safe(szData, 10, "freeeyes");

    ACE_Message_Block* pmblk = App_MessageBlockManager::instance()->Create(10);
    ACE_OS::memcpy(pmblk->wr_ptr(), szData, ACE_OS::strlen(szData));
    pmblk->wr_ptr(ACE_OS::strlen(szData));

    ACE_INET_Addr AddrRemote;

    if (0 != Make_Common_Dispose_Client_WorkTread_Message(m_u2CommandID, u4ServerID, pmblk, AddrRemote))
    {
        OUR_DEBUG((LM_INFO, "[Make_Common_Dispose_Client_WorkTread_Message]Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Make_Common_Dispose_Client_WorkTread_Message]Error.", true == blRet);
    }
}

void CUnit_BaseConnectClient::Test_Common_Send_ConnectError(void)
{
    ACE_INET_Addr objAddrServer;
    CPostServerData objPostServerData;

    ACE_Message_Block* pMb = App_MessageBlockManager::instance()->Create(10);

    Common_Send_ConnectError(pMb, objAddrServer, dynamic_cast<IClientMessage*>(&objPostServerData));
}

#endif
