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

#include "MessageBlockManager.h"
#include "ClientMessage.h"
#include "AceReactorManager.h"

class CConnectClient : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
    CConnectClient(void);
    ~CConnectClient(void);

    virtual int open(void*);
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);

    void SetClientMessage(IClientMessage* pClientMessage); //设置消息接收处理类
    void SetServerID(int nServerID);                       //设置当前的ServerID
    int  GetServerID();                                    //获得当前ServerID
    bool SendData(ACE_Message_Block* pmblk);
    bool Close();

    void ClinetClose();                                    //主动关闭
    _ClientConnectInfo GetClientConnectInfo();             //得到当前链接信息
    
private:
    int RecvData();                                        //接收数据，正常模式

private:
    typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> Super;

public:
    char                       m_szError[MAX_BUFF_500];
    ACE_INET_Addr              m_addrRemote;
    int                        m_nIOCount;                     //当前IO操作的个数
    ACE_Recursive_Thread_Mutex m_ThreadLock;
    IClientMessage*            m_pClientMessage;               //消息处理类的指针
    ACE_Message_Block*         m_pCurrMessage;                 //当前的MB对象
    int                        m_nServerID;                    //服务器ID
    uint32                     m_u4CurrSize;                   //当前接收到的字节数
    uint32                     m_u4MaxPacketSize;              //最大接收包长

    ACE_Time_Value              m_atvBegin;          //链接建立时间
    uint32                      m_u4SendSize;        //发送字节数
    uint32                      m_u4SendCount;       //发送数据包数
    uint32                      m_u4RecvSize;        //接受字节数
    uint32                      m_u4RecvCount;       //接受数据包数
    uint32                      m_u4CostTime;        //消息处理总时间
    em_State                    m_emState;           //数据包状态 
};
#endif
