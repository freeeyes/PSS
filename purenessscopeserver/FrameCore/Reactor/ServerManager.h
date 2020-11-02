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
#include "ReactorUDPHander.h"
#include "ReTTyClientManager.h"
#include "ControlListen.h"
#include "FileTestManager.h"
#include "IpCheck.h"
#include "TMService.h"

class CServerManager : public IServerManager
{
public:
    CServerManager(void);

    bool Init();
    bool Start();
    bool Close();


private:
    bool Init_Reactor(uint8 u1ReactorCount, NETWORKMODE u1NetMode) const;
    bool Run();

    bool Start_Tcp_Listen() const;                                //启动TCP监听
    bool Start_Udp_Listen() const;                                //启动UDP监听
    bool Start_Console_Tcp_Listen();                              //启动Console TCP监听
    void Multiple_Process_Start();                                //多进程启动
    void Run_Child_Process_Start(int nNumChlid, int& fd_lock);    //运行子进程
    ConnectConsoleAcceptor  m_ConnectConsoleAcceptor;             //后台管理链接
    CFrameCommand           m_objFrameCommand;                    //框架命令
    CTMService              m_TMService;                          //TS定时器
};


typedef ACE_Singleton<CServerManager, ACE_Null_Mutex> App_ServerManager;

#endif
