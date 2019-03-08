#ifndef _RANDOMNUMBER_H
#define _RANDOMNUMBER_H

//����������ĺ���
//����ָ����Χ���������������

#include <stdlib.h>
#include "define.h"

#if PSS_PLATFORM == PLATFORM_WIN
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
    int GetRandom(int nRandType = 0);                //����㷨��0Ĭ����α�����1�������

private:
    int GetRandomSeed_Logic(const char* pPath);

private:
    int m_nMinNumber;
    int m_nMaxNumber;
};
#endif
