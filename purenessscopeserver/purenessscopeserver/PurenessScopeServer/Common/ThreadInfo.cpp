// MessageService.h
// 记录线程中的一些状态，用于自检和分析
// 写到这里，说什么呢。呵呵，什么都不想说了，继续写吧。
// add by freeeyes
// 2009-01-27

#include "ThreadInfo.h"

CThreadInfo::CThreadInfo(void)
{
    m_pAllThreadInfo = NULL;
    m_nThreadCount = 0;
}

CThreadInfo::~CThreadInfo(void)
{
    OUR_DEBUG((LM_ERROR, "[CThreadInfo::~CThreadInfo].\n"));
    Close();
    OUR_DEBUG((LM_ERROR, "[CThreadInfo::~CThreadInfo]End.\n"));
}

void CThreadInfo::Init(int nCount)
{
    Close();

    m_pAllThreadInfo = new _ThreadInfo*[nCount];

    if (NULL != m_pAllThreadInfo)
    {
        memset(m_pAllThreadInfo, 0, sizeof(_ThreadInfo*)*nCount);
    }

    m_nThreadCount   = nCount;
}

bool CThreadInfo::AddThreadInfo(uint32 u4ThreadID)
{
    if(u4ThreadID >= (uint32)m_nThreadCount)
    {
        return false;
    }

    if(NULL != m_pAllThreadInfo[u4ThreadID])
    {
        OUR_DEBUG((LM_ERROR, "[CThreadInfo::AddThreadInfo] u4ThreadID = %d is exist.\n", u4ThreadID));
        return false;
    }
    else
    {
        _ThreadInfo* pThreadInfo = new _ThreadInfo();

        if (NULL == pThreadInfo)
        {
            OUR_DEBUG((LM_ERROR, "[CThreadInfo::AddThreadInfo] pThreadInfo is NULL.\n"));
            return false;
        }

        pThreadInfo->m_u4ThreadID = u4ThreadID;

        m_pAllThreadInfo[u4ThreadID] = pThreadInfo;
    }

    return true;
}

bool CThreadInfo::AddThreadInfo(uint32 u4ThreadID, _ThreadInfo* pOrcThreadInfo)
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
        _ThreadInfo* pThreadInfo = new _ThreadInfo();

        (*pThreadInfo) = (*pOrcThreadInfo);
        pThreadInfo->m_u4ThreadID = u4ThreadID;

        m_pAllThreadInfo[u4ThreadID] = pThreadInfo;
    }

    return true;
}
void CThreadInfo::Close()
{
    for(int i = 0; i < m_nThreadCount; i++)
    {
        _ThreadInfo* pThreadInfo = m_pAllThreadInfo[i];

        if(NULL != pThreadInfo)
        {
            SAFE_DELETE(pThreadInfo);
        }
    }

    SAFE_DELETE_ARRAY(m_pAllThreadInfo);
    m_nThreadCount = 0;

}

_ThreadInfo* CThreadInfo::GetThreadInfo()
{
    //这里不再单独判断ThreadID，因为一个svc目前只有一个线程ID
    return m_pAllThreadInfo[0];
}

_ThreadInfo* CThreadInfo::GetThreadInfo(uint32 u4ThreadID)
{
    //这里不再单独判断ThreadID，因为一个svc目前只有一个线程ID
    if(u4ThreadID >= (uint32)m_nThreadCount)
    {
        return NULL;
    }

    return m_pAllThreadInfo[u4ThreadID];
}

bool CThreadInfo::CloseThread(uint32 u4ThreadID)
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

int CThreadInfo::GetThreadCount()
{
    return m_nThreadCount;
}
