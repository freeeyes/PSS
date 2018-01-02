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
	
	//输出成文件格式
	char* pHtmlFile = new char[nTestCount*MAX_BUFF_1024];
	memset(pHtmlFile, 0, nTestCount*MAX_BUFF_1024);
	
	sprintf(pHtmlFile, HTML_REPORT, strResultInfo.c_str());
	
	FILE* pFile = fopen(HTML_NAME, "w");
	if(NULL != pFile)
	{
		fwrite(pHtmlFile, sizeof(char), strlen(pHtmlFile), pFile);
	
		fclose(pFile);
	}
	
	return 0;
}