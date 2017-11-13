#ifndef _CONSOLEHANDLE_H
#define _CONSOLEHANDLE_H

//服务于对服务器控制的模块，接受远程控制信令并解析
//从最初版本就一直相加，但是最近才开始搞，主要是自己懒，这次决定不再给自己这个理由。
//add by freeeyes
#include "define.h"

#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "AceReactorManager.h"
#include "MessageService.h"
#include "IConnectManager.h"
#include "ConsoleMessage.h"
#include "MessageBlockManager.h"
#include "PacketConsoleParse.h"
#include "BuffPacketManager.h"
#include "MessageBlockManager.h"

class CConsoleHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:
    CConsoleHandler(void);
    ~CConsoleHandler(void);

    //重写继承方法
    virtual int open(void*);                                                //用户建立一个链接
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);

    bool SendMessage(IBuffPacket* pBuffPacket, uint8 u1OutputType);

    void Close(int nIOCount = 1);
    bool ServerClose();

    const char* GetError();
    void        SetConnectID(uint32 u4ConnectID);
    uint32      GetConnectID();
    uint8       GetConnectState();                                           //得到链接状态
    uint8       GetSendBuffState();                                          //得到发送状态
    uint8       GetIsClosing();                                              //链接是否应该关闭

private:
    bool CheckMessage();                                                      //处理接收的数据
    bool PutSendPacket(ACE_Message_Block* pMbData);                           //发送数据

private:
    uint32                     m_u4HandlerID;                  //此Hander生成时的ID
    uint32                     m_u4ConnectID;                  //链接的ID
    uint32                     m_u4AllRecvCount;               //当前链接接收数据包的个数
    uint32                     m_u4AllSendCount;               //当前链接发送数据包的个数
    uint32                     m_u4AllRecvSize;                //当前链接接收字节总数
    uint32                     m_u4AllSendSize;                //当前链接发送字节总数
    uint32                     m_u4SendThresHold;              //发送阀值(消息包的个数)
    uint32                     m_u4SendCheckTime;              //发送检测时间的阀值
    uint32                     m_u4CurrSize;                   //当前MB缓冲字符长度
    int                        m_nIOCount;                     //当前IO操作的个数
    uint16                     m_u2SendQueueMax;               //发送队列最大长度
    uint16                     m_u2SendCount;                  //当前数据包的个数
    uint16                     m_u2MaxConnectTime;             //最大时间链接判定
    uint8                      m_u1ConnectState;               //目前链接处理状态
    uint8                      m_u1SendBuffState;              //目前缓冲器是否有等待发送的数据
    uint8                      m_u1IsClosing;                  //是否应该关闭 0为否，1为是
    char                       m_szError[MAX_BUFF_500];
    ACE_INET_Addr              m_addrRemote;
    ACE_Time_Value             m_atvConnect;
    ACE_Time_Value             m_atvInput;
    ACE_Time_Value             m_atvOutput;
    ACE_Time_Value             m_atvSendAlive;
    ACE_Recursive_Thread_Mutex m_ThreadLock;
    _TimerCheckID*             m_pTCClose;
    CBuffPacket                m_AlivePacket;                  //服务器生存包
    CConsolePacketParse*       m_pPacketParse;                 //数据包解析类
    ACE_Message_Block*         m_pCurrMessage;                 //当前的MB对象
};

#endif
