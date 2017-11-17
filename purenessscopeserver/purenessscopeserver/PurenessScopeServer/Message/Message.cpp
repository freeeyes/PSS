// BuffPacket.h
// 处理消息的类，用于将消息分发到需要处理的类中
// 学习是必要的，但更重要的是自我的实践。当疲乏来临是，需要的是战胜它。
// add by freeeyes
// 2009-01-10

#include "Message.h"

CMessage::CMessage(void)
{
    m_pMessageBase  = NULL;
    m_pmbHead       = NULL;
    m_pmbBody       = NULL;
    m_szError[0]    = '\0';
    m_nHashID       = 0;

    m_pMessageBase = new _MessageBase();

    //这里设置消息队列模块指针内容，这样就不必反复的new和delete，提升性能
    //指针关系也可以在这里直接指定，不必使用的使用再指定
    m_pmbQueuePtr  = new ACE_Message_Block(sizeof(CMessage*));

    CMessage** ppMessage = (CMessage**)m_pmbQueuePtr->base();
    *ppMessage = this;
}

CMessage::~CMessage(void)
{
    //OUR_DEBUG((LM_INFO, "[CMessage::~CMessage].\n"));
    Close();
    //OUR_DEBUG((LM_INFO, "[CMessage::~CMessage]End.\n"));
}

void CMessage::SetHashID(int nHasnID)
{
    m_nHashID = nHasnID;
}

int CMessage::GetHashID()
{
    return m_nHashID;
}

const char* CMessage::GetError()
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

_MessageBase* CMessage::GetMessageBase()
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

CMessagePool::~CMessagePool()
{
    OUR_DEBUG((LM_INFO, "[CMessagePool::~CMessagePool].\n"));
    Close();
    OUR_DEBUG((LM_INFO, "[CMessagePool::~CMessagePool]End.\n"));
}

void CMessagePool::Init(uint32 u4PacketCount)
{
    Close();

    //初始化HashTable
    m_objHashMessageList.Init((int)u4PacketCount);

    for(int i = 0; i < (int)u4PacketCount; i++)
    {
        CMessage* pPacket = new CMessage();

        if(NULL != pPacket)
        {
            //添加到Hashmap里面
            char szMessageID[10] = {'\0'};
            sprintf_safe(szMessageID, 10, "%d", i);
            int nHashPos = m_objHashMessageList.Add_Hash_Data(szMessageID, pPacket);

            if(-1 != nHashPos)
            {
                pPacket->SetHashID(i);
            }
        }
    }
}

void CMessagePool::Close()
{
    //清理所有已存在的指针
    vector<CMessage*> vecMessage;
    m_objHashMessageList.Get_All_Used(vecMessage);

    for(int i = 0; i < (int)vecMessage.size(); i++)
    {
        CMessage* pMessage = vecMessage[i];
        SAFE_DELETE(pMessage);
    }

    m_objHashMessageList.Close();
}

int CMessagePool::GetUsedCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return m_objHashMessageList.Get_Count() - m_objHashMessageList.Get_Used_Count();
}

int CMessagePool::GetFreeCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return m_objHashMessageList.Get_Used_Count();
}

CMessage* CMessagePool::Create()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    CMessage* pMessage = NULL;

    //在Hash表中弹出一个已使用的数据
    pMessage = m_objHashMessageList.Pop();

    //没找到空余的
    return pMessage;
}

bool CMessagePool::Delete(CMessage* pObject)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if(NULL == pObject)
    {
        return false;
    }

    pObject->Clear();

    char szHashID[10] = {'\0'};
    sprintf_safe(szHashID, 10, "%d", pObject->GetHashID());
    bool blState = m_objHashMessageList.Push(szHashID, pObject);

    if(false == blState)
    {
        OUR_DEBUG((LM_INFO, "[CMessagePool::Delete]HashID=%s(0x%08x).\n", szHashID, pObject));
    }
    else
    {
        //OUR_DEBUG((LM_INFO, "[CSendMessagePool::Delete]HashID=%s(0x%08x) nPos=%d.\n", szHashID, pObject, nPos));
    }

    return true;
}



