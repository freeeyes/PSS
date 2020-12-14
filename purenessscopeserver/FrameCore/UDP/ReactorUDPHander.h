#ifndef _REAACTORUDPHANDLER_H
#define _REAACTORUDPHANDLER_H

#include "ace/ACE.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_string.h"
#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Dgram.h"

#include "BaseHander.h"
#include "CommandAccount.h"
#include "UDPConnectManager.h"

class CReactorUDPHander : public ACE_Event_Handler, public IHandler
{
public:
    CReactorUDPHander(void) = default;

    void CloseFinaly();

    ACE_HANDLE get_handle(void) const final;
    int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE) final;
    int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask) final;

    void SetPacketParseInfoID(uint32 u4PacketParseInfoID);                    //设置对应的m_u4PacketParseInfoID
    int  OpenAddress(const ACE_INET_Addr& AddrRemote, ACE_Reactor* pReactor);

    void Close(uint32 u4ConnectID) final;
    bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize) final;
    bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend) final;
    void SetIsLog(bool blIsLog) final;
    bool Device_Send_Data(const char* pData, ssize_t nLen) final;
    _ClientConnectInfo GetClientConnectInfo() const;
    void GetFlowInfo(uint32& u4FlowIn, uint32& u4FlowOut) const;                    //得到所有的出口流量

private:
    bool CheckMessage(uint32 u4ConnectID, const char* pData, uint32 u4Len, const ACE_INET_Addr& addrRemote); //这里解析数据包并放入数据队列
    int  Init_Open_Address(const ACE_INET_Addr& AddrRemote);                                                 //初始化UDP连接对象
    void SaveSendInfo(uint32 u4Len);                                                                         //记录发送信息
    void Send_Hander_Event(uint32 u4ConnandID, uint8 u1Option, const ACE_INET_Addr& addrRemote);             //发送链接建立消息

    ACE_SOCK_Dgram           m_skRemote;
    ACE_INET_Addr            m_addrLocal;                          //监听方的IP信息
    shared_ptr<CPacketParse> m_pPacketParse;                       //数据包解析类

    PSS_Time_Point          m_atvInput;                             //接收包的时间
    PSS_Time_Point          m_atvOutput;                            //发送包的时间
    uint32                  m_u4RecvPacketCount   = 0;              //接收数据包的数量
    uint32                  m_u4SendPacketCount   = 0;              //发送数据包的数量
    uint32                  m_u4RecvSize          = 0;              //接收数据的总大小
    uint32                  m_u4SendSize          = 0;              //发送数据的总大小
    uint32                  m_u4PacketParseInfoID = 0;              //对应处理packetParse的模块ID
	ACE_Message_Block*      m_pBlockMessage            = nullptr;   //当前发送缓冲等待数据块
	ACE_Message_Block*      m_pBlockRecv               = nullptr;   //接收数据缓冲块
    CMakePacket             m_MakePacket;                           //工作线程消息生成器 
    shared_ptr<_Packet_Parse_Info> m_pPacketParseInfo  = nullptr;   //PacketParse解析器

};
#endif
