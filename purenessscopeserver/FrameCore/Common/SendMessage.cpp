#include "SendMessage.h"

CSendMessagePool::CSendMessagePool(void)
{
}

void CSendMessagePool::Init(int32 nObjcetCount)
{
    Close();

    //初始化HashTable
    m_objSendMessageList.Init((uint32)nObjcetCount);
    m_objHashHandleList.Init((int32)nObjcetCount);

    for(int32 i = 0; i < nObjcetCount; i++)
    {
        _SendMessage* pMessage = m_objSendMessageList.GetObject(i);

        if(NULL != pMessage)
        {
            //添加到hash数组里面
            char szMessageID[10] = {'\0'};
            sprintf_safe(szMessageID, 10, "%d", i);
            int32 nHashPos = m_objHashHandleList.Add_Hash_Data(szMessageID, pMessage);

            if(-1 != nHashPos)
            {
                pMessage->SetHashID(i);
            }
        }
    }
}

void CSendMessagePool::Close()
{
    OUR_DEBUG((LM_INFO, "[CSendMessagePool::Close]Begin.\n"));
    //清理所有已存在的指针
    m_objHashHandleList.Close();

    //回收所有指针
    for (uint32 i = 0; i < m_objSendMessageList.GetCount(); i++)
    {
        m_objSendMessageList.GetObject(i)->Close();
    }

    OUR_DEBUG((LM_INFO, "[CSendMessagePool::Close]End.\n"));
}

_SendMessage* CSendMessagePool::Create()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    //在Hash表中弹出一个已使用的数据
    _SendMessage*  pMessage = m_objHashHandleList.Pop();

    //没找到空余的
    return pMessage;
}

bool CSendMessagePool::Delete(_SendMessage* pObject)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if(NULL == pObject)
    {
        return false;
    }

    char szHashID[10] = {'\0'};
    sprintf_safe(szHashID, 10, "%d", pObject->GetHashID());
    pObject->Clear();
    bool blState = m_objHashHandleList.Push(szHashID, pObject);

    if(false == blState)
    {
        OUR_DEBUG((LM_INFO, "[CSendMessagePool::Delete]HashID=%s(0x%08x).\n", szHashID, pObject));
    }

    return true;
}

int32 CSendMessagePool::GetUsedCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return m_objHashHandleList.Get_Count() - m_objHashHandleList.Get_Used_Count();
}

int32 CSendMessagePool::GetFreeCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return m_objHashHandleList.Get_Used_Count();
}


