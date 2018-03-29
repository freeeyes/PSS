#include "BuffPacketManager.h"

CBuffPacketManager::CBuffPacketManager()
{

}

CBuffPacketManager::~CBuffPacketManager()
{
    OUR_DEBUG((LM_INFO, "[CBuffPacketManager::~CBuffPacketManager]Begin.\n"));
    CObjectPoolManager::Close();
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
    return CObjectPoolManager::GetUsedCount();
}

uint32 CBuffPacketManager::GetBuffPacketFreeCount()
{
    return CObjectPoolManager::GetFreeCount();
}

void CBuffPacketManager::OutputCreateInfo()
{
    CObjectPoolManager::OutputCreateInfo();
}

void CBuffPacketManager::SetCreateFlag(bool blTagCreateInfo)
{
    CObjectPoolManager::SetCreateFlag(blTagCreateInfo);
}

IBuffPacket* CBuffPacketManager::Create(const char* pFileName, uint32 u4Line)
{
    return dynamic_cast<IBuffPacket*>(CObjectPoolManager::Create(pFileName, u4Line));
}

bool CBuffPacketManager::Delete(IBuffPacket* pBuffPacket)
{
    CBuffPacket* pBuff = dynamic_cast<CBuffPacket*>(pBuffPacket);
    pBuff->Clear();
    return CObjectPoolManager::Delete(pBuff->GetBuffID(), pBuff);
}

void CBuffPacketManager::GetCreateInfoList(vector<_Object_Create_Info>& objCreateList)
{
    return CObjectPoolManager::GetCreateInfoList(objCreateList);
}
