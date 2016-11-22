#include "PacketParsePool.h"

CPacketParsePool::CPacketParsePool()
{
	m_u4CulationIndex = 0;
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

	//初始化HashTable
	int nKeySize = 10;
	size_t nArraySize = (sizeof(_Hash_Table_Cell<CPacketParse>) + nKeySize + sizeof(CPacketParse* )) * u4PacketCount;
	char* pHashBase = new char[nArraySize];
	m_objPacketParseList.Set_Base_Addr(pHashBase, (int)u4PacketCount);
	m_objPacketParseList.Set_Base_Key_Addr(pHashBase + sizeof(_Hash_Table_Cell<CPacketParse>) * u4PacketCount, 
																	nKeySize * u4PacketCount, nKeySize);
	m_objPacketParseList.Set_Base_Value_Addr(pHashBase + (sizeof(_Hash_Table_Cell<CPacketParse>) + nKeySize) * u4PacketCount, 
																	sizeof(CPacketParse* ) * u4PacketCount, sizeof(CPacketParse* ));

	for(int i = 0; i < (int)u4PacketCount; i++)
	{
		CPacketParse* pPacket = new CPacketParse();
		if(NULL != pPacket)
		{
			//设置默认包头长度
			pPacket->SetPacket_Mode(App_MainConfig::instance()->GetPacketParseInfo()->m_u1Type);
			pPacket->SetPacket_Head_Src_Length( App_MainConfig::instance()->GetPacketParseInfo()->m_u4OrgLength);
			//添加到HashPool里面
			char szPacketID[10] = {'\0'};
			sprintf_safe(szPacketID, 10, "%d", i);
			int nHashPos = m_objPacketParseList.Add_Hash_Data(szPacketID, pPacket);
			if(-1 != nHashPos)
			{
				pPacket->SetHashID(nHashPos);
			}
		}
	}
	m_u4CulationIndex = 1;
}

void CPacketParsePool::Close()
{
	//清理所有已存在的指针
	for(int i = 0; i < m_objPacketParseList.Get_Count(); i++)
	{
		CPacketParse* pPacket = m_objPacketParseList.Get_Index(i);
		SAFE_DELETE(pPacket);
	}

	m_objPacketParseList.Close();
	m_u4CulationIndex = 1;
}

int CPacketParsePool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return m_objPacketParseList.Get_Count() - m_objPacketParseList.Get_Used_Count();
}

int CPacketParsePool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return  m_objPacketParseList.Get_Used_Count();
}

CPacketParse* CPacketParsePool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//如果free池中已经没有了，则添加到free池中。
	CPacketParse* pPacket = NULL;

	//在Hash表中弹出一个已使用的数据
	//判断循环指针是否已经找到了尽头，如果是则从0开始继续
	if(m_u4CulationIndex >= (uint32)(m_objPacketParseList.Get_Count() - 1))
	{
		m_u4CulationIndex = 0;
	}

	//第一次寻找，从当前位置往后找
	for(int i = (int)m_u4CulationIndex; i < m_objPacketParseList.Get_Count(); i++)
	{
		pPacket = m_objPacketParseList.Get_Index(i);
		if(NULL != pPacket)
		{
			//已经找到了，返回指针
			int nDelPos = m_objPacketParseList.Set_Index_Clear(i);
			if(-1 == nDelPos)
			{
				OUR_DEBUG((LM_INFO, "[CPacketParsePool::Create]HashID=%d, nPos=%d, nDelPos=%d, (0x%08x).\n", pPacket->GetHashID(), i, nDelPos, pPacket));
			}
			else
			{
				//OUR_DEBUG((LM_INFO, "[CPacketParsePool::Create]HashID=%d, nPos=%d, nDelPos=%d, (0x%08x).\n", pPacket->GetHashID(), i, nDelPos, pHandler));
			}
			m_u4CulationIndex = i;
			pPacket->Init();
			return pPacket;
		}
	}

	//第二次寻找，从0到当前位置
	for(int i = 0; i < (int)m_u4CulationIndex; i++)
	{
		pPacket = m_objPacketParseList.Get_Index(i);
		if(NULL != pPacket)
		{
			//已经找到了，返回指针
			int nDelPos = m_objPacketParseList.Set_Index_Clear(i);
			if(-1 == nDelPos)
			{
				OUR_DEBUG((LM_INFO, "[CPacketParsePool::Create]HashID=%d, nPos=%d, nDelPos=%d, (0x%08x).\n", pPacket->GetHashID(), i, nDelPos, pPacket));
			}
			else
			{
				//OUR_DEBUG((LM_INFO, "[CPacketParsePool::Create]HashID=%d, nPos=%d, nDelPos=%d, (0x%08x).\n", pPacket->GetHashID(), i, nDelPos, pHandler));
			}
			m_u4CulationIndex = i;
			pPacket->Init();
			return pPacket;
		}
	}

	return pPacket;
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

	char szHashID[10] = {'\0'};
	sprintf_safe(szHashID, 10, "%d", pPacketParse->GetHashID());
	//int nPos = m_objHashHandleList.Add_Hash_Data(szHandlerID, pObject);
	int nPos = m_objPacketParseList.Set_Index(pPacketParse->GetHashID(), szHashID, pPacketParse);
	if(-1 == nPos)
	{
		OUR_DEBUG((LM_INFO, "[CPacketParsePool::Delete]HashID=%s(0x%08x) nPos=%d.\n", szHashID, pPacketParse, nPos));
	}
	else
	{
		//OUR_DEBUG((LM_INFO, "[CSendMessagePool::Delete]HashID=%s(0x%08x) nPos=%d.\n", szHashID, pObject, nPos));
	}

	return true;
}
