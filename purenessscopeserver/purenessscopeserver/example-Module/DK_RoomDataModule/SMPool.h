#ifndef _SMPOOL_H
#define _SMPOOL_H

#include "SMAccessObject.h"
#include "Serial.h"

#include <map>

using namespace std;

template<typename T>
class CSMPool
{
private:
	//记录每个队列的数据容器
	struct _SMBlock
	{
		unsigned int	m_offT;         //数据对象偏移，这里不能用T*，因为共享内存中不应该存入指针值，应该存入偏移量
		int				m_nID;          //数据当前编号
		bool			m_blUse;        //是否在使用true是正在使用，false是没有使用
		time_t			m_ttUpdateTime; //DS服务器更新完成后回写的信息时间。

		_SMBlock()
		{
			m_offT  = 0;
			m_nID   = 0;
			m_blUse = false;
		}
	};

public:
	CSMPool()
	{
		m_pSMAccessObject = NULL;
	};

	~CSMPool()
	{
		if(m_pSMAccessObject != NULL)
		{
			delete m_pSMAccessObject;
			m_pSMAccessObject = NULL;
		}
	};

	bool Init(SMKey key, int nMaxCount)
	{
		bool blRet = false;

		m_pSMAccessObject = new CSMAccessObject();
		if(NULL == m_pSMAccessObject)
		{
			return false;
		}

		//首先尝试打开，看看是否存在已有，如果不存在，则创建新的。
		blRet = m_pSMAccessObject->Open(key, nMaxCount*sizeof(T), nMaxCount*sizeof(_SMBlock));
		if(false == blRet)
		{
			printf("[Init]Create.\n");
			//如果没有，则新建。
			blRet = m_pSMAccessObject->Create(key, nMaxCount*sizeof(T), nMaxCount*sizeof(_SMBlock));
			if(false == blRet)
			{
				return false;
			}

			//开始初始化数据头类
			m_Serial.Init(m_pSMAccessObject->GetHeadData(), sizeof(_SMBlock), nMaxCount);

			//开始划分共享内存空间
			m_mapSMBlock.clear();
			m_mapFreeSMBlock.clear();
			m_mapUsedSMBlock.clear();
			for(int i = 0; i < nMaxCount; i++)
			{
				_SMBlock* pSMBlock = (_SMBlock* )m_Serial.GetData(i);

				//这里因为_SMBlock是新的，所以这里初始化这个结构
				new (pSMBlock)_SMBlock();
				if(pSMBlock == NULL)
				{
					return false;
				}

				pSMBlock->m_nID = i;
				pSMBlock->m_offT = i * sizeof(T);
				T* pT  = reinterpret_cast<T*>(m_pSMAccessObject->GetData(pSMBlock->m_offT));
				new (pT)T();
				if(NULL == pT)
				{
					return false;
				}
				m_mapSMBlock.insert(typename mapSMBlock::value_type(i, pSMBlock));
				m_mapFreeSMBlock.insert(typename mapFreeSMBlock::value_type(i, pSMBlock));
			}

			m_nMaxCount = nMaxCount;
		}
		else
		{
			printf("[Init]Open.\n");

			//开始初始化数据头类
			m_Serial.Init(m_pSMAccessObject->GetHeadData(), sizeof(_SMBlock), nMaxCount);

			//还原回map对象
			m_mapSMBlock.clear();
			m_mapFreeSMBlock.clear();
			m_mapUsedSMBlock.clear();
			for(int i = 0; i < nMaxCount; i++)
			{
				_SMBlock* pSMBlock = (_SMBlock* )m_Serial.GetData(i);
				if(pSMBlock == NULL)
				{
					return false;
				}

				pSMBlock->m_offT = i * sizeof(T);
				T* pT  = reinterpret_cast<T*>(m_pSMAccessObject->GetData(pSMBlock->m_offT));
				m_mapSMBlock.insert(typename mapSMBlock::value_type(pSMBlock->m_nID, pSMBlock));

				//分类
				if(pSMBlock->m_blUse == true)
				{
					//放入正在使用的列表
					m_mapUsedSMBlock.insert(typename mapUsedSMBlock::value_type(pT, pSMBlock));
				}
				else
				{
					//放入没有使用的列表
					m_mapFreeSMBlock.insert(typename mapFreeSMBlock::value_type(i, pSMBlock));
				}
			}

			m_nMaxCount = nMaxCount;
		}


		return true;
	};

	void Close()
	{
		typename mapSMBlock::iterator b = m_mapSMBlock.begin();
		typename mapSMBlock::iterator e = m_mapSMBlock.end();
		for(b; b != e; b++)
		{
			_SMBlock* pSMBlock = (_SMBlock* )b->second;
			if(pSMBlock != NULL)
			{
				delete pSMBlock;
			}
		}
		m_mapSMBlock.clear();

		m_pSMAccessObject->Destory();
	}

	T* NewObject()
	{
		//在m_mapSMBlock中查找一个空余的对象。
		if(m_mapFreeSMBlock.size() > 0)
		{
			typename mapFreeSMBlock::iterator b = m_mapFreeSMBlock.begin();

			if(b == m_mapFreeSMBlock.end())
			{
				return NULL;
			}

			_SMBlock* pSMBlock = (_SMBlock* )b->second;
			if(NULL == pSMBlock)
			{
				return NULL;
			}
			else
			{
				m_mapFreeSMBlock.erase(b);
				pSMBlock->m_blUse        = true;
				pSMBlock->m_ttUpdateTime = time(NULL);
				T* pT  = reinterpret_cast<T*>(m_pSMAccessObject->GetData(pSMBlock->m_offT));
				m_mapUsedSMBlock.insert(typename mapUsedSMBlock::value_type(pT, pSMBlock));

				return pT;
			}
		}
		else
		{
			return NULL;
		}
	};

	int GetDataPos(T* pT)
	{
		//根据地址偏移获得当前数据的相对位置 
		char* pData = m_pSMAccessObject->GetHeadData() + m_pSMAccessObject->GetHeadDataSize();
		int nPos = (int)(((char* )pT - pData)/sizeof(T));
		if(nPos < m_nMaxCount && nPos >= 0)
		{
			return nPos;
		}
		else
		{
			return -1;
		}
	}

	T* GetPosToData(int nPos)
	{
		//根据一个偏移地址获得一个指定的T*
		if(nPos < m_nMaxCount && nPos >= 0)
		{
			T* pT = (T*)(m_pSMAccessObject->GetHeadData() + m_pSMAccessObject->GetHeadDataSize() + nPos*sizeof(T)); 
			return pT;
		}
		else
		{
			return NULL;
		}
	}

	bool DeleteObject(T* pData)
	{
		typename mapUsedSMBlock::iterator f = m_mapUsedSMBlock.find(pData);
		if(f == m_mapUsedSMBlock.end())
		{
			return false;
		}
		else
		{
			_SMBlock* pSMBlock = (_SMBlock* )f->second;
			m_mapUsedSMBlock.erase(f);
			pSMBlock->m_blUse = false;
			m_mapFreeSMBlock.insert(typename mapFreeSMBlock::value_type(pSMBlock->m_nID, pSMBlock));

			return true;
		}
	};

	int GetFreeObjectCount()
	{
		return (int)m_mapFreeSMBlock.size();
	}

	int GetUsedObjectCount()
	{
		return (int)m_mapUsedSMBlock.size();
	};

	T* GetUsedObject(int nIndex)
	{
		if(nIndex >= (int)m_mapUsedSMBlock.size())
		{
			return NULL;
		}

		typename mapUsedSMBlock::iterator b = m_mapUsedSMBlock.begin();
		typename mapUsedSMBlock::iterator e = m_mapUsedSMBlock.end();

		int nPos = 0;
		for(b; b != e; b++)
		{
			if(nPos == nIndex)
			{
				_SMBlock* pSMBlock = (_SMBlock* )b->second;
				if(NULL != pSMBlock)
				{
					T* pT  = reinterpret_cast<T*>(m_pSMAccessObject->GetData(pSMBlock->m_offT));
					return pT;
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				if(nPos > nIndex)
				{
					return NULL;
				}
				nPos++;
			}
		}

		return NULL;
	};

	const time_t GetObjectHeadTimeStamp(T* pData)
	{
		typename mapUsedSMBlock::iterator f = m_mapUsedSMBlock.find(pData);
		if(f == m_mapUsedSMBlock.end())
		{
			return NULL;
		}
		else
		{
			_SMBlock* pSMBlock = (_SMBlock* )f->second;
			return (const time_t)pSMBlock->m_ttUpdateTime;
		}
	};

	bool SetObjectHeadTimeStamp(T* pData)
	{
		typename mapUsedSMBlock::iterator f = m_mapUsedSMBlock.find(pData);
		if(f == m_mapUsedSMBlock.end())
		{
			return false;
		}
		else
		{
			_SMBlock* pSMBlock = (_SMBlock* )f->second;
			pSMBlock->m_ttUpdateTime = time(NULL);
			return true;
		}
	}

private:
	typedef map<int, _SMBlock*> mapSMBlock;
	typedef map<T*, _SMBlock*>  mapUsedSMBlock;
	typedef map<int, _SMBlock*> mapFreeSMBlock;

private:
	CSMAccessObject* m_pSMAccessObject; //调用对共享内存的接口数据
	mapSMBlock       m_mapSMBlock;      //当前已存在的内存块单元
	mapUsedSMBlock   m_mapUsedSMBlock;  //正在使用的内存块单元
	mapFreeSMBlock   m_mapFreeSMBlock;  //目前尚未使用的内存块单元
	CSerial          m_Serial;          //序列化的类，用于存放所有内存块的地址，并把这样的结构存在共享内存中
	int              m_nMaxCount;       //当前池中最大内存块数量
};

#endif
