
// ConnectHandle.h
// 处理客户端链接
// 很多时间，做出来就是做出来了，做不出来，就是做不出来了。没有什么借口好讲。
// 今天是2009年的大年初一，我再继续着我在这的思维，我想把在蓝翼身上的东西，在这里完整的继承下来。
// 对于我，这可能有些难，但是我并不在乎。因为我知道，梦想的道路从不平坦，不怕重新开始，因为我能感到蓝翼对我的期望。
// 添加对链接流量，数据包数的管控。
// add by freeeyes
// 2008-12-22

#ifndef _CONNECTHANDLE_H
#define _CONNECTHANDLE_H

#include "define.h"

#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "AceReactorManager.h"
#include "MessageService.h"
#include "IConnectManager.h"
#include "MakePacket.h"
#include "MessageBlockManager.h"
#include "PacketParsePool.h"
#include "BuffPacketManager.h"
#include "ForbiddenIP.h"
#include "IPAccount.h"
#include "TimerManager.h"
#include "SendMessage.h"
#include "CommandAccount.h"

#ifdef __LINUX__
#include "netinet/tcp.h"
#endif

#include <map>

using namespace std;

class CConnectHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:
    CConnectHandler(void);
    ~CConnectHandler(void);

    //重写继承方法
    virtual int open(void*);                                                 //用户建立一个链接
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);            //接受客户端收到的数据块
    virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);           //链接关闭事件

    void Init(uint16 u2HandlerID);                                           //Connect Pool初始化调用时候调用的方法

    bool CheckSendMask(uint32 u4PacketLen);                                  //检测指定的连接发送数据是否超过阻塞阀值
    bool SendMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, bool blState, uint8 u1SendType, uint32& u4PacketSize, bool blDelete);  //发送当前数据

    bool SetRecvQueueTimeCost(uint32 u4TimeCost);                            //记录当前接收数据到模块处理完成的具体时间消耗
    bool SetSendQueueTimeCost(uint32 u4TimeCost);                            //记录当前从发送队列到数据发送完成的具体时间消耗

    bool Close(int nIOCount = 1);                                            //关闭当前连接
    bool ServerClose();                                                      //服务器关闭客户端链接方法

    const char* GetError();                                                  //得到当前错误信息
    void        SetConnectID(uint32 u4ConnectID);                            //设置当前链接ID
    uint32      GetConnectID();                                              //得到当前链接ID
    uint8       GetConnectState();                                           //得到链接状态
    uint8       GetSendBuffState();                                          //得到发送状态
    bool        CheckAlive();
    _ClientConnectInfo GetClientInfo();                                      //得到客户端信息
    _ClientIPInfo      GetClientIPInfo();                                    //得到客户端IP信息
    void SetConnectName(const char* pName);                                  //设置当前连接名称
    char* GetConnectName();                                                  //得到别名
    void SetIsLog(bool blIsLog);                                             //设置当前连接数据是否写入日志 
	bool GetIsLog();                                                         //获得当前连接是否可以写入日志 

private:
    bool CheckMessage();                                                     //处理接收的数据
    bool PutSendPacket(ACE_Message_Block* pMbData);                          //发送数据
    void ClearPacketParse();                                                 //清理正在使用的PacketParse
    
    int RecvData();                                                          //接收数据，正常模式
    int RecvData_et();                                                       //接收数据，et模式 

private:
    char                       m_szError[MAX_BUFF_500];        //错误信息描述文字
    ACE_INET_Addr              m_addrRemote;                   //远程链接客户端地址
    ACE_Time_Value             m_atvConnect;                   //当前链接建立时间
    ACE_Time_Value             m_atvInput;                     //最后一次接收数据时间
    ACE_Time_Value             m_atvOutput;                    //最后一次发送数据时间
    ACE_Time_Value             m_atvSendAlive;                 //链接存活时间

    uint8                      m_u1ConnectState;               //目前链接处理状态
    uint8                      m_u1SendBuffState;              //目前缓冲器是否有等待发送的数据
    uint16                     m_u2SendQueueMax;               //发送队列最大长度
    uint16                     m_u2SendCount;                  //当前数据包的个数
    uint32                     m_u4HandlerID;                  //此Hander生成时的ID
    uint32                     m_u4ConnectID;                  //链接的ID
    uint32                     m_u4AllRecvCount;               //当前链接接收数据包的个数
    uint32                     m_u4AllSendCount;               //当前链接发送数据包的个数
    uint32                     m_u4AllRecvSize;                //当前链接接收字节总数
    uint32                     m_u4AllSendSize;                //当前链接发送字节总数
    uint16                     m_u2MaxConnectTime;             //最大时间链接判定
    uint32                     m_u4MaxPacketSize;              //单个数据包的最大长度
    uint64                     m_u8RecvQueueTimeCost;          //成功接收数据到数据处理完成（未发送）花费的时间总和
    uint32                     m_u4RecvQueueCount;             //当前链接被处理的数据包数
    uint64                     m_u8SendQueueTimeCost;          //成功发送数据到数据处理完成（只发送）花费的时间总和
    uint64                     m_u8SendQueueTimeout;           //发送超时时间，超过这个时间的都会被记录到日志中
    uint64                     m_u8RecvQueueTimeout;           //接受超时时间，超过这个时间的都会被记录到日志中

    ACE_Recursive_Thread_Mutex m_ThreadLock;

    uint32                     m_u4SendThresHold;              //发送阀值(消息包的个数)
    uint32                     m_u4SendCheckTime;              //发送检测时间的阀值
    int                        m_nIOCount;                     //当前IO操作的个数
    bool                       m_blBlockState;                 //是否处于阻塞状态 false为不在阻塞状态，true为在阻塞状态
    int                        m_nBlockCount;                  //发生阻塞的次数
    int                        m_nBlockMaxCount;               //阻塞允许发生的最大次数
    int                        m_nBlockSize;                   //阻塞发生时缓冲区的大小
    uint32                     m_u4BlockTimerID;               //阻塞定时器的ID

    uint32                     m_u4ReadSendSize;               //准备发送的字节数（水位标）
    uint32                     m_u4SuccessSendSize;            //实际客户端接收到的总字节数（水位标）
    uint16                     m_u2TcpNodelay;                 //Nagle算法开关

    _TimerCheckID*             m_pTCClose;                     //定时检测链接存活状态的定时器

    CBuffPacket                m_AlivePacket;                  //服务器生存包
    CPacketParse*              m_pPacketParse;                 //数据包解析类
    ACE_Message_Block*         m_pCurrMessage;                 //当前的MB对象
    ACE_Message_Block*         m_pBlockMessage;                //当前发送缓冲等待数据块
    uint32                     m_u4CurrSize;                   //当前MB缓冲字符长度
    CPacketParse               m_objSendPacketParse;           //发送数据包组织结构
    _TimeConnectInfo           m_TimeConnectInfo;              //链接健康检测器
    char                       m_szConnectName[MAX_BUFF_100];  //连接名称，可以开放给逻辑插件去设置
    bool                       m_blIsLog;                      //是否写入日志，false为不写入，true为写入
};

//管理所有已经建立的链接
class CConnectManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CConnectManager(void);
    ~CConnectManager(void);

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);   //定时器检查

    virtual int open(void* args = 0);
    virtual int svc(void);
    virtual int close(u_long);

    void CloseAll();
    bool AddConnect(uint32 u4ConnectID, CConnectHandler* pConnectHandler);
    bool SendMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket,  uint16 u2CommandID, bool blSendState, uint8 u1SendType, ACE_Time_Value& tvSendBegin, bool blDelete = true);    //同步发送                                                                     //发送缓冲数据
    bool PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDelete = true); //异步发送
    bool PostMessageAll(IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDelete = true);                  //异步群发
    bool Close(uint32 u4ConnectID);                                                                          //客户单关闭
    bool CloseConnect(uint32 u4ConnectID);                                                                   //服务器关闭
    void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo);                                         //返回当前存活链接的信息
    void SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost);                                        //记录指定链接数据处理时间
	void GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo);                         //得到指定别名的所有设置信息

    _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                                                       //得到指定链接信息

    bool StartTimer();                                                                                       //开启定时器
    bool KillTimer();                                                                                        //关闭定时器

    int         GetCount();
    const char* GetError();
    
    bool SetConnectName(uint32 u4ConnectID, const char* pName);                                  //设置当前连接名称
    bool SetIsLog(uint32 u4ConnectID, bool blIsLog);                                             //设置当前连接数据是否写入日志     

private:
    bool IsRun();
    void CloseQueue();

private:
    typedef map<uint32, CConnectHandler*> mapConnectManager;
    typedef vector<uint32> vecConnectManager;
    mapConnectManager           m_mapConnectManager;     //当前已经存在并且激活的链接map
    char                        m_szError[MAX_BUFF_500]; //错误信息描述
    uint32                      m_u4TimeCheckID;         //定时器检查的TimerID
    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;       //用于循环监控和断开链接时候的数据锁
    _TimerCheckID*              m_pTCTimeSendCheck;      //定时器的参数结构体，用于一个定时器执行不同的事件
    ACE_Time_Value              m_tvCheckConnect;        //定时器下一次检测链接时间
    bool                        m_blRun;                 //线程是否在运行
    CSendMessagePool            m_SendMessagePool;       //发送消息体
    uint32                      m_u4SendQueuePutTime;    //发送队列入队超时时间
	uint32                      m_u4TimeConnect;         //单位时间连接建立数
	uint32                      m_u4TimeDisConnect;      //单位时间连接断开数
};

//链接ConnectHandler内存池
class CConnectHandlerPool
{
public:
    CConnectHandlerPool(void);
    ~CConnectHandlerPool(void);

    void Init(int nObjcetCount);
    void Close();

    CConnectHandler* Create();
    bool Delete(CConnectHandler* pObject);

    int GetUsedCount();
    int GetFreeCount();

private:
    typedef map<CConnectHandler*, CConnectHandler*> mapHandle;
    mapHandle                   m_mapMessageUsed;                      //已使用的
    mapHandle                   m_mapMessageFree;                      //没有使用的
    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //控制多线程锁
    uint32                      m_u4CurrMaxCount;
};

//经过思考，想把发送对象分在几个线程内去做，提高性能。在这里尝试一下。(多线程模式，一个线程一个队列，这样保持并发能力)
class CConnectManagerGroup : public IConnectManager
{
public:
    CConnectManagerGroup();
    ~CConnectManagerGroup();

    void Init(uint16 u2SendQueueCount);

    bool AddConnect(CConnectHandler* pConnectHandler);
    bool PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDelete = true);                      //异步发送
    bool PostMessage(uint32 u4ConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDelete = true);            //异步发送
    bool PostMessage(vector<uint32> vecConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDelete = true);             //异步群发指定的ID
    bool PostMessage(vector<uint32> vecConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDelete = true);   //异步群发指定的ID
    bool PostMessageAll(IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDelete = true);
    bool PostMessageAll(const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDelete = true);
    bool CloseConnect(uint32 u4ConnectID);                                                                   //服务器关闭
    _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                                                       //得到指定链接信息
   	void GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo);                         //得到指定别名的所有设置信息
    void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo);                                         //返回当前存活链接的信息
    void SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost);                                        //记录指定链接数据处理时间

    int  GetCount();
    void CloseAll();
    bool Close(uint32 u4ConnectID);                                                                          //客户单关闭
    bool SetConnectName(uint32 u4ConnectID, const char* pName);                                              //设置当前连接名称
    bool SetIsLog(uint32 u4ConnectID, bool blIsLog);                                                         //设置当前连接数据是否写入日志      

    bool StartTimer();                                                                                       //开启定时器
    const char* GetError();

private:
    uint32 GetGroupIndex();                                                                                  //得到当前链接的ID自增量

private:
    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                                                           //控制多线程锁
    typedef map<int, CConnectManager*> mapConnectManager;                                                    //所有链接管理者
    uint16            m_u2ThreadQueueCount;                                                                  //当前发送线程队列个数
    mapConnectManager m_mapConnectManager;                                                                   //管理当前所有发送线程池对象
    uint32            m_u4CurrMaxCount;                                                                      //当前链接自增量
};


typedef ACE_Singleton<CConnectManagerGroup, ACE_Recursive_Thread_Mutex> App_ConnectManager;
typedef ACE_Singleton<CConnectHandlerPool, ACE_Null_Mutex> App_ConnectHandlerPool;

#endif
