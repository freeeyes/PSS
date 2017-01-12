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
#include "HashTable.h"

using namespace std;

struct _MakePacket
{
	uint8          m_PacketType;     //数据包类型 
	uint32         m_u4ConnectID;    //链接ID
	uint8          m_u1Option;       //操作类型
	CPacketParse*  m_pPacketParse;   //数据包数据指针
	ACE_INET_Addr  m_AddrRemote;     //数据包的来源IP信息
	ACE_INET_Addr  m_AddrListen;     //数据包来源监听IP信息 

	int            m_nHashID;        //对应记录hash的ID

	_MakePacket()
	{
		m_PacketType        = PACKET_TCP;  //0为TCP,1是UDP 默认是TCP
		m_u4ConnectID       = 0;
		m_u1Option          = 0;
		m_pPacketParse      = NULL;
		m_nHashID          = 0;
	}

	void Clear()
	{
		m_PacketType        = PACKET_TCP;  //0为TCP,1是UDP 默认是TCP
		m_u4ConnectID       = 0;
		m_u1Option          = 0;
		m_pPacketParse      = NULL;
	}

	void SetHashID(int nHashID)
	{
		m_nHashID = nHashID;
	}

	int GetHashID()
	{
		return m_nHashID;
	}
};

class CMakePacket
{
public:
	CMakePacket(void);
	~CMakePacket(void);

	bool Init();

	bool PutMessageBlock(uint32 u4ConnectID, _MakePacket* pMakePacket, ACE_Time_Value& tvNow);                 //处理TCP数据包
	bool PutUDPMessageBlock(_MakePacket* pMakePacket, ACE_Time_Value& tvNow);                                  //处理UDP数据包 
	bool PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, ACE_Time_Value& tvNow);                      //发送失败消息回调 

private:
	void SetMessage(_MakePacket* pMakePacket, CMessage* pMessage, ACE_Time_Value& tvNow);                                              //一般数据包消息
	void SetMessageConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow);                                              //用户链接数据包消息
	void SetMessageCDisConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow);                                          //用户断开链接数据包消息
	void SetMessageSDisConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow);                                          //服务器断开链接数据包消息
	void SetMessageSendTimeout(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow);                                          //服务器发送给客户端数据超时消息
	void SetMessageCheckTimeout(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow);                                         //服务器心跳超时消息 
	void SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, CMessage* pMessage, ACE_Time_Value& tvNow);           //服务发送失败回调数据包消息

	bool ProcessMessageBlock(_MakePacket* pMakePacket, ACE_Time_Value& tvNow);                                   //组成队列消息
private:
	ACE_Recursive_Thread_Mutex     m_ThreadWriteLock;
};
typedef ACE_Singleton<CMakePacket, ACE_Null_Mutex> App_MakePacket; 

#endif
