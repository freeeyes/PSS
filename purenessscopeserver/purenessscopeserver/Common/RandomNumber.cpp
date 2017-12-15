#include "RandomNumber.h"

CRandomNumber::CRandomNumber(void)
{
    m_nMinNumber = 0;
    m_nMaxNumber = 0;
}

CRandomNumber::~CRandomNumber(void)
{
}

void CRandomNumber::SetRange(int nMinNumber, int nMaxNumber)
{
    m_nMinNumber = nMinNumber;
    m_nMaxNumber = nMaxNumber + 1;
}

int CRandomNumber::GetRandom(int nRandType)
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
        nRandomSeed = GetRandomSeed_Pseudo();
    }
    else
    {
        //否则，采用真随机，速度慢。
        nRandomSeed = GetRandomSeed();
    }

    return m_nMinNumber + nRandomSeed % (m_nMaxNumber - m_nMinNumber);
}

int CRandomNumber::GetRandomSeed_Pseudo()
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
    int fd = open("/dev/urandom", O_RDONLY);

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
}

int CRandomNumber::GetRandomSeed()
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
    int fd = open("/dev/random", O_RDONLY);

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
}

