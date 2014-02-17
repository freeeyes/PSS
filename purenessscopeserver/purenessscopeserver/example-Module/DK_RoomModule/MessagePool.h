#ifndef _MESSAGEPOOL_H
#define _MESSAGEPOOL_H

//数据缓冲数据池
//add by freeeyes

#include <map>
#include "ace/Malloc.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Message_Block.h"
#include "ace/Guard_T.h"
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"

#include "define.h"

using namespace std;

#define MAX_BUFF_SIZE 1024   //最大缓冲块长度

//命令结构信息
struct _CommandInfo
{
	uint32 u4OpreationID;   //操作ID
	uint32 u4RoomID;        //房间ID
	uint32 u4PlayerID;      //玩家ID
	uint32 u4CardID;        //卡牌ID
	uint32 u4Data;          //其他数据

	_CommandInfo()
	{
		Clear();
	}

	_CommandInfo& operator = (const _CommandInfo& ar)
	{
		this->u4OpreationID    = ar.u4OpreationID;
		this->u4RoomID         = ar.u4RoomID;
		this->u4PlayerID       = ar.u4PlayerID;
		this->u4CardID         = ar.u4CardID;
		this->u4Data           = ar.u4Data;

		return *this;
	}

	void Clear()
	{
		u4OpreationID = 0;
		u4RoomID      = 0;
		u4PlayerID    = 0;
		u4CardID      = 0;
		u4Data        = 0;
	}
};

//队列中的消息信息结构体
struct _QueueMessage
{
	uint32       m_u4CommandID;        //命令名称
	_CommandInfo m_objData;            //命令具体对象

	_QueueMessage()
	{
		m_u4CommandID = 0;
	}

	void Clear()
	{
		m_u4CommandID = 0;
		m_objData.Clear();
	}
};

//Message对象池
class CMessagePool
{
public:
	CMessagePool();
	~CMessagePool();

	void Init(uint32 u4PacketCount);
	void Close();

	_QueueMessage* Create();
	bool Delete(_QueueMessage* pMakePacket);

	int GetUsedCount();
	int GetFreeCount();

private:
	typedef map<_QueueMessage*, _QueueMessage*> mapMessage;
	mapMessage                  m_mapMessageUsed;                      //已使用的
	mapMessage                  m_mapMessageFree;                      //没有使用的
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //控制多线程锁
};


//Message对象池
class CMessageBlockPool
{
public:
	CMessageBlockPool();
	~CMessageBlockPool();

	void Close();

	ACE_Message_Block* Create(uint32 u4Size);
	bool Delete(ACE_Message_Block* pMakePacket);

private:
	ACE_Allocator * m_pmsgallocator;
	ACE_Allocator * m_pdata_allocator;
	ACE_Allocator * m_pbuff_allocator;

	//ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //控制多线程锁
};

//缓冲结构体
struct _BufferInfo
{
	uint32  m_u4BufferID;
	char    m_szBuff[MAX_BUFF_SIZE];    //缓冲块
	uint32  m_u4WritePos;               //写入点

	_BufferInfo()
	{
		Clear();
	}

	bool SetBuffer(uint32 u4Data)
	{
		if(m_u4WritePos + sizeof(int) >= MAX_BUFF_SIZE)
		{
			return false;
		}
		else
		{
			ACE_OS::memcpy(&m_szBuff + m_u4WritePos, &u4Data, sizeof(int));
			m_u4WritePos += sizeof(int);
			return true;
		}
	}

	bool SetBuffer(const char* pData, uint32 u4Len)
	{
		if(m_u4WritePos + u4Len + sizeof(int) >= MAX_BUFF_SIZE)
		{
			return false;
		}
		else
		{
			ACE_OS::memcpy(&m_szBuff + m_u4WritePos, &u4Len, sizeof(int));
			m_u4WritePos += sizeof(int);
			ACE_OS::memcpy(&m_szBuff + m_u4WritePos, pData, u4Len);
			m_u4WritePos += u4Len;
			return true;
		}
	}

	void Clear()
	{
		m_u4WritePos  = 0;
		m_szBuff[0]   = '\0';
	}
};

//发送数据缓冲池
class CBufferInfoPool
{
public:
	CBufferInfoPool();
	~CBufferInfoPool();

	void Init(uint32 u4Count);
	void Close();
	_BufferInfo* Create();
	int Delete(uint32 u4BufferID);

	int GetUsedCount();
	int GetFreeCount();

	bool SetBufferInfo(uint32 u4BufferID, uint32 u4Data);
	bool SetBufferInfo(uint32 u4BufferID, const char* pData, uint32 u4Len);

private:
	typedef map<uint32, _BufferInfo*> mapMessage;
	mapMessage                  m_mapMessageUsed;                      //已使用的
	mapMessage                  m_mapMessageFree;                      //没有使用的
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //控制多线程锁
};

typedef ACE_Singleton<CBufferInfoPool, ACE_Null_Mutex> App_BufferInfoPool;

#endif
