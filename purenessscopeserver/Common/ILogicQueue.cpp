#include "ILogicQueue.h"

ILogicQueue::ILogicQueue(uint32 u4LogicThreadID, uint32 u4Timeout, std::string& strDesc)
    : m_u4LogicThreadID(u4LogicThreadID),
      m_u4Timeout(u4Timeout),
      m_strDesc(strDesc)
{
}

ILogicQueue::~ILogicQueue()
{
}

uint32 ILogicQueue::GetLogicThreadID()
{
    return m_u4LogicThreadID;
}

void ILogicQueue::SetTimeOut(uint32 u4Timeout)
{
    m_u4Timeout = u4Timeout;
}
uint32 ILogicQueue::GetTimeOut()
{
    return m_u4Timeout;
}

std::string ILogicQueue::GetDescriptor()
{
    return m_strDesc;
}