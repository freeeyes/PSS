#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

#include <string>

//����ͻ�������(TCP)
#define COMMAND_BASE            0x1001
#define COMMAND_RETURN_ALIVE    0xf000

using namespace std;

class CBaseCommand : public CClientCommand
{
public:
	CBaseCommand(void);
	~CBaseCommand(void);

	int DoMessage(IMessage* pMessage, bool& bDeleteFlag);

	void SetServerObject(CServerObject* pServerObject);

private:
	CServerObject* m_pServerObject;
	int            m_nCount;
};
