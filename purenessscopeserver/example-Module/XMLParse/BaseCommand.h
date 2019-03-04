#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "XmlOpeation.h"

#include <string>

//定义客户端信令(TCP)
#define COMMAND_BASE            0x1000
#define COMMAND_RETURN_ALIVE    0xf000

#define MESSAGE_FUNCTION_BEGIN(x) switch(x) { 
#define MESSAGE_FUNCTION(x,y,z) case x: { y(z); break; }
#define MESSAGE_FUNCTION_END }

using namespace std;

class CBaseCommand : public CClientCommand
{
public:
	CBaseCommand(void);
	~CBaseCommand(void);

	void Init_XML();

	int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
	void SetServerObject(CServerObject* pServerObject);

private:
	int Do_Connect(IMessage* pMessage);
	int Do_DisConnect(IMessage* pMessage);
	int Do_ClientSendTimeout(IMessage* pMessage);
	int Do_Base(IMessage* pMessage);
private:
	CServerObject* m_pServerObject;
	int            m_nCount;
};
