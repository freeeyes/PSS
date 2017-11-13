// Define.h
// 这里定义Server的主要接口（启动监听端口等等）
// 悠然自得，即使什么都不做，也是一种雅兴。
// add by freeeyes
// 2008-12-23


#ifndef _SERVERMANAGER_H
#define _SERVERMANAGER_H

#include "IServerManager.h"
#include "define.h"
#include "MainConfig.h"
#include "ForbiddenIP.h"
#include "ConnectAccept.h"
#include "ConsoleAccept.h"
#include "MessageService.h"
#include "LoadModule.h"
#include "LogManager.h"
#include "FileLogger.h"
#include "IObject.h"
#include "BuffPacketManager.h"
#include "ClientReConnectManager.h"
#include "ReUDPManager.h"
#include "CommandAccount.h"
#include "ModuleMessageManager.h"
#include "ControlListen.h"
#include "FrameCommand.h"

class Frame_Logging_Strategy;

class CServerManager : public IServerManager
{
public:
    CServerManager(void);
    ~CServerManager(void);

    bool Init();
    bool Start();
    bool Close();


private:
    bool Init_Reactor(uint8 u1ReactorCount, uint8 u1NetMode);
    bool Run();

private:
    ConnectConsoleAcceptor  m_ConnectConsoleAcceptor;    //后台管理链接
    Frame_Logging_Strategy* m_pFrameLoggingStrategy;     //输出对象
    CFrameCommand           m_objFrameCommand;           //框架命令
};


typedef ACE_Singleton<CServerManager, ACE_Null_Mutex> App_ServerManager;

#endif
