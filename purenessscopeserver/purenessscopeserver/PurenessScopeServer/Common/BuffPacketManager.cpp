#include "BuffPacketManager.h"

CBuffPacketManager::CBuffPacketManager(void)
{
    //默认为主机序
    m_blSortType = false;
}

CBuffPacketManager::~CBuffPacketManager(void)
{
    OUR_DEBUG((LM_ERROR, "[CBuffPacketManager::~CBuffPacketManager].\n"));
    //Close();
}

IBuffPacket* CBuffPacketManager::Create()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    CBuffPacket* pBuffPacket = NULL;

    pBuffPacket = m_objHashBuffPacketList.Pop();
    return dynamic_cast<IBuffPacket*>(pBuffPacket);
}

bool CBuffPacketManager::Delete(IBuffPacket* pBuffPacket)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    CBuffPacket* pBuff = dynamic_cast<CBuffPacket*>(pBuffPacket);

    if(NULL == pBuff)
    {
        return false;
    }

    pBuff->Clear();
    pBuff->SetNetSort(m_blSortType);

    char szPacketID[10] = {'\0'};
    sprintf_safe(szPacketID, 10, "%d", pBuff->GetBuffID());
    bool blState = m_objHashBuffPacketList.Push(szPacketID, pBuff);

    if(false == blState)
    {
        OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Delete]szPacketID=%s(0x%08x).\n", szPacketID, pBuff));
    }
    else
    {
        //OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Delete]szPacketID=%s(0x%08x) nPos=%d.\n", szPacketID, pBuff, nPos));
    }

    return true;
}

void CBuffPacketManager::Close()
{
    //清理所有已存在的指针
    vector<CBuffPacket* > vecBuffPacket;
    m_objHashBuffPacketList.Get_All_Used(vecBuffPacket);

    for(int32 i = 0; i < (int32)vecBuffPacket.size(); i++)
    {
        CBuffPacket* pBuffPacket = vecBuffPacket[i];

        if(NULL != pBuffPacket)
        {
            pBuffPacket->Close();
            SAFE_DELETE(pBuffPacket);
        }
    }

    m_objHashBuffPacketList.Close();
}

void CBuffPacketManager::Init(uint32 u4PacketCount, uint32 u4MaxBuffSize, bool blByteOrder)
{
    Close();

    //初始化Hash表
    m_objHashBuffPacketList.Init((int32)u4PacketCount);

    for(int32 i = 0; i < m_objHashBuffPacketList.Get_Count(); i++)
    {
        CBuffPacket* pBuffPacket = new CBuffPacket(DEFINE_PACKET_SIZE, u4MaxBuffSize);

        if(NULL != pBuffPacket)
        {
            //设置BuffPacket默认字序
            pBuffPacket->SetNetSort(blByteOrder);
            pBuffPacket->SetBuffID(i);

            char szPacketID[10] = {'\0'};
            sprintf_safe(szPacketID, 10, "%d", i);

            //添加到Hash数组里面
            int32 nHashPos = m_objHashBuffPacketList.Add_Hash_Data(szPacketID, pBuffPacket);

            if(-1 != nHashPos)
            {
                pBuffPacket->SetHashID(i);
            }
        }
    }

    //设定当前对象池的字序
    m_blSortType = blByteOrder;

}

uint32 CBuffPacketManager::GetBuffPacketUsedCount()
{
    return (uint32)m_objHashBuffPacketList.Get_Count() - m_objHashBuffPacketList.Get_Used_Count();
}

uint32 CBuffPacketManager::GetBuffPacketFreeCount()
{
    return (uint32)m_objHashBuffPacketList.Get_Used_Count();
}

