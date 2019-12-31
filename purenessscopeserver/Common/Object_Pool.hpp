#pragma once
#include "define.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <assert.h>
#include "ACEMemory.hpp"
#include "ThreadLock.h"

/*
说明:	
CObjectPool<T>为对象池类 生成和管理所有除基础类型的对象.
CObjectPool_Factory为对象池工厂,单件类,生成和管理所有的CObjectPool<T>类型

1.可以设置固定长度,一旦设置后对象池数量不可变.
2.固定长度为0,可以自动增长长度,并且可以使用GC回收.多余分配的对象.

by liuruiqi 2019.6.20
*/

// 对象池基类接口，提供了gc垃圾收集的能力
class IObjectPool
{
public:
	// 垃圾收集
	virtual void GC(bool bEnforce) = 0;
	virtual ~IObjectPool() {}
};

// 对象池的实体类，通过模板实现通用化
template<class ObjectType>
class CObjectPool : public IObjectPool
{
private:
	typedef std::unordered_map<int, ObjectType*>  FreePointer;
	typedef std::vector<ObjectType*>   FreeIndex;
public:
	CObjectPool()
	{
		m_isFixed = false;
	}

	~CObjectPool()
	{
		GC(true);
	}

	//设置固定长度uFixedLength的值
	//uFixedLength等于0,内存对象可以自动增长调用GC可回收,否则为固定长度GC无用
	void Init(uint32 uFixedLength = 0)
	{
		CAutoLock lock(&m_lock);
		if (0 == uFixedLength)
		{
			//按64作为第一次增长,之后每次增加一倍
			m_growSize = 64;
			m_CurrentObject = 0;
			Grow();
			m_isFixed = false;
		}
		else
		{
			m_growSize = uFixedLength;
			Grow();
			m_isFixed = true;
		}

		SetFixedLength(uFixedLength);
	}

	bool isFiexdPool()
	{
		return m_isFixed;
	}

	uint32 GetCurrentObjects()
	{
		return m_CurrentObject;
	}

	//无参构造
	ObjectType* Construct()
	{
		CAutoLock lock(&m_lock);

		auto pData = GetFreePointer();
		if (pData == nullptr)
		{
			//记录日志
			OUR_DEBUG((LM_ERROR, "[CObjectPool::Construct()] pData == nullptr (%s).(%d)\n", __FILE__, __LINE__));
			assert(pData);
			return nullptr;
		}

		ObjectType * const ret = new (pData)ObjectType();

		return ret;
	}

	//带参构造
	template<class ... Args>
	ObjectType* Construct(Args && ... args)
	{
		CAutoLock lock(&m_lock);

		ObjectType* pData = GetFreePointer();
		if (pData == nullptr)
		{
			//记录日志
			OUR_DEBUG((LM_ERROR, "[CObjectPool::Construct(Args && ... args)] pData == nullptr (%s).(%d)\n", __FILE__, __LINE__));
			assert(pData);
			return nullptr;
		}

		//调用模板的构造函数
		ObjectType* const ret = new (pData) ObjectType(std::forward<Args>(args)...);

		return ret;
	}

	// 销毁一个对象
	void Destroy(ObjectType* const object)
	{
		CAutoLock lock(&m_lock);

		object->~ObjectType();
		m_FreeIndexs.push_back(object);
	}

	//内存回收(true为真释放内存,false为回收地址)
	void GC(bool bEnforce = false)
	{
		CAutoLock lock(&m_lock);
		
		ObjectType* object = nullptr;
		ObjectType* pData = nullptr;

		// 构造一个map来使查找m_FreeIndexs更加快捷一些
		std::unordered_map<ObjectType*, bool> findexs;
		{
			for (auto it : m_FreeIndexs)
			{
				findexs.insert(std::make_pair(it, true));
			}
		}

		//回收内存
		std::vector<int> deleteList;
		deleteList.clear();

		bool bCanGC = false;
		
		auto it = m_FreePointerIndexs.begin(), itEnd = m_FreePointerIndexs.end();
		for (; it != itEnd; ++it)
		{
			// 查找是否可以回收[即自己的指针是否全部在m_FreeIndexs，有一个不在则已分配至少一份出去，不可回收]
			bCanGC = true;
			for (int i = 0; i < it->first; ++i)
			{
				pData = it->second + i;
				if (findexs.find(pData) == findexs.end())
				{
					//如果bEnforce = false;
					//则只是匹配数据
					if (!bEnforce)
					{
						bCanGC = false;
						break;
					}
					else
					{
						// 强制回收则都要回收掉
						object = (ObjectType*)pData;
						object->~ObjectType();
					}
				}
			}
			// 可以回收
			if (bCanGC)
			{
				//grow()最后增加的,那么减少的时候应该最先减少.
				m_growSize /= 2;
				// 回收空闲索引
				for (int i = 0; i < it->first; ++i)
				{
					pData = it->second + i;
					findexs.erase(pData);
				}
				// 回收指针
				App_ACEMemory::instance()->free(static_cast<void *>(it->second));
				// 删除该key
				deleteList.push_back(m_growSize);

				//记录当前个数
				m_CurrentObject -= m_growSize;
			}
		}

		// 写回空闲索引
		m_FreeIndexs.clear();
		for (auto it : findexs)
		{
			m_FreeIndexs.push_back(it.first);
		}
		// 真正删除
		size_t size = deleteList.size();
		for (size_t i = 0; i < size; ++i)
		{
			m_FreePointerIndexs.erase(deleteList[i]);
		}
	}

private:
	//设置固定长度
	void SetFixedLength(uint32 uFixedLength)
	{
		CAutoLock lock(&m_lock);
		m_uFixedLength = uFixedLength;
	}

	uint32 GetFixedLength()
	{
		return m_uFixedLength;
	}

	void Grow()
	{
		//当前个数大于最大数量,不给申请.直接返回.
		//固定长度已经设置,不能再次分配大小
		if (isFiexdPool())
		{
			return;
		}
		
		int objectSize = sizeof(ObjectType);

		ObjectType* pData = static_cast<ObjectType*>(App_ACEMemory::instance()->malloc(m_growSize * objectSize));
		if (pData == NULL) 
			return;
		// 加入指针map中
		m_FreePointerIndexs.insert(std::make_pair(m_growSize, pData));
		// 加入空闲索引中
		for (uint32 i = 0; i < m_growSize; ++i)
		{ 
			m_FreeIndexs.push_back(pData + i);
		}

		//记录当前个数
		m_CurrentObject += m_growSize;
		//最后一步增加 下次增长一倍
		m_growSize *= 2;
	}

	ObjectType* GetFreePointer()
	{
		if (m_FreeIndexs.empty())
			Grow();
		if (m_FreeIndexs.empty())
			return NULL;
		ObjectType* pData = m_FreeIndexs.back();
		m_FreeIndexs.pop_back();
		return pData;
	}
private:
	FreePointer  m_FreePointerIndexs;// 空闲指针索引map,key为growSize
	FreeIndex    m_FreeIndexs;       // 空闲索引列表
	uint32       m_growSize;         // 内存增长的大小
	uint32		m_CurrentObject;		//当前对象个数
	uint32		m_uFixedLength;			//固定长度
	bool		m_isFixed;				//固定增长判断true为是
	CThreadLock m_lock;
};

// 对象池工厂
template<class ObjectType>
class CObjectPool_Factory
{
protected:
	CObjectPool_Factory(){}
public:
	~CObjectPool_Factory(){}

	// 获得单例
	static CObjectPool_Factory& GetSingleton()
	{
		static CObjectPool_Factory poolFactory;
		return poolFactory;
	}

	// 获得ObjectPool
	CObjectPool<ObjectType>* GetObjectPool(const std::string& name)
	{
		CAutoLock lock(&m_lock);

		CObjectPool<ObjectType>* pool = nullptr;
		auto it = m_poolMap.find(name);
		if (it == m_poolMap.end())
		{
			pool = new CObjectPool<ObjectType>();
			m_poolMap.insert(std::make_pair(name, pool));
		}
		else
		{
			pool = (CObjectPool<ObjectType>*)it->second;
		}
		return pool;
	}

	// 全体gc
	void GC()
	{
		CAutoLock lock(&m_lock);

		for (auto it : m_poolMap)
		{
			it.second->GC(false);
		}
	}

private:
	typedef std::unordered_map<std::string, CObjectPool<ObjectType>* > PoolMap;
	PoolMap m_poolMap;
	CThreadLock m_lock;
};

// 对象池指针定义
#define DefineObjectPoolPtr(T, pPool) CObjectPool<T>* pPool
// 获得特定对象池指针。
#define GetObjectPoolPtr(T) CObjectPool_Factory<T>::GetSingleton().GetObjectPool(#T)
// 直接定义对象池
#define ObjectPoolPtr(T, pPool) DefineObjectPoolPtr(T, pPool) = GetObjectPoolPtr(T)