#ifndef _PROFORMANCECOUNTER_H
#define _PROFORMANCECOUNTER_H

//��������ͳ�Ƽ���������
//���ڲ���PSS������λ��

#include "define.h"

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
	ACE_Time_Value m_tvBegin;
	string m_strName;
};

#endif
