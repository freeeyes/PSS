#include "ProSigHandle.h"

CProSigHandle::CProSigHandle(void)
{
}

CProSigHandle::~CProSigHandle(void)
{
}

int CProSigHandle::RegisterSignal(ACE_Proactor* pProactor)
{
    //��ǰ�汾���¼���Ӧֻ���windows��IOCP���������unix
    ACE_Sig_Set signalSet;
    signalSet.empty_set();
    signalSet.sig_add(SIGINT);
    signalSet.sig_add(SIGUSR1);
    signalSet.sig_add(SIGPIPE);
    signalSet.sig_add(SIGALRM);

    if(nullptr == pProactor)
    {
        //return ACE_Proactor::instance()->register_handle(signalSet,this);
    }
    else
    {
        //return pProactor->register_handler(signalSet,this);
    }

    return 0;
}

int CProSigHandle::handle_signal(int nSigNum, siginfo_t* pSigInfo, ucontext_t* pContext)
{
    return 0;
}

