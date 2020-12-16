#ifndef _PROGRAMSERVICE_H
#define _PROGRAMSERVICE_H

#include "define.h"

#if PSS_PLATFORM == PLATFORM_WIN

#include "ace/Time_Value.h"
#include "ace/Event_Handler.h"
#include "ace/NT_Service.h"
#include "ace/Singleton.h"
#include "ace/Mutex.h"

#include "ProServerManager.h"
#include <tchar.h>

static int32 ServerMain()
{
    //���������������
    if(!App_ProServerManager::instance()->Init())
    {
        OUR_DEBUG((LM_INFO, "[main]App_ProServerManager::instance()->Init() error.\n"));
        App_ProServerManager::instance()->Close();
        return 0;
    }

    if(!App_ProServerManager::instance()->Start())
    {
        OUR_DEBUG((LM_INFO, "[main]App_ProServerManager::instance()->Start() error.\n"));
        App_ProServerManager::instance()->Close();
        return 0;
    }

    if(GetXmlConfigAttribute(xmlServerType)->Type == 0)
    {
        //�ȴ��������
        ACE_Thread_Manager::instance()->wait();
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

    virtual int handle_timeout (const ACE_Time_Value& tv,const void* arg = 0);

private:
    typedef ACE_NT_Service inherited;

private:
    bool m_blsStop;
};


using App_Service = PSS_singleton<CProgramService>;

#endif
#endif

