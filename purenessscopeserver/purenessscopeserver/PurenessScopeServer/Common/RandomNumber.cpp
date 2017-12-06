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

int CRandomNumber::GetRandom()
{
    int nRandom = (int)(m_nMinNumber + GetRandomSeed() % (m_nMaxNumber - m_nMinNumber));
    return nRandom;
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
    int fd = open("/dev/urandom", O_RDONLY);

    if (fd != -1)
    {
        (void)read(fd, (void*)&rnum, sizeof(int));
        (void)close(fd);
    }

#endif
    return rnum;
}

