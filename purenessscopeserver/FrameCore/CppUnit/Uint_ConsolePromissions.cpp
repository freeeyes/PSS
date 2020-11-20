#include "Uint_ConsolePromissions.h"

#ifdef _CPPUNIT_TEST

void CUnit_ConsolePromissions::setUp(void)
{
    m_pConsolePromissions = std::make_shared<CConsolePromissions>();
    m_pConsolePromissions->Init(CONSOLECONFIG);
}

void CUnit_ConsolePromissions::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_ConsolePromissions::tearDown]Finish.\n"));
}

void CUnit_ConsolePromissions::Test_Promission_User(void)
{
    bool blRet = false;
    int nPromission = m_pConsolePromissions->Check_Promission("LoadModule", "freeeyes");

    if (0 != nPromission)
    {
        OUR_DEBUG((LM_INFO, "[Test_Promission_User]freeeyes user valid fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Read_Write_String_BuffPacket]freeeyes user valid fail.", true == blRet);
    }
}

#endif

