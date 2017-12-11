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

#include "MainConfig.h"
#include "MessageBlockManager.h"
#include "ClientMessage.h"
#include "AceReactorManager.h"
#include "LogManager.h"
#include "ServerMessageTask.h"

class CConnectClient : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:
    CConnectClient(void);
    ~CConnectClient(void);

    virtual int open(void*);
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);
    virtual int handle_output(ACE_HANDLE fd = ACE_INVALID_HANDLE);

    void SetClientMessage(IClientMessage* pClientMessage); //设置消息接收处理类
    void SetServerID(int nServerID);                       //设置当前的ServerID
    int  GetServerID();                                    //获得当前ServerID
    bool SendData(ACE_Message_Block* pmblk);
    void Close();

    bool GetTimeout(ACE_Time_Value tvNow);                 //获得当前数据处理是否超时

    void ClientClose();                                    //主动关闭
    _ClientConnectInfo GetClientConnectInfo();             //得到当前链接信息

private:
    int RecvData();                                        //接收数据，正常模式
    int RecvData_et();                                     //接收数据，et模式

public:
    uint32                      m_u4SendSize;           //发送字节数
    uint32                      m_u4SendCount;          //发送数据包数
    uint32                      m_u4RecvSize;           //接受字节数
    uint32                      m_u4RecvCount;          //接受数据包数
    uint32                      m_u4CostTime;           //消息处理总时间
    uint32                      m_u4CurrSize;           //当前接收到的字节数
    uint32                      m_u4MaxPacketSize;      //最大接收包长
    int                         m_nIOCount;             //当前IO操作的个数
    int                         m_nServerID;            //服务器ID
    uint8                       m_u1ConnectState;       //连接状态
    char                        m_szError[MAX_BUFF_500];
    ACE_INET_Addr               m_addrRemote;

    ACE_Recursive_Thread_Mutex  m_ThreadLock;
    IClientMessage*             m_pClientMessage;       //消息处理类的指针
    ACE_Message_Block*          m_pCurrMessage;         //当前的MB对象
    ACE_Time_Value              m_atvBegin;             //链接建立时间

    EM_s2s                      m_ems2s;                //是否需要回调状态
    ACE_Time_Value              m_atvRecv;              //数据接收时间
    EM_Server_Recv_State        m_emRecvState;          //0为未接收数据，1为接收数据完成，2为处理数据完成
};
#endif
