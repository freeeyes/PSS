#ifndef _PROSIGHANDLE_H
#define _PROSIGHANDLE_H

#include "define.h"
#include "ace/Synch.h"
#include "ace/Singleton.h"
#include "ace/Process_Mutex.h"
#include "ace/Signal.h"
#include "ace/Proactor.h"

class CProSigHandle : public ACE_Event_Handler
{
public:
	CProSigHandle(void);
	~CProSigHandle(void);

	virtual int handle_signal(int nSigNum, siginfo_t* pSigInfo = 0, ucontext_t* pContext = 0);
	int RegisterSignal(ACE_Proactor* pProactor = NULL);
};

typedef ACE_Singleton<CProSigHandle, ACE_Null_Mutex> App_ProSigHandler; 
#endif
