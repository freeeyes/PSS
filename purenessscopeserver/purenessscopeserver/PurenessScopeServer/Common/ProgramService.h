#ifndef _PROGRAMSERVICE_H
#define _PROGRAMSERVICE_H

#ifdef WIN32

#include "ace/Time_Value.h"
#include "ace/Event_Handler.h"
#include "ace/NT_Service.h"
#include "ace/Singleton.h"
#include "ace/Mutex.h"

#include "ProServerManager.h"
#include <tchar.h>

static int ServerMain()
{
	//启动主服务器监控
	if(!App_ProServerManager::instance()->Init())
	{
		OUR_DEBUG((LM_INFO, "[main]App_ProServerManager::instance()->Init() error.\n"));
	}

	if(!App_ProServerManager::instance()->Start())
	{
		OUR_DEBUG((LM_INFO, "[main]App_ProServerManager::instance()->Start() error.\n"));
	}

	OUR_DEBUG((LM_INFO, "[main]Server Run is End.\n"));

	return 0;
}

class CProgramService : public  ACE_NT_Service
{
public:
	CProgramService(void);
	~CProgramService(void);

	virtual int svc(void);

	virtual int  handle_exception (ACE_HANDLE h);

	virtual void handle_control (DWORD control_code);

private:
	typedef ACE_NT_Service inherited;

private:
	bool m_blsStop;
};


typedef ACE_Singleton<CProgramService, ACE_Mutex> App_Service;

#endif
#endif 

