#include "SendMessage.h"

void CSendMessagePool::Init(int32 nObjcetCount)
{
    Close();

    //��ʼ��HashTable
    m_objSendMessageList.Init((uint32)nObjcetCount);
    m_objHashHandleList.Init(nObjcetCount);

    for(int32 i = 0; i < nObjcetCount; i++)
    {
        _SendMessage* pMessage = m_objSendMessageList.GetObject(i);

        if(nullptr != pMessage)
        {
            //��ӵ�hash��������
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
    //���������Ѵ��ڵ�ָ��
    m_objHashHandleList.Close();

    OUR_DEBUG((LM_INFO, "[CSendMessagePool::Close]End.\n"));
}

_SendMessage* CSendMessagePool::Create()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    //��Hash���е���һ����ʹ�õ�����
    _SendMessage*  pMessage = m_objHashHandleList.Pop();

    //û�ҵ������
    return pMessage;
}

bool CSendMessagePool::Delete(_SendMessage* pObject)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if(nullptr == pObject)
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


