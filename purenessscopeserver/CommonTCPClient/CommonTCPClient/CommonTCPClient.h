#ifndef _COMMONTCPLINET_H
#define _COMMONTCPLINET_H

#include "TpClientManager.h"

#ifdef TCPLINET_BUILD_DLL
#define DLL_PUBLIC __declspec(dllexport)
#else
#define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif


DLL_PUBLIC int TPClient_Run(bool blDebug = false);

DLL_PUBLIC int TPClient_Connect(int nServerID, const char* pIP, int nPort, CRecvData* pRecvData = NULL);

DLL_PUBLIC int TPClient_DisConnect(int nServerID);

DLL_PUBLIC int TPClient_DisConnect_All();

DLL_PUBLIC int TPClient_Send(int nServerID, const char* pBuff, int nLen);

DLL_PUBLIC int TPClient_ConnectState(int nServerID);

DLL_PUBLIC int TPClient_ReConnect(int nServerID);

#endif
