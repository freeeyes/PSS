#include "Unit_EChartlog.h"

#ifdef _CPPUNIT_TEST

void CUnit_EChartlog::setUp(void)
{
    m_pEchartlog = App_Echartlog::instance();
}

void CUnit_EChartlog::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_EChartlog::tearDown]is Close.");
}

void CUnit_EChartlog::Test_EChartlog(void)
{
    bool blRet = false;

    bool nRet = m_pEchartlog->Writelog("./", "testchart.txt", "²âÊÔ±¨±í", "15");

    if (true != nRet)
    {
        blRet = true;
        CPPUNIT_ASSERT_MESSAGE("[CUnit_EChartlog]m_pTMService->AddMessage is fail.", true == blRet);
        return;
    }

    PSS_LOGGER_DEBUG("[CUnit_EChartlog]objActiveTimer is finish.");
}

#endif
