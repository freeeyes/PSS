#ifndef _BASEREACTORHANDLER_H
#define _BASEREACTORHANDLER_H

#include "ace/Task_T.h"

#include "define.h"
#include "MakePacket.h"
#include "SendMessage.h"
#include "MessageBlockManager.h"
#include "LoadPacketParse.h"
#include "PacketParse.h"
#include "FileTest.h"
#include "ace/SOCK_Dgram.h"
#include "XmlConfig.h"
#include "TcpRedirection.h"
#include "MakePacket_Common.h"
#include "IPAccount.h"
#include <string>
#include <sstream>
#include <iomanip>

//所有的handler用到的公共函数
//add by freeeyes


//回收内存
void Recovery_Message(bool blDelete, char*& pMessage);

//Udp_Common_Send_Message的传参
class _Send_Message_Param
{
public:
    uint32 m_u4PacketParseInfoID      = 0;
    uint32 m_u4SendLength             = 0;
    string m_strClientIP;
    uint16 m_u2Port                   = 0;
    uint16 m_u2CommandID              = 0;
    bool   m_blDlete                  = false;
    EM_SEND_PACKET_PARSE m_emSendType = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
};

//将数据发送入工作线程消息队列
void Send_MakePacket_Queue(CMakePacket& MakePacketDispose, const _MakePacket& objMakePacket, bool blCommit = true);

//将错误数据发送到工作线程消息队列
void Send_MakePacket_Queue_Error(CMakePacket& MakePacketDispose, uint32 u4ConnectID, ACE_Message_Block* pMessageBlock, const PSS_Time_Point& tvNow);

//udp函数发送数据包合成函数
bool Udp_Common_Send_Message(_Send_Message_Param const& obj_Send_Message_Param, shared_ptr<IBuffPacket> pBuffPacket, const ACE_SOCK_Dgram& skRemote, shared_ptr<_Packet_Parse_Info> pPacketParseInfo, ACE_Message_Block* pBlockMessage);

//udp处理消息头函数
bool Udp_Common_Recv_Head(uint32 u4ConnectID, ACE_Message_Block* pMBHead, shared_ptr<CPacketParse> pPacketParse, shared_ptr<_Packet_Parse_Info> pPacketParseInfo, uint32 u4Len);

//udp处理消息体函数
bool Udp_Common_Recv_Body(uint32 u4ConnectID, ACE_Message_Block* pMBBody, shared_ptr<CPacketParse> pPacketParse, shared_ptr<_Packet_Parse_Info> pPacketParseInfo);

//udp流消息处理
bool Udp_Common_Recv_Stream(uint32 u4ConnectID, ACE_Message_Block* pMbData, shared_ptr<CPacketParse> pPacketParse, shared_ptr<_Packet_Parse_Info> pPacketParseInfo);

//提交udp数据到工作线程
bool Udp_Common_Send_WorkThread(CMakePacket& MakePacket, uint32 u4ConnectID, shared_ptr<CPacketParse> pPacketParse, const ACE_INET_Addr& addrRemote, const ACE_INET_Addr& addrLocal, const PSS_Time_Point& tvCheck);

//清理数据缓冲
void Recovery_Common_BuffPacket(bool blDelete, shared_ptr<IBuffPacket> pBuffPacket);

//TCP流消息处理
uint8 Tcp_Common_Recv_Stream(uint32 u4ConnectID, ACE_Message_Block* pMbData, shared_ptr<CPacketParse> pPacketParse, uint32 u4PacketParseInfoID);

//记录日志方法
void Output_Debug_Data(const ACE_Message_Block* pMbData, uint16 u2LogType, const ACE_INET_Addr& addrRemote);

//File_Message的参数
class _File_Message_Param
{
public:
    uint32 m_u4ConnectID          = 0;
    IFileTestManager* m_pFileTest = nullptr;
    bool m_blDelete               = false;
    uint32 m_u4PacketDebugSize    = 0;
    ACE_INET_Addr m_addrRemote;

    _File_Message_Param() = default;
};

//文件压测数据入工作线程
bool Tcp_Common_File_Message(_File_Message_Param const& obj_File_Message_Param, shared_ptr<IBuffPacket> pBuffPacket, const char* pConnectName);

//Tcp_Common_ClientInfo的参数
class _ClientConnectInfo_Param
{
public:
    uint32 m_u4ConnectID         = 0;
    ACE_INET_Addr m_addrRemote;
    uint32 m_u4AllRecvCount      = 0;
    uint32 m_u4AllSendCount      = 0;
    uint32 m_u4AllRecvSize       = 0;
    uint32 m_u4AllSendSize       = 0;
    PSS_Time_Point m_atvConnect;
    uint32 m_u4RecvQueueCount    = 0;

    _ClientConnectInfo_Param() = default;
};

//返回当前Handler的信息
_ClientConnectInfo Tcp_Common_ClientInfo(_ClientConnectInfo_Param const& obj_ClientConnectInfo_Param);

//Tcp_Common_Send_Input_To_Cache的参数
class _Input_To_Cache_Param
{
public:
    uint32 m_u4ConnectID              = 0;
    uint32 m_u4PacketParseInfoID      = 0;
    uint32 m_u4SendMaxBuffSize        = 0;
    EM_SEND_PACKET_PARSE m_emSendType = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    uint16 m_u2CommandID              = 0;
    bool m_blDelete                   = false;

    _Input_To_Cache_Param() = default;
};

//将数据添加入发送缓冲区
bool Tcp_Common_Send_Input_To_Cache(CMakePacket& MakePacket, 
    _Input_To_Cache_Param obj_Input_To_Cache_Param,
    ACE_Message_Block* pBlockMessage, uint32& u4PacketSize,
    shared_ptr<IBuffPacket> pBuffPacket);

//Tcp_Common_Make_Send_Packet的参数
class _Send_Packet_Param
{
public:
    uint32 m_u4ConnectID               = 0;
    EM_SEND_PACKET_PARSE m_emSendType  = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    uint32 m_u4PacketParseInfoID       = 0;
    uint32 m_u4SendMaxBuffSize         = 0;
    uint16 m_u2CommandID               = 0;
    bool m_blDelete                    = false;

    _Send_Packet_Param() = default;
};

//组装发送数据
bool Tcp_Common_Make_Send_Packet(CMakePacket& MakePacket,
    _Send_Packet_Param obj_Send_Packet_Param,
    shared_ptr<IBuffPacket> pBuffPacket,
    ACE_Message_Block* pBlockMessage,
    uint32& u4PacketSize);

//Tcp_Common_Manager_Post_Message的参数
class _Post_Message_Param
{
public:
    uint32 m_u4ConnectID         = 0;
    uint8 m_u1SendType           = 0;
    uint16 m_u2CommandID         = 0;
    uint8 m_u1SendState          = 0;
    bool m_blDelete              = false;
    int m_nMessageID             = 0;
    uint16 m_u2SendQueueMax      = 0;
    uint32 m_u4SendQueuePutTime  = 0;

    _Post_Message_Param() = default;
};

//定时器输出统计日志
void Tcp_Common_Manager_Timeout_CheckInfo(int nActiveConnectCount);

//获得连接姓名
_ClientNameInfo Tcp_Common_ClientNameInfo(uint32 u4ConnectID, const char* pConnectName, const char* pClientIP, uint16 u2ClientPort, bool IsLog);

#endif
