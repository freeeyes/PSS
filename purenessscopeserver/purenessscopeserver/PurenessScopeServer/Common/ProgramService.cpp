#ifdef WIN32
#include "ProgramService.h"

CProgramService::CProgramService(void)
{
    reactor (ACE_Reactor::instance ());
    m_blsStop = false;
}

CProgramService::~CProgramService(void)
{
    if (ACE_Reactor::instance ()->cancel_timer(this) == -1)
    {
        OUR_DEBUG ((LM_ERROR,"Service::~Service failed to cancel_timer.\n"));
    }
}

int CProgramService::svc( void )
{
    OUR_DEBUG ((LM_DEBUG,ACE_TEXT ("Service::svc\n")));

    if (report_status (SERVICE_RUNNING) == 0)
    {
        reactor ()->owner (ACE_Thread::self ());
    }

    this->m_blsStop = false;

    ACE_Time_Value tv (2, 0);
    ACE_Reactor::instance ()->schedule_timer (this, 0, tv, tv);

    ServerMain();

    while (!this->m_blsStop)
    {
        reactor ()->handle_events ();
    }

    // Cleanly terminate connections, terminate threads.
    OUR_DEBUG ((LM_DEBUG,ACE_TEXT ("Shutting down Service::svc\n")));
    reactor ()->cancel_timer (this);
    return 0;
}

void CProgramService::handle_control(DWORD control_code)
{
    if (control_code == SERVICE_CONTROL_SHUTDOWN || control_code == SERVICE_CONTROL_STOP)
    {
        report_status (SERVICE_STOP_PENDING);

        OUR_DEBUG ((LM_INFO,ACE_TEXT ("Service control stop requested\n")));

        m_blsStop = true;
        App_ProServerManager::instance()->Close();
        reactor ()->notify (this,ACE_Event_Handler::EXCEPT_MASK);
    }
    else
    {
        inherited::handle_control (control_code);
    }
}

int CProgramService::handle_exception( ACE_HANDLE h )
{
    return 0;
}

int CProgramService::handle_timeout (const ACE_Time_Value& tv,const void*)
{
    ACE_UNUSED_ARG (tv);
    MessageBeep (MB_OK);
    //OUR_DEBUG ((LM_DEBUG, ACE_TEXT ("%T (%t): Beep...\n")));
    return 0;
}

#endif


