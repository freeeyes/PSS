#pragma once

#include "ace/pre.h"
#include "ace/Date_Time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_string.h"
#include "ace/INET_Addr.h"

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
