#include "BuffPacketManager.h"

CBuffPacketManager::CBuffPacketManager()
{

}

void CBuffPacketManager::Init_Callback(int nIndex, CBuffPacket* pBuffPacket)
{
    //ÉèÖÃBuffPacketÄ¬ÈÏ×ÖÐò
    pBuffPacket->Init(DEFINE_PACKET_SIZE, GetXmlConfigAttribute(xmlMessage)->Msg_Buff_Max_Size);
    pBuffPacket->SetNetSort(GetXmlConfigAttribute(xmlNetWorkMode)->NetByteOrder);
    pBuffPacket->SetBuffID(nIndex);
    pBuffPacket->SetHashID(nIndex);
}

void CBuffPacketManager::Close_Callback(int nIndex, CBuffPacket* pBuffPacket)
{
    if (false == pBuffPacket->Close())
    {
        OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Close_Callback]Close error[%d](%s).\n",
                   nIndex,
                   pBuffPacket->GetError()));
    }
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
    if (NULL != pBuffPacket)
    {
        auto pBuff = dynamic_cast<CBuffPacket*>(pBuffPacket);
        pBuff->Clear();
        return CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>::Delete(pBuff->GetBuffID(), pBuff);
    }
    else
    {
        return false;
    }
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
