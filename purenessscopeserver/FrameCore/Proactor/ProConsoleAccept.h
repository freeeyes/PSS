#ifndef _PROCONSOLEACCEPT_H
#define _PROCONSOLEACCEPT_H

#include <vector>

using namespace std;

#include "ForbiddenIP.h"
#include "ProConsoleHandle.h"

//用于后台管理的Acceptor
class CProConsoleConnectAcceptor: public ACE_Asynch_Acceptor<CProConsoleHandle>
{
private:
    virtual CProConsoleHandle* make_handler (void);
    virtual int validate_connection (const ACE_Asynch_Accept::Result& result,
                                     const ACE_INET_Addr& remote,
                                     const ACE_INET_Addr& local);
};

#endif
