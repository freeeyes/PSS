#pragma once

#include <stdio.h>
#if defined(_WIN32_PLATFORM_)
#include <WinSock2.h>
#else
#include <sys/time.h>
#include <linux/errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#endif


class CTcpClientConnect
{
public:
    CTcpClientConnect(void);
    ~CTcpClientConnect(void);

    void Init(const char* pIp, int nPort, const char* pKey);
    char* GetKey();

    bool SendConsoleMessage(const char* pMessage, int nMessageLen, char* pRevBuff, int& nRecvLen);

    char* GetServerIP();
    int GetServerPort();
    char* GetServerKey();

private:
    char m_szServerIP[20];
    int  m_nServerPort;
    char m_szConsoleKey[50];
};
