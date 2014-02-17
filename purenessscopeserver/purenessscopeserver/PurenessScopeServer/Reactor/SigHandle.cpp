// MessageService.h
// 处理系统中出现的信号量数据
// 做东西要细心，任何可能的异常都不要放过。
// add by freeeyes
// 2009-01-30

#include "SigHandle.h"

CSigHandle::CSigHandle(void)
{
}

CSigHandle::~CSigHandle(void)
{
}

int CSigHandle::RegisterSignal(ACE_Reactor* pReactor)
{
	ACE_Sig_Set signalSet;
	signalSet.empty_set();
	signalSet.sig_add(SIGINT);
	signalSet.sig_add(SIGUSR1);
	signalSet.sig_add(SIGPIPE);
	signalSet.sig_add(SIGALRM);	

	if(NULL == pReactor)
	{
		return ACE_Reactor::instance()->register_handler(signalSet, this);
	}
	else
	{
		OUR_DEBUG((LM_INFO,"[CSigHandle::handle_signal] pReactor=0x%08x.\n", pReactor));
		return pReactor->register_handler(signalSet, this);
	}
}

int CSigHandle::handle_signal(int nSigNum, siginfo_t* pSigInfo, ucontext_t* pContext)
{
	if(pSigInfo != NULL && pContext != NULL)
	{
	}
	
	switch(nSigNum)
	{
	case SIGINT:
		OUR_DEBUG((LM_INFO,"[CSigHandle::handle_signal] CSigHandle::handle_signal --> You pressed SIGINT\n"));
		App_ServerManager::instance()->Close();
		break;
	default:
		OUR_DEBUG((LM_INFO,"[CSigHandle::handle_signal] AceSigHandler::handle_signal --> received signal [%d]\n", nSigNum));
	}   
	return 0;
}
