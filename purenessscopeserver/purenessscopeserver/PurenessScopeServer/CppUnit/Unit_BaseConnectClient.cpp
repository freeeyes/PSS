#include "Unit_BaseConnectClient.h"

#ifdef _CPPUNIT_TEST

CUnit_BaseConnectClient::~CUnit_BaseConnectClient()
{

}

void CUnit_BaseConnectClient::setUp(void)
{
}

void CUnit_BaseConnectClient::tearDown(void)
{
}

void CUnit_BaseConnectClient::Test_Make_Common_Dispose_Client_WorkTread_Message(void)
{
    bool blRet = false;

    uint16 u2CommandID = 0x1003;
    uint32 u4ServerID  = 1;

    char szData[10] = { '\0' };
    sprintf_safe(szData, 10, "freeeyes");

    ACE_Message_Block* pmblk = App_MessageBlockManager::instance()->Create(10);
    ACE_OS::memcpy(pmblk->wr_ptr(), szData, ACE_OS::strlen(szData));
    pmblk->wr_ptr(ACE_OS::strlen(szData));

    ACE_INET_Addr AddrRemote;

    if (0 != Make_Common_Dispose_Client_WorkTread_Message(u2CommandID, u4ServerID, pmblk, AddrRemote))
    {
        OUR_DEBUG((LM_INFO, "[Make_Common_Dispose_Client_WorkTread_Message]Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Make_Common_Dispose_Client_WorkTread_Message]Error.", true == blRet);
        return;
    }
}

#endif
