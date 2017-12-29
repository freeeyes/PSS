#include "commons.h"
#include "testcase.h"

int main()
{
	//‘À––≤‚ ‘”√¿˝
	int nIndex = 0;
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
	
	CheckMultipleTcpPacket(10, objClientInfo, objResultInfo);
	objResultInfo.Display();
	
	CheckMultipleTcpConnect(10, objClientInfo, objResultInfo);
	objResultInfo.Display();	
	
	CheckTcpErrorPacketHead(objClientInfo, objResultInfo);
	objResultInfo.Display();		
	
	CheckTcpHalfPacket(objClientInfo, objResultInfo);
	objResultInfo.Display();		
	
	return 0;
}