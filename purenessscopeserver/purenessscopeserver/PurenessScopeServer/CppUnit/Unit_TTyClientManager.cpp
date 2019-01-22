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

void CUnit_TTyClientManager::Test_TTyHandler(void)
{
    bool blRet = false;
    CReTTyHandler objReTTyHandler;
    _TTyDevParam inParam;

    objReTTyHandler.GetParams();

    objReTTyHandler.SetPause(false);

    blRet = objReTTyHandler.GetPause();

    siginfo_t* psinfo   = NULL;
    ucontext_t* context = NULL;
    objReTTyHandler.handle_signal(0, psinfo, context);

    ACE_HANDLE handle           = ACE_INVALID_HANDLE;
    ACE_Reactor_Mask close_mask = 0;
    objReTTyHandler.handle_close(handle, close_mask);

    ACE_Time_Value tvNow;
    objReTTyHandler.handle_timeout(tvNow, NULL);

    objReTTyHandler.Close();

    CPPUNIT_ASSERT_MESSAGE("[Test_TTyHandler]m_nBaudRate is fail.", true == blRet);
}

#endif


