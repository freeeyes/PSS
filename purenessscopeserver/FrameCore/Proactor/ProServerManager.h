#ifndef _PROSERVERMANAGER_H
#define _PROSERVERMANAGER_H

#include "HandlerManager.h"
#include "BaseServerManager.h"
#include "ProConnectAccept.h"
#include "ProConsoleAccept.h"
#include "ProactorUDPHandler.h"
#include "ProTTyClientManager.h"
#include "AceProactorManager.h"
#include "ClientProConnectManager.h"
#include "XmlConfig.h"
#include "FileTestManager.h"
#include "IpCheck.h"

//添加对服务器控制的支持，Console模块用于支持外网对服务器的控制
//add by freeeyes

class CProServerManager : public IServerManager
{
public:
    CProServerManager(void);

    bool Init();
    bool Start();
    bool Close();

private:
    CProConsoleConnectAcceptor m_ProConsoleConnectAcceptor;      //用于关联管理工具链接
    CFrameCommand              m_objFrameCommand;                //框架命令
};

using App_ProServerManager = PSS_singleton<CProServerManager> ;
#endif
