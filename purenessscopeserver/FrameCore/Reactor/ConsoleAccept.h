#ifndef _CONSOLEACCEPT_H
#define _CONSOLEACCEPT_H

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ConnectHandler.h"
#include "ConsoleHandler.h"

//用于后台管理接口
class ConnectConsoleAcceptor : public ACE_Acceptor<CConsoleHandler, ACE_SOCK_ACCEPTOR>
{
protected:
    int make_svc_handler(CConsoleHandler*& sh) final;

public:
    int Init_Open(const ACE_INET_Addr& local_addr,
                  int flags = 0,
                  int use_select = 1,
                  int reuse_addr = 1,
                  int backlog = ACE_DEFAULT_BACKLOG);

    int Run_Open(ACE_Reactor* reactor);
};
#endif
