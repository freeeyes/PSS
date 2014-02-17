#include "Serial.h"

CSerial::CSerial()
{
	m_pData      = NULL;
	m_nSize      = 0;
	m_nCellSize  = 0;
	m_nCount     = 0;
}

CSerial::~CSerial()
{
	Close();
}

void CSerial::Close()
{
	m_nSize      = 0;
	m_nCellSize  = 0;
	m_nCount     = 0;
	m_pData      = NULL;  //这里不用释放内存，因为这个指针是共享内存指针的地址。
}

//初始化序列化类，这里必须指定共享内存指针首地址，此后的地址都是依靠这个偏移量获得。
bool CSerial::Init(char* pData, int nCellSize, int nCount)
{
	if(NULL != m_pData)
	{
		Close();
	}

	m_pData = pData;
	if(NULL == m_pData)
	{
		return false;
	}

	m_nCellSize  = nCellSize;
	m_nCount     = nCount;
	m_nSize      = nCellSize * nCount;
	return true;
}

char* CSerial::GetData(int nIndex)
{
	if(NULL == m_pData)
	{
		return NULL;
	}

	if(nIndex >= m_nCount)
	{
		return NULL;
	}
	else
	{
		return (char* )(m_pData + (m_nCellSize * nIndex));
	}
}

char* CSerial::GetBase() const
{
	return m_pData;
}

