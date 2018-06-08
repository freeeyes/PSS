#include "Unit_ConnectTcp.h"

#ifdef _CPPUNIT_TEST

CUnit_ConnectTcp::~CUnit_ConnectTcp()
{

}

void CUnit_ConnectTcp::setUp(void)
{
}

void CUnit_ConnectTcp::tearDown(void)
{
}

void CUnit_ConnectTcp::Test_Connect_Tcp_Server(void)
{
    bool blRet                 = false;
    CPostServerData objPostServerData;
    objPostServerData.SetServerID(1);

    //连接远程服务器
    if (false == App_ClientReConnectManager::instance()->Connect(1,
            "127.0.0.1",
            10002,
            TYPE_IPV4,
            (IClientMessage*)&objPostServerData))
    {
        OUR_DEBUG((LM_INFO, "[Test_Connect_Tcp_Server]Connect[127.0.0.1:10002]Tcp connect is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Tcp_Server]Connect[127.0.0.1:10002]Tcp connect is fail.", true == blRet);
        return;
    }

    ACE_Time_Value tvConnectSleep(0, 10000);
    ACE_OS::sleep(tvConnectSleep);

    //拼装测试发送数据
    char szSendBuffer[MAX_BUFF_200] = { '\0' };
    char szBuff[20]                 = { '\0' };
    char szSession[32]              = { '\0' };
    sprintf_safe(szBuff, 20, "freeeyes");
    sprintf_safe(szSession, 32, "FREEEYES");

    short sVersion = 1;
    short sCommand = (short)COMMAND_AUTOTEST_HEAD;
    int nPacketLen = ACE_OS::strlen(szBuff);

    memcpy(szSendBuffer, (char*)&sVersion, sizeof(short));
    memcpy((char*)&szSendBuffer[2], (char*)&sCommand, sizeof(short));
    memcpy((char*)&szSendBuffer[4], (char*)&nPacketLen, sizeof(int));
    memcpy((char*)&szSendBuffer[8], (char*)&szSession, sizeof(char) * 32);
    memcpy((char*)&szSendBuffer[40], (char*)szBuff, sizeof(char) * nPacketLen);
    int nSendLen = nPacketLen + 40;

    char* pData = (char* )szSendBuffer;

    if (false == App_ClientReConnectManager::instance()->SendData(1, pData, nSendLen, false))
    {
        OUR_DEBUG((LM_INFO, "[Test_Connect_Tcp_Server]Send TCP [127.0.0.1:10002] is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_Connect_Tcp_Server]Send TCP [127.0.0.1:10002] is fail.\n", true == blRet);
    }

    //等待3秒处理接收数据的时间
    ACE_Time_Value tvSleep(0, 20*1000);
    ACE_OS::sleep(tvSleep);

    App_ClientReConnectManager::instance()->Close(1);
    OUR_DEBUG((LM_INFO, "[Test_Connect_Tcp_Server]Close OK.\n"));
}

#endif


