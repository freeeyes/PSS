#include "PacketParsePool.h"

CPacketParsePool::CPacketParsePool()
{
}

CPacketParsePool::~CPacketParsePool()
{
	OUR_DEBUG((LM_INFO, "[CPacketParsePool::~CPacketParsePool].\n"));
	Close();
	OUR_DEBUG((LM_INFO, "[CPacketParsePool::~CPacketParsePool] End.\n"));
}

void CPacketParsePool::Init(uint32 u4PacketCount)
{
	Close();

	for(int i = 0; i < (int)u4PacketCount; i++)
	{
		CPacketParse* pPacket = new CPacketParse();
		if(NULL != pPacket)
		{
			//添加到Free map里面
			mapPacketParse::iterator f = m_mapPacketFree.find(pPacket);
			if(f == m_mapPacketFree.end())
			{
				m_mapPacketFree.insert(mapPacketParse::value_type(pPacket, pPacket));
			}
		}
	}
}

void CPacketParsePool::Close()
{
	//清理所有已存在的指针
	for(mapPacketParse::iterator itorFreeB = m_mapPacketFree.begin(); itorFreeB != m_mapPacketFree.end(); itorFreeB++)
	{
		CPacketParse* pPacket = (CPacketParse* )itorFreeB->second;
		SAFE_DELETE(pPacket);
	}

	for(mapPacketParse::iterator itorUsedB = m_mapPacketUsed.begin(); itorUsedB != m_mapPacketUsed.end(); itorUsedB++)
	{
		CPacketParse* pPacket = (CPacketParse* )itorUsedB->second;
		OUR_DEBUG((LM_ERROR, "[CPacketParsePool::Close]CPacketParse has used!!memory address[0x%08x].\n", pPacket));
		SAFE_DELETE(pPacket);
	}

	m_mapPacketFree.clear();
	m_mapPacketUsed.clear();
}

int CPacketParsePool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapPacketUsed.size();
}

int CPacketParsePool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapPacketFree.size();
}

CPacketParse* CPacketParsePool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//如果free池中已经没有了，则添加到free池中。
	if(m_mapPacketFree.size() <= 0)
	{
		CPacketParse* pPacket = new CPacketParse();

		if(pPacket != NULL)
		{
			//添加到Free map里面
			mapPacketParse::iterator f = m_mapPacketFree.find(pPacket);
			if(f == m_mapPacketFree.end())
			{
				m_mapPacketFree.insert(mapPacketParse::value_type(pPacket, pPacket));
			}
		}
		else
		{
			return NULL;
		}
	}

	//从free池中拿出一个,放入到used池中
	mapPacketParse::iterator itorFreeB = m_mapPacketFree.begin();
	CPacketParse* pPacket = (CPacketParse* )itorFreeB->second;
	m_mapPacketFree.erase(itorFreeB);
	//添加到used map里面
	mapPacketParse::iterator f = m_mapPacketUsed.find(pPacket);
	if(f == m_mapPacketUsed.end())
	{
		m_mapPacketUsed.insert(mapPacketParse::value_type(pPacket, pPacket));
	}

	pPacket->Init();
	return (CPacketParse* )pPacket;
}

bool CPacketParsePool::Delete(CPacketParse* pPacketParse)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	CPacketParse* pBuff = (CPacketParse* )pPacketParse;
	if(NULL == pBuff)
	{
		return false;
	}

	pPacketParse->Clear();

	mapPacketParse::iterator f = m_mapPacketUsed.find(pBuff);
	if(f != m_mapPacketUsed.end())
	{
		m_mapPacketUsed.erase(f);

		//添加到Free map里面
		mapPacketParse::iterator f = m_mapPacketFree.find(pBuff);
		if(f == m_mapPacketFree.end())
		{
			m_mapPacketFree.insert(mapPacketParse::value_type(pBuff, pBuff));
		}
	}

	return true;
}
