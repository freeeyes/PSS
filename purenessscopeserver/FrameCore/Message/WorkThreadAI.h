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
#include "TimeStamp.hpp"
#include "ConvertBuffer.h"

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

    _WorkThreadAIInfo() = default;
};

//AI������Ϣ��
using vecWorkThreadAIInfo = vector<_WorkThreadAIInfo>;

//��ʱ���Ԫ
class _CommandTimeout
{
public:
    uint32 m_u4ThreadID  = 0;       //�����߳�ID
    uint16 m_u2CommandID = 0;       //��ʱ������
    uint64 m_u8Second    = 0;       //��ʱ��ǰʱ�䣬��1970��������ʼ���������
    uint32 m_u4Timeout   = 0;       //����ִ��ʱ�䣬��λ�Ǻ���

    _CommandTimeout() = default;
};

//�����û��ش���ز�ѯ��Ϣ
using vecCommandTimeout = vector<_CommandTimeout>;

class CWorkThreadAI
{
public:
    CWorkThreadAI() = default;

    void Close();

    void Init(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTTimeoutCount, uint32 u4WTStopTime, uint8 u1WTReturnDataType, const char* pReturnData);

    bool SaveTimeout(uint16 u2CommandID, uint32 u4TimeCost);

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
    string     m_strWTReturnData;                             //���ص������壬���1K

    //��ʱ�������
    class _CommandTime
    {
    public:
        uint16 m_u2CommandID = 0;
        CRingLink<_CommandTimeout> m_objTime;

        _CommandTime() = default;
    };

    vector<shared_ptr<_CommandTime>> m_vecCommandTime;
    vector<_CommandTimeout> m_vecCommandTimeout;
};
#endif
