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
#include "TMService.h"

//��ӶԷ��������Ƶ�֧�֣�Consoleģ������֧�������Է������Ŀ���
//add by freeeyes

class CProServerManager : public IServerManager
{
public:
    CProServerManager(void);

    bool Init();
    bool Start();
    bool Close();

private:
    CProConsoleConnectAcceptor m_ProConsoleConnectAcceptor;      //���ڹ�������������
    CFrameCommand              m_objFrameCommand;                //�������
    CTMService                 m_TMService;                      //TS��ʱ���̴߳���
};

typedef ACE_Singleton<CProServerManager, ACE_Null_Mutex> App_ProServerManager;
#endif
