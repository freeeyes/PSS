#include "TcpClientConnect.h"

CTcpClientConnect::CTcpClientConnect(void)
{
}

CTcpClientConnect::~CTcpClientConnect(void)
{
}

void CTcpClientConnect::Init( const char* pIp, int nPort, const char* pKey )
{
  ACE_OS::snprintf(m_szServerIP, 20, "%s", pIp);
  ACE_OS::snprintf(m_szConsoleKey, 50, "%s", pKey);
  m_nServerPort = nPort;
}

bool CTcpClientConnect::SendConsoleMessage( const char* pMessage, int nMessageLen, char* pRevBuff, int& nRecvLen )
{
  ACE_HANDLE sckClient;
  char* pBuff = new char[1024*100];
  int nPacketLen = 0;

  //socket创建的准备工作
  //struct sockaddr_in sockaddr;
  ACE_INET_Addr addrServer;
  addrServer.set(m_nServerPort, m_szServerIP);

  sckClient = ACE_OS::socket(AF_INET, SOCK_STREAM, 0);

  //连接远程服务器
  int nErr = ACE_OS::connect(sckClient, reinterpret_cast<sockaddr  *>(addrServer.get_addr()), addrServer.get_size());
  if(0 != nErr)
  {
    delete[] pBuff;
	ACE_OS::closesocket(sckClient);
    return false;
  }

  int nTotalRecvLen = nRecvLen;
  int nTotalSendLen = nMessageLen;
  int nBeginSend    = 0;
  int nCurrSendLen  = 0;
  int nFinishRecv   = 0;
  bool blSendFlag   = false;

  while(true)
  {
    nCurrSendLen = ACE_OS::send(sckClient, pMessage + nBeginSend, nTotalSendLen, 0);
    if(nCurrSendLen <= 0)
    {
      delete[] pBuff;
      ACE_OS::closesocket(sckClient);
      return false;
    }
    else
    {
      nTotalSendLen -= nCurrSendLen;
      if(nTotalSendLen == 0)
      {
        //发送完成
        blSendFlag = true;
        break;
      }
      else
      {
        nBeginSend += nCurrSendLen;
      }
    }
  }

  int nCurrRecvLen = 0;
  //接收数据
  if(blSendFlag == true)
  {
    while(true)
    {
      //接收数据
      int nLen = ACE_OS::recv(sckClient, (char* )&pBuff[nCurrRecvLen], 1024*100 - nCurrRecvLen, 0);
      if(nLen <= 0)
      {
        delete[] pBuff;
        ACE_OS::closesocket(sckClient);
        return false;
      }

      if((int)nLen >= 4)
      {
        ACE_OS::memcpy(&nPacketLen, pBuff, sizeof(int));
        nFinishRecv = nPacketLen + 4;
      }

      nCurrRecvLen += (int)nLen;

      if(nCurrRecvLen >= nFinishRecv)
      {
        break;
      }
    }
  }

  if(nFinishRecv <= nRecvLen)
  {
    ACE_OS::memcpy(pRevBuff, pBuff, nFinishRecv);
    nRecvLen = nFinishRecv;
  }

  delete[] pBuff;
  ACE_OS::closesocket(sckClient);
  return true;
}

char* CTcpClientConnect::GetKey()
{
  return m_szConsoleKey;
}

char* CTcpClientConnect::GetServerIP()
{
	return m_szServerIP;
}

int CTcpClientConnect::GetServerPort()
{
	return m_nServerPort;
}

char* CTcpClientConnect::GetServerKey()
{
	return m_szConsoleKey;
}
