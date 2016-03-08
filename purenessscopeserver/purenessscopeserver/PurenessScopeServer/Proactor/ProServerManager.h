#ifndef _PROSERVERMANAGER_H
#define _PROSERVERMANAGER_H

#include "IServerManager.h"
#include "define.h"
#include "MainConfig.h"
#include "ForbiddenIP.h"
#include "ProConnectAccept.h"
#include "ProConsoleAccept.h"
#include "AceProactorManager.h"
#include "MessageService.h"
#include "LoadModule.h"
#include "LogManager.h"
#include "FileLogger.h"
#include "IObject.h"
#include "ClientProConnectManager.h"
#include "ProUDPManager.h"
#include "ModuleMessageManager.h"

//添加对服务器控制的支持，Console模块用于支持外网对服务器的控制
//add by freeeyes


class Frame_Logging_Strategy;

class CProServerManager : public IServerManager
{
public:
	CProServerManager(void);
	~CProServerManager(void);

	bool Init();
	bool Start();
	bool Close();

private:
	//CProConnectAcceptManager   m_ConnectAcceptorManager;         //用于关联客户端链接
	CProConsoleConnectAcceptor m_ProConsoleConnectAcceptor;      //用于关联管理工具链接
	Frame_Logging_Strategy*    m_pFrameLoggingStrategy;          //输出对象
};


typedef ACE_Singleton<CProServerManager, ACE_Null_Mutex> App_ProServerManager;
#endif
