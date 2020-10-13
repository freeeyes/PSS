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
		m_tvBegin = ACE_OS::gettimeofday();
		
	}
	else if(m_u4CurrCount == m_u4MaxCount)
	{
		ACE_Time_Value tvCost = ACE_OS::gettimeofday() - m_tvBegin;
		OUR_DEBUG((LM_INFO, "[%s]m_u4MaxCount=%d, cost=%d.\n", m_strName.c_str(), m_u4MaxCount, tvCost.msec()));
		m_u4CurrCount = 0;
	}

}

void CPerformanceCounter::set_start()
{
	m_tvBegin = ACE_OS::gettimeofday();
}

void CPerformanceCounter::reset()
{
	m_u4CurrCount = 0;
}
