#include "Unit_TTyClientManager.h"

#ifdef _CPPUNIT_TEST

CUnit_TTyClientManager::~CUnit_TTyClientManager()
{

}

void CUnit_TTyClientManager::setUp(void)
{
}

void CUnit_TTyClientManager::tearDown(void)
{
}

void CUnit_TTyClientManager::Test_TTyClientManager(void)
{
    bool blRet = false;
    _TTyDevParam inParam;
    _TTyDevParam outParam;
    CTTyMessage* pMessageRecv = new CTTyMessage();
    uint16 u2ConnectID            = 1;
    char szSendInfo[MAX_BUFF_100] = { '\0' };
    char szDevName[32]            = { '\0' };

    sprintf_safe(szDevName, 32, "/dev/ttyS0");
    sprintf_safe(inParam.m_szDevName, 32, "%s", szDevName);
    sprintf_safe(szSendInfo, MAX_BUFF_100, "hello freeeyes");
    inParam.m_nBaudRate = 19200;

    char* pSendInfo = (char* )szSendInfo;

    OUR_DEBUG((LM_INFO, "[CUnit_TTyClientManager::Test_TyClientManager]Begin.\n"));

    App_ReTTyClientManager::instance()->Connect(u2ConnectID, szDevName, inParam, (ITTyMessage* )pMessageRecv);

    App_ReTTyClientManager::instance()->GetClientDevInfo(u2ConnectID, outParam);

    if (outParam.m_nBaudRate != inParam.m_nBaudRate)
    {
        blRet = true;
        CPPUNIT_ASSERT_MESSAGE("[Test_TyClientManager]m_nBaudRate is fail.", true == blRet);
        return;
    }

    App_ReTTyClientManager::instance()->SendMessage(u2ConnectID, pSendInfo, ACE_OS::strlen(szSendInfo));

    App_ReTTyClientManager::instance()->Close(u2ConnectID);
}

#endif


