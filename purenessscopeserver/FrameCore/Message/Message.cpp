// BuffPacket.h
// ������Ϣ���࣬���ڽ���Ϣ�ַ�����Ҫ���������
// ѧϰ�Ǳ�Ҫ�ģ�������Ҫ�������ҵ�ʵ������ƣ�������ǣ���Ҫ����սʤ����
// add by freeeyes
// 2009-01-10

#include "Message.h"

CMessage::CMessage(void)
{
    //����������Ϣ����ģ��ָ�����ݣ������Ͳ��ط�����new��delete����������
    //ָ���ϵҲ����������ֱ��ָ��������ʹ�õ�ʹ����ָ��
    m_pmbQueuePtr  = new ACE_Message_Block(sizeof(CMessage*));

    CMessage** ppMessage = (CMessage**)m_pmbQueuePtr->base();
    *ppMessage = this;
}

void CMessage::SetHashID(int nHasnID)
{
    m_nHashID = nHasnID;
}

int CMessage::GetHashID() const
{
    return m_nHashID;
}

const char* CMessage::GetError() const
{
    return m_szError;
}

ACE_Message_Block* CMessage::GetQueueMessage()
{
    return m_pmbQueuePtr;
}

void CMessage::SetMessageBase(_MessageBase* pMessageBase)
{
    if(NULL != m_pMessageBase)
    {
        delete m_pMessageBase;
        m_pMessageBase = NULL;
    }

    m_pMessageBase = pMessageBase;
}

ACE_Message_Block* CMessage::GetMessageHead()
{
    return m_pmbHead;
}

ACE_Message_Block* CMessage::GetMessageBody()
{
    return m_pmbBody;
}

_MessageBase* CMessage::GetMessageBase() const
{
    return m_pMessageBase;
}

bool CMessage::GetPacketHead(_PacketInfo& PacketInfo)
{
    if(m_pmbHead != NULL)
    {
        PacketInfo.m_pData    = m_pmbHead->rd_ptr();
        PacketInfo.m_nDataLen = (int)m_pmbHead->length();
        return true;
    }
    else
    {
        return false;
    }
}

bool CMessage::GetPacketBody(_PacketInfo& PacketInfo)
{
    if(m_pmbBody != NULL)
    {
        PacketInfo.m_pData    = m_pmbBody->rd_ptr();
        PacketInfo.m_nDataLen = (int)m_pmbBody->length();
        return true;
    }
    else
    {
        return false;
    }
}

bool CMessage::SetPacketHead(ACE_Message_Block* pmbHead)
{
    m_pmbHead = pmbHead;
    return true;
}

bool CMessage::SetPacketBody(ACE_Message_Block* pmbBody)
{
    m_pmbBody = pmbBody;
    return true;
}

void CMessage::Close()
{
    if(NULL != m_pmbHead)
    {
        App_MessageBlockManager::instance()->Close(m_pmbHead);
        m_pmbHead = NULL;
    }

    if(NULL != m_pmbBody)
    {
        App_MessageBlockManager::instance()->Close(m_pmbBody);
        m_pmbBody = NULL;
    }

    if(NULL != m_pmbQueuePtr)
    {
        m_pmbQueuePtr->release();
        m_pmbQueuePtr = NULL;
    }

    SAFE_DELETE(m_pMessageBase);
}

void CMessage::Clear()
{
    if(NULL != m_pmbHead)
    {
        App_MessageBlockManager::instance()->Close(m_pmbHead);
        m_pmbHead = NULL;
    }

    if(NULL != m_pmbBody)
    {
        App_MessageBlockManager::instance()->Close(m_pmbBody);
        m_pmbBody = NULL;
    }

    if (NULL != m_pMessageBase)
    {
        m_pMessageBase->Clear();
    }
}

CMessagePool::CMessagePool()
{
}

void CMessagePool::Init_Callback(int nIndex, CMessage* pMessage)
{
    pMessage->SetHashID(nIndex);
}

void CMessagePool::Close_Callback(int nIndex, CMessage* pMessage)
{
    ACE_UNUSED_ARG(nIndex);

    if (NULL != pMessage)
    {
        pMessage->Close();
    }
}

int CMessagePool::GetUsedCount()
{
    return CObjectPoolManager<CMessage, ACE_Recursive_Thread_Mutex>::GetUsedCount();
}

int CMessagePool::GetFreeCount()
{
    return CObjectPoolManager<CMessage, ACE_Recursive_Thread_Mutex>::GetFreeCount();
}

void CMessagePool::GetCreateInfoList(vector<_Object_Create_Info>& objCreateList)
{
    return CObjectPoolManager<CMessage, ACE_Recursive_Thread_Mutex>::GetCreateInfoList(objCreateList);
}

CMessage* CMessagePool::Create()
{
    return CObjectPoolManager<CMessage, ACE_Recursive_Thread_Mutex>::Create(__FILE__, __LINE__);
}

bool CMessagePool::Delete(CMessage* pMessage)
{
    if(NULL == pMessage)
    {
        return false;
    }

    pMessage->Clear();

    bool blState = CObjectPoolManager<CMessage, ACE_Recursive_Thread_Mutex>::Delete_withoutLock(pMessage->GetHashID(), pMessage);

    if(false == blState)
    {
        OUR_DEBUG((LM_INFO, "[CMessagePool::Delete]HashID=%d(0x%08x).\n", pMessage->GetHashID(), pMessage));
    }

    return true;
}



