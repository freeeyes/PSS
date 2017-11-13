#ifndef _PROCONSOLEHANDLE_H
#define _PROCONSOLEHANDLE_H

//服务于对服务器控制的模块，接受远程控制信令并解析
//从最初版本就一直相加，但是最近才开始搞，主要是自己懒，这次决定不再给自己这个理由。
//add by freeeyes
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Asynch_IO.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Proactor.h"
#include "ace/Thread_Mutex.h"

#include "AceProactorManager.h"
#include "IConnectManager.h"
#include "TimerManager.h"
#include "ConsoleMessage.h"
#include "PacketConsoleParse.h"
#include "BuffPacketManager.h"
#include "Fast_Asynch_Read_Stream.h"
#include "MessageBlockManager.h"

class CProConsoleHandle : public ACE_Service_Handler
{
public:
    CProConsoleHandle();
    ~CProConsoleHandle();

    virtual void open(ACE_HANDLE h, ACE_Message_Block&);                                 //用户建立一个链接
    virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result& result);
    virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result& result);
    virtual void addresses(const ACE_INET_Addr& remote_address, const ACE_INET_Addr& local_address);

    bool SendMessage(IBuffPacket* pBuffPacket, uint8 u1OutputType);
    void Close(int nIOCount = 1);
    bool ServerClose();

    const char* GetError();
    void        SetConnectID(uint32 u4ConnectID);
    uint32      GetConnectID();
    uint8       GetConnectState();                                           //得到链接状态
    uint8       GetSendBuffState();                                          //得到发送状态
    bool        GetIsClosing();                                              //链接是否应该关闭

private:
    bool RecvClinetPacket(uint32 u4PackeLen);                                 //接受数据包
    bool CheckMessage();                                                      //处理接收的数据
    bool PutSendPacket(ACE_Message_Block* pMbData);                           //将发送数据放入队列

private:
    uint32           m_u4HandlerID;                  //此Hander生成时的ID
    uint32           m_u4ConnectID;                  //当前Connect的流水号
    uint32           m_u4AllRecvCount;               //当前链接接收数据包的个数
    uint32           m_u4AllSendCount;               //当前链接发送数据包的个数
    uint32           m_u4AllRecvSize;                //当前链接接收字节总数
    uint32           m_u4AllSendSize;                //当前链接发送字节总数
    uint32           m_u4SendThresHold;              //发送阀值(消息包的个数)
    uint32           m_u4SendCheckTime;              //发送检测时间的阀值
    int              m_u4RecvPacketCount;            //接受包的个数
    int              m_nIOCount;                     //当前IO操作的个数
    uint16           m_u2SendQueueMax;               //发送队列最大长度
    uint16           m_u2MaxConnectTime;             //最大链接时间判定
    uint8            m_u1ConnectState;               //目前链接处理状态
    uint8            m_u1SendBuffState;              //目前缓冲器是否有等待发送的数据
    bool             m_blCanWrite;                   //上一个数据包是否发送结束
    bool             m_blTimeClose;                  //是否正在关闭
    char             m_szError[MAX_BUFF_500];
    ACE_INET_Addr    m_addrRemote;
    ACE_Time_Value   m_atvConnect;
    ACE_Time_Value   m_atvInput;
    ACE_Time_Value   m_atvOutput;
    ACE_Time_Value   m_atvSendAlive;
    CBuffPacket      m_AlivePacket;                  //服务器生存包

    CConsolePacketParse* m_pPacketParse;             //数据包解析类

    ACE_Recursive_Thread_Mutex m_ThreadWriteLock;

    Fast_Asynch_Read_Stream  m_Reader;
    Fast_Asynch_Write_Stream m_Writer;
};

#endif
