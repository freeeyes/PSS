// MessageService.h
// ��¼�߳��е�һЩ״̬�������Լ�ͷ���
// д�����˵ʲô�ء��Ǻǣ�ʲô������˵�ˣ�����д�ɡ�
// add by freeeyes
// 2009-01-27

#include "ThreadInfo.h"

CThreadInfoList::CThreadInfoList(void)
{
}

void CThreadInfoList::Init(int nCount)
{
    Close();

    m_pAllThreadInfo = new _ThreadInfo*[nCount];

    if (NULL != m_pAllThreadInfo)
    {
        memset(m_pAllThreadInfo, 0, sizeof(_ThreadInfo*)*nCount);
    }

    m_nThreadCount   = nCount;
}

bool CThreadInfoList::AddThreadInfo(uint32 u4ThreadID)
{
    if(u4ThreadID >= (uint32)m_nThreadCount)
    {
        return false;
    }

    if(NULL != m_pAllThreadInfo[u4ThreadID])
    {
        OUR_DEBUG((LM_ERROR, "[CThreadInfoList::AddThreadInfo] u4ThreadID = %d is exist.\n", u4ThreadID));
        return false;
    }
    else
    {
        auto pThreadInfo = new _ThreadInfo();

        pThreadInfo->m_u4ThreadID = u4ThreadID;

        m_pAllThreadInfo[u4ThreadID] = pThreadInfo;
    }

    return true;
}

bool CThreadInfoList::AddThreadInfo(uint32 u4ThreadID, _ThreadInfo* pOrcThreadInfo)
{
    if (u4ThreadID >= (uint32)m_nThreadCount)
    {
        return false;
    }

    if (NULL != m_pAllThreadInfo[u4ThreadID])
    {
        m_pAllThreadInfo[u4ThreadID] = pOrcThreadInfo;
    }
    else
    {
        auto pThreadInfo = new _ThreadInfo();

        (*pThreadInfo) = (*pOrcThreadInfo);
        pThreadInfo->m_u4ThreadID = u4ThreadID;

        m_pAllThreadInfo[u4ThreadID] = pThreadInfo;
    }

    return true;
}
void CThreadInfoList::Close()
{
    OUR_DEBUG((LM_ERROR, "[CThreadInfoList::~Close]Begin.\n"));

    for(int i = 0; i < m_nThreadCount; i++)
    {
        SAFE_DELETE(m_pAllThreadInfo[i]);
    }

    SAFE_DELETE_ARRAY(m_pAllThreadInfo);
    m_nThreadCount = 0;

    OUR_DEBUG((LM_ERROR, "[CThreadInfoList::~Close]End.\n"));
}

_ThreadInfo* CThreadInfoList::GetThreadInfo()
{
    //���ﲻ�ٵ����ж�ThreadID����Ϊһ��svcĿǰֻ��һ���߳�ID
    return m_pAllThreadInfo[0];
}

_ThreadInfo* CThreadInfoList::GetThreadInfo(uint32 u4ThreadID)
{
    //���ﲻ�ٵ����ж�ThreadID����Ϊһ��svcĿǰֻ��һ���߳�ID
    if(u4ThreadID >= (uint32)m_nThreadCount)
    {
        return NULL;
    }

    return m_pAllThreadInfo[u4ThreadID];
}

bool CThreadInfoList::CloseThread(uint32 u4ThreadID)
{
    if(u4ThreadID >= (uint32)m_nThreadCount)
    {
        return false;
    }

    if(NULL != m_pAllThreadInfo[u4ThreadID])
    {
        SAFE_DELETE(m_pAllThreadInfo[u4ThreadID]);
        return true;
    }
    else
    {
        return false;
    }
}

int CThreadInfoList::GetThreadCount() const
{
    return m_nThreadCount;
}
