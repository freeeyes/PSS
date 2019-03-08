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
    //��֤��������ֲ����Ǹ���
    if (m_nMaxNumber <= m_nMinNumber || m_nMaxNumber < 0 || m_nMinNumber < 0 || m_nMinNumber == 2147483647)
    {
        return 0;
    }

    int nRandomSeed = 0;

    if (0 == nRandType)
    {
        //�����0������α������ٶȿ졣
        nRandomSeed = GetRandomSeed_Logic("/dev/urandom");
    }
    else
    {
        //���򣬲�����������ٶ�����
        nRandomSeed = GetRandomSeed_Logic("/dev/random");
    }

    return m_nMinNumber + nRandomSeed % (m_nMaxNumber - m_nMinNumber);
}

int CRandomNumber::GetRandomSeed_Logic(const char* pPath)
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
            //��ȡ�ļ�ʧ��
            rnum = 0;
        }

        close(fd);
    }

#endif

    //������Ӳ��ܲ�������
    if (rnum < 0)
    {
        rnum = 0 - rnum;
    }

    return rnum;
}


