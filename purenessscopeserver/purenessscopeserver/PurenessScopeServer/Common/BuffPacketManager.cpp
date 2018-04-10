#include "BuffPacketManager.h"

CBuffPacketManager::CBuffPacketManager()
{

}

CBuffPacketManager::~CBuffPacketManager()
{
    OUR_DEBUG((LM_INFO, "[CBuffPacketManager::~CBuffPacketManager]Begin.\n"));
    CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>::Close();
}

void CBuffPacketManager::Init_Callback(int nIndex, CBuffPacket* pBuffPacket)
{
    //ÉèÖÃBuffPacketÄ¬ÈÏ×ÖÐò
    pBuffPacket->Init(DEFINE_PACKET_SIZE, App_MainConfig::instance()->GetMsgMaxBuffSize());
    pBuffPacket->SetNetSort(App_MainConfig::instance()->GetByteOrder());
    pBuffPacket->SetBuffID(nIndex);
    pBuffPacket->SetHashID(nIndex);
}

uint32 CBuffPacketManager::GetBuffPacketUsedCount()
{
    return CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>::GetUsedCount();
}

uint32 CBuffPacketManager::GetBuffPacketFreeCount()
{
    return CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>::GetFreeCount();
}

void CBuffPacketManager::OutputCreateInfo()
{
    CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>::OutputCreateInfo();
}

void CBuffPacketManager::SetCreateFlag(bool blTagCreateInfo)
{
    CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>::SetCreateFlag(blTagCreateInfo);
}

IBuffPacket* CBuffPacketManager::Create(const char* pFileName, uint32 u4Line)
{
    return dynamic_cast<IBuffPacket*>(CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>::Create(pFileName, u4Line));
}

bool CBuffPacketManager::Delete(IBuffPacket* pBuffPacket)
{
    CBuffPacket* pBuff = dynamic_cast<CBuffPacket*>(pBuffPacket);
    pBuff->Clear();
    return CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>::Delete(pBuff->GetBuffID(), pBuff);
}


uint32 CBuffPacketManager::GetCreateInfoCount()
{
    m_objCreateList.clear();

    CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>::GetCreateInfoList(m_objCreateList);

    return (uint32)m_objCreateList.size();
}

bool CBuffPacketManager::GetCreateInfoList(uint32 u4Index, _Object_Create_Info& objCreateInfo)
{
    if (u4Index >= (uint32)m_objCreateList.size())
    {
        return false;
    }

    objCreateInfo = m_objCreateList[u4Index];
    return true;
}
