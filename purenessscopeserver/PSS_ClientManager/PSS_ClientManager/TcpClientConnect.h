#pragma once

#include <WinSock2.h>

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
