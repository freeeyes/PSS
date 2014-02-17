#ifndef _MAPTEMPLATE_H
#define _MAPTEMPLATE_H

#include <map>
#include <vector>

using namespace std;

//负责模板化map的类

template <class Key, class T>
class CMapTemplate
{
private:
	typedef map<Key, T*> mapData;          //定义map结构
	typedef vector<Key>  vecmapInidex;     //定义索引值
	mapData              m_mapData; 
	vecmapInidex         m_vecmapIndex;	
	
public:
	CMapTemplate()
	{
	}

	~CMapTemplate() 
	{
		//Clear();
	}

	//添加数据
	bool AddMapData(Key mapkey, T* pData)
	{
		typename mapData::iterator f = m_mapData.find(mapkey);

		if(f != m_mapData.end())
		{
			return false;
		}

		m_mapData.insert(typename mapData::value_type(mapkey, pData));
		m_vecmapIndex.push_back(mapkey);
		return true;
	}

	//删除数据
	bool DelMapData(Key mapkey)
	{
		typename mapData::iterator f = m_mapData.find(mapkey);

		if(f != m_mapData.end())
		{
			T* pData = (T*)f->second;
			if(pData != NULL)
			{
				delete pData;
			}
			m_mapData.erase(f);

			//删除索引中的数据
			typename vecmapInidex::iterator itrIndex = m_vecmapIndex.begin();
			while(itrIndex != m_vecmapIndex.end())
			{
				if(*itrIndex == mapkey)
				{
					m_vecmapIndex.erase(itrIndex);
					break;
				}
				itrIndex++;
			}
			//m_vecmapIndex.erase((m_vecmapIndex.begin(), m_vecmapIndex.end(), mapkey, m_vecmapIndex.end()));

			return true;
		}
		else
		{
			return false;
		}
	}

	//清除map内的所有数据
	void Clear()
	{
		for(typename mapData::iterator b = m_mapData.begin(); b != m_mapData.end(); b++)
		{
			T* pData = (T*)b->second;
			if(pData != NULL)
			{
				//delete pData;
			}
		}

		m_mapData.clear();
		m_vecmapIndex.clear();
	}

	//查找数据
	T* SearchMapData(Key mapkey)
	{
		typename mapData::iterator f = m_mapData.find(mapkey);

		if(f != m_mapData.end())
		{
			T* pData = (T*)f->second;
			if(pData != NULL)
			{
				return pData;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}

	//得到当前的map的个数
	int GetSize()
	{
		return (int)m_mapData.size();
	}

	//遍历获得数据
	T* GetMapData(int nIndex)
	{
		typename mapData::iterator itrCurr;
		
		if(nIndex >= (int)m_vecmapIndex.size())
		{
			return NULL;
		}
		else
		{
			Key mapkey = m_vecmapIndex[nIndex];
			return SearchMapData(mapkey);
		}
	}

	//遍历获得数据First
	Key GetMapDataKey(int nIndex)
	{
		typename mapData::iterator itrCurr;

		if(nIndex >= (int)m_vecmapIndex.size())
		{
			return 0;
		}
		else
		{
			return m_vecmapIndex[nIndex];
		}
	}
};
#endif
