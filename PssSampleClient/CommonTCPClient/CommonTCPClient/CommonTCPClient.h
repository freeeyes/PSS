#ifndef _COMMONTCPLINET_H
#define _COMMONTCPLINET_H

#include "TpClientManager.h"



 int TPClient_Run(bool blDebug = false);

 int TPClient_Connect(int nServerID, const char* pIP, int nPort, CRecvData* pRecvData = NULL);

 int TPClient_DisConnect(int nServerID);

 int TPClient_DisConnect_All();

 int TPClient_Send(int nServerID, const char* pBuff, int nLen);

 int TPClient_ConnectState(int nServerID);

 int TPClient_ReConnect(int nServerID);

#endif
