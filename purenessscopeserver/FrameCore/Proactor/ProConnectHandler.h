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

#include "IHandler.h"
#include "BaseHander.h"
#include "BaseTask.h"
#include "ObjectArrayList.h"
#include "HashTable.h"
#include "AceProactorManager.h"
#include "IConnectManager.h"
#include "TimerManager.hpp"
#include "BuffPacketManager.h"
#include "Fast_Asynch_Read_Stream.h"
#include "ForbiddenIP.h"
#include "IPAccount.h"
#include "SendMessage.h"
#include "CommandAccount.h"
#include "TimeWheelLink.h"
#include "FileTest.h"
#include "XmlConfig.h"
#include "TcpRedirection.h"
#include "PerformanceCounter.h"
#include "ConnectCounter.h"

class CProConnectHandler : public ACE_Service_Handler, public IHandler
{
public:
    CProConnectHandler(void);
    virtual ~CProConnectHandler(void);

    //重写继承方法
    virtual void open(ACE_HANDLE h, ACE_Message_Block&);                                             //用户建立一个链接
    virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result& result);                   //处理接受到用户数据包信息事件
    virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result& result);                 //处理发送到用户数据完成的事件
    virtual void addresses(const ACE_INET_Addr& remote_address, const ACE_INET_Addr& local_address); //获得当前远程客户端的IP地址信息
    virtual bool Device_Send_Data(const char* pData, ssize_t nLen);                                  //透传数据接口

    uint32 file_open(IFileTestManager* pFileTest);                                           //文件入口打开接口
    int handle_write_file_stream(const char* pData, uint32 u4Size, uint8 u1ParseID);         //文件接口模拟数据包入口

    void Init(uint16 u2HandlerID);                                                           //Connect Pool初始化调用的函数

    virtual bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize);     //发送给客户端数据的函数
    virtual void Close(uint32 u4ConnectID);                                                  //当前连接对象关闭
    void SetLocalIPInfo(const char* pLocalIP, uint16 u2LocalPort);                           //设置监听IP和端口信息

    uint32             GetHandlerID();                                        //得到当前初始化的HanddlerID
    void               SetConnectID(uint32 u4ConnectID);                      //设置当前链接的ID
    uint32             GetConnectID();                                        //获得当前链接的ID
    CONNECTSTATE       GetConnectState();                                     //得到链接状态
    CONNECTSTATE       GetSendBuffState();                                    //得到发送状态
    _ClientConnectInfo GetClientInfo();                                       //得到客户端信息
    _ClientIPInfo      GetClientIPInfo();                                     //得到客户端IP信息
    _ClientIPInfo      GetLocalIPInfo();                                      //得到监听IP信息
    void               SetConnectName(const char* pName);                     //设置当前连接名称
    char*              GetConnectName();                                      //得到别名
    virtual void       SetIsLog(bool blIsLog);                                //设置当前连接数据是否写入日志
    bool               GetIsLog();                                            //得到是否可以写日志
    void               SetHashID(int nHashID);                                //设置HashID
    int                GetHashID();                                           //得到当前HashID

    void SetPacketParseInfoID(uint32 u4PacketParseInfoID);                    //设置对应的m_u4PacketParseInfoID
    uint32 GetPacketParseInfoID();                                            //获得相应的m_u4PacketParseInfoID
    bool SendTimeoutMessage();                                                //发送连接超时消息
    virtual bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend);       //将发送数据发送出去

private:
    ENUM_WHILE_STATE Recv_Packet_Cut(bool& blRet);                           //数据切包
    bool Dispose_Recv_buffer();                                              //处理接收到数据，切包
    void Move_Recv_buffer();                                                 //整理接收内存缓冲区
    void Send_Hander_Event(uint8 u1Option);                                  //发送Handler的事件通知业务线程
    void Get_Recv_length();                                                  //得到要处理的数据长度
    int  Dispose_Paceket_Parse_Head(ACE_Message_Block* pmb);                 //处理消息头函数
    int  Dispose_Paceket_Parse_Body(ACE_Message_Block* pmb, uint32 u4SrcBodyLength);                 //处理消息头函数
    int  Dispose_Paceket_Parse_Stream(ACE_Message_Block* pCurrMessage);      //处理流消息函数

    bool Write_SendData_To_File(bool blDelete, shared_ptr<IBuffPacket> pBuffPacket);                  //将发送数据写入文件
    bool Send_Input_To_Cache(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize);              //讲发送对象放入缓存
    bool Send_Input_To_TCP(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize);                //将数据发送给对端

    bool RecvClinetPacket();                                                                          //接受数据包
    bool CheckMessage();                                                                              //处理接收的数据

    void ClearPacketParse();                                                                          //清理正在使用的PacketParse
    void PutSendPacketError(ACE_Message_Block* pMbData);                                              //发送失败回调

    uint32             m_u4MaxPacketSize;              //单个数据包的最大长度
    uint32             m_u4RecvQueueCount;             //当前链接被处理的数据包数
    uint32             m_u4HandlerID;                  //此Hander生成时的ID
    uint32             m_u4ConnectID;                  //当前Connect的流水号
    uint32             m_u4AllRecvSize;                //当前链接接收字节总数
    uint32             m_u4AllSendSize;                //当前链接发送字节总数
    uint32             m_u4SendMaxBuffSize;            //发送数据最大缓冲长度
    uint16             m_u2LocalPort;                  //本地监听端口
    uint32             m_u4SendCheckTime;              //发送检测时间的阀值
    uint32             m_u4PacketParseInfoID;          //对应处理packetParse的模块ID
    uint32             m_u4PacketDebugSize;            //记录能存二进制数据包的最大字节
    int                m_nHashID;                      //对应Hash列表中的ID
    uint16             m_u2MaxConnectTime;             //最大链接时间判定
    uint16             m_u2RecvQueueTimeout;           //接受超时时间，超过这个时间的都会被记录到日志中
    uint16             m_u2TcpNodelay;                 //Nagle算法开关
    CONNECTSTATE       m_u1ConnectState;               //目前链接处理状态
    CONNECTSTATE       m_u1SendBuffState;              //目前缓冲器是否有等待发送的数据
    bool               m_blIsLog;                      //是否写入日志，false为不写入，true为写入

    ACE_INET_Addr      m_addrRemote;                   //远程链接客户端地址
    ACE_INET_Addr      m_addrListen;                   //本地监听地址
    PSS_Time_Point     m_atvConnect;                   //当前链接建立时间
    PSS_Time_Point     m_atvInput;                     //最后一次接收数据时间
    PSS_Time_Point     m_atvOutput;                    //最后一次发送数据时间
    shared_ptr<CPacketParse> m_pPacketParse;           //数据包解析类
    char               m_szConnectName[MAX_BUFF_100];  //连接名称，可以开放给逻辑插件去设置
    char               m_szLocalIP[MAX_BUFF_50];       //本地监听IP
    string             m_strDeviceName;                //转发接口名称

    ACE_Message_Block*  m_pBlockMessage    = nullptr;  //当前发送缓冲等待数据块
    ACE_Message_Block*  m_pBlockRecv       = nullptr;  //接收数据缓冲块
    shared_ptr<_Packet_Parse_Info> m_pPacketParseInfo = nullptr;  //PacketParse的解析器

    shared_ptr<CPacketParse> m_objSendPacketParse;          //发送数据包组织结构

    EM_IO_TYPE          m_emIOType;                    //当前IO入口类型
    IFileTestManager*   m_pFileTest;                   //文件测试接口入口

    Fast_Asynch_Read_Stream  m_Reader;
    Fast_Asynch_Write_Stream m_Writer;

    CMakePacket         m_MakePacket;

    CPerformanceCounter m_SendCounter;
    CPerformanceCounter m_RecvCounter;
};

//链接ConnectHandler内存池
class CProConnectHandlerPool
{
public:
	CProConnectHandlerPool(void);
	~CProConnectHandlerPool(void);

	void Init(int nObjcetCount);
	void Close();

	CProConnectHandler* Create();
	bool Delete(CProConnectHandler* pObject);

	int GetUsedCount();
	int GetFreeCount();

private:
	ACE_Recursive_Thread_Mutex           m_ThreadWriteLock;                     //控制多线程锁
	CHashTable<CProConnectHandler>       m_objHashHandleList;                   //Hash管理表
	CObjectArrayList<CProConnectHandler> m_objHandlerList;                      //数据列表对象
};

using App_ProConnectHandlerPool = PSS_singleton<CProConnectHandlerPool>;

#endif
