#ifndef _BASEREACTORHANDLER_H
#define _BASEREACTORHANDLER_H

#include "ace/Task_T.h"

#include "define.h"
#include "SendMessage.h"
#include "MessageBlockManager.h"
#include "LoadPacketParse.h"
#include "PacketParsePool.h"
#include "MakePacket.h"
#include "FileTest.h"
#include "SendCacheManager.h"

//所有的handler用到的公共函数
//add by freeeyes


//回收内存
void Recovery_Message(bool blDelete, char*& pMessage);

//Udp_Common_Send_Message的传参
class _Send_Message_Param
{
public:
    uint32 m_u4PacketParseInfoID;
    char*  m_pIP;
    int    m_nPort;
    bool   m_blHead;
    uint16 m_u2CommandID;
    bool   m_blDlete;
    uint32 m_u4Len;

    _Send_Message_Param()
    {
        m_u4PacketParseInfoID = 0;
        m_pIP = NULL;
        m_nPort = 0;
        m_blHead = true;
        m_u2CommandID = 0;
        m_blDlete = false;
        m_u4Len = 0;
    }
};

//udp函数发送数据包合成函数
bool Udp_Common_Send_Message(_Send_Message_Param obj_Send_Message_Param, ACE_INET_Addr& AddrRemote, char*& pMessage, ACE_Message_Block*& pMbData);

//udp处理消息头函数
bool Udp_Common_Recv_Head(ACE_Message_Block* pMBHead, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID, uint32 u4Len);

//udp处理消息体函数
bool Udp_Common_Recv_Body(ACE_Message_Block* pMBBody, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID);

//udp流消息处理
bool Udp_Common_Recv_Stream(ACE_Message_Block* pMbData, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID);

//提交udp数据到工作线程
bool Udp_Common_Send_WorkThread(CPacketParse*& pPacketParse, ACE_INET_Addr addrRemote, ACE_INET_Addr addrLocal, ACE_Time_Value& tvCheck);

//清理数据缓冲
void Recovery_Common_BuffPacket(bool blDelete, IBuffPacket* pBuffPacket);

//错误信息返回工作线程
void Tcp_Common_Send_Message_Error(bool blDelete, IBuffPacket* pBuffPacket);

//TCP流消息处理
uint8 Tcp_Common_Recv_Stream(uint32 u4ConnectID, ACE_Message_Block* pMbData, CPacketParse* pPacketParse, uint32 u4PacketParseInfoID);

//将数据发送入工作线程消息队列
void Send_MakePacket_Queue(uint32 u4ConnectID, uint32 u4PacketParseID, CPacketParse* m_pPacketParse, uint8 u1Option,
                           ACE_INET_Addr& addrRemote, const char* pLocalIP, uint32 u4LocalPort);

//File_Message的参数
class _File_Message_Param
{
public:
    uint32 m_u4ConnectID;
    IFileTestManager* m_pFileTest;
    bool m_blDelete;
    char* m_pPacketDebugData;
    uint32 m_u4PacketDebugSize;
    ACE_INET_Addr m_addrRemote;

    _File_Message_Param()
    {
        m_u4ConnectID       = 0;
        m_pFileTest         = NULL;
        m_blDelete          = false;
        m_pPacketDebugData  = NULL;
        m_u4PacketDebugSize = 0;
    }
};

//文件压测数据入工作线程
bool Tcp_Common_File_Message(_File_Message_Param obj_File_Message_Param, IBuffPacket*& pBuffPacket, const char* pConnectName);

//Tcp_Common_ClientInfo的参数
class _ClientConnectInfo_Param
{
public:
    uint32 m_u4ConnectID;
    ACE_INET_Addr m_addrRemote;
    uint32 m_u4AllRecvCount;
    uint32 m_u4AllSendCount;
    uint32 m_u4AllRecvSize;
    uint32 m_u4AllSendSize;
    ACE_Time_Value m_atvConnect;
    uint32 m_u4RecvQueueCount;
    uint64 m_u8RecvQueueTimeCost;
    uint64 m_u8SendQueueTimeCost;

    _ClientConnectInfo_Param()
    {
        m_u4ConnectID         = 0;
        m_u4AllRecvCount      = 0;
        m_u4AllSendCount      = 0;
        m_u4AllRecvSize       = 0;
        m_u4AllSendSize       = 0;
        m_u4RecvQueueCount    = 0;
        m_u8RecvQueueTimeCost = 0;
        m_u8SendQueueTimeCost = 0;
    }
};

//返回当前Handler的信息
_ClientConnectInfo Tcp_Common_ClientInfo(_ClientConnectInfo_Param obj_ClientConnectInfo_Param);

//将数据添加入发送缓冲区
bool Tcp_Common_Send_Input_To_Cache(uint32 u4ConnectID, uint32 u4PacketParseInfoID, uint32 u4SendMaxBuffSize,
                                    ACE_Message_Block* pBlockMessage, uint8 u1SendType, uint32& u4PacketSize,
                                    uint16 u2CommandID, bool blDelete, IBuffPacket*& pBuffPacket);

//组装发送数据
bool Tcp_Common_Make_Send_Packet(uint32 u4ConnectID, uint8 u1SendType, uint32 u4PacketParseInfoID,
                                 uint32 u4SendMaxBuffSize, IBuffPacket*& pBuffPacket, uint16 u2CommandID,
                                 bool blDelete, ACE_Message_Block* pBlockMessage);

//发送Manager消息队列关闭信息
bool Tcp_Common_CloseConnect_By_Queue(uint32 u4ConnectID, CSendMessagePool& objSendMessagePool,
                                      uint32 u4SendQueuePutTime, ACE_Task<ACE_MT_SYNCH>* pTask);

//发送Manager消息队列发送数据消息
bool Tcp_Common_Manager_Post_Message(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType,
                                     uint16 u2CommandID, uint8 u1SendState, bool blDelete, int nMessageID,
                                     CSendMessagePool& objSendMessagePool, uint16 u2SendQueueMax,
                                     uint32 u4SendQueuePutTime, ACE_Task<ACE_MT_SYNCH>* pTask);

//定时器输出统计日志
void Tcp_Common_Manager_Timeout_CheckInfo(int nActiveConnectCount);

//获得连接姓名
_ClientNameInfo Tcp_Common_ClientNameInfo(uint32 u4ConnectID, const char* pConnectName, const char* pClientIP, int nClientPort, bool IsLog);

//初始化Manager
void  Tcp_Common_Manager_Init(uint16 u2Index, CCommandAccount& objCommandAccount, uint16& u2SendQueueMax, CSendCacheManager& objSendCacheManager);

#endif
