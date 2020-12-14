#ifndef _PROACTORUDPHANDLE_H
#define _PROACTORUDPHANDLE_H

#include "ace/ACE.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_string.h"
#include "ace/Proactor.h"
#include "ace/Asynch_IO.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"

#include "IHandler.h"
#include "XmlConfig.h"
#include "BaseHander.h"
#include "ForbiddenIP.h"
#include "CommandAccount.h"
#include "LoadPacketParse.h"
#include "UDPConnectManager.h"

class CProactorUDPHandler : public ACE_Service_Handler, public IHandler
{
public:
    CProactorUDPHandler(void);
    virtual ~CProactorUDPHandler(void);

    void SetPacketParseInfoID(uint32 u4PacketParseInfoID);                    //设置对应的m_u4PacketParseInfoID
    virtual void handle_read_dgram(const ACE_Asynch_Read_Dgram::Result& result);

    int  OpenAddress(const ACE_INET_Addr& AddrLocal, ACE_Proactor* pProactor);
    virtual void Close(uint32 u4ConnectID);
    virtual bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize);
    virtual bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend);
    virtual void SetIsLog(bool blIsLog);                                      //记录日志
    virtual bool Device_Send_Data(const char* pData, ssize_t nLen);
    _ClientConnectInfo GetClientConnectInfo();
    void GetFlowInfo(uint32& u4FlowIn, uint32& u4FlowOut);                    //得到所有的流量信息
    void SetRecvSize(uint32 u4RecvSize);                                      //设置接收数据包最大尺寸
    uint32 GetRecvSize();                                                     //得到数据包最大尺寸

private:
    bool CheckMessage(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Len, ACE_INET_Addr addrRemote);  //这里解析数据包并放入数据队列
    void SaveProSendInfo(uint32 u4Len);                                                                         //记录发送信息
    void Send_Hander_Event(uint32 u4ConnandID, uint8 u1Option, ACE_INET_Addr addrRemote);                       //发送链接建立消息

    shared_ptr<CPacketParse>       m_pPacketParse     = nullptr;       //数据包解析类
    shared_ptr<_Packet_Parse_Info> m_pPacketParseInfo = nullptr;       //处理业务的PacketParse库
    ACE_INET_Addr           m_addrLocal;                               //监听方的IP信息
    ACE_SOCK_Dgram          m_skRemote;
    ACE_Asynch_Read_Dgram   m_Read;
    ACE_Asynch_Write_Dgram  m_Write;

    PSS_Time_Point          m_atvInput;                     //接收包的时间
    PSS_Time_Point          m_atvOutput;                    //发送包的时间
    uint32                  m_u4RecvPacketCount;            //接收数据包的数量
    uint32                  m_u4SendPacketCount;            //发送数据包的数量
    uint32                  m_u4RecvSize;                   //接收数据的总大小
    uint32                  m_u4SendSize;                   //发送数据的总大小
    uint32                  m_u4PacketParseInfoID;          //对应处理packetParse的模块ID
    uint32                  m_u4MaxRecvSize;                //最大接收数据包尺寸
    char                    m_szCompletionkey[MAX_BUFF_20]; //完成端口的Key
    char                    m_szAct[MAX_BUFF_20];           //动作
	ACE_Message_Block*      m_pBlockMessage = nullptr;      //当前发送缓冲等待数据块
	ACE_Message_Block*      m_pBlockRecv    = nullptr;      //接收数据缓冲块

    _TimeConnectInfo        m_TimeConnectInfo;              //链接健康检测器
    CCommandAccount         m_CommandAccount;               //数据包统计

    CMakePacket             m_MakePacket;
};

#endif
