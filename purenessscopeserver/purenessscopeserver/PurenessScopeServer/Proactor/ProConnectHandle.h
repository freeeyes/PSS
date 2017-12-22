// ConnectHandle.h
// 处理客户端链接
// 不得不说，今天是一个艰苦的考验，经过几个月完善优化的reactor架构，竟然不能适应多于1024个的链接请求
// 对于ACE架构的理解，使我重新的认识，当然，好在的是，替换架构的部分，只有发送和接受部分。其他部分都可以保留。
// 费尽心机做出来的东西，竟然被自己否定，是一个很痛苦的过程，当然，如果想做的更好，这是必须的代价。
// 坚信自己能够做的更好，发现问题，解决问题即可。
// 加油吧，你能做到的。
// add by freeeyes
// 2009-08-23
//Bobo提出的一个好问题，如果IOCP下，发送和接收不对等，会造成内存无限增长的情况。
//这个问题很好，解决方法是，设置两个水位标，如果两个水位标差距超过了一个容忍值，那么就断开连接。
//linux下不会有这个问题，只在IOCP下出现。
//add by freeyes
//2013-09-18

#ifndef _PROCONNECTHANDLE_H
#define _PROCONNECTHANDLE_H

#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Asynch_IO.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Proactor.h"

#include "HashTable.h"
#include "AceProactorManager.h"
#include "IConnectManager.h"
#include "TimerManager.h"
#include "MakePacket.h"
#include "PacketParsePool.h"
#include "BuffPacketManager.h"
#include "Fast_Asynch_Read_Stream.h"
#include "ForbiddenIP.h"
#include "IPAccount.h"
#include "SendMessage.h"
#include "CommandAccount.h"
#include "SendCacheManager.h"
#include "LoadPacketParse.h"
#include "TimeWheelLink.h"
#include "FileTest.h"

#include <vector>

class CProConnectHandle : public ACE_Service_Handler
{
public:
    CProConnectHandle(void);
    ~CProConnectHandle(void);

    //重写继承方法
    virtual void open(ACE_HANDLE h, ACE_Message_Block&);                                             //用户建立一个链接
    virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result& result);                   //处理接受到用户数据包信息事件
    virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result& result);                 //处理发送到用户数据完成的事件
    virtual void addresses(const ACE_INET_Addr& remote_address, const ACE_INET_Addr& local_address); //获得当前远程客户端的IP地址信息

    uint32 file_open(IFileTestManager* pFileTest);                                           //文件入口打开接口
    int handle_write_file_stream(const char* pData, uint32 u4Size, uint8 u1ParseID);         //文件接口模拟数据包入口

    void Init(uint16 u2HandlerID);                                            //Connect Pool初始化调用的函数

    bool SendMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, uint8 u1State, uint8 u1SendType, uint32& u4PacketSize, bool blDelete, int nMessageID);   //发送给客户端数据的函数
    void Close(int nIOCount = 1, int nErrno = 0);                                                  //当前连接对象关闭
    bool ServerClose(EM_Client_Close_status emStatus, uint8 u1OptionEvent = PACKET_SDISCONNECT);   //服务器关闭客户端链接的函数
    void SetLocalIPInfo(const char* pLocalIP, uint32 u4LocalPort);            //设置监听IP和端口信息

    uint32             GetHandlerID();                                        //得到当前初始化的HanddlerID
    const char*        GetError();                                            //得到当前链接错误信息
    void               SetConnectID(uint32 u4ConnectID);                      //设置当前链接的ID
    uint32             GetConnectID();                                        //获得当前链接的ID
    uint8              GetConnectState();                                     //得到链接状态
    uint8              GetSendBuffState();                                    //得到发送状态
    _ClientConnectInfo GetClientInfo();                                       //得到客户端信息
    _ClientIPInfo      GetClientIPInfo();                                     //得到客户端IP信息
    _ClientIPInfo      GetLocalIPInfo();                                      //得到监听IP信息
    void               SetConnectName(const char* pName);                     //设置当前连接名称
    char*              GetConnectName();                                      //得到别名
    void               SetIsLog(bool blIsLog);                                //设置当前连接数据是否写入日志
    bool               GetIsLog();                                            //得到是否可以写日志
    void               SetHashID(int nHashID);                                //设置HashID
    int                GetHashID();                                           //得到当前HashID

    void SetRecvQueueTimeCost(uint32 u4TimeCost);                             //记录当前接收数据到模块处理完成的具体时间消耗
    void SetSendQueueTimeCost(uint32 u4TimeCost);                             //记录当前从发送队列到数据发送完成的具体时间消耗
    void SetSendCacheManager(ISendCacheManager* pSendCacheManager);           //设置发送缓冲接口
    void SetPacketParseInfoID(uint32 u4PacketParseInfoID);                    //设置对应的m_u4PacketParseInfoID
    uint32 GetPacketParseInfoID();                                            //获得相应的m_u4PacketParseInfoID

private:
    bool RecvClinetPacket(uint32 u4PackeLen);                                 //接受数据包
    bool CheckMessage();                                                      //处理接收的数据
    bool PutSendPacket(ACE_Message_Block* pMbData);                           //将发送数据放入队列
    void ClearPacketParse(ACE_Message_Block& mbCurrBlock);                    //清理正在使用的PacketParse
    void PutSendPacketError(ACE_Message_Block* pMbData);                      //发送失败回调

private:
    uint64             m_u8RecvQueueTimeCost;          //成功接收数据到数据处理完成（未发送）花费的时间总和
    uint64             m_u8SendQueueTimeCost;          //成功发送数据到数据处理完成（只发送）花费的时间总和
    uint32             m_u4MaxPacketSize;              //单个数据包的最大长度
    uint32             m_u4RecvQueueCount;             //当前链接被处理的数据包数
    uint32             m_u4HandlerID;                  //此Hander生成时的ID
    uint32             m_u4ConnectID;                  //当前Connect的流水号
    uint32             m_u4AllRecvCount;               //当前链接接收数据包的个数
    uint32             m_u4AllSendCount;               //当前链接发送数据包的个数
    uint32             m_u4AllRecvSize;                //当前链接接收字节总数
    uint32             m_u4AllSendSize;                //当前链接发送字节总数
    uint32             m_u4SendMaxBuffSize;            //发送数据最大缓冲长度
    uint32             m_u4ReadSendSize;               //准备发送的字节数（水位标）
    uint32             m_u4SuccessSendSize;            //实际客户端接收到的总字节数（水位标）
    uint32             m_u4LocalPort;                  //本地监听端口
    uint32             m_u4SendThresHold;              //发送阀值(消息包的个数)
    uint32             m_u4SendCheckTime;              //发送检测时间的阀值
    uint32             m_u4PacketParseInfoID;          //对应处理packetParse的模块ID
    uint32             m_u4PacketDebugSize;            //记录能存二进制数据包的最大字节
    int                m_nHashID;                      //对应Hash列表中的ID
    int                m_u4RecvPacketCount;            //接受包的个数
    int                m_nIOCount;                     //当前IO操作的个数
    uint16             m_u2SendQueueMax;               //发送队列最大长度
    uint16             m_u2MaxConnectTime;             //最大链接时间判定
    uint16             m_u2SendQueueTimeout;           //发送超时时间,超过这个时间的都会被记录到日志中
    uint16             m_u2RecvQueueTimeout;           //接受超时时间，超过这个时间的都会被记录到日志中
    uint16             m_u2TcpNodelay;                 //Nagle算法开关
    uint8              m_u1ConnectState;               //目前链接处理状态
    uint8              m_u1SendBuffState;              //目前缓冲器是否有等待发送的数据
    uint8              m_u1IsActive;                   //连接是否为激活状态，0为否，1为是
    bool               m_blIsLog;                      //是否写入日志，false为不写入，true为写入

    ACE_INET_Addr      m_addrRemote;                   //远程链接客户端地址
    ACE_Time_Value     m_atvConnect;                   //当前链接建立时间
    ACE_Time_Value     m_atvInput;                     //最后一次接收数据时间
    ACE_Time_Value     m_atvOutput;                    //最后一次发送数据时间
    ACE_Time_Value     m_atvSendAlive;                 //链接存活时间
    CBuffPacket        m_AlivePacket;                  //服务器生存包
    CPacketParse*      m_pPacketParse;                 //数据包解析类
    char               m_szConnectName[MAX_BUFF_100];  //连接名称，可以开放给逻辑插件去设置
    char               m_szError[MAX_BUFF_500];        //错误信息描述文字
    char               m_szLocalIP[MAX_BUFF_50];       //本地监听IP


    ACE_Recursive_Thread_Mutex m_ThreadWriteLock;
    _TimeConnectInfo    m_TimeConnectInfo;              //链接健康检测器
    ACE_Message_Block*  m_pBlockMessage;                //当前发送缓冲等待数据块
    EM_Client_Close_status m_emStatus;                  //当前服务器关闭标记

    CPacketParse        m_objSendPacketParse;           //发送数据包组织结构
    char*               m_pPacketDebugData;             //记录数据包的Debug缓冲字符串

    EM_IO_TYPE          m_emIOType;                     //当前IO入口类型
    IFileTestManager*   m_pFileTest;                    //文件测试接口入口

    Fast_Asynch_Read_Stream  m_Reader;
    Fast_Asynch_Write_Stream m_Writer;
};

//管理所有已经建立的链接
class CProConnectManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CProConnectManager(void);
    ~CProConnectManager(void);

    void Init(uint16 u2Index);

    static void TimeWheel_Timeout_Callback(void* pArgsContext, vector<CProConnectHandle*> vecProConnectHandle);

    virtual int open(void* args = 0);
    virtual int svc (void);
    virtual int close (u_long);
    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);

    void CloseAll();                                                                                         //关闭所有链接信息
    bool AddConnect(uint32 u4ConnectID, CProConnectHandle* pConnectHandler);                                 //添加一个新的链接信息
    bool SetConnectTimeWheel(CProConnectHandle* pConnectHandler);                                            //设置消息轮盘
    bool DelConnectTimeWheel(CProConnectHandle* pConnectHandler);                                            //删除消息轮盘
    bool SendMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint16 u2CommandID, uint8 u1SendState, uint8 u1SendType, ACE_Time_Value& tvSendBegin, bool blDelete, int nMessageID);          //发送数据
    bool PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                     uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDelete = true, int nMessageID = 0);    //异步发送
    bool PostMessageAll(IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                        uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDelete = true, int nMessageID = 0);    //异步群发
    bool Close(uint32 u4ConnectID);                                                                          //客户端关闭
    bool CloseConnect(uint32 u4ConnectID, EM_Client_Close_status emStatus);                                  //服务器关闭
    bool CloseConnect_By_Queue(uint32 u4ConnectID);                                                          //服务器关闭(主动关闭送到消息队列中关闭)
    void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo);                                         //返回当前存活链接的信息
    void SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost);                                        //记录指定链接数据处理时间

    bool SetConnectName(uint32 u4ConnectID, const char* pName);                                              //设置当前连接名称
    bool SetIsLog(uint32 u4ConnectID, bool blIsLog);                                                         //设置当前连接数据是否写入日志
    void GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo);                         //得到指定别名的所有设置信息
    _CommandData* GetCommandData(uint16 u2CommandID);                                                        //得到命令相关信息

    _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                                                       //得到指定链接信息
    _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID);                                                        //得到监听链接信息
    uint32 GetCommandFlowAccount();                                                                          //得到出口流量信息
    EM_Client_Connect_status GetConnectState(uint32 u4ConnectID);                                            //得到指定的连接状态
    CSendCacheManager* GetSendCacheManager();                                                                //得到内存块管理器

    bool StartTimer();
    bool KillTimer();

    int handle_write_file_stream(uint32 u4ConnectID, const char* pData, uint32 u4Size, uint8 u1ParseID);     //文件接口模拟数据包入口

    int         GetCount();
    const char* GetError();

private:
    virtual int CloseMsgQueue();

private:
    //关闭消息队列条件变量
    ACE_Thread_Mutex m_mutex;
    ACE_Condition<ACE_Thread_Mutex> m_cond;
private:
    uint32                             m_u4SendQueuePutTime;    //发送队列入队超时时间
    uint32                             m_u4TimeConnect;         //单位时间连接建立数
    uint32                             m_u4TimeDisConnect;      //单位时间连接断开数
    uint32                             m_u4TimeCheckID;         //定时器检查的TimerID
    bool                               m_blRun;                 //线程是否在运行
    char                               m_szError[MAX_BUFF_500]; //错误信息描述
    CHashTable<CProConnectHandle>      m_objHashConnectList;    //记录当前已经连接的节点，使用固定内存结构
    ACE_Recursive_Thread_Mutex         m_ThreadWriteLock;       //用于循环监控和断开链接时候的数据锁
    ACE_Time_Value                     m_tvCheckConnect;        //定时器下一次检测链接时间
    CSendMessagePool                   m_SendMessagePool;       //发送对象库
    CCommandAccount                    m_CommandAccount;        //当前线程命令统计数据
    CSendCacheManager                  m_SendCacheManager;      //发送缓冲池
    CTimeWheelLink<CProConnectHandle>  m_TimeWheelLink;         //连接时间轮盘
};

//链接ConnectHandler内存池
class CProConnectHandlerPool
{
public:
    CProConnectHandlerPool(void);
    ~CProConnectHandlerPool(void);

    void Init(int nObjcetCount);
    void Close();

    CProConnectHandle* Create();
    bool Delete(CProConnectHandle* pObject);

    int GetUsedCount();
    int GetFreeCount();

private:
    ACE_Recursive_Thread_Mutex    m_ThreadWriteLock;                     //控制多线程锁
    CHashTable<CProConnectHandle> m_objHashHandleList;                   //Hash管理表
    uint32                        m_u4CurrMaxCount;                      //当前池里Handler总数
};

//经过思考，想把发送对象分在几个线程内去做，提高性能。在这里尝试一下。(多线程模式，一个线程一个队列，这样保持并发能力)
class CProConnectManagerGroup : public IConnectManager
{
public:
    CProConnectManagerGroup();
    ~CProConnectManagerGroup();

    void Init(uint16 u2SendQueueCount);
    void Close();

    bool AddConnect(CProConnectHandle* pConnectHandler);

    bool SetConnectTimeWheel(CProConnectHandle* pConnectHandler);                                            //设置消息轮盘
    bool DelConnectTimeWheel(CProConnectHandle* pConnectHandler);                                            //删除消息轮盘

    bool PostMessage(uint32 u4ConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                     uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0);    //异步发送
    bool PostMessage(uint32 u4ConnectID, const char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                     uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0);    //异步发送
    bool PostMessage(vector<uint32> vecConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                     uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0);    //异步群发指定的ID
    bool PostMessage(vector<uint32> vecConnectID, const char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                     uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0);    //异步群发指定的ID
    bool PostMessageAll(IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                        uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0);    //异步群发
    bool PostMessageAll(const char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                        uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0);    //异步群发
    bool CloseConnect(uint32 u4ConnectID);                                                                   //服务器关闭
    _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                                                       //得到指定链接信息
    _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID);                                                        //得到监听链接信息
    void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo);                                         //返回当前存活链接的信息
    void SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost);                                        //记录指定链接数据处理时间
    bool SetConnectName(uint32 u4ConnectID, const char* pName);                                              //设置当前连接名称
    bool SetIsLog(uint32 u4ConnectID, bool blIsLog);                                                         //设置当前连接数据是否写入日志
    void GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo);                         //得到指定别名的所有设置信息

    int  GetCount();
    void CloseAll();
    bool Close(uint32 u4ConnectID);                                                                          //客户单关闭

    bool StartTimer();                                                                                       //开启定时器
    const char* GetError();                                                                                  //得到错误信息
    void GetCommandData(uint16 u2CommandID, _CommandData& objCommandData);                                   //获得指定命令统计信息
    void GetCommandFlowAccount(_CommandFlowAccount& objCommandFlowAccount);                                  //得到出口流量信息
    EM_Client_Connect_status GetConnectState(uint32 u4ConnectID);                                            //得到连接状态

    int handle_write_file_stream(uint32 u4ConnectID, const char* pData, uint32 u4Size, uint8 u1ParseID);     //文件接口模拟数据包入口

private:
    uint32 GetGroupIndex();                                                                                  //得到当前链接的ID自增量

private:
    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                                                           //控制多线程锁
    CProConnectManager** m_objProConnnectManagerList;                                                        //所有链接管理者
    uint16            m_u2ThreadQueueCount;                                                                  //当前发送线程队列个数
    uint32            m_u4CurrMaxCount;                                                                      //当前链接自增量
};


typedef ACE_Singleton<CProConnectManagerGroup, ACE_Null_Mutex> App_ProConnectManager;
typedef ACE_Singleton<CProConnectHandlerPool, ACE_Null_Mutex> App_ProConnectHandlerPool;

#endif
