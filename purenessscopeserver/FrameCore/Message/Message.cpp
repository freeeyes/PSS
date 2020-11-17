// BuffPacket.h
// 处理消息的类，用于将消息分发到需要处理的类中
// 学习是必要的，但更重要的是自我的实践。当疲乏来临是，需要的是战胜它。
// add by freeeyes
// 2009-01-10

#include "Message.h"

CMessage::CMessage(void)
{
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
    return &m_MessageBase;
}

bool CMessage::GetPacketHead(_PacketInfo& PacketInfo)
{
    if(nullptr != m_pmbHead)
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
    if(nullptr != m_pmbBody)
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

CWorkThreadMessage::CWorkThreadMessage()
{
	//这里设置消息队列模块指针内容，这样就不必反复的new和delete，提升性能
    //指针关系也可以在这里直接指定，不必使用的使用再指定
	m_pmbQueuePtr = new ACE_Message_Block(sizeof(CMessage*));

    CWorkThreadMessage** ppMessage = (CWorkThreadMessage**)m_pmbQueuePtr->base();
	*ppMessage = this;
}

ACE_Message_Block* CWorkThreadMessage::GetQueueMessage()
{
    return m_pmbQueuePtr;
}

void CWorkThreadMessage::SetHashID(int nHashID)
{
    m_nHashID = nHashID;
}

int CWorkThreadMessage::GetHashID()
{
    return m_nHashID;
}

void CWorkThreadMessage::Clear()
{
	if (nullptr != m_pmbRecvHead)
	{
        App_MessageBlockManager::instance()->Close(m_pmbRecvHead);
		m_pmbRecvHead = nullptr;
	}

	if (nullptr != m_pmbRecvBody)
	{
        App_MessageBlockManager::instance()->Close(m_pmbRecvBody);
		m_pmbRecvBody = nullptr;
	}
}

void CWorkThreadMessage::Close()
{
	if (nullptr != m_pmbRecvHead)
	{
        App_MessageBlockManager::instance()->Close(m_pmbRecvHead);
		m_pmbRecvHead = nullptr;
	}

	if (nullptr != m_pmbRecvBody)
	{
        App_MessageBlockManager::instance()->Close(m_pmbRecvBody);
		m_pmbRecvBody = nullptr;
	}

	if (nullptr != m_pmbQueuePtr)
	{
		m_pmbQueuePtr->release();
		m_pmbQueuePtr = nullptr;
	}
}

CDeviceHandlerPool::CDeviceHandlerPool()
{
}

void CDeviceHandlerPool::Init_Callback(int nIndex, CWorkThread_Handler_info* pHandler)
{
    pHandler->SetHashID(nIndex);
}

void CDeviceHandlerPool::Close_Callback(int nIndex, CWorkThread_Handler_info* pHandler)
{
    ACE_UNUSED_ARG(nIndex);
    pHandler->Clear();
}

CWorkThread_Handler_info* CDeviceHandlerPool::Create()
{
    return CObjectPoolManager<CWorkThread_Handler_info, ACE_Recursive_Thread_Mutex>::Create(__FILE__, __LINE__);
}

bool CDeviceHandlerPool::Delete(CWorkThread_Handler_info* pHandler)
{
	if (nullptr == pHandler)
	{
		return false;
	}

	bool blState = CObjectPoolManager<CWorkThread_Handler_info, ACE_Recursive_Thread_Mutex>::Delete_withoutLock(pHandler->GetHashID(), pHandler);

	if (false == blState)
	{
		OUR_DEBUG((LM_INFO, "[CDeviceHandlerPool::Delete]HashID=%d(0x%08x).\n", pHandler->GetHashID(), pHandler));
	}

	return true;
}

int CDeviceHandlerPool::GetUsedCount()
{
    return CObjectPoolManager<CWorkThread_Handler_info, ACE_Recursive_Thread_Mutex>::GetUsedCount();
}

int CDeviceHandlerPool::GetFreeCount()
{
    return CObjectPoolManager<CWorkThread_Handler_info, ACE_Recursive_Thread_Mutex>::GetFreeCount();
}

void CDeviceHandlerPool::GetCreateInfoList(vector<_Object_Create_Info>& objCreateList)
{
    return CObjectPoolManager<CWorkThread_Handler_info, ACE_Recursive_Thread_Mutex>::GetCreateInfoList(objCreateList);
}

