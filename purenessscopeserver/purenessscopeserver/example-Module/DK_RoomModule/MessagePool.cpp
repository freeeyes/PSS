#include "MessagePool.h"

CMessagePool::CMessagePool()
{
}

CMessagePool::~CMessagePool()
{
	Close();
}

void CMessagePool::Init(uint32 u4PacketCount)
{
	Close();

	for(int i = 0; i < (int)u4PacketCount; i++)
	{
		_QueueMessage* pPacket = new _QueueMessage();
		if(NULL != pPacket)
		{
			//添加到Free map里面
			mapMessage::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				m_mapMessageFree.insert(mapMessage::value_type(pPacket, pPacket));
			}
		}
	}
}

void CMessagePool::Close()
{
	//清理所有已存在的指针
	for(mapMessage::iterator itorFreeB = m_mapMessageFree.begin(); itorFreeB != m_mapMessageFree.end(); itorFreeB++)
	{
		_QueueMessage* pPacket = (_QueueMessage* )itorFreeB->second;
		SAFE_DELETE(pPacket);
	}

	for(mapMessage::iterator itorUsedB = m_mapMessageUsed.begin(); itorUsedB != m_mapMessageUsed.end(); itorUsedB++)
	{
		_QueueMessage* pPacket = (_QueueMessage* )itorUsedB->second;
		SAFE_DELETE(pPacket);
	}

	m_mapMessageFree.clear();
	m_mapMessageUsed.clear();
}

int CMessagePool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageUsed.size();
}

int CMessagePool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageFree.size();
}

_QueueMessage* CMessagePool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//如果free池中已经没有了，则添加到free池中。
	if(m_mapMessageFree.size() <= 0)
	{
		_QueueMessage* pPacket = new _QueueMessage();

		if(pPacket != NULL)
		{
			//添加到Free map里面
			mapMessage::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				m_mapMessageFree.insert(mapMessage::value_type(pPacket, pPacket));
			}
		}
		else
		{
			return NULL;
		}
	}

	//从free池中拿出一个,放入到used池中
	mapMessage::iterator itorFreeB = m_mapMessageFree.begin();
	_QueueMessage* pPacket = (_QueueMessage* )itorFreeB->second;
	m_mapMessageFree.erase(itorFreeB);
	//添加到used map里面
	mapMessage::iterator f = m_mapMessageUsed.find(pPacket);
	if(f == m_mapMessageUsed.end())
	{
		m_mapMessageUsed.insert(mapMessage::value_type(pPacket, pPacket));
	}

	return (_QueueMessage* )pPacket;
}

bool CMessagePool::Delete(_QueueMessage* pBuffPacket)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	_QueueMessage* pBuff = (_QueueMessage* )pBuffPacket;
	if(NULL == pBuff)
	{
		return false;
	}

	pBuffPacket->Clear();

	mapMessage::iterator f = m_mapMessageUsed.find(pBuff);
	if(f != m_mapMessageUsed.end())
	{
		m_mapMessageUsed.erase(f);

		//添加到Free map里面
		mapMessage::iterator f = m_mapMessageFree.find(pBuff);
		if(f == m_mapMessageFree.end())
		{
			m_mapMessageFree.insert(mapMessage::value_type(pBuff, pBuff));
		}
	}

	return true;
}


CMessageBlockPool::CMessageBlockPool()
{
}

CMessageBlockPool::~CMessageBlockPool()
{
	Close();
}

void CMessageBlockPool::Close()
{

}

ACE_Message_Block* CMessageBlockPool::Create(uint32 u4Size)
{
	ACE_Message_Block* pmb = NULL;

	ACE_NEW_MALLOC_NORETURN(pmb, 
		static_cast<ACE_Message_Block*>(m_pmsgallocator->malloc(sizeof(ACE_Message_Block))),
		ACE_Message_Block(u4Size, // size
		ACE_Message_Block::MB_DATA, // type
		0,
		0,
		m_pbuff_allocator, // allocator_strategy
		0, // locking strategy
		ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, // priority
		ACE_Time_Value::zero,
		ACE_Time_Value::max_time,
		m_pdata_allocator,
		m_pmsgallocator
		));

	return pmb;
}

bool CMessageBlockPool::Delete(ACE_Message_Block* pBuffPacket)
{
	if(pBuffPacket != NULL)
	{
		pBuffPacket->release();
	}
	return true;
}

CBufferInfoPool::CBufferInfoPool()
{

}

CBufferInfoPool::~CBufferInfoPool()
{
	Close();
}

void CBufferInfoPool::Init(uint32 u4PacketCount)
{
	Close();

	for(int i = 0; i < (int)u4PacketCount; i++)
	{
		_BufferInfo* pPacket = new _BufferInfo();
		if(NULL != pPacket)
		{
			pPacket->m_u4BufferID = i;
			//添加到Free map里面
			mapMessage::iterator f = m_mapMessageFree.find(i);
			if(f == m_mapMessageFree.end())
			{
				m_mapMessageFree.insert(mapMessage::value_type(i, pPacket));
			}
		}
	}
}

void CBufferInfoPool::Close()
{
	//清理所有已存在的指针
	for(mapMessage::iterator itorFreeB = m_mapMessageFree.begin(); itorFreeB != m_mapMessageFree.end(); itorFreeB++)
	{
		_BufferInfo* pPacket = (_BufferInfo* )itorFreeB->second;
		SAFE_DELETE(pPacket);
	}

	for(mapMessage::iterator itorUsedB = m_mapMessageUsed.begin(); itorUsedB != m_mapMessageUsed.end(); itorUsedB++)
	{
		_BufferInfo* pPacket = (_BufferInfo* )itorUsedB->second;
		SAFE_DELETE(pPacket);
	}

	m_mapMessageFree.clear();
	m_mapMessageUsed.clear();
}

int CBufferInfoPool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageUsed.size();
}

int CBufferInfoPool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageFree.size();
}

_BufferInfo* CBufferInfoPool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//如果free池中已经没有了，则返回NNULL
	if(m_mapMessageFree.size() <= 0)
	{
		return NULL;
	}

	//从free池中拿出一个,放入到used池中
	mapMessage::iterator itorFreeB = m_mapMessageFree.begin();
	_BufferInfo* pPacket = (_BufferInfo* )itorFreeB->second;
	m_mapMessageFree.erase(itorFreeB);
	//添加到used map里面
	mapMessage::iterator f = m_mapMessageUsed.find(pPacket->m_u4BufferID);
	if(f == m_mapMessageUsed.end())
	{
		m_mapMessageUsed.insert(mapMessage::value_type(pPacket->m_u4BufferID, pPacket));
	}

	return (_BufferInfo* )pPacket;
}

int CBufferInfoPool::Delete(uint32 u4BufferID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	mapMessage::iterator f = m_mapMessageUsed.find(u4BufferID);
	if(f != m_mapMessageUsed.end())
	{
		_BufferInfo* pPacket = (_BufferInfo* )f->second;
		pPacket->Clear();
		m_mapMessageUsed.erase(f);

		//添加到Free map里面
		mapMessage::iterator f = m_mapMessageFree.find(pPacket->m_u4BufferID);
		if(f == m_mapMessageFree.end())
		{
			m_mapMessageFree.insert(mapMessage::value_type(pPacket->m_u4BufferID, pPacket));
		}
	}

	return 0;
}

bool CBufferInfoPool::SetBufferInfo(uint32 u4BufferID, uint32 u4Data)
{
	mapMessage::iterator f = m_mapMessageUsed.find(u4BufferID);
	if(f != m_mapMessageUsed.end())
	{
		_BufferInfo* pPacket = (_BufferInfo* )f->second;
		if(NULL == pPacket)
		{
			return false;
		}
		else
		{
			return pPacket->SetBuffer(u4Data);
		}
	}

	return false;
}

bool CBufferInfoPool::SetBufferInfo(uint32 u4BufferID, const char* pData, uint32 u4Len)
{
	mapMessage::iterator f = m_mapMessageUsed.find(u4BufferID);
	if(f != m_mapMessageUsed.end())
	{
		_BufferInfo* pPacket = (_BufferInfo* )f->second;
		if(NULL == pPacket)
		{
			return false;
		}
		else
		{
			return pPacket->SetBuffer(pData, u4Len);
		}
	}

	return false;
}
