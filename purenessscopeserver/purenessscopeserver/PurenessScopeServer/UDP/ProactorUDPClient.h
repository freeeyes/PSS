#ifndef _PROACTORUDPCLIENT_H
#define _PROACTORUDPCLIENT_H

#include "ace/ACE.h"
#include "ace/OS_main.h"
#include "ace/Proactor.h"
#include "ace/Asynch_IO.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"

#include "define.h"
#include "MessageBlockManager.h"
#include "ClientUDPMassage.h"

class CProactorUDPClient : public ACE_Service_Handler
{
public:
    CProactorUDPClient(void);
    ~CProactorUDPClient(void);

    virtual void handle_read_dgram(const ACE_Asynch_Read_Dgram::Result& result);

    int  OpenAddress(const ACE_INET_Addr& AddrLocal, EM_UDP_TYPE emType, ACE_Proactor* pProactor, IClientUDPMessage* pClientUDPMessage);
    void Close();
    bool SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, int nPort);
    _ClientConnectInfo GetClientConnectInfo();

private:
    bool CheckMessage(ACE_Message_Block* pMbData, uint32 u4Len);     //这里解析数据包并放入数据队列

private:
    ACE_INET_Addr           m_addrRemote;                   //数据发送方的IP信息
    ACE_SOCK_Dgram          m_skRemote;
    ACE_Asynch_Read_Dgram   m_Read;
    ACE_Asynch_Write_Dgram  m_Write;
    IClientUDPMessage*      m_pClientUDPMessage;            //需要回调的消息类

    ACE_Time_Value          m_atvInput;                     //接收包的时间
    ACE_Time_Value          m_atvOutput;                    //发送包的时间
    uint32                  m_u4RecvPacketCount;            //接收数据包的数量
    uint32                  m_u4SendPacketCount;            //发送数据包的数量
    uint32                  m_u4RecvSize;                   //接收数据的总大小
    uint32                  m_u4SendSize;                   //发送数据的总大小
};
#endif
