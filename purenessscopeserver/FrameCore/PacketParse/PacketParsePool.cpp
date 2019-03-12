#include "PacketParsePool.h"

CPacketParsePool::CPacketParsePool()
{
}

CPacketParsePool::CPacketParsePool(const CPacketParsePool& ar)
{
    (*this) = ar;
}

CPacketParsePool::~CPacketParsePool()
{
    OUR_DEBUG((LM_INFO, "[CPacketParsePool::~CPacketParsePool].\n"));
    Close();
    OUR_DEBUG((LM_INFO, "[CPacketParsePool::~CPacketParsePool] End.\n"));
}

void CPacketParsePool::Init_Callback(int nIndex, CPacketParse* pPacketParse)
{
    pPacketParse->Init();
    pPacketParse->SetHashID(nIndex);
}

void CPacketParsePool::Close()
{
    //清理所有已存在的指针
    CObjectPoolManager<CPacketParse, ACE_Recursive_Thread_Mutex>::Close();
}

int CPacketParsePool::GetUsedCount()
{
    return CObjectPoolManager<CPacketParse, ACE_Recursive_Thread_Mutex>::GetUsedCount();
}

int CPacketParsePool::GetFreeCount()
{
    return  CObjectPoolManager<CPacketParse, ACE_Recursive_Thread_Mutex>::GetFreeCount();
}

CPacketParse* CPacketParsePool::Create(const char* pFileName, uint32 u4Line)
{
    return CObjectPoolManager<CPacketParse, ACE_Recursive_Thread_Mutex>::Create(pFileName, u4Line);
}

bool CPacketParsePool::Delete(CPacketParse* pPacketParse, bool blDelete)
{
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
