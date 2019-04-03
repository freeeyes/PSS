#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

#include <string>

//定义客户端信令(TCP)
#define COMMAND_HANDIN          0xee01
#define COMMAND_DATAIN          0xee02
#define COMMAND_RETURN_HANDIN   0xff01
#define COMMAND_RETURN_DATAIN   0xff02

//设置握手相关信息
#define CLIENT_KEY_TAG     "Sec-WebSocket-Key:"
#define HANDIN_RETURN_DATA "HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept:%s\r\n\r\n"

using namespace std;

//添加一个左右去空格的函数
//用于规范化数据串，因为文本串可能会有左右的空格
inline void TrimString(char* pOld, char* pNew)
{
	int len = ACE_OS::strlen(pOld);
	int i = 0;
	int j = 1;
	while (*(pOld + i) == ' ')
	{
		i++;
	}
	while (*(pOld + len - j) == ' ')
	{
		j++;
	}
	len = len - i - j + 1;

	for (int p = 0; p < len; p++)
	{
		*(pNew + p) = *(pOld + i + p);
	}
	pNew[len] = '\0'; 
}

class CBaseCommand : public CClientCommand
{
public:
  CBaseCommand(void);
  ~CBaseCommand(void);

  int DoMessage(IMessage* pMessage, bool& bDeleteFlag);

  void SetServerObject(CServerObject* pServerObject);

private:
  int DoMessage_HandIn(IMessage* pMessage, bool& bDeleteFlag);
  int DoMessage_DataIn(IMessage* pMessage, bool& bDeleteFlag);

  //发送数据包websocket加密
  bool WebSocketEncrypt(char* pOriData, uint32 u4OriLen, char* pEncryData, uint32& u4EnCryLen);

private:
  CServerObject* m_pServerObject;
  int            m_nCount;
};
