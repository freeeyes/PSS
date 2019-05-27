#include "Unit_CommandAccount.h"

#ifdef _CPPUNIT_TEST

CUnit_CommandAccount::CUnit_CommandAccount()
{
    m_pCommandAccount  = 0;
    m_u1CommandAccount = 1;
    m_u1Flow           = 0;
    m_u2RecvTimeout    = 30;
}

CUnit_CommandAccount::CUnit_CommandAccount(const CUnit_CommandAccount& ar)
{
    (*this) = ar;
}

CUnit_CommandAccount::~CUnit_CommandAccount()
{
}

void CUnit_CommandAccount::setUp(void)
{
    m_pCommandAccount = new CCommandAccount();

    m_u1CommandAccount     = 1;
    m_u1Flow               = 1;
    m_u2RecvTimeout        = 30;

    m_pCommandAccount->InitName("Test", 10);
    m_pCommandAccount->Init(m_u1CommandAccount, m_u1Flow, m_u2RecvTimeout);
}

void CUnit_CommandAccount::tearDown(void)
{
    delete m_pCommandAccount;
    m_pCommandAccount = NULL;
}

void CUnit_CommandAccount::Test_CommandAccount_Init(void)
{
    bool blRet             = false;

    m_pCommandAccount->SaveCommandData(0x1000, 10002, CONNECT_IO_TCP, 10);
    m_pCommandAccount->SaveCommandData(0x1000, 10002, CONNECT_IO_TCP, 10);

    _CommandData* pCommandData = m_pCommandAccount->GetCommandData(0x1000);

    if (NULL == pCommandData)
    {
        OUR_DEBUG((LM_INFO, "[Test_CommandAccount_Init]GetCommandData is NULL.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_CommandAccount_Init]GetCommandData is NULL.", true == blRet);
        return;
    }

    if (COMMAND_TYPE_IN == pCommandData->m_u1CommandType &&
        0x1000 == pCommandData->m_u2CommandID &&
        20 == pCommandData->m_u4PacketSize &&
        2 == pCommandData->m_u4CommandCount &&
        CONNECT_IO_TCP == pCommandData->m_u1PacketType)
    {
        blRet = true;
    }


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
        OUR_DEBUG((LM_INFO, "[Test_CommandAccount_Alert]Test_CommandAccount_Alert is %d error.\n", CommandAlertDataList.size()));
        CPPUNIT_ASSERT_MESSAGE("[Test_CommandAccount_Alert]GetCommandData is error.", true == blRet);
        return;
    }

    if (0x1000 == CommandAlertDataList[0].m_u2CommandID &&
        1 == CommandAlertDataList[0].m_u4CommandCount &&
        0 == CommandAlertDataList[0].m_u4CurrCount &&
        0 == CommandAlertDataList[0].m_u4MailID)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_CommandAccount_Alert]pCommandData is Error.", true == blRet);
}

void CUnit_CommandAccount::Test_CommandAccount_Flow(void)
{
    bool blRet = false;

    m_pCommandAccount->SaveCommandData(0x1000, 10002, CONNECT_IO_TCP, 10);
    m_pCommandAccount->SaveCommandData(0x1000, 10002, CONNECT_IO_TCP, 10);

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

    m_pCommandAccount->SaveCommandData(0x1000, 10002, CONNECT_IO_TCP, 10);
    m_pCommandAccount->SaveCommandData(0x1000, 10002, CONNECT_IO_TCP, 10);

    vector<_Port_Data_Account> vec_Port_Data_Account;
    m_pCommandAccount->GetFlowPortList(vec_Port_Data_Account);

    if (1 != vec_Port_Data_Account.size())
    {
        OUR_DEBUG((LM_INFO, "[Test_CommandAccount_PortList]vec_Port_Data_Account is %d.\n", vec_Port_Data_Account.size()));
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
