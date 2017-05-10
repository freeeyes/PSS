#include "ConsoleAccept.h"

int ConnectConsoleAcceptor::make_svc_handler(CConsoleHandler*& sh)
{
    //如果正在处理的链接超过了服务器设定的数值，则不允许链接继续链接服务器
    CConsoleHandler* pConsoleHandler = new CConsoleHandler();

    if (NULL != pConsoleHandler)
    {
        pConsoleHandler->reactor(this->reactor());
        sh = pConsoleHandler;
        return 0;
    }
    else
    {
        return -1;
    }
}

int ConnectConsoleAcceptor::Init_Open(const ACE_INET_Addr& local_addr, int flags, int use_select, int reuse_addr, int backlog)
{
    this->flags_ = flags;
    this->use_select_ = use_select;
    this->reuse_addr_ = reuse_addr;
    this->peer_acceptor_addr_ = local_addr;

    // Open the underlying PEER_ACCEPTOR.
    if (this->peer_acceptor_.open(local_addr, 1, 0, backlog) == -1)
    {
        return -1;
    }

    (void) this->peer_acceptor_.enable(ACE_NONBLOCK);

    return 0;
}

int ConnectConsoleAcceptor::Run_Open(ACE_Reactor* reactor)
{
    this->reactor(reactor);
    int result = this->reactor()->register_handler(this,
                 ACE_Event_Handler::ACCEPT_MASK);

    if (result != -1)
    {
        return result;
    }
    else
    {
        this->peer_acceptor_.close();
    }

    return result;
}

