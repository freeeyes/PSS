#include "Uint_AceReactorManager.h"

#ifdef _CPPUNIT_TEST

CUnit_AceReactorManager::CUnit_AceReactorManager()
{
    m_pReactorManager = NULL;
}

CUnit_AceReactorManager::~CUnit_AceReactorManager()
{

}

void CUnit_AceReactorManager::setUp(void)
{
    m_pReactorManager = new CAceReactorManager();
    m_pReactorManager->Init(10);
}

void CUnit_AceReactorManager::tearDown(void)
{
    delete m_pReactorManager;
    m_pReactorManager = NULL;
}

void CUnit_AceReactorManager::Test_DoMessage_AceReactorManager(void)
{
    //测试创建指定的反应器
    int blRet = false;
    blRet = m_pReactorManager->AddNewReactor(1, Reactor_Select);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_DoMessage_AceReactorManager]AddNewReactor Reactor_Select is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_DoMessage_AceReactorManager]AddNewReactor Reactor_Select is fail.", true == blRet);
        return;
    }

    blRet = m_pReactorManager->AddNewReactor(2, Reactor_TP);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_DoMessage_AceReactorManager]AddNewReactor Reactor_TP is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_DoMessage_AceReactorManager]AddNewReactor Reactor_TP is fail.", true == blRet);
        return;
    }

    CAceReactor* pAceReactor = NULL;
    pAceReactor = m_pReactorManager->GetAceReactor(1);

    if (1 != pAceReactor->GetThreadCount() || Reactor_Select != pAceReactor->GetReactorType())
    {
        OUR_DEBUG((LM_INFO, "[Test_DoMessage_AceReactorManager]Reactor_Select GetThreadCount(%s) is fail.\n", pAceReactor->GetError()));
        CPPUNIT_ASSERT_MESSAGE("[Test_DoMessage_AceReactorManager]Reactor_Select GetThreadCount is fail.", true == blRet);
        return;
    }

    pAceReactor = m_pReactorManager->GetAceReactor(2);

    if (1 != pAceReactor->GetThreadCount() || Reactor_TP != pAceReactor->GetReactorType())
    {
        OUR_DEBUG((LM_INFO, "[Test_DoMessage_AceReactorManager]Reactor_TP GetThreadCount(%s) is fail.\n", pAceReactor->GetError()));
        CPPUNIT_ASSERT_MESSAGE("[Test_DoMessage_AceReactorManager]Reactor_TP GetThreadCount is fail.", true == blRet);
        return;
    }

    m_pReactorManager->Close();

}

#endif

