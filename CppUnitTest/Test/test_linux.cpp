#include <string>
#include "commons.h"
#include "testcase.h"

using namespace std;

int main()
{
    //运行测试用例
    int nTestCount = 0;
    string strResultInfo;
    char szHtmlResult[MAX_BUFF_1024] = {'\0'};
    _ResultInfo objResultInfo;
    _ClientInfo objClientInfo;

    //获得当前测试运行时间
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

    //开始测试用例
    char szBuff[20] = {'\0'};
    sprintf(szBuff, "freeeyes");

    sprintf(objClientInfo.m_szServerIP, "127.0.0.1");
    objClientInfo.m_nPort       = 10002;
    objClientInfo.m_pSendBuffer = szBuff;
    objClientInfo.m_nSendLength = (int)strlen(szBuff);
    objClientInfo.m_nRecvLength = (int)strlen(szBuff);

    CheckTcpPacket(objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    CheckMultipleTcpPacket(10, objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    CheckMultipleTcpConnect(10, objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    CheckTcpErrorPacketHead(objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    CheckTcpHalfPacket(objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

    CheckTcpMulipleThreadPacket(10, objClientInfo, objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;

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

    CheckConsolePacket(objResultInfo);
    objResultInfo.Display();
    objResultInfo.To_Html(szHtmlResult);
    strResultInfo += szHtmlResult;
    nTestCount++;
    
    //输出成文件格式
    char* pHtmlFile = new char[nTestCount*MAX_BUFF_1024];
    memset(pHtmlFile, 0, nTestCount*MAX_BUFF_1024);

    sprintf(pHtmlFile, HTML_REPORT, szTimeNow, strResultInfo.c_str());

    FILE* pFile = fopen(HTML_NAME, "w");

    if(NULL != pFile)
    {
        fwrite(pHtmlFile, sizeof(char), strlen(pHtmlFile), pFile);

        fclose(pFile);
    }

    return 0;
}
