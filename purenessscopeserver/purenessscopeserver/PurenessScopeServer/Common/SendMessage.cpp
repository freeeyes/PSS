#include "SendMessage.h"

CSendMessagePool::CSendMessagePool(void)
{
	m_u4CulationIndex = 0;
}

CSendMessagePool::~CSendMessagePool(void)
{
	OUR_DEBUG((LM_INFO, "[CSendMessagePool::~CSendMessagePool].\n"));
	Close();
	OUR_DEBUG((LM_INFO, "[CSendMessagePool::~CSendMessagePool] End.\n"));
}

void CSendMessagePool::Init(int nObjcetCount)
{
	Close();

	//初始化HashTable
	size_t nArraySize = (sizeof(_Hash_Table_Cell<_SendMessage>)) * nObjcetCount;
	char* pHashBase = new char[nArraySize];
	m_objHashHandleList.Set_Base_Addr(pHashBase, (int)nObjcetCount);

	for(int i = 0; i < nObjcetCount; i++)
	{
		_SendMessage* pMessage = new _SendMessage();
		if(NULL != pMessage)
		{
			//添加到hash数组里面
			char szMessageID[10] = {'\0'};
			sprintf_safe(szMessageID, 10, "%d", i);
			int nHashPos = m_objHashHandleList.Add_Hash_Data(szMessageID, pMessage);
			if(-1 != nHashPos)
			{
				pMessage->SetHashID(nHashPos);
			}
		}
	}
	m_u4CulationIndex = 1;
}

void CSendMessagePool::Close()
{
	//清理所有已存在的指针
	for(int i = 0; i < m_objHashHandleList.Get_Count(); i++)
	{
		_SendMessage* pMessage = m_objHashHandleList.Get_Index(i);
		SAFE_DELETE(pMessage);
	}

	m_objHashHandleList.Close();
	m_u4CulationIndex = 1;
}

_SendMessage* CSendMessagePool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	_SendMessage* pMessage = NULL;

	//在Hash表中弹出一个已使用的数据
	//判断循环指针是否已经找到了尽头，如果是则从0开始继续
	if(m_u4CulationIndex >= (uint32)(m_objHashHandleList.Get_Count() - 1))
	{
		m_u4CulationIndex = 0;
	}

	//第一次寻找，从当前位置往后找
	for(int i = (int)m_u4CulationIndex; i < m_objHashHandleList.Get_Count(); i++)
	{
		pMessage = m_objHashHandleList.Get_Index(i);
		if(NULL != pMessage)
		{
			//已经找到了，返回指针
			int nDelPos = m_objHashHandleList.Set_Index_Clear(i);
			if(-1 == nDelPos)
			{
				OUR_DEBUG((LM_INFO, "[CSendMessagePool::Create]HashID=%d, nPos=%d, nDelPos=%d, (0x%08x).\n", pMessage->GetHashID(), i, nDelPos, pMessage));
			}
			else
			{
				//OUR_DEBUG((LM_INFO, "[CSendMessagePool::Create]szHandlerID=%s, nPos=%d, nDelPos=%d, (0x%08x).\n", szHandlerID, i, nDelPos, pHandler));
			}
			m_u4CulationIndex = i;
			return pMessage;
		}
	}

	//第二次寻找，从0到当前位置
	for(int i = 0; i < (int)m_u4CulationIndex; i++)
	{
		pMessage = m_objHashHandleList.Get_Index(i);
		if(NULL != pMessage)
		{
			//已经找到了，返回指针
			int nDelPos = m_objHashHandleList.Set_Index_Clear(i);
			if(-1 == nDelPos)
			{
				OUR_DEBUG((LM_INFO, "[CSendMessagePool::Create]HashID=%d, nPos=%d, nDelPos=%d, (0x%08x).\n", pMessage->GetHashID(), i, nDelPos, pMessage));
			}
			else
			{
				//OUR_DEBUG((LM_INFO, "[CSendMessagePool::Create]szHandlerID=%s, nPos=%d, nDelPos=%d, (0x%08x).\n", szHandlerID, i, nDelPos, pHandler));
			}
			m_u4CulationIndex = i;
			return pMessage;
		}
	}

	//没找到空余的
	return pMessage;
}

bool CSendMessagePool::Delete(_SendMessage* pObject)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	if(NULL == pObject)
	{
		return false;
	}

	char szHashID[10] = {'\0'};
	sprintf_safe(szHashID, 10, "%d", pObject->GetHashID());
	//int nPos = m_objHashHandleList.Add_Hash_Data(szHandlerID, pObject);
	int nPos = m_objHashHandleList.Set_Index(pObject->GetHashID(), szHashID, pObject);
	if(-1 == nPos)
	{
		OUR_DEBUG((LM_INFO, "[CSendMessagePool::Delete]HashID=%s(0x%08x) nPos=%d.\n", szHashID, pObject, nPos));
	}
	else
	{
		//OUR_DEBUG((LM_INFO, "[CSendMessagePool::Delete]HashID=%s(0x%08x) nPos=%d.\n", szHashID, pObject, nPos));
	}

	return true;
}

int CSendMessagePool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return m_objHashHandleList.Get_Count() - m_objHashHandleList.Get_Used_Count();
}

int CSendMessagePool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return m_objHashHandleList.Get_Used_Count();
}


