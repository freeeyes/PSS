#ifndef _SENDMESSAGE_H
#define _SENDMESSAGE_H

#include "define.h"
#include "IBuffPacket.h"
#include "HashTable.h"

using namespace std;

//定义一个发送数据容器，用于异步发送队列
struct _SendMessage
{
	uint32              m_u4ConnectID;    //要发送的远程ID
	uint16              m_u2CommandID;    //要发送的命令ID，用于统计功能
	uint8               m_u1SendState;    //要发送的状态，0是立即发送，1是先缓存不发送，2是立即发送后关闭
	bool                m_blDelete;       //发送完成后是否删除，true是删除，false是不删除
	IBuffPacket*        m_pBuffPacket;    //数据包内容
	uint8               m_nEvents;        //发送类型，0：使用PacketParse组织发送数据，1：不使用PacketParse组织数据
	ACE_Time_Value      m_tvSend;         //数据包发送的时间戳

	ACE_Message_Block*  m_pmbQueuePtr;    //消息队列指针块

	int                 m_nHashID;        //当前对象的HashID

	_SendMessage()
	{
		m_u1SendState = 0;
		m_blDelete    = true;
		m_u4ConnectID = 0;
		m_nEvents     = 0;
		m_u2CommandID = 0;
		m_nHashID     = 0;

		//这里设置消息队列模块指针内容，这样就不必反复的new和delete，提升性能
		//指针关系也可以在这里直接指定，不必使用的使用再指定
		m_pmbQueuePtr  = new ACE_Message_Block(sizeof(_SendMessage*));

		_SendMessage** ppMessage = (_SendMessage**)m_pmbQueuePtr->base();
		*ppMessage = this;

	}

	~_SendMessage()
	{
		if(NULL != m_pmbQueuePtr)
		{
			m_pmbQueuePtr->release();
			m_pmbQueuePtr = NULL;
		}
	}

	ACE_Message_Block* GetQueueMessage()
	{
		return m_pmbQueuePtr;
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

class CSendMessagePool
{
public:
	CSendMessagePool(void);
	~CSendMessagePool(void);

	void Init(int nObjcetCount = MAX_MSG_THREADQUEUE);
	void Close();

	_SendMessage* Create();
	bool Delete(_SendMessage* pObject);

	int GetUsedCount();
	int GetFreeCount();

private:
	CHashTable<_SendMessage>    m_objHashHandleList;
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //控制多线程锁
};
#endif

