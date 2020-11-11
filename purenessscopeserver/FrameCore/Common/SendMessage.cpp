#include "SendMessage.h"

CSendMessagePool::CSendMessagePool(void)
{
}

void CSendMessagePool::Init(int32 nObjcetCount)
{
    Close();

    //初始化HashTable
    m_objSendMessageList.Init((uint32)nObjcetCount);
    m_objHashHandleList.Init(nObjcetCount);

    for(int32 i = 0; i < nObjcetCount; i++)
    {
        _SendMessage* pMessage = m_objSendMessageList.GetObject(i);

        if(NULL != pMessage)
        {
            //添加到hash数组里面
            std::stringstream ss_format;
            ss_format << i;
            string strHashID = ss_format.str();

            int32 nHashPos = m_objHashHandleList.Add_Hash_Data(strHashID.c_str(), pMessage);

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

    std::stringstream ss_format;
    ss_format << pObject->GetHashID();
    string strHashID = ss_format.str();

    pObject->Clear();
    bool blState = m_objHashHandleList.Push(strHashID.c_str(), pObject);

    if(false == blState)
    {
        OUR_DEBUG((LM_INFO, "[CSendMessagePool::Delete]HashID=%s(0x%08x).\n", strHashID.c_str(), pObject));
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


