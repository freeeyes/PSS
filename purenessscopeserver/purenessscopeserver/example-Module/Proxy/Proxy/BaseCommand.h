#pragma once
#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "ProxyClient.h"

#include <string>

//定义客户端信令(TCP)
#define COMMAND_PROXY           0x1030

#define PROXY_SERVER_IP   "127.0.0.1"
#define PROXY_SERVER_PORT 10040

using namespace std;

class CBaseCommand : public CClientCommand
{
public:
  CBaseCommand(void);
  ~CBaseCommand(void);

  int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
  void SetServerObject(CServerObject* pServerObject);
  void Init();

private:
	void Do_Proxy_Connect(IMessage* pMessage);          //与代理服务器连接建立  
	void Do_Proxy_DisConnect(IMessage* pMessage);       //与连接服务器断开连接
	void Do_Proxy_Data(IMessage* pMessage);             //发送代理数据

private:
  CServerObject*           m_pServerObject;
  int                      m_nCount;

};
