#ifndef _IPACCOUNT_H
#define _IPACCOUNT_H

//添加对客户端链接的统计信息
//add by freeeyes
//2011-10-08
//新增对IP的动态追踪，便于数据流的追踪


#include <string>
#include "Ring.h"
#include "MapTemplate.h"
#include "ace/Date_Time.h"

//IP访问统计模块
struct _IPAccount
{
	string         m_strIP;              //当前链接地址
	int            m_nCount;             //当前链接次数
	int            m_nAllCount;          //指定IP链接次数总和 
	int            m_nMinute;            //当前分钟数
	ACE_Date_Time  m_dtLastTime;         //最后链接时间

	_IPAccount()
	{
		m_strIP      = "";
		m_nCount     = 0;
		m_nAllCount  = 0;
		m_dtLastTime.update();
		m_nMinute    = (int)m_dtLastTime.minute();
	}

	void Add()
	{
		m_dtLastTime.update();
		if(m_dtLastTime.minute() != m_nMinute)
		{
			m_nMinute  = (int)m_dtLastTime.minute();
			m_nCount   = 1;
			m_nAllCount++;
		}
		else
		{
			m_nCount++;
			m_nAllCount++;
		}
	}
};

//IP追踪记录数据信息模块
struct _IPTrackInfo
{
	char           m_szClientIP[MAX_BUFF_20]; //连接IP
	int            m_nPort;                   //连接端口
	uint32         m_u4RecvByteSize;          //接收字节数
	uint32         m_u4SendByteSize;          //发送字节数  
	ACE_Date_Time  m_dtConnectStart;          //连接建立时间
	ACE_Date_Time  m_dtConnectEnd;            //连接断开时间
	uint8          m_u1State;                 //当前连接状态，0为正在连接，1为已关闭 

	_IPTrackInfo()
	{
		m_szClientIP[0]  = '\0';
		m_nPort          = 0;
		m_u4RecvByteSize = 0;
		m_u4SendByteSize = 0;
		m_u1State        = 0;
	}
};

typedef vector<_IPAccount> vecIPAccount;

class CIPAccount
{
public:
	CIPAccount() 
	{ 
		m_nMaxConnectCount = 100;  //默认每秒最高100次 
		m_szTrackIP[0]     = '\0';
	};

	~CIPAccount() 
	{
		OUR_DEBUG((LM_INFO, "[CIPAccount::~CIPAccount].\n"));
		Close(); 
		OUR_DEBUG((LM_INFO, "[CIPAccount::~CIPAccount]End.\n"));
	};

	void Close()
	{
		m_mapIPAccount.Clear();
	}

	void Init(uint32 u4IPCount, uint32 u4TrackLogCount)
	{
		m_nMaxConnectCount = u4IPCount;

		m_objRing.Init(u4TrackLogCount);
	};

	bool AddIP(string strIP, int nPort)
	{
		bool blRet = false;
		//看看需要不需要判定，如果需要，则进行IP统计
		if(m_nMaxConnectCount > 0)
		{
			_IPAccount* pIPAccount = m_mapIPAccount.SearchMapData(strIP);
			if(NULL == pIPAccount)
			{
				//没有找到，添加
				pIPAccount = new _IPAccount();
				if(NULL == pIPAccount)
				{
					blRet = true;
				}
				else
				{
					pIPAccount->m_strIP     = strIP;
					pIPAccount->Add();
					m_mapIPAccount.AddMapData(strIP, pIPAccount);
				}
			}
			else
			{
				pIPAccount->Add();
				if(pIPAccount->m_nCount >= m_nMaxConnectCount)
				{
					blRet = false;
				}
			}

			blRet = true;
		}
		else
		{
			blRet = true;
		}

		//查看这个IP是否属于要追踪的数据流
		if(ACE_OS::strlen(m_szTrackIP) > 0)
		{
			if(ACE_OS::strcmp(m_szTrackIP, strIP.c_str()) == 0)
			{
				//添加新的数据
				_IPTrackInfo* pIPTrackInfo = m_objRing.GetFreeData();
				if(NULL != pIPTrackInfo)
				{
					sprintf_safe(pIPTrackInfo->m_szClientIP, MAX_BUFF_20, "%s", strIP.c_str());
					pIPTrackInfo->m_nPort = nPort;
					pIPTrackInfo->m_dtConnectStart.update();
					m_objRing.Add();
				}
			}
		}

		return blRet;
	};

	bool CloseIP(string strIP, int nPort, uint32 u4RecvSize, uint32 u4SendSize)
	{
		if(ACE_OS::strlen(m_szTrackIP) > 0)
		{
			for(int i = 0; i < m_objRing.GetCount(); i++)
			{
				_IPTrackInfo* pIPTrackInfo = m_objRing.GetLinkData(i);
				if(ACE_OS::strcmp(pIPTrackInfo->m_szClientIP, strIP.c_str()) == 0 && pIPTrackInfo->m_nPort == nPort)
				{
					//找到了匹配的IP，记录终止时间
					pIPTrackInfo->m_dtConnectEnd.update();
					pIPTrackInfo->m_u4RecvByteSize = u4RecvSize;
					pIPTrackInfo->m_u4SendByteSize = u4SendSize;
					pIPTrackInfo->m_u1State        = 1;
					return true;
				}
			}

			return false;
		}

		return true;
	}

	bool UpdateIP(string strIP, int nPort, uint32 u4RecvSize, uint32 u4SendSize)
	{
		if(ACE_OS::strlen(m_szTrackIP) > 0)
		{
			for(int i = 0; i < m_objRing.GetCount(); i++)
			{
				_IPTrackInfo* pIPTrackInfo = m_objRing.GetLinkData(i);
				if(ACE_OS::strcmp(pIPTrackInfo->m_szClientIP, strIP.c_str()) == 0 && pIPTrackInfo->m_nPort == nPort)
				{
					//找到了匹配的IP，记录终止时间
					pIPTrackInfo->m_u4RecvByteSize = u4RecvSize;
					pIPTrackInfo->m_u4SendByteSize = u4SendSize;
					return true;
				}
			}

			return false;
		}

		return true;
	}

	int GetCount()
	{
		return m_mapIPAccount.GetSize();
	}

	void GetInfo(vecIPAccount& VecIPAccount)
	{
		for(int i = 0; i < m_mapIPAccount.GetSize(); i++)
		{
			_IPAccount* pIPAccount = m_mapIPAccount.GetMapData(i);
			if(NULL != pIPAccount)
			{
				VecIPAccount.push_back(*pIPAccount);
			}
		}
	}

	void SetTrackIP(const char* pIP)
	{
		//清理以前的历史记录
		m_objRing.Clear();

		sprintf_safe(m_szTrackIP, MAX_BUFF_20, "%s", pIP);
	}

	void ClearTrackIP()
	{
		//清理以前的历史记录
		m_objRing.Clear();

		m_szTrackIP[0] = '\0';
	}

	int GetTrackIPInfoCount()
	{
		//得到TrackIP历史的个数
		return m_objRing.GetCount();
	}

	_IPTrackInfo* GetBase()
	{
		return m_objRing.GetBase();
	}

private:
	CMapTemplate<string, _IPAccount> m_mapIPAccount;                       //IP统计信息
	vector<_IPTrackInfo>             m_vecIPTrack;                         //要追踪的IP连接信息
	char                             m_szTrackIP[MAX_BUFF_20];             //要追踪的数据流IP，目前只考虑动态追一个，否则批量的很消耗内存，也无必要。
	int                              m_nMaxConnectCount;                   //每秒允许的最大连接数，前提是m_nNeedCheck = 0;才会生效
	CRingLink<_IPTrackInfo>          m_objRing;                            //环形连接日志，记录监控IP的活动 
};

typedef ACE_Singleton<CIPAccount, ACE_Recursive_Thread_Mutex> App_IPAccount;

//单位时间连接数统计
class CConnectAccount
{
public:
	CConnectAccount()
	{
		m_u4ConnectMin    = 0;
		m_u4ConnectMax    = 0;
		m_u4DisConnectMin = 0;
		m_u4DisConnectMax = 0;
	};

	~CConnectAccount()
	{

	}

	void Init(uint32 u4ConnectMin, uint32 u4ConnectMax, uint32 u4DisConnectMin, uint32 u4DisConnectMax)
	{
		m_u4ConnectMin    = u4ConnectMin;
		m_u4ConnectMax    = u4ConnectMax;
		m_u4DisConnectMin = u4DisConnectMin;
		m_u4DisConnectMax = u4DisConnectMax;

		ACE_Date_Time  dtLastTime;
		m_u1Minute = (uint8)dtLastTime.minute();
	}

	bool AddConnect()
	{
		if(m_u4ConnectMax > 0)
		{
			ACE_Date_Time  dtLastTime;
			if(m_u1Minute != (uint8)dtLastTime.minute())
			{
				//新的一分钟
				m_u4CurrConnect = 1;
			}
			else
			{
				m_u4CurrConnect++;
			}

			return true;
		}
		else
		{
			return true;
		}
	}

	bool AddDisConnect()
	{
		if(m_u4ConnectMax > 0)
		{
			ACE_Date_Time  dtLastTime;
			if(m_u1Minute != (uint8)dtLastTime.minute())
			{
				//新的一分钟
				m_u4CurrDisConnect = 1;
			}
			else
			{
				m_u4CurrDisConnect++;
			}

			return true;
		}
		else
		{
			return true;
		}
	}
	
	bool CheckConnectCount()
	{
		if(m_u4ConnectMax > 0)
		{
			if(m_u4CurrConnect > m_u4ConnectMax)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}

	bool CheckDisConnectCount()
	{
		if(m_u4DisConnectMax > 0)
		{
			if(m_u4CurrDisConnect > m_u4DisConnectMax)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}

private:
	uint32 m_u4CurrConnect;
	uint32 m_u4CurrDisConnect;

	uint32 m_u4ConnectMin;
	uint32 m_u4ConnectMax;
	uint32 m_u4DisConnectMin;
	uint32 m_u4DisConnectMax;
	uint8  m_u1Minute;            //当前分钟数
};

typedef ACE_Singleton<CConnectAccount, ACE_Recursive_Thread_Mutex> App_ConnectAccount;
#endif
