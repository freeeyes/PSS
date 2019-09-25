// Define.h
// 这里定义Server的主要接口（启动监听端口等等）
// 悠然自得，即使什么都不做，也是一种雅兴。
// add by freeeyes
// 2008-12-23


#ifndef _SERVERMANAGER_H
#define _SERVERMANAGER_H

#include "BaseServerManager.h"
#include "ConnectAccept.h"
#include "ConsoleAccept.h"
#include "ClientReConnectManager.h"
#include "ReUDPManager.h"
#include "ReTTyClientManager.h"
#include "MessageQueueManager.h"
#include "ControlListen.h"
#include "FileTestManager.h"
#include "IpCheck.h"
#include "TimerEvent.h"

class Frame_Logging_Strategy;

class CServerManager : public IServerManager
{
public:
    CServerManager(void);

    bool Init();
    bool Start();
    bool Close();


private:
    bool Init_Reactor(uint8 u1ReactorCount, uint8 u1NetMode);
    bool Run();

    bool Start_Tcp_Listen();                             //启动TCP监听
    bool Start_Udp_Listen();                             //启动UDP监听
    bool Start_Console_Tcp_Listen();                     //启动Console TCP监听
    void Multiple_Process_Start();                       //多进程启动
    ConnectConsoleAcceptor  m_ConnectConsoleAcceptor;    //后台管理链接
    Frame_Logging_Strategy* m_pFrameLoggingStrategy;     //输出对象
    CFrameCommand           m_objFrameCommand;           //框架命令
    CTimerManager           m_TSThread;                  //TS定时器
};


typedef ACE_Singleton<CServerManager, ACE_Null_Mutex> App_ServerManager;

#endif
