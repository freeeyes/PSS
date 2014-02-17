// CommonTcpTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../CommonTCPClient/CommonTCPClient.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TPClient_Connect(1, "127.0.0.1", 10040, NULL);
	TPClient_Run(true);
	//TPClient_DisConnect(1);

	getchar();
	return 0;
}

