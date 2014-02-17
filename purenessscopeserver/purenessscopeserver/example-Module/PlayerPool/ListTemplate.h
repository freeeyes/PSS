#ifndef _LISTTEMPLIT_H
#define _LISTTEMPLIT_H

//一个链表类
//Create by freeeyes
//2010-11-02

class CListNode
{
public:
	CListNode()
	{
		m_pData           = NULL;
		m_pNextListNode   = NULL;
		m_pBeforeListNode = NULL;
	};

	~CListNode()
	{
		Close();
	};

	void Close()
	{
		if(NULL != m_pData)
		{
			delete m_pData;
			m_pData = NULL;
		}
	};

	void SetNodeData(void* pData)
	{
		m_pData = pData;
	};

	void SetNodeNext(CListNode* pListNode)
	{
		m_pNextListNode = pListNode;
	}

	void SetNodeBefore(CListNode* pListNode)
	{
		m_pBeforeListNode = pListNode;
	}

	void* GetNodeData()
	{
		return m_pData;
	}

	CListNode* GetNodeNext()
	{
		return m_pNextListNode;
	}

	CListNode* GetNodeBefore()
	{
		return m_pBeforeListNode;
	}

private:
	void* m_pData;
	CListNode* m_pNextListNode;
	CListNode* m_pBeforeListNode;
};

class CLinkList
{
public:
	CLinkList()
	{
		m_pListNode      = NULL;
		m_pFirstListNode = NULL;
		m_pLastListNode  = NULL;
	};

	~CLinkList()
	{
		Close();
	};

	bool Add(CListNode* pListNode, void* pData)
	{
		if(NULL == pListNode)
		{
			return false;
		}

		pListNode->SetNodeData(pData);
		if(NULL == m_pFirstListNode)
		{
			//如果是链表的第一个
			m_pListNode      = pListNode;
			m_pFirstListNode = pListNode;
			m_pLastListNode  = pListNode;
		}
		else
		{
			pListNode->SetNodeBefore(m_pLastListNode);
			m_pLastListNode->SetNodeNext(pListNode);
			m_pLastListNode = pListNode;
		}

		return true;
	};

	bool MoveTop(CListNode* pListNode)
	{
		if(NULL == pListNode)
		{
			return false;
		}

		if(NULL == m_pFirstListNode)
		{
			return false;
		}

		if(pListNode == m_pFirstListNode)
		{
			return true;
		}

		CListNode* pBefore = pListNode->GetNodeBefore();
		CListNode* pNext   = pListNode->GetNodeNext();

		if(NULL == pBefore)
		{
			//如果就是第一个
			return true;
		}

		pBefore->SetNodeNext(pNext);
		if(NULL != pNext)
		{
			pNext->SetNodeBefore(pBefore);
		}

		if(pListNode == m_pLastListNode)
		{
			if(NULL != pBefore)
			{
				m_pLastListNode = pBefore;
			}
		}

		pListNode->SetNodeNext(m_pFirstListNode);
		pListNode->SetNodeBefore(NULL);
		m_pFirstListNode->SetNodeBefore(pListNode);
		m_pFirstListNode = pListNode;
		return true;
	}

	bool DelNode(CListNode* pListNode, bool blDel = true)
	{
		if(NULL == pListNode)
		{
			return false;
		}

		CListNode* pBefore = pListNode->GetNodeBefore();
		CListNode* pNext   = pListNode->GetNodeNext();

		if(pBefore != NULL)
		{
			pBefore->SetNodeNext(pNext);
		}
		else
		{
			m_pFirstListNode = pListNode->GetNodeNext();
			if(NULL != m_pFirstListNode)
			{
				m_pFirstListNode->SetNodeBefore(NULL);
			}
		}

		if(pNext != NULL)
		{
			pNext->SetNodeBefore(pBefore);
		}
		else
		{
			m_pLastListNode = pListNode->GetNodeBefore();
			if(NULL != m_pLastListNode)
			{
				m_pLastListNode->SetNodeNext(NULL);
			}
		}

		if(blDel == true)
		{
			delete pListNode;
		}

		pListNode->SetNodeBefore(NULL);
		pListNode->SetNodeNext(NULL);
		return true;
	}

	CListNode* GetFirst()
	{
		return m_pFirstListNode;
	}

	CListNode* GetLast()
	{
		return m_pLastListNode;
	}

	void Display()
	{
		CListNode* pListNode = NULL;
		if(m_pFirstListNode != NULL)
		{
			pListNode = m_pFirstListNode; 
			while(pListNode)
			{
				CListNode* pNextListNode = pListNode->GetNodeNext();
				printf("..%s..", pListNode->GetNodeData());
				pListNode = pNextListNode;
			}
		}
	}

	CListNode* GetLastNode()
	{
		if(m_pFirstListNode == NULL)
		{
			return NULL;
		}

		CListNode* pListNode = m_pLastListNode;
		CListNode* pBefor = m_pLastListNode->GetNodeBefore();
		if(NULL == pBefor)
		{
			m_pFirstListNode = NULL;
			m_pLastListNode  = NULL;
		}
		else
		{
			pBefor->SetNodeNext(NULL);
			m_pLastListNode = pBefor;
		}

		pListNode->SetNodeBefore(NULL);
		pListNode->SetNodeNext(NULL);
		return pListNode;
	}

	void Close()
	{
		CListNode* pListNode = NULL;
		if(m_pFirstListNode != NULL)
		{
			pListNode = m_pFirstListNode; 
			while(pListNode)
			{
				CListNode* pNextListNode = pListNode->GetNodeNext();
				delete pListNode;
				pListNode = pNextListNode;
			}
		}
	}

private:
	CListNode* m_pListNode;
	CListNode* m_pFirstListNode;
	CListNode* m_pLastListNode;
};

#endif 
