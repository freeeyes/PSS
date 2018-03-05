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

IBuffPacket* CBuffPacketManager::Create(const char* pFileName, uint32 u4Line)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    IBuffPacket* pBuffPacket = dynamic_cast<IBuffPacket*>(m_objHashBuffPacketList.Pop());

    if (NULL != pBuffPacket)
    {
        pBuffPacket->SetCreateInfo(pFileName, u4Line);
    }

    return pBuffPacket;
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

void CBuffPacketManager::GetCreateInfoList(vector<_Packet_Create_Info>& objCreateList)
{
    objCreateList.clear();

    //输出所有正在使用的对象创建信息
    uint32 u4Count = m_objBuffPacketList.GetCount();

    for (uint32 i = 0; i < u4Count; i++)
    {
        CBuffPacket* pBuffPacket = m_objBuffPacketList.GetObject(i);

        int nCreateLine       = pBuffPacket->GetCreateLine();
        char* pCreateFileName = pBuffPacket->GetCreateFileName();

        if (strlen(pCreateFileName) > 0 && nCreateLine > 0)
        {
            bool blIsFind = false;

            //正在使用的对象，进行统计
            for (int j = 0; j < (int)objCreateList.size(); j++)
            {
                if (0 == ACE_OS::strcmp(pCreateFileName, objCreateList[j].m_szCreateFileName)
                    && nCreateLine == objCreateList[j].m_u4Line)
                {
                    blIsFind = true;
                    objCreateList[j].m_u4Count++;
                    break;
                }
            }

            if (false == blIsFind)
            {
                _Packet_Create_Info obj_Packet_Create_Info;
                sprintf_safe(obj_Packet_Create_Info.m_szCreateFileName, MAX_BUFF_100, "%s", pCreateFileName);
                obj_Packet_Create_Info.m_u4Line  = nCreateLine;
                obj_Packet_Create_Info.m_u4Count = 1;
                objCreateList.push_back(obj_Packet_Create_Info);
            }
        }
    }
}

void CBuffPacketManager::Close()
{
    //清理所有已存在的指针
    m_objHashBuffPacketList.Close();
}

void CBuffPacketManager::Init(uint32 u4PacketCount, uint32 u4MaxBuffSize, bool blByteOrder)
{
    Close();

    //初始化Hash表
    m_objBuffPacketList.Init(u4PacketCount);
    m_objHashBuffPacketList.Init((int32)u4PacketCount);

    uint32 u4Size = m_objHashBuffPacketList.Get_Count();

    for(uint32 i = 0; i < u4Size; i++)
    {
        CBuffPacket* pBuffPacket = m_objBuffPacketList.GetObject(i);

        if(NULL != pBuffPacket)
        {
            //设置BuffPacket默认字序
            pBuffPacket->Init(DEFINE_PACKET_SIZE, u4MaxBuffSize);
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

