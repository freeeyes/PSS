#include "Uint_ConsolePromissions.h"

#ifdef _CPPUNIT_TEST

void CUnit_ConsolePromissions::setUp(void)
{
    m_pConsolePromissions = std::make_shared<CConsolePromissions>();
    m_pConsolePromissions->Init(CONSOLECONFIG);
}

void CUnit_ConsolePromissions::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_ConsolePromissions::tearDown]Finish.");
}

void CUnit_ConsolePromissions::Test_Promission_User(void)
{
    bool blRet = false;
    int nPromission = m_pConsolePromissions->Check_Promission("LoadModule", "freeeyes");

    if (0 != nPromission)
    {
        PSS_LOGGER_DEBUG("[Test_Promission_User]freeeyes user valid fail.");
        CPPUNIT_ASSERT_MESSAGE("[Read_Write_String_BuffPacket]freeeyes user valid fail.", true == blRet);
    }
}

#endif

