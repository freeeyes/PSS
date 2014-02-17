#ifndef _SIGHANDLE_H
#define _SIGHANDLE_H

#include "define.h"
#include "ace/Synch.h"
#include "ace/Singleton.h"
#include "ace/Process_Mutex.h"
#include "ace/Signal.h"
#include "ace/Reactor.h"
#include "ServerManager.h"

class CSigHandle : public ACE_Event_Handler
{
public:
	CSigHandle(void);
	~CSigHandle(void);

	virtual int handle_signal(int nSigNum, siginfo_t* pSigInfo = 0, ucontext_t* pContext = 0);
	int RegisterSignal(ACE_Reactor* pReactor = NULL);
};

typedef ACE_Singleton<CSigHandle, ACE_Null_Mutex> App_SigHandler; 
#endif
