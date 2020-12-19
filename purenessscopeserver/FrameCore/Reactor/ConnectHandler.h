
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

#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "BaseTask.h"
#include "ObjectArrayList.h"
#include "HashTable.h"
#include "AceReactorManager.h"
#include "MessageService.h"
#include "IConnectManager.h"
#include "BaseHander.h"
#include "BuffPacketManager.h"
#include "ForbiddenIP.h"
#include "IPAccount.h"
#include "TimerManager.hpp"
#include "SendMessage.h"
#include "CommandAccount.h"
#include "TimeWheelLink.h"
#include "FileTest.h"
#include "TcpRedirection.h"
#include "PerformanceCounter.h"
#include "ConnectCounter.h"

#if PSS_PLATFORM != PLATFORM_WIN
#include "netinet/tcp.h"
#endif

class CConnectHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>, public IHandler
{
public:
    CConnectHandler(void) = default;

    //重写继承方法
    int open(void*) final;                                                 //用户建立一个链接
    int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE) final;            //接受客户端收到的数据块
    int handle_output(ACE_HANDLE fd = ACE_INVALID_HANDLE) final;           //发送客户端数据
    int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask) final;           //链接关闭事件
    bool Device_Send_Data(const char* pData, ssize_t nLen) final;          //透传数据接口

    uint32 file_open(const IFileTestManager* pFileTest) const;                               //文件入口打开接口
    int handle_write_file_stream(const char* pData, uint32 u4Size, uint8 u1ParseID) const;   //文件接口模拟数据包入口

    void Init(uint16 u2HandlerID);                                           //Connect Pool初始化调用时候调用的方法
    void SetPacketParseInfoID(uint32 u4PacketParseInfoID);                   //设置对应的m_u4PacketParseInfoID
    uint32 GetPacketParseInfoID() const;                                     //获得相应的m_u4PacketParseInfoID


    bool CheckSendMask(uint32 u4PacketLen) const;                            //检测指定的连接发送数据是否超过阻塞阀值
    bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize) final;  //发送当前数据

    bool SendTimeoutMessage();                                               //发送连接超时消息

    void SetLocalIPInfo(const char* pLocalIP, uint16 u2LocalPort);           //设置监听IP和端口信息

    void Close(uint32 u4ConnectID) final;                                    //关闭当前连接
    void CloseFinally();                                                     //替代析构函数，符合roz规则

    uint32      GetHandlerID() const;                                        //得到当前的handlerID
    void        SetConnectID(uint32 u4ConnectID);                            //设置当前链接ID
    uint32      GetConnectID() const;                                        //得到当前链接ID
    CONNECTSTATE       GetConnectState() const;                              //得到链接状态
    CONNECTSTATE       GetSendBuffState() const;                             //得到发送状态
    _ClientConnectInfo GetClientInfo() const;                                //得到客户端信息
    _ClientIPInfo      GetClientIPInfo() const;                              //得到客户端IP信息
    _ClientIPInfo      GetLocalIPInfo() const;                               //得到监听IP信息
    void SetConnectName(const char* pName);                                  //设置当前连接名称
    const char* GetConnectName() const;                                      //得到别名
    void SetIsLog(bool blIsLog) final;                                       //设置当前连接数据是否写入日志
    bool GetIsLog() const;                                                   //获得当前连接是否可以写入日志
    void SetHashID(int nHashID);                                             //设置Hash数组下标
    int  GetHashID() const;                                                  //得到Hash数组下标

    bool Write_SendData_To_File(bool blDelete, shared_ptr<IBuffPacket> pBuffPacket);                              //将发送数据写入文件
    bool Send_Input_To_Cache(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize);               //讲发送对象放入缓存
    bool Send_Input_To_TCP(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize);                 //将数据发送给对端
    bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend) final;//发送数据

private:
    ENUM_WHILE_STATE Recv_Packet_Cut(bool& blRet);                           //数据切包
	bool Dispose_Recv_buffer();                                              //处理接收到数据，切包
	void Move_Recv_buffer();                                                 //整理接收内存缓冲区
    void Send_Hander_Event(uint8 u1Option);                                  //发送Handler的事件通知业务线程
    void ConnectOpen();                                                      //设置连接相关打开代码
    uint32 Get_Recv_length() const;                                          //得到要处理的数据长度
    int  Dispose_Paceket_Parse_Head(ACE_Message_Block* pmbHead);             //处理消息头函数
    int  Dispose_Paceket_Parse_Body(ACE_Message_Block* pmbBody, uint32 u4SrcBodyLength); //处理消息头函数
    int  Dispose_Paceket_Parse_Stream(ACE_Message_Block* pCurrMessage);      //处理流消息函数
    bool CheckMessage();                                                     //处理接收的数据
    
    void ClearPacketParse();                                                 //清理正在使用的PacketParse

    int  Dispose_Paceket_Parse_Stream_Single(ACE_Message_Block* pCurrMessage);//处理单一数据包

    int  Dispose_Recv_Data();                                                 //处理接收数据
    int  Init_Open_Connect();                                                 //当第一次建立连接初始化的时候调用

    uint32                     m_u4HandlerID          = 0;                     //此Hander生成时的ID
    uint32                     m_u4ConnectID          = 0;                     //链接的ID
    uint32                     m_u4AllRecvSize        = 0;                     //当前链接接收字节总数
    uint32                     m_u4AllSendSize        = 0;                     //当前链接发送字节总数
    uint32                     m_u4MaxPacketSize      = MAX_MSG_PACKETLENGTH;  //单个数据包的最大长度
    uint32                     m_u4SendMaxBuffSize    = 5 * MAX_BUFF_1024;     //发送数据最大缓冲长度
    uint16                     m_u2LocalPort          = 0;                     //监听的端口号
    uint32                     m_u4PacketParseInfoID  = 0;                     //对应处理packetParse的模块ID
    uint32                     m_u4CurrSize           = 0;                     //当前MB缓冲字符长度
    uint32                     m_u4PacketDebugSize    = 0;                     //记录能存二进制数据包的最大字节
    int                        m_nHashID              = 0;                     //对应的Pool的Hash数组下标
    uint16                     m_u2MaxConnectTime     = 0;                     //最大时间链接判定
    uint16                     m_u2TcpNodelay         = TCP_NODELAY_ON;        //Nagle算法开关
    CONNECTSTATE               m_u1ConnectState       = CONNECTSTATE::CONNECT_INIT;          //目前链接处理状态
    CONNECTSTATE               m_u1SendBuffState      = CONNECTSTATE::CONNECT_SENDNON;       //目前缓冲器是否有等待发送的数据
    bool                       m_blBlockState         = false;                 //是否处于阻塞状态 false为不在阻塞状态，true为在阻塞状态
    bool                       m_blIsLog              = false;                 //是否写入日志，false为不写入，true为写入
    string                     m_strLocalIP;                                   //本地的IP端口
    string                     m_strConnectName;                               //连接名称，可以开放给逻辑插件去设置
    ACE_INET_Addr              m_addrRemote;                                   //远程链接客户端地址
    ACE_INET_Addr              m_addrLocal;                                    //远程链接客户端地址
    PSS_Time_Point             m_atvConnect;                                   //当前链接建立时间
    PSS_Time_Point             m_atvInput;                                     //最后一次接收数据时间
    PSS_Time_Point             m_atvOutput;                                    //最后一次发送数据时间
    shared_ptr<CPacketParse>   m_pPacketParse;                               //数据包解析类
    ACE_Message_Block*         m_pBlockRecv           = nullptr;               //接收数据缓冲块
    ACE_Message_Block*         m_pBlockMessage        = nullptr;               //当前发送缓冲等待数据块
    EM_IO_TYPE                 m_emIOType             = EM_IO_TYPE::NET_INPUT; //当前IO入口类型
    IFileTestManager*          m_pFileTest            = nullptr;               //文件测试接口入口
    string                     m_strDeviceName;                                //转发接口名称
    CMakePacket                m_MakePacket;                                   //工作线程消息生成器 
    shared_ptr<_Packet_Parse_Info> m_pPacketParseInfo     = nullptr;           //PacketParse的解析器
    
};

//链接ConnectHandler内存池
class CConnectHandlerPool
{
public:
    CConnectHandlerPool(void) = default;

    void Init(int nObjcetCount);
    void Close();

    CConnectHandler* Create();
    bool Delete(CConnectHandler* pObject);

    int GetUsedCount();
    int GetFreeCount();

private:
    ACE_Recursive_Thread_Mutex        m_ThreadWriteLock;                     //控制多线程锁
    CHashTable<CConnectHandler>       m_objHashHandleList;                   //Hash管理表
    CObjectArrayList<CConnectHandler> m_objHandlerList;                      //数据列表对象
};


using App_ConnectHandlerPool = PSS_singleton<CConnectHandlerPool>;

#endif
