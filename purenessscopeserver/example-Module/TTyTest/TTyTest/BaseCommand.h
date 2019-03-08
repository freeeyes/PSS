#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "ITTyMessage.h"
#include "ITTyClientManager.h"
#include <string>

//����ͻ�������(TCP)
#define COMMAND_BASE            0x1001
#define COMMAND_RETURN_ALIVE    0xf000


// ���ڱ��ID
#define TTY_CONNECT_ID_COM2 0x0002 // COM1 
#define TTY_CONNECT_ID_COM3 0x0003 // COM2 

using namespace std;

class CRecvTTyMessage : ITTyMessage
{
public:
	CRecvTTyMessage();
	~CRecvTTyMessage();
	virtual bool RecvData(uint16 u2ConnectID, const char* pData, uint32 u4len);
	virtual bool ReportMessage(uint16 u2ConnectID, uint32 u4ErrorNo, EM_TTY_EVENT_TYPE em_type) ;

	void SetServerObject(CServerObject* pServerObject);
private:
	CServerObject* m_pServerObject;
};


class CBaseCommand : public CClientCommand
{
public:
	CBaseCommand(void);
	~CBaseCommand(void);

	int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
	void SetServerObject(CServerObject* pServerObject);

	int OpenTTy();
	void CloseAll();

	bool Pause(uint16 u2ConnectId);
	bool Resume(uint16 u2ConnectId);


private:
	CServerObject* m_pServerObject;
	CRecvTTyMessage* m_pRecvTTyMsg;
};
