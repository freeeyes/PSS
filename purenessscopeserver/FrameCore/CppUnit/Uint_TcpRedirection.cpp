#include "Uint_TcpRedirection.h"

#ifdef _CPPUNIT_TEST

CUnit_Redirection::CUnit_Redirection()
{
    m_pForwardManager = NULL;
}

void CUnit_Redirection::setUp(void)
{
    m_pForwardManager = App_ForwardManager::instance();
    m_pForwardManager->AddForward("127.0.0.1:12005", "127.0.0.1:12006");
}

void CUnit_Redirection::tearDown(void)
{
    m_pForwardManager = NULL;
}

void CUnit_Redirection::Test_Redirection(void)
{
    //测试转发模块
    int sockclient1;
    int sockclient2;
    bool blRet = false;
    char szBuffSend[MAX_BUFF_100] = { '\0' };
    char szBuffRecv[MAX_BUFF_100] = { '\0' };

    sockclient1 = Create_client(12005, "127.0.0.1", 10002);
    sockclient2 = Create_client(12006, "127.0.0.1", 10002);

    OUR_DEBUG((LM_INFO, "[CUnit_Redirection::Test_Redirection]sockclient1=%d, sockclient1=%d.\n", sockclient1, sockclient2));

    if (0 == sockclient1 || 0 == sockclient2)
    {
        OUR_DEBUG((LM_INFO, "[CUnit_Redirection::Test_Redirection]connect is error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[CUnit_Redirection::Test_Redirection]connect is error.", true == blRet);
        return;
    }

    //尝试发送数据
    sprintf_safe(szBuffSend, MAX_BUFF_100, "freeeyes");
    send(sockclient1, szBuffSend, ACE_OS::strlen(szBuffSend), 0);

    int nRecvLen = recv(sockclient2, szBuffRecv, MAX_BUFF_100, 0);

    if (nRecvLen != (int)ACE_OS::strlen(szBuffSend))
    {
        OUR_DEBUG((LM_INFO, "[CUnit_Redirection::Test_Redirection]connect is error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[CUnit_Redirection::Test_Redirection]recv is error.", true == blRet);
    }

    close(sockclient1);
    close(sockclient2);
}

int CUnit_Redirection::Create_client(int nClientPort, const char* pIP, int nServerPort)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in mine;
    struct sockaddr_in dest;

    memset(&mine, 0, sizeof(mine));

    mine.sin_family = AF_INET;

    mine.sin_port = htons(nClientPort);

    inet_pton(AF_INET, pIP, &mine.sin_addr);

    memset(&dest, 0, sizeof(dest));

    dest.sin_family = AF_INET;

    dest.sin_port = htons(nServerPort);

    inet_pton(AF_INET, pIP, &dest.sin_addr);

    int b = bind(sockfd, (struct sockaddr*)&mine, sizeof(mine));

    if (b == -1)
    {
        return 0;
    }

    if (0 == connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)))
    {
        ACE_Time_Value tvSleep(0, 1000);
        ACE_OS::sleep(tvSleep);

        return sockfd;
    }
    else
    {
        return 0;
    }
}

#endif

