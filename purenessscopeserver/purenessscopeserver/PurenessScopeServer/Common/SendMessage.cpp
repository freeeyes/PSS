#include "SendMessage.h"

CSendMessagePool::CSendMessagePool(void)
{
}

CSendMessagePool::~CSendMessagePool(void)
{
    OUR_DEBUG((LM_INFO, "[CSendMessagePool::~CSendMessagePool].\n"));
    Close();
    OUR_DEBUG((LM_INFO, "[CSendMessagePool::~CSendMessagePool] End.\n"));
}

void CSendMessagePool::Init(int32 nObjcetCount)
{
    Close();

    //初始化HashTable
    m_objHashHandleList.Init((int32)nObjcetCount);

    for(int32 i = 0; i < nObjcetCount; i++)
    {
        _SendMessage* pMessage = new _SendMessage();

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
    //清理所有已存在的指针
    vector<_SendMessage*> vecSendMessage;
    m_objHashHandleList.Get_All_Used(vecSendMessage);

    for(int32 i = 0; i < (int32)vecSendMessage.size(); i++)
    {
        _SendMessage* pMessage = vecSendMessage[i];
        SAFE_DELETE(pMessage);
    }

    m_objHashHandleList.Close();
}

_SendMessage* CSendMessagePool::Create()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    _SendMessage* pMessage = NULL;

    //在Hash表中弹出一个已使用的数据
    pMessage = m_objHashHandleList.Pop();

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
    else
    {
        //OUR_DEBUG((LM_INFO, "[CSendMessagePool::Delete]HashID=%s(0x%08x) nPos=%d.\n", szHashID, pObject, nPos));
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


