#ifndef MAKEPACKET_H
#define MAKEPACKET_H

//专门处理数据解包操作
//把数据包的拼接和解包放在一起
//add by freeeyes
//2010-06-11
#pragma once

#include "define.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "PacketParsePool.h"
#include "MessageService.h"
#include "ProfileTime.h"

#include <map>

using namespace std;

struct _MakePacket
{
	uint8          m_PacketType;     //数据包类型 
	uint32         m_u4ConnectID;    //链接ID
	uint8          m_u1Option;       //操作类型
	CPacketParse*  m_pPacketParse;   //数据包数据指针
	ACE_INET_Addr  m_AddrRemote;     //数据包的来源IP信息

	_MakePacket()
	{
		m_PacketType        = PACKET_TCP;  //0为TCP,1是UDP 默认是TCP
		m_u4ConnectID       = 0;
		m_u1Option          = 0;
		m_pPacketParse      = NULL;
	}

	void Clear()
	{
		m_PacketType        = PACKET_TCP;  //0为TCP,1是UDP 默认是TCP
		m_u4ConnectID       = 0;
		m_u1Option          = 0;
		m_pPacketParse      = NULL;
	}
};

//_MakePacket对象池，循环使用，不new数据。提升生成速度，应用于CMakePacket
class CMakePacketPool
{
public:
	CMakePacketPool();
	~CMakePacketPool();

	void Init(uint32 u4PacketCount);
	void Close();

	_MakePacket* Create();
	bool Delete(_MakePacket* pMakePacket);

	int GetUsedCount();
	int GetFreeCount();

private:
	typedef map<_MakePacket*, _MakePacket*> mapPacket;
	mapPacket                  m_mapPacketUsed;                       //已使用的
	mapPacket                  m_mapPacketFree;                       //没有使用的
	ACE_Recursive_Thread_Mutex m_ThreadWriteLock;                     //控制多线程锁
};

class CMakePacket
{
public:
	CMakePacket(void);
	~CMakePacket(void);

	bool Init();

	bool PutMessageBlock(uint32 u4ConnectID, uint8 u1Option, CPacketParse* pPacketParse);                        //处理TCP数据包
	bool PutUDPMessageBlock(const ACE_INET_Addr& AddrRemote, uint8 u1Option, CPacketParse* pPacketParse);        //处理UDP数据包 
	bool ProcessMessageBlock(_MakePacket* pMakePacket);                                                          //组成队列消息
	bool PutSebdErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage);                               //发送失败消息回调 

private:
	CMessage* SetMessage(CPacketParse* pPacketParse, uint32 u4ConnectID);                                         //一般数据包消息(TCP)
	CMessage* SetMessage(CPacketParse* pPacketParse, const ACE_INET_Addr& AddrRemote);                            //一般数据包消息(UDP)
	CMessage* SetMessageConnect(uint32 u4ConnectID);                                                              //用户链接数据包消息
	CMessage* SetMessageCDisConnect(uint32 u4ConnectID);                                                          //用户断开链接数据包消息
	CMessage* SetMessageSDisConnect(uint32 u4ConnectID);                                                          //服务器断开链接数据包消息
	CMessage* SetMessageSendTimeout(uint32 u4ConnectID);                                                          //服务器发送给客户端数据超时消息
	CMessage* SetMessageCheckTimeout(uint32 u4ConnectID);                                                         //服务器心跳超时消息 
	CMessage* SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage);                           //服务发送失败回调数据包消息

private:
	CMakePacketPool                m_MakePacketPool;
	ACE_Recursive_Thread_Mutex     m_ThreadWriteLock;
};
typedef ACE_Singleton<CMakePacket, ACE_Null_Mutex> App_MakePacket; 

#endif
