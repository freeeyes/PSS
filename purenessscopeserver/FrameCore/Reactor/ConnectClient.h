#ifndef _CONNECTCLIENT_H
#define _CONNECTCLIENT_H

//处理客户端消息
//Reactor处理模式，解析数据包并将数据放在逻辑类处理。
//添加完了Proactor下的客户端发送，开始添加reactor的系统。
//add by freeeyes
//2011-01-17

#include "define.h"

#include "ace/Reactor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "AceReactorManager.h"
#include "BaseConnectClient.h"
#include "LogManager.h"
#include "BaseHander.h"
#include "TcpRedirection.h"

class CConnectClient : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>, public IHandler
{
public:
    CConnectClient(void) = default;
    ~CConnectClient(void) final = default;

    int open(void*) final;
    int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE) final;
    int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask) final;
    bool Device_Send_Data(const char* pData, ssize_t nLen) final; //透传数据接口

    void Close(uint32 u4ConnectID) final;
    bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize) final;
    bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend) final;
    void SetIsLog(bool blIsLog) final;

    void SetClientMessage(IClientMessage* pClientMessage); //设置消息接收处理类
    void SetServerID(int nServerID);                       //设置当前的ServerID
    int  GetServerID() const;                              //获得当前ServerID
    void SetPacketParseInfoID(uint32 u4PacketParseInfoID); //设置PacketParseID
    bool SendData(ACE_Message_Block* pmblk);
    void Close();

    bool GetTimeout(PSS_Time_Point const& tvNow) const;           //获得当前数据处理是否超时

    void ClientClose();                                           //主动关闭
    _ClientConnectInfo GetClientConnectInfo() const;              //得到当前链接信息

private:
    int RecvData();                                                       //接收数据，正常模式
    int SendMessageGroup(uint16 u2CommandID, ACE_Message_Block* pmblk) const;   //将接收的数据包发给框架消息处理模块

    int Dispose_Recv_Data(ACE_Message_Block* pCurrMessage);               //处理接收到的数据

    uint32                      m_u4SendSize            = 0;          //发送字节数
    uint32                      m_u4SendCount           = 0;          //发送数据包数
    uint32                      m_u4RecvSize            = 0;          //接受字节数
    uint32                      m_u4RecvCount           = 0;          //接受数据包数
    uint32                      m_u4CostTime            = 0;          //消息处理总时间
    uint32                      m_u4CurrSize            = 0;          //当前接收到的字节数
    uint32                      m_u4MaxPacketSize       = MAX_MSG_PACKETLENGTH;      //最大接收包长
    uint32                      m_u4PacketParseInfoID   = 0;          //框架处理模块ID
    int                         m_nIOCount              = 1;          //当前IO操作的个数
    int                         m_nServerID             = 0;          //服务器ID
    CONNECTSTATE                m_u1ConnectState        = CONNECTSTATE::CONNECT_INIT; //连接状态
    string                      m_strError;
    ACE_INET_Addr               m_addrRemote;

    ACE_Recursive_Thread_Mutex  m_ThreadLock;
    IClientMessage*             m_pClientMessage = nullptr;              //消息处理类的指针
    ACE_Message_Block*          m_pCurrMessage   = nullptr;              //当前的MB对象
    PSS_Time_Point              m_atvBegin;                           //链接建立时间

    EM_s2s                      m_ems2s       = EM_s2s::S2S_NEED_CALLBACK;    //是否需要回调状态
	EM_Server_Recv_State        m_emRecvState = EM_Server_Recv_State::SERVER_RECV_INIT;     //0为未接收数据，1为接收数据完成，2为处理数据完成
	EM_CONNECT_IO_DISPOSE       m_emDispose   = EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN;    //处理模式，框架处理 or 业务处理
    PSS_Time_Point              m_atvRecv;                            //数据接收时间
    string                      m_strDeviceName;                      //转发接口名称
    CMakePacket                 m_MakePacket;
};
#endif
