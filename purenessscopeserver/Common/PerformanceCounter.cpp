#include "PerformanceCounter.h"


CPerformanceCounter::CPerformanceCounter()
{
}

void CPerformanceCounter::init(string strName, int nMaxCount)
{
	m_u4MaxCount = nMaxCount;
	m_strName = strName;
}

void CPerformanceCounter::counter()
{
	if (m_u4MaxCount == 0)
	{
		return;
	}

	m_u4CurrCount++;

	if (m_u4CurrCount == 1)
	{
		m_tvBegin = CTimeStamp::Get_Time_Stamp();
		
	}
	else if(m_u4CurrCount == m_u4MaxCount)
	{
		auto tvCost = CTimeStamp::Get_Time_Difference(CTimeStamp::Get_Time_Stamp(), m_tvBegin);
		PSS_LOGGER_DEBUG("[{0}]m_u4MaxCount={1}, cost={2}.", m_strName, m_u4MaxCount, tvCost);
		m_u4CurrCount = 0;
	}

}

void CPerformanceCounter::set_start()
{
	m_tvBegin = CTimeStamp::Get_Time_Stamp();
}

void CPerformanceCounter::reset()
{
	m_u4CurrCount = 0;
}
