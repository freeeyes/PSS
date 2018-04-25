#ifndef _RANDOMNUMBER_H
#define _RANDOMNUMBER_H

//产生随机数的函数
//给出指定范围，产生出随机数。

#include <stdlib.h>

#ifdef WIN32
#include "windows.h"
#include "wincrypt.h"
#else
#include <unistd.h>
#include <fcntl.h>
#endif

class CRandomNumber
{
public:
    CRandomNumber(void);
    ~CRandomNumber(void);

    void SetRange(int nMinNumber, int nMaxNumber);
    int GetRandom(int nRandType = 0);                //随机算法，0默认是伪随机，1是真随机

private:
    int GetRandomSeed_Logic(const char* pPath);

private:
    int m_nMinNumber;
    int m_nMaxNumber;
};
#endif
