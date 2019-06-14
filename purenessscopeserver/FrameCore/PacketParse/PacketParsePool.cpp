#include "PacketParsePool.h"

CPacketParsePool::CPacketParsePool()
{
}

void CPacketParsePool::Init_Callback(int nIndex, CPacketParse* pPacketParse)
{
    pPacketParse->Init();
    pPacketParse->SetHashID(nIndex);
}

void CPacketParsePool::Close()
{
    OUR_DEBUG((LM_INFO, "[CPacketParsePool::~CPacketParsePool].\n"));
    //清理所有已存在的指针
    CObjectPoolManager<CPacketParse, ACE_Recursive_Thread_Mutex>::Close();
    OUR_DEBUG((LM_INFO, "[CPacketParsePool::~CPacketParsePool] End.\n"));
}

int CPacketParsePool::GetUsedCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    return CObjectPoolManager<CPacketParse, ACE_Recursive_Thread_Mutex>::GetUsedCount();
}

int CPacketParsePool::GetFreeCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    return  CObjectPoolManager<CPacketParse, ACE_Recursive_Thread_Mutex>::GetFreeCount();
}

CPacketParse* CPacketParsePool::Create(const char* pFileName, uint32 u4Line)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    return CObjectPoolManager<CPacketParse, ACE_Recursive_Thread_Mutex>::Create(pFileName, u4Line);
}

bool CPacketParsePool::Delete(CPacketParse* pPacketParse, bool blDelete)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if (NULL == pPacketParse)
    {
        OUR_DEBUG((LM_INFO, "[CPacketParsePool::Delete] pPacketParse is NULL.\n"));
        return false;
    }

    if (true == blDelete)
    {
        //清理包头和包体的内存
        if (NULL != pPacketParse->GetMessageHead())
        {
            App_MessageBlockManager::instance()->Close(pPacketParse->GetMessageHead());
        }

        if (NULL != pPacketParse->GetMessageBody())
        {
            App_MessageBlockManager::instance()->Close(pPacketParse->GetMessageBody());
        }
    }

    pPacketParse->Clear();

    return CObjectPoolManager<CPacketParse, ACE_Recursive_Thread_Mutex>::Delete(pPacketParse->GetHashID(), pPacketParse);
}
