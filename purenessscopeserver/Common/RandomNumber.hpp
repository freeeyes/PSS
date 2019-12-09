#ifndef _RANDOMNUMBER_H
#define _RANDOMNUMBER_H

//产生随机数的函数
//给出指定范围，产生出随机数。

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
    CRandomNumber(void) : m_nMinNumber(0), m_nMaxNumber(0)
    {
    };

    ~CRandomNumber(void)
    {
    };

    void SetRange(int nMinNumber, int nMaxNumber)
    {
        m_nMinNumber = nMinNumber;
        m_nMaxNumber = nMaxNumber + 1;
    };

    int GetRandom(int nRandType = 0)
    {
        //保证算出的数字不能是负数
        if (m_nMaxNumber <= m_nMinNumber || m_nMaxNumber < 0 || m_nMinNumber < 0 || m_nMinNumber == 2147483647)
        {
            return 0;
        }

        int nRandomSeed = 0;

        if (0 == nRandType)
        {
            //如果是0，采用伪随机，速度快。
            nRandomSeed = GetRandomSeed_Logic("/dev/urandom");
        }
        else
        {
            //否则，采用真随机，速度慢。
            nRandomSeed = GetRandomSeed_Logic("/dev/random");
        }

        return m_nMinNumber + nRandomSeed % (m_nMaxNumber - m_nMinNumber);
    };                //随机算法，0默认是伪随机，1是真随机

private:
    int GetRandomSeed_Logic(const char* pPath)
    {
        int rnum = 0;
#if defined WIN32
        HCRYPTPROV hProvider = 0;
        const DWORD dwLength = sizeof(int);
        BYTE pbBuffer[dwLength] = {};
        ::CryptAcquireContext(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
        ::CryptGenRandom(hProvider, dwLength, pbBuffer);
        rnum = *(int*)pbBuffer;
        ::CryptReleaseContext(hProvider, 0);
#else
        int fd = open(pPath, O_RDONLY);

        if (fd != -1)
        {
            size_t stReadLen = read(fd, (void*)&rnum, sizeof(int));

            if (stReadLen != sizeof(int))
            {
                //读取文件失败
                rnum = 0;
            }

            close(fd);
        }

#endif

        //随机种子不能产生负数
        if (rnum < 0)
        {
            rnum = 0 - rnum;
        }

        return rnum;
    };

private:
    int m_nMinNumber;
    int m_nMaxNumber;
};
#endif
