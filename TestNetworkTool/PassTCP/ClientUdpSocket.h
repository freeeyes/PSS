#pragma once

#include "define.h"
#include <time.h>
#include "LuaFn.h"

class CClientUdpSocket
{
public:
	CClientUdpSocket(void);
	~CClientUdpSocket(void);

	void Close();

	void Run();
	void Stop();

	void SetSocketThread(_Socket_Info* pSocket_Info, _Socket_State_Info* pSocket_State_Info);
	_Socket_State_Info* GetStateInfo();

	void SetThreadID(int nThreadID);

private:
	bool WriteFile_SendBuff(const char* pData, int nLen);
	bool WriteFile_RecvBuff(const char* pData, int nLen);
	bool WriteFile_Error(const char* pError, int nErrorNumber);

private:
	_Socket_Info*       m_pSocket_Info;
	_Socket_State_Info* m_pSocket_State_Info;
	bool                m_blRun;
	CLuaFn              m_objLuaFn;
	int                 m_nThreadID;     //Ïß³ÌID 
};
