#include "define.h"

#include <map>

using namespace std;

//这个类是记录所有的websocket连接状态的
//add by freeeyes

//记录所有websocket的连接状态，区分不同状态的连接，是做握手还是处理数据包
enum WEBSOCKET_CONNECT_STATE
{
	WEBSOCKET_STATE_HANDIN = 0,      //需要握手包状态
	WEBSOCKET_STATE_DATAIN,          //需要数据包状态
};

//记录websokcet的连接状态，如果是初次连接，则设置为WEBSOCKET_STATE_HANDIN
struct _WebSocketInfo
{
	uint32                  m_u4ConnectID;
	WEBSOCKET_CONNECT_STATE m_emState;

	_WebSocketInfo()
	{
		m_u4ConnectID = 0;
		m_emState     = WEBSOCKET_STATE_HANDIN;
	}
};

//(暂时不用) 需要的时候可以随时加上，替代new和delete
//声明一个内存管理池,管理所有的_WebSocketInfo指针
class CWebSocketInfoPool
{
public:
	CWebSocketInfoPool(uint32 u4Size = 10000)
	{
		for(uint32 i = 0; i < u4Size; i++)
		{
			_WebSocketInfo* pWebSocketInfo = new _WebSocketInfo();
			if(NULL != pWebSocketInfo)
			{
				//添加到Free map里面
				mapPacket::iterator f = m_mapPacketFree.find(pWebSocketInfo);
				if(f == m_mapPacketFree.end())
				{
					m_mapPacketFree.insert(mapPacket::value_type(pWebSocketInfo, pWebSocketInfo));
				}
			}
		}
	};

	~CWebSocketInfoPool()
	{
		Close();
	};

	_WebSocketInfo* Create()
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

		//如果free池中已经没有了，则添加到free池中。
		if(m_mapPacketFree.size() <= 0)
		{
			_WebSocketInfo* pWebSocketInfo = new _WebSocketInfo();

			if(pWebSocketInfo != NULL)
			{
				//添加到Free map里面
				mapPacket::iterator f = m_mapPacketFree.find(pWebSocketInfo);
				if(f == m_mapPacketFree.end())
				{
					m_mapPacketFree.insert(mapPacket::value_type(pWebSocketInfo, pWebSocketInfo));
				}
			}
			else
			{
				return NULL;
			}
		}

		//从free池中拿出一个,放入到used池中
		mapPacket::iterator itorFreeB = m_mapPacketFree.begin();
		_WebSocketInfo* pWebSocketInfo = (_WebSocketInfo* )itorFreeB->second;
		m_mapPacketFree.erase(itorFreeB);
		//添加到used map里面
		mapPacket::iterator f = m_mapPacketUsed.find(pWebSocketInfo);
		if(f == m_mapPacketUsed.end())
		{
			m_mapPacketUsed.insert(mapPacket::value_type(pWebSocketInfo, pWebSocketInfo));
		}

		return (_WebSocketInfo* )pWebSocketInfo;
	};

	bool Delete(_WebSocketInfo* pWebSocketInfo)
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

		_WebSocketInfo* pBuff = (_WebSocketInfo* )pWebSocketInfo;
		if(NULL == pBuff)
		{
			return false;
		}

		mapPacket::iterator f = m_mapPacketUsed.find(pBuff);
		if(f != m_mapPacketUsed.end())
		{
			m_mapPacketUsed.erase(f);

			//添加到Free map里面
			mapPacket::iterator f = m_mapPacketFree.find(pBuff);
			if(f == m_mapPacketFree.end())
			{
				m_mapPacketFree.insert(mapPacket::value_type(pBuff, pBuff));
			}
		}

		return true;
	};

	void Close()
	{
		//清理所有已存在的指针
		for(mapPacket::iterator itorFreeA = m_mapPacketFree.begin(); itorFreeA != m_mapPacketFree.end(); itorFreeA++)
		{
			_WebSocketInfo* pWebSocketInfo = (_WebSocketInfo* )itorFreeA->second;
			SAFE_DELETE(pWebSocketInfo);
		}

		for(mapPacket::iterator itorUsedB = m_mapPacketUsed.begin(); itorUsedB != m_mapPacketUsed.end(); itorUsedB++)
		{
			_WebSocketInfo* pWebSocketInfo = (_WebSocketInfo* )itorUsedB->second;
			SAFE_DELETE(pWebSocketInfo);
		}

		m_mapPacketFree.clear();
		m_mapPacketUsed.clear();
	};

private:
	typedef map<_WebSocketInfo*, _WebSocketInfo*> mapPacket;
	mapPacket                  m_mapPacketUsed;                       //已使用的
	mapPacket                  m_mapPacketFree;                       //没有使用的
	ACE_Recursive_Thread_Mutex m_ThreadWriteLock;
};

//管理所有的连接类
class CWebSocketInfoManager
{
public:
	CWebSocketInfoManager() {};
	~CWebSocketInfoManager() { Close(); };

	void Close()
	{
		for(mapWebSocketInfo::iterator b = m_mapWebSocketInfo.begin(); b != m_mapWebSocketInfo.end(); b++)
		{
			_WebSocketInfo* pWebSocketInfo = (_WebSocketInfo* )b->second;
			SAFE_DELETE(pWebSocketInfo);
		}

		m_mapWebSocketInfo.clear();
	}

	//插入一个新的数据连接状态
	bool Insert(uint32 u4ConnectID)
	{
		mapWebSocketInfo::iterator f = m_mapWebSocketInfo.find(u4ConnectID);
		if(f != m_mapWebSocketInfo.end())
		{
			OUR_DEBUG((LM_ERROR, "[CPacketParse::Connect]ConnectID=%d is exist.\n"));
			return false;
		}

		_WebSocketInfo* pWebSocketInfo = new _WebSocketInfo();
		pWebSocketInfo->m_u4ConnectID = u4ConnectID;
		m_mapWebSocketInfo.insert(mapWebSocketInfo::value_type(u4ConnectID, pWebSocketInfo));

		return true;
	}

	//删除一个新的数据库连接状态
	void Delete(uint32 u4ConnectID)
	{
		mapWebSocketInfo::iterator f = m_mapWebSocketInfo.find(u4ConnectID);
		if(f != m_mapWebSocketInfo.end())
		{
			_WebSocketInfo* pWebSocketInfo = (_WebSocketInfo* )f->second;
			SAFE_DELETE(pWebSocketInfo);
			m_mapWebSocketInfo.erase(f);
		}
	}

	//查找指定的连接状态
	_WebSocketInfo* GetWebSocketInfo(uint32 u4ConnectID)
	{
		mapWebSocketInfo::iterator f = m_mapWebSocketInfo.find(u4ConnectID);
		if(f == m_mapWebSocketInfo.end())
		{
			//没有找到
			return NULL;
		}
		else
		{
			//找到了
			return (_WebSocketInfo* )f->second;
		}
	}

private:
	typedef map<uint32, _WebSocketInfo*> mapWebSocketInfo;

	//考虑到大量并发下的数据验证，采用map可能优于vector
	mapWebSocketInfo m_mapWebSocketInfo;
};

typedef ACE_Singleton<CWebSocketInfoManager, ACE_Null_Mutex> App_WebSocketInfoManager;
