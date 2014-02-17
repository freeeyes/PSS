#ifndef _PARAMDATA_H
#define _PARAMDATA_H

#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

#include <vector>

#define MAX_PARAM_200 200

using namespace std;

struct _ParamData
{
public:
	void* m_pParam;
	char  m_szType[MAX_PARAM_200];
	int   m_TypeLen;

public:
	_ParamData()
	{
		m_pParam    = NULL;
		m_szType[0] = '\0';
		m_TypeLen   = 0;
	};

	_ParamData(void* pParam, const char* szType, int nTypeLen)
	{
		SetParam(pParam, szType, nTypeLen);
	}

	~_ParamData() {};

	void SetParam(void* pParam, const char* szType, int nTypeLen)
	{
		m_pParam = pParam;
		ACE_OS::sprintf(m_szType, "%s", szType);
		m_TypeLen = nTypeLen;
	};

	bool SetData(void* pParam, int nLen)
	{
		if(m_TypeLen < nLen)
		{
			return false;
		}

		if(nLen > 0)
		{
			memcpy(m_pParam, pParam, nLen);
		}
		else
		{
			if(m_TypeLen == 4)
			{
				//如果是指针
				m_pParam = pParam;
			}
			else
			{
				memcpy(m_pParam, pParam, m_TypeLen);
			}
		}
		return true;
	}

	void* GetParam()
	{
		return m_pParam;
	}

	const char* GetType()
	{
		return m_szType;
	}

	bool CompareType(const char* pType)
	{
		if(0 == strcmp(m_szType, pType))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};


class CParamGroup
{
public:
	CParamGroup() {};
	~CParamGroup()
	{ 
		Close(); 
	};

	void Init()
	{
		m_vecParamData.clear();
	};

	void Close()
	{
		for(int i = 0; i < (int)m_vecParamData.size(); i++)
		{
			_ParamData* pParamData = m_vecParamData[i];
			delete pParamData;
			pParamData = NULL;
		}
		m_vecParamData.clear();
	};

	void Push(_ParamData* pParam)
	{
		if(pParam != NULL)
		{
			m_vecParamData.push_back(pParam);
		}
	};

	_ParamData* GetParam(int nIndex)
	{
		if(nIndex < (int)m_vecParamData.size())
		{
			return m_vecParamData[nIndex];
		}
		else
		{
			return NULL;
		}
	};

	int GetCount()
	{
		return (int)m_vecParamData.size();
	}

private:
	typedef vector<_ParamData*> vecParamData;
	vecParamData m_vecParamData;
};


#endif
