#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

#include <string>

//����ͻ�������(TCP)
#define COMMAND_HANDIN          0xee01
#define COMMAND_DATAIN          0xee02
#define COMMAND_RETURN_HANDIN   0xff01
#define COMMAND_RETURN_DATAIN   0xff02

//�������������Ϣ
#define CLIENT_KEY_TAG     "Sec-WebSocket-Key:"
#define HANDIN_RETURN_DATA "HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept:%s\r\n\r\n"

using namespace std;

//���һ������ȥ�ո�ĺ���
//���ڹ淶�����ݴ�����Ϊ�ı������ܻ������ҵĿո�
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

  //�������ݰ�websocket����
  bool WebSocketEncrypt(char* pOriData, uint32 u4OriLen, char* pEncryData, uint32& u4EnCryLen);

private:
  CServerObject* m_pServerObject;
  int            m_nCount;
};
