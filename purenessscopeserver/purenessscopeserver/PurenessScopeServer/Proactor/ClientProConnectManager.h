#ifndef _PROCONNECTMANAGER_H
#define _PROCONNECTMANAGER_H

//处理连接远程服务器的管理类
//在这里有一个结构，包含了ConectHandler指针
//add by freeeyes 2010-12-27

#include "ace/INET_Addr.h"
#include "ace/Guard_T.h"

#include "TimerManager.h"
#include "ProAsynchConnect.h"
#include "IClientManager.h"
#include "ProactorUDPClient.h"
#include "HashTable.h"

#define PRO_CONNECT_SERVER_TIMEOUT 100*1000

#define WAIT_FOR_PROCONNECT_FINISH 5000

class CProactorClientInfo
{
public:
    CProactorClientInfo();
    ~CProactorClientInfo();

    bool Init(const char* pIP, int nPort, uint8 u1IPType, int nServerID, CProAsynchConnect* pProAsynchConnect, IClientMessage* pClientMessage);  //初始化链接地址和端口
    void SetLocalAddr(const char* pIP, int nPort, uint8 u1IPType);                             //设置本地IP和端口
    bool Run(bool blIsReadly, EM_Server_Connect_State emState = SERVER_CONNECT_RECONNECT);     //开始链接
    bool SendData(ACE_Message_Block* pmblk);                                                   //发送数据
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
class CClientProConnectManager : public ACE_Task<ACE_MT_SYNCH>, public IClientManager
{
public:
    CClientProConnectManager(void);
    ~CClientProConnectManager(void);

    bool Init(ACE_Proactor* pProactor);                                                                                        //初始化链接器
    bool Connect(int nServerID, const char* pIP, int nPort, uint8 u1IPType, IClientMessage* pClientMessage);                   //链接指定的服务器（TCP）
    bool Connect(int nServerID, const char* pIP, int nPort, uint8 u1IPType, const char* pLocalIP, int nLocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage);  //连接服务器(TCP)，指定本地地址
    bool ConnectUDP(int nServerID, const char* pIP, int nPort, uint8 u1IPType, EM_UDP_TYPE emType, IClientUDPMessage* pClientUDPMessage);                                //建立一个指向UDP的链接（UDP）
    bool ReConnect(int nServerID);                                                                                             //重新连接一个指定的服务器(TCP)
    bool CloseByClient(int nServerID);                                                                                         //远程被动关闭(TCP)
    bool Close(int nServerID);                                                                                                 //关闭连接（TCP）
    bool CloseUDP(int nServerID);                                                                                              //关闭链接（UDP）
    bool ConnectErrorClose(int nServerID);                                                                                     //由内部错误引起的失败，由ProConnectClient调用
    bool SendData(int nServerID, char*& pData, int nSize, bool blIsDelete = true);                                             //发送数据（TCP）
    bool SendDataUDP(int nServerID,const char* pIP, int nPort, const char*& pMessage, uint32 u4Len, bool blIsDelete = true);   //发送数据（UDP）
    bool SetHandler(int nServerID, CProConnectClient* pProConnectClient);                                                      //将指定的CProConnectClient*绑定给nServerID
    IClientMessage* GetClientMessage(int nServerID);                                                                           //获得ClientMessage对象
    bool StartConnectTask(int nIntervalTime = CONNECT_LIMIT_RETRY);                                                            //设置自动重连的定时器
    void CancelConnectTask();                                                                                                  //关闭重连定时器
    void Close();                                                                                                              //关闭所有连接
    ACE_INET_Addr GetServerAddr(int nServerID);                                                                                //得到指定服务器的远程地址连接信息
    bool SetServerConnectState(int nServerID, EM_Server_Connect_State objState);                                               //设置指定连接的连接状态
    bool DeleteIClientMessage(IClientMessage* pClientMessage);                                                                 //删除一个生命周期结束的IClientMessage

    void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo);      //返回当前存活链接的信息（TCP）
    void GetUDPConnectInfo(vecClientConnectInfo& VecClientConnectInfo);   //返回当前存活链接的信息（UDP）
    EM_Server_Connect_State GetConnectState(int nServerID);               //得到一个当前连接状态
    bool GetServerIPInfo(int nServerID, _ClientIPInfo& objServerIPInfo);  //得到一个nServerID对应的ServerIP信息

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);                       //定时检测

private:
    bool ConnectTcpInit(int nServerID, const char* pIP, int nPort, uint8 u1IPType, const char* pLocalIP, int nLocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage, CProactorClientInfo*& pClientInfo);
    bool ConnectUdpInit(int nServerID, CProactorUDPClient*& pProactorUDPClient);

private:
    CProAsynchConnect               m_ProAsynchConnect;
    CHashTable<CProactorClientInfo> m_objClientTCPList;            //TCP客户端链接
    CHashTable<CProactorUDPClient>  m_objClientUDPList;            //UDP客户端链接
    ACE_Recursive_Thread_Mutex      m_ThreadWritrLock;             //线程锁
    ActiveTimer                     m_ActiveTimer;                 //时间管理器
    int                             m_nTaskID;                     //定时检测工具
    bool                            m_blProactorFinish;            //Proactor是否已经注册
    uint32                          m_u4ConnectServerTimeout;      //连接间隔时间
    uint32                          m_u4MaxPoolCount;              //连接池的上限
};

typedef ACE_Singleton<CClientProConnectManager, ACE_Recursive_Thread_Mutex> App_ClientProConnectManager;

#endif
