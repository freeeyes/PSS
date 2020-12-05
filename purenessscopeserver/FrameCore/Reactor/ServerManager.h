// Define.h
// ���ﶨ��Server����Ҫ�ӿڣ����������˿ڵȵȣ�
// ��Ȼ�Եã���ʹʲô��������Ҳ��һ�����ˡ�
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
    CServerManager(void) = default;

    bool Init();
    bool Start();
    bool Close() final;

private:
    bool Init_Reactor(uint8 u1ReactorCount, NETWORKMODE u1NetMode) const;
    bool Run();

    bool Start_Tcp_Listen() const;                                //����TCP����
    bool Start_Udp_Listen();                                      //����UDP����
    bool Start_Console_Tcp_Listen();                              //����Console TCP����
    void Multiple_Process_Start();                                //���������
    void Run_Child_Process_Start(int nNumChlid, const int& fd_lock);    //�����ӽ���

    ConnectConsoleAcceptor  m_ConnectConsoleAcceptor;             //��̨��������
    CFrameCommand           m_objFrameCommand;                    //�������
    CTMService              m_TMService;                          //TS��ʱ��
    vector<shared_ptr<CReactorUDPHander>> m_vecUDPList;           //UDP���������б�
};


using App_ServerManager = ACE_Singleton<CServerManager, ACE_Null_Mutex>;

#endif
