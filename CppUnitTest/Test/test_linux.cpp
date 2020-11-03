#include <string>
#include "commons.h"
#include "testcase.h"

using namespace std;

int main()
{
    //���в�������
    int nTestCount = 0;
    string strResultInfo;
    char szHtmlResult[MAX_BUFF_1024] = {'\0'};
    _ResultInfo objResultInfo;
    _ClientInfo objClientInfo;

    //��õ�ǰ��������ʱ��
    time_t ttCurrentTime;
    time(&ttCurrentTime);
    struct tm* pTime;
    pTime = gmtime(&ttCurrentTime);
    char szTimeNow[50] = {'\0'};
    sprintf(szTimeNow, "[Test CI run UTC time] %04d-%02d-%02d %02d:%02d:%02d",
            pTime->tm_year + 1900,
            pTime->tm_mon + 1,
            pTime->tm_mday,
            pTime->tm_hour,
            pTime->tm_min,
            pTime->tm_sec);

    //��ʼ��������
    char szBuff[20] = {'\0'};
    sprintf(szBuff, "freeeyes");

    sprintf(objClientInfo.m_szServerIP, "127.0.0.1");
    objClientInfo.m_nPort       = 10002;
    objClientInfo.m_pSendBuffer = szBuff;
    objClientInfo.m_nSendLength = (int)strlen(szBuff);
    objClientInfo.m_nRecvLength = (int)strlen(szBuff);

    printf("Test Begin.\n");

    CheckTcpPacket(objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    printf("Test Begin (%d)\n", nTestCount);

    CheckMultipleTcpPacket(10, objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    printf("Test Begin (%d)\n", nTestCount);

    CheckMultipleTcpConnect(10, objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    printf("Test Begin (%d)\n", nTestCount);

    CheckTcpErrorPacketHead(objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    printf("Test Begin (%d)\n", nTestCount);

    CheckTcpHalfPacket(objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    printf("Test Begin (%d)\n", nTestCount);

    CheckTcpMulipleThreadPacket(10, objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    printf("Test Begin (%d)\n", nTestCount);

    _ResultInfo objRecvResultInfo;
    Thread_CheckUdpPacket(objClientInfo, objResultInfo, objRecvResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;
    objRecvResultInfo.Display();
    objRecvResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    printf("Test Begin (%d)\n", nTestCount);

    CheckConsolePacket(objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    printf("Test Begin (%d)\n", nTestCount);

    //������ļ���ʽ
    char* pHtmlFile = new char[nTestCount*MAX_BUFF_1024];
    memset(pHtmlFile, 0, nTestCount*MAX_BUFF_1024);

    sprintf(pHtmlFile, HTML_REPORT, szTimeNow, strResultInfo.c_str());

    FILE* pFile = fopen(HTML_NAME, "w");

    if(NULL != pFile)
    {
        fwrite(pHtmlFile, sizeof(char), strlen(pHtmlFile), pFile);

        fclose(pFile);
    }

    printf("Test End.\n");

    return 0;
}
