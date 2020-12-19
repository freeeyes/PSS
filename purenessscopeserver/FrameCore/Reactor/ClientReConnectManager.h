#ifndef _CLIENTCONNECTMANAGER_H
#define _CLIENTCONNECTMANAGER_H

#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"

#include "TimerManager.hpp"
#include "BaseClientConnectManager.h"
#include "ReactorUDPClient.h"
#include "HashTable.h"
#include "XmlConfig.h"
#include "ConnectClient.h"
#include <unordered_map>

const uint32 RE_CONNECT_SERVER_TIMEOUT = 100 * 1000;
const uint32 WAIT_FOR_RECONNECT_FINISH = 5000;

using CConnectClientConnector = ACE_Connector<CConnectClient, ACE_SOCK_CONNECTOR>;

class CReactorClientInfo
{
public:
    CReactorClientInfo()  = default;

    bool Init(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, CConnectClientConnector* pReactorConnect, IClientMessage* pClientMessage, ACE_Reactor* pReactor, uint32 u4PacketParseID);  //初始化链接地址和端口
    void SetLocalAddr(const char* pIP, uint16 u2Port, uint8 u1IPType);                         //绑定本地的IP和端口
    bool Run(bool blIsReady, EM_Server_Connect_State emState = EM_Server_Connect_State::SERVER_CONNECT_RECONNECT);  //开始链接
    bool SendData(ACE_Message_Block* pmblk);                                               //发送数据
    int  GetServerID() const;                                                              //得到服务器ID
    bool Close();                                                                          //关闭服务器链接
    void SetConnectClient(CConnectClient* pConnectClient);                                 //设置链接状态
    CConnectClient* GetConnectClient();                                                    //得到ProConnectClient指针
    IClientMessage* GetClientMessage();                                                    //获得当前的消息处理指针
    ACE_INET_Addr GetServerAddr() const;                                                   //获得服务器的地址
    EM_Server_Connect_State GetServerConnectState() const;                                 //得到当前连接状态
    void SetServerConnectState(EM_Server_Connect_State objState);                          //设置当前连接状态
    void SetPacketParseID(uint32 u4PacketParseID);                                         //设置解析器ID
    uint32 GetPacketParseID() const;                                                       //获得解析器ID

private:
    ACE_INET_Addr              m_AddrLocal;                               //本地的连接地址（可以指定）
    ACE_INET_Addr              m_AddrServer;                              //远程服务器的地址
    CConnectClient*            m_pConnectClient  = nullptr;               //当前链接对象
    CConnectClientConnector*   m_pReactorConnect = nullptr;               //Connector链接对象
    IClientMessage*            m_pClientMessage  = nullptr;               //回调函数类，回调返回错误和返回数据方法
    uint32                     m_u4PacketParseID = 0;                     //可设置的PacketParseID
    ACE_Reactor*               m_pReactor        = nullptr;               //记录使用的反应器
    bool                       m_blIsLocal       = false;                 //是否需要制定本地端口
    int                        m_nServerID       = 0;                     //远程服务器的ID
    EM_Server_Connect_State    m_emConnectState  = EM_Server_Connect_State::SERVER_CONNECT_READY;  //连接状态
};

class CClientReConnectManager : public IClientManager
{
public:
    CClientReConnectManager(void) = default;

    bool Init(ACE_Reactor* pReactor);
    bool Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, IClientMessage* pClientMessage, uint32 u4PacketParseID) final;                                                                 //链接服务器(TCP)
    bool Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage, uint32 u4PacketParseID) final;  //连接服务器(TCP)，指定本地地址

    bool ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, uint32 u4PacketParseID) final;                //连接指定的服务器，并给出PacketParseID
    bool ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, uint32 u4PacketParseID) final;    //连接指定的服务器，并给出PacketParseID

    bool ConnectUDP(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, EM_UDP_TYPE emType, IClientUDPMessage* pClientUDPMessage) final;                                //建立一个指向UDP的链接（UDP）
    bool ReConnect(int nServerID);                                                                                                  //重新连接一个指定的服务器(TCP)
    bool CloseByClient(int nServerID);                                                                                              //远程被动关闭(TCP)
    bool Close(int nServerID) final;                                                                                                //关闭连接
    bool CloseUDP(int nServerID) final;                                                                                             //关闭链接（UDP）
    bool ConnectErrorClose(int nServerID);                                                                                          //由内部错误引起的失败，由ProConnectClient调用
    bool SendData(int nServerID, char*& pData, int nSize, bool blIsDelete = true) final;                                            //发送数据
    bool SendDataUDP(int nServerID, const char* pIP, uint16 u2Port, char*& pMessage, uint32 u4Len, bool blIsDelete = true) final;   //发送数据（UDP）
    bool SetHandler(int nServerID, CConnectClient* pConnectClient);                                                                 //将指定的CProConnectClient*绑定给nServerID
    IClientMessage* GetClientMessage(int nServerID) final;                                                                          //获得ClientMessage对象
    bool StartConnectTask(uint16 u2IntervalTime = CONNECT_LIMIT_RETRY) final;                                                           //设置自动重连的定时器
    void CancelConnectTask() final;                                                                                                 //关闭重连定时器
    void Close() final;                                                                                                             //关闭所有连接
    ACE_INET_Addr GetServerAddr(int nServerID);                                                                                     //得到指定服务器的远程地址连接信息
    bool SetServerConnectState(int nServerID, EM_Server_Connect_State objState);                                                    //设置指定连接的连接状态
    bool GetServerIPInfo(int nServerID, _ClientIPInfo& objServerIPInfo) final;                                                      //得到一个nServerID对应的ServerIP信息
    bool DeleteIClientMessage(IClientMessage* pClientMessage) final;                                                                //删除一个生命周期结束的IClientMessage

    void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo);      //返回当前存活链接的信息（TCP）
    void GetUDPConnectInfo(vecClientConnectInfo& VecClientConnectInfo);   //返回当前存活链接的信息（UDP）
    EM_Server_Connect_State GetConnectState(int nServerID) final;         //得到一个当前连接状态
    uint32 GetPacketParseID(int nServerID) final;                         //得到当前的PacketParseID

    int timer_task(brynet::TimerMgr::Ptr timerMgr);                       //定时任务
    void start_new_task(brynet::TimerMgr::Ptr timerMgr);                  //设置定时器


private:
    shared_ptr<CReactorClientInfo> ConnectTcpInit(int nServerID);
    shared_ptr<CReactorUDPClient> ConnectUdpInit(int nServerID);

public:
    using hashmapClientTCPList = unordered_map<int, shared_ptr<CReactorClientInfo>>;
    using hashmapClientUDPList = unordered_map<int, shared_ptr<CReactorUDPClient>>;
    hashmapClientTCPList            m_objClientTCPList;                            //TCP客户端链接
    hashmapClientUDPList            m_objClientUDPList;                            //UDP客户端链接
    CConnectClientConnector         m_ReactorConnect;                              //Reactor连接客户端对象
    ACE_Recursive_Thread_Mutex      m_ThreadWritrLock;                             //线程锁
    ACE_Reactor*                    m_pReactor               = nullptr;            //当前的反应器
    bool                            m_blReactorFinish        = false;              //Reactor是否已经注册
    bool                            m_blTimerState           = true;               //定时器是否运行
    uint16                          m_u2ThreadTimeCheck = CONNECT_LIMIT_RETRY;     //定时器的间隔时间
    uint32                          m_u4ConnectServerTimeout = 0;                  //连接间隔时间
    uint32                          m_u4MaxPoolCount         = 0;                  //连接池的上限
    EM_S2S_Run_State                m_emS2SRunState          = EM_S2S_Run_State::S2S_Run_State_Init; //当前服务连接状态
    vector<CS2SConnectGetRandyInfo> m_GetReadyInfoList;                            //需要等待连接的数据信息
};

using App_ClientReConnectManager = PSS_singleton<CClientReConnectManager>;
#endif
