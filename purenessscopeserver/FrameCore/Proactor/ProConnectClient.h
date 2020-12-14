#ifndef _PROCONNECTCLIENT_H
#define _PROCONNECTCLIENT_H

//提供客户端反复链接的解决方案
//用户链接类，只管数据的收发，自动重连策略由上层指定
//add by freeeyes 2010-12-24

#include "ace/Recursive_Thread_Mutex.h"
#include "ace/INET_Addr.h"
#include "ace/Asynch_IO.h"
#include "ace/Mutex.h"
#include "ace/Thread_Mutex.h"

#include "AceProactorManager.h"
#include "BaseConnectClient.h"
#include "LogManager.h"
#include "BaseHander.h"
#include "TcpRedirection.h"

#define MAX_BUFF_1024 1024
#define MAX_IP_LEN    16

class CProConnectClient : public ACE_Service_Handler, public IHandler
{
public:
    CProConnectClient(void);
    virtual ~CProConnectClient(void);

    virtual void open(ACE_HANDLE h, ACE_Message_Block&);                                               //用户建立一个链接
    virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result& result);                     //接受用户数据
    virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result& result);                   //发送用户数据
    virtual void addresses(const ACE_INET_Addr& remote_address, const ACE_INET_Addr& local_address);   //获得链接地址
    virtual bool Device_Send_Data(const char* pData, ssize_t nLen);                                    //透传数据接口

    void Close(uint32 u4ConnectID) final;
    bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize) final;
    bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend) final;
    void SetIsLog(bool blIsLog) final;

    void Close();                                          //链接关闭
    void ClientClose(EM_s2s& ems2s);                       //客户端自我关闭
    void SetServerID(int nServerID);                       //设置当前的ServerID
    void SetPacketParseInfoID(uint32 u4PacketParseInfoID); //设置PacketParseID
    void SetClientMessage(IClientMessage* pClientMessage); //设置消息接收处理类
    int  GetServerID();                                    //获得当前ServerID
    bool SendData(ACE_Message_Block* pmblk);               //发送数据

    bool GetTimeout(PSS_Time_Point const& tvNow);                 //获得当前数据处理是否超时

    _ClientConnectInfo GetClientConnectInfo();             //得到当前链接信息

private:
    bool RecvData(uint32 u4PacketLen, ACE_Message_Block* pmbSave);        //接收数据
    int SendMessageGroup(uint16 u2CommandID, ACE_Message_Block* pmblk);   //将接收的数据包发给框架消息处理模块

private:
    ACE_INET_Addr               m_AddrRemote;                //链接远程服务器的地址
    ACE_Message_Block*          m_mbRecv;                    //收取数据缓冲
    ACE_Asynch_Read_Stream      m_Reader;                    //读取流
    ACE_Asynch_Write_Stream     m_Writer;                    //写入流
    IClientMessage*             m_pClientMessage;            //处理消息接收的类
    int                         m_nIOCount;                  //当前引用个数
    uint32                      m_u4MaxPacketSize;           //最大接收包长
    int                         m_nServerID;                 //当前ServerID
    ACE_Recursive_Thread_Mutex  m_ThreadWritrLock;           //线程锁

    PSS_Time_Point              m_atvBegin;                  //链接建立时间
    uint32                      m_u4SendSize;                //发送字节数
    uint32                      m_u4SendCount;               //发送数据包数
    uint32                      m_u4RecvSize;                //接受字节数
    uint32                      m_u4RecvCount;               //接受数据包数
    uint32                      m_u4CostTime;                //消息处理总时间
    EM_s2s                      m_ems2s;                     //是否需要回调
    PSS_Time_Point              m_atvRecv;                   //数据接收时间
    EM_Server_Recv_State        m_emRecvState;               //0为未接收数据，1为接收数据完成，2为处理数据完成
    EM_CONNECT_IO_DISPOSE       m_emDispose;                 //处理模式，框架处理 or 业务处理
    uint32                      m_u4PacketParseInfoID;       //框架处理模块ID
    string                      m_strDeviceName;             //转发接口名称
    CMakePacket                 m_MakePacket;
};
#endif