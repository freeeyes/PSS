#include "Unit_FileTestManager.h"

#ifdef _CPPUNIT_TEST

void CUnit_FileTestManager::setUp(void)
{
    m_pFileTestManager = std::make_shared <CFileTestManager>();
}

void CUnit_FileTestManager::tearDown(void)
{
    m_pFileTestManager->Close();
}

void CUnit_FileTestManager::Test_FileTestStart(void)
{
    bool blRet = false;
    FileTestResultInfoSt objInfo = m_pFileTestManager->FileTestStart("./FileTestCfg.xml");

    if (objInfo.n4ConnectNum != 1 ||
        objInfo.n4TimeInterval != 10 ||
        objInfo.n4ProNum != 1 ||
        objInfo.n4Result != FILE_TEST_RESULT::RESULT_OK)
    {
        PSS_LOGGER_DEBUG("[Test_FileTestStart]objInfo.n4ConnectNum={0}.", objInfo.n4ConnectNum);
        PSS_LOGGER_DEBUG("[Test_FileTestStart]objInfo.n4TimeInterval={0}.", objInfo.n4TimeInterval);
        PSS_LOGGER_DEBUG("[Test_FileTestStart]objInfo.n4ProNum={0}.", objInfo.n4ProNum);
        PSS_LOGGER_DEBUG("[Test_FileTestStart]objInfo.n4Result={0}.", objInfo.n4Result);
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
    //测试完整文件命令运行情况
    bool blRet = false;
    int  nRet = 0;

    FileTestResultInfoSt objInfo = m_pFileTestManager->FileTestStart("./FileTestCfg.xml");

    if (1 != objInfo.n4ConnectNum)
    {
        PSS_LOGGER_DEBUG("[Test_FileLogger]m_pFileTestManager->FileTestStart() fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pFileTestManager->FileTestStart() fail.", true == blRet);
        return;
    }

    ACE_Time_Value tvSleep(0, 10000);
    ACE_OS::sleep(tvSleep);

    m_pFileTestManager->HandlerServerResponse(1);

    nRet = m_pFileTestManager->FileTestEnd();

    if (0 == nRet)
    {
        blRet = true;
    }

    m_pFileTestManager->Close();

    CPPUNIT_ASSERT_MESSAGE("[Test_handle_timeout]handle_timeout error.", true == blRet);
}

#endif