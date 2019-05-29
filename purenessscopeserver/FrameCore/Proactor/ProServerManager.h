#ifndef _PROSERVERMANAGER_H
#define _PROSERVERMANAGER_H

#include "BaseServerManager.h"
#include "ProConnectAccept.h"
#include "ProConsoleAccept.h"
#include "ProTTyClientManager.h"
#include "AceProactorManager.h"
#include "ClientProConnectManager.h"
#include "ProUDPManager.h"
#include "XmlConfig.h"
#include "MessageQueueManager.h"
#include "IpCheck.h"

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
    CProConsoleConnectAcceptor m_ProConsoleConnectAcceptor;      //用于关联管理工具链接
    Frame_Logging_Strategy*    m_pFrameLoggingStrategy;          //输出对象
    CFrameCommand              m_objFrameCommand;                //框架命令
    ts_timer::CTimerThread     m_TSThread;                       //TS定时器
};

typedef ACE_Singleton<CProServerManager, ACE_Null_Mutex> App_ProServerManager;
#endif
