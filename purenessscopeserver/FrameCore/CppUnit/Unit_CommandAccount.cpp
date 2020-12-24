#include "Unit_CommandAccount.h"

#ifdef _CPPUNIT_TEST

void CUnit_CommandAccount::setUp(void)
{
    m_pCommandAccount = std::make_shared<CCommandAccount>();

    m_u1CommandAccount     = 1;
    m_u1Flow               = 1;
    m_u2RecvTimeout        = 30;

    m_pCommandAccount->InitName("Test", 10);
    m_pCommandAccount->Init(m_u1CommandAccount, m_u1Flow, m_u2RecvTimeout);
}

void CUnit_CommandAccount::tearDown(void)
{
    m_pCommandAccount->Close();
}

void CUnit_CommandAccount::Test_CommandAccount_Init(void)
{
    bool blRet             = false;

    blRet = m_pCommandAccount->SaveCommandData(0x1000, 10002, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP, 10);
    blRet = m_pCommandAccount->SaveCommandData(0x1000, 10002, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP, 10);

    CPPUNIT_ASSERT_MESSAGE("[Test_CommandAccount_Init]pCommandData is Error.", true == blRet);
}

void CUnit_CommandAccount::Test_CommandAccount_Alert(void)
{
    bool blRet = false;

    m_pCommandAccount->AddCommandAlert(0x1000, 1, 0);

    vecCommandAlertData CommandAlertDataList;
    m_pCommandAccount->GetCommandAlertData(CommandAlertDataList);

    if (1 != CommandAlertDataList.size())
    {
        PSS_LOGGER_DEBUG("[Test_CommandAccount_Alert]Test_CommandAccount_Alert is {0} error.", CommandAlertDataList.size());
        CPPUNIT_ASSERT_MESSAGE("[Test_CommandAccount_Alert]GetCommandData is error.", true == blRet);
        return;
    }

    if (0x1000 == CommandAlertDataList[0].m_u2CommandID &&
        1 == CommandAlertDataList[0].m_u4CommandCount &&
        0 == CommandAlertDataList[0].m_u4CurrCount &&
        0 == CommandAlertDataList[0].m_u2MailID)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_CommandAccount_Alert]pCommandData is Error.", true == blRet);
}

void CUnit_CommandAccount::Test_CommandAccount_Flow(void)
{
    bool blRet = false;

    m_pCommandAccount->SaveCommandData(0x1000, 10002, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP, 10);
    m_pCommandAccount->SaveCommandData(0x1000, 10002, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP, 10);

    if (20 == m_pCommandAccount->GetFlowIn() &&
        0 == m_pCommandAccount->GetFlowOut() &&
        1 == m_pCommandAccount->GetFLow())
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_CommandAccount_Flow]pCommandData is Error.", true == blRet);
}

void CUnit_CommandAccount::Test_CommandAccount_PortList(void)
{
    bool blRet = false;

    m_pCommandAccount->SaveCommandData(0x1000, 10002, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP, 10);
    m_pCommandAccount->SaveCommandData(0x1000, 10002, EM_CONNECT_IO_TYPE::CONNECT_IO_TCP, 10);

    vector<_Port_Data_Account> vec_Port_Data_Account;
    m_pCommandAccount->GetFlowPortList(vec_Port_Data_Account);

    if (1 != vec_Port_Data_Account.size())
    {
        PSS_LOGGER_DEBUG("[Test_CommandAccount_PortList]vec_Port_Data_Account is {0}.", vec_Port_Data_Account.size());
        CPPUNIT_ASSERT_MESSAGE("[Test_CommandAccount_PortList]vec_Port_Data_Account is error.", true == blRet);
        return;
    }

    if (10002 == vec_Port_Data_Account[0].m_u4Port &&
        20 == vec_Port_Data_Account[0].GetFlowIn() &&
        0 == vec_Port_Data_Account[0].GetFlowOut())
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_CommandAccount_PortList]pCommandData is Error.", true == blRet);
}

#endif
