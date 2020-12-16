#ifndef _WINDOWSPROCESS_H
#define _WINDOWSPROCESS_H

#include "define.h"

#if PSS_PLATFORM == PLATFORM_WIN

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "ace/OS_NS_errno.h"

#include "ProgramService.h"

#define DEFAULT_SERVICE_INIT_STARTUP     SERVICE_AUTO_START

class Process
{
public:
    Process (void);
    ~Process (void);

    void startprocesslog();
    int run(int argc, ACE_TCHAR* argv[]);
    void stopprocesslog();
private:
    bool parse_args (int argc, ACE_TCHAR* argv[]);
    void print_usage_and_die (void);

private:
    char m_szProgName[128];

    int m_nOptinstall;
    int m_nOptremove;
    int m_nOptstart;
    int m_nOptkill;
    int m_nOpttype;
    int m_nOptdebug;
    int m_nOptstartup;
};

using App_Process = PSS_singleton<Process>;
#endif

#endif
