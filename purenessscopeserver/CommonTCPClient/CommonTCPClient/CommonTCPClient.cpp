// CommonTCPClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CommonTCPClient.h"

//add by freeeyes

int TPClient_Run(bool blDebug)
{
	CTpClientManager::Instance().TPClient_Run(blDebug);
	return 0;
}

int TPClient_Connect(int nServerID, const char* pIP, int nPort, CRecvData* pRecvData)
{
	printf_s("[TPClient_Connect]Do.\n");
	return CTpClientManager::Instance().TPClient_Connect(nServerID, pIP, nPort, pRecvData);
}

int TPClient_DisConnect(int nServerID)
{
	printf_s("[TPClient_DisConnect]Do.\n");
	return CTpClientManager::Instance().TPClient_DisConnect(nServerID);
}

int TPClient_DisConnect_All()
{
	printf_s("[TPClient_DisConnect_All]Do.\n");
	return CTpClientManager::Instance().TPClient_DisConnect_All();
}

int TPClient_Send(int nServerID, const char* pBuff, int nLen)
{
	printf_s("[TPClient_Send]Do.\n");
	return CTpClientManager::Instance().TPClient_Send(nServerID, pBuff, nLen);
}

int TPClient_ConnectState(int nServerID)
{
	printf_s("[TPClient_ConnectState]Do.\n");
	return CTpClientManager::Instance().TPClient_ConnectState(nServerID);
}

int TPClient_ReConnect(int nServerID)
{
	printf_s("[TPClient_ReConnect]Do.\n");
	return CTpClientManager::Instance().TPClient_ReConnect(nServerID);
}


