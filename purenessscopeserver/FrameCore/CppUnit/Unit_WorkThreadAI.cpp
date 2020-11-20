#include "Unit_WorkThreadAI.h"

#ifdef _CPPUNIT_TEST

void CUnit_WorkThreadAI::setUp(void)
{
    m_pWorkThreadAI = std::make_shared<CWorkThreadAI>();
    m_pWorkThreadAI->Init(1, 10, 30, 1, 1, 1, "ff ff");
}

void CUnit_WorkThreadAI::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_WorkThreadAI::tearDown]Finish.\n"));
}

void CUnit_WorkThreadAI::Test_WorkThreadAI(void)
{
    bool blRet = false;

    //测试计算超时功能
    uint16 u2Command = 0x1000;

    //故意插入一条超时的指令
    if (false == m_pWorkThreadAI->SaveTimeout(u2Command, 10))
    {
        OUR_DEBUG((LM_INFO, "[Test_WorkThreadAI]m_pWorkThreadAI->SaveTimeout() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_WorkThreadAI]m_pWorkThreadAI->SaveTimeout() Error.", true == blRet);
        return;
    }

    uint64 u8Now = ACE_OS::gettimeofday().sec();

    if (true != m_pWorkThreadAI->CheckCurrTimeout(u2Command, u8Now))
    {
        OUR_DEBUG((LM_INFO, "[Test_WorkThreadAI]m_pWorkThreadAI->CheckCurrTimeout() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_WorkThreadAI]m_pWorkThreadAI->CheckCurrTimeout() Error.", true == blRet);
        return;
    }

    const char* pData = m_pWorkThreadAI->GetReturnData();

    if (0xff != (unsigned char)pData[0] || 0xff != (unsigned char)pData[1])
    {
        OUR_DEBUG((LM_INFO, "[Test_WorkThreadAI]m_pWorkThreadAI->GetReturnData() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_WorkThreadAI]m_pWorkThreadAI->GetReturnData() Error.", true == blRet);
        return;
    }

    if (2 != m_pWorkThreadAI->GetReturnDataLength())
    {
        OUR_DEBUG((LM_INFO, "[Test_WorkThreadAI]m_pWorkThreadAI->GetReturnDataLength() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_WorkThreadAI]m_pWorkThreadAI->GetReturnDataLength() Error.", true == blRet);
        return;
    }

    m_pWorkThreadAI->ReSet(1, 10, 30, 1);

    vecCommandTimeout objTimeout;
    m_pWorkThreadAI->GetAllTimeout(1, objTimeout);

    if (1 != objTimeout.size())
    {
        OUR_DEBUG((LM_INFO, "[Test_WorkThreadAI]m_pWorkThreadAI->GetAllTimeout() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_WorkThreadAI]m_pWorkThreadAI->GetAllTimeout() Error.", true == blRet);
        return;
    }

    vecCommandTimeout objForbiden;
    m_pWorkThreadAI->GetAllForbiden(1, objForbiden);

    if (1 != objForbiden.size())
    {
        OUR_DEBUG((LM_INFO, "[Test_WorkThreadAI]m_pWorkThreadAI->GetAllForbiden() Error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_WorkThreadAI]m_pWorkThreadAI->GetAllForbiden() Error.", true == blRet);
    }
}

#endif
