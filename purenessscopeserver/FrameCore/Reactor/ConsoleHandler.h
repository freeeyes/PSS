#ifndef _CONSOLEHANDLE_H
#define _CONSOLEHANDLE_H

//服务于对服务器控制的模块，接受远程控制信令并解析
//从最初版本就一直相加，但是最近才开始搞，主要是自己懒，这次决定不再给自己这个理由。
//add by freeeyes
#include "define.h"
#include "TimeStamp.hpp"

#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "AceReactorManager.h"
#include "MessageService.h"
#include "IConnectManager.h"
#include "BaseConsoleHandle.h"
#include "PacketConsoleParse.h"

class CConsoleHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:
    CConsoleHandler(void) = default;

    //重写继承方法
    int open(void*) final;                                                //用户建立一个链接
    int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE) final;
    int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask) final;

    bool SendMessage(shared_ptr<IBuffPacket> pBuffPacket, uint8 u1OutputType);

    void Close(int nIOCount = 1);
    bool ServerClose();

    const char*  GetError() const;
    void         SetConnectID(uint32 u4ConnectID);
    uint32       GetConnectID() const;
    CONNECTSTATE GetConnectState() const;                                     //得到链接状态
    CONNECTSTATE GetSendBuffState() const;                                    //得到发送状态
    uint8        GetIsClosing() const;                                        //链接是否应该关闭

private:
    bool CheckMessage();                                                      //处理接收的数据
    bool PutSendPacket(ACE_Message_Block* pMbData);                           //发送数据
    void Clear_PacketParse();                                                 //清理不需要的PacketParse
    bool CompareConsoleClinetIP(const char* pIP) const;                       //检查一个Console连接的IP是否在指定的允许列表中

    uint32                     m_u4ConnectID           = 0;                  //链接的ID
    uint32                     m_u4AllRecvCount        = 0;                  //当前链接接收数据包的个数
    uint32                     m_u4AllSendCount        = 0;                  //当前链接发送数据包的个数
    uint32                     m_u4AllRecvSize         = 0;                  //当前链接接收字节总数
    uint32                     m_u4AllSendSize         = 0;                  //当前链接发送字节总数
    uint32                     m_u4CurrSize            = 0;                  //当前MB缓冲字符长度
    int                        m_nIOCount              = 1;                  //当前IO操作的个数
    CONNECTSTATE               m_u1ConnectState        = CONNECTSTATE::CONNECT_INIT;       //目前链接处理状态
    CONNECTSTATE               m_u1SendBuffState       = CONNECTSTATE::CONNECT_SENDNON;    //目前缓冲器是否有等待发送的数据
    uint8                      m_u1IsClosing           = HANDLE_ISCLOSE_NO;  //是否应该关闭 0为否，1为是
    string                     m_strError;
    ACE_INET_Addr              m_addrRemote;
    PSS_Time_Point             m_atvConnect;
    PSS_Time_Point             m_atvInput;
    PSS_Time_Point             m_atvOutput;
    PSS_Time_Point             m_atvSendAlive;
    ACE_Recursive_Thread_Mutex m_ThreadLock;
    _TimerCheckID*                  m_pTCClose             = nullptr;
    shared_ptr<CConsolePacketParse> m_pPacketParse         = nullptr;                 //数据包解析类
    ACE_Message_Block*              m_pCurrMessage         = nullptr;                 //当前的MB对象
};

#endif
