#ifndef _THREADINFO_H
#define _THREADINFO_H

#include "define.h"

class _ThreadInfo
{
public:
    uint32              m_u4ThreadIndex     = 0;                         //ϵͳ�߳�ID
    THREADSTATE         m_u4State           = THREADSTATE::THREAD_INIT;  //0Ϊû���������ڴ���1Ϊ���ڴ���������ݡ�
    uint32              m_u4RecvPacketCount = 0;                         //���߳��½��հ��Ĵ������
    uint32              m_u4CurrPacketCount = 0;                         //��λʱ���ڵİ���������
    uint16              m_u2CommandID       = 0;                         //��ǰ���������ID
    uint16              m_u2PacketTime      = 0;                         //ƽ�����ݰ�����ʱ��
    ACE_thread_t        m_u4ThreadID        = 0;                         //�߳�ID
    ACE_Time_Value      m_tvUpdateTime      = ACE_OS::gettimeofday();    //�߳���������ݵ�ʱ��
    ACE_Time_Value      m_tvCreateTime      = ACE_OS::gettimeofday();    //�̴߳���ʱ��

    _ThreadInfo()
    {
    }
};

class CThreadInfoList
{
public:
    CThreadInfoList(void);

    void Init(int nCount);

    bool AddThreadInfo(uint32 u4ThreadID);
    bool AddThreadInfo(uint32 u4ThreadID, _ThreadInfo* pOrcThreadInfo);
    int  GetThreadCount() const;
    _ThreadInfo* GetThreadInfo(uint32 u4ThreadID);
    _ThreadInfo* GetThreadInfo();
    bool CloseThread(uint32 u4ThreadID);
    void Close();

private:
    _ThreadInfo** m_pAllThreadInfo = nullptr;
    int           m_nThreadCount   = 0;
};
#endif
