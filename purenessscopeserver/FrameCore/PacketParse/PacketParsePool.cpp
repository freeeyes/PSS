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
    //���������Ѵ��ڵ�ָ��
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

    if (nullptr == pPacketParse)
    {
        OUR_DEBUG((LM_INFO, "[CPacketParsePool::Delete] pPacketParse is nullptr.\n"));
        return false;
    }

    if (true == blDelete)
    {
        //�����ͷ�Ͱ�����ڴ�
        if (nullptr != pPacketParse->GetMessageHead())
        {
            App_MessageBlockManager::instance()->Close(pPacketParse->GetMessageHead());
        }

        if (nullptr != pPacketParse->GetMessageBody())
        {
            App_MessageBlockManager::instance()->Close(pPacketParse->GetMessageBody());
        }
    }

    pPacketParse->Clear();

    return CObjectPoolManager<CPacketParse, ACE_Recursive_Thread_Mutex>::Delete(pPacketParse->GetHashID(), pPacketParse);
}
