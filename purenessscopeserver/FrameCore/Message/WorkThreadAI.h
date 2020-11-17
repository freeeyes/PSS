#ifndef _WORKTHREADAI_H
#define _WORKTHREADAI_H

//ʵ�ֶԹ����߳�Ч�ʵļ��
//���뷨��Դ��Bobo�Ľ��飬�����һ�������̳߳������������������ݻ�����ô�죿
//ɱ�������̺߳����ӹ����̶߳�������õİ취
//��õİ취�������ҵ�������׵��Ǹ����ݰ�
//��������£�Ӧ����ĳ�����ݴ������������⣬�����ܲ���ȫ�������߳�
//�������������ݰ���һ��ʱ���������˹����߳�̫��ʱ�䣬��ô�͹��ϲ�ȡ��ʩ
//ֹͣ�������һ��ʱ�䣬�����ظ��ͻ��˸�֪
//ֱ���߶̻�·��Ӧ�����쳣����֤�������ݰ����������ݴ���
//����¼��־������������ά������
//����������������Ϊĳһ��ָ����������µ������������ӦЧ�ʵ��µ�����
//�˹��ܣ����Ը�����������Ƿ�������main.xml�������ÿ���
//����������ݰ������ڻ������ڴ�AI�Ĺ���Χ֮��
//PSS���ɻ����������ұ������ƣ����������߳�
//������Ȼȫ�����ݰ�����ʱ�ˣ�Ӧ��Ҳ���������û�������
//�˹���ֻ������޶ȵı����û����飬�û������Ƿ���������ʳ��ĸ
//add by freeeyes

#include "Ring.h"
#include "define.h"

const uint16 COMMAND_RETURN_BUSY = 0xffff;

//�����ƻ�ש�࣬��������ƵĴ���ת��
class CConvertBuffer
{
public:
    CConvertBuffer() {}

    int GetBufferSize(const char* pData, int nSrcLen) const
    {
        char szData[3] = {'\0'};
        int nPos         = 0;
        int nCurrSize    = 0;
        int nConvertSize = 0;
        bool blState     = false;   //ת������ַ����Ƿ���Ч
        bool blSrcState  = true;    //Ԫ�ַ����Ƿ���Ч
        unsigned char cData;

        while(nPos < nSrcLen)
        {
            if(pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
            {
                if(nPos == nSrcLen - 1 && nCurrSize < 2)
                {
                    szData[nCurrSize++] = pData[nPos];
                }

                if (nCurrSize < 3)
                {
                    szData[nCurrSize] = '\0';
                }
                else
                {
                    blSrcState = false;
                }

                if(blSrcState == true)
                {
                    blState = ConvertStr2char(szData, cData);
                }

				if (blState == true)
				{
					nConvertSize++;
				}

                nCurrSize  = 0;
                blSrcState = true;
                nPos++;
            }
            else
            {
                if(nCurrSize < 2)
                {
                    szData[nCurrSize++] = pData[nPos];
                }
                else
                {
                    blSrcState = false;
                }

                nPos++;
            }
        }

        return nConvertSize;
    }

    bool Convertstr2charArray(const char* pData, int nSrcLen, unsigned char* pDes, int& nMaxLen) const 
    {
        char szData[3] = {'\0'};
        int nPos         = 0;
        int nCurrSize    = 0;
        int nConvertSize = 0;
        bool blState     = false;   //ת������ַ����Ƿ���Ч
        bool blSrcState  = true;    //Ԫ�ַ����Ƿ���Ч

        while(nPos < nSrcLen)
        {
            if(pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
            {
                if(nPos == nSrcLen - 1 && nCurrSize < 2)
                {
                    szData[nCurrSize++] = pData[nPos];
                }

                if (nCurrSize < 3)
                {
                    szData[nCurrSize] = '\0';
                }
                else
                {
                    blSrcState = false;
                }

                if(nConvertSize < nMaxLen && blSrcState == true)
                {
                    blState = ConvertStr2char(szData, pDes[nConvertSize]);
                }

				if (blState == true)
				{
					nConvertSize++;
				}

                nCurrSize  = 0;
                blSrcState = true;
                nPos++;
            }
            else
            {
                if(nCurrSize < 2)
                {
                    szData[nCurrSize++] = pData[nPos];
                }
                else
                {
                    blSrcState = false;
                }

                nPos++;
            }
        }

        nMaxLen = nConvertSize;
        return true;
    }
private:
    bool Get_binary_Char(unsigned char cTag, unsigned char& cDes) const
    {
        if(cTag >='A'&&  cTag <='F')
        {
            cDes = cTag - 'A' + 10;
            return true;
        }
        else if(cTag >='a'&&  cTag <='f')
        {
            cDes = cTag - 'a' + 10;
            return true;
        }
        else if(cTag >= '0'&& cTag<= '9')
        {
            cDes = cTag-'0';
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ConvertStr2char(const char* pData, unsigned char& cData) const
    {
        if(pData == nullptr || strlen(pData) != 2)
        {
            return false;
        }

        char cFirst = pData[1];
        unsigned char cTemp = 0;
        bool blStste = Get_binary_Char(cFirst, cTemp);

        if(false == blStste)
        {
            return false;
        }

        cData = cTemp;
        char cSecond = pData[0];
        blStste  = Get_binary_Char(cSecond, cTemp);

        if(false == blStste)
        {
            return false;
        }

        cTemp = (unsigned char)(cTemp << 4);
        cData = cData | cTemp;

        return true;
    }
};

//��ز������
class _WorkThreadAIInfo
{
public:
    uint32     m_u4ThreadID       = 0;                    //�����߳�ID
    uint8      m_u1WTAI           = 0;                    //�����߳�AI���أ�0Ϊ�رգ�1Ϊ��
    uint32     m_u4DisposeTime    = 0;                    //ҵ�������ʱʱ��
    uint32     m_u4WTCheckTime    = 0;                    //�����̳߳�ʱ����ʱ�䷶Χ����λ����
    uint32     m_u4WTTimeoutCount = 0;                    //�����̳߳�ʱ���ĵ�λʱ���ڵĳ�ʱ��������
    uint32     m_u4WTStopTime     = 0;                    //ֹͣ����������ʱ��

    _WorkThreadAIInfo()
    {
    }
};

//AI������Ϣ��
typedef vector<_WorkThreadAIInfo> vecWorkThreadAIInfo;

//��ʱ���Ԫ
class _CommandTimeout
{
public:
    uint32 m_u4ThreadID  = 0;       //�����߳�ID
    uint16 m_u2CommandID = 0;       //��ʱ������
    uint64 m_u8Second    = 0;       //��ʱ��ǰʱ�䣬��1970��������ʼ���������
    uint32 m_u4Timeout   = 0;       //����ִ��ʱ�䣬��λ�Ǻ���

    _CommandTimeout()
    {
    }
};

//�����û��ش���ز�ѯ��Ϣ
typedef vector<_CommandTimeout> vecCommandTimeout;

class CWorkThreadAI
{
public:
    CWorkThreadAI();

    void Close();

    void Init(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTTimeoutCount, uint32 u4WTStopTime, uint8 u1WTReturnDataType, const char* pReturnData);

    bool SaveTimeout(uint16 u2CommandID, uint32 u4TimeCost);

    char* GetReturnData();
    uint16 GetReturnDataLength() const;

    void GetAIInfo(_WorkThreadAIInfo& objWorkThreadAIInfo) const;

    void ReSet(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime);

    bool CheckCurrTimeout(uint16 u2CommandID, uint64 u8Now);

    void GetAllTimeout(uint32 u4ThreadID, vecCommandTimeout& objTimeout) const;
    void GetAllForbiden(uint32 u4ThreadID, vecCommandTimeout& objForbiden) const;

private:
    int Do_Command_Account(uint16 u2CommandID, uint64 u8Now, uint32 u4TimeCost, bool& blRet);   //ͳ��Command��AI����

    uint8      m_u1WTAI                        = 0;           //�����߳�AI���أ�0Ϊ�رգ�1Ϊ��
    uint16     m_u2ReturnDataLen               = 0;           //���������峤��
    uint32     m_u4DisposeTime                 = 0;           //ҵ�������ʱʱ��
    uint32     m_u4WTCheckTime                 = 0;           //�����̳߳�ʱ����ʱ�䷶Χ����λ����
    uint32     m_u4WTTimeoutCount              = 0;           //�����̳߳�ʱ���ĵ�λʱ���ڵĳ�ʱ��������
    uint32     m_u4WTStopTime                  = 0;           //ֹͣ����������ʱ��
    uint8      m_u1WTReturnDataType            = 0;           //���ش������ݵ����ͣ�1Ϊ�����ƣ�2Ϊ�ı�
    char       m_szWTReturnData[MAX_BUFF_1024] = {'\0'};      //���ص������壬���1K

    //��ʱ�������
    class _CommandTime
    {
    public:
        uint16 m_u2CommandID = 0;
        CRingLink<_CommandTimeout> m_objTime;

        _CommandTime()
        {
        }
    };

    vector<_CommandTime*>   m_vecCommandTime;
    vector<_CommandTimeout> m_vecCommandTimeout;
};
#endif
