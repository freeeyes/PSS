// CommonTcpTest.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include "../CommonTCPClient/CommonTCPClient.h"

int main(int argc, char* argv[])
{
	TPClient_Connect(1, "127.0.0.1", 10040, NULL);
	TPClient_Run(true);
	//TPClient_DisConnect(1);

	getchar();
	return 0;
}

