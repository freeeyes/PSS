#include "Unit_WorkThreadAI.h"

#ifdef _CPPUNIT_TEST

void CUnit_WorkThreadAI::setUp(void)
{
    m_pWorkThreadAI = std::make_shared<CWorkThreadAI>();
    m_pWorkThreadAI->Init(1, 10, 30, 1, 1, 1, "ff ff");
}

void CUnit_WorkThreadAI::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_WorkThreadAI::tearDown]Finish.");
}

void CUnit_WorkThreadAI::Test_WorkThreadAI(void)
{
    bool blRet = false;

    //测试计算超时功能
    uint16 u2Command = 0x1000;

    //故意插入一条超时的指令
    if (false == m_pWorkThreadAI->SaveTimeout(u2Command, 10))
    {
        PSS_LOGGER_DEBUG("[Test_WorkThreadAI]m_pWorkThreadAI->SaveTimeout() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_WorkThreadAI]m_pWorkThreadAI->SaveTimeout() Error.", true == blRet);
        return;
    }

    auto u8Now = CTimeStamp::Get_Time_use_second(CTimeStamp::Get_Time_Stamp());

    if (true != m_pWorkThreadAI->CheckCurrTimeout(u2Command, u8Now))
    {
        PSS_LOGGER_DEBUG("[Test_WorkThreadAI]m_pWorkThreadAI->CheckCurrTimeout() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_WorkThreadAI]m_pWorkThreadAI->CheckCurrTimeout() Error.", true == blRet);
        return;
    }

    if (5 != m_pWorkThreadAI->GetReturnDataLength())
    {
        PSS_LOGGER_DEBUG("[Test_WorkThreadAI]m_pWorkThreadAI->GetReturnDataLength() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_WorkThreadAI]m_pWorkThreadAI->GetReturnDataLength() Error.", true == blRet);
        return;
    }

    m_pWorkThreadAI->ReSet(1, 10, 30, 1);

    vecCommandTimeout objTimeout;
    m_pWorkThreadAI->GetAllTimeout(1, objTimeout);

    if (1 != objTimeout.size())
    {
        PSS_LOGGER_DEBUG("[Test_WorkThreadAI]m_pWorkThreadAI->GetAllTimeout() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_WorkThreadAI]m_pWorkThreadAI->GetAllTimeout() Error.", true == blRet);
        return;
    }

    vecCommandTimeout objForbiden;
    m_pWorkThreadAI->GetAllForbiden(1, objForbiden);

    if (1 != objForbiden.size())
    {
        PSS_LOGGER_DEBUG("[Test_WorkThreadAI]m_pWorkThreadAI->GetAllForbiden() Error.");
        CPPUNIT_ASSERT_MESSAGE("[Test_WorkThreadAI]m_pWorkThreadAI->GetAllForbiden() Error.", true == blRet);
    }
}

#endif
