#ifndef _IPACCOUNT_H
#define _IPACCOUNT_H

//添加对客户端链接的统计信息
//add by freeeyes
//2011-10-08

#include <string>
#include "MapTemplate.h"
#include "ace/Date_Time.h"

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

typedef vector<_IPAccount> vecIPAccount;

class CIPAccount
{
public:
	CIPAccount() 
  { 
    m_nNeedCheck       = 0;
    m_nMaxConnectCount = 100;  //默认每秒最高100次 
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

	void Init(int nNeedCheck, int nMaxConnectCount)
	{
    m_nNeedCheck       = nNeedCheck;
		m_nMaxConnectCount = nMaxConnectCount;
	};

	bool AddIP(string strIP)
	{
    //看看需要不需要判定，如果需要，则进行IP统计
    if(m_nNeedCheck == 0)
    {
		  _IPAccount* pIPAccount = m_mapIPAccount.SearchMapData(strIP);
		  if(NULL == pIPAccount)
		  {
			  //没有找到，添加
			  pIPAccount = new _IPAccount();
			  if(NULL == pIPAccount)
			  {
				  return true;
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
				  return false;
			  }
		  }

      return true;
    }
    else
    {
      return true;
    }
	};

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

private:
	CMapTemplate<string, _IPAccount> m_mapIPAccount;
	int            m_nMaxConnectCount;
  int            m_nNeedCheck;        //是否需要验证，0为需要，1为不需要
};

typedef ACE_Singleton<CIPAccount, ACE_Recursive_Thread_Mutex> App_IPAccount;

#endif
