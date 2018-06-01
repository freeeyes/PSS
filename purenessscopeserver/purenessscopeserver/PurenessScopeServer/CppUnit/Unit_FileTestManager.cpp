#include "Unit_FileTestManager.h"

#ifdef _CPPUNIT_TEST

void CUnit_FileTestManager::setUp(void)
{
    m_pFileTestManager = new CFileTestManager();
}

void CUnit_FileTestManager::tearDown(void)
{
    delete m_pFileTestManager;
    m_pFileTestManager = NULL;
}

void CUnit_FileTestManager::Test_FileTestStart(void)
{
    bool blRet = false;
    FileTestResultInfoSt objInfo = m_pFileTestManager->FileTestStart("./FileTestCfg.xml");

    if (objInfo.n4ConnectNum != 1 ||
        objInfo.n4TimeInterval != 10 ||
        objInfo.n4ProNum != 1 ||
        objInfo.n4Result != 0)
    {
        OUR_DEBUG((LM_INFO, "[Test_FileTestStart]objInfo.n4ConnectNum=%d.\n", objInfo.n4ConnectNum));
        OUR_DEBUG((LM_INFO, "[Test_FileTestStart]objInfo.n4TimeInterval=%d.\n", objInfo.n4TimeInterval));
        OUR_DEBUG((LM_INFO, "[Test_FileTestStart]objInfo.n4ProNum=%d.\n", objInfo.n4ProNum));
        OUR_DEBUG((LM_INFO, "[Test_FileTestStart]objInfo.n4Result=%d.\n", objInfo.n4Result));
        CPPUNIT_ASSERT_MESSAGE("[Test_FileTestStart]FileTestResultInfoSt error.", true == blRet);
    }
}

void CUnit_FileTestManager::Test_FileTestStop(void)
{
    bool blRet = false;

    m_pFileTestManager->HandlerServerResponse(1);

    int nRet = m_pFileTestManager->FileTestEnd();

    if (0 == nRet)
    {
        blRet = true;
    }

    m_pFileTestManager->Close();

    CPPUNIT_ASSERT_MESSAGE("[Test_FileTestStop]FileTestEnd error.", true == blRet);
}

void CUnit_FileTestManager::Test_handle_timeout(void)
{
    bool blRet = false;

    ACE_Time_Value tvNow = ACE_OS::gettimeofday();
    int nRet = m_pFileTestManager->handle_timeout(tvNow, NULL);

    if (0 == nRet)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_handle_timeout]handle_timeout error.", true == blRet);
}

#endif