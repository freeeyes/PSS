#ifndef _THREADINFO_H
#define _THREADINFO_H

#include "define.h"

using namespace std;

struct _ThreadInfo
{
    uint32              m_u4ThreadIndex;      //系统线程ID
    uint32              m_u4State;            //0为没有数据正在处理，1为正在处理接受数据。
    uint32              m_u4RecvPacketCount;  //此线程下接收包的处理个数
    uint32              m_u4CurrPacketCount;  //单位时间内的包处理数量
    uint16              m_u2CommandID;        //当前处理的信令ID
    uint16              m_u2PacketTime;       //平均数据包处理时间
    ACE_thread_t        m_u4ThreadID;         //线程ID
    ACE_Time_Value      m_tvUpdateTime;       //线程最后处理数据的时间
    ACE_Time_Value      m_tvCreateTime;       //线程创建时间

    _ThreadInfo()
    {
        m_u4State           = THREAD_INIT;
        m_tvUpdateTime      = ACE_OS::gettimeofday();
        m_tvCreateTime      = ACE_OS::gettimeofday();
        m_u4RecvPacketCount = 0;
        m_u2CommandID       = 0;
        m_u4ThreadIndex     = 0;
        m_u2PacketTime      = 0;
        m_u4CurrPacketCount = 0;
        m_u4ThreadID        = 0;
    }

    _ThreadInfo(const _ThreadInfo& ar)
    {
        this->m_u4ThreadIndex        = ar.m_u4ThreadIndex;
        this->m_tvUpdateTime         = ar.m_tvUpdateTime;
        this->m_tvCreateTime         = ar.m_tvCreateTime;
        this->m_u4State              = ar.m_u4State;
        this->m_u4RecvPacketCount    = ar.m_u4RecvPacketCount;
        this->m_u2CommandID          = ar.m_u2CommandID;
        this->m_u2PacketTime         = ar.m_u2PacketTime;
        this->m_u4CurrPacketCount    = ar.m_u4CurrPacketCount;
        this->m_u4ThreadID           = ar.m_u4ThreadID;
    }

    _ThreadInfo& operator = (const _ThreadInfo& ar)
    {
        this->m_u4ThreadIndex        = ar.m_u4ThreadIndex;
        this->m_tvUpdateTime         = ar.m_tvUpdateTime;
        this->m_tvCreateTime         = ar.m_tvCreateTime;
        this->m_u4State              = ar.m_u4State;
        this->m_u4RecvPacketCount    = ar.m_u4RecvPacketCount;
        this->m_u2CommandID          = ar.m_u2CommandID;
        this->m_u2PacketTime         = ar.m_u2PacketTime;
        this->m_u4CurrPacketCount    = ar.m_u4CurrPacketCount;
        return *this;
    }
};

class CThreadInfo
{
public:
    CThreadInfo(void);
    ~CThreadInfo(void);

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
