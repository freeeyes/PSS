#ifdef WIN32
#include "ProgramService.h"

CProgramService::CProgramService(void)
{
	m_blsStop = false;
}

CProgramService::~CProgramService(void)
{
}

int CProgramService::svc( void )
{
	m_blsStop = true;
	//ACE_Time_Value tvSleep(2, 0);

	report_status (SERVICE_RUNNING);

	ServerMain();

	return 0;
}

void CProgramService::handle_control(DWORD control_code)
{
	if (control_code == SERVICE_CONTROL_SHUTDOWN
		|| control_code == SERVICE_CONTROL_STOP)
	{
		report_status(SERVICE_STOP_PENDING);
		App_ProServerManager::instance()->Close();
	}
	else
	{
		inherited::handle_control(control_code);
	}
}

int CProgramService::handle_exception( ACE_HANDLE h )
{
	return 0;
}
#endif


