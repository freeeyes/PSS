#include "Unit_ConnectUdp.h"

#ifdef _CPPUNIT_TEST

void CUnit_ConnectUdp::setUp(void)
{
    m_nServerID = 2;
}

void CUnit_ConnectUdp::tearDown(void)
{
    m_nServerID = 0;
}

void CUnit_ConnectUdp::Test_Connect_Udp_Server(void)
{
    bool blRet                 = false;
    CPostUdpServerData objUdpPostServerData;

    //����Զ�̷�����
    if (false == App_ClientReConnectManager::instance()->ConnectUDP(m_nServerID,
            "127.0.0.1",
            20002,
            TYPE_IPV4,
            EM_UDP_TYPE::UDP_SINGLE,
            (IClientUDPMessage* )&objUdpPostServerData))
    {
        OUR_DEBUG((LM_INFO, "[Test_Connect_Udp_Server]Connect[127.0.0.1:10003]Udp connect is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Udp_Server]Connect[127.0.0.1:10003]Udp connect is fail.", true == blRet);
        return;
    }

    ACE_Time_Value tvConnectSleep(0, 10000);
    ACE_OS::sleep(tvConnectSleep);

    //ƴװ���Է�������
    char szSendBuffer[MAX_BUFF_200] = { '\0' };
    char szBuff[20]                 = { '\0' };
    char szSession[32]              = { '\0' };
    sprintf_safe(szBuff, 20, "freeeyes");
    sprintf_safe(szSession, 32, "FREEEYES");

    short sVersion = 1;
    short sCommand = COMMAND_AUTOTEST_UDP_HEAD;
    int nPacketLen = (int)ACE_OS::strlen(szBuff);

    memcpy(szSendBuffer, (char*)&sVersion, sizeof(short));
    memcpy(&szSendBuffer[2], (char*)&sCommand, sizeof(short));
    memcpy(&szSendBuffer[4], (char*)&nPacketLen, sizeof(int));
    memcpy(&szSendBuffer[8], (char*)&szSession, sizeof(char) * 32);
    memcpy(&szSendBuffer[40], (char*)szBuff, sizeof(char) * nPacketLen);
    int nSendLen = nPacketLen + 40;

    char* pData = (char* )szSendBuffer;

    if (false == App_ClientReConnectManager::instance()->SendDataUDP(m_nServerID, "127.0.0.1", 10003, pData, nSendLen, false))
    {
        OUR_DEBUG((LM_INFO, "[Test_Connect_Udp_Server]Send UDP [127.0.0.1:10003] is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Udp_Server]Send TCP [127.0.0.1:10003] is fail.\n", true == blRet);
    }

    //�ȴ�3�봦��������ݵ�ʱ��
    ACE_Time_Value tvSleep(0, 20*1000);
    ACE_OS::sleep(tvSleep);

    App_ClientReConnectManager::instance()->CloseUDP(m_nServerID);
    OUR_DEBUG((LM_INFO, "[Test_Connect_Udp_Server]Close OK.\n"));
    m_nTestCount++;
}

#endif


