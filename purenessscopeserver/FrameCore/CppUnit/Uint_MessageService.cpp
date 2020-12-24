#include "Uint_MessageService.h"

#ifdef _CPPUNIT_TEST

void CUnit_MessageService::setUp(void)
{
    m_pMessageService = std::make_shared<CMessageService>();
    m_pMessageService->Init(1);
}

void CUnit_MessageService::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_MessageService::tearDown]Finish.");
}

void CUnit_MessageService::Test_MessageService(void)
{
    //测试创建指定的反应器
    bool blRet = false;
    uint32 u4ThreadID = m_pMessageService->GetThreadID();

    if (1 != u4ThreadID)
    {
        PSS_LOGGER_DEBUG("[Test_MessageService]GetThreadID is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_MessageService]GetThreadID is fail.", true == blRet);
        return;
    }

    m_pMessageService->SetThreadState(MESSAGE_SERVICE_THREAD_STATE::THREAD_RUN);

    if (MESSAGE_SERVICE_THREAD_STATE::THREAD_RUN != m_pMessageService->GetThreadState())
    {
        PSS_LOGGER_DEBUG("[Test_MessageService]GetThreadState is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_MessageService]GetThreadState is fail.", true == blRet);
        return;
    }

    if (0 != m_pMessageService->GetUsedMessageCount())
    {
        PSS_LOGGER_DEBUG("[Test_MessageService]GetUsedMessageCount is fail({0}).", m_pMessageService->GetUsedMessageCount());
        CPPUNIT_ASSERT_MESSAGE("[Test_MessageService]GetUsedMessageCount is fail.", true == blRet);
        return;
    }

    if (THREADSTATE::THREAD_INIT != m_pMessageService->GetStepState())
    {
        PSS_LOGGER_DEBUG("[Test_MessageService]GetStepState is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_MessageService]GetStepState is fail.", true == blRet);
        return;
    }

    App_MessageServiceGroup::instance()->CheckCPUAndMemory(true);

    m_pMessageService->Close();
}

#endif

