#ifndef _PROFORMANCECOUNTER_H
#define _PROFORMANCECOUNTER_H

//用于性能统计计数器的类
//用于测试PSS的性能位置

#include "define.h"
#include "TimeStamp.hpp"

const int PROFORMANCE_MAX_COUNT = 1000;

class CPerformanceCounter
{
public:
	CPerformanceCounter();

	void init(string strName, int nMaxCount = PROFORMANCE_MAX_COUNT);

	void counter();

	void set_start();

	void reset();

private:
	uint32 m_u4CurrCount = 0;
	uint32 m_u4MaxCount  = 0;
	PSS_Time_Point m_tvBegin;
	string m_strName;
};

#endif
