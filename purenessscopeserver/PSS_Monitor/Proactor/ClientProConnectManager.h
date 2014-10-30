#ifndef _PROCONNECTMANAGER_H
#define _PROCONNECTMANAGER_H

//处理连接远程服务器的管理类
//在这里有一个结构，包含了ConectHandler指针
//add by freeeyes 2010-12-27

#include "ace/INET_Addr.h"
#include "ace/Guard_T.h"

#include "TimerManager.h"
#include "ProAsynchConnect.h"

#define PRO_CONNECT_SERVER_TIMEOUT 100*1000

#include <map>

using namespace std;

class CProactorClientInfo
{
public:
	CProactorClientInfo();
	~CProactorClientInfo();

	bool Init(const char* pIP, int nPort, uint8 u1IPType, int nServerID, CProAsynchConnect* pProAsynchConnect, IClientMessage* pClientMessage);  //初始化链接地址和端口
	void SetLocalAddr(const char* pIP, int nPort, uint8 u1IPType);                             //设置本地IP和端口
	bool Run(bool blIsReadly, EM_Server_Connect_State emState = SERVER_CONNECT_RECONNECT);     //开始链接
	bool SendData(ACE_Message_Block* pmblk);                                                   //发送数据
	bool ConnectError(int nError);                                                             //链接错误，报错
	int  GetServerID();                                                                        //得到服务器ID
	bool Close();                                                                              //关闭服务器链接
	void SetProConnectClient(CProConnectClient* pProConnectClient);                            //设置ProConnectClient指针
	CProConnectClient* GetProConnectClient();                                                  //得到ProConnectClient指针
	IClientMessage* GetClientMessage();                                                        //获得当前的消息处理指针
	ACE_INET_Addr GetServerAddr();                                                             //获得服务器的地址
	EM_Server_Connect_State GetServerConnectState();                                           //得到当前连接状态
	void SetServerConnectState(EM_Server_Connect_State objState);                              //设置当前连接状态

private:
	ACE_INET_Addr             m_AddrLocal;                //本地的连接地址（可以指定）
	ACE_INET_Addr             m_AddrServer;               //远程服务器的地址
	CProConnectClient*        m_pProConnectClient;        //当前链接对象
	CProAsynchConnect*        m_pProAsynchConnect;        //异步链接对象
	IClientMessage*           m_pClientMessage;           //回调函数类，回调返回错误和返回数据方法
	char                      m_szServerIP[MAX_BUFF_20];  //远端服务器地址
	int                       m_nPort;                    //远端服务器端口
	int                       m_nServerID;                //服务器ID，由用户起名，用于区别连接
	EM_Server_Connect_State   m_emConnectState;           //连接状态
};

//管理所有连接到其他服务器的管理类
class CClientProConnectManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CClientProConnectManager(void);
	~CClientProConnectManager(void);

	bool Init(ACE_Proactor* pProactor);                                                                                        //初始化链接器
	bool Connect(int nServerID, const char* pIP, int nPort, uint8 u1IPType, IClientMessage* pClientMessage);                                                             //链接指定的服务器（TCP）
	bool Connect(int nServerID, const char* pIP, int nPort, uint8 u1IPType, const char* pLocalIP, int nLocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage);  //连接服务器(TCP)，指定本地地址                                                   //建立一个指向UDP的链接（UDP）
	bool ReConnect(int nServerID);                                                                                             //重新连接一个指定的服务器(TCP)  
	bool CloseByClient(int nServerID);                                                                                         //远程被动关闭(TCP)
	bool Close(int nServerID);                                                                                                 //关闭连接（TCP）
	bool ConnectErrorClose(int nServerID);                                                                                     //由内部错误引起的失败，由ProConnectClient调用
	bool SendData(int nServerID, const char* pData, int nSize, bool blIsDelete = true);                                        //发送数据（TCP）
	bool SetHandler(int nServerID, CProConnectClient* pProConnectClient);                                                      //将指定的CProConnectClient*绑定给nServerID
	IClientMessage* GetClientMessage(int nServerID);                                                                           //获得ClientMessage对象
	bool StartConnectTask(int nIntervalTime = CONNECT_LIMIT_RETRY);                                                            //设置自动重连的定时器
	void CancelConnectTask();                                                                                                  //关闭重连定时器
	void Close();                                                                                                              //关闭所有连接 
	bool GetConnectState(int nServerID);                                                                                       //得到指定连接的当前状态
	ACE_INET_Addr GetServerAddr(int nServerID);                                                                                //得到指定服务器的远程地址连接信息
	bool SetServerConnectState(int nServerID, EM_Server_Connect_State objState);                                               //设置指定连接的连接状态

	void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo);      //返回当前存活链接的信息（TCP）

	virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);                       //定时检测

private:
	typedef map<int, CProactorClientInfo*> mapProactorClientInfo;              //管理所有已经存在的客户端链接信息（TCP）

private:
	CProAsynchConnect           m_ProAsynchConnect;
	mapProactorClientInfo       m_mapClientInfo;               //TCP客户端链接
	ACE_Recursive_Thread_Mutex  m_ThreadWritrLock;             //线程锁
	ActiveTimer                 m_ActiveTimer;                 //时间管理器
	int                         m_nTaskID;                     //定时检测工具
	bool                        m_blProactorFinish;            //Proactor是否已经注册 
	uint32                      m_u4ConnectServerTimeout;      //连接间隔时间
};

typedef ACE_Singleton<CClientProConnectManager, ACE_Recursive_Thread_Mutex> App_ClientProConnectManager;

#endif 
