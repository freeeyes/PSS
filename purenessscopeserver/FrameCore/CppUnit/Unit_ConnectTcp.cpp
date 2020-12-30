#include "Unit_ConnectTcp.h"

#ifdef _CPPUNIT_TEST

void CUnit_ConnectTcp::setUp(void)
{
    m_nServerID = 1;
}

void CUnit_ConnectTcp::tearDown(void)
{
    m_nServerID = 0;
}

void CUnit_ConnectTcp::Test_Connect_Tcp_Server(void)
{
    bool blRet                 = false;
    CPostServerData objPostServerData;
    objPostServerData.SetServerID(m_nServerID);

    //����Զ�̷�����
    if (false == App_ClientReConnectManager::instance()->Connect(m_nServerID,
        "127.0.0.1",
        10002,
        TYPE_IPV4,
        (IClientMessage*)&objPostServerData, 
        0))
    {
        PSS_LOGGER_DEBUG("[Test_Connect_Tcp_Server]Connect[127.0.0.1:10002]Tcp connect is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Tcp_Server]Connect[127.0.0.1:10002]Tcp connect is fail.", true == blRet);
        return;
    }

    ACE_Time_Value tvConnectSleep(0, 10000);
    ACE_OS::sleep(tvConnectSleep);

    //��õ�ǰ������Ϣ
    vecClientConnectInfo VecClientConnectInfo;
    App_ClientReConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);

    PSS_LOGGER_DEBUG("[Test_Connect_Tcp_Server]VecClientConnectInfo.size()={0}.", VecClientConnectInfo.size());
    if (1 != VecClientConnectInfo.size())
    {
        PSS_LOGGER_DEBUG("[Test_Connect_Tcp_Server]App_ClientReConnectManager::instance()->GetConnectInfo() is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Tcp_Server]App_ClientReConnectManager::instance()->GetConnectInfo() is fail.", true == blRet);
        return;
    }

    //���ServerID������״̬
    EM_Server_Connect_State emConnectState = App_ClientReConnectManager::instance()->GetConnectState(m_nServerID);

    if (emConnectState != EM_Server_Connect_State::SERVER_CONNECT_OK)
    {
        PSS_LOGGER_DEBUG("[Test_Connect_Tcp_Server]App_ClientReConnectManager::instance()->GetConnectState is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Tcp_Server]App_ClientReConnectManager::instance()->GetConnectState is fail.", true == blRet);
        return;
    }

    //��õ�ǰ�ѽ�˿���Ϣ
    App_ClientReConnectManager::instance()->GetServerAddr(m_nServerID);

    _ClientIPInfo objServerIPInfo;
    App_ClientReConnectManager::instance()->GetServerIPInfo(m_nServerID, objServerIPInfo);

    //����OK
    if (false == App_ClientReConnectManager::instance()->SetServerConnectState(m_nServerID, EM_Server_Connect_State::SERVER_CONNECT_OK))
    {
        PSS_LOGGER_DEBUG("[Test_Connect_Tcp_Server]App_ClientReConnectManager::instance()->SetServerConnectState is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Tcp_Server]App_ClientReConnectManager::instance()->SetServerConnectState is fail.", true == blRet);
        return;
    }

    //ƴװ���Է�������
    char szSendBuffer[MAX_BUFF_200] = { '\0' };
    char szBuff[20]                 = { '\0' };
    char szSession[32]              = { '\0' };
    sprintf_safe(szBuff, 20, "freeeyes");
    sprintf_safe(szSession, 32, "FREEEYES");

    short sVersion = 1;
    short sCommand = COMMAND_AUTOTEST_HEAD;
    int nPacketLen = (int)ACE_OS::strlen(szBuff);

    memcpy(szSendBuffer, (char*)&sVersion, sizeof(short));
    memcpy(&szSendBuffer[2], (char*)&sCommand, sizeof(short));
    memcpy(&szSendBuffer[4], (char*)&nPacketLen, sizeof(int));
    memcpy(&szSendBuffer[8], (char*)&szSession, sizeof(char) * 32);
    memcpy(&szSendBuffer[40], (char*)szBuff, sizeof(char) * nPacketLen);
    int nSendLen = nPacketLen + 40;

    char* pData = (char* )szSendBuffer;

    if (false == App_ClientReConnectManager::instance()->SendData(m_nServerID, pData, nSendLen, false))
    {
        PSS_LOGGER_DEBUG("[Test_Connect_Tcp_Server]Send TCP [127.0.0.1:10002] is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Tcp_Server]Send TCP [127.0.0.1:10002] is fail.\n", true == blRet);
        return;
    }

    //�ȴ�3�봦��������ݵ�ʱ��
    ACE_Time_Value tvSleep(0, 20*1000);
    ACE_OS::sleep(tvSleep);

    if (false == App_ClientReConnectManager::instance()->DeleteIClientMessage((IClientMessage*)&objPostServerData))
    {
        PSS_LOGGER_DEBUG("[Test_Connect_Tcp_Server]Close TCP [127.0.0.1:10002] is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Tcp_Server]Close TCP [127.0.0.1:10002] is fail.\n", true == blRet);
    }

    m_nTestCount++;
}

void CUnit_ConnectTcp::Test_Connect_Tcp_Server_With_Local(void)
{
    bool blRet = false;
    CPostServerData objPostServerData;
    objPostServerData.SetServerID(2);

    //����Զ�̷�����
    if (false == App_ClientReConnectManager::instance()->Connect(2,
        "127.0.0.1",
        10002,
        TYPE_IPV4,
        "127.0.0.1",
        20032,
        TYPE_IPV4,
        (IClientMessage*)&objPostServerData, 
        0))
    {
        PSS_LOGGER_DEBUG("[Test_Connect_Tcp_Server_With_Local]Connect[127.0.0.1:10002]Tcp connect is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Tcp_Server_With_Local]Connect[127.0.0.1:10002]Tcp connect is fail.", true == blRet);
        return;
    }

    ACE_Time_Value tvConnectSleep(0, 10000);
    ACE_OS::sleep(tvConnectSleep);

    if (false == App_ClientReConnectManager::instance()->Close(2))
    {
        PSS_LOGGER_DEBUG("[Test_Connect_Tcp_Server_With_Local]Close TCP [127.0.0.1:10002] is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Tcp_Server_With_Local]Close TCP [127.0.0.1:10002] is fail.\n", true == blRet);
        return;
    }

    //���Թرմ��������
    if (false != App_ClientReConnectManager::instance()->ConnectErrorClose(3))
    {
        PSS_LOGGER_DEBUG("[Test_Connect_Tcp_Server_With_Local]ConnectErrorClose is fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Tcp_Server_With_Local]ConnectErrorClose is fail.\n", true == blRet);
    }

    m_nTestCount++;
}

#endif


