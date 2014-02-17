#ifndef _RANDOMNUMBER_H
#define _RANDOMNUMBER_H

//产生随机数的函数
//给出指定范围，产生出随机数。

#include <stdlib.h>
#include <time.h>

class CRandomNumber
{
public:
	CRandomNumber(void);
	~CRandomNumber(void);

	void SetRange(int nMinNumber, int nMaxNumber);
	int GetRandom();

private:
	int m_nMinNumber;
	int m_nMaxNumber;
};
#endif
