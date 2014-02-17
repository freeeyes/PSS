#include "SMAccessObject.h"

CSMAccessObject::CSMAccessObject(void)
{
	m_Key       = 0;
	m_nSize     = 0;
	m_nHeadSize = 0;
	m_pHeadData = NULL;
	m_pData     = NULL;
	m_SMHandle  = SM_INVALID_HANDLE;
	m_Key       = 0;
}

CSMAccessObject::~CSMAccessObject(void)
{
	Destory();
}

bool CSMAccessObject::Create(SMKey key, int nSize, int nHeadSize)
{
	m_SMHandle = ShareMemoryAPI::CreateShareMemory(key, nHeadSize + nSize);
	if(SM_INVALID_HANDLE == m_SMHandle)
	{
		return false;
	}

	m_pHeadData = ShareMemoryAPI::MapShareMemory(m_SMHandle);
	if(NULL == m_pHeadData)
	{
		return false;
	}

	m_pData     = m_pHeadData + nHeadSize;
	m_Key       = key;
	m_nSize     = nSize;
	m_nHeadSize = nHeadSize;

	return true;
}

void CSMAccessObject::Destory()
{
	if(m_pData != NULL)
	{
		ShareMemoryAPI::UnMapShareMemory(m_pData);
		m_pData = NULL;
	}

	if(m_SMHandle != SM_INVALID_HANDLE)
	{
		ShareMemoryAPI::CloseSharememory(m_SMHandle);
		m_SMHandle = SM_INVALID_HANDLE;
	}

	m_Key       = 0;
	m_nSize     = 0;
	m_nHeadSize = 0;
	m_pHeadData = NULL;
	m_pData     = NULL;
}

bool CSMAccessObject::Open(SMKey key, int nSize, int nHeadSize)
{
	m_SMHandle = ShareMemoryAPI::OpenShareMemory(key, nHeadSize + nSize);
	if(SM_INVALID_HANDLE == m_SMHandle)
	{
		return false;
	}

	m_pHeadData = ShareMemoryAPI::MapShareMemory(m_SMHandle);
	if(NULL == m_pHeadData)
	{
		return false;
	}

	m_pData     = m_pHeadData + nHeadSize;
	m_Key       = key;
	m_nSize     = nSize;
	m_nHeadSize = nHeadSize;

	return true;
}

char* CSMAccessObject::GetData(int nSize)
{
	if(nSize >= m_nSize)
	{
		return NULL;
	}
	else
	{
		return (char* )(m_pData + nSize);
	}
}

char* CSMAccessObject::GetHeadData()
{
	return m_pHeadData;
}

int CSMAccessObject::GetDataSize()
{
	return m_nSize;
}

int CSMAccessObject::GetHeadDataSize()
{
	return m_nHeadSize;
}
