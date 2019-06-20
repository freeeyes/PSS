#ifndef _THREADINFO_H
#define _THREADINFO_H

#include "define.h"

class _ThreadInfo
{
public:
    uint32              m_u4ThreadIndex;      //系统线程ID
    uint32              m_u4State;            //0为没有数据正在处理，1为正在处理接受数据。
    uint32              m_u4RecvPacketCount;  //此线程下接收包的处理个数
    uint32              m_u4CurrPacketCount;  //单位时间内的包处理数量
    uint16              m_u2CommandID;        //当前处理的信令ID
    uint16              m_u2PacketTime;       //平均数据包处理时间
    ACE_thread_t        m_u4ThreadID;         //线程ID
    ACE_Time_Value      m_tvUpdateTime;       //线程最后处理数据的时间
    ACE_Time_Value      m_tvCreateTime;       //线程创建时间

    _ThreadInfo() : m_u4ThreadIndex(0),
        m_u4State(THREAD_INIT),
        m_u4RecvPacketCount(0),
        m_u4CurrPacketCount(0),
        m_u2CommandID(0),
        m_u2PacketTime(0),
        m_u4ThreadID(0),
        m_tvUpdateTime(ACE_OS::gettimeofday()),
        m_tvCreateTime(ACE_OS::gettimeofday())
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
    int  GetThreadCount();
    _ThreadInfo* GetThreadInfo(uint32 u4ThreadID);
    _ThreadInfo* GetThreadInfo();
    bool CloseThread(uint32 u4ThreadID);
    void Close();

private:
    _ThreadInfo** m_pAllThreadInfo;
    int           m_nThreadCount;
};
#endif
