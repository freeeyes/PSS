#ifndef _PROACTORUDPHANDLE_H
#define _PROACTORUDPHANDLE_H

#include "ace/ACE.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_string.h"
#include "ace/Proactor.h"
#include "ace/Asynch_IO.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"

#include "define.h"
#include "MessageBlockManager.h"
#include "PacketParsePool.h"
#include "MakePacket.h"
#include "ForbiddenIP.h"
#include "CommandAccount.h"
#include "LoadPacketParse.h"

class CProactorUDPHandler : public ACE_Service_Handler
{
public:
    CProactorUDPHandler(void);
    ~CProactorUDPHandler(void);

    void SetPacketParseInfoID(uint32 u4PacketParseInfoID);                    //设置对应的m_u4PacketParseInfoID
    virtual void handle_read_dgram(const ACE_Asynch_Read_Dgram::Result& result);

    int  OpenAddress(const ACE_INET_Addr& AddrLocal, ACE_Proactor* pProactor);
    void Close();
    bool SendMessage(const char*& pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead = true, uint16 u2CommandID = 0, bool blDlete = true);
    _ClientConnectInfo GetClientConnectInfo();
    void GetCommandData(uint16 u2CommandID, _CommandData& objCommandData);                                   //获得指定命令统计信息

private:
    bool CheckMessage(ACE_Message_Block* pMbData, uint32 u4Len);     //这里解析数据包并放入数据队列

private:
    CPacketParse*           m_pPacketParse;                 //数据包解析类
    ACE_INET_Addr           m_addrRemote;                   //数据发送方的IP信息
    ACE_INET_Addr           m_addrLocal;                    //监听方的IP信息
    ACE_SOCK_Dgram          m_skRemote;
    ACE_Asynch_Read_Dgram   m_Read;
    ACE_Asynch_Write_Dgram  m_Write;

    ACE_Time_Value          m_atvInput;                     //接收包的时间
    ACE_Time_Value          m_atvOutput;                    //发送包的时间
    uint32                  m_u4RecvPacketCount;            //接收数据包的数量
    uint32                  m_u4SendPacketCount;            //发送数据包的数量
    uint32                  m_u4RecvSize;                   //接收数据的总大小
    uint32                  m_u4SendSize;                   //发送数据的总大小
    char                    m_szCompletionkey[MAX_BUFF_20]; //完成端口的Key
    char                    m_szAct[MAX_BUFF_20];           //动作
    _TimeConnectInfo        m_TimeConnectInfo;              //链接健康检测器
    CCommandAccount         m_CommandAccount;               //数据包统计
    uint32                  m_u4PacketParseInfoID;          //对应处理packetParse的模块ID
};
#endif
