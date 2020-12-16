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

class CServerManager : public IServerManager
{
public:
    CServerManager(void) = default;

    bool Init();
    bool Start();
    bool Close() final;

private:
    bool Init_Reactor(uint8 u1ReactorCount, NETWORKMODE u1NetMode) const;
    bool Run();

    bool Start_Tcp_Listen() const;                                //启动TCP监听
    bool Start_Udp_Listen();                                      //启动UDP监听
    bool Start_Console_Tcp_Listen();                              //启动Console TCP监听
    void Multiple_Process_Start();                                //多进程启动
    void Run_Child_Process_Start(int nNumChlid, const int& fd_lock);    //运行子进程

    ConnectConsoleAcceptor  m_ConnectConsoleAcceptor;             //后台管理链接
    CFrameCommand           m_objFrameCommand;                    //框架命令
    vector<shared_ptr<CReactorUDPHander>> m_vecUDPList;           //UDP监听对象列表
};


using App_ServerManager = PSS_singleton<CServerManager>;

#endif
